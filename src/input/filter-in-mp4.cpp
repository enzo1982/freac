 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-mp4.h>

#include <dllinterfaces.h>
#include <utilities.h>

BonkEnc::FilterInMP4::FilterInMP4(Config *config, Track *format) : InputFilter(config, format)
{
	packageSize = 0;
}

BonkEnc::FilterInMP4::~FilterInMP4()
{
}

Bool BonkEnc::FilterInMP4::Activate()
{
	if (GetTempFile(format->origFilename) != format->origFilename)
	{
		File	 mp4File(format->origFilename);

		mp4File.Copy(GetTempFile(format->origFilename));
	}

	mp4File		= ex_MP4Read(GetTempFile(format->origFilename), 0);
	mp4Track	= GetAudioTrack();

	if (mp4Track >= 0)
	{
		handle	= ex_NeAACDecOpen();
		fConfig	= ex_NeAACDecGetCurrentConfiguration(handle);

		fConfig->defSampleRate	= 44100;
		fConfig->defObjectType	= LC;
		fConfig->outputFormat	= FAAD_FMT_16BIT;

		ex_NeAACDecSetConfiguration(handle, fConfig);

		unsigned char	*buffer		= NIL;
		unsigned long	 buffer_size	= 0;

		ex_MP4GetTrackESConfiguration(mp4File, mp4Track, (u_int8_t **) &buffer, (u_int32_t *) &buffer_size);

		unsigned long	 rate;
		unsigned char	 channels;

		ex_NeAACDecInit2(handle, (unsigned char *) buffer, buffer_size, &rate, &channels);

		sampleId = 0;

		ex_MP4Free(buffer);
	}

	return true;
}

Bool BonkEnc::FilterInMP4::Deactivate()
{
	if (mp4Track >= 0) ex_NeAACDecClose(handle);

	ex_MP4Close(mp4File);

	if (GetTempFile(format->origFilename) != format->origFilename)
	{
		File	 tempFile(GetTempFile(format->origFilename));

		tempFile.Delete();
	}

	return true;
}

Int BonkEnc::FilterInMP4::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	if (size <= 0) return -1;

	inBytes += size;

	Void	*samples = NIL;
	Int	 samplesRead = 0;

	samplesBuffer.Resize(0);

	do
	{
		unsigned char	*buffer		= NIL;
		unsigned long	 buffer_size	= 0;

		ex_MP4ReadSample(mp4File, mp4Track, sampleId++, (u_int8_t **) &buffer, (u_int32_t *) &buffer_size, NIL, NIL, NIL, NIL);

		NeAACDecFrameInfo frameInfo;

		samples = ex_NeAACDecDecode(handle, &frameInfo, buffer, buffer_size);

		ex_MP4Free(buffer);

	        if ((frameInfo.error == 0) && (frameInfo.samples > 0) && (samples != NIL))
		{
			samplesBuffer.Resize((samplesRead + frameInfo.samples) * 2);

			memcpy(samplesBuffer + samplesRead * 2, samples, frameInfo.samples * 2);

			samplesRead += frameInfo.samples;
		}
	}
	while (samples != NIL && sampleId < ((format->length / 2048) * (double(inBytes) / format->fileSize)));

        if (samplesRead > 0)
	{
		data.Resize(samplesRead * 2);

		memcpy((unsigned char *) data, samplesBuffer, samplesRead * 2);
	}

	return samplesRead * 2;
}

