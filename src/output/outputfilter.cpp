 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <output/outputfilter.h>

#include <iolib/drivers/driver_posix.h>
#include <iolib/drivers/driver_unicode.h>

#include <dllinterfaces.h>

OutputFilter::OutputFilter(bonkEncConfig *config, bonkEncTrack *iformat)
{
	error		= 0;
	lastPacket	= false;
	format		= iformat;
	currentConfig	= config;
}

OutputFilter::~OutputFilter()
{
}

void OutputFilter::PrepareLastPacket()
{
	lastPacket = true;
}

OutStream *OutputFilter::CreateFile(String fileName)
{
	if (Setup::enableUnicode)	iolibDriver = new IOLibDriverUnicode(fileName, OS_OVERWRITE);
	else				iolibDriver = new IOLibDriverPOSIX(fileName, OS_OVERWRITE);

	return new OutStream(STREAM_DRIVER, iolibDriver);
}

S::Int OutputFilter::CloseFile(OutStream *stream)
{
	delete stream;
	delete iolibDriver;

	return Success;
}

S::Int OutputFilter::RenderID3V2Tag(unsigned char *buffer)
{
	ID3Tag		*tag = ex_ID3Tag_New();

	ex_ID3Tag_SetPadding(tag, false);

	ID3Frame	*artist = ex_ID3Frame_NewID(ID3FID_LEADARTIST);

	if (format->artist != NIL)
	{
		if (String::IsUnicode(format->artist))
		{
			ex_ID3Field_SetEncoding(ex_ID3Frame_GetField(artist, ID3FN_TEXT), ID3TE_UTF8);
			ex_ID3Field_SetASCII(ex_ID3Frame_GetField(artist, ID3FN_TEXT), (char *) format->artist.ConvertTo("UTF-8"));
		}
		else
		{
			ex_ID3Field_SetASCII(ex_ID3Frame_GetField(artist, ID3FN_TEXT), (char *) format->artist);
		}

		ex_ID3Tag_AddFrame(tag, artist);
	}

	ID3Frame	*title = ex_ID3Frame_NewID(ID3FID_TITLE);

	if (format->title != NIL)
	{
		if (String::IsUnicode(format->title))
		{
			ex_ID3Field_SetEncoding(ex_ID3Frame_GetField(title, ID3FN_TEXT), ID3TE_UTF8);
			ex_ID3Field_SetASCII(ex_ID3Frame_GetField(title, ID3FN_TEXT), (char *) format->title.ConvertTo("UTF-8"));
		}
		else
		{
			ex_ID3Field_SetASCII(ex_ID3Frame_GetField(title, ID3FN_TEXT), (char *) format->title);
		}

		ex_ID3Tag_AddFrame(tag, title);
	}

	ID3Frame	*album = ex_ID3Frame_NewID(ID3FID_ALBUM);

	if (format->album != NIL)
	{
		if (String::IsUnicode(format->album))
		{
			ex_ID3Field_SetEncoding(ex_ID3Frame_GetField(album, ID3FN_TEXT), ID3TE_UTF8);
			ex_ID3Field_SetASCII(ex_ID3Frame_GetField(album, ID3FN_TEXT), (char *) format->album.ConvertTo("UTF-8"));
		}
		else
		{
			ex_ID3Field_SetASCII(ex_ID3Frame_GetField(album, ID3FN_TEXT), (char *) format->album);
		}

		ex_ID3Tag_AddFrame(tag, album);
	}

	ID3Frame	*track = ex_ID3Frame_NewID(ID3FID_TRACKNUM);

	if (format->track > 0)
	{
		if (format->track < 10)	ex_ID3Field_SetASCII(ex_ID3Frame_GetField(track, ID3FN_TEXT), (char *) String("0").Append(String::FromInt(format->track)));
		else			ex_ID3Field_SetASCII(ex_ID3Frame_GetField(track, ID3FN_TEXT), (char *) String::FromInt(format->track));

		ex_ID3Tag_AddFrame(tag, track);
	}

	ID3Frame	*year = ex_ID3Frame_NewID(ID3FID_YEAR);

	if (format->year > 0)
	{
		ex_ID3Field_SetASCII(ex_ID3Frame_GetField(year, ID3FN_TEXT), (char *) String::FromInt(format->year));

		ex_ID3Tag_AddFrame(tag, year);
	}

	ID3Frame	*genre = ex_ID3Frame_NewID(ID3FID_CONTENTTYPE);

	if (format->genre != NIL)
	{
		if (String::IsUnicode(format->genre))
		{
			ex_ID3Field_SetEncoding(ex_ID3Frame_GetField(genre, ID3FN_TEXT), ID3TE_UTF8);
			ex_ID3Field_SetASCII(ex_ID3Frame_GetField(genre, ID3FN_TEXT), (char *) format->genre.ConvertTo("UTF-8"));
		}
		else
		{
			ex_ID3Field_SetASCII(ex_ID3Frame_GetField(genre, ID3FN_TEXT), (char *) format->genre);
		}

		ex_ID3Tag_AddFrame(tag, genre);
	}

	ID3Frame	*comment = ex_ID3Frame_NewID(ID3FID_COMMENT);

	if (String::IsUnicode(currentConfig->default_comment))
	{
		ex_ID3Field_SetEncoding(ex_ID3Frame_GetField(comment, ID3FN_TEXT), ID3TE_UTF8);
		ex_ID3Field_SetASCII(ex_ID3Frame_GetField(comment, ID3FN_TEXT), (char *) currentConfig->default_comment.ConvertTo("UTF-8"));
	}
	else
	{
		ex_ID3Field_SetASCII(ex_ID3Frame_GetField(comment, ID3FN_TEXT), (char *) currentConfig->default_comment);
	}

	ex_ID3Tag_AddFrame(tag, comment);

	Int	 size = ex_ID3Tag_Render(tag, buffer, ID3TT_ID3V2);

	ex_ID3Tag_Delete(tag);
	ex_ID3Frame_Delete(artist);
	ex_ID3Frame_Delete(title);
	ex_ID3Frame_Delete(album);
	ex_ID3Frame_Delete(track);
	ex_ID3Frame_Delete(year);
	ex_ID3Frame_Delete(genre);
	ex_ID3Frame_Delete(comment);

	return size;
}
