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

#include "AspiCD.h"
#include "CDRip.h"
#include "CDRomSettings.h"
#include <math.h>
#include <assert.h>

// Toshiba Speed Control Parameters
struct TOSHIBA_MODE_PAGE_20 
{		
	BYTE		nPageCode;				// parsave & pagecode 
	BYTE		nPageLen;				// 0x01 = length of paramter block from this point 
	BYTE		bitSpeed:1;
	BYTE		bitRes	:7;
};

// CAspiCD constructor
CAspiCD::CAspiCD()
	:CDRomSettings()
{
	m_bAvailable = FALSE;

	// Set time out value to 10 seconds
	m_nTimeOut = 10000;
	
	// Clear sense structure
	memset( m_SenseData, 0x00, SENSE_LEN );

	// Clear last error value
	m_btLastError = SS_COMP;

	m_bUseNtScsi = FALSE;

	m_bAvailable = FALSE;
}

CAspiCD::~CAspiCD()
{
}



BYTE CAspiCD::GetDeviceType(BYTE btAdapterID,BYTE btTargetID,BYTE btLunID)
{
	// Return device type
	return (BYTE) -1;
}

void CAspiCD::SetTimeOut(int nMilliSeconds)
{
	m_nTimeOut=nMilliSeconds;
}

void CAspiCD::GetDiskInfo()
{
	// Create SRB_GETDISKINFO header
	SRB_GETDISKINFO	mySrb;

	// Clear all fields
	memset(&mySrb,0x00,sizeof(SRB_GDEVBLOCK));

	// Set SRB fields
	mySrb.SRB_Cmd=SC_GET_DISK_INFO;

	mySrb.SRB_HaId = GetAdapterID();
	mySrb.SRB_Target = GetTargetID();
	mySrb.SRB_Lun = GetLunID();
	mySrb.SRB_Flags = 0;

	// Send command
	SendASPI32Command((LPSRB)&mySrb);

	m_btLastError=mySrb.SRB_Status;

	// Check aspi function call
	IsScsiError((LPSRB)&mySrb);
}

BYTE CAspiCD::ReadSubChannel(BYTE btDataFormat,BYTE* pbtBuffer,int nBufSize,BYTE btTrack)
{
	BYTE bReturn = 0;

	BYTE cmd [10];

	cmd[0]=		0x42;
	cmd[1]=		GetLunID()<<5;
	cmd[2]=		0;
	cmd[3]=		btDataFormat;
	cmd[4]=		0;
	cmd[5]=		0;
	cmd[6]=		btTrack;
	cmd[7]=		nBufSize>>8;
	cmd[8]=		nBufSize & 0xFF;
	cmd[9]=		0;

	switch (btDataFormat)
	{
		case 0x00:
		break;
		case 0x02:
		case 0x01:
			cmd[2]=0x40;
		break;
		default:
			break;
	}

	// Clear buffer
	memset(pbtBuffer,0x00,nBufSize);

	bReturn = IssueScsiCmd(SRB_DIR_IN,cmd,sizeof(cmd),pbtBuffer,nBufSize);

	return bReturn;
}



BYTE CAspiCD::ReadSubChannelQ(BYTE btDataFormat, BYTE *pbtBuffer, int nBufSize)
{
	BYTE	bReturn = 0;
	BYTE	cmd [12];
	
	cmd[0]=		0x42;
	cmd[1]=		GetLunID()<<5;
	cmd[2]=		0x40;
	cmd[3]=		btDataFormat;
	cmd[4]=		0;
	cmd[5]=		0;
	cmd[6]=		0;
	cmd[7]=		nBufSize>>8;
	cmd[8]=		nBufSize & 0xFF;
	cmd[9]=		0;
	cmd[10]=	0;
	cmd[11]=	0;

	memset(pbtBuffer,0x00,nBufSize);

	bReturn = IssueScsiCmd( SRB_DIR_IN, cmd, 10, pbtBuffer, nBufSize );

	return bReturn;
}


BYTE CAspiCD::IsAudioPlaying()
{
	BYTE bReturn = 0;

	// Create buffer
	BYTE pbtBuffer[48];

	// Read SubChannel information 0
	ReadSubChannel(0x01,pbtBuffer,sizeof(pbtBuffer));

	// Return result
	bReturn = pbtBuffer[1];

	return bReturn;
}


BYTE CAspiCD::CurrentPosition( DWORD& dwRelPos, DWORD& dwAbsPos )
{
	// Create buffer
	BYTE pbtBuffer[48];

	// Read SubChannel information 0
	BYTE btReturn=ReadSubChannel(0x01,pbtBuffer,sizeof(pbtBuffer));

	dwAbsPos=	((DWORD)pbtBuffer[11]);
	dwAbsPos+=	((DWORD)pbtBuffer[10])<<8;
	dwAbsPos+=	((DWORD)pbtBuffer[ 9])<<16;
	dwAbsPos+=	((DWORD)pbtBuffer[ 8])<<24;

	dwRelPos=	((DWORD)pbtBuffer[15]);
	dwRelPos+=	((DWORD)pbtBuffer[14])<<8;
	dwRelPos+=	((DWORD)pbtBuffer[13])<<16;
	dwRelPos+=	((DWORD)pbtBuffer[12])<<24;

	// Return result
	return btReturn;
}


BYTE CAspiCD::GetSubChannelTrackInfo(	int&	nReadIndex,
										int&	nReadTrack,
										DWORD&	dwReadPos )
{
	// Create buffer
	BYTE pbtBuffer[ 16 ];

	// Read SubChannel information 0
	BYTE btReturn = ReadSubChannelQ( 0x01, pbtBuffer, sizeof(pbtBuffer) );

	nReadTrack = pbtBuffer[ 6 ];
	nReadIndex = pbtBuffer[ 7 ];

	dwReadPos=	( (DWORD)pbtBuffer[11] );
	dwReadPos+=	( (DWORD)pbtBuffer[10] )<<8;
	dwReadPos+=	( (DWORD)pbtBuffer[ 9] )<<16;
	dwReadPos+=	( (DWORD)pbtBuffer[ 8] )<<24;

	return btReturn;
}


BYTE CAspiCD::Seek( DWORD dwAbsPos )
{
	BYTE		btReturn = 0;
	static BYTE cmd[10] = {0x2B,GetLunID()<<5, 0,0, 0, 0,0,0,0,0};

	cmd[2] =(BYTE)((dwAbsPos>>24) & 0xff);
	cmd[3] =(BYTE)((dwAbsPos>>16) & 0xff);
	cmd[4] =(BYTE)((dwAbsPos>>8 ) & 0xff);
	cmd[5] =(BYTE)(dwAbsPos&0xff);

	btReturn = IssueScsiCmd( SRB_DIR_OUT, cmd, sizeof( cmd ) );

	return btReturn;
}


