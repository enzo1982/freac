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

#ifndef ASPI_INCLUDED
#define ASPI_INCLUDED

#include <Windows.h>
#include <Stdio.h>
#include <Stdlib.h>
#include "CDRip.h"

// Handle to the ASPI libaray
extern HINSTANCE hAspiLib;
void GetAspiError(int nErrorCode,LPSTR lpszError);

// 1 byte alignment or SCSI structures
#pragma pack(push,1)

#define MAX_SCSIDEVICES 16
#define TIMEOUT 10000

typedef void (*POSTPROCFUNC)();

typedef struct
{
	BYTE	 _reserved1;
	BYTE	 bFlags;
	BYTE	 bTrack;
	BYTE	 _reserved2;
	DWORD	 dwStartSector;
}
TOC;

// The SRB_Flags are defined below.  These flags may be OR'd together to form
// the final value for SRB_Flags in the SRB.  Note that SRB_POSTING and
// SRB_EVENT_NOTIFY are mutually exclusive, as are SRB_DIR_IN and SRB_DIR_OUT.  In
// addition, the directioin bits (SRB_DIR_IN and SRB_DIR_OUT) MUST be set
// correctly on commands which transfer data.  Using SRB_DIR_SCSI is no longer
// an option as in ASPI for DOS and ASPI for Win16.

#define	SRB_POSTING			0x01	// Enable ASPI command completion posting. See section on posting below.
#define	SRB_ENABLE_RESIDUAL_COUNT	0x04	// Enables reporting of residual byte count.This flag is only significant if the host adapter reports support for residual byte count in the SC_HA_INQUIRY command.  When data underrun occurs, the SRB_BufLen field is updated to reflect the remaining bytes to transfer.
#define	SRB_DIR_IN			0x08	// Data transfer from SCSI target to host.
#define SRB_DIR_OUT			0x10	// Data transfer from host to SCSI target.
#define	SRB_EVENT_NOTIFY		0x40	// Enable ASPI command event notification. See section on event notification below.

// Inquiry DeviceTypeCodes 
#define DTC_DISK	0x00	// Direct-access device
#define DTC_TAPE	0x01	// Sequential-access device 
#define DTC_PRINTER	0x02	// Printer device 
#define DTC_PROCESSOR	0x03	// Processor device 
#define DTC_WORM	0x04	// Write-once device 
#define DTC_CDROM	0x05	// CD-ROM device 
#define DTC_SCANNER	0x06	// Scanner device 
#define DTC_OPTICAL	0x07	// Optical memory device 
#define DTC_JUKEBOX	0x08	// Medium changer device 
#define DTC_COMM	0x09	// Communications device 
#define DTC_PREPRESS1	0x0A	// Pre-press device 1 
#define DTC_PREPRESS2	0x0B	// Pre-press device 2 
#define DTC_UNKNOWN	0x1F	// Unknown or no device type 

/***************************************************************************
 ** SRB Status
 ***************************************************************************/
#define SS_PENDING                0x00   /* SRB being processed            */
#define SS_COMP                   0x01   /* SRB completed without error    */
#define SS_ABORTED                0x02   /* SRB aborted                    */
#define SS_ABORT_FAIL             0x03   /* Unable to abort SRB            */
#define SS_ERR                    0x04   /* SRB completed with error       */
#define SS_INVALID_CMD            0x80   /* Invalid ASPI command           */
#define SS_INVALID_HA             0x81   /* Invalid host adapter number    */
#define SS_NO_DEVICE              0x82   /* SCSI device not installed      */
#define SS_INVALID_SRB            0xE0   /* Invalid parameter set in SRB   */
#define SS_OLD_MANAGER            0xE1   /* ASPI manager doesn't support   */
                                         /* windows                        */
#define SS_BUFFER_ALIGN           0xE1   /* Buffer not aligned (replaces   */
                                         /* SS_OLD_MANAGER in Win32)       */
#define SS_ILLEGAL_MODE           0xE2   /* Unsupported Windows mode       */
#define SS_NO_ASPI                0xE3   /* No ASPI managers               */
#define SS_FAILED_INIT            0xE4   /* ASPI for windows failed init   */
#define SS_ASPI_IS_BUSY           0xE5   /* No resources available to      */
                                         /* execute command                */
