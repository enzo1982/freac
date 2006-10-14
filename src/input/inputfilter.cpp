 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/inputfilter.h>

#include <dllinterfaces.h>

BonkEnc::InputFilter::InputFilter(Config *config, Track *iFormat)
{
	error		= 0;
	inBytes		= 0;
	fileSize	= 0;
	format		= iFormat;
	currentConfig	= config;
}

BonkEnc::InputFilter::~InputFilter()
{
}

Bool BonkEnc::InputFilter::SetFileSize(UnsignedInt newFileSize)
{
	fileSize = newFileSize;

	return true;
}

Int BonkEnc::InputFilter::GetInBytes()
{
	return inBytes;
}

Bool BonkEnc::InputFilter::ParseID3V2Tag(unsigned char *buffer, Int size, Track *nFormat)
{
	ID3Tag		*tag = ex_ID3Tag_New();

	ex_ID3Tag_ParseBuffer(tag, buffer, size);

	Bool	 retVal = ParseID3V2Tag(tag, nFormat);

	ex_ID3Tag_Delete(tag);

	return retVal;
}

Bool BonkEnc::InputFilter::ParseID3V2Tag(const String &fileName, Track *nFormat)
{
	ID3Tag	*tag = ex_ID3Tag_New();

	ex_ID3Tag_Link(tag, CreateTempFile(fileName));

	RemoveTempFile(fileName);

	Bool	 retVal = ParseID3V2Tag(tag, nFormat);

	ex_ID3Tag_Delete(tag);

	return retVal;
}