CDEX_ERR CAspiCD::GetCDRomDevices()
{
	CDEX_ERR bReturn = CDEX_OK;

	bReturn = GetStatus();

	if ( CDEX_OK == bReturn )
	{
		BYTE btAdapterID=0;
		BYTE btLunID=0;
		BYTE btTargetID=0;

		for (btAdapterID=0; btAdapterID < m_bNumHA; btAdapterID++)
		{
			for (btTargetID = 0; btTargetID < 16; btTargetID++)
			{
				int nMaxLunID = 7;

				for (btLunID = 0; btLunID <= nMaxLunID; btLunID++)
				{
					SCSI_INQUIRY_RESULT	 inqResult;

					if ( SS_COMP == InquiryCommand( btAdapterID, btTargetID, btLunID, &inqResult ) )
					{
						CHAR lpszDeviceName[ 25 ];
						DWORD dwDeviceType = inqResult.btDeviceType;

						// Create a string with CD-ROM Vendor and type information
						strncpy( lpszDeviceName, inqResult.lpsVendorId , sizeof( lpszDeviceName ) - 1 );
						lpszDeviceName[ sizeof( lpszDeviceName ) - 1 ] = '\0';

						if ( dwDeviceType == DTC_CDROM || dwDeviceType == DTC_WORM )
						{
							// Add entry to array of CD-ROM Settings
							AddCDRom( lpszDeviceName, btAdapterID, btTargetID, btLunID );

						}
					}
				}
			}
		}
	}

	return bReturn;
}



BOOL CAspiCD::IsMMC( LPSTR lpszInfo )
{ 
	BOOL bReturn = FALSE;

	BYTE pbtBuffer[ 0xFF ];
	
	// Clear buffer
	memset( pbtBuffer, 0x00, sizeof( pbtBuffer ) );

	ModeSense( 0x2A, pbtBuffer, sizeof( pbtBuffer ) );

	SCISMODEHEADER* pBlockHeader=(SCISMODEHEADER*)pbtBuffer;
	SCSICDMODEPAGE2A* pPage=(SCSICDMODEPAGE2A*)( &pbtBuffer[ 4 ] );

	int n2aSize = sizeof( SCSICDMODEPAGE2A );
	int nBlockSize = sizeof( SCISMODEHEADER );

	strcpy( lpszInfo, "Drive does not support MMC" );

	if( ( pbtBuffer[ 4 ] & 0x3F ) == 0x2A )
	{
		// Set mmc variable or so
		// MMC style drive!
		if(pbtBuffer[5]>=4)
		{
			if(pbtBuffer[9]&0x1)
			{
				strcpy( lpszInfo, "Drive is MMC style" );
				bReturn = TRUE;
			}
		}
		else
		{
			strcpy( lpszInfo, "MMC drive, but reports CDDA incapable" );
			bReturn = TRUE;
		}
	}

	return bReturn;
}

BYTE CAspiCD::InquiryCommand(BYTE btAdapterID, BYTE btTargetID, BYTE btLunID, SCSI_INQUIRY_RESULT *pbtResult)
{
	BYTE		 btReturn	= (BYTE) -1;
	static BYTE	 cmd[6]		= { SCSI_CMD_INQUIRY, btLunID << 5, 0, 0, sizeof(SCSI_INQUIRY_RESULT), 0 };

	// Clear buffer
	memset(pbtResult, 0x00, sizeof(SCSI_INQUIRY_RESULT));

	// do SCSI call
	btReturn = IssueScsiCmd(SRB_DIR_IN, cmd, sizeof(cmd), (PBYTE) pbtResult, sizeof(SCSI_INQUIRY_RESULT), btAdapterID, btTargetID, btLunID);

	return btReturn;
}

void CAspiCD::GetDeviceName(BYTE btAdapterID,BYTE btTargetID,BYTE btLunID,LPSTR lpszDviceName)
{
	static BYTE cmd[6] = {0x12,btLunID<<5, 0,0, 0x24, 0};

	static BYTE pbtBuffer[ 0x25];

	// Clear buffer
	memset( pbtBuffer, 0x00, sizeof( pbtBuffer ) );

	IssueScsiCmd( SRB_DIR_IN, cmd, sizeof( cmd ), pbtBuffer, sizeof( pbtBuffer ), btAdapterID, btTargetID, btLunID );

	// Create a string with CD-ROM Vendor and type information
	strncpy( lpszDviceName, (LPSTR)&pbtBuffer[ 8 ], 24 );
}

BYTE CAspiCD::ModeSense(int nPage,BYTE* pbtBuffer,int nBufSize)
{
	BYTE	btReturn = 0;

	// Clear result buffer
	memset( pbtBuffer, 0x00, nBufSize );

	BYTE cmd[12];

	// Init buffers
	memset(cmd,0x00,sizeof(cmd));

	// Common settings
	cmd[1] = GetLunID()<<5;				// Lun
	cmd[2] = nPage & 0x3F;				// Get page values (current value)

	// ATAPI or SCSI sense ?
	if (GetDriveTable().bAtapi==TRUE)
	{
		cmd[0]= 0x5A;					// Operation Code
		cmd[7]=	nBufSize>>8;
		cmd[8]=	nBufSize&0xFF;

		// Call SCSI command
		btReturn=IssueScsiCmd(SRB_DIR_IN,cmd,10,pbtBuffer,nBufSize);

		int nLen=pbtBuffer[1];
		nLen+=((int)pbtBuffer[0])<<8;

		// Convert header to standard SCSI header
		pbtBuffer[0]=(BYTE)(nLen-4);
		pbtBuffer[1]=pbtBuffer[2];
		pbtBuffer[2]=pbtBuffer[3];
		pbtBuffer[3]=pbtBuffer[7];
		memmove(pbtBuffer+4,pbtBuffer+8,nBufSize-4);

	}
	else
	{
		cmd[0]=0x1A;		// Operation Code
		cmd[8]=	nBufSize;	// Set low word of buffer size

		// Call SCSI command
		btReturn=IssueScsiCmd(SRB_DIR_IN,cmd,6,pbtBuffer,nBufSize);
	}
	return btReturn;
}


BYTE CAspiCD::ModeSelect(int nPageSize,BYTE* pbtBuffer,int nBufSize,BOOL bPageFormat)
{
	BYTE	btReturn;
	BYTE	cmd[10];

	// Init cmd memory
	memset(cmd,0x00,sizeof(cmd));

	if (GetAtapi()==TRUE)
	{
		int nBlkSize=8;					// = size of ATAPI block

		cmd[0]=0x55;					// Operation Code
		cmd[1]=GetLunID()<<5;				// Lun 
		
		if (bPageFormat)
			cmd[1]|=0x10;				// Have to select Page Format for vendor specific pages

		cmd[8]=nPageSize+nBlkSize;		// sizeof mode

		// Shift the stuff 8 bytes for ATAPI block header
		memmove(&pbtBuffer[nBlkSize],&pbtBuffer[0],nBufSize-nBlkSize);

		// Init first eight bytes
		memset(pbtBuffer,0x00,nBlkSize);

		// Setup ATAPI block descriptor
		pbtBuffer[nBlkSize-1]=8;			// Block Descriptor Length

		// Call SCSI command (10 bytes command)
		btReturn = IssueScsiCmd(SRB_DIR_OUT,cmd,10,pbtBuffer,nPageSize+nBlkSize);
	}
	else
	{
		int nBlkSize=4;						// = size of SCSI block
		cmd[0]=0x15;						// Operation Code
		cmd[1]=GetLunID()<<5;				// Lun 
		
		if (bPageFormat)
			cmd[1]|=0x10;				// Have to select Page Format for vendor specific pages

		cmd[4]=nPageSize+nBlkSize;			// sizeof mode 

		// Shift the stuff 4 bytes for SCSI block header
		memmove(&pbtBuffer[nBlkSize],&pbtBuffer[0],nBufSize-nBlkSize);

		// Init first four bytes
		memset(pbtBuffer,0x00,nBlkSize);

		// Setup SCSI block descriptor
		pbtBuffer[nBlkSize-1]=8;			// Block Descriptor Length

		// Call SCSI command (6 BYTES command)
		btReturn=IssueScsiCmd(SRB_DIR_OUT,cmd,6,pbtBuffer,nPageSize+nBlkSize);
	}
	return btReturn;
}



