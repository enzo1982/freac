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

bonkFormatInfo *FilterInBONK::GetFileInfo(String inFile)
{
	bonkFormatInfo	*nFormat = new bonkFormatInfo;
	InStream	*in = new InStream(STREAM_FILE, inFile, IS_READONLY);
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

	delete in;

	if (id3tag_size > 0 && currentConfig->enable_id3) 
	{
		ID3Tag		*tag = ex_ID3Tag_New();
		ID3Frame	*frame;
		ID3Field	*field;
		int		 tbufsize = 1024;
		char		*abuffer = new char [tbufsize];
		wchar_t		*wbuffer = new wchar_t [tbufsize];

		nFormat->trackInfo->track = -1;
		nFormat->trackInfo->outfile = NIL;
		nFormat->trackInfo->hasText = True;

		ex_ID3Tag_ParseBuffer(tag, id3tag, id3tag_size);

		ZeroMemory(abuffer, tbufsize);
		ZeroMemory(wbuffer, 2 * tbufsize);

		if ((frame = ex_ID3Tag_FindFrameWithID(tag, ID3FID_LEADARTIST)) != NIL)
			if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXTENC)) != NIL)
				if (ex_ID3Field_GetINT(field) == ID3TE_ASCII)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetASCII(field, abuffer, tbufsize);

					nFormat->trackInfo->artist.ImportFrom("UTF-8", abuffer);
				}
				else if (ex_ID3Field_GetINT(field) == ID3TE_UNICODE)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetUNICODE(field, (unicode_t *) wbuffer, tbufsize);

					nFormat->trackInfo->artist = wbuffer;
				}

		ZeroMemory(abuffer, tbufsize);
		ZeroMemory(wbuffer, 2 * tbufsize);

		if ((frame = ex_ID3Tag_FindFrameWithID(tag, ID3FID_TITLE)) != NIL)
			if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXTENC)) != NIL)
				if (ex_ID3Field_GetINT(field) == ID3TE_ASCII)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetASCII(field, abuffer, tbufsize);

					nFormat->trackInfo->title.ImportFrom("UTF-8", abuffer);
				}
				else if (ex_ID3Field_GetINT(field) == ID3TE_UNICODE)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetUNICODE(field, (unicode_t *) wbuffer, tbufsize);

					nFormat->trackInfo->title = wbuffer;
				}

		ZeroMemory(abuffer, tbufsize);
		ZeroMemory(wbuffer, 2 * tbufsize);

		if ((frame = ex_ID3Tag_FindFrameWithID(tag, ID3FID_ALBUM)) != NIL)
			if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXTENC)) != NIL)
				if (ex_ID3Field_GetINT(field) == ID3TE_ASCII)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetASCII(field, abuffer, tbufsize);

					nFormat->trackInfo->album.ImportFrom("UTF-8", abuffer);
				}
				else if (ex_ID3Field_GetINT(field) == ID3TE_UNICODE)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetUNICODE(field, (unicode_t *) wbuffer, tbufsize);

					nFormat->trackInfo->album = wbuffer;
				}

		ZeroMemory(abuffer, tbufsize);
		ZeroMemory(wbuffer, 2 * tbufsize);

		if ((frame = ex_ID3Tag_FindFrameWithID(tag, ID3FID_CONTENTTYPE)) != NIL)
			if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXTENC)) != NIL)
				if (ex_ID3Field_GetINT(field) == ID3TE_ASCII)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetASCII(field, abuffer, tbufsize);

					int startByte = 0;

					if (abuffer[0] == '(')
					{
						for (int i = 0; i < tbufsize; i++)
						{
							startByte++;

							if (abuffer[i] == ')') break;
						}
					}

					if ((abuffer + startByte)[0] != 0)	nFormat->trackInfo->genre.ImportFrom("UTF-8", abuffer + startByte);
					else if (startByte > 0)			nFormat->trackInfo->genre = GetID3CategoryName(String(abuffer + 1).ToInt());
				}
				else if (ex_ID3Field_GetINT(field) == ID3TE_UNICODE)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetUNICODE(field, (unicode_t *) wbuffer, tbufsize);

					int startByte = 0;

					if (wbuffer[0] == '(')
					{
						for (int i = 0; i < tbufsize; i++)
						{
							startByte++;

							if (wbuffer[i] == ')') break;
						}
					}

					nFormat->trackInfo->genre = wbuffer;
				}

		ZeroMemory(abuffer, tbufsize);
		ZeroMemory(wbuffer, 2 * tbufsize);

		if ((frame = ex_ID3Tag_FindFrameWithID(tag, ID3FID_YEAR)) != NIL)
			if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXTENC)) != NIL)
				if (ex_ID3Field_GetINT(field) == ID3TE_ASCII)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetASCII(field, abuffer, tbufsize);

					nFormat->trackInfo->year = String(abuffer).ToInt();
				}
				else if (ex_ID3Field_GetINT(field) == ID3TE_UNICODE)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetUNICODE(field, (unicode_t *) wbuffer, tbufsize);

					nFormat->trackInfo->year = String(wbuffer).ToInt();
				}

		ZeroMemory(abuffer, tbufsize);
		ZeroMemory(wbuffer, 2 * tbufsize);

		if ((frame = ex_ID3Tag_FindFrameWithID(tag, ID3FID_TRACKNUM)) != NIL)
			if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXTENC)) != NIL)
				if (ex_ID3Field_GetINT(field) == ID3TE_ASCII)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetASCII(field, abuffer, tbufsize);

					nFormat->trackInfo->track = String(abuffer).ToInt();
				}
				else if (ex_ID3Field_GetINT(field) == ID3TE_UNICODE)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetUNICODE(field, (unicode_t *) wbuffer, tbufsize);

					nFormat->trackInfo->track = String(wbuffer).ToInt();
				}

		delete [] abuffer;
		delete [] wbuffer;

		ex_ID3Tag_Delete(tag);
	}

	return nFormat;
}
