/* (c)Copyright 1996-2000 NTT Cyber Space Laboratories */
/*                Modified on 2000.09.06 by N. Iwakami */

// Chunk1.cpp: CChunk クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdio.h>
#include "Chunk.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CChunk::CChunk( std::string ID )
{
	m_id = ID;
	m_iter = m_data.begin();
}

CChunk::~CChunk()
{
	//std::cout << "ID: " << m_id << " --- destrctred" << std::endl;

}

int CChunk::PutData( int size, char inputData[] )
{
	for ( int ii=0; ii<size; ii++ ) {
		m_data.push_back( inputData[ii] );
	}

	return 0;
}

int CChunk::PutData( CChunkData& inputData )
{
	for ( unsigned int ii=0; ii<inputData.size(); ii++ ) {
		m_data.push_back( inputData[ii] );
	}
	return 0;
}

int CChunk::PutData( std::string& theString )
{
	std::string::iterator it;

	for ( it=theString.begin(); it!=theString.end(); it++ ) {
		m_data.push_back( *it );
	}

	return 0;
}

std::string CChunk::GetRndString( int length )
{
	if ( length == 0 ) {
		length = GetSize();
	}

	if ( m_iter + length > m_data.end() ) {
		return "";
	}

	std::string theString;
	for ( int ii=0; ii<length; ii++ ) {
		theString += *m_iter++;
	}

	return theString;
}

std::vector<char> CChunk::GetVector( int size )
{
	CChunkData retval;

	if ( size == 0 ) {
		size = m_data.end() - m_iter;
	}

	if ( m_iter + size > m_data.end() ) {
		return retval;
	}

	for ( int ii=0; ii<size; ii++ ) {
		retval.push_back( *m_iter++ );
	}
	return retval;
}

int CChunk::GetNInt( int size )
{
	int ibyte;
	unsigned long retval;

	retval = 0;
	for ( ibyte=0; ibyte<size; ibyte++ ) {
		retval <<= 8;
		retval |= (unsigned char) *m_iter++;
	}

	return retval;

	return 0;
}

void CChunk::PutNInt(unsigned int inputData, int size)
{
	const int mask = 0xFF;
	int ibyte;
	char data_tmp;

	// データ書き込み
	for ( ibyte=0; ibyte<size; ibyte++ ) {
		data_tmp = (char)( ( inputData >> ( 8 * (size-ibyte-1) ) ) & mask );
		m_data.push_back( data_tmp );
	}
}

void CChunk::cnk_rewind()
{
	m_iter = m_data.begin();
}

void CChunk::cnk_delete()
{
	int ii, size;
	size = m_data.size();
	for ( ii=0; ii<size; ii++ ) {
		m_data.pop_back();
	}
	cnk_rewind();
}

//////////////////////////////////////////////////////////////////////
// CStringChunk クラス
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


CStringChunk::~CStringChunk()
{

}


//////////////////////////////////////////////////////////////////////
// CChunkChunk クラス
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

/*
CChunkChunk::CChunkChunk()
{

}
*/

CChunkChunk::~CChunkChunk()
{

}

CChunk* CChunkChunk::GetNextChunk(int idSize)
{
	std::string id;
	if ( (id = this->GetRndString(idSize)) != "" ){ // チャンク ID を取得
		CChunk* subChunk = new CChunk( id );		// サブチャンクを作成

		int size = this->GetNInt(sizeof(unsigned long));	// 入力チャンクのサイズを取得
		if ( size > 0 ){
			CChunk::CChunkData theData;
			theData = this->GetVector( size );			// サイズ分のデータを読む
			subChunk->PutData( theData );						// データをサブチャンクに書き込む
		}
		return subChunk;
	}

	return NULL;
}

/*============================================================================*/
/* Name:        CChunkChunk::AppendChunk                                      */
/* Description: チャンクデータにサブチャンクを加える                          */
/* Return:      なし                                                          */
/* Access:      public                                                        */
/*============================================================================*/
void CChunkChunk::PutChunk(CChunk& src)
{
	// 本体チャンクにサブチャンクのIDを書き込む
	std::string id = src.GetID();
	PutData( id );
	// 本体チャンクにサブチャンクのサイズを書き込む
	PutNInt( src.GetSize(), sizeof(long) );
	// 本体チャンクにサブチャンクのデータを書き込む
	CChunk::CChunkData data = src.GetData();
	PutData( data );
}



//////////////////////////////////////////////////////////////////////
// CCommChunk クラス
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CCommChunk::CCommChunk( const CChunk& parent, string version ) : CChunk( parent )
{
	m_version = version;

	// データを読み込む
	this->cnk_rewind();
	m_channelMode   = this->GetNInt();
	m_bitRate       = this->GetNInt();
	m_samplingRate  = this->GetNInt();
	m_securityLevel = this->GetNInt();
}

CCommChunk::CCommChunk(CommData channelMode,
					   CommData bitRate,
					   CommData samplingRate,
					   CommData securityLevel,
					   string version) : CChunk( "COMM" )
{
	// バージョンをセット
	m_version = version;

	// データを初期化
	m_channelMode   = channelMode;
	m_bitRate       = bitRate;
	m_samplingRate  = samplingRate;
	m_securityLevel = securityLevel;

	// データをチャンクにコピー
	this->cnk_rewind();
	this->PutNInt( m_channelMode );
	this->PutNInt( m_bitRate );
	this->PutNInt( m_samplingRate );
	this->PutNInt( m_securityLevel );

}

CCommChunk::~CCommChunk()
{

}


//////////////////////////////////////////////////////////////////////
// CEncdChunk クラス
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CEncdChunk::CEncdChunk(std::string id,
					   short year,
					   char month,
					   char day,
					   char hour,
					   char minute,
					   char timeZone ) : CChunk( id )
{
	m_year     = year;
	m_month    = month;
	m_day      = day;
	m_hour     = hour;
	m_minute   = minute;
	m_timeZone = timeZone;

	PutNInt ( m_year, sizeof(m_year) );
	PutNInt ( m_month, sizeof(m_month) );
	PutNInt ( m_day, sizeof(m_day) );
	PutNInt ( m_hour, sizeof(m_hour) );
	PutNInt ( m_minute, sizeof(m_minute) );
	PutNInt ( m_timeZone, sizeof(m_timeZone) );
}

CEncdChunk::CEncdChunk( const CChunk& parent ) : CChunk( parent ) {
	cnk_rewind();
	m_year     = GetNInt( sizeof(m_year) );
	m_month    = GetNInt( sizeof(m_month) );
	m_day      = GetNInt( sizeof(m_day) );
	m_hour     = GetNInt( sizeof(m_hour) );
	m_minute   = GetNInt( sizeof(m_minute) );
	m_timeZone = GetNInt( sizeof(m_timeZone) );
}

CEncdChunk::~CEncdChunk()
{

}

