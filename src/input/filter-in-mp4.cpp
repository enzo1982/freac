 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <iolib-cxx.h>
#include <input/filter-in-mp4.h>
#include <dllinterfaces.h>

FilterInMP4::FilterInMP4(bonkEncConfig *config, bonkEncTrack *format) : InputFilter(config, format)
{
	packageSize = 0;
}

FilterInMP4::~FilterInMP4()
{
}

bool FilterInMP4::Activate()
{
	mp4File		= ex_MP4Read(format->origFilename, 0);
	mp4Track	= GetAudioTrack();

	if (mp4Track >= 0)
	{
		handle	= ex_faacDecOpen();
		fConfig	= ex_faacDecGetCurrentConfiguration(handle);

		fConfig->defSampleRate	= 44100;
		fConfig->defObjectType	= LOW;
		fConfig->outputFormat	= FAAD_FMT_16BIT;

		ex_faacDecSetConfiguration(handle, fConfig);

		char		*buffer		= NIL;
		unsigned long	 buffer_size	= 0;

		ex_MP4GetTrackESConfiguration(mp4File, mp4Track, (unsigned char **) &buffer, &buffer_size);

		unsigned long	 rate;
		unsigned char	 channels;

		ex_faacDecInit2(handle, (unsigned char *) buffer, buffer_size,
 &rate, &channels);

		sampleId = 0;

		free(buffer);
	}

	return true;
}

bool FilterInMP4::Deactivate()
{
	if (mp4Track >= 0) ex_faacDecClose(handle);

	ex_MP4Close(mp4File);

	return true;
}

int FilterInMP4::ReadData(unsigned char **data, int size)
{
	if (size <= 0) return -1;

	inBytes += size;

	Void		*samples = NIL;
	Int		 samplesRead = 0;
	unsigned char	*samplesBuffer = new unsigned char [0];

	do
	{
		unsigned char	*buffer		= NIL;
		unsigned long	 buffer_size	= 0;

		ex_MP4ReadSample(mp4File, mp4Track, sampleId++, &buffer, &buffer_size, NIL, NIL, NIL, NIL);

		faacDecFrameInfo frameInfo;

		samples = ex_faacDecDecode(handle, &frameInfo, buffer, buffer_size);

		free(buffer);

	        if ((frameInfo.error == 0) && (frameInfo.samples > 0))
		{
			unsigned char	*backBuffer = samplesBuffer;

			samplesBuffer = new unsigned char [(samplesRead + frameInfo.samples) * 2];

			memcpy((Void *) samplesBuffer, (Void *) backBuffer, samplesRead * 2);
			memcpy((Void *) (samplesBuffer + samplesRead * 2), samples, frameInfo.samples * 2);

			delete [] backBuffer;

			samplesRead += frameInfo.samples;
		}
	}
	while (samples != NIL && sampleId < ((format->length / 2048) * (double(inBytes) / format->fileSize)));

	delete [] *data;

        if (samplesRead > 0)
	{
		*data = new unsigned char [samplesRead * 2];

		memcpy((Void *) *data, (Void *) samplesBuffer, samplesRead * 2);
	}
	else
	{
		*data = new unsigned char [0];
	}

	delete [] samplesBuffer;

	return samplesRead * 2;
}

bonkEncTrack *FilterInMP4::GetFileInfo(String inFile)
{
	bonkEncTrack	*nFormat = new bonkEncTrack;
	InStream	*f_in = OpenFile(inFile);

	nFormat->fileSize	= f_in->Size();
	nFormat->length		= -1;

	CloseFile(f_in);

	mp4File = ex_MP4Read(inFile, 0);

	char		*buffer		= NIL;
	unsigned long	 buffer_size	= 0;
	unsigned short	 trackNr	= 0;
	unsigned short	 nOfTracks	= 0;

	if (ex_MP4GetMetadataName(mp4File, &buffer)) { nFormat->title = buffer; free(buffer); }
	if (ex_MP4GetMetadataArtist(mp4File, &buffer)) { nFormat->artist = buffer; free(buffer); }
	if (ex_MP4GetMetadataComment(mp4File, &buffer)) { nFormat->comment = buffer; free(buffer); }
	if (ex_MP4GetMetadataYear(mp4File, &buffer)) { nFormat->year = String(buffer).ToInt(); free(buffer); }
	if (ex_MP4GetMetadataAlbum(mp4File, &buffer)) { nFormat->album = buffer; free(buffer); }
	if (ex_MP4GetMetadataGenre(mp4File, &buffer)) { nFormat->genre = buffer; free(buffer); }
	if (ex_MP4GetMetadataTrack(mp4File, &trackNr, &nOfTracks)) { nFormat->track = trackNr; }

	mp4Track = GetAudioTrack();

	if (mp4Track >= 0)
	{
		handle	= ex_faacDecOpen();
		fConfig	= ex_faacDecGetCurrentConfiguration(handle);

		fConfig->defSampleRate	= 44100;
		fConfig->defObjectType	= LOW;
		fConfig->outputFormat	= FAAD_FMT_16BIT;

		ex_faacDecSetConfiguration(handle, fConfig);

		buffer		= NIL;
		buffer_size	= 0;

		ex_MP4GetTrackESConfiguration(mp4File, mp4Track, (unsigned char **) &buffer, &buffer_size);

		ex_faacDecInit2(handle, (unsigned char *) buffer, buffer_size,
 (unsigned long *) &nFormat->rate, (unsigned char *) &nFormat->channels);

		nFormat->length		= ex_MP4GetTrackNumberOfSamples(mp4File, mp4Track) * 2048;
		nFormat->order		= BYTE_INTEL;
		nFormat->bits		= 16;

		free(buffer);

		ex_faacDecClose(handle);
	}

	ex_MP4Close(mp4File);

	return nFormat;
}

Int FilterInMP4::GetAudioTrack()
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