#define SS_BUFFER_TO_BIG          0xE6   /* Buffer size too big to handle  */
#define SS_BUFFER_TOO_BIG         0xE6   /* Correct spelling of 'too'      */
#define SS_MISMATCHED_COMPONENTS  0xE7   /* The DLLs/EXEs of ASPI don't    */
                                         /* version check                  */
#define SS_NO_ADAPTERS            0xE8   /* No host adapters to manager    */
#define SS_INSUFFICIENT_RESOURCES 0xE9   /* Couldn't allocate resources    */
                                         /* needed to init                 */
#define SS_ASPI_IS_SHUTDOWN       0xEA   /* Call came to ASPI after        */
                                         /* PROCESS_DETACH                 */
#define SS_BAD_INSTALL            0xEB   /* The DLL or other components    */
                                         /* are installed wrong            */

// SRB defines
#define	SC_HA_INQUIRY		0x00	// Get information about installed host adapters,including the number of installed adapters.
#define	SC_GET_DEV_TYPE		0x01	// Get information about installed SCSI devices.
#define	SC_EXEC_SCSI_CMD	0x02	// Execute SCSI I/O.
#define	SC_ABORT_SRB		0x03	// Abort an outstanding I/O request.
#define	SC_RESET_DEV		0x04	// Reset an individual SCSI target.
#define	SC_GET_DISK_INFO	0x06	// Get information on disk type SCSI devices (not available under Windows NT).
#define SC_GETSET_TIMEOUTS	0x08

// MISC defines
#define CDSAMPLEFREQ 44100
#define TRACKSPERSEC 75

#define CB_CDDASECTORSIZE	2352
#define CB_CDDAC2SIZE		294
#define CB_QSUBCHANNEL		0
#define CB_CDROMSECTOR		2048
#define CB_AUDIO		(CB_CDDASECTORSIZE-CB_QSUBCHANNEL)

typedef struct
{
	BYTE        SRB_Cmd;		// ASPI command code
	BYTE        SRB_Status;		// ASPI command status byte
	BYTE        SRB_HaId;		// ASPI host adapter number
	BYTE        SRB_Flags;		// ASPI request flags
	DWORD       SRB_Hdr_Rsvd;	// Reserved, MUST = 0
}
SRB_HEADER, *LPSRB;

typedef struct
{
	BYTE	 SRB_Cmd;		// ASPI command code = SC_HA_INQUIRY
	BYTE	 SRB_Status;		// ASPI command status byte
	BYTE	 SRB_HaId;		// ASPI host adapter number
	BYTE	 SRB_Flags;		// ASPI request flags
	DWORD	 SRB_Hdr_Rsvd;		// Reserved, MUST = 0
	BYTE	 HA_Count;		// Number of host adapters present
	BYTE	 HA_SCSI_ID;		// SCSI ID of host adapter
	BYTE	 HA_ManagerId[16];	// String describing the manager
	BYTE	 HA_Identifier[16];	// String describing the host adapter
	BYTE	 HA_Unique[16];		// Host Adapter Unique parameters
	WORD	 HA_Rsvd1;
}
SRB_HAINQUIRY, *LPSRB_HAINQUIRY;

typedef struct
{
	BYTE	 SRB_Cmd;		// ASPI command code = SC_GET_DEV_TYPE
	BYTE	 SRB_Status;		// ASPI command status byte
	BYTE	 SRB_HaId;		// ASPI host adapter number
	BYTE	 SRB_Flags;		// Reserved
	DWORD	 SRB_Hdr_Rsvd;		// Reserved
	BYTE	 SRB_Target;		// Target's SCSI ID
	BYTE	 SRB_Lun;		// Target's LUN number
	BYTE	 SRB_DeviceType;	// Target's peripheral device type
	BYTE	 SRB_Rsvd1;		// Reserved for alignment
}
SRB_GDEVBLOCK, *LPSRB_GDEVBLOCK;

#define SENSE_LEN 14	// Maximum sense length

