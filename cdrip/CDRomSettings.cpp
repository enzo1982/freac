/*
** Copyright (C) 1999 Albert L. Faber
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

#include "CDRomSettings.h"
#include "Ini.h"
#include "Aspi.h"
#include <math.h>

#ifndef min
	#define min(x,y) ((x)>(y)?(y):(x))
#endif

#ifndef max
	#define max(x,y) ((x)<(y)?(y):(x))
#endif

void CToc::FillEntry(BYTE* btStream)
{

//	m_reserved1=btStream[0];
	m_btaFlags[m_nNumTracks]=btStream[1];
	m_btaTrackNumber[m_nNumTracks]=btStream[2];
//	m_reserved2=btStream[3];
	memcpy(&m_dwaStartSector[m_nNumTracks],&btStream[4],sizeof(DWORD));

	// Swap DWORD order
    Swap (&m_dwaStartSector[m_nNumTracks], 4);

	// Increase the number of total tracks
	m_nNumTracks++;
}

// CONSTRUCTOR
CToc::CToc()
{
	// Initialize variables
	Clear();
}


void CToc::Clear()
{
	m_nNumTracks=0;
	memset(m_dwaStartSector,0x00,sizeof(m_dwaStartSector));
	memset(m_btaFlags,0x00,sizeof(m_btaFlags));
	memset(m_btaTrackNumber,0x00,sizeof(m_btaTrackNumber));
}


// DESTRUCTOR
CToc::~CToc()
{
}



void CToc::DumpToc(LPBYTE pbtBuffer,int nSize)
{
/*
	int			i;
	DWORD		dw;
	unsigned	mins;
	double		secnds;
	extern double fmod(double,double);


	// Open Dump File
	CADebug myLogFile(CString("C:\\CDEX_TOC.txt"));

	// A header would not hurt
	myLogFile.printf("Start of TOC dump\n\n");

	// Dump binary data
	myLogFile.printf("Dumping binary data first, buffer size=%d =>%d toc entries\n\n",nSize,nSize/8);
	
	myLogFile.DumpBinaray(pbtBuffer,nSize);

	dw = GetStartSector(m_wNumTracks-1);

	// Calculate number of minutes
	mins= dw / (TRACKSPERSEC*60L);

	secnds= (fmod(dw , 60*TRACKSPERSEC) / (DOUBLE)TRACKSPERSEC);


	myLogFile.printf("\n\nDumping Toc Entries\n");
	for (i=0; i<m_wNumTracks; i++) 
	{
		myLogFile.printf("Toc Entry=%2d  StartSector=%8d Flags=%2d",i,m_TrackInfo[i].GetStartSector(),m_TrackInfo[i].GetFlags());
		// Skip nonaudio tracks 
//		if (m_TrackInfo[i].GetFlags() != AUDIOTRKFLAG) 
//			continue;					


		if (m_TrackInfo[i].GetTrack() <= MAXTRK) 
		{
			DWORD dw2;

			dw2= m_TrackInfo[i].GetStartSector();

			dw = m_TrackInfo[i+1].GetStartSector();

			mins= (dw - dw2) / (TRACKSPERSEC*60L);
			secnds= fmod(dw-dw2 , 60*TRACKSPERSEC) / TRACKSPERSEC;


			CString strDuration=GetTrackDuration(i+1);
			CString strStart=GetStartTime(i+1);

			CString strNew;
			myLogFile.printf(" => Start time=%s Duration=%s\n",strStart,strDuration);
		}

	}
	myLogFile.printf("\n\nEnd of TOC dump\n");
*/
}

BOOL CToc::IsAudioTrack(int p_track)
{
	if ( !( m_btaFlags[p_track] & CDROMDATAFLAG) )
		return TRUE;
	return FALSE;
}

DWORD CToc::GetSize(int nTrack)
{
	DWORD dwSectors = GetStartSector( nTrack + 1 ) - GetStartSector( nTrack );
	return dwSectors * CB_CDDASECTORSIZE;
}




