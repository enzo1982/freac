 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <iolib-cxx.h>
#include <input/filter-in-bonk.h>
#include <dllinterfaces.h>
#include <id3/tag.h>

FilterInBONK::FilterInBONK(bonkEncConfig *config) : InputFilter(config)
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
		driver->Seek(0);

		f_in = new InStream(STREAM_DRIVER, driver);

		decoder = ex_bonk_create_decoder(f_in, (uint32 *) &format.length, (uint32 *) &format.rate, (int *) &format.channels);

		format.order = BYTE_INTEL;
		format.bits = 16;

		setup = true;
	}

	vector<int>	 samples;

	samples.clear();

	if (!ex_bonk_decode_packet(decoder, samples)) return 0;

	*data = new unsigned char [samples.size() * 2];

	for (unsigned int i = 0; i < samples.size(); i++) ((short *) *data)[i] = min(max(samples[i], -32768), 32767);

	return samples.size() * 2;
}

bonkFormatInfo FilterInBONK::GetFileInfo(S::String inFile)
{
	bonkFormatInfo	 nFormat;
	InStream	*in = new InStream(STREAM_FILE, inFile, IS_READONLY);
	void		*decoder = ex_bonk_create_decoder(in, (uint32 *) &nFormat.length, (uint32 *) &nFormat.rate, (int *) &nFormat.channels);

	nFormat.order = BYTE_INTEL;
	nFormat.bits = 16;
	nFormat.trackInfo = NIL;
	nFormat.fileSize = in->Size();

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

	if (id3tag_size > 0) 
	{
		ID3_Tag		*tag = new ID3_Tag();
		ID3_Frame	*frame;
		ID3_Field	*field;
		int		 tbufsize = 1024;
		char		*tbuffer = new char [tbufsize];

		nFormat.trackInfo = new bonkTrackInfo;

		nFormat.trackInfo->track = -1;
		nFormat.trackInfo->outfile = NIL;
		nFormat.trackInfo->cdText = True;

		tag->Parse(id3tag, id3tag_size);

		tbuffer[0] = 0;
		if ((frame = tag->Find(ID3FID_LEADARTIST)) != NIL)
			if ((field = frame->GetField(ID3FN_TEXT)) != NIL) field->Get(tbuffer, tbufsize);
		nFormat.trackInfo->artist = tbuffer;

		tbuffer[0] = 0;
		if ((frame = tag->Find(ID3FID_TITLE)) != NIL)
			if ((field = frame->GetField(ID3FN_TEXT)) != NIL) field->Get(tbuffer, tbufsize);
		nFormat.trackInfo->title = tbuffer;

		tbuffer[0] = 0;
		if ((frame = tag->Find(ID3FID_ALBUM)) != NIL)
			if ((field = frame->GetField(ID3FN_TEXT)) != NIL) field->Get(tbuffer, tbufsize);
		nFormat.trackInfo->album = tbuffer;

		tbuffer[0] = 0;
		if ((frame = tag->Find(ID3FID_CONTENTTYPE)) != NIL)
			if ((field = frame->GetField(ID3FN_TEXT)) != NIL) field->Get(tbuffer, tbufsize);
		nFormat.trackInfo->genre = tbuffer;

		tbuffer[0] = 0;
		if ((frame = tag->Find(ID3FID_YEAR)) != NIL)
			if ((field = frame->GetField(ID3FN_TEXT)) != NIL) field->Get(tbuffer, tbufsize);
		nFormat.trackInfo->year = String(tbuffer).ToInt();

		tbuffer[0] = 0;
		if ((frame = tag->Find(ID3FID_TRACKNUM)) != NIL)
			if ((field = frame->GetField(ID3FN_TEXT)) != NIL) field->Get(tbuffer, tbufsize);
		nFormat.trackInfo->track = String(tbuffer).ToInt();

		if (nFormat.trackInfo->artist.Length() != 0 || nFormat.trackInfo->title.Length() != 0)
		{
			if (nFormat.trackInfo->artist.Length() == 0) nFormat.trackInfo->artist = "unknown artist";
			if (nFormat.trackInfo->title.Length() == 0) nFormat.trackInfo->title = "unknown title";
		}

		delete [] tbuffer;
		delete [] id3tag;
		delete tag;
	}

	return nFormat;
}