BonkEnc::Track *BonkEnc::FilterInMP4::GetFileInfo(const String &inFile)
{
	if (GetTempFile(inFile) != inFile)
	{
		File	 mp4File(inFile);

		mp4File.Copy(GetTempFile(inFile));
	}

	Track		*nFormat = new Track;
	InStream	*f_in = new InStream(STREAM_FILE, GetTempFile(inFile), IS_READONLY);

	nFormat->fileSize	= f_in->Size();
	nFormat->length		= -1;

	delete f_in;

	mp4File = ex_MP4Read(GetTempFile(inFile), 0);

	char		*buffer		= NIL;
	unsigned short	 trackNr	= 0;
	unsigned short	 nOfTracks	= 0;

	char	*prevInFormat = String::SetInputFormat("UTF-8");

	if (ex_MP4GetMetadataName(mp4File, &buffer)) { nFormat->title = buffer; ex_MP4Free(buffer); }
	if (ex_MP4GetMetadataArtist(mp4File, &buffer)) { nFormat->artist = buffer; ex_MP4Free(buffer); }
	if (ex_MP4GetMetadataComment(mp4File, &buffer)) { nFormat->comment = buffer; ex_MP4Free(buffer); }
	if (ex_MP4GetMetadataYear(mp4File, &buffer)) { nFormat->year = String(buffer).ToInt(); ex_MP4Free(buffer); }
	if (ex_MP4GetMetadataAlbum(mp4File, &buffer)) { nFormat->album = buffer; ex_MP4Free(buffer); }
	if (ex_MP4GetMetadataGenre(mp4File, &buffer)) { nFormat->genre = buffer; ex_MP4Free(buffer); }
	if (ex_MP4GetMetadataTrack(mp4File, (u_int16_t *) &trackNr, (u_int16_t *) &nOfTracks)) { nFormat->track = trackNr; }

	String::SetInputFormat(prevInFormat);

	mp4Track = GetAudioTrack();

	if (mp4Track >= 0)
	{
		handle	= ex_NeAACDecOpen();
		fConfig	= ex_NeAACDecGetCurrentConfiguration(handle);

		fConfig->defSampleRate	= 44100;
		fConfig->defObjectType	= LC;
		fConfig->outputFormat	= FAAD_FMT_16BIT;

		ex_NeAACDecSetConfiguration(handle, fConfig);

		unsigned char	*esc_buffer	= NIL;
		unsigned long	 buffer_size	= 0;

		ex_MP4GetTrackESConfiguration(mp4File, mp4Track, (u_int8_t **) &esc_buffer, (u_int32_t *) &buffer_size);

		ex_NeAACDecInit2(handle, (unsigned char *) esc_buffer, buffer_size,
 (unsigned long *) &nFormat->rate, (unsigned char *) &nFormat->channels);

		nFormat->length		= Math::Round(ex_MP4GetTrackDuration(mp4File, mp4Track) * nFormat->channels * nFormat->rate / ex_MP4GetTrackTimeScale(mp4File, mp4Track));
		nFormat->order		= BYTE_INTEL;
		nFormat->bits		= 16;

		ex_MP4Free(esc_buffer);

		ex_NeAACDecClose(handle);
	}

	ex_MP4Close(mp4File);

	if (GetTempFile(inFile) != inFile)
	{
		File	 tempFile(GetTempFile(inFile));

		tempFile.Delete();
	}

	return nFormat;
}

Int BonkEnc::FilterInMP4::GetAudioTrack()
{
	Int nOfTracks = ex_MP4GetNumberOfTracks(mp4File, NIL, 0);

	for (Int i = 0; i < nOfTracks; i++)
	{
		MP4TrackId	 trackId	= ex_MP4FindTrackId(mp4File, i, NIL, 0);
		String		 trackType	= ex_MP4GetTrackType(mp4File, trackId);

		if (trackType == MP4_AUDIO_TRACK_TYPE) return trackId;
	}

	return -1;
} 

String BonkEnc::FilterInMP4::GetTempFile(const String &oFileName)
{
	String	 rVal	= oFileName;
	Int	 lastBs	= -1;

	for (Int i = 0; i < rVal.Length(); i++)
	{
		if (rVal[i] > 255)	rVal[i] = '#';
		if (rVal[i] == '\\')	lastBs = i;
	}

	if (rVal == oFileName) return rVal;

	String	 tempDir = Utilities::GetTempDirectory();

	for (Int j = lastBs + 1; j < rVal.Length(); j++)
	{
		tempDir[tempDir.Length()] = rVal[j];
	}

	return tempDir.Append(".in.temp");
}