CDSETTINGSPARAMS::CDSETTINGSPARAMS()
{
	strcpy( lpszCDROMID, "DONTKNOW" );
	nOffsetStart = 0;
	nOffsetEnd = 0;	
	nSpeed = 0;
	nSpinUpTime = 0;
	bJitterCorrection = 1;
	bSwapLefRightChannel = 0;
	nNumOverlapSectors = 7;
	nNumReadSectors = 26;	
	nNumCompareSectors = 1;	
	nMultiReadCount = 0;
	bMultiReadFirstOnly = FALSE;
	bLockDuringRead = TRUE;
	bUseCDText = TRUE;

	btTargetID=0;
	btAdapterID=0;
	btLunID=0;
	nAspiRetries=0;
	nAspiTimeOut=3000;


	DriveTable.DriveType=GENERIC;
	DriveTable.ReadMethod=READMMC;
	DriveTable.SetSpeed=SPEEDMMC;
	DriveTable.Endian=LITTLEENDIAN;
	DriveTable.EnableMode=ENABLENONE;
	DriveTable.nDensity=0;
	DriveTable.bAtapi=TRUE;

	bAspiPosting=FALSE;
	nRippingMode = 0;
	nParanoiaMode = 0;

}

CDSETTINGSPARAMS::~CDSETTINGSPARAMS()
{

}


CDRomSettings::CDRomSettings()
{
	memset( m_lpszIniFname, 0x00, sizeof( m_lpszIniFname ) );
	m_nTransportLayer = TRANSPLAYER_ASPI;
	m_nActive = 0;
}

CDRomSettings::~CDRomSettings()
{
}


void CDRomSettings::AddCDRom( LPSTR lpszName, BYTE btAdapterID, BYTE btTargetID, BYTE btLunID )
{
	int nDeviceOffset=0;


	// Loop through Array and see if there are any cd rom with the same name
	for ( int i=0; i < m_CDParams.size() ; i++ )
	{
		// Check if name to add is equal to current indexed CDROM setting
		if ( strstr(m_CDParams[i].lpszCDROMID,lpszName ) !=NULL )
		{
			// Increase number
			nDeviceOffset++;
		}
	}

	// There appears to be CDROMs with the same name
	if ( nDeviceOffset > 0 )
	{
		char lpszDevNum[4];
		// Add (nDeviceOffset) to strName
		sprintf( lpszDevNum, "(%d)", nDeviceOffset );
		lpszName = strcat( lpszName, lpszDevNum );
	}
	
	// Create new CDROM setting
	CDSETTINGSPARAMS newSettings;

	// Set device name
	strcpy( newSettings.lpszCDROMID,lpszName );
	newSettings.btTargetID = btTargetID;
	newSettings.btAdapterID = btAdapterID;
	newSettings.btLunID = btLunID;

	// Add to array of CDROM settings
	m_CDParams.push_back( newSettings );

	// Get default CD-ROM parameter values ( if available )
	LoadCDSettingsEntry( m_CDParams.back(), "CDROM_DEFAULTS" );

	// Also add a new Table of contents
	m_Toc.push_back( CToc() );

}

