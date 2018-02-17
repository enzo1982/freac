 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
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

freac::FilterInMP4::FilterInMP4(Config *config, Track *format) : InputFilter(config, format)
{
	packageSize = 0;
}

freac::FilterInMP4::~FilterInMP4()
{
}

Bool freac::FilterInMP4::Activate()
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

		ex_MP4GetTrackESConfiguration(mp4File, mp4Track, (uint8_t **) &buffer, (uint32_t *) &buffer_size);

		unsigned long	 rate;
		unsigned char	 channels;

		ex_NeAACDecInit2(handle, (unsigned char *) buffer, buffer_size, &rate, &channels);

		sampleId = 1;

		ex_MP4Free(buffer);
	}

	return true;
}

Bool freac::FilterInMP4::Deactivate()
{
	if (mp4Track >= 0) ex_NeAACDecClose(handle);

	ex_MP4Close(mp4File, 0);

	if (GetTempFile(format->origFilename) != format->origFilename)
	{
		File	 tempFile(GetTempFile(format->origFilename));

		tempFile.Delete();
	}

	return true;
}

Int freac::FilterInMP4::ReadData(Buffer<UnsignedByte> &data, Int size)
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

		ex_MP4ReadSample(mp4File, mp4Track, sampleId++, (uint8_t **) &buffer, (uint32_t *) &buffer_size, NIL, NIL, NIL, NIL);

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
	while (samples != NIL && sampleId <= ((format->length / 2048) * (double(inBytes) / format->fileSize)));

        if (samplesRead > 0)
	{
		data.Resize(samplesRead * 2);

		memcpy((unsigned char *) data, samplesBuffer, samplesRead * 2);
	}

	return samplesRead * 2;
}

freac::Track *freac::FilterInMP4::GetFileInfo(const String &inFile)
{
	if (GetTempFile(inFile) != inFile)
	{
		File	 mp4File(inFile);

		mp4File.Copy(GetTempFile(inFile));
	}

	Track		*nFormat = new Track;
	InStream	*f_in = new InStream(STREAM_FILE, GetTempFile(inFile), IS_READ);

	nFormat->fileSize	= f_in->Size();
	nFormat->length		= -1;

	delete f_in;

	mp4File = ex_MP4Read(GetTempFile(inFile), 0);

	const MP4Tags	*mp4Tags = ex_MP4TagsAlloc();

	ex_MP4TagsFetch(mp4Tags, mp4File);

	String	 prevInFormat = String::SetInputFormat("UTF-8");

	if	(mp4Tags->name	      != NIL) nFormat->title    = mp4Tags->name;
	if	(mp4Tags->artist      != NIL) nFormat->artist   = mp4Tags->artist;
	if	(mp4Tags->releaseDate != NIL) nFormat->year     = String(mp4Tags->releaseDate).ToInt();
	if	(mp4Tags->album	      != NIL) nFormat->album    = mp4Tags->album;
	if	(mp4Tags->comments    != NIL) nFormat->comment  = mp4Tags->comments;

	if	(mp4Tags->genre	      != NIL) nFormat->genre    = mp4Tags->genre;
	else if (mp4Tags->genreType   != NIL) nFormat->genre    = Utilities::GetID3CategoryName(*mp4Tags->genreType - 1);

	if (mp4Tags->track != NIL)
	{
		nFormat->track	   = mp4Tags->track->index;
		nFormat->numTracks = mp4Tags->track->total;
	}

	if (mp4Tags->disk != NIL)
	{
		nFormat->disc	   = mp4Tags->disk->index;
		nFormat->numDiscs  = mp4Tags->disk->total;
	}

	for (UnsignedInt i = 0; i < mp4Tags->artworkCount; i++)
	{
		unsigned char	*buffer	= (unsigned char *) mp4Tags->artwork[i].data;
		uint32_t	 size	= mp4Tags->artwork[i].size;

		if (size > 0)
		{
			Picture	*picture = new Picture();

			if	(i == 0) picture->type = 3; // Cover (front)
			else if (i == 1) picture->type = 4; // Cover (back)
			else		 picture->type = 0; // Other

			picture->data.Resize(size);

			memcpy(picture->data, buffer, picture->data.Size());

			if (picture->data.Size() >= 16 && picture->data[0] != 0 && picture->data[1] != 0)
			{
				if	(picture->data[0] == 0xFF && picture->data[1] == 0xD8) picture->mime = "image/jpeg";
				else if (picture->data[0] == 0x89 && picture->data[1] == 0x50 &&
					 picture->data[2] == 0x4E && picture->data[3] == 0x47 &&
					 picture->data[4] == 0x0D && picture->data[5] == 0x0A &&
					 picture->data[6] == 0x1A && picture->data[7] == 0x0A) picture->mime = "image/png";

				nFormat->pictures.Add(picture);
			}
			else
			{
				delete picture;
			}
		}
	}

	ex_MP4TagsFree(mp4Tags);

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

		ex_MP4GetTrackESConfiguration(mp4File, mp4Track, (uint8_t **) &esc_buffer, (uint32_t *) &buffer_size);

		ex_NeAACDecInit2(handle, (unsigned char *) esc_buffer, buffer_size, (unsigned long *) &nFormat->rate, (unsigned char *) &nFormat->channels);

		nFormat->length		= Math::Round(ex_MP4GetTrackDuration(mp4File, mp4Track) * nFormat->channels * nFormat->rate / ex_MP4GetTrackTimeScale(mp4File, mp4Track));
		nFormat->order		= BYTE_INTEL;
		nFormat->bits		= 16;

		ex_MP4Free(esc_buffer);

		ex_NeAACDecClose(handle);
	}

	ex_MP4Close(mp4File, 0);

	if (GetTempFile(inFile) != inFile)
	{
		File	 tempFile(GetTempFile(inFile));

		tempFile.Delete();
	}

	return nFormat;
}

Int freac::FilterInMP4::GetAudioTrack()
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

String freac::FilterInMP4::GetTempFile(const String &oFileName)
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

	return tempDir.Append(".in.temp");
}
