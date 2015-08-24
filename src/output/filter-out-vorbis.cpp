 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <output/filter-out-vorbis.h>
#include <utilities.h>
#include <dllinterfaces.h>

#include <time.h>
#include <stdlib.h>

BonkEnc::FilterOutVORBIS::FilterOutVORBIS(Config *config, Track *format) : OutputFilter(config, format)
{
	if (format->channels > 2)
	{
		Utilities::ErrorMessage(String(BonkEnc::appName).Append(" does not support more than 2 channels!"));

		errorState = True;

		return;
	}
}

BonkEnc::FilterOutVORBIS::~FilterOutVORBIS()
{
}

Bool BonkEnc::FilterOutVORBIS::Activate()
{
	srand(clock());

	ex_vorbis_info_init(&vi);

	switch (currentConfig->vorbis_mode)
	{
		case 0:
			ex_vorbis_encode_init_vbr(&vi, format->channels, format->rate, ((double) currentConfig->vorbis_quality) / 100);
			break;
		case 1:
			ex_vorbis_encode_init(&vi, format->channels, format->rate, -1, currentConfig->vorbis_bitrate * 1000, -1);
			break;
	}

	ex_vorbis_comment_init(&vc);

	if (currentConfig->enable_vctags)
	{
		String	 prevOutFormat = String::SetOutputFormat(currentConfig->vctag_encoding);

		if (format->artist != NIL || format->title != NIL)
		{
			if	(format->title	  != NIL) ex_vorbis_comment_add_tag(&vc, (char *) "TITLE", format->title);
			if	(format->artist	  != NIL) ex_vorbis_comment_add_tag(&vc, (char *) "ARTIST", format->artist);
			if	(format->album	  != NIL) ex_vorbis_comment_add_tag(&vc, (char *) "ALBUM", format->album);
			if	(format->track	   >   0) ex_vorbis_comment_add_tag(&vc, (char *) "TRACKNUMBER", String(format->track < 10 ? "0" : NIL).Append(String::FromInt(format->track)));
			if	(format->numTracks >   0) ex_vorbis_comment_add_tag(&vc, (char *) "TRACKTOTAL", String(format->numTracks < 10 ? "0" : NIL).Append(String::FromInt(format->numTracks)));
			if	(format->disc	   >   0) ex_vorbis_comment_add_tag(&vc, (char *) "DISCNUMBER", String(format->disc < 10 ? "0" : NIL).Append(String::FromInt(format->disc)));
			if	(format->numDiscs  >   0) ex_vorbis_comment_add_tag(&vc, (char *) "DISCTOTAL", String(format->numDiscs < 10 ? "0" : NIL).Append(String::FromInt(format->numDiscs)));
			if	(format->year	   >   0) ex_vorbis_comment_add_tag(&vc, (char *) "DATE", String::FromInt(format->year));
			if	(format->genre	  != NIL) ex_vorbis_comment_add_tag(&vc, (char *) "GENRE", format->genre);
			if	(format->label	  != NIL) ex_vorbis_comment_add_tag(&vc, (char *) "ORGANIZATION", format->label);
			if	(format->isrc	  != NIL) ex_vorbis_comment_add_tag(&vc, (char *) "ISRC", format->isrc);

			if	(format->comment != NIL && !currentConfig->overwriteComments) ex_vorbis_comment_add_tag(&vc, (char *) "COMMENT", format->comment);
			else if (currentConfig->default_comment != NIL)			      ex_vorbis_comment_add_tag(&vc, (char *) "COMMENT", currentConfig->default_comment);
		}

		/* Save cover art.
		 */
		if (currentConfig->copy_picture_tags)
		{
			/* This is the official way to store cover art in Vorbis
			 * comments. It is used by most newer software.
			 */
			foreach (const Picture *picInfo, format->pictures)
			{
				Buffer<UnsignedByte>	 picBuffer((picInfo->mime	 != NIL ? strlen(picInfo->mime)	       : 0) +
								   (picInfo->description != NIL ? strlen(picInfo->description) : 0) + picInfo->data.Size() + 32);
				OutStream		 picOut(STREAM_BUFFER, picBuffer, picBuffer.Size());

				picOut.OutputNumberRaw(picInfo->type, 4);

				picOut.OutputNumberRaw(picInfo->mime != NIL ? strlen(picInfo->mime) : 0, 4);
				picOut.OutputString(picInfo->mime);

				picOut.OutputNumberRaw(picInfo->description != NIL ? strlen(picInfo->description) : 0, 4);
				picOut.OutputString(picInfo->description);

				picOut.OutputNumberRaw(0, 4);
				picOut.OutputNumberRaw(0, 4);
				picOut.OutputNumberRaw(0, 4);
				picOut.OutputNumberRaw(0, 4);

				picOut.OutputNumberRaw(picInfo->data.Size(), 4);
				picOut.OutputData(picInfo->data, picInfo->data.Size());

				picOut.Close();

				ex_vorbis_comment_add_tag(&vc, (char *) "METADATA_BLOCK_PICTURE", Encoding::Base64(picBuffer).Encode());
			}
		}

		String::SetOutputFormat(prevOutFormat);
	}

	ex_vorbis_analysis_init(&vd, &vi);
	ex_vorbis_block_init(&vd, &vb);

	ex_ogg_stream_init(&os, rand());

	ogg_packet	 header;
	ogg_packet	 header_comm;
	ogg_packet	 header_code;


	ex_vorbis_analysis_headerout(&vd, &vc, &header, &header_comm, &header_code);

	ex_ogg_stream_packetin(&os, &header); /* automatically placed in its own page */

	ex_ogg_stream_packetin(&os, &header_comm);
	ex_ogg_stream_packetin(&os, &header_code);

	int	 dataLength = 0;

	do
	{
		int result = ex_ogg_stream_flush(&os, &og);

		if (result == 0) break;

		backBuffer.Resize(dataLength);

		memcpy(backBuffer, dataBuffer, dataLength);

		dataBuffer.Resize(dataLength + og.header_len + og.body_len);

		memcpy(dataBuffer, backBuffer, dataLength);
		memcpy(((unsigned char *) dataBuffer) + dataLength, og.header, og.header_len);
		memcpy(((unsigned char *) dataBuffer) + dataLength + og.header_len, og.body, og.body_len);

		dataLength += og.header_len;
		dataLength += og.body_len;
	}
	while (true);

	driver->WriteData(dataBuffer, dataLength);

	return true;
}

