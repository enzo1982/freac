/*
** Copyright (C) 1999 - 2002 Albert L. Faber
**  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include "CDRip.h"

using namespace std;

#include "CDExtract.h"
#include <math.h>

static BOOL g_bParanoiaJitterErrors = FALSE;

CDSTATUSINFO g_CDStatusInfo;

// CONSTRUCTOR
CCDExtract::CCDExtract() : CAspiCD()
{
	m_nPeakValue			= 0;
	m_pbtReadBuffer			= NULL;
	m_pbtOverlapBuffer		= NULL;
	m_pbtReadBufferCompare		= NULL;
	m_nBeginSector			= 0;
	m_nEndSector			= 0;
	m_nJitterPos			= 50;
	m_nJitterErrors			= 0;
	m_lSector			= 0;
	m_dwBytesToDo			= 0;

	m_pParanoia			= NULL;
	m_pParanoiaDrive		= NULL;

	m_nCurrentSpeed			= 0;
	m_nLastSpeedAdjust		= -1;

	m_pbAbort			= NULL;

	m_dwReadSectors			= 0;
	m_bC2ErrorDetectionEnabled	= FALSE;

	m_dwReadBufferSize		= 0;
}


// DESRUCTOR
CCDExtract::~CCDExtract()
{
	// de-allocate memory buffers
	delete [] m_pbtReadBuffer; m_pbtReadBuffer=NULL;

	// de-allocate Multiple read memory buffers
	delete [] m_pbtReadBufferCompare;m_pbtReadBufferCompare=NULL;

	// de-allocate memory buffers
	delete [] m_pbtOverlapBuffer; m_pbtOverlapBuffer=NULL;

	delete m_pParanoiaDrive;
	if ( m_pParanoia  ) paranoia_free( m_pParanoia );

	m_pParanoia = NULL;
	m_pParanoiaDrive = NULL;
}

BOOL CCDExtract::SetupTrackExtract(int nBeginSector, int nEndSector)
{
	// Reset current sector
	m_lSector=0;

	m_nCurrentSpeed			= GetSpeed();
	m_nLastSpeedAdjust		= -1;

	if ( TRUE == IsC2ErrorDetectionEnabled() )
	{
		// Adjust for additional C2 error information
		m_dwReadSectors		= GetNumReadSectors() - ( GetNumReadSectors() / ( CB_CDDASECTORSIZE / CB_CDDAC2SIZE ) + 1 );
		m_dwReadBufferSize	= m_dwReadSectors * ( CB_CDDASECTORSIZE + CB_CDDAC2SIZE );
	}
	else
	{
		m_dwReadSectors		= GetNumReadSectors();
		m_dwReadBufferSize	= m_dwReadSectors * CB_CDDASECTORSIZE;
	}

	m_pbtReadBuffer = new BYTE[ m_dwReadBufferSize ];

	// Allocate memory for multi read buffers 
	m_pbtReadBufferCompare = new BYTE[ m_dwReadBufferSize ];

	// Allocate memory for the two read buffers
	if ( GetNumCompareSectors() > 0 )
	{
		m_pbtOverlapBuffer = new BYTE[ m_dwReadBufferSize ];
	}
	else
	{
		m_pbtOverlapBuffer=NULL;
	}

	// Rip at leat 10 sectors
	if (nBeginSector+10>nEndSector)
	{
		nEndSector = nBeginSector + 10;
	}

	// Set starting point
	m_nBeginSector = max( (long)nBeginSector + (long)GetOffsetStart(), 0 );

	// Set end point
	m_nEndSector = (long)nEndSector + (long)GetOffsetEnd();


	// Call to enable CDDA (needed for certain drives)
	EnableCdda( TRUE );

	// Don't allow a CD change while recording
	PreventMediaRemoval( TRUE );

	// Set SCSI timeout (in seconds)
	SetScsiTimeOut( 10 );

	// Check SCSI timeout
	int	nTimeout = GetScsiTimeOut();

	// Clear number of jitter errors
	m_nJitterErrors = 0;

	// Clear peak value
	m_nPeakValue = 0;

	// Set First Read = TRUE
	m_bFirstRead = TRUE;
	
	// calculate the number of sectors to rip
	m_dwBytesToDo = CB_CDDASECTORSIZE * ( m_nEndSector - m_nBeginSector );

	return TRUE;
}

BOOL CCDExtract::SetupTrackExtractParanoia(int nBeginSector, int nEndSector)
{
	BOOL	 bReturn = SetupTrackExtract(nBeginSector, nEndSector);

	if (NULL == m_pParanoia)
	{
		// first time -> allocate paranoia structure 
		m_pParanoiaDrive = new cdrom_drive;
		m_pParanoiaDrive->cdr = this;
		m_pParanoiaDrive->nsectors = m_dwReadSectors;
		m_pParanoia = paranoia_init(m_pParanoiaDrive);
	}

	int	 nParanoiaMode = PARANOIA_MODE_FULL ^ PARANOIA_MODE_NEVERSKIP;

	switch ( GetParanoiaMode() )
	{
		case 0:
			nParanoiaMode = PARANOIA_MODE_OVERLAP;
			break;
		case 1:
			nParanoiaMode &= ~PARANOIA_MODE_VERIFY;
			break;
		case 2:
			nParanoiaMode &= ~(PARANOIA_MODE_SCRATCH | PARANOIA_MODE_REPAIR);
			break;
		case 3:
			// use default setting
			break;
		default:
			break;
	}	

	// setup paranoia mode
	paranoia_modeset(m_pParanoia, nParanoiaMode);

	// setup paranoia extracting range
	paranoia_set_range(m_pParanoia, m_nBeginSector, m_nEndSector - 1 );

	// return result
	return bReturn;
}

BOOL CCDExtract::EndTrackExtract()
{
	// de-allocate memory buffers
	delete [] m_pbtReadBuffer;
	m_pbtReadBuffer=NULL;

	delete [] m_pbtReadBufferCompare;
	m_pbtReadBufferCompare = NULL;

	// de-allocate memory buffers
	delete [] m_pbtOverlapBuffer;
	m_pbtOverlapBuffer=NULL;

	if ( m_pParanoia  )
	{
		paranoia_free( m_pParanoia );
		m_pParanoia = NULL;
	}

	m_pParanoia = NULL;

	delete m_pParanoiaDrive;
	m_pParanoiaDrive = NULL;

	// Disable CDDA (needed for certain drives)
	EnableCdda( FALSE );

	// Allow a CD removal again
	PreventMediaRemoval( FALSE );

	m_pbAbort = NULL;

	return FALSE;
}

// now, figure out how many blocks to keep (jitter control) 
//  |-------- previous --------|                            
//                         |----------- buffer --------|    
//  we need to match the end of previous and start of buffer
//  so we do a match, and compute how many bytes of overlap 
//  to skip (bytes_to_skip)                                 

DWORD CCDExtract::CorrectJitter(BYTE *pbtOverlapPtr, BYTE *pbtReadPtr, DWORD dwBytesRead, BOOL &bJitterError)
{
	BOOL	bFound		= FALSE;
	int	nOffset		= 0;
	int 	nCompareBytes	= (int) GetNumCompareSectors() * (int) CB_CDDASECTORSIZE;
	int	nMaxByteCompare	= min(2 * (int) GetNumOverlapSectors() * (int) CB_CDDASECTORSIZE, dwBytesRead) - (int) nCompareBytes;
	int	nStartCompare	= nMaxByteCompare / 2;
	int 	nSkipBytes	= nStartCompare;

	BYTE	*pbtLCmp = pbtReadPtr + nStartCompare;
	BYTE	*pbtHCmp = pbtReadPtr + nStartCompare;

	bJitterError=FALSE;

	if ( NULL == pbtOverlapPtr )
	{
		return 0;
	}
	if ( NULL == pbtReadPtr)
	{
		return 0;
	}
	if ( 0 == dwBytesRead )
	{
		return 0;
	}
	if ( nMaxByteCompare <= 0 )
	{
		return 0;
	}

	for (nOffset = 0; nOffset < nStartCompare; nOffset += 4, pbtLCmp -= 4, pbtHCmp += 4 ) 
	{
		if (memcmp( pbtOverlapPtr, pbtLCmp,nCompareBytes) == 0) 
		{
			nSkipBytes= nStartCompare-nOffset+nCompareBytes;
			bFound = TRUE;
			break;
		}
		if ( memcmp( pbtOverlapPtr, pbtHCmp,nCompareBytes) == 0 ) 
		{
			nSkipBytes = nStartCompare+nOffset+nCompareBytes;
			bFound = TRUE;
			break;
		}

	}

	if ( !bFound && ( dwBytesRead > nCompareBytes ) ) 
	{
		bJitterError = TRUE;
		m_nJitterErrors++;
	}

	if (nMaxByteCompare)
	{
		m_nJitterPos = ( nSkipBytes - nCompareBytes ) * 100 / nMaxByteCompare;
	}
	else
	{
		m_nJitterPos = 50;
	}
	
	if ( nSkipBytes < 0 )
	{
		nSkipBytes=0;
	}

	if ( nSkipBytes >= dwBytesRead )
	{
		nSkipBytes = dwBytesRead - 1;
	}

	return nSkipBytes;
}

static char *callback_strings[15] = 
{
	"wrote",
	"finished",
	"read",
	"verify",
	"jitter",
	"correction",
	"scratch",
	"scratch repair",
	"skip",
	"drift",
	"backoff",
	"overlap",
	"dropped",
	"duped",
	"transport error"
};

void CCDExtract::paranoiaCallback(long inpos, int function)
{
	switch (function)
	{
		case PARANOIA_CB_SKIP:
			g_bParanoiaJitterErrors = TRUE;
			break;
	}
}

CDEX_ERR CCDExtract::RipChunkParanoia(BYTE *pbtStream, LONG *pNumBytes, BOOL &bAbort)
{
	*pNumBytes = 0;

	m_pbAbort = &bAbort;
	g_bParanoiaJitterErrors = FALSE;

	if (0 == m_dwBytesToDo)
	{
		SetPercentCompleted(100);
		return CDEX_RIPPING_DONE;
	}

	if (m_bFirstRead)
	{
		// Get starting sector which has to be copied
		m_lSector = m_nBeginSector;

		SetCDSpeed( GetSpeed() );

		// Spin up the CD Rom if necessary
		DWORD dwStart=::GetTickCount();

		while ((::GetTickCount() - dwStart) < (DWORD) GetSpinUpTime() * 1000)
		{
			// Read cd rom sector to Spin up the CD-ROM
			ReadCdRomSector(m_pbtReadBuffer, m_dwReadBufferSize, m_lSector, 1, IsC2ErrorDetectionEnabled());
		}

		m_bFirstRead = FALSE;
	}

	if ( m_pParanoia )
	{
		int nBlock = 0;

		for ( nBlock = 0; nBlock < m_dwReadSectors; nBlock++ )
		{
			// function return only one sector
			short* buf = paranoia_read( m_pParanoia, &CCDExtract::paranoiaCallback, &bAbort );

			if ( NULL == buf )
				return CDEX_ERROR;

			*pNumBytes+= CB_CDDASECTORSIZE;
			memcpy( pbtStream + nBlock * CB_CDDASECTORSIZE, buf, CB_CDDASECTORSIZE );


			m_dwBytesToDo -= min( m_dwBytesToDo, CB_CDDASECTORSIZE );

			if ( 0 == m_dwBytesToDo  )
			{
				SetPercentCompleted( 100 );
				return CDEX_OK;
			}
		}

		if ( g_bParanoiaJitterErrors )
		{
			m_nJitterErrors++;
		}

		m_lSector+= m_dwReadSectors;
		int nSectorsToDo = (m_nEndSector - m_lSector);

		SetPercentCompleted((int)( 100.0- (DOUBLE)nSectorsToDo * 100.0/ ( m_nEndSector - m_nBeginSector ) ) );

		// Calculate Peak Value
		short* psValue=(short*)(pbtStream);

		// Determine peak value
		for ( DWORD dwSample=0;dwSample< *pNumBytes/2; dwSample++ )
		{
			m_nPeakValue=max( abs( psValue[dwSample] ),m_nPeakValue );
		}

		if ( g_bParanoiaJitterErrors )
			return CDEX_JITTER_ERROR;

		return CDEX_OK;
	}

	return CDEX_ERROR;
}

CDEX_ERR CCDExtract::RipChunk(BYTE *pbtStream, LONG *pNumBytes, BOOL &bAbort)
{
	static LONG	lSectorEnd;
	static LONG	nSectorsToDo;
	static DWORD	dwSkipBytes=0;
	static DWORD	dwSkipBytesCompare=0;
	static DWORD	dwBytes=0;
	BOOL		bJitterCorr=FALSE;
	LONG		lOverlapSamples=GetNumOverlapSectors();

	if ( GetJitterCorrection() && ( lOverlapSamples > 0 ) )
	{
		bJitterCorr=TRUE;
	}
	else
	{
		lOverlapSamples=0;
	}

	if ( NULL == pNumBytes )
	{
		return CDEX_ERROR;
	}
	else
	{
		*pNumBytes=0;
	}

	if ( NULL == pbtStream || NULL == m_pbtReadBuffer )
	{
		return CDEX_ERROR;
	}

	if ( m_bFirstRead )
	{
		// Get starting sector which has to be copied
		m_lSector = m_nBeginSector;

		// Get last sector which has to be copied
		lSectorEnd = m_nEndSector;

		// calculate # of sectors to read
		nSectorsToDo = ( m_nEndSector - m_lSector );

		// Set the CD ripping speed
		SetCDSpeed( GetSpeed() );

		// Spin up the CD Rom if necessary
		DWORD dwStart=::GetTickCount();
		while ( (::GetTickCount()-dwStart)<(DWORD)GetSpinUpTime()*1000)
		{
			// Read cd rom sector to Spin up the CD-ROM
			ReadCdRomSector(m_pbtReadBuffer, m_dwReadBufferSize, m_lSector, 1, IsC2ErrorDetectionEnabled());
		}

	}

	// Loop through all the tracks, maybe something for a user thread in the near future
	// if (nSectorsToDo <=GetNumOverlapSectors()) 
	if ( 0 == m_dwBytesToDo )
	{
		SetPercentCompleted( 100 );
		return CDEX_RIPPING_DONE;
	}

	// Set recording percentage complete
	if ( (m_nEndSector - m_nBeginSector) != 0 )
	{
		SetPercentCompleted( (int)( 100.0 -(DOUBLE)nSectorsToDo * 100.0 / ( m_nEndSector - m_nBeginSector ) ) );
	}
	else
	{
		SetPercentCompleted( 0 );
	}

	BYTE* pbtWritePtr=NULL;
	DWORD dwBufferOffset = GetNumCompareSectors() * CB_CDDASECTORSIZE;
	BOOL	bJitterError=FALSE;

	LONG nSectorsToRead= min( nSectorsToDo, m_dwReadSectors );

	// Handle last sectors correctly
	// FIXME, CAN ALL DRIVERS HANDLE READING BEYOND THE LEAD-OUT LOCATION ?
	if ( nSectorsToRead <= lOverlapSamples )
	{
		// In this situation, we have to read the overlap area + the amount of sectors
		// that still have to be done.
		nSectorsToRead= min( lOverlapSamples + nSectorsToDo, m_dwReadSectors );
	}

	// We are reading only m_nReadSector*CB_CDDASECTORSIZE bytes
	DWORD dwBytesRead= nSectorsToRead * CB_CDDASECTORSIZE;

	// Should we do the multiple reads
	BOOL bJitterErrorCompare = FALSE; 
	BOOL bReadMatch = FALSE;

	BOOL doMultiReads = FALSE;
	// Read Multiple times and continue when match
	while ( FALSE == bReadMatch ) 
	{		
		// Default is to pass, will mark fail if found
		bReadMatch = TRUE;

		// Normal Read
		ReadChunk(	nSectorsToRead,
					lOverlapSamples,
					bJitterCorr,
					dwBytesRead,
					bJitterError,
					dwSkipBytes,
					m_pbtReadBuffer,
					m_dwReadBufferSize );
		
		// Read the compare blocks
		for (int rereads = 0; (bReadMatch) && (doMultiReads) && (rereads < GetMultiRead()) ; rereads++) 
		{

			// Read the block
			ReadChunk(	nSectorsToRead,
						lOverlapSamples,
						bJitterCorr,
						dwBytesRead,
						bJitterErrorCompare,
						dwSkipBytesCompare,
						m_pbtReadBufferCompare,
						m_dwReadBufferSize );

			// Compare
			for (int compareIndex = 0; (compareIndex + max(dwSkipBytes,dwSkipBytesCompare))  < dwBytesRead; compareIndex++)
			{				
				if (
					m_pbtReadBuffer[dwSkipBytes + compareIndex] != 
					m_pbtReadBufferCompare[dwSkipBytesCompare + compareIndex]
					)
				{
				bReadMatch = FALSE;
				continue;
				}
			}
		}

	}

	// If we did the multi read we need to copy the last block so the parameters that were
	// set in the read, like m_nJitterPos and m_nJitterErrors, will apply to the current
	// data
	if ( doMultiReads == TRUE )
	{
		memcpy(	m_pbtReadBuffer,
				m_pbtReadBufferCompare,
				dwBytesRead );
	}

	// Calculate Peak Value
	short* psValue=(short*)(m_pbtReadBuffer);

	// Determine peak value
	for ( DWORD dwSample=0; dwSample<dwBytesRead/2; dwSample++ )
	{
		m_nPeakValue=max( abs( psValue[ dwSample ] ),m_nPeakValue );
	}

	// Set beginning of 
	pbtWritePtr = m_pbtReadBuffer + dwSkipBytes;


	// Copy the compare region upfront 
	if ( GetNumCompareSectors() > 0 && m_pbtOverlapBuffer )
	{
		int nBytesToCopy=GetNumCompareSectors() * CB_CDDASECTORSIZE;
		int nCopyOffset=dwBytesRead-nBytesToCopy;

		if (nCopyOffset<0)
		{
			nCopyOffset=0;
			nBytesToCopy=dwBytesRead;
		}

		memcpy(m_pbtOverlapBuffer,m_pbtReadBuffer+nCopyOffset,nBytesToCopy);
	}

	// Set number of output bytes
	*pNumBytes=min( dwBytesRead - dwSkipBytes, m_dwBytesToDo );

	// Clip in case of last sector

	// Copy byte stream
	memcpy( pbtStream, pbtWritePtr, *pNumBytes );

	// Increase the sector offset
	m_lSector+=(nSectorsToRead-lOverlapSamples );


	// Lower the number of sectors still to do
	nSectorsToDo -= ( nSectorsToRead - lOverlapSamples );

	// Lower number of bytes todo
	m_dwBytesToDo -= *pNumBytes;

	if ( bJitterError )
	{
		return CDEX_JITTER_ERROR;
	}

	// And close CD device
	return CDEX_OK;
}

void CCDExtract::GetLastJitterErrorPosition(DWORD &dwStartSector, DWORD &dwEndSector)
{
	dwStartSector = m_lSector - ( m_dwReadSectors - GetNumOverlapSectors() );
	dwEndSector = m_lSector;
}

void CCDExtract::ReadChunk(const long nSectorsToRead, const long lOverlapSamples, const BOOL bJitterCorr, const DWORD dwBytesRead, BOOL &bJitterError, DWORD &dwSkipBytes, PBYTE pbtReadBuffer, DWORD dwReadBufferSize)
{
	// Read Chunk from CD-ROM
	if ((FALSE == ReadCdRomSector(pbtReadBuffer, dwReadBufferSize, m_lSector, nSectorsToRead, IsC2ErrorDetectionEnabled())) && (GetAspiRetries() > 0))
	{
		// If there was an error slow down the CDROM and try again
		FlushCache(m_lSector);

		SetCDSpeed(1);

		// Try it again
		ReadCdRomSector(pbtReadBuffer, dwReadBufferSize, m_lSector, nSectorsToRead, IsC2ErrorDetectionEnabled());
	}

	if (!m_bFirstRead && bJitterCorr)
	{
		// Perform Jitter Correction
		dwSkipBytes = CorrectJitter(m_pbtOverlapBuffer,  pbtReadBuffer, dwBytesRead, bJitterError);
	}
	else
	{
		dwSkipBytes=0;
		m_bFirstRead=FALSE; 
	}

	return;
}

// Interface for Monty's paranoia library:
// return the number of sectors?

long cdda_read(cdrom_drive *d, void *buffer, long beginsector, long sectors)
{
	int nRetries = 0;
	DWORD dwReadSize = 0;

	CCDExtract *cdr = (CCDExtract*)d->cdr;

	// when 100 sectors have be read without a problem, revert to default speed
	if (  ( ( beginsector - cdr->GetLastSectorSpeedAdjusted() ) > 500  ) &&
			(-1 != cdr->GetLastSectorSpeedAdjusted() ) )
	{
		if ( cdr->GetCurrentSpeed() != cdr->GetSpeed()  )
		{
			cdr->FlushCache( beginsector );
			cdr->SetCDSpeed( cdr->GetSpeed() );
			cdr->GetLastSectorSpeedAdjusted( -1 );
		}

	}

	if ( cdr->IsC2ErrorDetectionEnabled() )
	{
		dwReadSize = sectors * ( CB_CDDASECTORSIZE	+ CB_CDDAC2SIZE );
	}
	else
	{
		dwReadSize = sectors * ( CB_CDDASECTORSIZE	);
	}

	if ( sectors > 0 )
	{
		if ( ( FALSE == cdr->ReadCdRomSector(	(BYTE*)buffer,
												dwReadSize,
												beginsector,
												sectors,
												cdr->IsC2ErrorDetectionEnabled() ) ) 
				&& ( nRetries< 10 ) )
		{
			// read error, reduce spead if possible
			int nSpeed = cdr->GetCurrentSpeed();

			// always reset device after a read error
			cdr->FlushCache( beginsector );

			if ( nSpeed > 1)
			{
				cdr->GetLastSectorSpeedAdjusted( beginsector );
				cdr->SetCDSpeed( 1 );

			}

			// do a dummy read at the begin sector to flush cache buffer
			cdr->ReadCdRomSector(	(BYTE*)buffer, 
									dwReadSize,
									cdr->GetBeginSector(),
									sectors,
									cdr->IsC2ErrorDetectionEnabled() );

			sectors = -1;
		}
	}

	/* return -999 incase the abort button has been pressed */
	if ( cdr->GetAbort() )
	{
		sectors = -999;
	}
	
	return sectors;
}

CDEX_ERR CCDExtract::Init()
{
	return CAspiCD::Init();
}

void CCDExtract::FlushCache(DWORD dwCurrentSector)
{
	DWORD dwCenterSector = (m_nBeginSector + m_nEndSector) / 2;
	DWORD dwReadSector = 0;

	if (dwCurrentSector > dwCenterSector)
	{
		dwReadSector = m_nEndSector - m_dwReadSectors - 10;
	}
	else
	{
		dwReadSector = m_nBeginSector;
	}

	ReadCdRomSector(m_pbtReadBuffer, m_dwReadBufferSize, dwReadSector, m_dwReadSectors, IsC2ErrorDetectionEnabled());
}
