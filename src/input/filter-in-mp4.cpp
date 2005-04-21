 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-mp4.h>

FilterInMP4::FilterInMP4(bonkEncConfig *config, bonkEncTrack *format) : InputFilter(config, format)
{
	packageSize = 0;
}

FilterInMP4::~FilterInMP4()
{
}

bool FilterInMP4::Activate()
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
		fConfig->defObjectType	= LOW;
		fConfig->outputFormat	= FAAD_FMT_16BIT;

		ex_NeAACDecSetConfiguration(handle, fConfig);

		char		*buffer		= NIL;
		unsigned long	 buffer_size	= 0;

		ex_MP4GetTrackESConfiguration(mp4File, mp4Track, (unsigned char **) &buffer, &buffer_size);

		unsigned long	 rate;
		unsigned char	 channels;

		ex_NeAACDecInit2(handle, (unsigned char *) buffer, buffer_size,
 &rate, &channels);

		sampleId = 0;

		free(buffer);
	}

	return true;
}

bool FilterInMP4::Deactivate()
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

		NeAACDecFrameInfo frameInfo;

		samples = ex_NeAACDecDecode(handle, &frameInfo, buffer, buffer_size);

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
	if (GetTempFile(inFile) != inFile)
	{
		File	 mp4File(inFile);

		mp4File.Copy(GetTempFile(inFile));
	}

	bonkEncTrack	*nFormat = new bonkEncTrack;
	InStream	*f_in = OpenFile(GetTempFile(inFile));

	nFormat->fileSize	= f_in->Size();
	nFormat->length		= -1;

	CloseFile(f_in);

	mp4File = ex_MP4Read(GetTempFile(inFile), 0);

	char		*buffer		= NIL;
	unsigned long	 buffer_size	= 0;
	unsigned short	 trackNr	= 0;
	unsigned short	 nOfTracks	= 0;

	String	 prevInFormat = String::SetInputFormat("UTF-8");

	if (ex_MP4GetMetadataName(mp4File, &buffer)) { nFormat->title = buffer; free(buffer); }
	if (ex_MP4GetMetadataArtist(mp4File, &buffer)) { nFormat->artist = buffer; free(buffer); }
	if (ex_MP4GetMetadataComment(mp4File, &buffer)) { nFormat->comment = buffer; free(buffer); }
	if (ex_MP4GetMetadataYear(mp4File, &buffer)) { nFormat->year = String(buffer).ToInt(); free(buffer); }
	if (ex_MP4GetMetadataAlbum(mp4File, &buffer)) { nFormat->album = buffer; free(buffer); }
	if (ex_MP4GetMetadataGenre(mp4File, &buffer)) { nFormat->genre = buffer; free(buffer); }
	if (ex_MP4GetMetadataTrack(mp4File, (u_int16_t *) &trackNr, (u_int16_t *) &nOfTracks)) { nFormat->track = trackNr; }

	String::SetInputFormat(prevInFormat);

	mp4Track = GetAudioTrack();

	if (mp4Track >= 0)
	{
		handle	= ex_NeAACDecOpen();
		fConfig	= ex_NeAACDecGetCurrentConfiguration(handle);

		fConfig->defSampleRate	= 44100;
		fConfig->defObjectType	= LOW;
		fConfig->outputFormat	= FAAD_FMT_16BIT;

		ex_NeAACDecSetConfiguration(handle, fConfig);

		buffer		= NIL;
		buffer_size	= 0;

		ex_MP4GetTrackESConfiguration(mp4File, mp4Track, (unsigned char **) &buffer, &buffer_size);

		ex_NeAACDecInit2(handle, (unsigned char *) buffer, buffer_size,
 (unsigned long *) &nFormat->rate, (unsigned char *) &nFormat->channels);

		nFormat->length		= ex_MP4GetTrackNumberOfSamples(mp4File, mp4Track) * 2048;
		nFormat->order		= BYTE_INTEL;
		nFormat->bits		= 16;

		free(buffer);

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

String FilterInMP4::GetTempFile(const String &oFileName)
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

	return tempDir.Append(".in.temp");
}