void CDRomSettings::SaveSettings()
{
	CIni	myIni;
	CIni*	pIni = &myIni;

	pIni->SetIniFileName( m_lpszIniFname );

	// Loop through Array of CD-ROM settings
	for ( int i=0; i < m_CDParams.size(); i++ )
	{
		char lpszKey[255];

		strcpy(lpszKey,m_CDParams[i].lpszCDROMID);

		// Replace spaces with underscores
		for (int j=0;j<strlen(lpszKey);j++)
			if (lpszKey[j]==' ')
				lpszKey[j]='_';

		pIni->SetValue(lpszKey, "nOffsetStart"			,m_CDParams[i].nOffsetStart );
		pIni->SetValue(lpszKey, "nOffsetEnd"			,m_CDParams[i].nOffsetEnd );
		pIni->SetValue(lpszKey, "nSpeed"				,m_CDParams[i].nSpeed );
		pIni->SetValue(lpszKey, "nSpinUpTime"			,m_CDParams[i].nSpinUpTime );
		pIni->SetValue(lpszKey, "bJitterCorrection"		,m_CDParams[i].bJitterCorrection );
		pIni->SetValue(lpszKey, "bSwapLefRightChannel"	,m_CDParams[i].bSwapLefRightChannel );
		pIni->SetValue(lpszKey, "nNumOverlapSectors"	,m_CDParams[i].nNumOverlapSectors );
		pIni->SetValue(lpszKey, "nNumReadSectors"		,m_CDParams[i].nNumReadSectors );
		pIni->SetValue(lpszKey, "nNumCompareSectors"	,m_CDParams[i].nNumCompareSectors );
		pIni->SetValue(lpszKey, "nAspiTimeOut"			,m_CDParams[i].nAspiTimeOut );
		pIni->SetValue(lpszKey, "nAspiRetries"			,m_CDParams[i].nAspiRetries );
		pIni->SetValue(lpszKey, "bEnableMultiRead"		,m_CDParams[i].bEnableMultiRead );
		pIni->SetValue(lpszKey, "bMultiReadFirstOnly"	,m_CDParams[i].bMultiReadFirstOnly );
		pIni->SetValue(lpszKey, "nMultiReadCount"		,m_CDParams[i].nMultiReadCount );
		pIni->SetValue(lpszKey, "bLockDuringRead"		,m_CDParams[i].bLockDuringRead );
		pIni->SetValue(lpszKey, "bUseCDText"		,m_CDParams[i].bUseCDText );

		// Write Drive Settings
		pIni->SetValue(lpszKey, "DriveType"				,m_CDParams[i].DriveTable.DriveType );
		pIni->SetValue(lpszKey, "ReadMethod"			,m_CDParams[i].DriveTable.ReadMethod );
		pIni->SetValue(lpszKey, "SetSpeed"				,m_CDParams[i].DriveTable.SetSpeed );
		pIni->SetValue(lpszKey, "EnableMode"			,m_CDParams[i].DriveTable.EnableMode );
		pIni->SetValue(lpszKey, "nDensity"				,m_CDParams[i].DriveTable.nDensity );
		pIni->SetValue(lpszKey, "Endian"				,m_CDParams[i].DriveTable.Endian );
		pIni->SetValue(lpszKey, "bAtapi"				,m_CDParams[i].DriveTable.bAtapi );
		pIni->SetValue(lpszKey, "bAspiPosting"			,m_CDParams[i].bAspiPosting );
		pIni->SetValue(lpszKey, "nParanoiaMode"			,m_CDParams[i].nParanoiaMode );
		pIni->SetValue(lpszKey, "nRippingMode"			,m_CDParams[i].nRippingMode );

	}

	// Save active CD-ROM setting
	pIni->SetValue( "CD-ROM","nActive"	,m_nActive );

	pIni->SetValue( "CD-ROM","nTransportLayer",(int)m_nTransportLayer );

}

