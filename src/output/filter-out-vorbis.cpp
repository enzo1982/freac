 /* BonkEnc version 0.8
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <iolib-cxx.h>
#include <output/filter-out-vorbis.h>
#include <dllinterfaces.h>
#include <memory.h>

FilterOutVORBIS::FilterOutVORBIS(bonkEncConfig *config, bonkFormatInfo *format) : OutputFilter(config, format)
{
	packageSize = 0;

	setup = false;

	if (format->channels != 2)
	{
		SMOOTH::MessageBox("Input files must be stereo for Ogg Vorbis encoding!", "Error", MB_OK, IDI_HAND);

		error = 1;

		return;
	}

	if (format->bits != 16)
	{
		SMOOTH::MessageBox("Input files must be 16 bit for Ogg Vorbis encoding!", "Error", MB_OK, IDI_HAND);

		error = 1;

		return;
	}

	srand(clock());

	ex_vorbis_info_init(&vi);

	switch (currentConfig->vorbis_mode)
	{
		case 0:
			ex_vorbis_encode_init_vbr(&vi, 2, format->rate, ((double) currentConfig->vorbis_quality) / 100);
			break;
		case 1:
			ex_vorbis_encode_init(&vi, 2, format->rate, -1, currentConfig->vorbis_bitrate * 1000, -1);
			break;
	}

	ex_vorbis_comment_init(&vc);
	ex_vorbis_comment_add_tag(&vc, "COMMENT", "BonkEnc v0.8 <http://www.bonkenc.org>");

	if (format->trackInfo->cdText)
	{
		ex_vorbis_comment_add_tag(&vc, "TITLE", format->trackInfo->title);
		ex_vorbis_comment_add_tag(&vc, "ARTIST", format->trackInfo->artist);
		ex_vorbis_comment_add_tag(&vc, "ALBUM", format->trackInfo->album);

		if (format->trackInfo->track < 10)	ex_vorbis_comment_add_tag(&vc, "TRACKNUMBER", SMOOTHString("0").Append(SMOOTHString::IntToString(format->trackInfo->track)));
		else					ex_vorbis_comment_add_tag(&vc, "TRACKNUMBER", SMOOTHString::IntToString(format->trackInfo->track));

		ex_vorbis_comment_add_tag(&vc, "DATE", format->trackInfo->year);
		ex_vorbis_comment_add_tag(&vc, "GENRE", format->trackInfo->genre);
	}

	ex_vorbis_analysis_init(&vd, &vi);
	ex_vorbis_block_init(&vd, &vb);

	ex_ogg_stream_init(&os, rand());
}

FilterOutVORBIS::~FilterOutVORBIS()
{
}

int FilterOutVORBIS::WriteData(unsigned char *data, int size)
{
	unsigned char	*dataBuffer = NIL;
	int		 dataLength = 0;
	int		 samples_size = size / (format->bits / 8);

	if (!setup)
	{
		setup = true;

		ogg_packet	 header;
		ogg_packet	 header_comm;
		ogg_packet	 header_code;


		ex_vorbis_analysis_headerout(&vd, &vc, &header, &header_comm, &header_code);

		ex_ogg_stream_packetin(&os, &header); /* automatically placed in its own page */

		ex_ogg_stream_packetin(&os, &header_comm);
		ex_ogg_stream_packetin(&os, &header_code);

		do
		{
			int result = ex_ogg_stream_flush(&os, &og);

			if (result == 0) break;

			unsigned char	*backBuffer = new unsigned char [dataLength];

			memcpy((void *) backBuffer, (void *) dataBuffer, dataLength);

			delete [] dataBuffer;

			dataBuffer = new unsigned char [dataLength + og.header_len + og.body_len];

			memcpy((void *) dataBuffer, (void *) backBuffer, dataLength);
			memcpy((void *) (dataBuffer + dataLength), og.header, og.header_len);
			memcpy((void *) (dataBuffer + dataLength + og.header_len), og.body, og.body_len);

			dataLength += og.header_len;
			dataLength += og.body_len;
		}
		while (true);
	}

	float	**buffer = ex_vorbis_analysis_buffer(&vd, samples_size / 2);

	for (int j = 0; j < samples_size / 2; j++)
	{
		buffer[0][j] = ((((signed char *) data)[j * 4 + 1] << 8) | (0x00ff & ((signed char *) data)[j * 4 + 0])) / 32768.f;
		buffer[1][j] = ((((signed char *) data)[j * 4 + 3] << 8) | (0x00ff & ((signed char *) data)[j * 4 + 2])) / 32768.f;
	}

	ex_vorbis_analysis_wrote(&vd, samples_size / 2);

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

				unsigned char	*backBuffer = new unsigned char [dataLength];

				memcpy((void *) backBuffer, (void *) dataBuffer, dataLength);

				delete [] dataBuffer;

				dataBuffer = new unsigned char [dataLength + og.header_len + og.body_len];

				memcpy((void *) dataBuffer, (void *) backBuffer, dataLength);
				memcpy((void *) (dataBuffer + dataLength), og.header, og.header_len);
				memcpy((void *) (dataBuffer + dataLength + og.header_len), og.body, og.body_len);

				dataLength += og.header_len;
				dataLength += og.body_len;

				if (ex_ogg_page_eos(&og)) break;
			}
			while (true);
		}
	}

	if (lastPacket)
	{
		ex_vorbis_analysis_wrote(&vd, 0);

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

					unsigned char	*backBuffer = new unsigned char [dataLength];

					memcpy((void *) backBuffer, (void *) dataBuffer, dataLength);

					delete [] dataBuffer;

					dataBuffer = new unsigned char [dataLength + og.header_len + og.body_len];

					memcpy((void *) dataBuffer, (void *) backBuffer, dataLength);
					memcpy((void *) (dataBuffer + dataLength), og.header, og.header_len);
					memcpy((void *) (dataBuffer + dataLength + og.header_len), og.body, og.body_len);

					dataLength += og.header_len;
					dataLength += og.body_len;

					if (ex_ogg_page_eos(&og)) break;
				}
				while (true);
			}
		}

		ex_ogg_stream_clear(&os);
		ex_vorbis_block_clear(&vb);
		ex_vorbis_dsp_clear(&vd);
		ex_vorbis_comment_clear(&vc);
		ex_vorbis_info_clear(&vi);
	}


	driver->WriteData(dataBuffer, dataLength);

	delete [] dataBuffer;

	return dataLength;
}
