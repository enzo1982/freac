/*
 * written by A.L. Faber
 * partly copyright (C) 1999 Jay A. Key (AKrip)
 **********************************************************************
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 **********************************************************************
 *
 */

#include "CDRomSettings.h"
#include <stdio.h>
#include <stddef.h>
#include "NTScsi.h"

vector <int> NTSCSI_HALookupTable;

#define NUM_MAX_NTSCSI_DRIVES 26
#define NUM_FLOPPY_DRIVES 2

#define NTSCSI_HA_INQUIRY_SIZE 0x24

typedef struct {
	BYTE ha;
	BYTE tgt;
	BYTE lun;
	BYTE driveLetter;
	BOOL bIsCDDrive;
	HANDLE hDevice;
	BYTE inqData[ NTSCSI_HA_INQUIRY_SIZE ];
} NTSCSIDRIVE;

typedef struct
{
	BYTE		numAdapters;
	NTSCSIDRIVE	drive[ NUM_MAX_NTSCSI_DRIVES ];
} NTSCSIDRIVES;

void GetDriveInformation( BYTE i, NTSCSIDRIVE *pDrive );

static HANDLE GetFileHandle( BYTE i );

static NTSCSIDRIVES NtScsiDrives = {0,};

/*
 * Initialization of SCSI Pass Through Interface code.  Responsible for
 * setting up the array of SCSI devices.  This code will be a little
 * different from the normal code -- it will query each drive letter from
 * C: through Z: to see if it is  a CD.  When we identify a CD, we then 
 * send CDB with the INQUIRY command to it -- NT will automagically fill in
 * the PathId, TargetId, and Lun for us.
 */

int NtScsiInit( void )
{
	BYTE i;
	char buf[4];
	UINT uDriveType;
	int retVal = 0;

	NTSCSI_HALookupTable.clear();

	memset( &NtScsiDrives, 0x00, sizeof(NtScsiDrives) );

	// set very handle to invalid
	for( i = 0; i < NUM_MAX_NTSCSI_DRIVES; i++ )
	{
		NtScsiDrives.drive[ i ].hDevice = INVALID_HANDLE_VALUE;

		// Don't query the floppy drives
		if ( i >= NUM_FLOPPY_DRIVES )
		{
			wsprintf( buf, "%c:\\", (char)('A'+i) );

			uDriveType = GetDriveType( buf );

			/* check if this is a CDROM drive */
			if ( DRIVE_CDROM == uDriveType )
			{
				GetDriveInformation( i, &NtScsiDrives.drive[ i ] );

				if ( NtScsiDrives.drive[ i ].bIsCDDrive )
				{
					retVal++;
				}
			}
		}
	}

	NtScsiDrives.numAdapters = NtScsiGetNumAdapters( );

	return retVal;
}

void NtScsiDeInit( void )
{
	BYTE i;

	for( i = NUM_FLOPPY_DRIVES; i < NUM_MAX_NTSCSI_DRIVES; i++ )
	{
		if ( NtScsiDrives.drive[i].bIsCDDrive )
		{
			CloseHandle( NtScsiDrives.drive[ i ].hDevice );
		}
	}

	NtScsiDrives.numAdapters = 0;
	memset( &NtScsiDrives, 0x00, sizeof(NtScsiDrives) );
}


/*
 * Returns the number of "adapters" present. 
 */
BYTE NtScsiGetNumAdapters( void )
{
	BYTE buf[256];
	WORD i;

	memset( buf,0x00, sizeof(buf) );

	NTSCSI_HALookupTable.clear();

	// PortNumber 0 should exist, so pre-mark it.  This avoids problems
	// when the primary IDE drives are on PortNumber 0, but can't be opened
	// because of insufficient privelege (ie. non-admin).
	buf[0] = 1;

	for( i = 0; i < NUM_MAX_NTSCSI_DRIVES; i++ )
	{
		if ( NtScsiDrives.drive[i].bIsCDDrive )
		{
			buf[ NtScsiDrives.drive[i].ha ] = 1;
		}
	}

	for( i = 0; i < sizeof( buf ) / sizeof( buf[ 0 ] ); i++ )
	{
		if ( buf[ i ] )
		{
			// store value, since NTScsi port numbers are not always in order
			NTSCSI_HALookupTable.push_back( i );
		}
	}

	return NTSCSI_HALookupTable.size();
}


/*
 * Replacement for GetASPI32SupportInfo from wnaspi32.dll
 */
DWORD NtScsiGetASPI32SupportInfo( void )
{
	DWORD retVal = 0;


	if ( !NtScsiDrives.numAdapters )
	{
	    retVal = (DWORD)(MAKEWORD(0,SS_NO_ADAPTERS));
	}
	else
	{
		retVal = (DWORD)(MAKEWORD(NtScsiDrives.numAdapters,SS_COMP));
	}

	return retVal;
}