// return sector size and sets original density
DWORD CAspiCD::GetSectorSize( BYTE& btDensity )
{
	BYTE pbtBuffer[0xFF];

	DWORD dwSectorSize = CB_CDROMSECTOR;

	// Set default density, is this correct???
//	btDensity=0x81;
	btDensity = 0x00;

	// Sense Page 0x01 (error recovery) to get block decriptor
	ModeSense( 0x01, pbtBuffer, sizeof( pbtBuffer ) );
	
	// FIXME: some drives dont deliver block descriptors !!!
	// In that case, use the default values (i.e. secsize=2048, density=0x81)
	SCSIMODEHDR_6* pScsiHeader = (SCSIMODEHDR_6*)pbtBuffer;

	if ( pScsiHeader->btBlkDescrLen > 0 )
	{
		// Get original density???
		SCSIBLOCKDESCRIPTOR* pBlockDescriptor = (SCSIBLOCKDESCRIPTOR*)&pbtBuffer[ 4 ];

		btDensity = pBlockDescriptor->btDensity;

		dwSectorSize = pBlockDescriptor->btBlockLenL;
		dwSectorSize += (DWORD)(pBlockDescriptor->btBlockLenM)<<8;
	}

    return dwSectorSize;
}



void CAspiCD::SetSectorSize(DWORD dwSecSize,BYTE btDensity)
{
	// 8 additional bytes for the speed selection mode pages
	BYTE pbtBuffer[16+8];

	TOSHIBA_MODE_PAGE_20 ToshibaSpeedPage={0x20,0x01,0,8};

	BYTE PhilipsSpeedPage[8]={0x23,0x06,1,1,1,0,0,0};


	// Which speed is selected
	if ( GetSpeed() > 1 )
	{
		ToshibaSpeedPage.bitSpeed=1;

		// Philips speed can be set max to 4 for CD 2660
		PhilipsSpeedPage[4]=PhilipsSpeedPage[2]=min(GetSpeed(),4);
	}

	// Init buffers
	memset(pbtBuffer,0x00,sizeof(pbtBuffer));

	// Just a sanity check, this should never occur anyways
	if (dwSecSize==2048)
	{
		btDensity=0x81;
	}

	// Create SCSI block desciptor
	pbtBuffer[0]=btDensity;					// Density Code
	pbtBuffer[6]=(BYTE)(dwSecSize >> 8);	// block length "msb"
	pbtBuffer[7]=(BYTE)(dwSecSize & 0xFF);	// block length lsb

	if (GetSpeedMethod()==SPEEDTOSHIBA && dwSecSize!=2048 && GetSpeed()>0 )
	{
		// Add the mode page bits
		memcpy(pbtBuffer+8,&ToshibaSpeedPage,sizeof(ToshibaSpeedPage));

		// Call mode select, with Toshiba speed selection code
		ModeSelect(8+sizeof(ToshibaSpeedPage),pbtBuffer,sizeof(pbtBuffer),TRUE);
	}
	else
	if (GetSpeedMethod()==SPEEDPHILIPS && dwSecSize!=2048 && GetSpeed()>0 )
	{
		// Add the mode page bits
		memcpy(pbtBuffer+8,&PhilipsSpeedPage,sizeof(PhilipsSpeedPage));

		// Call mode select, with Toshiba speed selection code
		ModeSelect(8+sizeof(PhilipsSpeedPage),pbtBuffer,sizeof(pbtBuffer),TRUE);
	}
	else
	{
		// Call mode select, just to set the block desciptor only
		ModeSelect(8,pbtBuffer,16,FALSE);
	}

}


// switch Toshiba/DEC and HP drives from/to cdda density
// reserved, Medium type=0, Dev spec Parm = 0, block descriptor len 0 oder 8, Density (cd format) 
// (0=YellowBook, XA Mode 2=81h, XA Mode1=83h and raw audio via SCSI=82h),
// # blks msb, #blks, #blks lsb, reserved, blocksize, blocklen msb, blocklen lsb,
// MODE_SELECT, page = SCSI-2  save page disabled, reserved, reserved, parm list len, flags

void CAspiCD::EnableCdda(BOOL bAudioMode)
{
	static DWORD	dwSecSize=2048;			// Static variable !!
	static BYTE		btDensity=0;			// Static variable !!


	// Check if we have to do this anyways
	if (GetEnableMode()==ENABLENONE)
		return;

	// Get the original sector size (and denisity code), if we don't have it yet 
	if (dwSecSize==0)
	{
		dwSecSize=2048;
//		dwSecSize=GetSectorSize(btDensity);
	}

	// Switch to audio mode??
	if (bAudioMode)
	{
		// Switch to Audio mode
		SetSectorSize(CB_CDDASECTORSIZE,GetDensity());
	}
	else
	{
		// If dwSector is set, return old values
		if (dwSecSize!=0)
		{
			// Switch to Audio mode
			SetSectorSize(dwSecSize,btDensity);

			// Clear values
			dwSecSize=0;
			btDensity=0;
		}
	}
}


