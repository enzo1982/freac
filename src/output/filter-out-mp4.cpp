 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
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
	mp4File		= ex_MP4CreateEx(format->outfile.ConvertTo("UTF-8"), 0, 1, 1, NIL, 0, NIL, 0);
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
		const MP4Tags	*mp4Tags = ex_MP4TagsAlloc();

		ex_MP4TagsFetch(mp4Tags, mp4File);

		String	 prevOutFormat = String::SetOutputFormat(currentConfig->mp4meta_encoding);

		if (format->artist != NIL || format->title != NIL)
		{
			if (format->artist != NIL) ex_MP4TagsSetArtist(mp4Tags, format->artist);
			if (format->title  != NIL) ex_MP4TagsSetName(mp4Tags, format->title);
			if (format->album  != NIL) ex_MP4TagsSetAlbum(mp4Tags, format->album);
			if (format->year    >   0) ex_MP4TagsSetReleaseDate(mp4Tags, String::FromInt(format->year));
			if (format->genre  != NIL) ex_MP4TagsSetGenre(mp4Tags, format->genre);

			if (format->track > 0)
			{
				MP4TagTrack	 mp4Track = { (uint16_t) format->track, (uint16_t) format->numTracks };

				ex_MP4TagsSetTrack(mp4Tags, &mp4Track);
			}

			if (format->disc > 0)
			{
				MP4TagDisk	 mp4Disk = { (uint16_t) format->disc, (uint16_t) (format->numDiscs > 0 ? format->numDiscs : format->disc) };

				ex_MP4TagsSetDisk(mp4Tags, &mp4Disk);
			}

			if	(format->comment != NIL && !currentConfig->overwriteComments) ex_MP4TagsSetComments(mp4Tags, format->comment);
			else if (currentConfig->default_comment != NIL)			      ex_MP4TagsSetComments(mp4Tags, currentConfig->default_comment);
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
				MP4TagArtwork	 artwork = { const_cast<UnsignedByte *>((const UnsignedByte *) picInfo->data), (uint32_t) picInfo->data.Size(), picInfo->mime == "image/png" ? MP4_ART_PNG : MP4_ART_JPEG };

				ex_MP4TagsAddArtwork(mp4Tags, &artwork);
			}
		}

		ex_MP4TagsStore(mp4Tags, mp4File);
		ex_MP4TagsFree(mp4Tags);

		String::SetOutputFormat(prevOutFormat);
	}

	ex_MP4Close(mp4File, 0);

	ex_MP4Optimize(format->outfile.ConvertTo("UTF-8"), NIL);

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