void CDRomSettings::LoadCDSettingsEntry( CDSETTINGSPARAMS& cdSettings, const char* lpszKey )
{
	CIni	myIni;

	myIni.SetIniFileName( m_lpszIniFname );

	cdSettings.nOffsetStart			= myIni.GetValue(lpszKey,"nOffsetStart"			,cdSettings.nOffsetStart );
	cdSettings.nOffsetEnd			= myIni.GetValue(lpszKey,"nOffsetEnd"			,cdSettings.nOffsetEnd );
	cdSettings.nSpeed				= myIni.GetValue(lpszKey,"nSpeed"				,cdSettings.nSpeed );
	cdSettings.nSpinUpTime			= myIni.GetValue(lpszKey,"nSpinUpTime"			,cdSettings.nSpinUpTime );
	cdSettings.bJitterCorrection	= myIni.GetValue(lpszKey,"bJitterCorrection"	,cdSettings.bJitterCorrection );
	cdSettings.bSwapLefRightChannel	= myIni.GetValue(lpszKey,"bSwapLefRightChannel" ,cdSettings.bSwapLefRightChannel );
	cdSettings.nNumOverlapSectors	= myIni.GetValue(lpszKey,"nNumOverlapSectors"	,cdSettings.nNumOverlapSectors );
	cdSettings.nNumReadSectors		= myIni.GetValue(lpszKey,"nNumReadSectors"		,cdSettings.nNumReadSectors );
	cdSettings.nNumCompareSectors	= myIni.GetValue(lpszKey,"nNumCompareSectors"	,cdSettings.nNumCompareSectors );
	cdSettings.nAspiTimeOut			= myIni.GetValue(lpszKey,"nAspiTimeOut"			,cdSettings.nAspiTimeOut );
	cdSettings.nAspiRetries			= myIni.GetValue(lpszKey,"nAspiRetries"			,cdSettings.nAspiRetries );
	cdSettings.bEnableMultiRead		= myIni.GetValue(lpszKey,"bEnableMultiRead"		,cdSettings.bEnableMultiRead );
	cdSettings.bMultiReadFirstOnly	= myIni.GetValue(lpszKey,"bMultiReadFirstOnly"	,cdSettings.bMultiReadFirstOnly );
	cdSettings.nMultiReadCount		= myIni.GetValue(lpszKey,"nMultiReadCount"		,cdSettings.nMultiReadCount );
	cdSettings.bLockDuringRead		= myIni.GetValue(lpszKey,"bLockDuringRead"		,cdSettings.bLockDuringRead );
	cdSettings.bUseCDText			= myIni.GetValue(lpszKey,"bUseCDText"			,cdSettings.bUseCDText );

	// Custom Drive Settings
	cdSettings.DriveTable.DriveType		= DRIVETYPE(myIni.GetValue(lpszKey,"DriveType",cdSettings.DriveTable.DriveType ));
	cdSettings.DriveTable.ReadMethod	= READMETHOD(myIni.GetValue(lpszKey,"ReadMethod",cdSettings.DriveTable.ReadMethod ));
	cdSettings.DriveTable.SetSpeed		= SETSPEED(myIni.GetValue(lpszKey,"SetSpeed",cdSettings.DriveTable.SetSpeed ));
	cdSettings.DriveTable.EnableMode	= ENABLEMODE(myIni.GetValue(lpszKey,"EnableMode",cdSettings.DriveTable.EnableMode ));
	cdSettings.DriveTable.nDensity		= myIni.GetValue(lpszKey,"nDensity",cdSettings.DriveTable.nDensity );
	cdSettings.DriveTable.Endian		= ENDIAN(myIni.GetValue(lpszKey,"Endian",cdSettings.DriveTable.Endian ));
	cdSettings.DriveTable.bAtapi		= myIni.GetValue(lpszKey,"bAtapi",cdSettings.DriveTable.bAtapi );
	cdSettings.bAspiPosting				= myIni.GetValue(lpszKey,"bAspiPosting"		,cdSettings.bAspiPosting );
	cdSettings.nRippingMode				= myIni.GetValue(lpszKey,"nRippingMode"		,cdSettings.nRippingMode );
	cdSettings.nParanoiaMode			= myIni.GetValue(lpszKey,"nParanoiaMode"	,cdSettings.nParanoiaMode );
}

void CDRomSettings::LoadSettings( BOOL bUpdateDriveSettings )
{
	CIni	myIni;
	CIni*	pIni = NULL;

	pIni = &myIni;

	pIni->SetIniFileName( m_lpszIniFname );

	// Loop through Array of CD-ROM settings
	for (int i=0;i<m_CDParams.size();i++)
	{
		char lpszKey[255];
		
		strcpy(lpszKey,m_CDParams[i].lpszCDROMID);

		// Replace spaces with underscores
		for (int j=0;j<strlen(lpszKey);j++)
		{
			if (lpszKey[j]==' ')
			{
				lpszKey[j]='_';
			}
		}

		LoadCDSettingsEntry( m_CDParams[i], lpszKey );

	}
	// Load active CD-ROM setting
	m_nActive = pIni->GetValue("CD-ROM","nActive",m_nActive );

	// Make sure selection if valid
	m_nActive=min(max(0,m_nActive),m_CDParams.size()-1);
	m_nTransportLayer = pIni->GetValue( "CD-ROM", "nTransportLayer", 0 );


	if ( bUpdateDriveSettings )
	{
		// Get default values when not a CUSTOM drive type
		UpdateDriveSettings();
	}
}


