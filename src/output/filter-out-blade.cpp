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
	if (format->trackInfo->hasText && currentConfig->enable_tags && currentConfig->enable_id3)
	{
		ID3Tag		*tag = ex_ID3Tag_New();

		ex_ID3Tag_SetPadding(tag, false);

		ID3Frame	*artist = ex_ID3Frame_NewID(ID3FID_LEADARTIST);

		if (format->trackInfo->artist != NIL)
		{
			if (String::IsUnicode(format->trackInfo->artist))
			{
				ex_ID3Field_SetEncoding(ex_ID3Frame_GetField(artist, ID3FN_TEXT), ID3TE_UTF8);
				ex_ID3Field_SetASCII(ex_ID3Frame_GetField(artist, ID3FN_TEXT), (char *) format->trackInfo->artist.ConvertTo("UTF-8"));
			}
			else
			{
				ex_ID3Field_SetASCII(ex_ID3Frame_GetField(artist, ID3FN_TEXT), (char *) format->trackInfo->artist);
			}

			ex_ID3Tag_AddFrame(tag, artist);
		}

		ID3Frame	*title = ex_ID3Frame_NewID(ID3FID_TITLE);

		if (format->trackInfo->title != NIL)
		{
			if (String::IsUnicode(format->trackInfo->title))
			{
				ex_ID3Field_SetEncoding(ex_ID3Frame_GetField(title, ID3FN_TEXT), ID3TE_UTF8);
				ex_ID3Field_SetASCII(ex_ID3Frame_GetField(title, ID3FN_TEXT), (char *) format->trackInfo->title.ConvertTo("UTF-8"));
			}
			else
			{
				ex_ID3Field_SetASCII(ex_ID3Frame_GetField(title, ID3FN_TEXT), (char *) format->trackInfo->title);
			}

			ex_ID3Tag_AddFrame(tag, title);
		}

		ID3Frame	*album = ex_ID3Frame_NewID(ID3FID_ALBUM);

		if (format->trackInfo->album != NIL)
		{
			if (String::IsUnicode(format->trackInfo->album))
			{
				ex_ID3Field_SetEncoding(ex_ID3Frame_GetField(album, ID3FN_TEXT), ID3TE_UTF8);
				ex_ID3Field_SetASCII(ex_ID3Frame_GetField(album, ID3FN_TEXT), (char *) format->trackInfo->album.ConvertTo("UTF-8"));
			}
			else
			{
				ex_ID3Field_SetASCII(ex_ID3Frame_GetField(album, ID3FN_TEXT), (char *) format->trackInfo->album);
			}

			ex_ID3Tag_AddFrame(tag, album);
		}

		ID3Frame	*track = ex_ID3Frame_NewID(ID3FID_TRACKNUM);

		if (format->trackInfo->track > 0)
		{
			if (format->trackInfo->track < 10)	ex_ID3Field_SetASCII(ex_ID3Frame_GetField(track, ID3FN_TEXT), (char *) String("0").Append(String::FromInt(format->trackInfo->track)));
			else					ex_ID3Field_SetASCII(ex_ID3Frame_GetField(track, ID3FN_TEXT), (char *) String::FromInt(format->trackInfo->track));

			ex_ID3Tag_AddFrame(tag, track);
		}

		ID3Frame	*year = ex_ID3Frame_NewID(ID3FID_YEAR);

		if (format->trackInfo->year > 0)
		{
			ex_ID3Field_SetASCII(ex_ID3Frame_GetField(year, ID3FN_TEXT), (char *) String::FromInt(format->trackInfo->year));

			ex_ID3Tag_AddFrame(tag, year);
		}

		ID3Frame	*genre = ex_ID3Frame_NewID(ID3FID_CONTENTTYPE);

		if (format->trackInfo->genre != NIL)
		{
			if (String::IsUnicode(format->trackInfo->genre))
			{
				ex_ID3Field_SetEncoding(ex_ID3Frame_GetField(genre, ID3FN_TEXT), ID3TE_UTF8);
				ex_ID3Field_SetASCII(ex_ID3Frame_GetField(genre, ID3FN_TEXT), (char *) format->trackInfo->genre.ConvertTo("UTF-8"));
			}
			else
			{
				ex_ID3Field_SetASCII(ex_ID3Frame_GetField(genre, ID3FN_TEXT), (char *) format->trackInfo->genre);
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

		unsigned char	*buffer = new unsigned char [32768];
		int		 size = ex_ID3Tag_Render(tag, buffer, ID3TT_ID3V2);

		driver->WriteData(buffer, size);

		delete [] buffer;

		ex_ID3Tag_Delete(tag);
		ex_ID3Frame_Delete(artist);
		ex_ID3Frame_Delete(title);
		ex_ID3Frame_Delete(album);
		ex_ID3Frame_Delete(track);
		ex_ID3Frame_Delete(year);
		ex_ID3Frame_Delete(genre);
		ex_ID3Frame_Delete(comment);
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
