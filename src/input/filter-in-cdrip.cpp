 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <iolib-cxx.h>
#include <input/filter-in-cdrip.h>
#include <3rdparty/cdrip/cdrip.h>
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

typedef struct
{
	unsigned char	 packType;
	unsigned char	 trackNumber;
	unsigned char	 sequenceNumber;

	unsigned char	 characterPosition	:4;
	unsigned char	 block			:3;
	unsigned char	 bDBC			:1;

	unsigned char	 data[12];
	unsigned char	 crc0;
	unsigned char	 crc1;
}
cdTextPackage;

FilterInCDRip::FilterInCDRip(bonkEncConfig *config, bonkEncTrack *format) : InputFilter(config, format)
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

	debug_out->EnterMethod("FilterInCDRip::ReadData(unsigned char **, int)");

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

	debug_out->LeaveMethod();

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
	params.bEnableMultiRead = true;
	params.nMultiReadCount = 2;

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

bonkEncTrack *FilterInCDRip::GetFileInfo(String inFile)
{
	bonkEncTrack	*nFormat = new bonkEncTrack;

	nFormat->channels = 2;
	nFormat->rate = 44100;
	nFormat->bits = 16;
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

			if ((i > 0) && (entry.btFlag != nextentry.btFlag) && (nextentry.btTrackNumber != 0xAA))
			{
				trackLength = nextentry.dwStartSector - entry.dwStartSector - 11250;
			}
			else
			{
				trackLength = nextentry.dwStartSector - entry.dwStartSector;
			}

			if (!(entry.btFlag & CDROMDATAFLAG) && (entry.btTrackNumber == track.ToInt()))	break;
			else										entry.btTrackNumber = 0;
		}

		trackNumber = entry.btTrackNumber;
	}
	else if (inFile.Length() >= 5)
	{
		if (inFile[inFile.Length() - 4] == '.' && inFile[inFile.Length() - 3] == 'c' && inFile[inFile.Length() - 2] == 'd' && inFile[inFile.Length() - 1] == 'a')
		{
			InStream	*in = OpenFile(inFile);

			in->Seek(22);

			trackNumber = in->InputNumber(2);

			in->Seek(32);

			trackLength = in->InputNumber(4);

			CloseFile(in);

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

			Int	 numTocEntries = ex_CR_GetNumTocEntries();

			TOCENTRY	 entry;
			TOCENTRY	 nextentry;

			entry.btTrackNumber = 0;

			for (Int i = 0; i < numTocEntries; i++)
			{
				entry = ex_CR_GetTocEntry(i);
				nextentry = ex_CR_GetTocEntry(i + 1);

				if ((i > 0) && (entry.btFlag != nextentry.btFlag) && (nextentry.btTrackNumber != 0xAA))
				{
					trackLength = nextentry.dwStartSector - entry.dwStartSector - 11250;
				}
				else
				{
					trackLength = nextentry.dwStartSector - entry.dwStartSector;
				}

				if (!(entry.btFlag & CDROMDATAFLAG) && (entry.btTrackNumber == trackNumber))	break;
				else										entry.btTrackNumber = 0;
			}
		}
	}

	ReadCDText();

	if (trackNumber == 0)
	{
		delete nFormat;

		return NIL;
	}

	nFormat->length = (trackLength * 2352) / (nFormat->bits / 8);
	nFormat->fileSize = trackLength * 2352;

	bonkEncCDDB	 cddb(currentConfig);

	cddb.SetActiveDrive(audiodrive);

	Array<bonkEncTrack *>	*cdInfo = NIL;

	if (currentConfig->enable_cddb_cache) cdInfo = bonkEncCDDB::infoCache.GetEntry(cddb.ComputeDiscID());

	if (cdInfo == NIL && currentConfig->enable_auto_cddb)
	{
		Int	 oDrive = currentConfig->cdrip_activedrive;

		currentConfig->cdrip_activedrive = audiodrive;

		cdInfo = currentConfig->appMain->GetCDDBData();

		bonkEncCDDB::infoCache.RemoveEntry(cddb.ComputeDiscID());
		bonkEncCDDB::infoCache.AddEntry(cdInfo, cddb.ComputeDiscID());

		currentConfig->cdrip_activedrive = oDrive;
	}

	if (cdInfo != NIL)
	{
		nFormat->track		= trackNumber;
		nFormat->cdTrack	= trackNumber;
		nFormat->discid		= cddb.GetDiscIDString();
		nFormat->drive		= audiodrive;
		nFormat->outfile	= NIL;
		nFormat->artist		= cdInfo->GetEntry(0)->artist;
		nFormat->title		= cdInfo->GetEntry(trackNumber)->title;
		nFormat->album		= cdInfo->GetEntry(0)->album;
		nFormat->genre		= cdInfo->GetEntry(0)->genre;
		nFormat->year		= cdInfo->GetEntry(0)->year;
	}
	else if (cdText.GetEntry(trackNumber) != NIL)
	{
		nFormat->track		= trackNumber;
		nFormat->cdTrack	= trackNumber;
		nFormat->discid		= cddb.GetDiscIDString();
		nFormat->drive		= audiodrive;
		nFormat->outfile	= NIL;
		nFormat->artist		= cdText.GetEntry(0);
		nFormat->title		= cdText.GetEntry(trackNumber);
		nFormat->album		= cdText.GetEntry(100);
	}
	else
	{
		nFormat->track		= trackNumber;
		nFormat->cdTrack	= trackNumber;
		nFormat->discid		= cddb.GetDiscIDString();
		nFormat->drive		= audiodrive;
		nFormat->outfile	= NIL;
	}

	nFormat->isCDTrack	= True;
	nFormat->origFilename	= String("Audio CD ").Append(String::FromInt(nFormat->drive)).Append(" - Track ");

	if (nFormat->track < 10) nFormat->origFilename.Append("0");

	nFormat->origFilename.Append(String::FromInt(nFormat->track));

	FreeCDText();

	return nFormat;
}

