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

#ifndef CDROMSETTINGS_INCLUDED
#define CDROMSETTINGS_INCLUDED

#include <Windows.h>
#include <Stdio.h>
#include <Stdlib.h>
#include "vector"
#include "CDRip.h"

using namespace std ;

#define MAX_TRACKS 500

class CDSETTINGSPARAMS:public CDROMPARAMS
{
public:
	CDSETTINGSPARAMS();
	~CDSETTINGSPARAMS();
};


class CToc
{
	int		m_nNumTracks;
	BYTE	m_btaFlags[MAX_TRACKS];
	BYTE	m_btaTrackNumber[MAX_TRACKS];
	DWORD	m_dwaStartSector[MAX_TRACKS];

public:
	// CONSTRUCTOR
	CToc();
	// DESTRUCTOR
	~CToc();

	void	Clear();
	void	FillEntry(BYTE* btStream);
	DWORD	GetStartSector(int nIndex)		const {return m_dwaStartSector[nIndex];}
	BYTE	GetFlags(int nIndex)			const {return m_btaFlags[nIndex];}
	BYTE	GetTrackNumber(int nIndex)		const {return m_btaTrackNumber[nIndex];}
	INT	GetNumTracks()				const {return m_nNumTracks-1;}
	BOOL	IsAudioTrack(int p_track);
	DWORD	GetSize(int nTrack);
};


class CDRomSettings
{
private:
	int				m_nActive;
	vector<CDSETTINGSPARAMS>	m_CDParams;
	vector<CToc>			m_Toc;
	CHAR				m_lpszIniFname[255];
	INT				m_nTransportLayer;
public:
	CToc&	GetToc() {return m_Toc[m_nActive];}

	// Standard constructor
	CDRomSettings();

	// Standard destructor
	virtual ~CDRomSettings();

	void UpdateDriveSettings();

	void LoadSettings( BOOL bUpdateDriveSettings  );

	int	GetNumDrives() const {return m_CDParams.size();}

	// Set the active CDROM device
	void SetActiveCDROM( BYTE nValue );
	BYTE GetActiveCDROM() const {return m_nActive;}
	

	// Set the CDRom ID
	void AddCDRom(LPSTR lpszName,BYTE btAdapterID,BYTE btTargetID,BYTE btLunID);
	LPCSTR GetCDROMID() const {return m_CDParams[m_nActive].lpszCDROMID;}
	LPCSTR GetCDROMIDByIndex(int i) const {return m_CDParams[i].lpszCDROMID;}

	// Set SCSI Target ID
	void SetTargetID(BYTE nValue)	{m_CDParams[m_nActive].btTargetID=nValue;}
	BYTE GetTargetID() const {return m_CDParams[m_nActive].btTargetID;}

	// Set SCSI Adapter ID
	void SetAdapterID(BYTE nValue)	{m_CDParams[m_nActive].btAdapterID=nValue;}
	BYTE GetAdapterID() const {return m_CDParams[m_nActive].btAdapterID;}

	// Set SCSI Logical Unit ID
	void SetLunID(BYTE nValue)	{m_CDParams[m_nActive].btLunID=nValue;}
	BYTE GetLunID() const {return m_CDParams[m_nActive].btLunID;}

	// Get the number of overlap sectors for the jitter correction
	void SetNumOverlapSectors(int nValue)	{m_CDParams[m_nActive].nNumOverlapSectors=nValue;}
	int GetNumOverlapSectors() const {return m_CDParams[m_nActive].nNumOverlapSectors;}

	// Get the number of sectors to read at once
	void SetNumReadSectors(int nValue)	{m_CDParams[m_nActive].nNumReadSectors=nValue;}
	int GetNumReadSectors() const {return m_CDParams[m_nActive].nNumReadSectors;}

	// Set the number of sectors that have to be compared for the Jitter correction
	void SetNumCompareSectors(int nValue)	{m_CDParams[m_nActive].nNumCompareSectors=nValue;}
	int GetNumCompareSectors() const {return m_CDParams[m_nActive].nNumCompareSectors;}

	// Boolean value which determines if jitter correction is active
	void SetJitterCorrection(BOOL bValue)	{m_CDParams[m_nActive].bJitterCorrection=bValue;}
	BOOL GetJitterCorrection() const {return m_CDParams[m_nActive].bJitterCorrection;}

	// Boolean value which determines if left and right chanel have to be swapped
	void SetSwapLefRightChannel(BOOL bValue)	{m_CDParams[m_nActive].bSwapLefRightChannel=bValue;}
	BOOL GetSwapLefRightChannel() const {return m_CDParams[m_nActive].bSwapLefRightChannel;}

	// Sets the value which offsets the start ripping point with nValue sectors
	void SetOffsetStart(int nValue)	{m_CDParams[m_nActive].nOffsetStart=nValue;}
	int GetOffsetStart() const {return m_CDParams[m_nActive].nOffsetStart;}
	
	// Sets the value which offsets the end of the ripping with nValue sectors
	void SetOffsetEnd(int nValue)	{m_CDParams[m_nActive].nOffsetEnd=nValue;}
	int GetOffsetEnd() const {return m_CDParams[m_nActive].nOffsetEnd;}

	// Sets the ripping speed
	void SetSpeed(int nValue)	{m_CDParams[m_nActive].nSpeed=nValue;}
	int GetSpeed() const {return m_CDParams[m_nActive].nSpeed;}