void CAspiCD::SetCDSpeed( int nSpeed )
{
	BYTE btH=0;
	BYTE btL=0;

	if ( nSpeed > 0 )
	{


		if ( nSpeed >= 48 )
		{
			btH = 0xFF;
			btL = 0xFF;
		}
		else
		{
			btH=(nSpeed*0xB0)>>8;			// Get speed High byte
			btL=(nSpeed*0xB0)&0xFF;		// Get speed Low Byte
		}

		static BYTE pbtBuffer[16];

		// Init buffers
		memset(pbtBuffer,0x00,sizeof(pbtBuffer));

		int nCmdSize=10;

		static BYTE cmd [12] = {0x00,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		switch (GetSpeedMethod())
		{
			case SPEEDMMC:	
				cmd [0]=0xBB; 
				nCmdSize=12;
				// Only set this when the user has requested it!
				if (nSpeed>0)
				{
					// Setup CMD structure
					cmd[0] = 0xBB;				// Operation Code
					cmd[1] = GetLunID() << 5 ;	// Lun
					cmd[2] = btH;				// High word of speed
					cmd[3] = btL;				// Low word of speed
					cmd[4] = 0xFF;
					cmd[5] = 0xFF;

					// Send to ASPI
					IssueScsiCmd( SRB_DIR_OUT, cmd,nCmdSize, NULL, 0 );
				}
			break;

	//		case READ10:	cmd [0]=0x28; nCmdSize=10;break;	// Read Std
	//		case SPEEDNEC:	cmd [0]=0xD4; nCmdSize=10;break;	// Read 10
			case SPEEDSONY:	
				if (nSpeed>0)
				{
					nCmdSize=6;
					cmd[0]=0x15;				// MODE_SELECT
					cmd[1]=0x10;				// no save page
					cmd[2]=0;					// reserved
					cmd[3]=0;					// reserved
					cmd[4]=4 + 4;				// sizeof(mode)
					cmd[5]=0;					// reserved

					pbtBuffer[0]=0;
					pbtBuffer[1]=0;
					pbtBuffer[2]=0;
					pbtBuffer[3]=0;
					pbtBuffer[4]=0x31;
					pbtBuffer[5]=2;
					pbtBuffer[6]=0;				// speed
					pbtBuffer[7]=0;				// reserved

					// speed values > 1 are drive dependent
					if (nSpeed > 4)
						nSpeed = 8;

					pbtBuffer[6] = (nSpeed / 2);

					// Send to ASPI
					IssueScsiCmd(SRB_DIR_OUT,cmd,nCmdSize,pbtBuffer,8);
				}
		}
	}
}



BOOL CAspiCD::IsUnitReady()
{
	static BYTE cmd[6] = {0x00, 0, 0,0, 0, 0};

	BOOL bReturn=(BOOL)IssueScsiCmd( 0, cmd, sizeof( cmd ) );

	return bReturn;
}

BOOL CAspiCD::RequestSense()
{
	static BYTE pbtBuffer[100];

	// Init buffers
	memset(pbtBuffer,0x00,sizeof(pbtBuffer));

	static BYTE cmd[6] = {0x03, 0, 0,0, (BYTE)sizeof(pbtBuffer),0};

	BOOL bReturn=(BOOL)IssueScsiCmd(SRB_DIR_IN,cmd,sizeof(cmd),pbtBuffer,sizeof(pbtBuffer));

	return bReturn;
}


CDMEDIASTATUS CAspiCD::IsMediaLoaded()
{
	CDMEDIASTATUS returnValue = CDMEDIA_NOT_PRESENT;

	static BYTE cmd[6] = {0x00, 0, 0,0, 0, 0};

	BYTE btResult = IssueScsiCmd( 0x00, cmd, sizeof(cmd) );

	switch ( g_CDStatusInfo.sk )
	{
		case 0:
			returnValue = CDMEDIA_PRESENT;
		break;
		case 2:
			if ( 0x3A == g_CDStatusInfo.asc )
			{
				if ( 0x00 == g_CDStatusInfo.ascq )
				{
					returnValue = CDMEDIA_NOT_PRESENT;
				}
				else if ( 0x01 == g_CDStatusInfo.ascq )
				{
					returnValue = CDMEDIA_NOT_PRESENT_TRAY_CLOSED;
				}
				else if ( 0x02 == g_CDStatusInfo.ascq )
				{
					returnValue = CDMEDIA_NOT_PRESENT_TRAY_OPEN;
				}
			}
	}

	return returnValue;
}


BOOL CAspiCD::Scan(DWORD dwSector,BOOL bForeWard)
{
	int nCmdSize=0;

	static BYTE cmd[12];

	// clear cmd buffer
	memset(cmd,0x00,sizeof(cmd));

	nCmdSize=10;
	
	cmd[0]= 0xBA;
	cmd[1]= GetLunID()<<5;
	cmd[1]+= (bForeWard)?0x00:0x10;

	cmd[2]= (BYTE)(dwSector >> 24);
	cmd[3]= (BYTE)((dwSector >> 16) & 0xFF);
	cmd[4]= (BYTE)((dwSector >> 8) & 0xFF);
	cmd[5]= (BYTE)(dwSector & 0xFF);

	return (BOOL)IssueScsiCmd(SRB_DIR_OUT,cmd,nCmdSize);
}

BOOL CAspiCD::PlayTrack(DWORD dwStartSector,DWORD dwEndSector)
{
	int nCmdSize=0;

	static BYTE cmd[12];

	// clear cmd buffer
	memset(cmd,0x00,sizeof(cmd));

	DWORD dwSector=dwStartSector;

	// There might be quite a few sectors
	DWORD dwNumSectors=dwEndSector-dwStartSector;

/*
if (g_config.GetPlayMethod()==10)
{
	nCmdSize=10;
	
	cmd[0]= 0x45;
	cmd[1]= GetLunID()<<5;
	cmd[2]= (BYTE)(dwSector >> 24);
	cmd[3]= (BYTE)((dwSector >> 16) & 0xFF);
	cmd[4]= (BYTE)((dwSector >> 8) & 0xFF);
	cmd[5]= (BYTE)(dwSector & 0xFF);

	// RESERVED
	cmd[6]= (BYTE)0x00;

	// Set number of sector to read
	cmd[7]= (BYTE)((dwNumSectors >>8) & 0xFF);
	cmd[8]= (BYTE) (dwNumSectors & 0xFF);
}



if (g_config.GetPlayMethod()==12)
{
*/	nCmdSize=12;
	cmd[0]= 0xA5;
	cmd[1]= GetLunID()<<5;

	// Start sector
	cmd[2]= (BYTE)(dwSector >> 24);
	cmd[3]= (BYTE)((dwSector >> 16) & 0xFF);
	cmd[4]= (BYTE)((dwSector >> 8) & 0xFF);
	cmd[5]= (BYTE)(dwSector & 0xFF);

	// Set Track Length
	cmd[6]= (BYTE)((dwNumSectors >>24) & 0xFF);
	cmd[7]= (BYTE)((dwNumSectors >>16) & 0xFF);
	cmd[8]= (BYTE)((dwNumSectors >>8) & 0xFF);
	cmd[9]= (BYTE) (dwNumSectors & 0xFF);

/*}

// USE MSF frames
if (g_config.GetPlayMethod()==6)
{
	nCmdSize=10;
	cmd[0]= 0x47;
	cmd[1]= GetLunID()<<5;

	// Start sector
	cmd[3]= (BYTE)((dwSector / 75) /60);
	cmd[4]= (BYTE)((dwSector / 75) %60);
	cmd[5]= (BYTE)((dwSector %75 )    );

	cmd[6]= (BYTE)((dwSector+dwNumSectors / 75) /60);
	cmd[7]= (BYTE)((dwSector+dwNumSectors / 75) %60);
	cmd[8]= (BYTE)((dwSector+dwNumSectors %75 )    );
}
*/
	return (BOOL)IssueScsiCmd(SRB_DIR_OUT,cmd,nCmdSize);

}


BOOL CAspiCD::StopPlayTrack()
{
	// Set up SCSI command buffer
	static BYTE cmd[6] = {0x1B,GetLunID()<<5|01,0,0,0,0};
	// Do command
	return (BOOL)IssueScsiCmd(SRB_DIR_OUT,cmd,sizeof(cmd));
}


short SWAPSHORT(short sSwap)
{
	return ((sSwap>>8)&0xFF) + ((sSwap&0xFF) <<8);
}

BOOL CAspiCD::EjectCD(BOOL bEject)
{
	BYTE cmd[6] = {0x1B, 0, 0, 0, 0, 0};
	
	LONG lSector = 0;

	WORD wNumSectors= 0;

	cmd [1]=GetLunID()<<5;

	if (bEject)
		cmd [4]=0x02;
	else
		cmd [4]=0x03;

	return (BOOL)IssueScsiCmd(SRB_DIR_OUT,cmd,sizeof(cmd));
}


BOOL CAspiCD::PauseCD(BOOL bPause)
{
	BYTE cmd[10] = {0x4B, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	cmd [1]=GetLunID()<<5;

	if (!bPause)
		cmd [8]=0x01;
	else
		cmd [8]=0x00;

	return (BOOL)IssueScsiCmd(SRB_DIR_OUT,cmd,sizeof(cmd));
}


WORD SWAPWORD(WORD nSwap)
{
	return ((nSwap>>8)&0xFF) || ((nSwap&0xFF)<<8);
}


BYTE CAspiCD::IssueScsiCmd(BYTE bFlags,LPBYTE lpcbData,int ncbLen)
{
	return IssueScsiCmd(bFlags,lpcbData,ncbLen,NULL,0,GetAdapterID(),GetTargetID(),GetLunID());
}

BYTE CAspiCD::IssueScsiCmd(BYTE bFlags,LPBYTE lpcbData,int ncbLen,LPBYTE lpBuffer,int nBufLen)
{
	return IssueScsiCmd(bFlags,lpcbData,ncbLen,lpBuffer,nBufLen,GetAdapterID(),GetTargetID(),GetLunID());
}


BOOL CAspiCD::ScsiAbort(SRB_EXECSCSICMD* sp,BYTE btAdapterID)
{
	DWORD			dwStatus = 0;
	SRB_Abort		s;

	// Clear SRB_Abort structure
	memset(&s,0x00,sizeof(s));

	// Set structure variables
	s.SRB_Cmd		= SC_ABORT_SRB;			// ASPI command code = SC_ABORT_SRB
	s.SRB_HaID		= btAdapterID;			// ASPI host adapter number
	s.SRB_Flags		= SRB_DIR_IN;			// Flags
	s.SRB_ToAbort	= (LPSRB)&sp;			// sp

	// Initiate SCSI abort
	dwStatus = SendASPI32Command((LPSRB)&s);

	m_btLastError=s.SRB_Status;

	// Check condition
	if (s.SRB_Status != SS_COMP)
	{
//		printf("Abort ERROR! 0x%08X\n", s.SRB_Status);

		// Indicate that error has occured
		return (FALSE);
	}

	// Everything went OK
	return (TRUE);
}

void  CAspiCD::SetScsiTimeOut(int nTimeOut)
{
}

int CAspiCD::GetScsiTimeOut()
{
	return 10*2;
}

BYTE CAspiCD::IssueScsiCmd(BYTE bFlags,LPBYTE lpcbData,int ncbLen,LPBYTE lpBuffer,int nBufLen,BYTE btAdapterID,BYTE btTargetID,BYTE btLunID)
{
	
// added by Andi, scenalyzer@blackbox.net
	SRB_EXECSCSICMD	mySrb;					// Create SRB_EXECSCSICMD header
	HANDLE			hEvent=NULL;			// Handle to event

// end added

    DWORD			dwASPIEventStatus=0;
    DWORD			dwASPIStatus;

	// Clear all fields
	memset(&mySrb,0x00,sizeof(SRB_EXECSCSICMD));

	// initialize sense status info
	g_CDStatusInfo.sk	       = 0x0B;
	g_CDStatusInfo.asc	       = 0;
	g_CDStatusInfo.ascq	       = 0;
	g_CDStatusInfo.ha_stat     = 0;
	g_CDStatusInfo.target_stat = 0;

    // **************************************************
    // Create event for MySRB.  Initial state         
    // non-signaled, manual reset.                    
    // **************************************************

	// Make sure it is a manual reset event!
    if ((hEvent = CreateEvent(NULL,TRUE,FALSE,NULL)) == NULL)
	{
          return SS_ABORTED;
	}

	// Set SRB fields
	mySrb.SRB_Cmd=SC_EXEC_SCSI_CMD;
	mySrb.SRB_HaId=btAdapterID;
	mySrb.SRB_Flags = SRB_EVENT_NOTIFY | SRB_DIR_IN;
	mySrb.SRB_Target=btTargetID;
	mySrb.SRB_Lun=btLunID;
	mySrb.SRB_SenseLen=SENSE_LEN;
	mySrb.SRB_CDBLen=ncbLen;
	mySrb.SRB_BufLen=nBufLen;
	mySrb.SRB_BufPointer=lpBuffer;
	mySrb.SRB_PostProc= (POSTPROCFUNC)hEvent;


	// Copy CBDByte data, if available
	if (lpcbData!=NULL)
	{
		memcpy(&mySrb.CDBByte,lpcbData,ncbLen);
	}

	// Clear event handle
	ResetEvent(hEvent);

	// Send ASPI32 command
	dwASPIStatus = SendASPI32Command( (LPSRB)&mySrb );

    // **************************************************
    // Block on event till signaled                     *
    // **************************************************
    if ( dwASPIStatus == SS_PENDING )
	{
		// Wait till time-out
		dwASPIEventStatus = WaitForSingleObject( hEvent, m_nTimeOut );


		// Check if time-out did occur
		if ( WAIT_TIMEOUT == dwASPIEventStatus )
 		{
			// Clear event, time out
			ResetEvent( hEvent );
		}

		if ( SS_PENDING == mySrb.SRB_Status )
		{
			// Time out condition
			// Abort SCSI command
			ScsiAbort(&mySrb,btAdapterID);

			// Close the event handle
			CloseHandle(hEvent);

			// Indicate that command has been aborted
			m_btLastError=mySrb.SRB_Status;
			return SS_ABORTED;
		}

	}

	g_CDStatusInfo.sk	       = mySrb.SenseArea[  2 ] & 0x0F;
	g_CDStatusInfo.asc	       = mySrb.SenseArea[ 12 ];
	g_CDStatusInfo.ascq	       = mySrb.SenseArea[ 13 ];
	g_CDStatusInfo.ha_stat     = mySrb.SRB_HaStat;
	g_CDStatusInfo.target_stat = mySrb.SRB_TargStat;

	m_btLastError = mySrb.SRB_Status;

	/*
	 * Check ASPI command status
	 */
	if (mySrb.SRB_Status != SS_COMP)
	{
		LogSenseData();
		// Clear event, time out
		//ResetEvent(hEvent);
	}

	// Close the event handle
	CloseHandle(hEvent);

	// Check aspi function call
	//dwAspiStatus=IsScsiError((LPSRB)&mySrb);


	return mySrb.SRB_Status;
}

void CAspiCD::BusDeviceReset()
{
	// Create SRB_GETDISKINFO header
	SRB_BUSDEVICERESET mySrb;

	// Clear all fields
	memset( &mySrb, 0x00, sizeof( SRB_BUSDEVICERESET ) );

	// Set SRB fields
	mySrb.SRB_Cmd = SC_RESET_DEV;

	mySrb.SRB_HaId = GetAdapterID();
	mySrb.SRB_Target = GetTargetID();
	mySrb.SRB_Lun = GetLunID();

	// Send command
	DWORD dwReturn = SendASPI32Command( (LPSRB)&mySrb );


	// Check aspi function call
	IsScsiError((LPSRB)&mySrb);

	m_btLastError = mySrb.SRB_Status;
}


void CAspiCD::PreventMediaRemoval (BOOL bAudioMode)
{
	if ( GetLockDuringRead() )
	{
		BYTE cmd [6] = {0x1E,GetLunID()<<5, 0, 0, 0, 0 };

		// Prevent Media removal by setting flagg
		if (bAudioMode) 
		{
			cmd[4] = 0x1;
		}

		// And issue SCSI command
		IssueScsiCmd( SRB_DIR_OUT, cmd,sizeof(cmd), NULL, 0 );
	}
}


// Method to get CD table of contents via SCSI interface
CDEX_ERR CAspiCD::ReadToc()
{
	const int nBufferSize=CB_CDROMSECTOR;

	BYTE pbtBuffer[nBufferSize];

	// Clear buffer
	memset(pbtBuffer,0x00,sizeof(pbtBuffer));

	BYTE cmd [10] = { 0x43,GetLunID()<<5, 0, 0, 0, 0, 1, CB_CDROMSECTOR >> 8, CB_CDROMSECTOR & 0xFF, 0 };

	// Issue Scsi command to read Table Of Contents
	IssueScsiCmd(SRB_DIR_IN,cmd,sizeof(cmd),pbtBuffer,nBufferSize);

	// Calculate total number of TOC entries
	WORD wNumTracks = ((pbtBuffer [0] << 8) + pbtBuffer [1] - 2) / 8 ;


	// Add TOC entries
	GetToc().Clear();

	// Set numbner of TOC entries
	if (wNumTracks>0)
	{
		// Get a pointer to the track information
		BYTE* btTocStream=pbtBuffer + 4;

		// Fill all the track info entries
		// Also add the lead out track (TRACK 0xAA)
		for (int iTrackEntry=0;iTrackEntry<wNumTracks;iTrackEntry++)
		{
			GetToc().FillEntry(btTocStream+iTrackEntry*8);
		}
	}

	return CDEX_OK;
}


// Method to get CD table of contents via SCSI interface
CDEX_ERR CAspiCD::ReadCDText( BYTE* pbtBuffer, int nBufferSize, LPINT pnCDTextSize)
{
	CDEX_ERR	bReturn = CDEX_OK;
	BYTE		cmd [10] = { 0x43,GetLunID()<<5, 5, 0, 0, 0, 1, 0, 4, 0 };

	// Clear buffer
	memset( pbtBuffer, 0x00, nBufferSize );

	// Issue Scsi command to read Table Of Contents
	IssueScsiCmd(SRB_DIR_IN,cmd,sizeof(cmd),pbtBuffer,4);

	// Get length of CD_TEXT fields
	*pnCDTextSize = ((pbtBuffer[0] << 8 ) | pbtBuffer[1]) + 2;

	// Check size, return if it is too small
	if ( (*pnCDTextSize > nBufferSize ) || ( *pnCDTextSize < 4 ) )
	{
		bReturn = CDEX_ERROR;
	}
	else
	{
		cmd[7] = (BYTE)( *pnCDTextSize >> 8    );
		cmd[8] = (BYTE)( *pnCDTextSize &  0xFF );

		// Issue Scsi command to read Table Of Contents
		IssueScsiCmd(	SRB_DIR_IN,
						cmd,
						sizeof( cmd ),
						pbtBuffer,
						nBufferSize );
	}

	return bReturn;
}


// added by Andi, scenalyzer@blackbox.net

#include "process.h"

struct ReadParas
{
	BYTE* pbtReadBuffer;
	int  nSectorsPerRead;
	LONG lSector;
	DWORD dwNumSectors;
	CRITICAL_SECTION crit;
	BOOL m_bOk;
	CAspiCD *pCd;
	DWORD	dwReadBufferSize;
};

void __cdecl ReadThreadProc(LPVOID param)
{
	ReadParas *pRd = (ReadParas*)param;

	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL); 

	while(1)
	{
		EnterCriticalSection(&pRd->crit);

		BYTE*	pbtReadBuffer = pRd->pbtReadBuffer;
		LONG	lSector = pRd->lSector;
		DWORD	dwNumSectors = pRd->dwNumSectors;

		if(dwNumSectors > pRd->nSectorsPerRead)
			dwNumSectors = pRd->nSectorsPerRead;

		pRd->pbtReadBuffer += CB_CDDASECTORSIZE * dwNumSectors;

		pRd->dwNumSectors -= dwNumSectors;

		pRd->lSector += dwNumSectors;

		LeaveCriticalSection(&pRd->crit);

		if(!dwNumSectors || !pRd->m_bOk)
			return;
		if( !pRd->pCd->ReadCdRomSector( pbtReadBuffer, 				
										pRd->dwReadBufferSize,
										lSector,
										dwNumSectors,
										FALSE ) )
			pRd->m_bOk = FALSE;
	};
};
// end added

BOOL CAspiCD::ReadCdRomSector(
	BYTE*	pbtReadBuffer,
	DWORD	dwReadBufferSize,
	LONG	lSector,
	DWORD	dwNumSectors,
	BOOL	bGetC2

	)
{
	int nCmdSize=10;

	BYTE cmd [12] = {0x00,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	switch (GetReadMethod())
	{
		case READ10:	cmd [0] =0x28; nCmdSize=10; break;	// Read 10 (standard)
		case READC3:	cmd [0] =0xA8; nCmdSize=12; break;	// Read A8_12
		case READNEC:	cmd [0] =0xD4; nCmdSize=10; break;	// Read Nec Specific
		case READC1:	cmd [0] =0xD4; nCmdSize=12; break;	// Read D4_12
		case READC2:	cmd [0] =0xD5; nCmdSize=10; break;	// Read D5_10
		case READSONY:	cmd [0] =0xD8; nCmdSize=12; break;	// Read Sony
		case READMMC:	cmd [0] =0xBE; nCmdSize=12; break;	// Read MMC
		case READMMC2:	cmd [0] =0xBE; nCmdSize=12; break;	// Read MMC12
		case READMMC3:	cmd [0] =0xBE; nCmdSize=12; break;	// Read MMC12
		case READMMC4:	cmd [0] =0xBE; nCmdSize=12; break;	// Read MMC12
	}

	cmd [1] = GetLunID()<<5;							// Set LUN
	cmd [2] = (BYTE)(lSector >> 24);				// Set Start Sector 
	cmd [3] = (BYTE)((lSector >> 16) & 0xFF);		// Set Start Sector
	cmd [4] = (BYTE)((lSector >> 8) & 0xFF);		// Set Start Sector
	cmd [5] = (BYTE)(lSector & 0xFF);				// Set Start Sector

	switch (GetReadMethod())
	{
		case READ10:	
			cmd[7]=(BYTE)((dwNumSectors >>8) & 0xFF);	// Number of sectors to read
			cmd[8]=(BYTE) (dwNumSectors & 0xFF);		// Number of sectors to read
		break;	// Read 10

		case READC3:	
			cmd[8]=(BYTE)((dwNumSectors >>8) & 0xFF);	// Number of sectors to read
			cmd[9]=(BYTE) (dwNumSectors & 0xFF);		// Number of sectors to read
		break;	// Read A8_12

		case READNEC:
			cmd[7]=(BYTE)((dwNumSectors >>8) & 0xFF);	// Number of sectors to read
			cmd[8]=(BYTE) (dwNumSectors & 0xFF);		// Number of sectors to read
		break;	// Read Nec Specific

		case READC1:	
			cmd[8]=(BYTE)((dwNumSectors >>8) & 0xFF);	// Number of sectors to read
			cmd[9]=(BYTE) (dwNumSectors & 0xFF);		// Number of sectors to read
		break;	// Read D4_12

		case READC2:	
			cmd[7]=(BYTE)((dwNumSectors >>8) & 0xFF);	// Number of sectors to read
			cmd[8]=(BYTE) (dwNumSectors & 0xFF);		// Number of sectors to read
		break;	// Read D5_10

		case READSONY:	
			cmd[8]=(BYTE)((dwNumSectors >>8) & 0xFF);	// Number of sectors to read
			cmd[9]=(BYTE) (dwNumSectors & 0xFF);		// Number of sectors to read
		break;	// Read Read Sony

		case READMMC3:	
			cmd[7]=(BYTE)((dwNumSectors >>8) & 0xFF);	// Number of sectors to read
			cmd[8]=(BYTE) (dwNumSectors & 0xFF);		// Number of sectors to read
			cmd[9]=0x10;								// Set vendor specific byte
		break;	// Read MMC

		case READMMC:	
			cmd[7]=(BYTE)((dwNumSectors >>8) & 0xFF);	// Number of sectors to read
			cmd[8]=(BYTE) (dwNumSectors & 0xFF);		// Number of sectors to read
			cmd[9]=0xF8; // Set vendor specific byte
		break;	// Read MMC3

		case READMMC2:	
			cmd[1]|=0x04;
			cmd[7]=(BYTE)((dwNumSectors >>8) & 0xFF);	// Number of sectors to read
			cmd[8]=(BYTE) (dwNumSectors & 0xFF);		// Number of sectors to read
			cmd[9]=0xF8;								// Set vendor specific byte
		break;	// Read MMC
		case READMMC4:	
			cmd[1]|=0x04;
			cmd[7]=(BYTE)((dwNumSectors >>8) & 0xFF);	// Number of sectors to read
			cmd[8]=(BYTE) (dwNumSectors & 0xFF);		// Number of sectors to read
			cmd[9]=0x10;								// Set vendor specific byte
		break;	// Read MMC
	}

	if ( TRUE == bGetC2 ) 
	{
		/* + EDC/ECC + C2 */
		cmd[ 9 ] |= 0x02;
	}

	// Clear old memory buffer
	memset( pbtReadBuffer, 0x00, dwReadBufferSize );

	// Issue SCSI command
	if ( SS_COMP != IssueScsiCmd(	SRB_DIR_IN,
									cmd,
									nCmdSize,
									pbtReadBuffer,
									dwReadBufferSize )  )
	{
		return FALSE;
	}


	if ( TRUE == bGetC2 )
	{
		DWORD dwErrors = 0;
		DWORD pdwErrorSectors[ 27 ]; // FIXME

		CDEX_ERR bErr = ExtractC2ErrorInfo(	pbtReadBuffer, 
											lSector,
											dwNumSectors,
											dwErrors,
											pdwErrorSectors );

		if ( dwErrors > 0 )
		{
			OutputDebugString( "C2 error detected \n" );
		}
	}

	// Do byte swapping if necessary
	if ( GetEndian() == BIGENDIAN )
	{
		// Get pointer to read buffer
		BYTE* pbtBuffer=pbtReadBuffer;

		// Loop trhough the entire read buffer and swap the bytes
		for ( DWORD i=0;i<dwNumSectors * CB_CDDASECTORSIZE ; i += 2 )
		{
			BYTE bTmp = pbtBuffer[1];
			pbtBuffer[1] = pbtBuffer[0];
			pbtBuffer[0] = bTmp;
			pbtBuffer += 2;
		}
	}

	// Write out the last bits and pieces
	if ( GetSwapLefRightChannel() )
	{
		// Get pointer to read buffer
		short* psBuffer=(short*)pbtReadBuffer;

		// Loop trhough the entire read buffer and swap the bytes
		for ( DWORD i = 0; i < dwNumSectors * CB_CDDASECTORSIZE / 2; i += 2 )
		{
			short sTmp = *(psBuffer+1);
			*(psBuffer+1) = *psBuffer;
			*(psBuffer) = sTmp;
			psBuffer += 2;
		}
	}

	return TRUE;
}

CDEX_ERR CAspiCD::GetStatus()
{
	CDEX_ERR bReturn = CDEX_OK;

	DWORD ASPIStatus;

    ASPIStatus = GetASPI32SupportInfo();

	m_btLastError = HIBYTE( LOWORD( ASPIStatus ) );

    switch( HIBYTE( LOWORD( ASPIStatus ) ) )
    {
        case SS_COMP:
            /* ASPI for Win32 is properly initialized */
            m_bNumHA = LOBYTE( LOWORD( ASPIStatus ) );
		break;

        default:
            /* Somhow got problems with the installed ASPI drivers */
			char lpszError[ 255 ];
			GetAspiError( m_btLastError, lpszError );

			bReturn = CDEX_FAILEDTOGETASPISTATUS;
    }

	return bReturn;
}


CDEX_ERR CAspiCD::ScanForC2Errors(	DWORD	dwStartSector,
									DWORD	dwNumSectors,
									DWORD&	dwErrors,
									DWORD*	pdwErrorSectors )
{
	DWORD	dwDataBufferSize = dwNumSectors * ( CB_CDDASECTORSIZE + CB_CDDASECTORSIZE / 8 );
	BYTE*	pDataBuf = new BYTE [dwDataBufferSize];
	DWORD	dwSample = 0;
	DWORD	dwSector = 0;
	int	nCmdSize = 12;

	// setup SCSI command package
	BYTE cmd [12] = { 0xBE,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	cmd[ 1 ] = GetLunID()<<5;								// Set LUN
	cmd[ 2 ] = (BYTE)((dwStartSector >> 24 ) & 0xFF );		// Set Start Sector 
	cmd[ 3 ] = (BYTE)((dwStartSector >> 16 ) & 0xFF );		// Set Start Sector
	cmd[ 4 ] = (BYTE)((dwStartSector >>  8 ) & 0xFF );		// Set Start Sector
	cmd[ 5 ] = (BYTE)( dwStartSector & 0xFF );				// Set Start Sector

	cmd[ 6 ] = (BYTE)(( dwNumSectors >> 16) & 0xFF );		// Number of sectors to read
	cmd[ 7 ] = (BYTE)(( dwNumSectors >> 8 ) & 0xFF );		// Number of sectors to read
	cmd[ 8 ] = (BYTE)(  dwNumSectors & 0xFF        );		// Number of sectors to read

	cmd[ 9 ] = 0x12;

		/* Sync + all headers + user data + EDC/ECC + C2 */
//		cmd[ 9 ] = (1 << 7 | 3 << 5 | 1 << 4 | 1 << 3 | 1 << 1);


	// Issue SCSI command
	IssueScsiCmd( SRB_DIR_IN, cmd, nCmdSize, pDataBuf, dwDataBufferSize );

	// check if all data in subchannel is zero
	for ( dwSector  = 0; dwSector < dwNumSectors; dwSector++ )
	{
		// get a pointer to start of C2 error info bits
		BYTE* pC2ErrorBits = &pDataBuf[ ( CB_CDDASECTORSIZE + CB_CDDASECTORSIZE / 8 ) * dwSector + CB_CDDASECTORSIZE ];

		// initialize the number of errors in this sector
		pdwErrorSectors[ dwSector ] = 0;

		// test all C2 error bits for this sector
		for ( dwSample = 0; dwSample< ( CB_CDDASECTORSIZE / 8 ); dwSample++ )
		{
			//  
			if ( 0 != pC2ErrorBits[ dwSample ] )
			{
				pdwErrorSectors[ dwSector ]++;
				dwErrors++;
			}		
		}
	}

	delete [] pDataBuf;

	LogSenseData();

	return CDEX_OK;
}

void CAspiCD::LogSenseData()
{
}

CDEX_ERR CAspiCD::Init()
{
	m_bUseNtScsi = ( TRANSPLAYER_NTSCSI == CR_GetTransportLayer() );

	CDEX_ERR bReturn = InitAspiDll( m_bUseNtScsi );


	// Load the ASPI dll;
	if ( CDEX_OK == bReturn ) 
	{
		m_bAvailable = TRUE;
	}

	return bReturn;
}

CDEX_ERR CAspiCD::ExtractC2ErrorInfo(	BYTE*  pData, 
										DWORD  dwStartSector,
										DWORD  dwNumSectors,
										DWORD& dwErrors,
										DWORD* pdwErrorSectors )
{
	CDEX_ERR	bReturn = CDEX_OK;
	DWORD		dwSector = 0;
	DWORD		dwSample = 0;

	// check if all data in subchannel is zero
	for ( dwSector  = 0; dwSector < dwNumSectors; dwSector++ )
	{
		// get a pointer to start of C2 error info bits
		BYTE* pC2ErrorBits = &pData[ ( CB_CDDASECTORSIZE + CB_CDDASECTORSIZE / 8 ) * dwSector + CB_CDDASECTORSIZE ];

		// initialize the number of errors in this sector
		pdwErrorSectors[ dwSector ] = 0;

		// test all C2 error bits for this sector
		for ( dwSample = 0; dwSample< ( CB_CDDASECTORSIZE / 8 ); dwSample++ )
		{
			//  
			if ( 0 != pC2ErrorBits[ dwSample ] )
			{
				pdwErrorSectors[ dwSector ]++;
				dwErrors++;
			}		
		}
	}
	for ( dwSector  = 1; dwSector < dwNumSectors; dwSector++ )
	{
		// remove the C2 error bits
		(void)memcpy(	&pData[ dwSector * CB_CDDASECTORSIZE ],
						&pData[ dwSector * ( CB_CDDASECTORSIZE + CB_CDDASECTORSIZE / 8 ) ],
						( CB_CDDASECTORSIZE + CB_CDDASECTORSIZE / 8 ) );
	}

	return bReturn;
}


CDEX_ERR CAspiCD::GetDetailedDriveInfo( 
	LPSTR lpszInfo, 
	DWORD dwInfoSize )
{
	CDEX_ERR	bReturn = CDEX_OK;
	BOOL		bIsMMC = FALSE;
	BYTE		pbtBuffer[ 0xFF ];
	CHAR		lpszLine[ 255 ] = {'\0',};
	CHAR		lpszTemp[ 255 ] = {'\0',};

	BYTE btAdapterID = GetAdapterID();
	BYTE btTargetID = GetTargetID();
	BYTE btLunID = GetLunID();

	// clear return string
	memset( lpszInfo, 0, dwInfoSize );

	// set drive info
	sprintf( lpszLine, "Detailed drive info Ha:%02X Id: %02X Lun:%02X \r\n",
				btAdapterID,
				btTargetID,
				btLunID );

	if ( strlen( lpszInfo ) < dwInfoSize + strlen( lpszLine ) )
	{
		strcat( lpszInfo, lpszLine );
	}

	GetDeviceName( btAdapterID, btTargetID,btLunID, lpszTemp );

	// set device name
	sprintf( lpszLine, "Device Name: \"%s\"\r\n", lpszTemp );

	if ( strlen( lpszInfo ) < dwInfoSize + strlen( lpszLine ) )
	{
		strcat( lpszInfo, lpszLine );
	}
	
	// Clear buffer
	memset( pbtBuffer, 0x00, sizeof( pbtBuffer ) );

	ModeSense( 0x2A, pbtBuffer, sizeof( pbtBuffer ) );

	SCISMODEHEADER* pBlockHeader=(SCISMODEHEADER*)pbtBuffer;
	SCSICDMODEPAGE2A* pPage=(SCSICDMODEPAGE2A*)( &pbtBuffer[ 4 ] );

	int n2aSize=sizeof(SCSICDMODEPAGE2A);
	int nBlockSize=sizeof(SCISMODEHEADER);

    if( pPage->p_code == 0x2A )
	{
		// Check the size of the page, in order to determine that we deal
		// with a MMC capable drive
		if( pPage->p_len >= 4 )
		{
			bIsMMC = TRUE;

			if( pPage->cd_da_supported )
			{
				strcpy( lpszLine,"Drive is MMC style\r\n");
			}
		}
		else
		{
			strcpy( lpszLine,"MMC drive, but reports CDDA incapable\r\n");
		}
    }
	else
	{
		strcpy( lpszLine,"Drive does not support MMC\r\n");
	}

	if ( strlen( lpszInfo ) < dwInfoSize + strlen( lpszLine ) )
	{
		strcat( lpszInfo, lpszLine );
	}

	if ( bIsMMC )
	{
		sprintf( lpszLine, "Read CD-RW=%d\r\nRead CD-R=%d\r\nSupport Method2=%d\r\nWrite CD-RW=%d\r\nWrite CD-R=%d\r\nTest Write=%d\r\n",
					pPage->cd_rw_read,
					pPage->cd_r_read,
					pPage->method2,
					pPage->cd_rw_write,
					pPage->cd_r_write,
					pPage->test_write );

		if ( strlen( lpszInfo ) < dwInfoSize + strlen( lpszLine ) )
			lpszInfo = strcat( lpszInfo, lpszLine );
		
		sprintf( lpszLine, "CDDA support=%d\r\nCDDA stream accurate=%d\r\n",
					pPage->cd_da_supported,
					pPage->cd_da_accurate );

		if ( strlen( lpszInfo ) < dwInfoSize + strlen( lpszLine ) )
			lpszInfo = strcat( lpszInfo, lpszLine );

		sprintf( lpszLine, "Current read speed=%d [kBps]\r\nMax read speed=%d [kBps]\r\nCurrent write speed=%d [kBps]\r\nMax write speed=%d [kBps]\r\n",
					pPage->cur_read_speed[0] * 255 + pPage->cur_read_speed[1],
					pPage->max_read_speed[0] * 255 + pPage->max_read_speed[1],
					pPage->cur_write_speed[0] * 255 + pPage->cur_write_speed[1],
					pPage->max_write_speed[0] * 255 + pPage->max_write_speed[1]
					);

		if ( strlen( lpszInfo ) < dwInfoSize + strlen( lpszLine ) )
			lpszInfo = strcat( lpszInfo, lpszLine );
	
		sprintf( lpszLine, "Supported buffer size= %d [kB]\r\n",
					pPage->buffer_size[0] * 255 + pPage->buffer_size[1] );

		if ( strlen( lpszInfo ) < dwInfoSize + strlen( lpszLine ) )
			lpszInfo = strcat( lpszInfo, lpszLine );
	}


	return bReturn;
}
