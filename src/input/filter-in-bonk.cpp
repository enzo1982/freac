 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-bonk.h>

#include <dllinterfaces.h>

freac::FilterInBONK::FilterInBONK(Config *config, Track *format) : InputFilter(config, format)
{
	decoder	= NIL;

	packageSize = 0;
}

freac::FilterInBONK::~FilterInBONK()
{
}

Bool freac::FilterInBONK::Activate()
{
	unsigned int	 length		= 0;
	unsigned int	 rate		= 0;
	int		 channels	= 0;

	decoder = ex_bonk_decoder_create();

	int		 bytes = Math::Min(driver->GetSize(), (Int64) 524288);

	dataBuffer.Resize(bytes);

	driver->ReadData(dataBuffer, bytes);

	ex_bonk_decoder_init(decoder, dataBuffer, bytes, &length, &rate, &channels);

	return true;
}

Bool freac::FilterInBONK::Deactivate()
{
	ex_bonk_decoder_finish(decoder);

	ex_bonk_decoder_close(decoder);

	return true;
}

Int freac::FilterInBONK::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	size = driver->ReadData(dataBuffer, size >= 0 ? size : 0);

	data.Resize(131072);

	Int	 nSamples = ex_bonk_decoder_decode_packet(decoder, dataBuffer, size, (signed short *) (unsigned char *) data, data.Size());

	return (nSamples == -1) ? 0 : nSamples * (format->bits / 8);
}

freac::Track *freac::FilterInBONK::GetFileInfo(const String &inFile)
{
	InStream	*in		= new InStream(STREAM_FILE, inFile, IS_READ);
	Track		*nFormat	= new Track;
	unsigned int	 length		= 0;
	unsigned int	 rate		= 0;
	int		 channels	= 0;
	void		*decoder	= ex_bonk_decoder_create();

	int		 bytes = Math::Min(in->Size(), (Int64) 524288);

	dataBuffer.Resize(bytes);

	in->InputData(dataBuffer, bytes);

	ex_bonk_decoder_init(decoder, dataBuffer, bytes, &length, &rate, &channels);

	nFormat->length = length;
	nFormat->rate = rate;
	nFormat->channels = channels;
	nFormat->order = BYTE_INTEL;
	nFormat->bits = 16;
	nFormat->fileSize = in->Size();

	unsigned char	*id3tag = NIL;
	int		 id3tag_size = 0;

	ex_bonk_decoder_get_id3_data(decoder, &id3tag, &id3tag_size);

	if (id3tag_size > 0 && currentConfig->enable_id3) 
	{
		nFormat->track = -1;
		nFormat->outfile = NIL;

		ParseID3V2Tag(id3tag, id3tag_size, nFormat);
	}

	ex_bonk_decoder_close(decoder);

	delete in;

	return nFormat;
}
