 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-bonk.h>
#include <dllinterfaces.h>
#include <3rdparty/id3/tag.h>

FilterInBONK::FilterInBONK(bonkEncConfig *config, bonkEncTrack *format) : InputFilter(config, format)
{
	setup	= false;
	f_in	= NIL;
	decoder	= NIL;

	packageSize = 0;
}

FilterInBONK::~FilterInBONK()
{
	if (decoder != NIL)
	{
		ex_bonk_close_decoder(decoder);

		delete f_in;
	}
}

int FilterInBONK::ReadData(unsigned char **data, int size)
{
	if (!setup)
	{
		Int	 length = 0;
		Int	 rate = 0;
		Int	 channels = 0;

		driver->Seek(0);

		f_in = new InStream(STREAM_DRIVER, driver);

		decoder = ex_bonk_create_decoder(f_in, (uint32 *) &length, (uint32 *) &rate, (int *) &channels);

		setup = true;
	}

	long		 buffersize = 131072;
	unsigned char	*buffer = new unsigned char [buffersize];

	size = ex_bonk_decode_packet(decoder, buffer, buffersize);

	if (size == -1) return 0;

	*data = new unsigned char [size];

	memcpy((void *) *data, (void *) buffer, size);

	delete [] buffer;

	return size;
}

bonkEncTrack *FilterInBONK::GetFileInfo(String inFile)
{
	bonkEncTrack	*nFormat = new bonkEncTrack;
	InStream	*in = OpenFile(inFile);
	void		*decoder = ex_bonk_create_decoder(in, (uint32 *) &nFormat->length, (uint32 *) &nFormat->rate, (int *) &nFormat->channels);

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

	CloseFile(in);

	if (id3tag_size > 0 && currentConfig->enable_id3) 
	{
		nFormat->track = -1;
		nFormat->outfile = NIL;

		ParseID3V2Tag(id3tag, id3tag_size, nFormat);
	}

	return nFormat;
}
