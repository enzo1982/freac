 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2007 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-bonk.h>

#include <dllinterfaces.h>

BonkEnc::FilterInBONK::FilterInBONK(Config *config, Track *format) : InputFilter(config, format)
{
	decoder	= NIL;

	packageSize = 0;
}

BonkEnc::FilterInBONK::~FilterInBONK()
{
}

Bool BonkEnc::FilterInBONK::Activate()
{
	unsigned int	 length		= 0;
	unsigned int	 rate		= 0;
	int		 channels	= 0;

	decoder = ex_bonk_decoder_create();

	dataBuffer.Resize(16384);

	driver->ReadData(dataBuffer, 16384);

	ex_bonk_decoder_init(decoder, dataBuffer, 16384, &length, &rate, &channels);

	return true;
}

Bool BonkEnc::FilterInBONK::Deactivate()
{
	ex_bonk_decoder_finish(decoder);

	ex_bonk_decoder_close(decoder);

	return true;
}

Int BonkEnc::FilterInBONK::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	size = driver->ReadData(dataBuffer, size >= 0 ? size : 0);

	data.Resize(131072);

	Int	 nSamples = ex_bonk_decoder_decode_packet(decoder, dataBuffer, size, (signed short *) (unsigned char *) data, data.Size());

	return (nSamples == -1) ? 0 : nSamples * (format->bits / 8);
}

BonkEnc::Track *BonkEnc::FilterInBONK::GetFileInfo(const String &inFile)
{
	InStream	*in		= new InStream(STREAM_FILE, inFile, IS_READONLY);
	Track		*nFormat	= new Track;
	unsigned int	 length		= 0;
	unsigned int	 rate		= 0;
	int		 channels	= 0;
	void		*decoder	= ex_bonk_decoder_create();

	dataBuffer.Resize(16384);

	in->InputData(dataBuffer, 16384);

	ex_bonk_decoder_init(decoder, dataBuffer, 16384, &length, &rate, &channels);

	nFormat->length = length;
	nFormat->rate = rate;
	nFormat->channels = channels;
	nFormat->order = BYTE_INTEL;
	nFormat->bits = 16;
	nFormat->fileSize = in->Size();

	unsigned char	*id3tag = NIL;
	int		 id3tag_size = 0;

	ex_bonk_decoder_get_id3_data(decoder, &id3tag, &id3tag_size);

	ex_bonk_decoder_close(decoder);

	delete in;

	if (id3tag_size > 0 && currentConfig->enable_id3) 
	{
		nFormat->track = -1;
		nFormat->outfile = NIL;

		ParseID3V2Tag(id3tag, id3tag_size, nFormat);
	}

	return nFormat;
}
