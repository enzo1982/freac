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

#include <math.h>

#include "CDRip.h"
#include "CDExtract.h"

CCDExtract *pExtract = NULL;

// Get the DLL version number
DLLEXPORT LONG CR_GetCDRipVersion()
{
	return 119;
}

DLLEXPORT CDEX_ERR CR_DeInit()
{
	CDEX_ERR bReturn = CDEX_OK;

	if ( NULL != pExtract )
	{
		pExtract->Clear();
		delete pExtract;
		pExtract = NULL;
	}

	(void)DeInitAspiDll( );

	return bReturn;
}

DLLEXPORT CDEX_ERR CR_Init(int nTransportLayer)
{
	CDEX_ERR bReturn = CDEX_OK;

	CR_DeInit();

	// create new extract model
	pExtract = new CCDExtract;

	if (pExtract != NULL)
	{
		pExtract->SetTransportLayer(nTransportLayer);

		bReturn = pExtract->Init();
	}
	else
	{
		bReturn = CDEX_OUTOFMEMORY;
	}

	// Check if low level CD-ROM drivers are intialized properly
	if (bReturn == CDEX_OK)
	{
		if ((pExtract != NULL) && pExtract->IsAvailable())
		{
			// Obtain the specs of the SCSI devices and select the proper CD Device
			pExtract->GetCDRomDevices();
		}
		else
		{
			bReturn = CDEX_ERROR;
		}
	}

	if (bReturn == CDEX_OK)
	{
		if (CR_GetNumCDROM() < 1)
		{
			bReturn = CDEX_NOCDROMDEVICES;

			pExtract->SetAvailable(false);
		}
	}

	return bReturn;
}

DLLEXPORT LONG CR_GetNumCDROM()
{
	LONG lReturn = 0L;

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		lReturn = pExtract->GetNumDrives();
	}

	return lReturn;

}

DLLEXPORT void CR_SetActiveCDROM(LONG nActiveDrive)
{
	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		pExtract->SetActiveCDROM( nActiveDrive );
	}
}

DLLEXPORT LONG CR_GetActiveCDROM()
{
	LONG lReturn = 0;

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		lReturn = pExtract->GetActiveCDROM();
	}
	else
	{
		lReturn = CDEX_ERROR;
	}

	return lReturn;
}

DLLEXPORT CDEX_ERR CR_SelectCDROMType(DRIVETYPE cdType)
{
	CDEX_ERR bReturn = CDEX_OK;

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		pExtract->SetDriveType( cdType );
		pExtract->UpdateDriveSettings();
	}
	else
	{
		bReturn = CDEX_ERROR;
	}

	return bReturn;
}

DLLEXPORT CDEX_ERR CR_GetCDROMParameters(CDROMPARAMS *pParam)
{
	CDEX_ERR bReturn = CDEX_OK;

	// Clear structure
	memset( pParam, 0x00, sizeof( CDROMPARAMS ) );

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		// Set CDROM ID
		strcpy(pParam->lpszCDROMID,pExtract->GetCDROMID()); 

		pParam->nOffsetStart		= pExtract->GetOffsetStart();
		pParam->nOffsetEnd		= pExtract->GetOffsetEnd();
		pParam->nSpeed			= pExtract->GetSpeed();
		pParam->nSpinUpTime		= pExtract->GetSpinUpTime();
		pParam->bJitterCorrection	= pExtract->GetJitterCorrection();
		pParam->bSwapLefRightChannel	= pExtract->GetSwapLefRightChannel();
		pParam->nNumOverlapSectors	= pExtract->GetNumOverlapSectors();
		pParam->DriveTable		= pExtract->GetDriveTable();
		pParam->nNumReadSectors		= pExtract->GetNumReadSectors();
		pParam->nNumCompareSectors	= pExtract->GetNumCompareSectors();
			
		pParam->btTargetID		= pExtract->GetTargetID();
		pParam->btAdapterID		= pExtract->GetAdapterID();
		pParam->btLunID			= pExtract->GetLunID();

		pParam->bAspiPosting		= pExtract->GetAspiPosting();
		pParam->nAspiTimeOut		= pExtract->GetAspiTimeOut();
		pParam->nAspiRetries		= pExtract->GetAspiRetries();

		pParam->bEnableMultiRead	= pExtract->GetMultiReadEnable();
		pParam->nMultiReadCount		= pExtract->GetMultiRead();
		pParam->bMultiReadFirstOnly	= pExtract->GetMultiReadFirstOnly();

		pParam->bLockDuringRead		= pExtract->GetLockDuringRead();

		pParam->nRippingMode		= pExtract->GetRippingMode();
		pParam->nParanoiaMode		= pExtract->GetParanoiaMode();

		pParam->bUseCDText		= pExtract->GetUseCDText();
	}
	else
	{
		bReturn = CDEX_ERROR;
	}

	return bReturn;
}