Bool BonkEnc::InputFilter::ParseID3V2Tag(ID3Tag *tag, Track *nFormat)
{
	if (ex_ID3Tag_NumFrames(tag) > 0)
	{
		ID3Frame	*frame;
		ID3Field	*field;
		int		 tbufsize = 1024;
		char		*abuffer = new char [tbufsize];
		wchar_t		*wbuffer = new wchar_t [tbufsize];

		ZeroMemory(abuffer, tbufsize);
		ZeroMemory(wbuffer, 2 * tbufsize);

		if ((frame = ex_ID3Tag_FindFrameWithID(tag, ID3FID_LEADARTIST)) != NIL)
			if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXTENC)) != NIL)
			{
				Int	 encoding = ex_ID3Field_GetINT(field);

				if (encoding == ID3TE_ISO8859_1 || encoding == ID3TE_UTF8)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetASCII(field, abuffer, tbufsize);

					if (encoding == ID3TE_ISO8859_1)	nFormat->artist.ImportFrom("ISO-8859-1", abuffer);
					else if (encoding == ID3TE_UTF8)	nFormat->artist.ImportFrom("UTF-8", abuffer);
				}
				else if (encoding == ID3TE_UTF16 || encoding == ID3TE_UTF16BE)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetUNICODE(field, (unicode_t *) wbuffer, tbufsize);

					nFormat->artist.ImportFrom("UTF-16BE", (char *) wbuffer);
				}
			}

		ZeroMemory(abuffer, tbufsize);
		ZeroMemory(wbuffer, 2 * tbufsize);

		if ((frame = ex_ID3Tag_FindFrameWithID(tag, ID3FID_TITLE)) != NIL)
			if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXTENC)) != NIL)
			{
				Int	 encoding = ex_ID3Field_GetINT(field);

				if (encoding == ID3TE_ISO8859_1 || encoding == ID3TE_UTF8)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetASCII(field, abuffer, tbufsize);

					if (encoding == ID3TE_ISO8859_1)	nFormat->title.ImportFrom("ISO-8859-1", abuffer);
					else if (encoding == ID3TE_UTF8)	nFormat->title.ImportFrom("UTF-8", abuffer);
				}
				else if (encoding == ID3TE_UTF16 || encoding == ID3TE_UTF16BE)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetUNICODE(field, (unicode_t *) wbuffer, tbufsize);

					nFormat->title.ImportFrom("UTF-16BE", (char *) wbuffer);
				}
			}

		ZeroMemory(abuffer, tbufsize);
		ZeroMemory(wbuffer, 2 * tbufsize);

		if ((frame = ex_ID3Tag_FindFrameWithID(tag, ID3FID_ALBUM)) != NIL)
			if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXTENC)) != NIL)
			{
				Int	 encoding = ex_ID3Field_GetINT(field);

				if (encoding == ID3TE_ISO8859_1 || encoding == ID3TE_UTF8)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetASCII(field, abuffer, tbufsize);

					if (encoding == ID3TE_ISO8859_1)	nFormat->album.ImportFrom("ISO-8859-1", abuffer);
					else if (encoding == ID3TE_UTF8)	nFormat->album.ImportFrom("UTF-8", abuffer);
				}
				else if (encoding == ID3TE_UTF16 || encoding == ID3TE_UTF16BE)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetUNICODE(field, (unicode_t *) wbuffer, tbufsize);

					nFormat->album.ImportFrom("UTF-16BE", (char *) wbuffer);
				}
			}

		ZeroMemory(abuffer, tbufsize);
		ZeroMemory(wbuffer, 2 * tbufsize);

		if ((frame = ex_ID3Tag_FindFrameWithID(tag, ID3FID_CONTENTTYPE)) != NIL)
			if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXTENC)) != NIL)
			{
				Int	 encoding = ex_ID3Field_GetINT(field);

				if (encoding == ID3TE_ISO8859_1 || encoding == ID3TE_UTF8)
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

					if ((abuffer + startByte)[0] != 0 && encoding == ID3TE_ISO8859_1)	nFormat->genre.ImportFrom("ISO-8859-1", abuffer + startByte);
					else if ((abuffer + startByte)[0] != 0 && encoding == ID3TE_UTF8)	nFormat->genre.ImportFrom("UTF-8", abuffer + startByte);
					else if (startByte > 0)							nFormat->genre = GetID3CategoryName(String(abuffer + 1).ToInt());
				}
				else if (encoding == ID3TE_UTF16 || encoding == ID3TE_UTF16BE)
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

					nFormat->genre.ImportFrom("UTF-16BE", (char *) wbuffer);
				}
			}

		ZeroMemory(abuffer, tbufsize);
		ZeroMemory(wbuffer, 2 * tbufsize);

		if ((frame = ex_ID3Tag_FindFrameWithID(tag, ID3FID_YEAR)) != NIL)
			if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXTENC)) != NIL)
			{
				Int	 encoding = ex_ID3Field_GetINT(field);

				if (encoding == ID3TE_ISO8859_1 || encoding == ID3TE_UTF8)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetASCII(field, abuffer, tbufsize);

					String	 year;

					if (encoding == ID3TE_ISO8859_1)	year.ImportFrom("ISO-8859-1", abuffer);
					else if (encoding == ID3TE_UTF8)	year.ImportFrom("UTF-8", abuffer);

					nFormat->year = year.ToInt();
				}
				else if (encoding == ID3TE_UTF16 || encoding == ID3TE_UTF16BE)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetUNICODE(field, (unicode_t *) wbuffer, tbufsize);

					String	 year;

					year.ImportFrom("UTF-16BE", (char *) wbuffer);

					nFormat->year = year.ToInt();
				}
			}

		ZeroMemory(abuffer, tbufsize);
		ZeroMemory(wbuffer, 2 * tbufsize);

		if ((frame = ex_ID3Tag_FindFrameWithID(tag, ID3FID_TRACKNUM)) != NIL)
			if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXTENC)) != NIL)
			{
				Int	 encoding = ex_ID3Field_GetINT(field);

				if (encoding == ID3TE_ISO8859_1 || encoding == ID3TE_UTF8)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetASCII(field, abuffer, tbufsize);

					String	 track;

					if (encoding == ID3TE_ISO8859_1)	track.ImportFrom("ISO-8859-1", abuffer);
					else if (encoding == ID3TE_UTF8)	track.ImportFrom("UTF-8", abuffer);

					nFormat->track = track.ToInt();
				}
				else if (encoding == ID3TE_UTF16 || encoding == ID3TE_UTF16BE)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL) ex_ID3Field_GetUNICODE(field, (unicode_t *) wbuffer, tbufsize);

					String	 track;

					track.ImportFrom("UTF-16BE", (char *) wbuffer);

					nFormat->track = track.ToInt();
				}
			}

		delete [] abuffer;
		delete [] wbuffer;
	}

	return True;
}