typedef struct
{
	BYTE	 SRB_Cmd;			// ASPI command code = SC_EXEC_SCSI_CMD
	BYTE	 SRB_Status;			// ASPI command status byte
	BYTE	 SRB_HaId;			// ASPI host adapter number
	BYTE	 SRB_Flags;			// ASPI request flags
	DWORD	 SRB_Hdr_Rsvd;			// Reserved
	BYTE	 SRB_Target;			// Target's SCSI ID
	BYTE	 SRB_Lun;			// Target's LUN number
	WORD	 SRB_Rsvd1;			// Reserved for Alignment
	DWORD	 SRB_BufLen;			// Data Allocation Length
	BYTE	*SRB_BufPointer;		// Data Buffer Point
	BYTE	 SRB_SenseLen;			// Sense Allocation Length
	BYTE	 SRB_CDBLen;			// CDB Length
	BYTE	 SRB_HaStat;			// Host Adapter Status
	BYTE	 SRB_TargStat;			// Target Status
	void	 (*SRB_PostProc)();		// Post routine
	void	*SRB_Rsvd2;			// Reserved
	BYTE	 SRB_Rsvd3[16];			// Reserved for expansion
	BYTE	 CDBByte[16];			// SCSI CDB
	BYTE	 SenseArea[SENSE_LEN+2];	// Request Sense buffer
}
SRB_EXECSCSICMD, *LPSRB_EXECSCSICMD;

typedef struct
{
	BYTE	 SRB_Cmd;		// ASPI command code = SC_EXEC_SCSI_CMD
	BYTE	 SRB_Status;		// ASPI command status byte
	BYTE	 SRB_HaId;		// ASPI host adapter number
	BYTE	 SRB_Flags;		// Reserved
	DWORD	 SRB_Hdr_Rsvd;		// Reserved
	BYTE	 SRB_Target;		// Target's SCSI ID
	BYTE	 SRB_Lun;		// Target's LUN number
	BYTE	 SRB_DriveFlags;	// Driver flags
	BYTE	 SRB_Int13HDriveInfo;	// Host Adapter Status
	BYTE	 SRB_Heads;		// Preferred number of heads translation
	BYTE	 SRB_Sectors;		// Preferred number of sectors translation
	BYTE	 SRB_Rsvd1[10];		// Reserved
}
SRB_GETDISKINFO, *LPSRB_GETDISKINFO;

typedef struct
{
	BYTE	 SRB_Cmd;	// ASPI command code = SC_RESET_DEV
	BYTE	 SRB_Status;	// ASPI command status byte
	BYTE	 SRB_HaId;	// ASPI host adapter number
	BYTE	 SRB_Flags;	// Reserved
	DWORD	 SRB_Hdr_Rsvd;	// Reserved
	BYTE	 SRB_Target;	// Target's SCSI ID
	BYTE	 SRB_Lun;	// Target's LUN number
	BYTE	 SRB_Rsvd1[12];	// Reserved for Alignment
	BYTE	 SRB_HaStat;	// Host Adapter Status
	BYTE	 SRB_TargStat;	// Target Status
	void	*SRB_PostProc;	// Post routine
	void	*SRB_Rsvd2;	// Reserved
	BYTE	 SRB_Rsvd3[32];	// Reserved
}
SRB_BUSDEVICERESET, *LPSRB_BUSDEVICERESET;

typedef struct
{
	BYTE	 SRB_Cmd;	/* 00/000 ASPI cmd code == SC_ABORT_SRB	*/
	BYTE	 SRB_Status;	/* 01/001 ASPI command status byte	*/
	BYTE	 SRB_HaID;	/* 02/002 ASPI host adapter number	*/
	BYTE	 SRB_Flags;	/* 03/003 Reserved, must = 0		*/
	DWORD	 SRB_Hdr_Rsvd;	/* 04/004 Reserved, must = 0		*/
	void	*SRB_ToAbort;	/* 08/008 Pointer to SRB to abort	*/
}
SRB_Abort, *PSRB_Abort, FAR *LPSRB_Abort;

typedef unsigned char	Ucbit;
typedef unsigned char	u_char;

#define	MP_P_CODE		\
	Ucbit	p_code	: 6;	\
	Ucbit	p_res	: 1;	\
	Ucbit	parsave	: 1

