 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <iolib-cxx.h>
#include <output/filter-out-blade.h>
#include <dllinterfaces.h>
#include <memory.h>
#include <id3/tag.h>

FilterOutBLADE::FilterOutBLADE(bonkEncConfig *config, bonkFormatInfo *format) : OutputFilter(config, format)
{
	if (format->bits != 16)
	{
		SMOOTH::MessageBox("Input files must be 16 bit for BladeEnc MP3 encoding!", "Error", MB_OK, IDI_HAND);

		error = 1;

		return;
	}

	if (format->rate != 32000 && format->rate != 44100 && format->rate != 48000)
	{
		SMOOTH::MessageBox("Bad sampling rate! BladeEnc supports only 32, 44.1 or 48kHz.", "Error", MB_OK, IDI_HAND);

		error = 1;

		return;
	}

	beConfig.dwConfig			= BE_CONFIG_MP3;
	beConfig.format.mp3.dwSampleRate	= format->rate;

	if (format->channels == 2)
	{
		if (currentConfig->blade_dualchannel)	beConfig.format.mp3.byMode = BE_MP3_MODE_DUALCHANNEL;
		else					beConfig.format.mp3.byMode = BE_MP3_MODE_STEREO;
	}
	else if (format->channels == 1)
	{
		beConfig.format.mp3.byMode = BE_MP3_MODE_MONO;
	}
	else
	{
		SMOOTH::MessageBox("BonkEnc does not support more than 2 channels!", "Error", MB_OK, IDI_HAND);

		error = 1;

		return;
	}

	beConfig.format.mp3.wBitrate	= currentConfig->blade_bitrate;
	beConfig.format.mp3.bCopyright	= currentConfig->blade_copyright;
	beConfig.format.mp3.bCRC	= currentConfig->blade_crc;
	beConfig.format.mp3.bOriginal	= currentConfig->blade_original;
	beConfig.format.mp3.bPrivate	= currentConfig->blade_private;

	ex_beInitStream(&beConfig, &samples_size, &buffersize, &handle);

	packageSize = samples_size * (format->bits / 8);
}

FilterOutBLADE::~FilterOutBLADE()
{
}

bool FilterOutBLADE::Activate()
{
	if (format->trackInfo->cdText && currentConfig->enable_tags)
	{
		ID3_Tag		*tag = new ID3_Tag();

		ID3_Frame	*artist = new ID3_Frame(ID3FID_LEADARTIST);
		ID3_Field	&artist_text = artist->Field(ID3FN_TEXT);

		if (format->trackInfo->artist != NIL && format->trackInfo->artist != "")
		{
			artist_text.Set((wchar_t *) format->trackInfo->artist);

			tag->AddFrame(artist);
		}

		ID3_Frame	*title = new ID3_Frame(ID3FID_TITLE);
		ID3_Field	&title_text = title->Field(ID3FN_TEXT);

		if (format->trackInfo->title != NIL && format->trackInfo->title != "")
		{
			title_text.Set((wchar_t *) format->trackInfo->title);

			tag->AddFrame(title);
		}

		ID3_Frame	*album = new ID3_Frame(ID3FID_ALBUM);
		ID3_Field	&album_text = album->Field(ID3FN_TEXT);

		if (format->trackInfo->album != NIL && format->trackInfo->album != "")
		{
			album_text.Set((wchar_t *) format->trackInfo->album);

			tag->AddFrame(album);
		}

		ID3_Frame	*track = new ID3_Frame(ID3FID_TRACKNUM);
		ID3_Field	&track_text = track->Field(ID3FN_TEXT);

		if (format->trackInfo->track > 0)
		{
			if (format->trackInfo->track < 10)	track_text.Set((wchar_t *) String("0").Append(String::IntToString(format->trackInfo->track)));
			else					track_text.Set((wchar_t *) String::IntToString(format->trackInfo->track));

			tag->AddFrame(track);
		}

		ID3_Frame	*year = new ID3_Frame(ID3FID_YEAR);
		ID3_Field	&year_text = year->Field(ID3FN_TEXT);

		if (format->trackInfo->year > 0)
		{
			year_text.Set((wchar_t *) String::IntToString(format->trackInfo->year));

			tag->AddFrame(year);
		}

		ID3_Frame	*genre = new ID3_Frame(ID3FID_CONTENTTYPE);
		ID3_Field	&genre_text = genre->Field(ID3FN_TEXT);

		if (format->trackInfo->genre != NIL && format->trackInfo->genre != "")
		{
			genre_text.Set((wchar_t *) format->trackInfo->genre);

			tag->AddFrame(genre);
		}

		ID3_Frame	*comment = new ID3_Frame(ID3FID_COMMENT);
		ID3_Field	&comment_text = comment->Field(ID3FN_TEXT);

		comment_text.Set((wchar_t *) currentConfig->default_comment);

		tag->AddFrame(comment);

		unsigned char	*buffer = new unsigned char [tag->Size()];
		int		 size = tag->Render(buffer);

		driver->WriteData(buffer, size);

		delete [] buffer;

		delete tag;
		delete artist;
		delete title;
		delete album;
		delete track;
		delete year;
		delete genre;
		delete comment;
	}

	return true;
}

bool FilterOutBLADE::Deactivate()
{
	unsigned char	*outbuffer = new unsigned char [buffersize];
	unsigned long	 bytes = 0;

	ex_beDeinitStream(handle, outbuffer, &bytes);

	driver->WriteData(outbuffer, bytes);

	ex_beCloseStream(handle);

	return true;
}

int FilterOutBLADE::WriteData(unsigned char *data, int size)
{
	unsigned char	*outbuffer = new unsigned char [buffersize];
	unsigned long	 bytes = 0;

	ex_beEncodeChunk(handle, samples_size, (signed short *) data, outbuffer, &bytes);

	driver->WriteData(outbuffer, bytes);

	delete [] outbuffer;

	return bytes;
}
