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
#include <input/filter-in-vorbis.h>
#include <dllinterfaces.h>

FilterInVORBIS::FilterInVORBIS(bonkEncConfig *config) : InputFilter(config)
{
	setup = false;

	packageSize = 0;

	ex_ogg_sync_init(&oy);
}

FilterInVORBIS::~FilterInVORBIS()
{
	ex_ogg_stream_clear(&os);

	ex_vorbis_block_clear(&vb);
	ex_vorbis_dsp_clear(&vd);
	ex_vorbis_comment_clear(&vc);
	ex_vorbis_info_clear(&vi);

	ex_ogg_sync_clear(&oy);

	delete [] buffer;
}

bool FilterInVORBIS::EncodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	return true;
}

bool FilterInVORBIS::DecodeData(unsigned char **data, int size, int *outsize)
{
	if (size <= 0)
	{
		*outsize = 0;

		return false;
	}

	inBytes += size;

	buffer = ex_ogg_sync_buffer(&oy, size);

	memcpy((void *) buffer, (void *) *data, size);

	delete [] *data;

	*data = NULL;

	*outsize = 0;

	int	 dataBufferLen = 0;

	ex_ogg_sync_wrote(&oy, size);

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

		format.rate = vi.rate;
		format.channels = vi.channels;
		format.bits = 16;
		format.order = BYTE_INTEL;
		format.length = -1;

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

				if (dataBufferLen < *outsize + (bout * vi.channels * 2))
				{
					dataBufferLen += 131072;

					unsigned char *backbuffer = new unsigned char [*outsize];

					memcpy((void *) backbuffer, (void *) *data, *outsize);

					delete [] *data;

					*data = new unsigned char [dataBufferLen];

					memcpy((void *) *data, (void *) backbuffer, *outsize);

					delete [] backbuffer;

					memcpy((void *) (*data + *outsize), (void *) convbuffer, bout * vi.channels * 2);

					*outsize += (bout * vi.channels * 2);
				}
				else
				{
					memcpy((void *) (*data + *outsize), (void *) convbuffer, bout * vi.channels * 2);

					*outsize += (bout * vi.channels * 2);
				}

				ex_vorbis_synthesis_read(&vd, bout);
			}
		}

		if (ex_ogg_page_eos(&og)) break;
	}

	return true;
}

bonkFormatInfo FilterInVORBIS::GetAudioFormat()
{
	return format;
}