	// Sets the enable flag for the multi read feature
	void SetMultiReadEnable(BOOL nValue)	{m_CDParams[m_nActive].bEnableMultiRead=nValue;}
	BOOL GetMultiReadEnable() const {return m_CDParams[m_nActive].bEnableMultiRead;}

	// Sets the multiple read compare
	void SetMultiRead(int nValue)	{m_CDParams[m_nActive].nMultiReadCount=nValue;}
	int GetMultiRead() const {return m_CDParams[m_nActive].nMultiReadCount;}

	// Sets the multiple read for first block only
	void SetMultiReadFirstOnly(BOOL nValue)	{m_CDParams[m_nActive].bMultiReadFirstOnly=nValue;}
	BOOL GetMultiReadFirstOnly() const {return m_CDParams[m_nActive].bMultiReadFirstOnly;}

	// Sets the multiple read for first block only
	void SetLockDuringRead(BOOL nValue)	{m_CDParams[m_nActive].bLockDuringRead=nValue;}
	BOOL GetLockDuringRead() const {return m_CDParams[m_nActive].bLockDuringRead;}


	// Sets the multiple read for first block only
	void SetUseCDText(BOOL nValue)	{m_CDParams[m_nActive].bUseCDText=nValue;}
	BOOL GetUseCDText() const {return m_CDParams[m_nActive].bUseCDText;}

	// Sets the CD-ROM spin up time in seconds
	void SetSpinUpTime(int nValue)	{m_CDParams[m_nActive].nSpinUpTime=nValue;}
	int GetSpinUpTime() const {return m_CDParams[m_nActive].nSpinUpTime;}

	DRIVETABLE	GetDriveTable()	const			{return m_CDParams[m_nActive].DriveTable;}
	void		SetDriveTable(DRIVETABLE& newDrive)   {m_CDParams[m_nActive].DriveTable=newDrive;}

	void		SetDriveType(DRIVETYPE cdType)	{m_CDParams[m_nActive].DriveTable.DriveType=cdType;}
	DRIVETYPE	GetDriveType() const			{return m_CDParams[m_nActive].DriveTable.DriveType;}

	// Use APSI posting or polling
	void SetAspiPosting(BOOL bValue)	{m_CDParams[m_nActive].bAspiPosting=bValue;}
	BOOL GetAspiPosting() const {return m_CDParams[m_nActive].bAspiPosting;}

	// Use APSI posting or polling
	void SetParanoiaMode( INT nValue )	{m_CDParams[m_nActive].nParanoiaMode = nValue;}
	INT GetParanoiaMode() const {return m_CDParams[m_nActive].nParanoiaMode;}

	// Set Paranoia ripping mode
	void SetRippingMode( INT nValue)	{m_CDParams[m_nActive].nRippingMode = nValue;}
	INT GetRippingMode() const {return m_CDParams[m_nActive].nRippingMode;}

	void		Clear()	{m_nActive=0;m_CDParams.clear();}


	// Set SCSI Logical Unit ID
	void SetAtapi(BOOL nValue)	{m_CDParams[m_nActive].DriveTable.bAtapi=nValue;}
	BOOL GetAtapi() const		{return m_CDParams[m_nActive].DriveTable.bAtapi;}

	void SetEnableMode(ENABLEMODE nValue)	{m_CDParams[m_nActive].DriveTable.EnableMode=nValue;}
	ENABLEMODE GetEnableMode() const		{return m_CDParams[m_nActive].DriveTable.EnableMode;}

	void	SetDensity(INT nValue)	{m_CDParams[m_nActive].DriveTable.nDensity=nValue;}
	INT		GetDensity() const		{return m_CDParams[m_nActive].DriveTable.nDensity;}
//	DRIVETYPE	GetDriveType() const			{return (DRIVETYPE)m_CDDrive;}
//	void		SetDriveType(DRIVETYPE nValue)	{m_nReadMethod=(int)nValue;}

	void		SetSpeedMethod(SETSPEED	nValue)	{m_CDParams[m_nActive].DriveTable.SetSpeed=nValue;}
	SETSPEED	GetSpeedMethod() const		{return m_CDParams[m_nActive].DriveTable.SetSpeed;}

	void		SetReadMethod(READMETHOD	nValue)	{m_CDParams[m_nActive].DriveTable.ReadMethod=nValue;}
	READMETHOD	GetReadMethod() const		{return m_CDParams[m_nActive].DriveTable.ReadMethod;}

	void		SetEndian(ENDIAN	nValue)	{m_CDParams[m_nActive].DriveTable.Endian=nValue;}
	ENDIAN		GetEndian() const			{return m_CDParams[m_nActive].DriveTable.Endian;}

	void SetAspiRetries(int nValue)	{m_CDParams[m_nActive].nAspiRetries=nValue;}
	int GetAspiRetries() const {return m_CDParams[m_nActive].nAspiRetries;}

	void SetAspiTimeOut(int nValue)	{m_CDParams[m_nActive].nAspiTimeOut=nValue;}
	int GetAspiTimeOut() const {return m_CDParams[m_nActive].nAspiTimeOut;}

	int	GetTransportLayer() ;
	void	SetTransportLayer( int nValue );

	void SetIniFileName( LPCSTR lpszIniFname);
	char* GetIniFileName() { return m_lpszIniFname;}
};

#endif
