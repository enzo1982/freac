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
	m_btaFlags[m_nNumTracks]	= btStream[1];
	m_btaTrackNumber[m_nNumTracks]	= btStream[2];

	memcpy(&m_dwaStartSector[m_nNumTracks], &btStream[4], sizeof(DWORD));

	// Swap DWORD order
	Swap(&m_dwaStartSector[m_nNumTracks], 4);

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
	m_nNumTracks = 0;

	memset(m_dwaStartSector, 0x00, sizeof(m_dwaStartSector));
	memset(m_btaFlags, 0x00, sizeof(m_btaFlags));
	memset(m_btaTrackNumber, 0x00, sizeof(m_btaTrackNumber));
}

// DESTRUCTOR
CToc::~CToc()
{
}

BOOL CToc::IsAudioTrack(int p_track)
{
	if (!(m_btaFlags[p_track] & CDROMDATAFLAG)) return TRUE;

	return FALSE;
}

DWORD CToc::GetSize(int nTrack)
{
	DWORD	 dwSectors = GetStartSector(nTrack + 1) - GetStartSector(nTrack);

	return dwSectors * CB_CDDASECTORSIZE;
}

CDSETTINGSPARAMS::CDSETTINGSPARAMS()
{
	strcpy(lpszCDROMID, "DONTKNOW");

	nOffsetStart		= 0;
	nOffsetEnd		= 0;	
	nSpeed			= 0;
	nSpinUpTime		= 0;
	bJitterCorrection	= 1;
	bSwapLefRightChannel	= 0;
	nNumOverlapSectors	= 7;
	nNumReadSectors		= 26;	
	nNumCompareSectors	= 1;	
	nMultiReadCount		= 0;
	bMultiReadFirstOnly	= FALSE;
	bLockDuringRead		= TRUE;
	bUseCDText		= TRUE;

	btTargetID		= 0;
	btAdapterID		= 0;
	btLunID			= 0;
	nAspiRetries		= 0;
	nAspiTimeOut		= 3000;

	DriveTable.DriveType	= GENERIC;
	DriveTable.ReadMethod	= READMMC;
	DriveTable.SetSpeed	= SPEEDMMC;
	DriveTable.Endian	= MY_LITTLEENDIAN;
	DriveTable.EnableMode	= ENABLENONE;
	DriveTable.nDensity	= 0;
	DriveTable.bAtapi	= TRUE;

	bAspiPosting		= FALSE;
	nRippingMode		= 0;
	nParanoiaMode		= 0;
}

CDSETTINGSPARAMS::~CDSETTINGSPARAMS()
{
}

CDRomSettings::CDRomSettings()
{
	memset(m_lpszIniFname, 0x00, sizeof(m_lpszIniFname));

	m_nTransportLayer = TRANSPLAYER_ASPI;
	m_nActive = 0;
}

CDRomSettings::~CDRomSettings()
{
}

void CDRomSettings::AddCDRom( LPSTR lpszName, BYTE btAdapterID, BYTE btTargetID, BYTE btLunID )
{
	int nDeviceOffset = 0;

	// Loop through Array and see if there are any cd rom with the same name
	for (int i = 0; i < m_CDParams.size(); i++)
	{
		// Check if name to add is equal to current indexed CDROM setting
		if (strstr(m_CDParams[i].lpszCDROMID, lpszName) != NULL)
		{
			// Increase number
			nDeviceOffset++;
		}
	}

	// There appears to be CDROMs with the same name
	if (nDeviceOffset > 0)
	{
		char lpszDevNum[4];

		// Add (nDeviceOffset) to strName
		sprintf(lpszDevNum, "(%d)", nDeviceOffset);

		lpszName = strcat(lpszName, lpszDevNum);
	}
	
	// Create new CDROM setting
	CDSETTINGSPARAMS newSettings;

	// Set device name
	strcpy(newSettings.lpszCDROMID, lpszName);

	newSettings.btTargetID = btTargetID;
	newSettings.btAdapterID = btAdapterID;
	newSettings.btLunID = btLunID;

	// Add to array of CDROM settings
	m_CDParams.push_back(newSettings);

	// Also add a new Table of contents
	m_Toc.push_back(CToc());
}

