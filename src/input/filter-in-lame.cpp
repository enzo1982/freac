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
#include <input/filter-in-lame.h>
#include <dllinterfaces.h>
#include <3rdparty/id3/tag.h>

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

bonkEncTrack *FilterInLAME::GetFileInfo(String inFile)
{
	ex_lame_decode_init();

	bonkEncTrack	*nFormat = new bonkEncTrack;
	InStream	*f_in = OpenFile(inFile);

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

	CloseFile(f_in);

	ex_lame_decode_exit();

	if (currentConfig->enable_id3)
	{
		ID3Tag	*tag = ex_ID3Tag_New();

		ex_ID3Tag_Link(tag, inFile);

		if (ex_ID3Tag_NumFrames(tag) > 0)
		{
			ID3Frame	*frame;
			ID3Field	*field;
			int		 tbufsize = 1024;
			char		*abuffer = new char [tbufsize];
			wchar_t		*wbuffer = new wchar_t [tbufsize];

			nFormat->track = -1;
			nFormat->outfile = NIL;

			ZeroMemory(abuffer, tbufsize);
			ZeroMemory(wbuffer, 2 * tbufsize);

			if ((frame = ex_ID3Tag_FindFrameWithID(tag, ID3FID_LEADARTIST)) != NIL)
				if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXTENC)) != NIL)
					if (ex_ID3Field_GetINT(field) == ID3TE_ASCII)
					{
						if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetASCII(field, abuffer, tbufsize);

						nFormat->artist.ImportFrom("UTF-8", abuffer);
					}
					else if (ex_ID3Field_GetINT(field) == ID3TE_UNICODE)
					{
						if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetUNICODE(field, (unicode_t *) wbuffer, tbufsize);

						nFormat->artist = wbuffer;
					}

			ZeroMemory(abuffer, tbufsize);
			ZeroMemory(wbuffer, 2 * tbufsize);

			if ((frame = ex_ID3Tag_FindFrameWithID(tag, ID3FID_TITLE)) != NIL)
				if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXTENC)) != NIL)
					if (ex_ID3Field_GetINT(field) == ID3TE_ASCII)
					{
						if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetASCII(field, abuffer, tbufsize);

						nFormat->title.ImportFrom("UTF-8", abuffer);
					}
					else if (ex_ID3Field_GetINT(field) == ID3TE_UNICODE)
					{
						if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetUNICODE(field, (unicode_t *) wbuffer, tbufsize);

						nFormat->title = wbuffer;
					}

			ZeroMemory(abuffer, tbufsize);
			ZeroMemory(wbuffer, 2 * tbufsize);

			if ((frame = ex_ID3Tag_FindFrameWithID(tag, ID3FID_ALBUM)) != NIL)
				if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXTENC)) != NIL)
					if (ex_ID3Field_GetINT(field) == ID3TE_ASCII)
					{
						if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetASCII(field, abuffer, tbufsize);

						nFormat->album.ImportFrom("UTF-8", abuffer);
					}
					else if (ex_ID3Field_GetINT(field) == ID3TE_UNICODE)
					{
						if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetUNICODE(field, (unicode_t *) wbuffer, tbufsize);

						nFormat->album = wbuffer;
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

						if ((abuffer + startByte)[0] != 0)	nFormat->genre.ImportFrom("UTF-8", abuffer + startByte);
						else if (startByte > 0)			nFormat->genre = GetID3CategoryName(String(abuffer + 1).ToInt());
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

						nFormat->genre = wbuffer;
					}

			ZeroMemory(abuffer, tbufsize);
			ZeroMemory(wbuffer, 2 * tbufsize);

			if ((frame = ex_ID3Tag_FindFrameWithID(tag, ID3FID_YEAR)) != NIL)
				if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXTENC)) != NIL)
					if (ex_ID3Field_GetINT(field) == ID3TE_ASCII)
					{
						if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetASCII(field, abuffer, tbufsize);

						nFormat->year = String(abuffer).ToInt();
					}
					else if (ex_ID3Field_GetINT(field) == ID3TE_UNICODE)
					{
						if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetUNICODE(field, (unicode_t *) wbuffer, tbufsize);

						nFormat->year = String(wbuffer).ToInt();
					}

			ZeroMemory(abuffer, tbufsize);
			ZeroMemory(wbuffer, 2 * tbufsize);

			if ((frame = ex_ID3Tag_FindFrameWithID(tag, ID3FID_TRACKNUM)) != NIL)
				if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXTENC)) != NIL)
					if (ex_ID3Field_GetINT(field) == ID3TE_ASCII)
					{
						if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetASCII(field, abuffer, tbufsize);

						nFormat->track = String(abuffer).ToInt();
					}
					else if (ex_ID3Field_GetINT(field) == ID3TE_UNICODE)
					{
						if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetUNICODE(field, (unicode_t *) wbuffer, tbufsize);

						nFormat->track = String(wbuffer).ToInt();
					}

			delete [] abuffer;
			delete [] wbuffer;
		}

		ex_ID3Tag_Delete(tag);
	}

	return nFormat;
}
