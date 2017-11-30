 /* fre:ac - free audio converter
  * Copyright (C) 2001-2017 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <output/filter-out-mp4.h>
#include <utilities.h>
#include <dllinterfaces.h>

freac::FilterOutMP4::FilterOutMP4(Config *config, Track *format) : OutputFilter(config, format)
{
	if (format->channels > 2)
	{
		Utilities::ErrorMessage(String(freac::appName).Append(" does not support more than 2 channels!"));

		errorState = True;

		return;
	}
}

freac::FilterOutMP4::~FilterOutMP4()
{
}

Bool freac::FilterOutMP4::Activate()
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
	fConfig->aacObjectType	= LOW;
	fConfig->jointmode	= currentConfig->faac_allowjs;
	fConfig->useTns		= currentConfig->faac_usetns;
	fConfig->quantqual	= currentConfig->faac_set_quality ? currentConfig->faac_aac_quality : 0;
	fConfig->bitRate	= currentConfig->faac_set_quality ? 0 : currentConfig->faac_bitrate * 1000;
	fConfig->bandWidth	= currentConfig->faac_bandwidth;
	fConfig->outputFormat	= RAW_STREAM;

	if (format->bits == 8)	fConfig->inputFormat	= FAAC_INPUT_16BIT;
	if (format->bits == 16)	fConfig->inputFormat	= FAAC_INPUT_16BIT;
	if (format->bits == 24)	fConfig->inputFormat	= FAAC_INPUT_32BIT;
	if (format->bits == 32)	fConfig->inputFormat	= FAAC_INPUT_FLOAT;

	ex_faacEncSetConfiguration(handle, fConfig);

	/* Create file and track.
	 */
	mp4File		= ex_MP4CreateEx(GetTempFile(format->outfile), 0, 0, 1, 1, NIL, 0, NIL, 0);
	mp4Track	= ex_MP4AddAudioTrack(mp4File, format->rate, MP4_INVALID_DURATION, MP4_MPEG4_AUDIO_TYPE);	

	ex_MP4SetAudioProfileLevel(mp4File, 0x0F);

	unsigned char	*buffer = NIL;

	ex_faacEncGetDecoderSpecificInfo(handle, &buffer, &bufferSize);

	ex_MP4SetTrackESConfiguration(mp4File, mp4Track, (uint8_t *) buffer, bufferSize);

	frameSize	= samplesSize / format->channels;

	totalSamples	= 0;
	encodedSamples	= 0;
	delaySamples	= frameSize;

	packageSize	= samplesSize * (format->bits / 8);

	return true;
}

Bool freac::FilterOutMP4::Deactivate()
{
	unsigned long	 bytes = 0;

	do
	{
		bytes = ex_faacEncEncode(handle, NIL, 0, outBuffer, outBuffer.Size());

		if (bytes > 0)
		{
			Int		 samplesLeft	= totalSamples - encodedSamples + delaySamples;
			MP4Duration	 dur		= samplesLeft > frameSize ? frameSize : samplesLeft;
			MP4Duration	 ofs		= encodedSamples > 0 ? 0 : delaySamples;

			ex_MP4WriteSample(mp4File, mp4Track, (uint8_t *) (unsigned char *) outBuffer, bytes, dur, ofs, true);

			encodedSamples += dur;
		}
	}
	while (bytes > 0);

	ex_faacEncClose(handle);

	if (currentConfig->enable_mp4meta)
	{
		String	 prevOutFormat = String::SetOutputFormat(currentConfig->mp4meta_encoding);

		if (format->artist != NIL || format->title != NIL)
		{
			if (format->title != NIL)	ex_MP4SetMetadataName(mp4File, format->title);
			if (format->artist != NIL)	ex_MP4SetMetadataArtist(mp4File, format->artist);
			if (format->year > 0)		ex_MP4SetMetadataYear(mp4File, String::FromInt(format->year));
			if (format->album != NIL)	ex_MP4SetMetadataAlbum(mp4File, format->album);
			if (format->genre != NIL)	ex_MP4SetMetadataGenre(mp4File, format->genre);
			if (format->track > 0)		ex_MP4SetMetadataTrack(mp4File, format->track, format->numTracks > 0 ? format->numTracks : 0);
			if (format->disc > 0)		ex_MP4SetMetadataDisk(mp4File, format->disc, format->numDiscs > 0 ? format->numDiscs : 0);

			if (format->comment != NIL && !currentConfig->overwriteComments) ex_MP4SetMetadataComment(mp4File, format->comment);
			else if (currentConfig->default_comment != NIL)			 ex_MP4SetMetadataComment(mp4File, currentConfig->default_comment);
		}

		/* Save cover art.
		 */
		if (currentConfig->copy_picture_tags)
		{
			/* Put front and back covers first.
			 */
			Array<const Picture *>	 pictures;

			foreach (const Picture *picInfo, format->pictures)
			{
				if	(picInfo->type == 3) pictures.InsertAtPos(0, picInfo);
				else if	(picInfo->type == 4) pictures.InsertAtPos((pictures.Length() > 0 && pictures.GetFirst()->type == 3) ? 1 : 0, picInfo);
				else			     pictures.Add(picInfo);
			}

			/* Add cover art to tag.
			 */
			foreach (const Picture *picInfo, pictures)
			{
				ex_MP4SetMetadataCoverArt(mp4File, (BYTE *) (UnsignedByte *) picInfo->data, (uint32_t) picInfo->data.Size());
			}
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

Int freac::FilterOutMP4::WriteData(Buffer<UnsignedByte> &data, Int size)
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
			if (format->bits == 32)	((float *) (int32_t *) samplesBuffer)[i] = (1.0 / 65536) * ((int32_t *) (unsigned char *) data)[i];
		}

		bytes = ex_faacEncEncode(handle, samplesBuffer, samplesRead, outBuffer, outBuffer.Size());
	}
	else
	{
		bytes = ex_faacEncEncode(handle, (int32_t *) (unsigned char *) data, samplesRead, outBuffer, outBuffer.Size());
	}

	if (bytes > 0)
	{
		Int		 samplesLeft	= totalSamples - encodedSamples + delaySamples;
		MP4Duration	 dur		= samplesLeft > frameSize ? frameSize : samplesLeft;
		MP4Duration	 ofs		= encodedSamples > 0 ? 0 : delaySamples;

		ex_MP4WriteSample(mp4File, mp4Track, (uint8_t *) (unsigned char *) outBuffer, bytes, dur, ofs, true);

		encodedSamples += dur;
	}

	return bytes;
}

String freac::FilterOutMP4::GetTempFile(const String &oFileName)
{
	String	 rVal	= oFileName;
	Int	 lastBs	= -1;

	for (Int i = 0; i < rVal.Length(); i++)
	{
		if (rVal[i] > 127)			rVal[i] = '#';
		if (rVal[i] == '\\' || rVal[i] == '/')	lastBs = i;
	}

	if (rVal == oFileName) return rVal;

	String	 tempDir = S::System::System::GetTempDirectory();

	for (Int j = lastBs + 1; j < rVal.Length(); j++)
	{
		tempDir[tempDir.Length()] = rVal[j];
	}

	return tempDir.Append(".out.temp");
}
