 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
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
	InStream	 in(STREAM_FILE, fileName, IS_READ);

	/* Look for ID3v2 tag.
	 */
	if (in.InputString(3) == "ID3" && in.InputNumber(1) <= 4)
	{
		/* Skip minor version and flags.
		 */
		in.InputNumber(1);
		in.InputNumber(1);

		/* Read tag size as a 4 byte unsynchronized integer.
		 */
		Int	 tagSize = (in.InputNumber(1) << 21) +
				   (in.InputNumber(1) << 14) +
				   (in.InputNumber(1) <<  7) +
				   (in.InputNumber(1)      );

		in.Seek(0);

		unsigned char	*buffer = new unsigned char [tagSize + 10];

		in.InputData(buffer, tagSize + 10);

		Int	 retVal = ParseID3V2Tag(buffer, tagSize + 10, nFormat);

		delete [] buffer;

		return retVal;
	}

	in.Seek(in.Size() - 128);

	/* Look for ID3v1 tag.
	 */
	if (in.InputString(3) == "TAG")
	{
		String	 prevInFormat = String::SetInputFormat("ISO-8859-1");

		nFormat->title	 = in.InputString(30);
		nFormat->artist	 = in.InputString(30);
		nFormat->album	 = in.InputString(30);
		nFormat->year	 = in.InputString(4).ToInt();
		nFormat->comment = in.InputString(28);

		if (in.InputNumber(1) == 0)
		{
			Int	 n = in.InputNumber(1);

			if (n > 0) nFormat->track = n;
		}
		else
		{
			in.RelSeek(-29);

			nFormat->comment = in.InputString(30);
		}

		nFormat->genre	 = Utilities::GetID3CategoryName(in.InputNumber(1));

		String::SetInputFormat(prevInFormat);

		return Success();
	}

	return Error();
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
		else if (ex_ID3Frame_GetID(frame) == ID3FID_YEAR)	nFormat->year = GetID3V2FrameString(frame).ToInt();
		else if (ex_ID3Frame_GetID(frame) == ID3FID_COMMENT)	nFormat->comment = GetID3V2FrameString(frame);
		else if (ex_ID3Frame_GetID(frame) == ID3FID_PUBLISHER)	nFormat->label = GetID3V2FrameString(frame);
		else if (ex_ID3Frame_GetID(frame) == ID3FID_ISRC)	nFormat->isrc = GetID3V2FrameString(frame);
		else if (ex_ID3Frame_GetID(frame) == ID3FID_TRACKNUM)
		{
			String	 trackString = GetID3V2FrameString(frame);

			nFormat->track = trackString.ToInt();

			if (trackString.Find("/") >= 0) nFormat->numTracks = trackString.Tail(trackString.Length() - trackString.Find("/") - 1).ToInt();
		}
		else if (ex_ID3Frame_GetID(frame) == ID3FID_PARTINSET)
		{
			String	 discString = GetID3V2FrameString(frame);

			nFormat->disc = discString.ToInt();

			if (discString.Find("/") >= 0) nFormat->numDiscs = discString.Tail(discString.Length() - discString.Find("/") - 1).ToInt();
		}
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
			else if (genreID != NIL)			nFormat->genre = Utilities::GetID3CategoryName(genreID.ToInt());
		}
		else if (ex_ID3Frame_GetID(frame) == ID3FID_PICTURE)
		{
			Picture	*picture = new Picture();

			Buffer<char>	 abuffer(1024);
			Buffer<wchar_t>	 wbuffer(1024);

			abuffer.Zero();
			wbuffer.Zero();

			ID3Field	*field = ex_ID3Frame_GetField(frame, ID3FN_MIMETYPE);

			if (field != NIL)
			{
				ex_ID3Field_GetASCII(field, abuffer, abuffer.Size());

				picture->mime.ImportFrom("ISO-8859-1", abuffer);
			}

			field = ex_ID3Frame_GetField(frame, ID3FN_PICTURETYPE);

			if (field != NIL)
			{
				picture->type = ex_ID3Field_GetINT(field);
			}

			abuffer.Zero();
			wbuffer.Zero();

			field = ex_ID3Frame_GetField(frame, ID3FN_TEXTENC);

			if (field != NIL)
			{
				Int	 encoding = ex_ID3Field_GetINT(field);

				if (encoding == ID3TE_ISO8859_1 || encoding == ID3TE_UTF8)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_DESCRIPTION)) != NIL)
					{
						ex_ID3Field_GetASCII(field, abuffer, abuffer.Size());

						if	(encoding == ID3TE_ISO8859_1)	picture->description.ImportFrom("ISO-8859-1", abuffer);
						else if (encoding == ID3TE_UTF8)	picture->description.ImportFrom("UTF-8", abuffer);
					}
				}
				else if (encoding == ID3TE_UTF16 || encoding == ID3TE_UTF16BE)
				{
					if ((field = ex_ID3Frame_GetField(frame, ID3FN_DESCRIPTION)) != NIL)
					{
						ex_ID3Field_GetUNICODE(field, (unicode_t *) (wchar_t *) wbuffer, wbuffer.Size());

						picture->description.ImportFrom("UTF-16BE", (char *) (wchar_t *) wbuffer);
					}
				}
			}

			field = ex_ID3Frame_GetField(frame, ID3FN_DATA);

			if (field != NIL)
			{
				picture->data.Resize(ex_ID3Field_Size(field));

				ex_ID3Field_GetBINARY(field, picture->data, picture->data.Size());
			}

			if	(picture->data[0] == 0xFF && picture->data[1] == 0xD8) picture->mime = "image/jpeg";
			else if (picture->data[0] == 0x89 && picture->data[1] == 0x50 &&
				 picture->data[2] == 0x4E && picture->data[3] == 0x47 &&
				 picture->data[4] == 0x0D && picture->data[5] == 0x0A &&
				 picture->data[6] == 0x1A && picture->data[7] == 0x0A) picture->mime = "image/png";

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

		Buffer<char>	 abuffer(1024);
		Buffer<wchar_t>	 wbuffer(1024);

		abuffer.Zero();
		wbuffer.Zero();

		if (encoding == ID3TE_ISO8859_1 || encoding == ID3TE_UTF8)
		{
			if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL)
			{
				ex_ID3Field_GetASCII(field, abuffer, abuffer.Size());

				if	(encoding == ID3TE_ISO8859_1)	result.ImportFrom("ISO-8859-1", abuffer);
				else if (encoding == ID3TE_UTF8)	result.ImportFrom("UTF-8", abuffer);
			}
		}
		else if (encoding == ID3TE_UTF16 || encoding == ID3TE_UTF16BE)
		{
			if ((field = ex_ID3Frame_GetField(frame, ID3FN_TEXT)) != NIL)
			{
				ex_ID3Field_GetUNICODE(field, (unicode_t *) (wchar_t *) wbuffer, wbuffer.Size());

				result.ImportFrom("UTF-16BE", (char *) (wchar_t *) wbuffer);
			}
		}
	}

	return result;
}
