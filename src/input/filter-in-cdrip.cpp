 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <iolib-cxx.h>
#include <input/filter-in-cdrip.h>
#include <cdrip/cdrip.h>
#include <dllinterfaces.h>
#include <memory.h>
#include <main.h>
#include <cddb.h>

#define PARANOIA_MODE_FULL        0xff
#define PARANOIA_MODE_DISABLE     0

#define PARANOIA_MODE_VERIFY      1
#define PARANOIA_MODE_FRAGMENT    2
#define PARANOIA_MODE_OVERLAP     4
#define PARANOIA_MODE_SCRATCH     8
#define PARANOIA_MODE_REPAIR      16
#define PARANOIA_MODE_NEVERSKIP   32

FilterInCDRip::FilterInCDRip(bonkEncConfig *config) : InputFilter(config)
{
	packageSize = 0;
	trackNumber = -1;
	buffer = NIL;
}

FilterInCDRip::~FilterInCDRip()
{
	if (buffer != NIL)
	{
		delete [] buffer;

		ex_CR_CloseRipper();

		if (currentConfig->cdrip_locktray) ex_CR_LockCD(false);
	}
}

int FilterInCDRip::ReadData(unsigned char **data, int size)
{
	if (trackNumber == -1) return true;

	if (byteCount >= trackSize)
	{
		if (buffer != NIL)
		{
			delete [] buffer;

			buffer = NIL;

			ex_CR_CloseRipper();

			if (currentConfig->cdrip_locktray) ex_CR_LockCD(false);
		}

		trackNumber = -1;

		return true;
	}

	LONG	 nBytes;
	BOOL	 abort = false;

	ex_CR_RipChunk(buffer, &nBytes, abort);

	byteCount += nBytes;

	size = nBytes;

	*data = new unsigned char [size];

	memcpy((void *) *data, (void *) buffer, size);


	return size;
}

S::Bool FilterInCDRip::SetTrack(Int newTrack)
{
	if (buffer != NIL)
	{
		delete [] buffer;

		ex_CR_CloseRipper();

		if (currentConfig->cdrip_locktray) ex_CR_LockCD(false);
	}

	trackNumber = newTrack;

	int	 numTocEntries;
	TOCENTRY entry;

	ex_CR_SetActiveCDROM(currentConfig->cdrip_activedrive);

	ex_CR_ReadToc();

	numTocEntries = ex_CR_GetNumTocEntries();

	entry.btTrackNumber = 0;

	for (int i = 0; i < numTocEntries; i++)
	{
		entry = ex_CR_GetTocEntry(i);

		if (!(entry.btFlag & CDROMDATAFLAG) && (entry.btTrackNumber == trackNumber)) break;
		else entry.btTrackNumber = 0;
	}

	if (entry.btTrackNumber == 0)
	{
		trackNumber = -1;

		return false;
	}

	int	 startSector = entry.dwStartSector;
	int	 endSector = 0;
	TOCENTRY entry2 = ex_CR_GetTocEntry(0);

	for (int j = 1; j <= numTocEntries; j++)
	{
		if (entry2.btTrackNumber == entry.btTrackNumber || entry2.btTrackNumber == 0xAA)

		{
			if ((j > 1) && (entry2.btFlag != ex_CR_GetTocEntry(j).btFlag) && (ex_CR_GetTocEntry(j).btTrackNumber != 0xAA))
			{
				endSector = ex_CR_GetTocEntry(j).dwStartSector - 11250 - 1;
			}
			else
			{
				endSector = ex_CR_GetTocEntry(j).dwStartSector - 1;
			}

			break;
		}

		entry2 = ex_CR_GetTocEntry(j);
	}

	trackSize = (endSector - startSector + 1) * 2352;
	byteCount = 0;

	LONG		 bufferSize = 0;
	CDROMPARAMS	 params;
	int		 nParanoiaMode = PARANOIA_MODE_FULL ^ PARANOIA_MODE_NEVERSKIP;

	switch (currentConfig->cdrip_paranoia_mode)
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
	}	
 

	ex_CR_GetCDROMParameters(&params);

	params.nRippingMode = currentConfig->cdrip_paranoia;
	params.nParanoiaMode = nParanoiaMode;
	params.bSwapLefRightChannel = currentConfig->cdrip_swapchannels;
	params.bJitterCorrection = currentConfig->cdrip_jitter;

	ex_CR_SetCDROMParameters(&params);

	if (currentConfig->cdrip_locktray) ex_CR_LockCD(true);

	ex_CR_OpenRipper(&bufferSize, startSector, endSector);

	buffer = new unsigned char [bufferSize];

	return true;
}

S::Int FilterInCDRip::GetTrackSize()
{
	if (trackNumber == -1) return 0;

	return trackSize;
}

