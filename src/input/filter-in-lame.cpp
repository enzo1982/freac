 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@gmx.net>
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
#include <id3/tag.h>

FilterInLAME::FilterInLAME(bonkEncConfig *config) : InputFilter(config)
{
	packageSize = 0;
}

FilterInLAME::~FilterInLAME()
{
}

bool FilterInLAME::Activate()
{
	ex_lame_decode_init();

	int	 size = 4096;

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

	for (int i = 0; i < nsamples; i++)
	{
		((short *) *data)[2 * i]	= pcm_l[i];
		((short *) *data)[2 * i + 1]	= pcm_r[i];
	}

	delete [] pcm_l;
	delete [] pcm_r;

	return nsamples * 4;
}

bonkFormatInfo *FilterInLAME::GetFileInfo(String inFile)
{
	ex_lame_decode_init();

	bonkFormatInfo	*nFormat = new bonkFormatInfo;
	InStream	*f_in = new InStream(STREAM_FILE, inFile, IS_READONLY);

	nFormat->order = BYTE_INTEL;
	nFormat->bits = 16;
	nFormat->fileSize = f_in->Size();

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

		nFormat->channels = mp3data.stereo;
		nFormat->rate = mp3data.samplerate;
		nFormat->length = -1;

		delete [] data;

		if (mp3data.stereo <= 2 && mp3data.stereo > 0) break;
	}
	while (f_in->GetPos() < f_in->Size());

	delete f_in;

	ex_lame_decode_exit();

	ID3_Tag	*tag = new ID3_Tag(inFile);

	if (tag->NumFrames() > 0)
	{
		ID3_Frame	*frame;
		ID3_Field	*field;
		int		 tbufsize = 1024;
		char		*abuffer = new char [tbufsize];
		wchar_t		*wbuffer = new wchar_t [tbufsize];

		nFormat->trackInfo->track = -1;
		nFormat->trackInfo->outfile = NIL;
		nFormat->trackInfo->hasText = True;

		ZeroMemory(abuffer, tbufsize);
		ZeroMemory(wbuffer, 2 * tbufsize);

		if ((frame = tag->Find(ID3FID_LEADARTIST)) != NIL)
			if ((field = frame->GetField(ID3FN_TEXTENC)) != NIL)
				if (field->Get() == ID3TE_ASCII)
				{
					if ((field = frame->GetField(ID3FN_TEXT)) != NIL) field->Get(abuffer, tbufsize);

					nFormat->trackInfo->artist = abuffer;
				}
				else if (field->Get() == ID3TE_UNICODE)
				{
					if ((field = frame->GetField(ID3FN_TEXT)) != NIL) field->Get((unicode_t *) wbuffer, tbufsize);

					nFormat->trackInfo->artist = wbuffer;
				}

		ZeroMemory(abuffer, tbufsize);
		ZeroMemory(wbuffer, 2 * tbufsize);

		if ((frame = tag->Find(ID3FID_TITLE)) != NIL)
			if ((field = frame->GetField(ID3FN_TEXTENC)) != NIL)
				if (field->Get() == ID3TE_ASCII)
				{
					if ((field = frame->GetField(ID3FN_TEXT)) != NIL) field->Get(abuffer, tbufsize);

					nFormat->trackInfo->title = abuffer;
				}
				else if (field->Get() == ID3TE_UNICODE)
				{
					if ((field = frame->GetField(ID3FN_TEXT)) != NIL) field->Get((unicode_t *) wbuffer, tbufsize);

					nFormat->trackInfo->title = wbuffer;
				}

		ZeroMemory(abuffer, tbufsize);
		ZeroMemory(wbuffer, 2 * tbufsize);

		if ((frame = tag->Find(ID3FID_ALBUM)) != NIL)
			if ((field = frame->GetField(ID3FN_TEXTENC)) != NIL)
				if (field->Get() == ID3TE_ASCII)
				{
					if ((field = frame->GetField(ID3FN_TEXT)) != NIL) field->Get(abuffer, tbufsize);

					nFormat->trackInfo->album = abuffer;
				}
				else if (field->Get() == ID3TE_UNICODE)
				{
					if ((field = frame->GetField(ID3FN_TEXT)) != NIL) field->Get((unicode_t *) wbuffer, tbufsize);

					nFormat->trackInfo->album = wbuffer;
				}

		ZeroMemory(abuffer, tbufsize);
		ZeroMemory(wbuffer, 2 * tbufsize);

		if ((frame = tag->Find(ID3FID_CONTENTTYPE)) != NIL)
			if ((field = frame->GetField(ID3FN_TEXTENC)) != NIL)
				if (field->Get() == ID3TE_ASCII)
				{
					if ((field = frame->GetField(ID3FN_TEXT)) != NIL) field->Get(abuffer, tbufsize);

					int startByte = 0;

					if (abuffer[0] == '(')
					{
						for (int i = 0; i < tbufsize; i++)
						{
							startByte++;

							if (abuffer[i] == ')') break;
						}
					}

					if ((abuffer + startByte)[0] != 0)	nFormat->trackInfo->genre = abuffer + startByte;
					else if (startByte > 0)			nFormat->trackInfo->genre = GetID3CategoryName(String(abuffer + 1).ToInt());
				}
				else if (field->Get() == ID3TE_UNICODE)
				{
					if ((field = frame->GetField(ID3FN_TEXT)) != NIL) field->Get((unicode_t *) wbuffer, tbufsize);

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

		if ((frame = tag->Find(ID3FID_YEAR)) != NIL)
			if ((field = frame->GetField(ID3FN_TEXTENC)) != NIL)
				if (field->Get() == ID3TE_ASCII)
				{
					if ((field = frame->GetField(ID3FN_TEXT)) != NIL) field->Get(abuffer, tbufsize);

					nFormat->trackInfo->year = String(abuffer).ToInt();
				}
				else if (field->Get() == ID3TE_UNICODE)
				{
					if ((field = frame->GetField(ID3FN_TEXT)) != NIL) field->Get((unicode_t *) wbuffer, tbufsize);

					nFormat->trackInfo->year = String(wbuffer).ToInt();
				}

		ZeroMemory(abuffer, tbufsize);
		ZeroMemory(wbuffer, 2 * tbufsize);

		if ((frame = tag->Find(ID3FID_TRACKNUM)) != NIL)
			if ((field = frame->GetField(ID3FN_TEXTENC)) != NIL)
				if (field->Get() == ID3TE_ASCII)
				{
					if ((field = frame->GetField(ID3FN_TEXT)) != NIL) field->Get(abuffer, tbufsize);

					nFormat->trackInfo->track = String(abuffer).ToInt();
				}
				else if (field->Get() == ID3TE_UNICODE)
				{
					if ((field = frame->GetField(ID3FN_TEXT)) != NIL) field->Get((unicode_t *) wbuffer, tbufsize);

					nFormat->trackInfo->track = String(wbuffer).ToInt();
				}

		delete [] abuffer;
		delete [] wbuffer;
	}

	delete tag;

	return nFormat;
}
