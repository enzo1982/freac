/* (c)Copyright 1996-2000 NTT Cyber Space Laboratories */
/*                Modified on 2000.09.06 by N. Iwakami */

// Chunk1.h: CChunk クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHUNK1_H__E059C7BF_6C3B_11D4_A71A_00C04F012175__INCLUDED_)
#define AFX_CHUNK1_H__E059C7BF_6C3B_11D4_A71A_00C04F012175__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>

//////////////////////////////////////
//////////////////////////////////////
// CChunk クラスのインタフェース
// CChunk クラスはチャンクの基本構造を提供する。生のデータを入出力する
class CChunk
{
public:
	typedef std::vector<char> CChunkData;

private:
	std::string m_id;
	CChunkData  m_data;
	CChunkData::iterator m_iter;

protected:
	int   GetNInt( int size=sizeof(long) );			// 現在位置から size バイト整数を取得（デフォルト：long整数）
	CChunkData GetVector( int size = 0 );			// 現在位置から size バイト分のベクトルを取得（デフォルト：残り全部）
	std::string GetRndString( int length = 0 );		// 現在位置から length だけの文字列を取得（デフォルト：残り全部）

	void PutNInt( unsigned int inputData, int size=sizeof(long) );	// 現在位置から size バイト整数を書き込み（デフォルト：long整数）

	void  cnk_rewind();								// 現在位置を先頭に戻す
	void cnk_delete();								// データを消去する
	const int   GetCurrentPosition()				// 現在位置を取得する
	{
		return m_iter - m_data.begin();
	};

	// 例外
	class err_FailPut { };

public:
	CChunk( std::string ID );	// コンストラクタ
	virtual ~CChunk();			// デストラクタ

	// 取得系メンバ関数
	const std::string& GetID() { return m_id; };		// ID の取得
	const int GetSize() { return m_data.size(); };	// チャンクサイズの取得
	const CChunkData& GetData() { return m_data; };

	// 書き込み系メンバ関数
	int PutData( int size, char inputData[] );		// データを追加、キャラクタ配列型
	int PutData( CChunkData& inputData );			// データを追加、ベクトル型
	int PutData( std::string& theString );			// データを追加、文字列型

	virtual std::string whatami() { return "Raw"; };

};


// CChunkChunk クラスのインターフェイス
// 文字列のみを格納するチャンク、汎用チャンク型
//
//////////////////////////////////////////////////////////////////////
class CStringChunk : public CChunk  
{
public:
	std::string GetString() { cnk_rewind(); return GetRndString(); };
	CStringChunk( const CChunk& parent ) : CChunk( parent ) { };
	CStringChunk( std::string ID, std::string data="" ) : CChunk( ID ) { if ( data != "" ) PutData( data ); };
	virtual ~CStringChunk();

	std::string whatami() { return "String"; };
};


// CIntChunk テンプレート
// 整数を１つだけ格納するチャンク型のテンプレート、汎用チャンク型
//
//////////////////////////////////////////////////////////////////////
template <class TINT>
class CIntChunk : public CChunk  
{
	int m_dataSize;
public:
	const TINT GetInt() { cnk_rewind(); return GetNInt(m_dataSize); };	// 整数データを取得する
	int PutInt( TINT data ) { cnk_delete(); PutNInt( data, m_dataSize ); };

	CIntChunk( const CChunk& parent ) : CChunk(parent) { m_dataSize = sizeof(TINT); };
	CIntChunk( std::string id, TINT data ) : CChunk(id) { 
		m_dataSize = sizeof(TINT);
		PutNInt( data, m_dataSize );
	}
	virtual ~CIntChunk() { };

	std::string whatami() { char retstr[20]; sprintf( retstr, "Integer, size=%d", m_dataSize ); return retstr; };
};


// CChunkChunk クラスのインターフェイス
// チャンクを格納するチャンク、汎用チャンク型
//
//////////////////////////////////////////////////////////////////////
class CChunkChunk : public CChunk  
{
public:
	CChunk* GetNextChunk( int idSize = 4 );	// 次のチャンクを返す
	void rewind() { cnk_rewind(); };		// 巻き戻し
	void PutChunk( CChunk& src );

	CChunkChunk( const CChunk& parent ) : CChunk( parent ) { };	// コンストラクタ・CChunk オブジェクトを種にする
	CChunkChunk( std::string ID ) : CChunk( ID ) { };		// コンストラクタ・ID だけ与え空のチャンクを作成する
	virtual ~CChunkChunk();									// デストラクタ

	// 例外
	class err_FailGetChunk { };		// チャンク取得に失敗
	class err_FailPutChunk { };		// チャンク書き込みに失敗

	std::string whatami() { return "Chunk"; };
};