bonkFormatInfo *FilterInCDRip::GetFileInfo(String inFile)
{
	bonkFormatInfo	*nFormat = new bonkFormatInfo;

	nFormat->channels = 2;
	nFormat->rate = 44100;
	nFormat->bits = 16;
	nFormat->trackInfo = NIL;
	nFormat->order = BYTE_INTEL;

	Int	 trackNumber = 0;
	Int	 trackLength = 0;
	Int	 audiodrive = 0;

	if (inFile.CompareN("/cda", 4) == 0)
	{
		String	 track;

		for (Int j = 4; j < inFile.Length(); j++) track[j - 4] = inFile[j];

		audiodrive = currentConfig->cdrip_activedrive;

		ex_CR_SetActiveCDROM(currentConfig->cdrip_activedrive);

		ex_CR_ReadToc();

		Int	 numTocEntries = ex_CR_GetNumTocEntries();

		TOCENTRY	 entry;
		TOCENTRY	 nextentry;

		entry.btTrackNumber = 0;

		for (Int i = 0; i < numTocEntries; i++)
		{
			entry = ex_CR_GetTocEntry(i);
			nextentry = ex_CR_GetTocEntry(i + 1);

			trackLength = nextentry.dwStartSector - entry.dwStartSector;

			if (!(entry.btFlag & CDROMDATAFLAG) && (entry.btTrackNumber == track.ToInt())) break;
			else entry.btTrackNumber = 0;
		}

		trackNumber = entry.btTrackNumber;
	}
	else if (inFile.Length() >= 5)
	{
		if (inFile[inFile.Length() - 4] == '.' && inFile[inFile.Length() - 3] == 'c' && inFile[inFile.Length() - 2] == 'd' && inFile[inFile.Length() - 1] == 'a')
		{
			InStream	*in = new InStream(STREAM_FILE, inFile, IS_READONLY);

			in->Seek(22);

			trackNumber = in->InputNumber(2);

			in->Seek(32);

			trackLength = in->InputNumber(4);

			delete in;

			for (audiodrive = 0; audiodrive < currentConfig->cdrip_numdrives; audiodrive++)
			{
				Bool	 done = false;

				ex_CR_SetActiveCDROM(audiodrive);

				ex_CR_ReadToc();

				Int	 numTocEntries = ex_CR_GetNumTocEntries();

				for (int j = 0; j < numTocEntries; j++)
				{
					TOCENTRY	 entry = ex_CR_GetTocEntry(j);
					TOCENTRY	 nextentry = ex_CR_GetTocEntry(j + 1);
					Int		 length = nextentry.dwStartSector - entry.dwStartSector;

					if (!(entry.btFlag & CDROMDATAFLAG) && entry.btTrackNumber == trackNumber && length == trackLength)
					{
						done = true;
						break;
					}
				}

				if (done) break;
			}
		}
	}

	currentConfig->appMain->ReadCDText();

	if (trackNumber == 0)
	{
		delete nFormat;

		return NIL;
	}

	nFormat->trackInfo = new bonkFormatInfo::bonkTrackInfo;

	nFormat->length = (trackLength * 2352) / (nFormat->bits / 8);
	nFormat->fileSize = trackLength * 2352;

	bonkEncCDDB	 cddb(currentConfig);

	cddb.SetActiveDrive(audiodrive);

	Array<bonkFormatInfo::bonkTrackInfo *>	*cdInfo = NIL;

	if (currentConfig->enable_cddb_cache) cdInfo = bonkEncCDDB::titleCache.GetEntry(cddb.ComputeDiscID());

	if (cdInfo == NIL)
	{
		Int	 oDrive = currentConfig->cdrip_activedrive;

		currentConfig->cdrip_activedrive = audiodrive;

		cdInfo = currentConfig->appMain->GetCDDBData();

		bonkEncCDDB::titleCache.DeleteEntry(cddb.ComputeDiscID());
		bonkEncCDDB::titleCache.AddEntry(cdInfo, cddb.ComputeDiscID());

		currentConfig->cdrip_activedrive = oDrive;
	}

	if (cdInfo != NIL)
	{
		nFormat->trackInfo->track	= trackNumber;
		nFormat->trackInfo->cdTrack	= trackNumber;
		nFormat->trackInfo->drive	= audiodrive;
		nFormat->trackInfo->outfile	= NIL;
		nFormat->trackInfo->hasText	= True;
		nFormat->trackInfo->artist	= cdInfo->GetEntry(0)->artist;
		nFormat->trackInfo->title	= cdInfo->GetEntry(trackNumber)->title;
		nFormat->trackInfo->album	= cdInfo->GetEntry(0)->album;
		nFormat->trackInfo->genre	= cdInfo->GetEntry(0)->genre;
		nFormat->trackInfo->year	= cdInfo->GetEntry(0)->year;
	}
	else if (currentConfig->appMain->cdText.GetEntry(trackNumber) != NIL)
	{
		nFormat->trackInfo->track	= trackNumber;
		nFormat->trackInfo->cdTrack	= trackNumber;
		nFormat->trackInfo->drive	= audiodrive;
		nFormat->trackInfo->outfile	= NIL;
		nFormat->trackInfo->hasText	= True;
		nFormat->trackInfo->artist	= currentConfig->appMain->cdText.GetEntry(0);
		nFormat->trackInfo->title	= currentConfig->appMain->cdText.GetEntry(trackNumber);
		nFormat->trackInfo->album	= currentConfig->appMain->cdText.GetEntry(100);
	}
	else
	{
		nFormat->trackInfo->track	= trackNumber;
		nFormat->trackInfo->cdTrack	= trackNumber;
		nFormat->trackInfo->drive	= audiodrive;
		nFormat->trackInfo->outfile	= NIL;
		nFormat->trackInfo->hasText	= False;
	}

	nFormat->trackInfo->isCDTrack = True;

	nFormat->trackInfo->origFilename = String("Audio CD ").Append(String::IntToString(nFormat->trackInfo->drive)).Append(" - Track ");

	if (nFormat->trackInfo->track < 10) nFormat->trackInfo->origFilename.Append("0");

	nFormat->trackInfo->origFilename.Append(String::IntToString(nFormat->trackInfo->track));

	currentConfig->appMain->FreeCDText();

	return nFormat;
}
