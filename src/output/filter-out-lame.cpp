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
#include <output/filter-out-lame.h>
#include <dllinterfaces.h>
#include <memory.h>

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

	ex_lame_init_params(lameFlags);
}

FilterOutLAME::~FilterOutLAME()
{
}

bool FilterOutLAME::EncodeData(unsigned char **data, int size, int *outsize)
{
	long		 buffersize = size + 7800;
	unsigned char	*outbuffer = new unsigned char [buffersize];

	if (format->channels == 2)	*outsize = ex_lame_encode_buffer_interleaved(lameFlags, (signed short *) *data, size / (format->bits / 8) / format->channels, outbuffer, buffersize);
	else				*outsize = ex_lame_encode_buffer(lameFlags, (signed short *) *data, (signed short *) *data, size / (format->bits / 8), outbuffer, buffersize);

	delete [] *data;

	*data = new unsigned char [*outsize];

	memcpy((void *) *data, (void *) outbuffer, *outsize);

	if (lastPacket)
	{
		unsigned long 	 bytes = ex_lame_encode_flush(lameFlags, outbuffer, buffersize);
		unsigned char	*buffer = new unsigned char [*outsize];

		memcpy((void *) buffer, (void *) *data, *outsize);

		delete [] *data;

		*data = new unsigned char [*outsize + bytes];

		memcpy((void *) *data, (void *) buffer, *outsize);

		delete [] buffer;

		memcpy((void *) (*data + *outsize), (void *) outbuffer, bytes);

		*outsize += bytes;

		ex_lame_close(lameFlags);
	}

	delete [] outbuffer;

	return true;
}

bool FilterOutLAME::DecodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	return true;
}
