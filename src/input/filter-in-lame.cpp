 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-lame.h>

#include <dllinterfaces.h>

BonkEnc::FilterInLAME::FilterInLAME(Config *config, Track *format) : InputFilter(config, format)
{
	packageSize = 0;
}

BonkEnc::FilterInLAME::~FilterInLAME()
{
}

Bool BonkEnc::FilterInLAME::Activate()
{
	ex_lame_decode_init();

	return true;
}

Bool BonkEnc::FilterInLAME::Deactivate()
{
	ex_lame_decode_exit();

	return true;
}

Int BonkEnc::FilterInLAME::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	if (size <= 0) return -1;

	inBytes += size;

	data.Resize(size);

	driver->ReadData(data, size);

	pcm_l.Resize(size * 64);
	pcm_r.Resize(size * 64);

	Int	 nSamples = ex_lame_decode(data, size, pcm_l, pcm_r);

	data.Resize(nSamples * format->channels * (format->bits / 8));

	for (Int i = 0; i < nSamples; i++)
	{
		for (Int j = 0; j < format->channels; j++) ((short *) (unsigned char *) data)[format->channels * i + j] = (j == 0) ? pcm_l[i] : pcm_r[i];
	}

	return data.Size();
}

BonkEnc::Track *BonkEnc::FilterInLAME::GetFileInfo(const String &inFile)
{
	ex_lame_decode_init();

	Track		*nFormat = new Track;
	InStream	*f_in = new InStream(STREAM_FILE, inFile, IS_READONLY);

	nFormat->order		= BYTE_INTEL;
	nFormat->bits		= 16;
	nFormat->fileSize	= f_in->Size();
	nFormat->length		= -1;

	Buffer<unsigned char>	 buffer(4096);

	do
	{
		f_in->InputData((void *) buffer, buffer.Size());

		pcm_l.Resize(buffer.Size() * 64);
		pcm_r.Resize(buffer.Size() * 64);

		mp3data_struct	 mp3data;

		ZeroMemory(&mp3data, sizeof(mp3data));

		Int	 nSamples = ex_lame_decode_headers(buffer, buffer.Size(), pcm_l, pcm_r, &mp3data);

		if (mp3data.header_parsed && nSamples > mp3data.framesize)
		{
			nFormat->channels	= mp3data.stereo;
			nFormat->rate		= mp3data.samplerate;

			if (mp3data.nsamp > 0)		nFormat->length = mp3data.nsamp * nFormat->channels;
			else if (mp3data.bitrate > 0)	nFormat->approxLength = nFormat->fileSize / (mp3data.bitrate * 1000 / 8) * nFormat->rate * nFormat->channels;

			break;
		}
	}
	while (f_in->GetPos() < f_in->Size());

	delete f_in;

	ex_lame_decode_exit();

	if (currentConfig->enable_id3)
	{
		nFormat->track = -1;
		nFormat->outfile = NIL;

		ParseID3V2Tag(inFile, nFormat);
	}

	return nFormat;
}
