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
#include <output/filter-out-blade.h>
#include <dllinterfaces.h>
#include <memory.h>
#include <id3/tag.h>

FilterOutBLADE::FilterOutBLADE(bonkEncConfig *config, bonkFormatInfo *format) : OutputFilter(config, format)
{
	if (format->rate != 32000 && format->rate != 44100 && format->rate != 48000)
	{
		QuickMessage("Bad sampling rate! BladeEnc supports only 32, 44.1 or 48kHz.", "Error", MB_OK, IDI_HAND);

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
		QuickMessage("BonkEnc does not support more than 2 channels!", "Error", MB_OK, IDI_HAND);

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
	if (format->trackInfo->hasText && currentConfig->enable_tags)
	{
		ID3_Tag		*tag = new ID3_Tag();

		tag->SetPadding(false);

		ID3_Frame	*artist = new ID3_Frame(ID3FID_LEADARTIST);

		if (format->trackInfo->artist != NIL)
		{
			if (String::IsUnicode(format->trackInfo->artist))
			{
				artist->Field(ID3FN_TEXT).SetEncoding(ID3TE_UTF8);
				artist->Field(ID3FN_TEXT).Set((char *) format->trackInfo->artist.ConvertTo("UTF-8"));
			}
			else
			{
				artist->Field(ID3FN_TEXT).Set((char *) format->trackInfo->artist);
			}

			tag->AddFrame(artist);
		}

		ID3_Frame	*title = new ID3_Frame(ID3FID_TITLE);

		if (format->trackInfo->title != NIL)
		{
			if (String::IsUnicode(format->trackInfo->title))
			{
				title->Field(ID3FN_TEXT).SetEncoding(ID3TE_UTF8);
				title->Field(ID3FN_TEXT).Set((char *) format->trackInfo->title.ConvertTo("UTF-8"));
			}
			else
			{
				title->Field(ID3FN_TEXT).Set((char *) format->trackInfo->title);
			}

			tag->AddFrame(title);
		}

		ID3_Frame	*album = new ID3_Frame(ID3FID_ALBUM);

		if (format->trackInfo->album != NIL)
		{
			if (String::IsUnicode(format->trackInfo->album))
			{
				album->Field(ID3FN_TEXT).SetEncoding(ID3TE_UTF8);
				album->Field(ID3FN_TEXT).Set((char *) format->trackInfo->album.ConvertTo("UTF-8"));
			}
			else
			{
				album->Field(ID3FN_TEXT).Set((char *) format->trackInfo->album);
			}

			tag->AddFrame(album);
		}

		ID3_Frame	*track = new ID3_Frame(ID3FID_TRACKNUM);

		if (format->trackInfo->track > 0)
		{
			if (format->trackInfo->track < 10)	track->Field(ID3FN_TEXT).Set((char *) String("0").Append(String::FromInt(format->trackInfo->track)));
			else					track->Field(ID3FN_TEXT).Set((char *) String::FromInt(format->trackInfo->track));

			tag->AddFrame(track);
		}

		ID3_Frame	*year = new ID3_Frame(ID3FID_YEAR);

		if (format->trackInfo->year > 0)
		{
			year->Field(ID3FN_TEXT).Set((char *) String::FromInt(format->trackInfo->year));

			tag->AddFrame(year);
		}

		ID3_Frame	*genre = new ID3_Frame(ID3FID_CONTENTTYPE);

		if (format->trackInfo->genre != NIL)
		{
			if (String::IsUnicode(format->trackInfo->genre))
			{
				genre->Field(ID3FN_TEXT).SetEncoding(ID3TE_UTF8);
				genre->Field(ID3FN_TEXT).Set((char *) format->trackInfo->genre.ConvertTo("UTF-8"));
			}
			else
			{
				genre->Field(ID3FN_TEXT).Set((char *) format->trackInfo->genre);
			}

			tag->AddFrame(genre);
		}

		ID3_Frame	*comment = new ID3_Frame(ID3FID_COMMENT);

		if (String::IsUnicode(currentConfig->default_comment))
		{
			comment->Field(ID3FN_TEXT).SetEncoding(ID3TE_UTF8);
			comment->Field(ID3FN_TEXT).Set((char *) currentConfig->default_comment.ConvertTo("UTF-8"));
		}
		else
		{
			comment->Field(ID3FN_TEXT).Set((char *) currentConfig->default_comment);
		}

		tag->AddFrame(comment);

		unsigned char	*buffer = new unsigned char [32768];
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

	delete [] outbuffer;

	ex_beCloseStream(handle);

	return true;
}

int FilterOutBLADE::WriteData(unsigned char *data, int size)
{
	signed short	*samples = new signed short [size / (format->bits / 8)];
	unsigned char	*outbuffer = new unsigned char [buffersize];
	unsigned long	 bytes = 0;

	for (int i = 0; i < size / (format->bits / 8); i++)
	{
		if (format->bits == 8)	samples[i] = (data[i] - 128) * 256;
		if (format->bits == 16)	samples[i] = ((short *) data)[i];
		if (format->bits == 24) samples[i] = (int) (data[3 * i] + 256 * data[3 * i + 1] + 65536 * data[3 * i + 2] - (data[3 * i + 2] & 128 ? 16777216 : 0)) / 256;
		if (format->bits == 32)	samples[i] = (int) ((long *) data)[i] / 65536;
	}

	ex_beEncodeChunk(handle, samples_size, samples, outbuffer, &bytes);

	driver->WriteData(outbuffer, bytes);

	delete [] samples;
	delete [] outbuffer;

	return bytes;
}
