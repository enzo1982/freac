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
#include <input/filter-in-lame.h>
#include <dllinterfaces.h>
#include <3rdparty/id3/tag.h>

FilterInLAME::FilterInLAME(bonkEncConfig *config, bonkEncTrack *format) : InputFilter(config, format)
{
	packageSize = 0;
}

FilterInLAME::~FilterInLAME()
{
}

bool FilterInLAME::Activate()
{
	ex_lame_decode_init();

	Int	 size = 4096;

	do
	{
		unsigned char	*data = new unsigned char [size];

		driver->ReadData(data, size);

		short	*pcm_l = new short [size * 64];
		short	*pcm_r = new short [size * 64];

		mp3data_struct	 mp3data;

		ex_lame_decode_headers(data, size, pcm_l, pcm_r, &mp3data);

		delete [] pcm_l;
		delete [] pcm_r;

		delete [] data;

		if (mp3data.stereo <= 2 && mp3data.stereo > 0) break;
	}
	while (driver->GetPos() < driver->GetSize());

	driver->Seek(0);

	ex_lame_decode_exit();
	ex_lame_decode_init();

	return true;
}

bool FilterInLAME::Deactivate()
{
	ex_lame_decode_exit();

	return true;
}

int FilterInLAME::ReadData(unsigned char **data, int size)
{
	if (size <= 0) return -1;

	inBytes += size;

	*data = new unsigned char [size];

	driver->ReadData(*data, size);

	short	*pcm_l = new short [size * 64];
	short	*pcm_r = new short [size * 64];

	int	 nsamples = ex_lame_decode(*data, size, pcm_l, pcm_r);

	delete [] *data;

	*data = new unsigned char [nsamples * 4];

	for (Int i = 0; i < nsamples; i++)
	{
		((short *) *data)[2 * i]	= pcm_l[i];
		((short *) *data)[2 * i + 1]	= pcm_r[i];
	}

	delete [] pcm_l;
	delete [] pcm_r;

	return nsamples * 4;
}

bonkEncTrack *FilterInLAME::GetFileInfo(String inFile)
{
	ex_lame_decode_init();

	bonkEncTrack	*nFormat = new bonkEncTrack;
	InStream	*f_in = OpenFile(inFile);

	nFormat->order		= BYTE_INTEL;
	nFormat->bits		= 16;
	nFormat->fileSize	= f_in->Size();
	nFormat->length		= -1;

	Int	 size = 4096;

	do
	{
		unsigned char	*data = new unsigned char [size];

		f_in->InputData((void *) data, size);

		short	*pcm_l = new short [size * 64];
		short	*pcm_r = new short [size * 64];

		mp3data_struct	 mp3data;

		ex_lame_decode_headers(data, size, pcm_l, pcm_r, &mp3data);

		delete [] pcm_l;
		delete [] pcm_r;

		nFormat->channels	= mp3data.stereo;
		nFormat->rate		= mp3data.samplerate;

		delete [] data;

		if (mp3data.stereo <= 2 && mp3data.stereo > 0) break;
	}
	while (f_in->GetPos() < f_in->Size());

	CloseFile(f_in);

	ex_lame_decode_exit();

	if (currentConfig->enable_id3)
	{
		nFormat->track = -1;
		nFormat->outfile = NIL;

		ParseID3V2Tag(inFile, nFormat);
	}

	return nFormat;
}