DLLEXPORT CDEX_ERR CR_SetCDROMParameters(CDROMPARAMS *pParam)
{
	CDEX_ERR bReturn = CDEX_OK;

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		pExtract->SetOffsetStart(pParam->nOffsetStart);
		pExtract->SetOffsetEnd(pParam->nOffsetEnd);
		pExtract->SetSpeed(pParam->nSpeed);
		pExtract->SetSpinUpTime(pParam->nSpinUpTime);
		pExtract->SetJitterCorrection(pParam->bJitterCorrection);
		pExtract->SetSwapLefRightChannel(pParam->bSwapLefRightChannel);
		pExtract->SetNumOverlapSectors(pParam->nNumOverlapSectors);
		pExtract->SetDriveTable(pParam->DriveTable);
		pExtract->SetNumReadSectors(pParam->nNumReadSectors	);
		pExtract->SetNumCompareSectors(pParam->nNumCompareSectors);

		pExtract->SetTargetID(pParam->btTargetID);
		pExtract->SetAdapterID(pParam->btAdapterID);
		pExtract->SetLunID(pParam->btLunID);

		pExtract->SetAspiPosting( pParam->bAspiPosting );

		pExtract->SetAspiRetries(pParam->nAspiRetries);
		pExtract->SetAspiTimeOut(pParam->nAspiTimeOut);

		pExtract->SetMultiReadEnable(pParam->bEnableMultiRead);
		pExtract->SetMultiRead(pParam->nMultiReadCount);
		pExtract->SetMultiReadFirstOnly(pParam->bMultiReadFirstOnly);

		pExtract->SetLockDuringRead( pParam->bLockDuringRead );

		if ( CUSTOMDRIVE != pParam->DriveTable.DriveType )
		{
			bReturn = CR_SelectCDROMType( pParam->DriveTable.DriveType );
		}

		pExtract->SetRippingMode( pParam->nRippingMode );

		pExtract->SetParanoiaMode( pParam->nParanoiaMode );

		pExtract->SetUseCDText( pParam->bUseCDText );
	}
	else
	{
		bReturn = CDEX_ERROR;
	}

	return bReturn;
}

DLLEXPORT CDEX_ERR CR_OpenRipper(LONG *plBufferSize, LONG dwStartSector, LONG dwEndSector)
{
	CDEX_ERR bReturn = CDEX_OK;

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		// Set Extract paramters, dwEndSector is inclusive !
		// thus if startsector=0 and endsector 1649, 1650 sectors are extracted
		switch ( pExtract->GetRippingMode() )
		{
			case CR_RIPPING_MODE_NORMAL:
				pExtract->SetupTrackExtract( dwStartSector, dwEndSector + 1 );
			break;
			case CR_RIPPING_MODE_PARANOIA:
				pExtract->SetupTrackExtractParanoia( dwStartSector, dwEndSector + 1 );
			break;
			default:
				return CDEX_ERROR;
		}


		// Start Thread
		//pExtract->StartThread(pExtract->ThreadFunc,pExtract);
		*plBufferSize= pExtract->GetNumReadSectors() * CB_CDDASECTORSIZE;
	}
	else
	{
		bReturn = CDEX_ERROR;
	}

	return bReturn;
}