String BonkEnc::InputFilter::GetID3CategoryName(Int id)
{
	if (id < 0 || id > 147) return "";

	String	 array[148] = { "Blues", "Classic Rock", "Country", "Dance", "Disco", "Funk", "Grunge", "Hip-Hop", "Jazz",
				"Metal", "New Age", "Oldies", "Other", "Pop", "R&B", "Rap", "Reggae", "Rock", "Techno",
				"Industrial", "Alternative", "Ska", "Death Metal", "Pranks", "Soundtrack", "Euro-Techno",
				"Ambient", "Trip-Hop", "Vocal", "Jazz+Funk", "Fusion", "Trance", "Classical", "Instrumental",
				"Acid", "House", "Game", "Sound Clip", "Gospel", "Noise", "Alt. Rock", "Bass", "Soul",
				"Punk", "Space", "Meditative", "Instrumental Pop", "Instrumental Rock", "Ethnic", "Gothic",
				"Darkwave", "Techno-Industrial", "Electronic", "Pop-Folk", "Eurodance", "Dream", "Southern Rock",
				"Comedy", "Cult", "Gangsta Rap", "Top 40", "Christian Rap", "Pop/Funk", "Jungle",
				"Native American", "Cabaret", "New Wave", "Psychedelic", "Rave", "Showtunes", "Trailer",
				"Lo-Fi", "Tribal", "Acid Punk", "Acid Jazz", "Polka", "Retro", "Musical", "Rock & Roll",
				"Hard Rock", "Folk", "Folk/Rock", "National Folk", "Swing", "Fast-Fusion", "Bebob", "Latin",
				"Revival", "Celtic", "Bluegrass", "Avantgarde", "Gothic Rock", "Progressive Rock",
				"Psychedelic Rock", "Symphonic Rock", "Slow Rock", "Big Band", "Chorus", "Easy Listening",
				"Acoustic", "Humour", "Speech", "Chanson", "Opera", "Chamber Music", "Sonata", "Symphony",
				"Booty Bass", "Primus", "Porn Groove", "Satire", "Slow Jam", "Club", "Tango", "Samba",
				"Folklore", "Ballad", "Power Ballad", "Rhythmic Soul", "Freestyle", "Duet", "Punk Rock",
				"Drum Solo", "A Capella", "Euro-House", "Dance Hall", "Goa", "Drum & Bass", "Club-House",
				"Hardcore", "Terror", "Indie", "BritPop", "Negerpunk", "Polsk Punk", "Beat",
				"Christian Gangsta Rap", "Heavy Metal", "Black Metal", "Crossover", "Contemporary Christian",
				"Christian Rock", "Merengue", "Salsa", "Thrash Metal", "Anime", "JPop", "Synthpop" };

	return array[id];
}

String BonkEnc::InputFilter::GetTempFileName(const String &oFileName)
{
	String	 rVal	= oFileName;
	Int	 lastBs	= -1;

	for (Int i = 0; i < rVal.Length(); i++)
	{
		if (rVal[i] > 255)	rVal[i] = '#';
		if (rVal[i] == '\\')	lastBs = i;
	}

	if (rVal == oFileName) return rVal;

	char	*tempa = new char [MAX_PATH];

	GetTempPathA(MAX_PATH, tempa);

	String	 tempDir = tempa;

	delete [] tempa;

	if (tempDir[tempDir.Length() - 1] != '\\') tempDir.Append("\\");

	for (Int j = lastBs + 1; j < rVal.Length(); j++)
	{
		tempDir[tempDir.Length()] = rVal[j];
	}

	return tempDir.Append(".out.temp");
}

String BonkEnc::InputFilter::CreateTempFile(const String &oFileName)
{
	String		 tempFileName = GetTempFileName(oFileName);

	if (tempFileName == oFileName) return oFileName;

	InStream	*in = new InStream(STREAM_FILE, oFileName, IS_READONLY);
	OutStream	*out = new OutStream(STREAM_FILE, tempFileName, OS_OVERWRITE);

	Buffer<unsigned char>	 buffer;

	buffer.Resize(1024);

	Int	 bytesleft = in->Size();

	while (bytesleft > 0)
	{
		out->OutputData(in->InputData(buffer, Math::Min(1024, bytesleft)), Math::Min(1024, bytesleft));

		bytesleft -= 1024;
	}

	delete in;
	delete out;

	return tempFileName;
}

Bool BonkEnc::InputFilter::RemoveTempFile(const String &oFileName)
{
	String		 tempFileName = GetTempFileName(oFileName);

	if (tempFileName == oFileName) return True;

	File(tempFileName).Delete();

	return True;
}
