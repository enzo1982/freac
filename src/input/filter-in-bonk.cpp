 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
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
	f_in	= NIL;
	decoder	= NIL;

	packageSize = 0;
}

BonkEnc::FilterInBONK::~FilterInBONK()
{
}

Bool BonkEnc::FilterInBONK::Activate()
{
	UnsignedInt	 length		= 0;
	UnsignedInt	 rate		= 0;
	int		 channels	= 0;

	f_in = new InStream(STREAM_DRIVER, driver);

	decoder = ex_bonk_create_decoder(f_in, &length, &rate, &channels);

	buffer.Resize(131072);

	return true;
}

Bool BonkEnc::FilterInBONK::Deactivate()
{
	ex_bonk_close_decoder(decoder);

	delete f_in;

	return true;
}

Int BonkEnc::FilterInBONK::ReadData(UnsignedByte **data, Int size)
{
	size = ex_bonk_decode_packet(decoder, buffer, buffer.Size());

	if (size == -1) return 0;

	*data = buffer;

	return size;
}

BonkEnc::Track *BonkEnc::FilterInBONK::GetFileInfo(const String &inFile)
{
	Track		*nFormat = new Track;
	InStream	*in = new InStream(STREAM_FILE, inFile, IS_READONLY);
	int		 channels = nFormat->channels;
	void		*decoder = ex_bonk_create_decoder(in, (uint32 *) &nFormat->length, (uint32 *) &nFormat->rate, &channels);

	nFormat->channels = channels;
	nFormat->order = BYTE_INTEL;
	nFormat->bits = 16;
	nFormat->fileSize = in->Size();

	ex_bonk_close_decoder(decoder);

	in->Seek(in->Size() - 4);

	bool		 loop = true;
	unsigned char	*id3tag = NIL;
	int		 id3tag_size = 0;

	do
	{
		switch (in->InputNumber(4))
		{
			case 862218528: // ' id3' string
			{
				in->RelSeek(-8);

				id3tag_size = in->InputNumber(4) - 10;

				in->RelSeek(-id3tag_size - 4);

				id3tag = new unsigned char [id3tag_size];

				for (int j = 0; j < id3tag_size; j++) id3tag[j] = in->InputNumber(1);

				in->RelSeek(-id3tag_size - 2);

				break;
			}
			case 1802399586: // 'bonk' string
			case 1868983913: // 'info' string
			{
				in->RelSeek(-8);
				in->RelSeek(4 - in->InputNumber(4));

				break;
			}
			default:
			{
				loop = false;

				break;
			}
		}

		if (in->GetPos() == 0)	loop = false;
		else if (loop)		in->RelSeek(-4);
	}
	while (loop);

	delete in;

	if (id3tag_size > 0 && currentConfig->enable_id3) 
	{
		nFormat->track = -1;
		nFormat->outfile = NIL;

		ParseID3V2Tag(id3tag, id3tag_size, nFormat);
	}

	return nFormat;
}