// CD Cap / mech status 
typedef struct SCSICDMODEPAGE2A_TAG
{		
	MP_P_CODE;			// parsave & pagecode			(0)
	u_char	  p_len;		// 0x14 = 20 Bytes			(1)

	Ucbit	cd_r_read	: 1;	// Reads CD-R  media			(2)
	Ucbit	cd_rw_read	: 1;	// Reads CD-RW media		     
	Ucbit	method2		: 1;	// Reads fixed packet method2 media  
	Ucbit	dvd_rom_read	: 1;	// Reads DVD ROM media		     
	Ucbit	dvd_r_read	: 1;	// Reads DVD-R media		     
	Ucbit	dvd_ram_read	: 1;	// Reads DVD-RAM media		     
	Ucbit	res_2_67	: 2;	// Reserved			     

	Ucbit	cd_r_write	: 1;	// Supports writing CD-R  media		(3)
	Ucbit	cd_rw_write	: 1;	// Supports writing CD-RW media	     
	Ucbit	test_write	: 1;	// Supports emulation write	     
	Ucbit	res_3_3		: 1;	// Reserved			     
	Ucbit	dvd_r_write	: 1;	// Supports writing DVD-R media	     
	Ucbit	dvd_ram_write	: 1;	// Supports writing DVD-RAM media    
	Ucbit	res_3_67	: 2;	// Reserved			     

	Ucbit	audio_play	: 1;	// Supports Audio play operation	(4)    
	Ucbit	composite	: 1;	// Deliveres composite A/V stream
	Ucbit	digital_port_2	: 1;	// Supports digital output on port 2 
	Ucbit	digital_port_1	: 1;	// Supports digital output on port 1 
	Ucbit	mode_2_form_1	: 1;	// Reads Mode-2 form 1 media (XA)    
	Ucbit	mode_2_form_2	: 1;	// Reads Mode-2 form 2 media	     
	Ucbit	multi_session	: 1;	// Reads multi-session media	     
	Ucbit	res_4		: 1;	// Reserved			     

	Ucbit	cd_da_supported	: 1;	// Reads audio data with READ CD cmd	(5)
	Ucbit	cd_da_accurate	: 1;	// READ CD data stream is accurate   
	Ucbit	rw_supported	: 1;	// Reads R-W sub channel information 
	Ucbit	rw_deint_cor	: 1;	// Reads de-interleved R-W sub chan  
	Ucbit	c2_pointers	: 1;	// Supports C2 error pointers	     
	Ucbit	ISRC		: 1;	// Reads ISRC information	     
	Ucbit	UPC		: 1;	// Reads media catalog number (UPC)  
	Ucbit	read_bar_code	: 1;	// Supports reading bar codes	     

	Ucbit	lock		: 1;	// PREVENT/ALLOW may lock media		(6)
	Ucbit	lock_state	: 1;	// Lock state 0=unlocked 1=locked    
	Ucbit	prevent_jumper	: 1;	// State of prev/allow jumper 0=pres 
	Ucbit	eject		: 1;	// Ejects disc/cartr with STOP LoEj  
	Ucbit	res_6_4		: 1;	// Reserved			     
	Ucbit	loading_type	: 3;	// Loading mechanism type	     

	Ucbit	sep_chan_vol	: 1;	// Vol controls each channel separat	(7)
	Ucbit	sep_chan_mute	: 1;	// Mute controls each channel separat
	Ucbit	disk_present_rep: 1;	// Changer supports disk present rep 
	Ucbit	sw_slot_sel	: 1;	// Load empty slot in changer	     
	Ucbit	res_7		: 4;	// Reserved			     

	BYTE	max_read_speed[2];	// Max. read speed in KB/s		(8)

	u_char	num_vol_levels[2];	// # of supported volume levels		(10)

	u_char	buffer_size[2];		// Buffer size for the data in KB	(12)
	u_char	cur_read_speed[2];	// Current read speed in KB/s		(14)	     
	u_char	res_16;			// Reserved				(16)
	
	Ucbit	res_17_0	: 1;	// Reserved				(17)
	Ucbit	BCK		: 1;	// Data valid on falling edge of BCK 
	Ucbit	RCK		: 1;	// Set: HIGH high LRCK=left channel  
	Ucbit	LSBF		: 1;	// Set: LSB first Clear: MSB first   
	Ucbit	length		: 2;	// 0=32BCKs 1=16BCKs 2=24BCKs 3=24I2c
	Ucbit	res_17		: 2;	// Reserved			     
	
	u_char	max_write_speed[2];	// Max. write speed supported in KB/s	(18)

	u_char	cur_write_speed[2];	// Current write speed in KB/s		(20)

}
SCSICDMODEPAGE2A;

char *fillbytes(void *tov, int cnt, char val);

typedef struct
{
	Ucbit	 sense_data_len	: 8;
	u_char	 medium_type;
	Ucbit	 res2		: 4;
	Ucbit	 cache		: 1;
	Ucbit	 res		: 2;
	Ucbit	 write_prot	: 1;
	BYTE	 nBlockLen;
}
SCISMODEHEADER;

