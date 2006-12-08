 /* CDRip Ripping Library
  * Copyright (C) 1999-2002 Albert L. Faber
  * Portions Copyright (C) 2002-2006 Robert Kausch <robert.kausch@cdrip.org>
  *  
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2 of the License, or
  * (at your option) any later version.
  * 
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  * 
  * You should have received a copy of the GNU General Public License
  * along with this program; if not, write to the Free Software 
  * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#ifndef CDRIP_INCLUDED
#define CDRIP_INCLUDED

#include <windows.h>

#pragma pack(push, 4)

#ifdef _WIN32
  #define CRCCONV _stdcall
#else
  #define CRCCONV
#endif

#undef DLLEXPORT

#ifdef CDEX_DLL
	#define DLLEXPORT __declspec (dllexport) 
#else
	#define DLLEXPORT __declspec (dllimport)
#endif

#define	CDEX_ERR				LONG
#define	CDEX_OK					0x00000000
#define	CDEX_ERROR				0x00000001
#define	CDEX_FILEOPEN_ERROR			0x00000002
#define	CDEX_JITTER_ERROR			0x00000003
#define	CDEX_RIPPING_DONE			0x00000004
#define	CDEX_RIPPING_INPROGRESS			0x00000005
#define	CDEX_FILEWRITE_ERROR			0x00000006
#define	CDEX_OUTOFMEMORY			0x00000007
#define	CDEX_NOCDROMDEVICES			0x00000008
#define	CDEX_FAILEDTOLOADASPIDRIVERS		0x00000009
#define	CDEX_NATIVEEASPINOTSUPPORTED		0x0000000A
#define	CDEX_FAILEDTOGETASPISTATUS		0x0000000B
#define	CDEX_NATIVEEASPISUPPORTEDNOTSELECTED	0x0000000C
#define	CDEX_ACCESSDENIED			0x0000000D
#define	CDEX_SOMEACCESSDENIED			0x0000000E

#define HASTAT_OK			0x00	// Host adapter did not detect an error.
#define HASTAT_TIMEOUT			0x09	// The time allocated for a bus transaction ran out.
#define HASTAT_COMMAND_TIMEOUT		0x0B	// SRB expired while waiting to be processed.
#define HASTAT_MESSAGE_REJECT		0x0D	// MESSAGE REJECT received while processing SRB.
#define HASTAT_BUS_RESET		0x0E	// A bus reset was detected.
#define HASTAT_PARITY_ERROR		0x0F	// A parity error was detected.
#define HASTAT_REQUEST_SENSE_FAILED	0x10	// The adapter failed in issuing a Request Sense after a check condition was reported by the target device.
#define HASTAT_SEL_TO			0x11	// Selection of target timed out.
#define HASTAT_DO_DU			0x12	// Data overrun.
#define HASTAT_BUS_FREE			0x13	// Unexpected Bus Free.
#define HASTAT_PHASE_ERR		0x14	// Target Bus phase sequence failure.

#define STATUS_GOOD			0x00	// No target status.
#define STATUS_CHKCOND			0x02	// Check status (sense data is in SenseArea).
#define STATUS_BUSY			0x08	// Specified Target/LUN is busy.
#define STATUS_RESCONF			0x18	// Reservation conflict.

#define	TRANSPLAYER_ASPI		(0)
#define	TRANSPLAYER_NTSCSI		(1)

#define CDROMDATAFLAG			0x04
#define AUDIOTRKFLAG			0x10

#define CR_RIPPING_MODE_NORMAL		(0)
#define CR_RIPPING_MODE_PARANOIA	(1)

enum CDMEDIASTATUS
{
	CDMEDIA_PRESENT = 0,
	CDMEDIA_NOT_PRESENT,
	CDMEDIA_NOT_PRESENT_TRAY_OPEN,
	CDMEDIA_NOT_PRESENT_TRAY_CLOSED,
};

typedef struct
{
	BYTE	 sk;
	BYTE	 asc;
	BYTE	 ascq;
	BYTE	 ha_stat;
	BYTE	 target_stat;
}
CDSTATUSINFO;

enum DRIVETYPE
{
	GENERIC = 0,
	TOSHIBA,
	TOSHIBANEW,
	IBM,
	NEC,
	DEC,
	IMS,
	KODAK,
	RICOH,
	HP,
	PHILIPS,
	PLASMON,
	GRUNDIGCDR100IPW,
	MITSUMICDR,
	PLEXTOR,
	SONY,
	YAMAHA,
	NRC,
	IMSCDD5,
	CUSTOMDRIVE,
	NUMDRIVETYPES
};

enum READMETHOD
{
	READMMC = 0,
	READ10,
	READNEC,
	READSONY,
	READMMC2,
	READMMC3,
	READC1,
	READC2,
	READC3,
	READMMC4,
	NUMREADMETHODS
};

enum SETSPEED
{
	SPEEDNONE = 0,
	SPEEDMMC,
	SPEEDSONY,
	SPEEDYAMAHA,
	SPEEDTOSHIBA,
	SPEEDPHILIPS,
	SPEEDNEC,
	NUMSPEEDMETHODS
};

enum ENDIAN
{
	MY_BIGENDIAN = 0,
	MY_LITTLEENDIAN,
	MY_NUMENDIAN
};

enum ENABLEMODE
{
	ENABLENONE = 0,
	ENABLESTD,
	NUMENABLEMODES
};

typedef struct
{
	DRIVETYPE	 DriveType;
	READMETHOD	 ReadMethod;
	SETSPEED	 SetSpeed;
	ENDIAN		 Endian;
	ENABLEMODE	 EnableMode;
	LONG		 nDensity;
	BOOL		 bAtapi;
} DRIVETABLE;

enum OUTPUTFORMAT
{
	STEREO44100 = 0,
	MONO44100,
	STEREO22050,
	MONO22050,
	STEREO11025,
	MONO11025,
	NUMOUTPUTFORMATS
};

typedef struct
{
	char		 lpszCDROMID[255];	// CD-ROM ID, must be unique to index settings in INI file
	LONG		 nNumReadSectors;	// Number of sector to read per burst
	LONG		 nNumOverlapSectors;	// Number of overlap sectors for jitter correction
	LONG		 nNumCompareSectors;	// Number of sector to compare for jitter correction
	LONG		 nOffsetStart;		// Fudge factor at start of ripping in sectors
	LONG		 nOffsetEnd;		// Fudge factor at the end of ripping in sectors
	LONG		 nSpeed;		// CD-ROM speed factor 0 .. 32 x
	LONG		 nSpinUpTime;		// CD-ROM spin up time in seconds
	BOOL		 bDetectJitterErrors;	// Try to detect jitter errors or not
	BOOL		 bDetectC2Errors;	// Try to detect C2 errors or not
	BOOL		 bJitterCorrection;	// Boolean indicates whether to use Jitter Correction
	BOOL		 bSwapLefRightChannel;	// Swap left and right channel ? 
	DRIVETABLE	 DriveTable;		// Drive specific parameters
		
	BYTE		 btTargetID;		// SCSI target ID
	BYTE		 btAdapterID;		// SCSI Adapter ID
	BYTE		 btLunID;		// SCSI LUN ID

	BOOL		 bAspiPosting;		// When set ASPI posting is used, otherwhiese ASPI polling is used
	INT		 nAspiRetries;
	INT		 nAspiTimeOut;

	BOOL		 bEnableMultiRead;	// Enables Multiple Read Verify Feature
	BOOL		 bMultiReadFirstOnly;	// Only do the multiple reads on the first block
	INT		 nMultiReadCount;	// Number of times to reread and compare

	BOOL		 bLockDuringRead;	// Lock the CD-ROM drive tray during the ripping

	INT		 nRippingMode;
	INT		 nParanoiaMode;

	BOOL		 bUseCDText;		// Read CD Text info?
}
CDROMPARAMS;

// Table of contents structure
typedef struct
{
	DWORD	 dwStartSector;	// Start sector of the track
	BYTE	 btFlag;	// Track flags (i.e. data or audio track)
	BYTE	 btTrackNumber;	// Track number
}
TOCENTRY;

extern "C"
{
	// Call Init before anything else
	DLLEXPORT CDEX_ERR CRCCONV CR_Init(int nTransportLayer);

	// Call DeInit when ripping library is no longer needed
	DLLEXPORT CDEX_ERR CRCCONV CR_DeInit();

	// Get the DLL version number
	DLLEXPORT LONG CRCCONV CR_GetCDRipVersion();

	// Get the number of detected CD-ROM drives
	DLLEXPORT LONG CRCCONV CR_GetNumCDROM();

	// Get the active CDROM drive index (0 ... GetNumCDROM() - 1)
	DLLEXPORT LONG CRCCONV CR_GetActiveCDROM();

	// Get the active CDROM drive (0 ... GetNumCDROM() - 1)
	DLLEXPORT void CRCCONV CR_SetActiveCDROM(LONG nActiveDrive);

	// Select the DRIVETYPE of the active drive
	DLLEXPORT CDEX_ERR CRCCONV CR_SelectCDROMType(DRIVETYPE cdType);

	// Get the Selected CDROM type
	DLLEXPORT DRIVETYPE CRCCONV CR_GetCDROMType();

	// Get the CDROM parameters of the active drive
	DLLEXPORT CDEX_ERR CRCCONV CR_GetCDROMParameters(CDROMPARAMS *pParam);

	// Set the CDROM parameters of the active drive
	DLLEXPORT CDEX_ERR CRCCONV CR_SetCDROMParameters(CDROMPARAMS *pParam);

	// Start ripping section, output is fetched to WriteBufferFunc
	// Data is extracted from dwStartSector to dwEndSector
	DLLEXPORT CDEX_ERR CRCCONV CR_OpenRipper(LONG *plBufferSize, LONG dwStartSector, LONG dwEndSector);

	// Close the ripper, has to be called when the ripping process is completed (i.e 100%)
	// Or it can be called to abort the current ripping section
	DLLEXPORT CDEX_ERR CRCCONV CR_CloseRipper();

	// Indicates how far the ripping process is right now
	// Returns 100% when the ripping is completed
	DLLEXPORT LONG CRCCONV CR_GetPercentCompleted();

	// Returns the peak value of the ripped section (0..2^15)
	DLLEXPORT LONG CRCCONV CR_GetPeakValue();

	// Get number of Jitter errors that have occured during the ripping
	// This function must be called before CloseRipper is called!
	DLLEXPORT LONG CRCCONV CR_GetNumberOfJitterErrors();
	DLLEXPORT void CRCCONV CR_GetLastJitterErrorPosition(DWORD &dwStartSector, DWORD &dwEndSector);

	// Get the jitter position of the extracted track
	DLLEXPORT LONG CRCCONV CR_GetJitterPosition();

	// Get number of C2 errors that have occured during the ripping
	// This function must be called before CloseRipper is called!
	DLLEXPORT LONG CRCCONV CR_GetNumberOfC2Errors();
	DLLEXPORT void CRCCONV CR_GetLastC2ErrorPosition(DWORD &dwSector);

	// Rip a chunk from the CD, pbtStream contains the ripped data, pNumBytes the
	// number of bytes that have been ripped and corrected for jitter (if enabled)
	DLLEXPORT CDEX_ERR CRCCONV CR_RipChunk(BYTE *pbtStream, LONG *pNumBytes, BOOL &bAbort);

	// Normalize the stream (i.e. multiply by dScaleFactor)
	DLLEXPORT void CRCCONV CR_NormalizeChunk(SHORT *pbsStream, LONG nNumSamples, DOUBLE dScaleFactor);

	// Read the table of contents
	DLLEXPORT CDEX_ERR CRCCONV CR_ReadToc();

	// Read CD Text entry
	DLLEXPORT CDEX_ERR CRCCONV CR_ReadCDText(BYTE *pbtBuffer, int nBufferSize, LPINT pnCDTextSize);

	// Get the number of TOC entries, including the lead out
	DLLEXPORT LONG CRCCONV CR_GetNumTocEntries();

	// Get the TOC entry
	DLLEXPORT TOCENTRY CRCCONV CR_GetTocEntry(LONG nTocEntry);

	// Checks if the unit is ready (i.e. is the CD media present)
	DLLEXPORT BOOL CRCCONV CR_IsUnitReady();

	// Checks if the Media is loaded
	DLLEXPORT CDEX_ERR CRCCONV CR_IsMediaLoaded(CDMEDIASTATUS &IsMediaLoaded);

	// Eject the CD, bEject == TRUE => the CD will be ejected, bEject == FALSE => the CD will be loaded
	DLLEXPORT BOOL CRCCONV CR_EjectCD(BOOL bEject);

	// Get debug information
	DLLEXPORT CDSTATUSINFO CRCCONV CR_GetCDStatusInfo();

	// Lock/unlock the CD Tray
	DLLEXPORT void CRCCONV CR_LockCD(BOOL bLock);

	DLLEXPORT void CRCCONV CR_GetSubChannelTrackInfo(int &nReadIndex, int &nReadTrack, DWORD &dwReadPos);

	DLLEXPORT DWORD CRCCONV CR_GetCurrentRipSector();

	// Change transport layer, DLL has to be re-initialzed when changing the transport layer!
	// 0 = ASPI drivers
	// 1 = Native NT scsi drivers

	DLLEXPORT VOID CRCCONV CR_SetTransportLayer(int nTransportLayer);
	DLLEXPORT INT CRCCONV CR_GetTransportLayer();

	DLLEXPORT CDEX_ERR CRCCONV CR_ScanForC2Errors(DWORD dwStartSector, DWORD dwNumSectors, DWORD &dwErrors, DWORD *pdwErrorSectors);

	DLLEXPORT CDEX_ERR CRCCONV CR_GetDetailedDriveInfo(LPSTR lpszInfo, DWORD dwInfoSize);
} // extern "C"

#pragma pack(pop)

#endif
