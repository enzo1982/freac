 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <output/filter-out-lame.h>
#include <dllinterfaces.h>

FilterOutLAME::FilterOutLAME(bonkEncConfig *config, bonkEncTrack *format) : OutputFilter(config, format)
{
	debug_out->EnterMethod("FilterOutLAME::FilterOutLame(bonkEncConfig *, bonkEncTrack *)");

	packageSize = 0;

	int	 effrate;

	if (currentConfig->lame_resample)	effrate = currentConfig->lame_resample;
	else					effrate = format->rate;

	debug_out->OutputLine("Creating LAME output filter...");

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
				QuickMessage("Bad bitrate! The selected bitrate is not supported for this sampling rate.", "Error", MB_OK, IDI_HAND);

				error = 1;

				return;
			}

			if (currentConfig->lame_set_min_vbr_bitrate && currentConfig->lame_vbrmode != vbr_off && (currentConfig->lame_min_vbr_bitrate == 192 || currentConfig->lame_min_vbr_bitrate == 224 || currentConfig->lame_min_vbr_bitrate == 256 || currentConfig->lame_min_vbr_bitrate == 320))
			{
				QuickMessage("Bad minimum VBR bitrate! The selected minimum VBR bitrate is not supported for this sampling rate.", "Error", MB_OK, IDI_HAND);

				error = 1;

				return;
			}

			if (currentConfig->lame_set_max_vbr_bitrate && currentConfig->lame_vbrmode != vbr_off && (currentConfig->lame_max_vbr_bitrate == 192 || currentConfig->lame_max_vbr_bitrate == 224 || currentConfig->lame_max_vbr_bitrate == 256 || currentConfig->lame_max_vbr_bitrate == 320))
			{
				QuickMessage("Bad maximum VBR bitrate! The selected maximum VBR bitrate is not supported for this sampling rate.", "Error", MB_OK, IDI_HAND);

				error = 1;

				return;
			}
			break;
		case 32000:
		case 44100:
		case 48000:
			if (currentConfig->lame_set_bitrate && currentConfig->lame_vbrmode == vbr_off && (currentConfig->lame_bitrate == 8 || currentConfig->lame_bitrate == 16 || currentConfig->lame_bitrate == 24 || currentConfig->lame_bitrate == 144))
			{
				QuickMessage("Bad bitrate! The selected bitrate is not supported for this sampling rate.", "Error", MB_OK, IDI_HAND);

				error = 1;

				return;
			}

			if (currentConfig->lame_set_min_vbr_bitrate && currentConfig->lame_vbrmode != vbr_off && (currentConfig->lame_min_vbr_bitrate == 8 || currentConfig->lame_min_vbr_bitrate == 16 || currentConfig->lame_min_vbr_bitrate == 24 || currentConfig->lame_min_vbr_bitrate == 144))
			{
				QuickMessage("Bad minimum VBR bitrate! The selected minimum VBR bitrate is not supported for this sampling rate.", "Error", MB_OK, IDI_HAND);

				error = 1;

				return;
			}

			if (currentConfig->lame_set_max_vbr_bitrate && currentConfig->lame_vbrmode != vbr_off && (currentConfig->lame_max_vbr_bitrate == 8 || currentConfig->lame_max_vbr_bitrate == 16 || currentConfig->lame_max_vbr_bitrate == 24 || currentConfig->lame_max_vbr_bitrate == 144))
			{
				QuickMessage("Bad maximum VBR bitrate! The selected maximum VBR bitrate is not supported for this sampling rate.", "Error", MB_OK, IDI_HAND);

				error = 1;

				return;
			}
			break;
		default:
			QuickMessage("Bad sampling rate! The selected sampling rate is not supported.", "Error", MB_OK, IDI_HAND);

			error = 1;

			return;
	}

	if (format->rate != 8000 && format->rate != 11025 && format->rate != 12000 && format->rate != 16000 && format->rate != 22050 && format->rate != 24000 && format->rate != 32000 && format->rate != 44100 && format->rate != 48000)
	{
		QuickMessage("Bad sampling rate! The selected sampling rate is not supported.", "Error", MB_OK, IDI_HAND);

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

				QuickMessage("BonkEnc does not support more than 2 channels!", "Error", MB_OK, IDI_HAND);

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

			if (currentConfig->lame_enable_ath)
			{
				if (currentConfig->lame_athtype != -1) ex_lame_set_ATHtype(lameFlags, currentConfig->lame_athtype);
			}
			else
			{
				ex_lame_set_noATH(lameFlags, 1);
			}

			ex_lame_set_useTemporal(lameFlags, currentConfig->lame_use_tns);

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

	debug_out->LeaveMethod();
}

