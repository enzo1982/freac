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
#include <input/filter-in-vorbis.h>
#include <dllinterfaces.h>

FilterInVORBIS::FilterInVORBIS(bonkEncConfig *config) : InputFilter(config)
{
	setup = false;

	packageSize = 0;

	buffer = NIL;

	ex_ogg_sync_init(&oy);
}

FilterInVORBIS::~FilterInVORBIS()
{
	if (buffer != NIL)
	{
		ex_ogg_stream_clear(&os);

		ex_vorbis_block_clear(&vb);
		ex_vorbis_dsp_clear(&vd);
		ex_vorbis_comment_clear(&vc);
		ex_vorbis_info_clear(&vi);
	}

	ex_ogg_sync_clear(&oy);
}

int FilterInVORBIS::ReadData(unsigned char **data, int size)
{
	if (size <= 0) return -1;

	inBytes += size;

	*data = new unsigned char [size];

	driver->ReadData(*data, size);

	buffer = ex_ogg_sync_buffer(&oy, size);

	memcpy((void *) buffer, (void *) *data, size);

	delete [] *data;

	*data = NULL;

	int	 dataBufferLen = 0;

	ex_ogg_sync_wrote(&oy, size);

	size = 0;

	if (!setup)
	{
		setup = true;

		ex_ogg_sync_pageout(&oy, &og);

		ex_ogg_stream_init(&os, ex_ogg_page_serialno(&og)); 

		ex_vorbis_info_init(&vi);
		ex_vorbis_comment_init(&vc);

		ex_ogg_stream_pagein(&os, &og);
		ex_ogg_stream_packetout(&os, &op);

		ex_vorbis_synthesis_headerin(&vi, &vc, &op);

		int	 i = 0;

		while (i < 2)
		{
			if (ex_ogg_sync_pageout(&oy, &og) == 1)
			{
				ex_ogg_stream_pagein(&os, &og);

				while (i < 2)
				{
					if (ex_ogg_stream_packetout(&os, &op) == 0) break;

					ex_vorbis_synthesis_headerin(&vi, &vc, &op); 

					i++;
				}
			}
		}

		ex_vorbis_synthesis_init(&vd, &vi);
		ex_vorbis_block_init(&vd, &vb);
	}

	while (true)
	{
		short	 convbuffer[4096];
		int	 convsize = 4096 / vi.channels;

		if (ex_ogg_sync_pageout(&oy, &og) == 0) break;

		ex_ogg_stream_pagein(&os, &og);

		while (true)
		{
			if (ex_ogg_stream_packetout(&os, &op) == 0) break;

			float  **pcm;
			int	 samples;

			ex_vorbis_synthesis(&vb, &op);
			ex_vorbis_synthesis_blockin(&vd, &vb);

			while ((samples = ex_vorbis_synthesis_pcmout(&vd, &pcm)) > 0)
			{
				int	 bout = (samples < convsize ? samples : convsize);

				for (int i = 0; i < vi.channels; i++)
				{
					short	*ptr = convbuffer + i;
					float	*mono = pcm[i];

					for (int j = 0; j < bout; j++)
					{
						int	 val = int(mono[j] * 32767.f);

						val = min(val, 32767);
						val = max(val, -32768);

						*ptr = val;
						ptr += vi.channels;
					}
				}

				if (dataBufferLen < size + (bout * vi.channels * 2))
				{
					dataBufferLen += ((bout * vi.channels * 2) + 131072);

					unsigned char *backbuffer = new unsigned char [size];

					memcpy((void *) backbuffer, (void *) *data, size);

					delete [] *data;

					*data = new unsigned char [dataBufferLen];

					memcpy((void *) *data, (void *) backbuffer, size);

					delete [] backbuffer;

					memcpy((void *) (*data + size), (void *) convbuffer, bout * vi.channels * 2);

					size += (bout * vi.channels * 2);
				}
				else
				{
					memcpy((void *) (*data + size), (void *) convbuffer, bout * vi.channels * 2);

					size += (bout * vi.channels * 2);
				}

				ex_vorbis_synthesis_read(&vd, bout);
			}
		}

		if (ex_ogg_page_eos(&og)) break;
	}

	return size;
}