void CDRomSettings::UpdateDriveSettings()
{
	if (m_CDParams[m_nActive].DriveTable.DriveType!=CUSTOMDRIVE)
	{
		// Set defaults	(SONY)
		m_CDParams[m_nActive].DriveTable.SetSpeed	=SPEEDNONE;
		m_CDParams[m_nActive].DriveTable.Endian		=LITTLEENDIAN;
		m_CDParams[m_nActive].DriveTable.nDensity	=0x00;
		m_CDParams[m_nActive].DriveTable.EnableMode	=ENABLENONE;
		m_CDParams[m_nActive].DriveTable.bAtapi		=FALSE;
	}

	// Setup the specific drive properties
	switch (m_CDParams[m_nActive].DriveTable.DriveType)
	{
		case GENERIC:	
			m_CDParams[m_nActive].DriveTable.ReadMethod	=READMMC;
			m_CDParams[m_nActive].DriveTable.SetSpeed	=SPEEDMMC;
			m_CDParams[m_nActive].DriveTable.bAtapi		=TRUE;
		break;

		case TOSHIBANEW:	
			m_CDParams[m_nActive].DriveTable.ReadMethod	=READ10;
			m_CDParams[m_nActive].DriveTable.SetSpeed	=SPEEDTOSHIBA;
			m_CDParams[m_nActive].DriveTable.nDensity	=0x82;
			m_CDParams[m_nActive].DriveTable.EnableMode	=ENABLESTD;
		break;
		case TOSHIBA:	
		case IBM:	
		case DEC:	
			m_CDParams[m_nActive].DriveTable.ReadMethod	=READ10;
			m_CDParams[m_nActive].DriveTable.SetSpeed	=SPEEDNONE;
			m_CDParams[m_nActive].DriveTable.nDensity	=0x82;
			m_CDParams[m_nActive].DriveTable.EnableMode	=ENABLESTD;
		break;
		case IMSCDD5:	//ReadTocCdrSCSI : ReadTocSCSI; not yet implemented yet
		case IMS:	
		case KODAK:	
		case RICOH:	
		case HP:	
		case PHILIPS:	
		case PLASMON:	
		case GRUNDIGCDR100IPW:
		case MITSUMICDR:
			m_CDParams[m_nActive].DriveTable.ReadMethod	=READ10;
			m_CDParams[m_nActive].DriveTable.SetSpeed	=SPEEDPHILIPS;
			m_CDParams[m_nActive].DriveTable.EnableMode	=ENABLESTD;
			m_CDParams[m_nActive].DriveTable.Endian		=BIGENDIAN;
		break;
		case NRC:
			m_CDParams[m_nActive].DriveTable.SetSpeed	=SPEEDNONE;
		break;
		case YAMAHA:
			m_CDParams[m_nActive].DriveTable.EnableMode	=ENABLESTD;
			m_CDParams[m_nActive].DriveTable.SetSpeed	=SPEEDYAMAHA;
		break;
		case SONY:
		case PLEXTOR:			// beware of type CD-ROM CDU55E can use MMC reading
			m_CDParams[m_nActive].DriveTable.ReadMethod	=READSONY;
			m_CDParams[m_nActive].DriveTable.SetSpeed	=SPEEDSONY;
		break;
		case NEC:
			m_CDParams[m_nActive].DriveTable.ReadMethod	=READNEC;
			m_CDParams[m_nActive].DriveTable.SetSpeed	=SPEEDNEC;
		break;
		case CUSTOMDRIVE:
		break;
		default:
			break;
	}
}





int CDRomSettings::GetTransportLayer()
{
	return m_nTransportLayer;
}

void CDRomSettings::SetTransportLayer( int nValue )
{
	m_nTransportLayer = nValue;
}

void CDRomSettings::SetIniFileName( LPCSTR lpszIniFname)
{
	if (lpszIniFname == NIL) return;

	strcpy( m_lpszIniFname, lpszIniFname );
}


void CDRomSettings::SetActiveCDROM( BYTE nValue )
{
	m_nActive = nValue;
}