FilterOutLAME::~FilterOutLAME()
{
}

bool FilterOutLAME::Activate()
{
	debug_out->EnterMethod("FilterOutLAME::Activate()");

	outBuffer.Resize(131072);

	if ((format->artist != NIL || format->title != NIL) && currentConfig->enable_tags && currentConfig->enable_id3)
	{
		Buffer<unsigned char>	 id3Buffer(32768);
		Int			 size = RenderID3V2Tag(id3Buffer);

		driver->WriteData(id3Buffer, size);
	}

	ex_lame_set_bWriteVbrTag(lameFlags, 1);

	debug_out->LeaveMethod();

	return true;
}

bool FilterOutLAME::Deactivate()
{
	debug_out->EnterMethod("FilterOutLAME::Deactivate()");

	unsigned long	 bytes = ex_lame_encode_flush(lameFlags, outBuffer, outBuffer.Size());

	driver->WriteData(outBuffer, bytes);

	if (currentConfig->lame_vbrmode != vbr_off)
	{
		FILE	*f_out = fopen(Application::GetApplicationDirectory().Append("xing.tmp"), "w+b");

		if (f_out != NIL)
		{
			Buffer<unsigned char>	 buffer(driver->GetSize());

			driver->Seek(0);
			driver->ReadData(buffer, buffer.Size());

			fwrite(buffer, 1, buffer.Size(), f_out);

			ex_lame_mp3_tags_fid(lameFlags, f_out);

			buffer.Resize(ftell(f_out));

			fseek(f_out, 0, SEEK_SET);

			fread((void *) buffer, 1, buffer.Size(), f_out);

			driver->Seek(0);
			driver->WriteData(buffer, buffer.Size());

			fclose(f_out);
			remove(Application::GetApplicationDirectory().Append("xing.tmp"));
		}
	}

	ex_lame_close(lameFlags);

	debug_out->LeaveMethod();

	return true;
}

int FilterOutLAME::WriteData(unsigned char *data, int size)
{
	debug_out->EnterMethod("FilterOutLAME::WriteData(unsigned char *, int)");

	unsigned long	 bytes = 0;

	outBuffer.Resize(size + 7200);

	if (format->bits != 16)
	{
		samplesBuffer.Resize(size / (format->bits / 8));

		for (int i = 0; i < size / (format->bits / 8); i++)
		{
			if (format->bits == 8)	samplesBuffer[i] = (data[i] - 128) * 256;
			if (format->bits == 24) samplesBuffer[i] = (int) (data[3 * i] + 256 * data[3 * i + 1] + 65536 * data[3 * i + 2] - (data[3 * i + 2] & 128 ? 16777216 : 0)) / 256;
			if (format->bits == 32)	samplesBuffer[i] = (int) ((long *) data)[i] / 65536;
		}

		if (format->channels == 2)	bytes = ex_lame_encode_buffer_interleaved(lameFlags, samplesBuffer, size / (format->bits / 8) / format->channels, outBuffer, outBuffer.Size());
		else				bytes = ex_lame_encode_buffer(lameFlags, samplesBuffer, samplesBuffer, size / (format->bits / 8), outBuffer, outBuffer.Size());
	}
	else
	{
		if (format->channels == 2)	bytes = ex_lame_encode_buffer_interleaved(lameFlags, (signed short *) data, size / (format->bits / 8) / format->channels, outBuffer, outBuffer.Size());
		else				bytes = ex_lame_encode_buffer(lameFlags, (signed short *) data, (signed short *) data, size / (format->bits / 8), outBuffer, outBuffer.Size());
	}

	driver->WriteData(outBuffer, bytes);

	debug_out->LeaveMethod();

	return bytes;
}