Int FilterInCDRip::ReadCDText()
{
	FreeCDText();

	const int	 nBufferSize	= 4 + 8 * sizeof(cdTextPackage) * 256;
	unsigned char	*pbtBuffer	= new unsigned char [nBufferSize];
	int		 nCDTextSize	= 0;
	char		*lpZero		= NIL;

	ex_CR_ReadCDText(pbtBuffer, nBufferSize, &nCDTextSize);

	if (nCDTextSize < 4) return Error;

	int		 nNumPacks		= (nCDTextSize - 4) / sizeof(cdTextPackage);
	cdTextPackage	*pCDtextPacks		= NIL;
	char		 lpszBuffer[1024]	= {'\0',};
	int		 nInsertPos		= 0;

	for (Int i = 0; i < nNumPacks; i++)
	{
		pCDtextPacks = (cdTextPackage *) &pbtBuffer[i * sizeof(cdTextPackage) + 4];

		if (pCDtextPacks->block == 0)
		{
			for (Int j = 0; j < 12; j++) lpszBuffer[nInsertPos++] = pCDtextPacks->data[j];

			while (nInsertPos > 0 && (lpZero = (char *) memchr(lpszBuffer, '\0', nInsertPos)) != NIL)
			{
				Int	 nOut = (lpZero - lpszBuffer) + 1;

				if (pCDtextPacks->packType == 0x80) // Album/Track title
				{
					if (pCDtextPacks->trackNumber == 0) cdText.AddEntry(lpszBuffer, 100);
					if (pCDtextPacks->trackNumber != 0) cdText.AddEntry(lpszBuffer, pCDtextPacks->trackNumber);
				}
				else if (pCDtextPacks->packType == 0x81) // Artist name
				{
					if (pCDtextPacks->trackNumber == 0) cdText.AddEntry(lpszBuffer, 0);
				}

				nInsertPos -= nOut;

				memmove(lpszBuffer, lpZero + 1, 1024 - nOut -1);

				pCDtextPacks->trackNumber++;

				while (nInsertPos > 0 && lpszBuffer[ 0 ] == '\0')
				{
					memmove(lpszBuffer, lpszBuffer + 1, 1024 -1);
					nInsertPos--;
				}
			}
		}
	}

	delete [] pbtBuffer;

	return Success;
}

Int FilterInCDRip::FreeCDText()
{
	cdText.RemoveAll();

	return Success;
}