/*
 * Needs to call the appropriate function for the lpsrb->SRB_Cmd specified.
 * Valid types are SC_HA_INQUIRY, SC_GET_DEV_TYPE, SC_EXEC_SCSI_CMD,
 * and SC_RESET_DEV.
 */
DWORD NtScsiSendASPI32Command( LPSRB lpsrb )
{
	DWORD dwReturn = SS_ERR;

	if (lpsrb != NULL)
	{
		// translate HA id from logical ASPI HaId to NTScsi Port Number
		if ( lpsrb->SRB_HaId < NTSCSI_HALookupTable.size() )
		{
			lpsrb->SRB_HaId = NTSCSI_HALookupTable[ lpsrb->SRB_HaId ];
		}
		else
		{
			lpsrb->SRB_HaId = 0;
		}


		switch( lpsrb->SRB_Cmd )
		{
			case SC_HA_INQUIRY:
				dwReturn = NtScsiHandleHaInquiry( (LPSRB_HAINQUIRY)lpsrb );
			break;

			case SC_GET_DEV_TYPE:
				dwReturn =  NtScsiGetDeviceType( (LPSRB_GDEVBLOCK)lpsrb );
			break;

			case SC_EXEC_SCSI_CMD:
			  dwReturn =  NtScsiExecSCSICommand( (LPSRB_EXECSCSICMD)lpsrb, FALSE );
			break;

			case SC_RESET_DEV:
			default:
			  lpsrb->SRB_Status = SS_ERR;
			  dwReturn = SS_ERR;
			break;
		}
	}
	return dwReturn;
}


/*
 * Universal function to get a file handle to the CD device.  Since
 * NT 4.0 wants just the GENERIC_READ flag, and Win2K wants both
 * GENERIC_READ and GENERIC_WRITE (why a read-only CD device needs
 * GENERIC_WRITE access is beyond me...), the easist workaround is to just
 * try them both.
 */

static HANDLE GetFileHandle( BYTE i )
{
	char			buf[12]={0,};
	HANDLE			fh = NULL;
	OSVERSIONINFO	osver;
	DWORD			dwFlags = GENERIC_READ;
	DWORD			dwAccessMode = FILE_SHARE_READ;

	memset( &osver, 0x00, sizeof( osver ) );
	memset( &buf, 0x00, sizeof( buf ) );

	osver.dwOSVersionInfoSize = sizeof(osver);
	GetVersionEx( &osver );

	if ( (osver.dwPlatformId == VER_PLATFORM_WIN32_NT) && (osver.dwMajorVersion > 4) )
	{
		dwFlags |= GENERIC_WRITE;

		dwAccessMode |= FILE_SHARE_WRITE;
	}

	wsprintf( buf, "\\\\.\\%c:", (char)('A'+i) );

	fh = CreateFile( buf, 
					dwFlags, 
					dwAccessMode,
					NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					NULL );

	if ( fh == INVALID_HANDLE_VALUE )
	{
		// it went foobar somewhere, so try it with the GENERIC_WRITE bit flipped
		dwFlags ^= GENERIC_WRITE;
		dwAccessMode ^= FILE_SHARE_WRITE;

		fh = CreateFile(	buf, 
							dwFlags, 
							dwAccessMode, 
							NULL, 
							OPEN_EXISTING, 
							FILE_ATTRIBUTE_NORMAL,
							NULL );
    }

	return fh;
}



/*
 * fills in a pDrive structure with information from a SCSI_INQUIRY
 * and obtains the ha:tgt:lun values via IOCTL_SCSI_GET_ADDRESS
 */
