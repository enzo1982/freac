 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-lame.h>
#include <3rdparty/id3/tag.h>

BonkEnc::FilterInLAME::FilterInLAME(Config *config, Track *format) : InputFilter(config, format)
{
	packageSize = 0;
}

BonkEnc::FilterInLAME::~FilterInLAME()
{
}

bool BonkEnc::FilterInLAME::Activate()
{
	ex_lame_decode_init();

	buffer.Resize(4096);

	do
	{
		driver->ReadData(buffer, buffer.Size());

		pcm_l.Resize(buffer.Size() * 64);
		pcm_r.Resize(buffer.Size() * 64);

		mp3data_struct	 mp3data;

		ex_lame_decode_headers(buffer, buffer.Size(), pcm_l, pcm_r, &mp3data);

		if (mp3data.stereo <= 2 && mp3data.stereo > 0) break;
	}
	while (driver->GetPos() < driver->GetSize());

	driver->Seek(0);

	ex_lame_decode_exit();
	ex_lame_decode_init();

	return true;
}

bool BonkEnc::FilterInLAME::Deactivate()
{
	ex_lame_decode_exit();

	return true;
}

int BonkEnc::FilterInLAME::ReadData(unsigned char **data, int size)
{
	if (size <= 0) return -1;

	inBytes += size;

	buffer.Resize(size);

	driver->ReadData(buffer, size);

	pcm_l.Resize(size * 64);
	pcm_r.Resize(size * 64);

	int	 nsamples = ex_lame_decode(buffer, size, pcm_l, pcm_r);

	buffer.Resize(nsamples * 4);

	for (Int i = 0; i < nsamples; i++)
	{
		((short *) (unsigned char *) buffer)[2 * i]	= pcm_l[i];
		((short *) (unsigned char *) buffer)[2 * i + 1]	= pcm_r[i];
	}

	*data = buffer;

	return buffer.Size();
}

Track *BonkEnc::FilterInLAME::GetFileInfo(String inFile)
{
	ex_lame_decode_init();

	Track		*nFormat = new Track;
	InStream	*f_in = OpenFile(inFile);

	nFormat->order		= BYTE_INTEL;
	nFormat->bits		= 16;
	nFormat->fileSize	= f_in->Size();
	nFormat->length		= -1;

	buffer.Resize(4096);

	do
	{
		f_in->InputData((void *) buffer, buffer.Size());

		pcm_l.Resize(buffer.Size() * 64);
		pcm_r.Resize(buffer.Size() * 64);

		mp3data_struct	 mp3data;

		ex_lame_decode_headers(buffer, buffer.Size(), pcm_l, pcm_r, &mp3data);

		nFormat->channels	= mp3data.stereo;
		nFormat->rate		= mp3data.samplerate;

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
