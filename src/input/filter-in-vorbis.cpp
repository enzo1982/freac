 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-vorbis.h>

#include <dllinterfaces.h>

BonkEnc::FilterInVORBIS::FilterInVORBIS(Config *config, Track *format) : InputFilter(config, format)
{
	packageSize = 0;
}

BonkEnc::FilterInVORBIS::~FilterInVORBIS()
{
}

Bool BonkEnc::FilterInVORBIS::Activate()
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

Bool BonkEnc::FilterInVORBIS::Deactivate()
{
	ex_ogg_stream_clear(&os);

	ex_vorbis_block_clear(&vb);
	ex_vorbis_dsp_clear(&vd);
	ex_vorbis_comment_clear(&vc);
	ex_vorbis_info_clear(&vi);

	ex_ogg_sync_clear(&oy);

	return true;
}

Int BonkEnc::FilterInVORBIS::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	if (size <= 0) return -1;

	inBytes += size;

	buffer = ex_ogg_sync_buffer(&oy, size);

	driver->ReadData((unsigned char *) buffer, size);

	ex_ogg_sync_wrote(&oy, size);

	size = 0;

	Int	 dataBufferLen = 0;

	while (true)
	{
		short	 convbuffer[6144];
		int	 convsize = 6144 / vi.channels;

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

					memcpy(backBuffer, data, size);

					data.Resize(dataBufferLen);

					memcpy(data, backBuffer, size);

					memcpy(((unsigned char *) data) + size, convbuffer, bout * vi.channels * 2);

					size += (bout * vi.channels * 2);
				}
				else
				{
					memcpy(((unsigned char *) data) + size, convbuffer, bout * vi.channels * 2);

					size += (bout * vi.channels * 2);
				}

				ex_vorbis_synthesis_read(&vd, bout);
			}
		}

		if (ex_ogg_page_eos(&og)) break;
	}

	return size;
}

BonkEnc::Track *BonkEnc::FilterInVORBIS::GetFileInfo(const String &inFile)
{
	Track		*nFormat = new Track;
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

	Int	 size = Math::Min(4096, nFormat->fileSize);
	char	*fbuffer = ex_ogg_sync_buffer(&foy, size);

	f_in->InputData(fbuffer, size);

	ex_ogg_sync_wrote(&foy, size);

	if (foy.data != NIL)
	{
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

		Int	 bitrate = 0;

		if (fvi.bitrate_nominal > 0)				 bitrate = fvi.bitrate_nominal;
	 	else if (fvi.bitrate_lower > 0 && fvi.bitrate_upper > 0) bitrate = (fvi.bitrate_lower + fvi.bitrate_upper) / 2;

		if (bitrate > 0) nFormat->approxLength = nFormat->fileSize / (bitrate / 8) * nFormat->rate * nFormat->channels;

		if (fvc.comments > 0)
		{
			nFormat->track = -1;
			nFormat->outfile = NIL;

			char	*prevInFormat = String::SetInputFormat("UTF-8");

			for (Int j = 0; j < fvc.comments; j++)
			{
				String	 comment = String(fvc.user_comments[j]);
				String	 id = String().CopyN(comment, comment.Find("=")).ToUpper();

				if	(id == "TITLE")		nFormat->title	= comment.Tail(comment.Length() - 6);
				else if (id == "ARTIST")	nFormat->artist	= comment.Tail(comment.Length() - 7);
				else if (id == "ALBUM")		nFormat->album	= comment.Tail(comment.Length() - 6);
				else if (id == "GENRE")		nFormat->genre	= comment.Tail(comment.Length() - 6);
				else if (id == "DATE")		nFormat->year	= comment.Tail(comment.Length() - 5).ToInt();
				else if (id == "TRACKNUMBER")	nFormat->track	= comment.Tail(comment.Length() - 12).ToInt();
			}

			String::SetInputFormat(prevInFormat);
		}

		ex_ogg_stream_clear(&fos);

		ex_vorbis_comment_clear(&fvc);
		ex_vorbis_info_clear(&fvi);
	}
	else
	{
		delete nFormat;

		nFormat = NIL;
	}

	ex_ogg_sync_clear(&foy);

	delete f_in;

	return nFormat;
}