DLLEXPORT CDEX_ERR CR_CloseRipper()
{
	CDEX_ERR bReturn = CDEX_OK;

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		// Set Extract paramters
		pExtract->EndTrackExtract();
	}
	else
	{
		bReturn = CDEX_ERROR;
	}

	return bReturn;
}

DLLEXPORT CDEX_ERR CR_RipChunk(BYTE *pbtStream, LONG *pNumBytes, BOOL &bAbort)
{
	CDEX_ERR bReturn = CDEX_OK;

	*pNumBytes=0;

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		switch ( pExtract->GetRippingMode() )
		{
			case CR_RIPPING_MODE_NORMAL:
				bReturn = pExtract->RipChunk( pbtStream, pNumBytes, bAbort );
				break;

			case CR_RIPPING_MODE_PARANOIA:
				bReturn = pExtract->RipChunkParanoia( pbtStream, pNumBytes, bAbort );
				break;

			default:
				bReturn = CDEX_ERROR;
				break;
		}
	}
	else
	{
		bReturn = CDEX_ERROR;
	}

	return bReturn;
}

DLLEXPORT LONG CR_GetPeakValue()
{
	LONG lReturn = 0;

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		lReturn = pExtract->GetPeakValue();
	}

	return lReturn;
}

DLLEXPORT LONG CR_GetPercentCompleted()
{
	LONG lReturn = 0;

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		lReturn = min( pExtract->GetPercentCompleted(), 99 );
	}

	return lReturn;
}

DLLEXPORT LONG CR_GetNumberOfJitterErrors()
{
	LONG	 lReturn = 0;

	if ((NULL != pExtract) && pExtract->IsAvailable())
	{
		lReturn = pExtract->GetJitterErrors();
	}

	return lReturn;
}

DLLEXPORT CDEX_ERR CR_ReadToc()
{
	CDEX_ERR bReturn = CDEX_OK;

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		bReturn = pExtract->ReadToc();
	}	

	return bReturn;
}

DLLEXPORT CDEX_ERR CR_ReadCDText(BYTE *pbtBuffer, int nBufferSize, LPINT pnCDTextSize)
{
	CDEX_ERR bReturn = CDEX_OK;

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() && 
		 ( TRUE == pExtract->GetUseCDText() ) )
	{
		bReturn = pExtract->ReadCDText( pbtBuffer, nBufferSize, pnCDTextSize );
	}
	else
	{
		bReturn = CDEX_ERROR;
	}

	return bReturn;
}

DLLEXPORT LONG CR_GetNumTocEntries()
{
	LONG lReturn = 0;

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		lReturn = pExtract->GetToc().GetNumTracks();
	}

	return lReturn;
}

DLLEXPORT TOCENTRY CR_GetTocEntry(LONG nTocEntry)
{
	TOCENTRY TocEntry;

	memset( &TocEntry, 0x00, sizeof( TocEntry ) );

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		TocEntry.dwStartSector = pExtract->GetToc().GetStartSector( nTocEntry );
		TocEntry.btFlag = pExtract->GetToc().GetFlags( nTocEntry );
		TocEntry.btTrackNumber = pExtract->GetToc().GetTrackNumber( nTocEntry );
	}

	return TocEntry;
}

DLLEXPORT void CR_NormalizeChunk(SHORT *pbsStream, LONG nNumSamples, DOUBLE dScaleFactor )
{
	int i;

	for ( i = 0; i < nNumSamples; i++)
	{
		pbsStream[ i ] = (short)( (double)pbsStream[ i ] * dScaleFactor );
	}
}

