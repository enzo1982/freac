 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-vorbis.h>

BonkEnc::FilterInVORBIS::FilterInVORBIS(Config *config, Track *format) : InputFilter(config, format)
{
	packageSize = 0;
}

BonkEnc::FilterInVORBIS::~FilterInVORBIS()
{
}

bool BonkEnc::FilterInVORBIS::Activate()
{
	ex_ogg_sync_init(&oy);

	Int	 size = 4096;

	inBytes += size;

	buffer = ex_ogg_sync_buffer(&oy, size);

	driver->ReadData((unsigned char *) buffer, size);

	ex_ogg_sync_wrote(&oy, size);

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
		else
		{
			inBytes += size;

			buffer = ex_ogg_sync_buffer(&oy, size);

			driver->ReadData((unsigned char *) buffer, size);

			ex_ogg_sync_wrote(&oy, size);
		}
	}

	ex_vorbis_synthesis_init(&vd, &vi);
	ex_vorbis_block_init(&vd, &vb);

	return true;
}

bool BonkEnc::FilterInVORBIS::Deactivate()
{
	ex_ogg_stream_clear(&os);

	ex_vorbis_block_clear(&vb);
	ex_vorbis_dsp_clear(&vd);
	ex_vorbis_comment_clear(&vc);
	ex_vorbis_info_clear(&vi);

	ex_ogg_sync_clear(&oy);

	return true;
}

int BonkEnc::FilterInVORBIS::ReadData(unsigned char **data, int size)
{
	if (size <= 0) return -1;

	inBytes += size;

	buffer = ex_ogg_sync_buffer(&oy, size);

	driver->ReadData((unsigned char *) buffer, size);

	ex_ogg_sync_wrote(&oy, size);

	size = 0;

	int	 dataBufferLen = 0;

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

					backBuffer.Resize(size);

					memcpy(backBuffer, dataBuffer, size);

					dataBuffer.Resize(dataBufferLen);

					memcpy(dataBuffer, backBuffer, size);

					memcpy(dataBuffer + size, convbuffer, bout * vi.channels * 2);

					size += (bout * vi.channels * 2);
				}
				else
				{
					memcpy(dataBuffer + size, convbuffer, bout * vi.channels * 2);

					size += (bout * vi.channels * 2);
				}

				ex_vorbis_synthesis_read(&vd, bout);
			}
		}

		if (ex_ogg_page_eos(&og)) break;
	}

	*data = dataBuffer;

	return size;
}

Track *BonkEnc::FilterInVORBIS::GetFileInfo(String inFile)
{
	Track		*nFormat = new Track;
	InStream	*f_in = OpenFile(inFile);

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

	Int	 size = 4096;
	char	*fbuffer = ex_ogg_sync_buffer(&foy, size);

	f_in->InputData(fbuffer, size);

	ex_ogg_sync_wrote(&foy, size);

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
		else
		{
			fbuffer = ex_ogg_sync_buffer(&foy, size);

			f_in->InputData(fbuffer, size);

			ex_ogg_sync_wrote(&foy, size);
		}
	}

	nFormat->rate = fvi.rate;
	nFormat->channels = fvi.channels;
	nFormat->length = -1;

	if (fvc.comments > 0)
	{
		nFormat->track = -1;
		nFormat->outfile = NIL;

		char	*prevInFormat = String::SetInputFormat("UTF-8");

		for (Int j = 0; j < fvc.comments; j++)
		{
			char	*buffer = new char [fvc.comment_lengths[j]];

			if (String("TITLE").CompareN(String(fvc.user_comments[j]).ToUpper(), 5) == 0)
			{
				for (Int p = 0; p < fvc.comment_lengths[j] - 5; p++)
				{
					buffer[p] = fvc.user_comments[j][p + 6];
				}

				nFormat->title = buffer;
			}
			else if (String("ARTIST").CompareN(String(fvc.user_comments[j]).ToUpper(), 6) == 0)
			{
				for (Int p = 0; p < fvc.comment_lengths[j] - 6; p++)
				{
					buffer[p] = fvc.user_comments[j][p + 7];
				}

				nFormat->artist = buffer;
			}
			else if (String("ALBUM").CompareN(String(fvc.user_comments[j]).ToUpper(), 5) == 0)
			{
				for (Int p = 0; p < fvc.comment_lengths[j] - 5; p++)
				{
					buffer[p] = fvc.user_comments[j][p + 6];
				}

				nFormat->album = buffer;
			}
			else if (String("GENRE").CompareN(String(fvc.user_comments[j]).ToUpper(), 5) == 0)
			{
				for (Int p = 0; p < fvc.comment_lengths[j] - 5; p++)
				{
					buffer[p] = fvc.user_comments[j][p + 6];
				}

				nFormat->genre = buffer;
			}
			else if (String("DATE").CompareN(String(fvc.user_comments[j]).ToUpper(), 4) == 0)
			{
				String	 year;

				for (Int p = 0; p < fvc.comment_lengths[j] - 5; p++)
				{
					year[p] = fvc.user_comments[j][p + 5];
				}

				nFormat->year = year.ToInt();
			}
			else if (String("TRACKNUMBER").CompareN(String(fvc.user_comments[j]).ToUpper(), 11) == 0)
			{
				String	 track;

				for (Int p = 0; p < fvc.comment_lengths[j] - 12; p++)
				{
					track[p] = fvc.user_comments[j][p + 12];
				}

				nFormat->track = track.ToInt();
			}

			delete [] buffer;
		}

		String::SetInputFormat(prevInFormat);
	}

	ex_ogg_stream_clear(&fos);

	ex_vorbis_comment_clear(&fvc);
	ex_vorbis_info_clear(&fvi);

	ex_ogg_sync_clear(&foy);

	CloseFile(f_in);

	return nFormat;
}