typedef struct
{
	BYTE	 btModeDataLen;		// 0
	BYTE	 btMediumType;		// 1
	BYTE	 btDevSpecificParam;	// 2
	BYTE	 btBlkDescrLen;		// 3
}
SCSIMODEHDR_6;

typedef struct
{
	BYTE	 btModeDataLenH;	// 0
	BYTE	 btModeDataLenL;	// 1
	BYTE	 btMediumType;		// 2
	BYTE	 btDevSpecificParam;	// 3
	BYTE	 btReserved1;		// 4
	BYTE	 btReserved2;		// 5 
	BYTE	 btBlkDescrLenH;	// 6
	BYTE	 btBlkDescrLenL;	// 7
}
SCSIMODEHDR_10;

typedef struct
{
	BYTE	 btDensity;		// 0
	BYTE	 btNumberOfBlocksH;	// 1
	BYTE	 btNumberOfBlocksM;	// 2
	BYTE	 btNumberOfBlocksL;	// 3
	BYTE	 btReserved;		// 4
	BYTE	 btBlockLenH;		// 5 
	BYTE	 btBlockLenM;		// 6
	BYTE	 btBlockLenL;		// 7
}
SCSIBLOCKDESCRIPTOR;

// Error recovery Parameters
typedef struct
{
	MP_P_CODE;				//  0	parsave & pagecode
	u_char	 p_len;				//  1	0x0A = 12 Bytes
	Ucbit	 disa_correction	: 1;	//  2	Byte 2
	Ucbit	 term_on_rec_err	: 1;
	Ucbit	 report_rec_err		: 1;
	Ucbit	 en_early_corr		: 1;
	Ucbit	 read_continuous	: 1;
	Ucbit	 tranfer_block		: 1;
	Ucbit	 en_auto_reall_r	: 1;
	Ucbit	 en_auto_reall_w	: 1;
	u_char	 rd_retry_count;		//  3	Byte 3
	u_char	 correction_span;		//  4
	char	 head_offset_count;		//  5
	char	 data_strobe_offset;		//  6
	u_char	 res;				//  7
	u_char	 wr_retry_count;		//  8
	u_char	 res_tape[2];			//  9
	u_char	 recov_timelim[2];		// 11
}
SCSICDMODEPAGE1A;

// FUNCTION PROTOTYPES
CDEX_ERR	 InitAspiDll(bool &bUseNtScsi);
CDEX_ERR	 DeInitAspiDll();
VOID*		 Swap(VOID *p, int size);
BYTE		 IsScsiError(LPSRB lpSrb);

typedef struct
{
	BYTE	 SRB_Cmd;	// ASPI command code = SC_GETSET_TIMEOUTS
	BYTE	 SRB_Status;	// ASPI command status byte
	BYTE	 SRB_HaId;	// ASPI host adapter number
	BYTE	 SRB_Flags;	// ASPI request flags
	DWORD	 SRB_Hdr_Rsvd;	// Reserved
	BYTE	 SRB_Target;	// Target's SCSI ID
	BYTE	 SRB_Lun;	// Target's LUN number
	DWORD	 SRB_Timeout;	// Timeout in half seconds
}
SRB_GetSetTimeouts, *PSRB_GetSetTimeouts;

typedef struct
{
	LPBYTE	 AB_BufPointer;	// Pointer to the ASPI allocated buffer
	DWORD	 AB_BufLen;	// Length in bytes of the buffer
	DWORD	 AB_ZeroFill;	// Flag set to 1 if buffer should be zeroed
	DWORD	 AB_Reserved;	// Reserved, MUST = 0
}
ASPI32BUFF, *PASPI32BUFF;

typedef DWORD (*GETASPI32SUPPORTINFO)	(void);
typedef DWORD (*SENDASPI32COMMAND)	(LPSRB lpSRB);

#define	TEXT_GETASPI32SUPPORTINFO	"GetASPI32SupportInfo"
#define	TEXT_SENDASPI32COMMAND		"SendASPI32Command"

extern GETASPI32SUPPORTINFO		GetASPI32SupportInfo;
extern SENDASPI32COMMAND		SendASPI32Command;

#define SCSI_CMD_INQUIRY (0x12)

#pragma pack(pop)

#endif