DLLEXPORT BOOL CR_IsUnitReady()
{
	CDEX_ERR bReturn = CDEX_OK;

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		bReturn = pExtract->IsUnitReady();
	}
	else
	{
		bReturn = CDEX_ERROR;
	}

	return bReturn;
}

DLLEXPORT CDEX_ERR CR_IsMediaLoaded(CDMEDIASTATUS &IsMediaLoaded)
{
	CDEX_ERR bReturn = CDEX_OK;

	IsMediaLoaded = CDMEDIA_NOT_PRESENT;

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		IsMediaLoaded = pExtract->IsMediaLoaded();
	}
	else
	{
		bReturn = CDEX_ERROR;
	}

	return bReturn;
}

DLLEXPORT BOOL CR_EjectCD(BOOL bEject)
{
	CDEX_ERR bReturn = CDEX_OK;

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		pExtract->PreventMediaRemoval( FALSE );
		bReturn = pExtract->EjectCD( bEject );
	}
	else
	{
		bReturn = CDEX_ERROR;
	}

	return bReturn;
}

DLLEXPORT void CR_LockCD(BOOL bLock)
{
	CDEX_ERR bReturn = CDEX_OK;

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		pExtract->PreventMediaRemoval( bLock );
	}
	else
	{
		bReturn = CDEX_ERROR;
	}
}

DLLEXPORT CDSTATUSINFO CR_GetCDStatusInfo()
{
	return g_CDStatusInfo;
}

DLLEXPORT DRIVETYPE CR_GetCDROMType()
{
	DRIVETYPE retDriveType = GENERIC;

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		retDriveType = pExtract->GetDriveType();
	}

	return retDriveType;
}

DLLEXPORT LONG CR_GetJitterPosition()
{
	LONG lReturn = 50;

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		lReturn = pExtract->GetJitterPosition();
	}

	return lReturn;
}

DLLEXPORT void CR_GetLastJitterErrorPosition(DWORD &dwStartSector, DWORD &dwEndSector)
{
	dwStartSector=0;
	dwEndSector=0;

	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		pExtract->GetLastJitterErrorPosition( dwStartSector, dwEndSector );
	}
}

DLLEXPORT void CR_GetSubChannelTrackInfo(int &nReadIndex, int &nReadTrack, DWORD &dwReadPos)
{
	if ( ( NULL != pExtract ) && 
		 pExtract->IsAvailable() )
	{
		pExtract->GetSubChannelTrackInfo( nReadIndex, nReadTrack, dwReadPos );
	}
}

DLLEXPORT VOID CR_SetTransportLayer(int nTransportLayer)
{
	if (pExtract)
	{
		pExtract->SetTransportLayer(nTransportLayer);
	}
}

DLLEXPORT INT CR_GetTransportLayer()
{
	INT nReturn = 0;

	if ( pExtract )
	{
		nReturn = pExtract->GetTransportLayer();
	}

	return nReturn;
}

DLLEXPORT CDEX_ERR CR_ScanForC2Errors(DWORD dwStartSector, DWORD dwNumSectors, DWORD &dwErrors, DWORD *pdwErrorSectors)
{
	CDEX_ERR bReturn = CDEX_ERROR;

	if ( pExtract )
	{
		bReturn = pExtract->ScanForC2Errors(dwStartSector, dwNumSectors, dwErrors, pdwErrorSectors);
	}

	return bReturn;
}

DLLEXPORT DWORD CR_GetCurrentRipSector()
{
	return 0;
}

DLLEXPORT CDEX_ERR CR_GetDetailedDriveInfo(LPSTR lpszInfo, DWORD dwInfoSize)
{
	CDEX_ERR bReturn = CDEX_OK;

	if ((NULL != pExtract) && pExtract->IsAvailable())
	{
		bReturn = pExtract->GetDetailedDriveInfo(lpszInfo, dwInfoSize);
	}
	else
	{
		bReturn = CDEX_ERROR;
	}

	return bReturn;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			CR_DeInit();
			break;
	}

	return TRUE;
}
