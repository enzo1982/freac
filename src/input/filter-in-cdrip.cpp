 /* BonkEnc version 0.8
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
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

		ex_CR_DeInit();
	}
}

bool FilterInCDRip::EncodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	return true;
}

bool FilterInCDRip::DecodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	if (trackNumber == -1) return true;

	if (byteCount >= trackSize)
	{
		if (buffer != NIL)
		{
			delete [] buffer;

			buffer = NIL;

			ex_CR_CloseRipper();

			if (currentConfig->cdrip_locktray) ex_CR_LockCD(false);

			ex_CR_DeInit();
		}

		trackNumber = -1;

		return true;
	}

	LONG	 nBytes;
	BOOL	 abort = false;

	ex_CR_RipChunk(buffer, &nBytes, abort);

	byteCount += nBytes;

	*outsize = nBytes;

	delete [] *data;

	*data = new unsigned char [*outsize];

	memcpy((void *) *data, (void *) buffer, *outsize);


	return true;
}

bool FilterInCDRip::SetTrack(int newTrack)
{
	trackNumber = newTrack;

	int		 numTocEntries;
	TOCENTRY	 entry;
	SMOOTHString	 file = SMOOTH::StartDirectory;

	file.Append("bonkenc.ini");

	ex_CR_SetTransportLayer(currentConfig->cdrip_ntscsi);

	ex_CR_Init(file);

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

	int		 startSector = entry.dwStartSector;
	int		 endSector = 0;
	TOCENTRY	 entry2 = ex_CR_GetTocEntry(0);

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

int FilterInCDRip::GetTrackSize()
{
	if (trackNumber == -1) return 0;

	return trackSize;
}

bonkFormatInfo FilterInCDRip::GetAudioFormat()
{
	bonkFormatInfo	 format;

	format.channels = 2;
	format.rate = 44100;
	format.bits = 16;
	format.length = GetTrackSize() / (format.bits / 8);
	format.order = BYTE_INTEL;

	return format;
}