bonkFormatInfo *FilterInVORBIS::GetFileInfo(String inFile)
{
	bonkFormatInfo	*nFormat = new bonkFormatInfo;
	InStream	*f_in = new InStream(STREAM_FILE, inFile, IS_READONLY);

	nFormat->order = BYTE_INTEL;
	nFormat->bits = 16;
	nFormat->fileSize = f_in->Size();

	ogg_sync_state		 foy;
	ogg_stream_state	 fos;
	ogg_page		 fog;
	ogg_packet		 fop;

	vorbis_info		 fvi;
	vorbis_comment		 fvc;

	ex_ogg_sync_init(&foy);

	Int		 size = 32768;
	unsigned char	*data = new unsigned char [size];

	f_in->InputData((void *) data, size);

	char	*fbuffer = ex_ogg_sync_buffer(&foy, size);

	memcpy((void *) fbuffer, (void *) data, size);

	delete [] data;

	ex_ogg_sync_wrote(&foy, size);

	size = 0;

	ex_ogg_sync_pageout(&foy, &fog);

	ex_ogg_stream_init(&fos, ex_ogg_page_serialno(&fog)); 

	ex_vorbis_info_init(&fvi);
	ex_vorbis_comment_init(&fvc);

	ex_ogg_stream_pagein(&fos, &fog);
	ex_ogg_stream_packetout(&fos, &fop);

	ex_vorbis_synthesis_headerin(&fvi, &fvc, &fop);

	Int	 i = 0;

	while (i < 2)
	{
		if (ex_ogg_sync_pageout(&foy, &fog) == 1)
		{
			ex_ogg_stream_pagein(&fos, &fog);

			while (i < 2)
			{
				if (ex_ogg_stream_packetout(&fos, &fop) == 0) break;

				ex_vorbis_synthesis_headerin(&fvi, &fvc, &fop); 

				i++;
			}
		}
	}

	nFormat->rate = fvi.rate;
	nFormat->channels = fvi.channels;
	nFormat->length = -1;

	if (fvc.comments > 0)
	{
		nFormat->trackInfo->track = -1;
		nFormat->trackInfo->outfile = NIL;
		nFormat->trackInfo->hasText = True;

		String	 prevInFormat = String::SetInputFormat("UTF-8");

		for (Int j = 0; j < fvc.comments; j++)
		{
			char	*buffer = new char [fvc.comment_lengths[j]];

			if (String("TITLE").CompareN(fvc.user_comments[j], 5) == 0)
			{
				for (Int p = 0; p < fvc.comment_lengths[j] - 5; p++)
				{
					buffer[p] = fvc.user_comments[j][p + 6];
				}

				nFormat->trackInfo->title = buffer;
			}
			else if (String("ARTIST").CompareN(fvc.user_comments[j], 6) == 0)
			{
				for (Int p = 0; p < fvc.comment_lengths[j] - 6; p++)
				{
					buffer[p] = fvc.user_comments[j][p + 7];
				}

				nFormat->trackInfo->artist = buffer;
			}
			else if (String("ALBUM").CompareN(fvc.user_comments[j], 5) == 0)
			{
				for (Int p = 0; p < fvc.comment_lengths[j] - 5; p++)
				{
					buffer[p] = fvc.user_comments[j][p + 6];
				}

				nFormat->trackInfo->album = buffer;
			}
			else if (String("GENRE").CompareN(fvc.user_comments[j], 5) == 0)
			{
				for (Int p = 0; p < fvc.comment_lengths[j] - 5; p++)
				{
					buffer[p] = fvc.user_comments[j][p + 6];
				}

				nFormat->trackInfo->genre = buffer;
			}
			else if (String("DATE").CompareN(fvc.user_comments[j], 4) == 0)
			{
				String	 year;

				for (Int p = 0; p < fvc.comment_lengths[j] - 5; p++)
				{
					year[p] = fvc.user_comments[j][p + 5];
				}

				nFormat->trackInfo->year = year.ToInt();
			}
			else if (String("TRACKNUMBER").CompareN(fvc.user_comments[j], 11) == 0)
			{
				String	 track;

				for (Int p = 0; p < fvc.comment_lengths[j] - 12; p++)
				{
					track[p] = fvc.user_comments[j][p + 12];
				}

				nFormat->trackInfo->track = track.ToInt();
			}

			delete [] buffer;
		}

		String::SetInputFormat(prevInFormat);
	}

	ex_ogg_stream_clear(&fos);

	ex_vorbis_comment_clear(&fvc);
	ex_vorbis_info_clear(&fvi);

	ex_ogg_sync_clear(&foy);

	delete f_in;

	return nFormat;
}