Bool BonkEnc::FilterOutVORBIS::Deactivate()
{
	ex_vorbis_analysis_wrote(&vd, 0);

	int	 dataLength = 0;

	while (ex_vorbis_analysis_blockout(&vd, &vb) == 1)
	{
		ex_vorbis_analysis(&vb, NULL);
		ex_vorbis_bitrate_addblock(&vb);

		while(ex_vorbis_bitrate_flushpacket(&vd, &op))
		{
			ex_ogg_stream_packetin(&os, &op);

			do
			{
				int	 result = ex_ogg_stream_pageout(&os, &og);

				if (result == 0) break;

				backBuffer.Resize(dataLength);

				memcpy(backBuffer, dataBuffer, dataLength);

				dataBuffer.Resize(dataLength + og.header_len + og.body_len);

				memcpy(dataBuffer, backBuffer, dataLength);
				memcpy(((unsigned char *) dataBuffer) + dataLength, og.header, og.header_len);
				memcpy(((unsigned char *) dataBuffer) + dataLength + og.header_len, og.body, og.body_len);

				dataLength += og.header_len;
				dataLength += og.body_len;

				if (ex_ogg_page_eos(&og)) break;
			}
			while (true);
		}
	}

	driver->WriteData(dataBuffer, dataLength);

	ex_ogg_stream_clear(&os);
	ex_vorbis_block_clear(&vb);
	ex_vorbis_dsp_clear(&vd);
	ex_vorbis_comment_clear(&vc);
	ex_vorbis_info_clear(&vi);

	return true;
}