void GetDriveInformation( BYTE i, NTSCSIDRIVE *pDrive )
{
	HANDLE			fh = INVALID_HANDLE_VALUE;
	BOOL			status = 0;
	SCSI_ADDRESS	scsiAddr = {0,};
	ULONG			returned;
	BYTE			inqData[ NTSCSI_HA_INQUIRY_SIZE ] = {0,};
	SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER swb = {0,};

	fh = GetFileHandle( i );

	if (fh != INVALID_HANDLE_VALUE)
	{
		// Get the drive inquiry data
		memset( inqData, 0x00, sizeof( inqData ) );
		memset( &swb, 0x00, sizeof( swb ) );

		swb.spt.Length = sizeof( SCSI_PASS_THROUGH_DIRECT );
		swb.spt.CdbLength = 6;
		swb.spt.SenseInfoLength = 24;
		swb.spt.DataIn = SCSI_IOCTL_DATA_IN;
		swb.spt.DataTransferLength = sizeof( inqData );
		swb.spt.TimeOutValue = 5;
		swb.spt.DataBuffer = inqData;
		swb.spt.SenseInfoOffset = offsetof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER,ucSenseBuf );
		swb.spt.Cdb[0] = SCSI_CMD_INQUIRY;
		swb.spt.Cdb[4] = NTSCSI_HA_INQUIRY_SIZE;

		status = DeviceIoControl( fh,
					IOCTL_SCSI_PASS_THROUGH_DIRECT,
					&swb,
					sizeof( swb ),
					&swb,
					sizeof( swb ),
					&returned,
					NULL );

		if ( status )
		{
			memcpy( pDrive->inqData, inqData, NTSCSI_HA_INQUIRY_SIZE );

			
			// get the address (path/tgt/lun) of the drive via IOCTL_SCSI_GET_ADDRESS
			scsiAddr.Length = sizeof( SCSI_ADDRESS );

			if ( DeviceIoControl(	fh, 
									IOCTL_SCSI_GET_ADDRESS, 
									NULL,
									0,
									&scsiAddr,
									sizeof( SCSI_ADDRESS ),
									&returned,
									NULL ) )
			{

				pDrive->bIsCDDrive = TRUE;
				pDrive->ha = scsiAddr.PortNumber;
				pDrive->tgt = scsiAddr.TargetId;
				pDrive->lun = scsiAddr.Lun;
				pDrive->driveLetter = i;
				pDrive->hDevice = INVALID_HANDLE_VALUE;
			}
			else
			{
				pDrive->bIsCDDrive     = FALSE;
			}

		}

		CloseHandle( fh );
	}
}



DWORD NtScsiHandleHaInquiry( LPSRB_HAINQUIRY lpsrb )
{
	DWORD	dwReturn = 0;

	lpsrb->HA_Count = NtScsiDrives.numAdapters;

	if ( lpsrb->SRB_HaId >= lpsrb->HA_Count )
    {
		lpsrb->SRB_Status = SS_INVALID_HA;
		dwReturn = SS_INVALID_HA;
	}
	else
	{
		// fill in ASPI driver emulation values
		lpsrb->HA_SCSI_ID  = 7;  

		strncpy( (char*)lpsrb->HA_ManagerId,  "CDEXASPI v0.03  ", 16 );
		strncpy( (char*)lpsrb->HA_Identifier, "SCSI Adapter    ", 16 );

		lpsrb->HA_ManagerId[ 15 ] = '\0';
		lpsrb->HA_Identifier[ 15 ] = '\0';

		lpsrb->HA_Identifier[ 13 ] = (char)( '0' + lpsrb->SRB_HaId );

		memset( lpsrb->HA_Unique, 0x00, 16 );
		lpsrb->HA_Unique[ 3 ] = 8;

		*( (LPDWORD)(&lpsrb->HA_Unique[ 4 ]) ) = 64 * 1024;

		lpsrb->SRB_Status = SS_COMP;

		dwReturn = SS_COMP;
	}

	return dwReturn;
}


/*
 * Scans through the drive array and returns DTYPE_CDROM type for all items
 * found, and DTYPE_UNKNOWN for all others.
 */
DWORD NtScsiGetDeviceType( LPSRB_GDEVBLOCK lpsrb )
{
	lpsrb->SRB_Status = SS_NO_DEVICE;

	if ( NtScsiGetDeviceIndex( lpsrb->SRB_HaId, lpsrb->SRB_Target, lpsrb->SRB_Lun ) )
	{
		lpsrb->SRB_Status = SS_COMP;
	}

	if ( lpsrb->SRB_Status == SS_COMP )
	{
		lpsrb->SRB_DeviceType = DTC_CDROM;
	}
	else
	{
		lpsrb->SRB_DeviceType = DTC_UNKNOWN;
	}

	return lpsrb->SRB_Status;
}


/*
 * Looks up the index in the drive array for a given ha:tgt:lun triple
 */
BYTE NtScsiGetDeviceIndex( BYTE ha, BYTE tgt, BYTE lun )
{
	BYTE i = 0;
	BYTE bReturn = 0;

	for( i = 2; i < NUM_MAX_NTSCSI_DRIVES; i++ )
    {
		if ( NtScsiDrives.drive[ i ].bIsCDDrive )
		{
			NTSCSIDRIVE *lpd;
			lpd = &NtScsiDrives.drive[ i ];

			if (	(lpd->ha == ha) && 
					(lpd->tgt == tgt) && 
					(lpd->lun == lun) )
			{
				bReturn = i;
				break;
			}
		}
    }

	return bReturn ;
}

/*
 * Converts ASPI-style SRB to SCSI Pass Through IOCTL
 */
