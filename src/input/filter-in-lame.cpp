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
#include <input/filter-in-lame.h>
#include <dllinterfaces.h>

FilterInLAME::FilterInLAME(bonkEncConfig *config) : InputFilter(config)
{
	setup = false;

	packageSize = 0;

	ex_lame_decode_init();
}

FilterInLAME::~FilterInLAME()
{
}

bool FilterInLAME::EncodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	return true;
}

bool FilterInLAME::DecodeData(unsigned char **data, int size, int *outsize)
{
	if (size <= 0)
	{
		*outsize = 0;

		return false;
	}

	inBytes += size;

	short	*pcm_l = new short [size * 32];
	short	*pcm_r = new short [size * 32];

	mp3data_struct	 mp3data;
	int		 nsamples = ex_lame_decode_headers(*data, size, pcm_l, pcm_r, &mp3data);

	if (!setup)
	{
		setup = true;

		format.order = BYTE_INTEL;
		format.channels = mp3data.stereo;
		format.rate = mp3data.samplerate;
		format.bits = 16;
		format.length = -1;
	}

	delete [] *data;

	*data = new unsigned char [nsamples * 4];

	for (int i = 0; i < nsamples; i++)
	{
		((short *) *data)[2 * i]	= pcm_l[i];
		((short *) *data)[2 * i + 1]	= pcm_r[i];
	}

	*outsize = nsamples * 4;

	delete [] pcm_l;
	delete [] pcm_r;

	return true;
}

bonkFormatInfo FilterInLAME::GetAudioFormat()
{
	return format;
}