Int BonkEnc::FilterOutVORBIS::WriteData(Buffer<UnsignedByte> &data, Int size)
{
	int	 dataLength = 0;
	int	 samples_size = size / (format->bits / 8);

	float	**buffer = ex_vorbis_analysis_buffer(&vd, samples_size / format->channels);

	if (format->bits != 16)
	{
		samplesBuffer.Resize(size / (format->bits / 8));

		for (int i = 0; i < size / (format->bits / 8); i++)
		{
			if (format->bits == 8)	samplesBuffer[i] = (data[i] - 128) * 256;
			if (format->bits == 24) samplesBuffer[i] = (int) (data[3 * i] + 256 * data[3 * i + 1] + 65536 * data[3 * i + 2] - (data[3 * i + 2] & 128 ? 16777216 : 0)) / 256;
			if (format->bits == 32)	samplesBuffer[i] = (int) ((long *) (unsigned char *) data)[i] / 65536;
		}

		if (format->channels == 1)
		{
			for (int j = 0; j < samples_size; j++)
			{
				buffer[0][j] = ((((signed char *) (unsigned short *) samplesBuffer)[j * 2 + 1] << 8) | (0x00ff & ((signed char *) (unsigned short *) samplesBuffer)[j * 2 + 0])) / 32768.f;
			}
		}

		if (format->channels == 2)
		{
			for (int j = 0; j < samples_size / 2; j++)
			{
				buffer[0][j] = ((((signed char *) (unsigned short *) samplesBuffer)[j * 4 + 1] << 8) | (0x00ff & ((signed char *) (unsigned short *) samplesBuffer)[j * 4 + 0])) / 32768.f;
				buffer[1][j] = ((((signed char *) (unsigned short *) samplesBuffer)[j * 4 + 3] << 8) | (0x00ff & ((signed char *) (unsigned short *) samplesBuffer)[j * 4 + 2])) / 32768.f;
			}
		}
	}
	else
	{
		if (format->channels == 1)
		{
			for (int j = 0; j < samples_size; j++)
			{
				buffer[0][j] = ((((signed char *) (unsigned char *) data)[j * 2 + 1] << 8) | (0x00ff & ((signed char *) (unsigned char *) data)[j * 2 + 0])) / 32768.f;
			}
		}

		if (format->channels == 2)
		{
			for (int j = 0; j < samples_size / 2; j++)
			{
				buffer[0][j] = ((((signed char *) (unsigned char *) data)[j * 4 + 1] << 8) | (0x00ff & ((signed char *) (unsigned char *) data)[j * 4 + 0])) / 32768.f;
				buffer[1][j] = ((((signed char *) (unsigned char *) data)[j * 4 + 3] << 8) | (0x00ff & ((signed char *) (unsigned char *) data)[j * 4 + 2])) / 32768.f;
			}
		}
	}

	ex_vorbis_analysis_wrote(&vd, samples_size / format->channels);

	while (ex_vorbis_analysis_blockout(&vd, &vb) == 1)
	{
		ex_vorbis_analysis(&vb, NULL);
		ex_vorbis_bitrate_addblock(&vb);

		while(ex_vorbis_bitrate_flushpacket(&vd, &op))
		{
			ex_ogg_stream_packetin(&os, &op);

			do
			{
				int	 result = ex_ogg_stream_pageout(&os, &og);

				if (result == 0) break;

				backBuffer.Resize(dataLength);

				memcpy(backBuffer, dataBuffer, dataLength);

				dataBuffer.Resize(dataLength + og.header_len + og.body_len);

				memcpy(dataBuffer, backBuffer, dataLength);
				memcpy(((unsigned char *) dataBuffer) + dataLength, og.header, og.header_len);
				memcpy(((unsigned char *) dataBuffer) + dataLength + og.header_len, og.body, og.body_len);

				dataLength += og.header_len;
				dataLength += og.body_len;

				if (ex_ogg_page_eos(&og)) break;
			}
			while (true);
		}
	}

	driver->WriteData(dataBuffer, dataLength);

	return dataLength;
}
