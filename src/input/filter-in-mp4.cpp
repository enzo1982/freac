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

FilterInMP4::FilterInMP4(bonkEncConfig *config) : InputFilter(config)
{
	packageSize = 0;
}

FilterInMP4::~FilterInMP4()
{
}

bool FilterInMP4::Activate()
{
	return true;
}

bool FilterInMP4::Deactivate()
{
	return true;
}

int FilterInMP4::ReadData(unsigned char **data, int size)
{
	if (size <= 0) return -1;

	return -1;
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
	unsigned short	 trackNr	= 0;
	unsigned short	 nOfTracks	= 0;

	if (ex_MP4GetMetadataName(mp4File, &buffer)) { nFormat->title = buffer; free(buffer); }
	if (ex_MP4GetMetadataArtist(mp4File, &buffer)) { nFormat->artist = buffer; free(buffer); }
	if (ex_MP4GetMetadataComment(mp4File, &buffer)) { nFormat->comment = buffer; free(buffer); }
	if (ex_MP4GetMetadataYear(mp4File, &buffer)) { nFormat->year = String(buffer).ToInt(); free(buffer); }
	if (ex_MP4GetMetadataAlbum(mp4File, &buffer)) { nFormat->album = buffer; free(buffer); }
	if (ex_MP4GetMetadataGenre(mp4File, &buffer)) { nFormat->genre = buffer; free(buffer); }
	if (ex_MP4GetMetadataTrack(mp4File, &trackNr, &nOfTracks)) { nFormat->track = trackNr; }

	Int	 trackId = GetAudioTrack();

	if (trackId >= 0)
	{
		nFormat->rate		= 44100;
		nFormat->channels	= 2;
		nFormat->length		= ex_MP4GetTrackNumberOfSamples(mp4File, trackId) * 2048;
		nFormat->order		= BYTE_INTEL;
		nFormat->bits		= 16;
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