// CCommChunk クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////
class CCommChunk : public CChunk  
{
public:
	typedef std::string string;
	typedef unsigned long CommData;

private:
	string m_version;
	CommData m_channelMode;
	CommData m_bitRate;
	CommData m_samplingRate;
	CommData m_securityLevel;

public:
	CCommChunk( const CChunk& parent, string version="TWIN97012000" );
	CCommChunk( CommData channelMode, CommData bitRate, CommData samplingRate, CommData securityLevel, string version="TWIN97012000" );
	virtual ~CCommChunk();

	CommData GetChannelMode()  { return m_channelMode;  };		// チャネルモードを取得
	CommData GetBitRate()      { return m_bitRate;      };		// ビットレートを取得
	CommData GetSamplingRate() { return m_samplingRate; };		// サンプリング周波数を取得
	CommData GetSecurityLevel()  { return m_securityLevel;  };	// 追加情報を取得

	// 例外
	class err_FailConstruction { };							// エラー：コンストラクションに失敗

	std::string whatami() { return "COMM"; };
};


// CYearChunk クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////
class CYearChunk : public CChunk  
{
	short m_year;
	char  m_month;

public:
	const short GetYear() { return m_year; };
	const char GetMonth() { return m_month; };

	CYearChunk( std::string id, short year, char month ) : CChunk( id ) {
		m_year = year;
		m_month = month;
		PutNInt ( m_year, sizeof(short) );
		PutNInt ( m_month, sizeof(char) );
	};
	CYearChunk( const CChunk& parent ) : CChunk( parent ) {
		cnk_rewind();
		m_year = GetNInt( sizeof(short) );
		m_month = GetNInt( sizeof(char) );
	};
	virtual ~CYearChunk() { };

	std::string whatami() { return "YEAR"; };
};

// CEncdChunk クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////
class CEncdChunk : public CChunk  
{
	short m_year;
	char  m_month;
	char  m_day;
	char  m_hour;
	char  m_minute;
	char  m_timeZone;

public:
	const short GetYear()     { return m_year; };
	const char  GetMonth()    { return m_month; };
	const char  GetDay()      { return m_day; };
	const char  GetHour()     { return m_hour; };
	const char  GetMinute()   { return m_minute; };
	const char  GetTimeZone() { return m_timeZone; };

	CEncdChunk( std::string id, short year, char month, char day, char hour, char minute, char timeZone );
	CEncdChunk( const CChunk& parent );
	virtual ~CEncdChunk();

	std::string whatami() { return "ENCD"; };
};



/////////////////////////////////////////////////////////
// 汎用チャンク型を利用するサブチャンク型の宣言
/////////////////////////////////////////////////////////
// TWIN チャンク
typedef CChunkChunk			CTwinChunk;	// TWIN

// 標準チャンク
										// COMM は汎用チャンクではない
typedef CStringChunk		CNameChunk;	// NAME
typedef CStringChunk		CComtChunk;	// COMT
typedef CStringChunk		CAuthChunk;	// AUTH
typedef CStringChunk		CCpyrChunk;	// (c)
typedef CStringChunk		CFileChunk;	// FILE
typedef CIntChunk<unsigned long>	CDsizChunk;	// DSIZ
typedef CChunk				CExtrChunk;	// EXTR

// 拡張チャンク・通常
typedef CStringChunk		CAlbmChunk;	// ALBM
										// YEAR は汎用チャンクではない
										// ENCD は汎用チャンクではない
typedef CIntChunk<short>	CTracChunk;	// TRAC
typedef CStringChunk		CLyrcChunk;	// LYRC
typedef CChunk				CGuidChunk;	// GUID
typedef	CStringChunk		CIsrcChunk;	// ISRC
typedef	CStringChunk		CWordChunk;	// WORD
typedef CStringChunk		CMuscChunk;	// MUSC
typedef CStringChunk		CArngChunk;	// ARNG
typedef CStringChunk		CProdChunk;	// PROD
typedef CStringChunk		CRemxChunk;	// REMX
typedef CStringChunk		CCdctChunk;	// CDCT
typedef CStringChunk		CSingChunk;	// SING
typedef CStringChunk		CBandChunk;	// BAND
typedef CStringChunk		CPrsnChunk;	// PRSN
typedef CStringChunk		CLablChunk;	// LABL
typedef CStringChunk		CNoteChunk;	// NOTE

// 拡張チャンク・補助
typedef CChunkChunk			CScndChunk;	// SCND

// 予約チャンク
typedef CChunk				C_Id3Chunk;	// _ID3
typedef CChunk				C_YmhChunk;	// _YMH
typedef CChunk				C_NttChunk;	// _NTT


#endif // !defined(AFX_CHUNK1_H__E059C7BF_6C3B_11D4_A71A_00C04F012175__INCLUDED_)
