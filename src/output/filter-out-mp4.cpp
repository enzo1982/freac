 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <output/filter-out-mp4.h>
#include <dllinterfaces.h>

BonkEnc::FilterOutMP4::FilterOutMP4(Config *config, Track *format) : OutputFilter(config, format)
{
	if (format->channels > 2)
	{
		QuickMessage("BonkEnc does not support more than 2 channels!", "Error", MB_OK, IDI_HAND);

		error = 1;

		return;
	}
}

BonkEnc::FilterOutMP4::~FilterOutMP4()
{
}

bool BonkEnc::FilterOutMP4::Activate()
{
	if (GetTempFile(format->outfile) != format->outfile)
	{
		File	 mp4File(format->outfile);

		mp4File.Delete();
	}

	unsigned long	 samplesSize	= 0;
	unsigned long	 bufferSize	= 0;

	handle = ex_faacEncOpen(format->rate, format->channels, &samplesSize, &bufferSize);

	outBuffer.Resize(bufferSize);
	samplesBuffer.Resize(samplesSize);

	fConfig = ex_faacEncGetCurrentConfiguration(handle);

	fConfig->mpegVersion	= MPEG4;
	fConfig->aacObjectType	= currentConfig->faac_type;
	fConfig->allowMidside	= currentConfig->faac_allowjs;
	fConfig->useTns		= currentConfig->faac_usetns;
	fConfig->bandWidth	= currentConfig->faac_bandwidth;
	fConfig->outputFormat	= 0; // Raw AAC frame headers

	if (currentConfig->faac_set_quality)	fConfig->quantqual	= currentConfig->faac_aac_quality;
	else					fConfig->bitRate	= currentConfig->faac_bitrate * 1000;

	if (format->bits == 8)	fConfig->inputFormat	= FAAC_INPUT_16BIT;
	if (format->bits == 16)	fConfig->inputFormat	= FAAC_INPUT_16BIT;
	if (format->bits == 24)	fConfig->inputFormat	= FAAC_INPUT_32BIT;
	if (format->bits == 32)	fConfig->inputFormat	= FAAC_INPUT_FLOAT;

	ex_faacEncSetConfiguration(handle, fConfig);

	mp4File		= ex_MP4CreateEx(GetTempFile(format->outfile), 0, 0, 1, 1, NIL, 0, NIL, 0);

	ex_MP4SetTimeScale(mp4File, 90000);

	mp4Track	= ex_MP4AddAudioTrack(mp4File, format->rate, MP4_INVALID_DURATION, MP4_MPEG4_AUDIO_TYPE);	

	ex_MP4SetAudioProfileLevel(mp4File, 0x0F);

	unsigned char	*buffer = NIL;

	ex_faacEncGetDecoderSpecificInfo(handle, &buffer, &bufferSize);

	ex_MP4SetTrackESConfiguration(mp4File, mp4Track, (u_int8_t *) buffer, bufferSize);

	frameSize	= samplesSize / format->channels;

	totalSamples	= 0;
	encodedSamples	= 0;
	delaySamples	= frameSize;

	packageSize	= samplesSize * (format->bits / 8);

	return true;
}

bool BonkEnc::FilterOutMP4::Deactivate()
{
	unsigned long	 bytes = ex_faacEncEncode(handle, NULL, 0, outBuffer, outBuffer.Size());

	if (bytes > 0)
	{
		Int		 samplesLeft	= totalSamples - encodedSamples + delaySamples;
		MP4Duration	 dur		= samplesLeft > frameSize ? frameSize : samplesLeft;
		MP4Duration	 ofs		= encodedSamples > 0 ? 0 : delaySamples;

		ex_MP4WriteSample(mp4File, mp4Track, bytes > 0 ? (u_int8_t *) (unsigned char *) outBuffer : NIL, bytes, dur, ofs, true);
	}

	ex_faacEncClose(handle);

	if (currentConfig->enable_mp4meta)
	{
		char	*prevOutFormat = String::SetOutputFormat(currentConfig->mp4meta_encoding);

		if (currentConfig->default_comment != NIL) ex_MP4SetMetadataComment(mp4File, currentConfig->default_comment);

		if (format->artist != NIL || format->title != NIL)
		{
			if (format->title != NIL)	ex_MP4SetMetadataName(mp4File, format->title);
			if (format->artist != NIL)	ex_MP4SetMetadataArtist(mp4File, format->artist);
			if (format->year > 0)		ex_MP4SetMetadataYear(mp4File, String::FromInt(format->year));
			if (format->album != NIL)	ex_MP4SetMetadataAlbum(mp4File, format->album);
			if (format->genre != NIL)	ex_MP4SetMetadataGenre(mp4File, format->genre);
			if (format->track > 0)		ex_MP4SetMetadataTrack(mp4File, format->track, 0);
		}

		String::SetOutputFormat(prevOutFormat);
	}

	ex_MP4Close(mp4File);

	ex_MP4Optimize(GetTempFile(format->outfile), NIL, 0);

	if (GetTempFile(format->outfile) != format->outfile)
	{
		File	 tempFile(GetTempFile(format->outfile));

		tempFile.Move(format->outfile);
	}

	return true;
}

int BonkEnc::FilterOutMP4::WriteData(unsigned char *data, int size)
{
	unsigned long	 bytes = 0;
	Int		 samplesRead = size / (format->bits / 8);

	totalSamples += samplesRead / format->channels;

	if (format->bits != 16)
	{
		for (int i = 0; i < samplesRead; i++)
		{
			if (format->bits == 8)	((short *) (int32_t *) samplesBuffer)[i] = (data[i] - 128) * 256;
			if (format->bits == 24) samplesBuffer[i] = data[3 * i] + 256 * data[3 * i + 1] + 65536 * data[3 * i + 2] - (data[3 * i + 2] & 128 ? 16777216 : 0);
			if (format->bits == 32)	((float *) (int32_t *) samplesBuffer)[i] = (1.0 / 65536) * ((int32_t *) data)[i];
		}

		bytes = ex_faacEncEncode(handle, samplesBuffer, samplesRead, outBuffer, outBuffer.Size());
	}
	else
	{
		bytes = ex_faacEncEncode(handle, (int32_t *) data, samplesRead, outBuffer, outBuffer.Size());
	}

	if (bytes > 0)
	{
		Int		 samplesLeft	= totalSamples - encodedSamples + delaySamples;
		MP4Duration	 dur		= samplesLeft > frameSize ? frameSize : samplesLeft;
		MP4Duration	 ofs		= encodedSamples > 0 ? 0 : delaySamples;

		ex_MP4WriteSample(mp4File, mp4Track, (u_int8_t *) (unsigned char *) outBuffer, bytes, dur, ofs, true);

		encodedSamples += dur;
	}

	return bytes;
}

String BonkEnc::FilterOutMP4::GetTempFile(const String &oFileName)
{
	String	 rVal	= oFileName;
	Int	 lastBs	= -1;

	for (Int i = 0; i < rVal.Length(); i++)
	{
		if (rVal[i] > 255)	rVal[i] = '#';
		if (rVal[i] == '\\')	lastBs = i;
	}

	if (rVal == oFileName) return rVal;

	char	*tempa = new char [MAX_PATH];

	GetTempPathA(MAX_PATH, tempa);

	String	 tempDir = tempa;

	delete [] tempa;

	if (tempDir[tempDir.Length() - 1] != '\\') tempDir.Append("\\");

	for (Int j = lastBs + 1; j < rVal.Length(); j++)
	{
		tempDir[tempDir.Length()] = rVal[j];
	}

	return tempDir.Append(".out.temp");
}
