/*
 * distilled information from various header files from Microsoft's
 * DDK for Windows NT 4.0
 */

#ifndef NTSCSI_H_INCLUDED
#define NTSCSI_H_INCLUDED

#include <windows.h>
#include "Aspi.h"

typedef struct
{
	USHORT	 Length;
	UCHAR	 ScsiStatus;
	UCHAR	 PathId;
	UCHAR	 TargetId;
	UCHAR	 Lun;
	UCHAR	 CdbLength;
	UCHAR	 SenseInfoLength;
	UCHAR	 DataIn;
	ULONG	 DataTransferLength;
	ULONG	 TimeOutValue;
	PVOID	 DataBuffer;
	ULONG	 SenseInfoOffset;
	UCHAR	 Cdb[16];
}
SCSI_PASS_THROUGH_DIRECT, *PSCSI_PASS_THROUGH_DIRECT;

typedef struct
{
	SCSI_PASS_THROUGH_DIRECT spt;
	UCHAR			 ucSenseBuf[32];
}
SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER, *PSCSI_PASS_THROUGH_DIRECT_WITH_BUFFER;

typedef struct
{
	UCHAR	 NumberOfLogicalUnits;
	UCHAR	 InitiatorBusId;
	ULONG	 InquiryDataOffset;
}
SCSI_BUS_DATA, *PSCSI_BUS_DATA;

typedef struct
{
	UCHAR		 NumberOfBusses;
	SCSI_BUS_DATA	 BusData[1];
}
SCSI_ADAPTER_BUS_INFO, *PSCSI_ADAPTER_BUS_INFO;

typedef struct
{
	UCHAR	 PathId;
	UCHAR	 TargetId;
	UCHAR	 Lun;
	BOOLEAN	 DeviceClaimed;
	ULONG	 InquiryDataLength;
	ULONG	 NextInquiryDataOffset;
	UCHAR	 InquiryData[1];
}
SCSI_INQUIRY_DATA, *PSCSI_INQUIRY_DATA;

typedef struct
{
	ULONG	 Length;
	UCHAR	 PortNumber;
	UCHAR	 PathId;
	UCHAR	 TargetId;
	UCHAR	 Lun;
}
SCSI_ADDRESS, *PSCSI_ADDRESS;

/*
 * method codes
 */
#define METHOD_BUFFERED		0
#define METHOD_IN_DIRECT	1
#define METHOD_OUT_DIRECT	2
#define METHOD_NEITHER		3

/*
 * file access values
 */
#define FILE_ANY_ACCESS		0
#define FILE_READ_ACCESS	(0x0001)
#define FILE_WRITE_ACCESS	(0x0002)

#define IOCTL_SCSI_BASE 0x00000004

/*
 * constants for DataIn member of SCSI_PASS_THROUGH* structures
 */
#define SCSI_IOCTL_DATA_OUT		0
#define SCSI_IOCTL_DATA_IN		1
#define SCSI_IOCTL_DATA_UNSPECIFIED	2

/*
 * Standard IOCTL define
 */
#define CTL_CODE(DevType, Function, Method, Access) \
	(((DevType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))

#define IOCTL_SCSI_PASS_THROUGH		CTL_CODE(IOCTL_SCSI_BASE, 0x0401, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_SCSI_MINIPORT		CTL_CODE(IOCTL_SCSI_BASE, 0x0402, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_SCSI_GET_INQUIRY_DATA	CTL_CODE(IOCTL_SCSI_BASE, 0x0403, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SCSI_GET_CAPABILITIES	CTL_CODE(IOCTL_SCSI_BASE, 0x0404, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SCSI_PASS_THROUGH_DIRECT	CTL_CODE(IOCTL_SCSI_BASE, 0x0405, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_SCSI_GET_ADDRESS		CTL_CODE(IOCTL_SCSI_BASE, 0x0406, METHOD_BUFFERED, FILE_ANY_ACCESS)

int	NtScsiInit(void);
void	NtScsiDeInit(void);

BYTE	NtScsiGetNumAdapters(void);
DWORD	NtScsiGetASPI32SupportInfo(void);
DWORD	NtScsiGetDeviceType(LPSRB_GDEVBLOCK lpsrb);
BYTE	NtScsiGetDeviceIndex(BYTE ha, BYTE tgt, BYTE lun);

DWORD	NtScsiHandleHaInquiry(LPSRB_HAINQUIRY lpsrb);
DWORD	NtScsiSendASPI32Command(LPSRB lpsrb);
DWORD	NtScsiExecSCSICommand(LPSRB_EXECSCSICMD lpsrb, BOOL bBeenHereBefore);

#endif
