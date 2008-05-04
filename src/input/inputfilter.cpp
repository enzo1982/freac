 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/inputfilter.h>

#include <dllinterfaces.h>
#include <utilities.h>

BonkEnc::InputFilter::InputFilter(Config *config, Track *iFormat)
{
	errorState	= False;
	errorString	= "Unknown error";

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
	ID3TagIterator	*iterator = ex_ID3Tag_CreateIterator(tag);

	for (UnsignedInt i = 0; i < ex_ID3Tag_NumFrames(tag); i++)
	{
		ID3Frame	*frame = ex_ID3TagIterator_GetNext(iterator);

		if	(ex_ID3Frame_GetID(frame) == ID3FID_LEADARTIST)	nFormat->artist = GetID3V2FrameString(frame);
		else if (ex_ID3Frame_GetID(frame) == ID3FID_TITLE)	nFormat->title = GetID3V2FrameString(frame);
		else if (ex_ID3Frame_GetID(frame) == ID3FID_ALBUM)	nFormat->album = GetID3V2FrameString(frame);
		else if (ex_ID3Frame_GetID(frame) == ID3FID_TRACKNUM)	nFormat->track = GetID3V2FrameString(frame).ToInt();
		else if (ex_ID3Frame_GetID(frame) == ID3FID_YEAR)	nFormat->year = GetID3V2FrameString(frame).ToInt();
		else if (ex_ID3Frame_GetID(frame) == ID3FID_PUBLISHER)	nFormat->label = GetID3V2FrameString(frame);
		else if (ex_ID3Frame_GetID(frame) == ID3FID_ISRC)	nFormat->isrc = GetID3V2FrameString(frame);
		else if (ex_ID3Frame_GetID(frame) == ID3FID_CONTENTTYPE)
		{
			String	 genre = GetID3V2FrameString(frame);
			String	 genreID;

			if (genre[0] == '(')
			{
				for (Int i = 1; i < genre.Length(); i++)
				{
					if (genre[i] == ')') break;

					genreID[i - 1] = genre[i];
				}
			}

			if (genreID == NIL)				nFormat->genre = genre;
			else if (genre.Length() > genreID.Length() + 2)	nFormat->genre = genre.Tail(genre.Length() - genreID.Length() - 2);
			else if (genreID != NIL)			nFormat->genre = GetID3CategoryName(genreID.ToInt());
		}
		else if (ex_ID3Frame_GetID(frame) == ID3FID_PICTURE)
		{
			Picture	*picture = new Picture();

			int	 tbufsize = 1024;
			char	*abuffer  = new char [tbufsize];
			wchar_t	*wbuffer  = new wchar_t [tbufsize];

			ZeroMemory(abuffer, tbufsize);
			ZeroMemory(wbuffer, tbufsize * 2);

			ID3Field	*field = ex_ID3Frame_GetField(frame, ID3FN_MIMETYPE);

			if (field != NIL)
			{
				ex_ID3Field_GetASCII(field, abuffer, tbufsize);

				picture->mime.ImportFrom("ISO-8859-1", abuffer);
			}

			field = ex_ID3Frame_GetField(frame, ID3FN_PICTURETYPE);

			if (field != NIL)
			{
				picture->type = ex_ID3Field_GetINT(field);
			}

			ZeroMemory(abuffer, tbufsize);
			ZeroMemory(wbuffer, tbufsize * 2);

			field = ex_ID3Frame_GetField(frame, ID3FN_TEXTENC);

			if (field != NIL)
			{
				Int	 encoding = ex_ID3Field_GetINT(field);

				if (encoding == ID3TE_ISO8859_1 || encoding == ID3TE_UTF8)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_DESCRIPTION)) != NIL)
					{
						ex_ID3Field_GetASCII(field, abuffer, tbufsize);

						if (encoding == ID3TE_ISO8859_1)	picture->description.ImportFrom("ISO-8859-1", abuffer);
						else if (encoding == ID3TE_UTF8)	picture->description.ImportFrom("UTF-8", abuffer);
					}
				}
				else if (encoding == ID3TE_UTF16 || encoding == ID3TE_UTF16BE)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_DESCRIPTION)) != NIL)
					{
						ex_ID3Field_GetUNICODE(field, (unicode_t *) wbuffer, tbufsize);

						picture->description.ImportFrom("UTF-16BE", (char *) wbuffer);
					}
				}
			}

			field = ex_ID3Frame_GetField(frame, ID3FN_DATA);

			if (field != NIL)
			{
				picture->data.Resize(ex_ID3Field_Size(field));

				ex_ID3Field_GetBINARY(field, picture->data, picture->data.Size());
			}

			delete [] abuffer;
			delete [] wbuffer;

			nFormat->pictures.Add(picture);
		}
	}

	ex_ID3TagIterator_Delete(iterator);

	return True;
}

String BonkEnc::InputFilter::GetID3V2FrameString(ID3Frame *frame)
{
	ID3Field	*field = ex_ID3Frame_GetField(frame, ID3FN_TEXTENC);
	String		 result;

	if (field != NIL)
	{
		Int	 encoding = ex_ID3Field_GetINT(field);

		int	 tbufsize = 1024;
		char	*abuffer  = new char [tbufsize];
		wchar_t	*wbuffer  = new wchar_t [tbufsize];

		ZeroMemory(abuffer, tbufsize);
		ZeroMemory(wbuffer, tbufsize * 2);

		if (encoding == ID3TE_ISO8859_1 || encoding == ID3TE_UTF8)
		{
			if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL)
			{
				ex_ID3Field_GetASCII(field, abuffer, tbufsize);

				if (encoding == ID3TE_ISO8859_1)	result.ImportFrom("ISO-8859-1", abuffer);
				else if (encoding == ID3TE_UTF8)	result.ImportFrom("UTF-8", abuffer);
			}
		}
		else if (encoding == ID3TE_UTF16 || encoding == ID3TE_UTF16BE)
		{
			if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL)
			{
				ex_ID3Field_GetUNICODE(field, (unicode_t *) wbuffer, tbufsize);

				result.ImportFrom("UTF-16BE", (char *) wbuffer);
			}
		}

		delete [] abuffer;
		delete [] wbuffer;
	}

	return result;
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

	String	 tempDir = Utilities::GetTempDirectory();

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
