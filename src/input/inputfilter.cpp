 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/inputfilter.h>

#include <iolib/drivers/driver_posix.h>
#include <iolib/drivers/driver_unicode.h>

InputFilter::InputFilter(bonkEncConfig *config, bonkEncTrack *iFormat)
{
	error		= 0;
	inBytes		= 0;
	fileSize	= 0;
	format		= iFormat;
	currentConfig	= config;
}

InputFilter::~InputFilter()
{
}

S::Bool InputFilter::SetFileSize(UnsignedInt newFileSize)
{
	fileSize = newFileSize;

	return true;
}

S::Int InputFilter::GetInBytes()
{
	return inBytes;
}

InStream *InputFilter::OpenFile(String fileName)
{
	if (Setup::enableUnicode)	iolibDriver = new IOLibDriverUnicode(fileName, IS_READONLY);
	else				iolibDriver = new IOLibDriverPOSIX(fileName, IS_READONLY);

	return new InStream(STREAM_DRIVER, iolibDriver);
}

S::Int InputFilter::CloseFile(InStream *stream)
{
	delete stream;
	delete iolibDriver;

	return Success;
}

S::Bool InputFilter::ParseID3V2Tag(unsigned char *buffer, Int size, bonkEncTrack *nFormat)
{
	ID3Tag		*tag = ex_ID3Tag_New();

	ex_ID3Tag_ParseBuffer(tag, buffer, size);

	Bool	 retVal = ParseID3V2Tag(tag, nFormat);

	ex_ID3Tag_Delete(tag);

	return retVal;
}

S::Bool InputFilter::ParseID3V2Tag(String fileName, bonkEncTrack *nFormat)
{
	ID3Tag	*tag = ex_ID3Tag_New();

	ex_ID3Tag_Link(tag, fileName);

	Bool	 retVal = ParseID3V2Tag(tag, nFormat);

	ex_ID3Tag_Delete(tag);

	return retVal;
}

S::Bool InputFilter::ParseID3V2Tag(ID3Tag *tag, bonkEncTrack *nFormat)
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

