 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#define __THROW_BAD_ALLOC exit(1)

#include <iolib-cxx.h>
#include <output/filter-out-bonk.h>
#include <dllinterfaces.h>
#include <memory.h>
#include <id3/tag.h>

FilterOutBONK::FilterOutBONK(bonkEncConfig *config, bonkFormatInfo *format) : OutputFilter(config, format)
{
	if (format->channels > 2)
	{
		SMOOTH::MessageBox("BonkEnc does not support more than 2 channels!", "Error", MB_OK, IDI_HAND);

		error = 1;

		return;
	}

	int	 packet_size = int(1024.0 * (currentConfig->bonk_lossless ? 1 : currentConfig->bonk_downsampling) * format->rate / 44100);

	packageSize = packet_size * format->channels * 2;
}

FilterOutBONK::~FilterOutBONK()
{
}

bool FilterOutBONK::Activate()
{
	d_out	= new OutStream(STREAM_DRIVER, driver);

	if (format->trackInfo->hasText && currentConfig->enable_tags)
	{
		ID3_Tag		*tag = new ID3_Tag();

		ID3_Frame	*artist = new ID3_Frame(ID3FID_LEADARTIST);
		ID3_Field	&artist_text = artist->Field(ID3FN_TEXT);

		if (format->trackInfo->artist != NIL && format->trackInfo->artist != "")
		{
			artist_text.Set((char *) format->trackInfo->artist);

			tag->AddFrame(artist);
		}

		ID3_Frame	*title = new ID3_Frame(ID3FID_TITLE);
		ID3_Field	&title_text = title->Field(ID3FN_TEXT);

		if (format->trackInfo->title != NIL && format->trackInfo->title != "")
		{
			title_text.Set((char *) format->trackInfo->title);

			tag->AddFrame(title);
		}

		ID3_Frame	*album = new ID3_Frame(ID3FID_ALBUM);
		ID3_Field	&album_text = album->Field(ID3FN_TEXT);

		if (format->trackInfo->album != NIL && format->trackInfo->album != "")
		{
			album_text.Set((char *) format->trackInfo->album);

			tag->AddFrame(album);
		}

		ID3_Frame	*track = new ID3_Frame(ID3FID_TRACKNUM);
		ID3_Field	&track_text = track->Field(ID3FN_TEXT);

		if (format->trackInfo->track > 0)
		{
			if (format->trackInfo->track < 10)	track_text.Set((char *) String("0").Append(String::FromInt(format->trackInfo->track)));
			else					track_text.Set((char *) String::FromInt(format->trackInfo->track));

			tag->AddFrame(track);
		}

		ID3_Frame	*year = new ID3_Frame(ID3FID_YEAR);
		ID3_Field	&year_text = year->Field(ID3FN_TEXT);

		if (format->trackInfo->year > 0)
		{
			year_text.Set((char *) String::FromInt(format->trackInfo->year));

			tag->AddFrame(year);
		}

		ID3_Frame	*genre = new ID3_Frame(ID3FID_CONTENTTYPE);
		ID3_Field	&genre_text = genre->Field(ID3FN_TEXT);

		if (format->trackInfo->genre != NIL && format->trackInfo->genre != "")
		{
			genre_text.Set((char *) format->trackInfo->genre);

			tag->AddFrame(genre);
		}

		ID3_Frame	*comment = new ID3_Frame(ID3FID_COMMENT);
		ID3_Field	&comment_text = comment->Field(ID3FN_TEXT);

		comment_text.Set((char *) currentConfig->default_comment);

		tag->AddFrame(comment);

		unsigned char	*buffer = new unsigned char [tag->Size()];
		int		 size = tag->Render(buffer);

		d_out->OutputNumber(0, 1);
		d_out->OutputNumber(32, 1);
		d_out->OutputData((void *) buffer, size);
		d_out->OutputNumber(size + 10, 4);
		d_out->OutputString(" id3");

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

	encoder	= ex_bonk_create_encoder(d_out,
		max((int) format->length, 0), format->rate, format->channels,
		currentConfig->bonk_lossless, currentConfig->bonk_jstereo,
		currentConfig->bonk_predictor, currentConfig->bonk_lossless ? 1 : currentConfig->bonk_downsampling,
		int(1024.0 * format->rate / 44100),
		0.05 * (double) currentConfig->bonk_quantization);

	return true;
}

bool FilterOutBONK::Deactivate()
{
	ex_bonk_close_encoder(encoder);

	delete d_out;

	return true;
}

int FilterOutBONK::WriteData(unsigned char *data, int size)
{
	int		 pos = d_out->GetPos();
	vector<int>	 samples(size / (format->bits / 8));

	for (int i = 0; i < size / (format->bits / 8); i++)
	{
		if (format->bits == 8)	samples[i] = (data[i] - 128) * 256;
		if (format->bits == 16)	samples[i] = ((short *) data)[i];
		if (format->bits == 24) samples[i] = (int) (data[3 * i] + 256 * data[3 * i + 1] + 65536 * data[3 * i + 2] - (data[3 * i + 2] & 128 ? 16777216 : 0)) / 256;
		if (format->bits == 32)	samples[i] = (int) ((long *) data)[i] / 65536;
	}

	ex_bonk_encode_packet(encoder, samples);

	return d_out->GetPos() - pos;
}
