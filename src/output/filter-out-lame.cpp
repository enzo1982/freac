 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <iolib-cxx.h>
#include <output/filter-out-lame.h>
#include <dllinterfaces.h>
#include <memory.h>
#include <id3/tag.h>

FilterOutLAME::FilterOutLAME(bonkEncConfig *config, bonkFormatInfo *format) : OutputFilter(config, format)
{
	packageSize = 0;

	if (format->bits != 16)
	{
		SMOOTH::MessageBox("Input files must be 16 bit for LAME MP3 encoding!", "Error", MB_OK, IDI_HAND);

		error = 1;

		return;
	}

	int	 effrate;

	if (currentConfig->lame_resample)	effrate = currentConfig->lame_resample;
	else					effrate = format->rate;

	switch (effrate)
	{
		case 8000:
		case 11025:
		case 12000:
		case 16000:
		case 22050:
		case 24000:
			if (currentConfig->lame_set_bitrate && currentConfig->lame_vbrmode == vbr_off && (currentConfig->lame_bitrate == 192 || currentConfig->lame_bitrate == 224 || currentConfig->lame_bitrate == 256 || currentConfig->lame_bitrate == 320))
			{
				SMOOTH::MessageBox("Bad bitrate! The selected bitrate is not supported for this sampling rate.", "Error", MB_OK, IDI_HAND);

				error = 1;

				return;
			}

			if (currentConfig->lame_set_min_vbr_bitrate && currentConfig->lame_vbrmode != vbr_off && (currentConfig->lame_min_vbr_bitrate == 192 || currentConfig->lame_min_vbr_bitrate == 224 || currentConfig->lame_min_vbr_bitrate == 256 || currentConfig->lame_min_vbr_bitrate == 320))
			{
				SMOOTH::MessageBox("Bad minimum VBR bitrate! The selected minimum VBR bitrate is not supported for this sampling rate.", "Error", MB_OK, IDI_HAND);

				error = 1;

				return;
			}

			if (currentConfig->lame_set_max_vbr_bitrate && currentConfig->lame_vbrmode != vbr_off && (currentConfig->lame_max_vbr_bitrate == 192 || currentConfig->lame_max_vbr_bitrate == 224 || currentConfig->lame_max_vbr_bitrate == 256 || currentConfig->lame_max_vbr_bitrate == 320))
			{
				SMOOTH::MessageBox("Bad maximum VBR bitrate! The selected maximum VBR bitrate is not supported for this sampling rate.", "Error", MB_OK, IDI_HAND);

				error = 1;

				return;
			}
			break;
		case 32000:
		case 44100:
		case 48000:
			if (currentConfig->lame_set_bitrate && currentConfig->lame_vbrmode == vbr_off && (currentConfig->lame_bitrate == 8 || currentConfig->lame_bitrate == 16 || currentConfig->lame_bitrate == 24 || currentConfig->lame_bitrate == 144))
			{
				SMOOTH::MessageBox("Bad bitrate! The selected bitrate is not supported for this sampling rate.", "Error", MB_OK, IDI_HAND);

				error = 1;

				return;
			}

			if (currentConfig->lame_set_min_vbr_bitrate && currentConfig->lame_vbrmode != vbr_off && (currentConfig->lame_min_vbr_bitrate == 8 || currentConfig->lame_min_vbr_bitrate == 16 || currentConfig->lame_min_vbr_bitrate == 24 || currentConfig->lame_min_vbr_bitrate == 144))
			{
				SMOOTH::MessageBox("Bad minimum VBR bitrate! The selected minimum VBR bitrate is not supported for this sampling rate.", "Error", MB_OK, IDI_HAND);

				error = 1;

				return;
			}

			if (currentConfig->lame_set_max_vbr_bitrate && currentConfig->lame_vbrmode != vbr_off && (currentConfig->lame_max_vbr_bitrate == 8 || currentConfig->lame_max_vbr_bitrate == 16 || currentConfig->lame_max_vbr_bitrate == 24 || currentConfig->lame_max_vbr_bitrate == 144))
			{
				SMOOTH::MessageBox("Bad maximum VBR bitrate! The selected maximum VBR bitrate is not supported for this sampling rate.", "Error", MB_OK, IDI_HAND);

				error = 1;

				return;
			}
			break;
		default:
			SMOOTH::MessageBox("Bad sampling rate! The selected sampling rate is not supported.", "Error", MB_OK, IDI_HAND);

			error = 1;

			return;
	}

	if (format->rate != 8000 && format->rate != 11025 && format->rate != 12000 && format->rate != 16000 && format->rate != 22050 && format->rate != 24000 && format->rate != 32000 && format->rate != 44100 && format->rate != 48000)
	{
		SMOOTH::MessageBox("Bad sampling rate! The selected sampling rate is not supported.", "Error", MB_OK, IDI_HAND);

		error = 1;

		return;
	}

	lameFlags = ex_lame_init();

	ex_lame_set_in_samplerate(lameFlags, format->rate);
	ex_lame_set_num_channels(lameFlags, format->channels);

	switch (currentConfig->lame_preset)
	{
		case 0:
			ex_lame_set_copyright(lameFlags, currentConfig->lame_copyright);
			ex_lame_set_original(lameFlags, currentConfig->lame_original);
			ex_lame_set_extension(lameFlags, currentConfig->lame_private);
			ex_lame_set_error_protection(lameFlags, currentConfig->lame_crc);
			ex_lame_set_strict_ISO(lameFlags, currentConfig->lame_strict_iso);
			ex_lame_set_padding_type(lameFlags, (Padding_type) currentConfig->lame_padding_type);

			if (currentConfig->lame_resample) ex_lame_set_out_samplerate(lameFlags, currentConfig->lame_resample);

			if (currentConfig->lame_vbrmode == vbr_off)
			{
				if (currentConfig->lame_set_bitrate)	ex_lame_set_brate(lameFlags, currentConfig->lame_bitrate);
				else					ex_lame_set_compression_ratio(lameFlags, ((double)currentConfig->lame_ratio) / 100);
			}

			if (currentConfig->lame_set_quality)	ex_lame_set_quality(lameFlags, currentConfig->lame_quality);
			else					ex_lame_set_quality(lameFlags, 5);

			if (currentConfig->lame_disable_filtering)
			{
				ex_lame_set_lowpassfreq(lameFlags, -1);
				ex_lame_set_highpassfreq(lameFlags, -1);
			}
			else
			{
				if (currentConfig->lame_set_lowpass) ex_lame_set_lowpassfreq(lameFlags, currentConfig->lame_lowpass);
				if (currentConfig->lame_set_highpass) ex_lame_set_highpassfreq(lameFlags, currentConfig->lame_highpass);

				if (currentConfig->lame_set_lowpass && currentConfig->lame_set_lowpass_width) ex_lame_set_lowpasswidth(lameFlags, currentConfig->lame_lowpass_width);
				if (currentConfig->lame_set_highpass && currentConfig->lame_set_highpass_width) ex_lame_set_highpasswidth(lameFlags, currentConfig->lame_highpass_width);
			}

			if (format->channels == 2)
			{
				if (currentConfig->lame_stereomode == 1)	ex_lame_set_mode(lameFlags, STEREO);
				else if (currentConfig->lame_stereomode == 2)	ex_lame_set_mode(lameFlags, JOINT_STEREO);
				else						ex_lame_set_mode(lameFlags, NOT_SET);

				if (currentConfig->lame_stereomode == 2)
				{
					if (currentConfig->lame_forcejs)	ex_lame_set_force_ms(lameFlags, 1);
					else					ex_lame_set_force_ms(lameFlags, 0);
				}
			}
			else if (format->channels == 1)
			{
				ex_lame_set_mode(lameFlags, MONO);
			}
			else
			{
				ex_lame_close(lameFlags);

				SMOOTH::MessageBox("BonkEnc does not support more than 2 channels!", "Error", MB_OK, IDI_HAND);

				error = 1;

				return;
			}

			switch (currentConfig->lame_vbrmode)
			{
				default:
				case vbr_off:
					break;
				case vbr_abr:
					ex_lame_set_VBR(lameFlags, vbr_abr);
					ex_lame_set_VBR_mean_bitrate_kbps(lameFlags, currentConfig->lame_abrbitrate);
					break;
				case vbr_rh:
					ex_lame_set_VBR(lameFlags, vbr_rh);
					ex_lame_set_VBR_q(lameFlags, currentConfig->lame_vbrquality);
					break;
				case vbr_mtrh:
					ex_lame_set_VBR(lameFlags, vbr_mtrh);
					ex_lame_set_VBR_q(lameFlags, currentConfig->lame_vbrquality);
					break;
			}

			if (currentConfig->lame_vbrmode != vbr_off && currentConfig->lame_set_min_vbr_bitrate) ex_lame_set_VBR_min_bitrate_kbps(lameFlags, currentConfig->lame_min_vbr_bitrate);
			if (currentConfig->lame_vbrmode != vbr_off && currentConfig->lame_set_max_vbr_bitrate) ex_lame_set_VBR_max_bitrate_kbps(lameFlags, currentConfig->lame_max_vbr_bitrate);

			break;
		case 1:
			ex_lame_set_preset(lameFlags, MEDIUM);
			break;
		case 2:
			ex_lame_set_preset(lameFlags, STANDARD);
			break;
		case 3:
			ex_lame_set_preset(lameFlags, EXTREME);
			break;
		case 4:
			ex_lame_set_preset(lameFlags, INSANE);
			break;
		case 5:
			ex_lame_set_preset(lameFlags, MEDIUM_FAST);
			break;
		case 6:
			ex_lame_set_preset(lameFlags, STANDARD_FAST);
			break;
		case 7:
			ex_lame_set_preset(lameFlags, EXTREME_FAST);
			break;
		case 8:
			ex_lame_set_preset(lameFlags, R3MIX);
			break;
		case 9:
			ex_lame_set_preset(lameFlags, currentConfig->lame_abrbitrate);
			break;
	}

	ex_lame_init_params(lameFlags);
}

FilterOutLAME::~FilterOutLAME()
{
}

bool FilterOutLAME::Activate()
{
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

bool FilterOutLAME::Deactivate()
{
	long		 buffersize = 131072;
	unsigned char	*outbuffer = new unsigned char [buffersize];
	unsigned long	 bytes = ex_lame_encode_flush(lameFlags, outbuffer, buffersize);

	driver->WriteData(outbuffer, bytes);

	ex_lame_close(lameFlags);

	return true;
}

int FilterOutLAME::WriteData(unsigned char *data, int size)
{
	long		 buffersize = size + 7200;
	unsigned char	*outbuffer = new unsigned char [buffersize];
	unsigned long	 bytes;

	if (format->channels == 2)	bytes = ex_lame_encode_buffer_interleaved(lameFlags, (signed short *) data, size / (format->bits / 8) / format->channels, outbuffer, buffersize);
	else				bytes = ex_lame_encode_buffer(lameFlags, (signed short *) data, (signed short *) data, size / (format->bits / 8), outbuffer, buffersize);

	driver->WriteData(outbuffer, bytes);

	delete [] outbuffer;

	return bytes;
}