S::String InputFilter::GetID3CategoryName(Int id)
{
	if (id == 0)	return "Blues";
	if (id == 1)	return "Classic Rock";
	if (id == 2)	return "Country";
	if (id == 3)	return "Dance";
	if (id == 4)	return "Disco";
	if (id == 5)	return "Funk";
	if (id == 6)	return "Grunge";
	if (id == 7)	return "Hip-Hop";
	if (id == 8)	return "Jazz";
	if (id == 9)	return "Metal";
	if (id == 10)	return "New Age";
	if (id == 11)	return "Oldies";
	if (id == 12)	return "Other";
	if (id == 13)	return "Pop";
	if (id == 14)	return "R&B";
	if (id == 15)	return "Rap";
	if (id == 16)	return "Reggae";
	if (id == 17)	return "Rock";
	if (id == 18)	return "Techno";
	if (id == 19)	return "Industrial";
	if (id == 20)	return "Alternative";
	if (id == 21)	return "Ska";
	if (id == 22)	return "Death Metal";
	if (id == 23)	return "Pranks";
	if (id == 24)	return "Soundtrack";
	if (id == 25)	return "Euro-Techno";
	if (id == 26)	return "Ambient";
	if (id == 27)	return "Trip-Hop";
	if (id == 28)	return "Vocal";
	if (id == 29)	return "Jazz+Funk";
	if (id == 30)	return "Fusion";
	if (id == 31)	return "Trance";
	if (id == 32)	return "Classical";
	if (id == 33)	return "Instrumental";
	if (id == 34)	return "Acid";
	if (id == 35)	return "House";
	if (id == 36)	return "Game";
	if (id == 37)	return "Sound Clip";
	if (id == 38)	return "Gospel";
	if (id == 39)	return "Noise";
	if (id == 40)	return "Alt. Rock";
	if (id == 41)	return "Bass";
	if (id == 42)	return "Soul";
	if (id == 43)	return "Punk";
	if (id == 44)	return "Space";
	if (id == 45)	return "Meditative";
	if (id == 46)	return "Instrumental Pop";
	if (id == 47)	return "Instrumental Rock";
	if (id == 48)	return "Ethnic";
	if (id == 49)	return "Gothic";
	if (id == 50)	return "Darkwave";
	if (id == 51)	return "Techno-Industrial";
	if (id == 52)	return "Electronic";
	if (id == 53)	return "Pop-Folk";
	if (id == 54)	return "Eurodance";
	if (id == 55)	return "Dream";
	if (id == 56)	return "Southern Rock";
	if (id == 57)	return "Comedy";
	if (id == 58)	return "Cult";
	if (id == 59)	return "Gangsta Rap";
	if (id == 60)	return "Top 40";
	if (id == 61)	return "Christian Rap";
	if (id == 62)	return "Pop/Funk";
	if (id == 63)	return "Jungle";
	if (id == 64)	return "Native American";
	if (id == 65)	return "Cabaret";
	if (id == 66)	return "New Wave";
	if (id == 67)	return "Psychedelic";
	if (id == 68)	return "Rave";
	if (id == 69)	return "Showtunes";
	if (id == 70)	return "Trailer";
	if (id == 71)	return "Lo-Fi";
	if (id == 72)	return "Tribal";
	if (id == 73)	return "Acid Punk";
	if (id == 74)	return "Acid Jazz";
	if (id == 75)	return "Polka";
	if (id == 76)	return "Retro";
	if (id == 77)	return "Musical";
	if (id == 78)	return "Rock & Roll";
	if (id == 79)	return "Hard Rock";
	if (id == 80)	return "Folk";
	if (id == 81)	return "Folk/Rock";
	if (id == 82)	return "National Folk";
	if (id == 83)	return "Swing";
	if (id == 84)	return "Fast-Fusion";
	if (id == 85)	return "Bebob";
	if (id == 86)	return "Latin";
	if (id == 87)	return "Revival";
	if (id == 88)	return "Celtic";
	if (id == 89)	return "Bluegrass";
	if (id == 90)	return "Avantgarde";
	if (id == 91)	return "Gothic Rock";
	if (id == 92)	return "Progressive Rock";
	if (id == 93)	return "Psychedelic Rock";
	if (id == 94)	return "Symphonic Rock";
	if (id == 95)	return "Slow Rock";
	if (id == 96)	return "Big Band";
	if (id == 97)	return "Chorus";
	if (id == 98)	return "Easy Listening";
	if (id == 99)	return "Acoustic";
	if (id == 100)	return "Humour";
	if (id == 101)	return "Speech";
	if (id == 102)	return "Chanson";
	if (id == 103)	return "Opera";
	if (id == 104)	return "Chamber Music";
	if (id == 105)	return "Sonata";
	if (id == 106)	return "Symphony";
	if (id == 107)	return "Booty Bass";
	if (id == 108)	return "Primus";
	if (id == 109)	return "Porn Groove";
	if (id == 110)	return "Satire";
	if (id == 111)	return "Slow Jam";
	if (id == 112)	return "Club";
	if (id == 113)	return "Tango";
	if (id == 114)	return "Samba";
	if (id == 115)	return "Folklore";
	if (id == 116)	return "Ballad";
	if (id == 117)	return "Power Ballad";
	if (id == 118)	return "Rhythmic Soul";
	if (id == 119)	return "Freestyle";
	if (id == 120)	return "Duet";
	if (id == 121)	return "Punk Rock";
	if (id == 122)	return "Drum Solo";
	if (id == 123)	return "A Capella";
	if (id == 124)	return "Euro-House";
	if (id == 125)	return "Dance Hall";
	if (id == 126)	return "Goa";
	if (id == 127)	return "Drum & Bass";
	if (id == 128)	return "Club-House";
	if (id == 129)	return "Hardcore";
	if (id == 130)	return "Terror";
	if (id == 131)	return "Indie";
	if (id == 132)	return "BritPop";
	if (id == 133)	return "Negerpunk";
	if (id == 134)	return "Polsk Punk";
	if (id == 135)	return "Beat";
	if (id == 136)	return "Christian Gangsta Rap";
	if (id == 137)	return "Heavy Metal";
	if (id == 138)	return "Black Metal";
	if (id == 139)	return "Crossover";
	if (id == 140)	return "Contemporary Christian";
	if (id == 141)	return "Christian Rock";
	if (id == 142)	return "Merengue";
	if (id == 143)	return "Salsa";
	if (id == 144)	return "Thrash Metal";
	if (id == 145)	return "Anime";
	if (id == 146)	return "JPop";
	if (id == 147)	return "Synthpop";

	return "";
}
