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
	switch (id)
	{
		default:  return "";
		case 0:   return "Blues";
		case 1:   return "Classic Rock";
		case 2:   return "Country";
		case 3:   return "Dance";
		case 4:   return "Disco";
		case 5:   return "Funk";
		case 6:   return "Grunge";
		case 7:   return "Hip-Hop";
		case 8:   return "Jazz";
		case 9:   return "Metal";
		case 10:  return "New Age";
		case 11:  return "Oldies";
		case 12:  return "Other";
		case 13:  return "Pop";
		case 14:  return "R&B";
		case 15:  return "Rap";
		case 16:  return "Reggae";
		case 17:  return "Rock";
		case 18:  return "Techno";
		case 19:  return "Industrial";
		case 20:  return "Alternative";
		case 21:  return "Ska";
		case 22:  return "Death Metal";
		case 23:  return "Pranks";
		case 24:  return "Soundtrack";
		case 25:  return "Euro-Techno";
		case 26:  return "Ambient";
		case 27:  return "Trip-Hop";
		case 28:  return "Vocal";
		case 29:  return "Jazz+Funk";
		case 30:  return "Fusion";
		case 31:  return "Trance";
		case 32:  return "Classical";
		case 33:  return "Instrumental";
		case 34:  return "Acid";
		case 35:  return "House";
		case 36:  return "Game";
		case 37:  return "Sound Clip";
		case 38:  return "Gospel";
		case 39:  return "Noise";
		case 40:  return "Alt. Rock";
		case 41:  return "Bass";
		case 42:  return "Soul";
		case 43:  return "Punk";
		case 44:  return "Space";
		case 45:  return "Meditative";
		case 46:  return "Instrumental Pop";
		case 47:  return "Instrumental Rock";
		case 48:  return "Ethnic";
		case 49:  return "Gothic";
		case 50:  return "Darkwave";
		case 51:  return "Techno-Industrial";
		case 52:  return "Electronic";
		case 53:  return "Pop-Folk";
		case 54:  return "Eurodance";
		case 55:  return "Dream";
		case 56:  return "Southern Rock";
		case 57:  return "Comedy";
		case 58:  return "Cult";
		case 59:  return "Gangsta Rap";
		case 60:  return "Top 40";
		case 61:  return "Christian Rap";
		case 62:  return "Pop/Funk";
		case 63:  return "Jungle";
		case 64:  return "Native American";
		case 65:  return "Cabaret";
		case 66:  return "New Wave";
		case 67:  return "Psychedelic";
		case 68:  return "Rave";
		case 69:  return "Showtunes";
		case 70:  return "Trailer";
		case 71:  return "Lo-Fi";
		case 72:  return "Tribal";
		case 73:  return "Acid Punk";
		case 74:  return "Acid Jazz";
		case 75:  return "Polka";
		case 76:  return "Retro";
		case 77:  return "Musical";
		case 78:  return "Rock & Roll";
		case 79:  return "Hard Rock";
		case 80:  return "Folk";
		case 81:  return "Folk/Rock";
		case 82:  return "National Folk";
		case 83:  return "Swing";
		case 84:  return "Fast-Fusion";
		case 85:  return "Bebob";
		case 86:  return "Latin";
		case 87:  return "Revival";
		case 88:  return "Celtic";
		case 89:  return "Bluegrass";
		case 90:  return "Avantgarde";
		case 91:  return "Gothic Rock";
		case 92:  return "Progressive Rock";
		case 93:  return "Psychedelic Rock";
		case 94:  return "Symphonic Rock";
		case 95:  return "Slow Rock";
		case 96:  return "Big Band";
		case 97:  return "Chorus";
		case 98:  return "Easy Listening";
		case 99:  return "Acoustic";
		case 100: return "Humour";
		case 101: return "Speech";
		case 102: return "Chanson";
		case 103: return "Opera";
		case 104: return "Chamber Music";
		case 105: return "Sonata";
		case 106: return "Symphony";
		case 107: return "Booty Bass";
		case 108: return "Primus";
		case 109: return "Porn Groove";
		case 110: return "Satire";
		case 111: return "Slow Jam";
		case 112: return "Club";
		case 113: return "Tango";
		case 114: return "Samba";
		case 115: return "Folklore";
		case 116: return "Ballad";
		case 117: return "Power Ballad";
		case 118: return "Rhythmic Soul";
		case 119: return "Freestyle";
		case 120: return "Duet";
		case 121: return "Punk Rock";
		case 122: return "Drum Solo";
		case 123: return "A Capella";
		case 124: return "Euro-House";
		case 125: return "Dance Hall";
		case 126: return "Goa";
		case 127: return "Drum & Bass";
		case 128: return "Club-House";
		case 129: return "Hardcore";
		case 130: return "Terror";
		case 131: return "Indie";
		case 132: return "BritPop";
		case 133: return "Negerpunk";
		case 134: return "Polsk Punk";
		case 135: return "Beat";
		case 136: return "Christian Gangsta Rap";
		case 137: return "Heavy Metal";
		case 138: return "Black Metal";
		case 139: return "Crossover";
		case 140: return "Contemporary Christian";
		case 141: return "Christian Rock";
		case 142: return "Merengue";
		case 143: return "Salsa";
		case 144: return "Thrash Metal";
		case 145: return "Anime";
		case 146: return "JPop";
		case 147: return "Synthpop";
	}
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