DWORD NtScsiExecSCSICommand( LPSRB_EXECSCSICMD lpsrb, BOOL bBeenHereBefore )
{
	BOOL	status;
	SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER swb;
	ULONG	returned = 0;
	BYTE	idx = 0;

	idx = NtScsiGetDeviceIndex( lpsrb->SRB_HaId, lpsrb->SRB_Target, lpsrb->SRB_Lun );

	if ( idx == 0 )
	{
		lpsrb->SRB_Status = SS_ERR;
		return SS_ERR;
	}

	// Special action required for an INQUIRY?
	if ( SCSI_CMD_INQUIRY == lpsrb->CDBByte[0] )
	{
		lpsrb->SRB_Status = SS_COMP;
		memcpy( lpsrb->SRB_BufPointer, NtScsiDrives.drive[idx].inqData, NTSCSI_HA_INQUIRY_SIZE );
		return SS_COMP;
	}

	if ( NtScsiDrives.drive[idx].hDevice == INVALID_HANDLE_VALUE )
	{
		NtScsiDrives.drive[ idx ].hDevice = GetFileHandle( NtScsiDrives.drive[idx].driveLetter );
	}

	memset( &swb, 0x00, sizeof(swb) );
	swb.spt.Length = sizeof( SCSI_PASS_THROUGH_DIRECT );

	if ( lpsrb->SRB_Flags & SRB_DIR_IN )
	{
		swb.spt.DataIn = SCSI_IOCTL_DATA_IN;
	}
	else if ( lpsrb->SRB_Flags & SRB_DIR_OUT )
	{
		swb.spt.DataIn = SCSI_IOCTL_DATA_OUT;
	}		
	else
	{
		swb.spt.DataIn = SCSI_IOCTL_DATA_UNSPECIFIED;
	}

	swb.spt.DataTransferLength	= lpsrb->SRB_BufLen;
	swb.spt.TimeOutValue		= 15;
	swb.spt.DataBuffer			= lpsrb->SRB_BufPointer;
	swb.spt.SenseInfoLength		= lpsrb->SRB_SenseLen;
	swb.spt.SenseInfoOffset		= offsetof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER, ucSenseBuf);
	swb.spt.CdbLength			= lpsrb->SRB_CDBLen;

	memcpy( swb.spt.Cdb, lpsrb->CDBByte, lpsrb->SRB_CDBLen );

  //////////////////////////////////////////////////////////////////////////////////
	status = DeviceIoControl(	NtScsiDrives.drive[ idx ].hDevice,
							    IOCTL_SCSI_PASS_THROUGH_DIRECT,
								&swb,
								sizeof( swb ),
								&swb,
								sizeof( swb ),
								&returned,
								NULL );

	// copy sense data
	memcpy( lpsrb->SenseArea, swb.ucSenseBuf, lpsrb->SRB_SenseLen );

	if ( status )
	{
		lpsrb->SRB_Status = SS_COMP;
	}
	else
	{
		DWORD dwErrCode;

		lpsrb->SRB_Status   = SS_ERR;
		lpsrb->SRB_TargStat = 0x0004;

		lpsrb->SRB_Hdr_Rsvd = dwErrCode = GetLastError();
		
		/*
		 * KLUDGE ALERT! KLUDGE ALERT! KLUDGE ALERT!
		 * Whenever a disk changer switches disks, it may render the device
		 * handle invalid.  We try to catch these errors here and recover
		 * from them.
		 */
		if ( !bBeenHereBefore &&
			((dwErrCode == ERROR_MEDIA_CHANGED) || (dwErrCode == ERROR_INVALID_HANDLE)) )
		{
			if ( dwErrCode != ERROR_INVALID_HANDLE )
			{
				CloseHandle( NtScsiDrives.drive[idx].hDevice );
				NtScsiDrives.drive[idx].hDevice = INVALID_HANDLE_VALUE;
			}		

			GetDriveInformation( idx, &NtScsiDrives.drive[idx] );

			lpsrb->SRB_Status = NtScsiExecSCSICommand( lpsrb, TRUE );
		}
	}

	return lpsrb->SRB_Status;
}

/*
 * Calls GetFileHandle for the CD refered to by ha:tgt:lun to open it for
 * use
 */
void NtScsiOpenCDHandle( BYTE ha, BYTE tgt, BYTE lun )
{
	BYTE idx = 0;

	idx = NtScsiGetDeviceIndex( ha, tgt, lun );

	if ( idx && NtScsiDrives.drive[idx].hDevice == INVALID_HANDLE_VALUE )
	{
		NtScsiDrives.drive[idx].hDevice = GetFileHandle( NtScsiDrives.drive[idx].driveLetter );  
	}
}