void CDRomSettings::LoadSettings(BOOL bUpdateDriveSettings)
{
	// Make sure selection is valid

	m_nActive = min(max(0, m_nActive), m_CDParams.size() - 1);
	m_nTransportLayer = 0;

	if (bUpdateDriveSettings)
	{
		// Get default values when not a CUSTOM drive type
		UpdateDriveSettings();
	}
}

void CDRomSettings::UpdateDriveSettings()
{
	if (m_CDParams[m_nActive].DriveTable.DriveType != CUSTOMDRIVE)
	{
		// Set defaults	(SONY)
		m_CDParams[m_nActive].DriveTable.SetSpeed	= SPEEDNONE;
		m_CDParams[m_nActive].DriveTable.Endian		= MY_LITTLEENDIAN;
		m_CDParams[m_nActive].DriveTable.nDensity	= 0x00;
		m_CDParams[m_nActive].DriveTable.EnableMode	= ENABLENONE;
		m_CDParams[m_nActive].DriveTable.bAtapi		= FALSE;
	}

	// Setup the specific drive properties
	switch (m_CDParams[m_nActive].DriveTable.DriveType)
	{
		case GENERIC:	
			m_CDParams[m_nActive].DriveTable.ReadMethod	= READMMC;
			m_CDParams[m_nActive].DriveTable.SetSpeed	= SPEEDMMC;
			m_CDParams[m_nActive].DriveTable.bAtapi		= TRUE;

			break;
		case TOSHIBANEW:	
			m_CDParams[m_nActive].DriveTable.ReadMethod	= READ10;
			m_CDParams[m_nActive].DriveTable.SetSpeed	= SPEEDTOSHIBA;
			m_CDParams[m_nActive].DriveTable.nDensity	= 0x82;
			m_CDParams[m_nActive].DriveTable.EnableMode	= ENABLESTD;

			break;
		case TOSHIBA:	
		case IBM:	
		case DEC:	
			m_CDParams[m_nActive].DriveTable.ReadMethod	= READ10;
			m_CDParams[m_nActive].DriveTable.SetSpeed	= SPEEDNONE;
			m_CDParams[m_nActive].DriveTable.nDensity	= 0x82;
			m_CDParams[m_nActive].DriveTable.EnableMode	= ENABLESTD;

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
			m_CDParams[m_nActive].DriveTable.ReadMethod	= READ10;
			m_CDParams[m_nActive].DriveTable.SetSpeed	= SPEEDPHILIPS;
			m_CDParams[m_nActive].DriveTable.EnableMode	= ENABLESTD;
			m_CDParams[m_nActive].DriveTable.Endian		= MY_BIGENDIAN;

			break;
		case NRC:
			m_CDParams[m_nActive].DriveTable.SetSpeed	= SPEEDNONE;

			break;
		case YAMAHA:
			m_CDParams[m_nActive].DriveTable.EnableMode	= ENABLESTD;
			m_CDParams[m_nActive].DriveTable.SetSpeed	= SPEEDYAMAHA;

			break;
		case SONY:
		case PLEXTOR:	// beware of type CD-ROM CDU55E can use MMC reading
			m_CDParams[m_nActive].DriveTable.ReadMethod	= READSONY;
			m_CDParams[m_nActive].DriveTable.SetSpeed	= SPEEDSONY;

			break;
		case NEC:
			m_CDParams[m_nActive].DriveTable.ReadMethod	= READNEC;
			m_CDParams[m_nActive].DriveTable.SetSpeed	= SPEEDNEC;

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

void CDRomSettings::SetTransportLayer(int nValue)
{
	m_nTransportLayer = nValue;
}

void CDRomSettings::SetIniFileName(LPCSTR lpszIniFname)
{
	if (lpszIniFname == NULL) return;

	strcpy(m_lpszIniFname, lpszIniFname);
}

void CDRomSettings::SetActiveCDROM(BYTE nValue)
{
	m_nActive = nValue;
}
