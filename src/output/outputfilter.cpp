 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <output/outputfilter.h>
#include <dllinterfaces.h>

BonkEnc::OutputFilter::OutputFilter(Config *config, Track *iFormat)
{
	errorState	= False;
	errorString	= "Unknown error";

	format		= iFormat;
	currentConfig	= config;
}

BonkEnc::OutputFilter::~OutputFilter()
{
}

Int BonkEnc::OutputFilter::RenderID3Tag(Int version, Buffer<unsigned char> &buffer)
{
	ID3Tag		*tag = ex_ID3Tag_New();

	ex_ID3Tag_SetPadding(tag, false);

	ID3_TextEnc	 encoding = ID3TE_NONE;
	String		 encString = (version == 1 ? currentConfig->id3v1_encoding : currentConfig->id3v2_encoding);

	if (encString == "UTF-8")		encoding = ID3TE_UTF8;
	else if (encString == "ISO-8859-1")	encoding = ID3TE_ISO8859_1;
	else if (encString == "UTF-16" ||
		 encString == "UTF-16LE" ||
		 encString == "UCS-2" ||
		 encString == "UCS-2LE")	encoding = ID3TE_UTF16;
	else if (encString == "UTF-16BE" ||
		 encString == "UCS-2BE")	encoding = ID3TE_UTF16BE;

	char	*prevOutFormat = String::SetOutputFormat(encString);

	String		 leBOM;

	leBOM[0] = 0xFEFF;

	ID3Frame	*artist = ex_ID3Frame_NewID(ID3FID_LEADARTIST);

	if (format->artist != NIL)
	{
		ex_ID3Field_SetINT(ex_ID3Frame_GetField(artist, ID3FN_TEXTENC), encoding);
		ex_ID3Field_SetEncoding(ex_ID3Frame_GetField(artist, ID3FN_TEXT), encoding);

		if (encoding == ID3TE_UTF16)		ex_ID3Field_SetUNICODE(ex_ID3Frame_GetField(artist, ID3FN_TEXT), (unicode_t *) String(leBOM).Append(format->artist).ConvertTo("UTF-16LE"));
		else if (encoding == ID3TE_UTF16BE)	ex_ID3Field_SetUNICODE(ex_ID3Frame_GetField(artist, ID3FN_TEXT), (unicode_t *) format->artist.ConvertTo("UTF-16BE"));
		else					ex_ID3Field_SetASCII(ex_ID3Frame_GetField(artist, ID3FN_TEXT), format->artist);

		ex_ID3Tag_AddFrame(tag, artist);
	}

	ID3Frame	*title = ex_ID3Frame_NewID(ID3FID_TITLE);

	if (format->title != NIL)
	{
		ex_ID3Field_SetINT(ex_ID3Frame_GetField(title, ID3FN_TEXTENC), encoding);
		ex_ID3Field_SetEncoding(ex_ID3Frame_GetField(title, ID3FN_TEXT), encoding);

		if (encoding == ID3TE_UTF16)		ex_ID3Field_SetUNICODE(ex_ID3Frame_GetField(title, ID3FN_TEXT), (unicode_t *) String(leBOM).Append(format->title).ConvertTo("UTF-16LE"));
		else if (encoding == ID3TE_UTF16BE)	ex_ID3Field_SetUNICODE(ex_ID3Frame_GetField(title, ID3FN_TEXT), (unicode_t *) format->title.ConvertTo("UTF-16BE"));
		else					ex_ID3Field_SetASCII(ex_ID3Frame_GetField(title, ID3FN_TEXT), format->title);

		ex_ID3Tag_AddFrame(tag, title);
	}

	ID3Frame	*album = ex_ID3Frame_NewID(ID3FID_ALBUM);

	if (format->album != NIL)
	{
		ex_ID3Field_SetINT(ex_ID3Frame_GetField(album, ID3FN_TEXTENC), encoding);
		ex_ID3Field_SetEncoding(ex_ID3Frame_GetField(album, ID3FN_TEXT), encoding);

		if (encoding == ID3TE_UTF16)		ex_ID3Field_SetUNICODE(ex_ID3Frame_GetField(album, ID3FN_TEXT), (unicode_t *) String(leBOM).Append(format->album).ConvertTo("UTF-16LE"));
		else if (encoding == ID3TE_UTF16BE)	ex_ID3Field_SetUNICODE(ex_ID3Frame_GetField(album, ID3FN_TEXT), (unicode_t *) format->album.ConvertTo("UTF-16BE"));
		else					ex_ID3Field_SetASCII(ex_ID3Frame_GetField(album, ID3FN_TEXT), format->album);

		ex_ID3Tag_AddFrame(tag, album);
	}

	ID3Frame	*track = ex_ID3Frame_NewID(ID3FID_TRACKNUM);

	if (format->track > 0)
	{
		ex_ID3Field_SetINT(ex_ID3Frame_GetField(track, ID3FN_TEXTENC), encoding);
		ex_ID3Field_SetEncoding(ex_ID3Frame_GetField(track, ID3FN_TEXT), encoding);

		if (encoding == ID3TE_UTF16)		ex_ID3Field_SetUNICODE(ex_ID3Frame_GetField(track, ID3FN_TEXT), (unicode_t *) String(leBOM).Append(String(format->track < 10 ? "0" : "").Append(String::FromInt(format->track))).ConvertTo("UTF-16LE"));
		else if (encoding == ID3TE_UTF16BE)	ex_ID3Field_SetUNICODE(ex_ID3Frame_GetField(track, ID3FN_TEXT), (unicode_t *) String(format->track < 10 ? "0" : "").Append(String::FromInt(format->track)).ConvertTo("UTF-16BE"));
		else					ex_ID3Field_SetASCII(ex_ID3Frame_GetField(track, ID3FN_TEXT), String(format->track < 10 ? "0" : "").Append(String::FromInt(format->track)));

		ex_ID3Tag_AddFrame(tag, track);
	}

	ID3Frame	*year = ex_ID3Frame_NewID(ID3FID_YEAR);

	if (format->year > 0)
	{
		ex_ID3Field_SetINT(ex_ID3Frame_GetField(year, ID3FN_TEXTENC), encoding);
		ex_ID3Field_SetEncoding(ex_ID3Frame_GetField(year, ID3FN_TEXT), encoding);

		if (encoding == ID3TE_UTF16)		ex_ID3Field_SetUNICODE(ex_ID3Frame_GetField(year, ID3FN_TEXT), (unicode_t *) String(leBOM).Append(String::FromInt(format->year)).ConvertTo("UTF-16LE"));
		else if (encoding == ID3TE_UTF16BE)	ex_ID3Field_SetUNICODE(ex_ID3Frame_GetField(year, ID3FN_TEXT), (unicode_t *) String::FromInt(format->year).ConvertTo("UTF-16BE"));
		else					ex_ID3Field_SetASCII(ex_ID3Frame_GetField(year, ID3FN_TEXT), String::FromInt(format->year));

		ex_ID3Tag_AddFrame(tag, year);
	}

	ID3Frame	*genre = ex_ID3Frame_NewID(ID3FID_CONTENTTYPE);

	if (format->genre != NIL)
	{
		ex_ID3Field_SetINT(ex_ID3Frame_GetField(genre, ID3FN_TEXTENC), encoding);
		ex_ID3Field_SetEncoding(ex_ID3Frame_GetField(genre, ID3FN_TEXT), encoding);

		if (encoding == ID3TE_UTF16)		ex_ID3Field_SetUNICODE(ex_ID3Frame_GetField(genre, ID3FN_TEXT), (unicode_t *) String(leBOM).Append(format->genre).ConvertTo("UTF-16LE"));
		else if (encoding == ID3TE_UTF16BE)	ex_ID3Field_SetUNICODE(ex_ID3Frame_GetField(genre, ID3FN_TEXT), (unicode_t *) format->genre.ConvertTo("UTF-16BE"));
		else					ex_ID3Field_SetASCII(ex_ID3Frame_GetField(genre, ID3FN_TEXT), format->genre);

		ex_ID3Tag_AddFrame(tag, genre);
	}

	ID3Frame	*comment = ex_ID3Frame_NewID(ID3FID_COMMENT);

	if (currentConfig->default_comment != NIL) 
	{
		ex_ID3Field_SetINT(ex_ID3Frame_GetField(comment, ID3FN_TEXTENC), encoding);
		ex_ID3Field_SetEncoding(ex_ID3Frame_GetField(comment, ID3FN_TEXT), encoding);

		if (encoding == ID3TE_UTF16)		ex_ID3Field_SetUNICODE(ex_ID3Frame_GetField(comment, ID3FN_TEXT), (unicode_t *) String(leBOM).Append(currentConfig->default_comment).ConvertTo("UTF-16LE"));
		else if (encoding == ID3TE_UTF16BE)	ex_ID3Field_SetUNICODE(ex_ID3Frame_GetField(comment, ID3FN_TEXT), (unicode_t *) currentConfig->default_comment.ConvertTo("UTF-16BE"));
		else					ex_ID3Field_SetASCII(ex_ID3Frame_GetField(comment, ID3FN_TEXT), currentConfig->default_comment);

		ex_ID3Tag_AddFrame(tag, comment);
	}

	Array<ID3Frame *>	 pictures;

	if (currentConfig->copy_picture_tags)
	{
		for (Int i = 0; i < format->pictures.GetNOfEntries(); i++)
		{
			ID3Frame	*picture = ex_ID3Frame_NewID(ID3FID_PICTURE);
			Picture		*picInfo = format->pictures.GetNth(i);

			ex_ID3Field_SetINT(ex_ID3Frame_GetField(picture, ID3FN_TEXTENC), encoding);
			ex_ID3Field_SetEncoding(ex_ID3Frame_GetField(picture, ID3FN_DESCRIPTION), encoding);

			if (encoding == ID3TE_UTF16)		ex_ID3Field_SetUNICODE(ex_ID3Frame_GetField(picture, ID3FN_DESCRIPTION), (unicode_t *) String(leBOM).Append(picInfo->description).ConvertTo("UTF-16LE"));
			else if (encoding == ID3TE_UTF16BE)	ex_ID3Field_SetUNICODE(ex_ID3Frame_GetField(picture, ID3FN_DESCRIPTION), (unicode_t *) picInfo->description.ConvertTo("UTF-16BE"));
			else					ex_ID3Field_SetASCII(ex_ID3Frame_GetField(picture, ID3FN_DESCRIPTION), picInfo->description);

			ex_ID3Field_SetASCII(ex_ID3Frame_GetField(picture, ID3FN_MIMETYPE), picInfo->mime.ConvertTo("ISO-8859-1"));
			ex_ID3Field_SetINT(ex_ID3Frame_GetField(picture, ID3FN_PICTURETYPE), picInfo->type);
			ex_ID3Field_SetBINARY(ex_ID3Frame_GetField(picture, ID3FN_DATA), picInfo->data, picInfo->data.Size());

			ex_ID3Tag_AddFrame(tag, picture);

			pictures.Add(picture);
		}
	}

	String::SetOutputFormat(prevOutFormat);

	buffer.Resize(ex_ID3Tag_Size(tag));

	Int	 size = ex_ID3Tag_Render(tag, buffer, version == 1 ? ID3TT_ID3V1 : ID3TT_ID3V2);

	ex_ID3Tag_Delete(tag);
	ex_ID3Frame_Delete(artist);
	ex_ID3Frame_Delete(title);
	ex_ID3Frame_Delete(album);
	ex_ID3Frame_Delete(track);
	ex_ID3Frame_Delete(year);
	ex_ID3Frame_Delete(genre);
	ex_ID3Frame_Delete(comment);

	for (Int j = 0; j < format->pictures.GetNOfEntries(); j++)
	{
		ex_ID3Frame_Delete(pictures.GetNth(j));
	}

	return size;
}
