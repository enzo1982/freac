 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <engine/encoder.h>

using namespace smooth::IO;

using namespace BoCA;
using namespace BoCA::AS;

BonkEnc::Encoder::Encoder()
{
	f_out	   = NIL;
	filter_out = NIL;

	chapter	   = 0;
	bytes	   = 0;
	offset	   = 0;
}

BonkEnc::Encoder::~Encoder()
{
	Destroy();
}

Bool BonkEnc::Encoder::Create(const String &encoderID, const String &fileName, const Track &track)
{
	Registry	&boca = Registry::Get();

	album	= track;
	chapter = 0;
	bytes	= 0;
	offset	= 0;

	f_out = new OutStream(STREAM_FILE, BoCA::Utilities::CreateDirectoryForFile(fileName), OS_REPLACE);
	f_out->SetPackageSize(196608);

	if (f_out->GetLastError() != IO_ERROR_OK)
	{
		BoCA::Utilities::ErrorMessage("Cannot create output file: %1", File(fileName).GetFileName());

		delete f_out;
		f_out = NIL;

		return False;
	}

	filter_out = (EncoderComponent *) boca.CreateComponentByID(encoderID);

	if (filter_out == NIL)
	{
		BoCA::Utilities::ErrorMessage("Cannot create encoder component: %1", encoderID);

		delete f_out;
		f_out = NIL;

		return False;
	}

	filter_out->SetAudioTrackInfo(album);

	if (f_out->AddFilter(filter_out) == False)
	{
		BoCA::Utilities::ErrorMessage("Cannot set up encoder for output file: %1\n\nError: %2", File(fileName).GetFileName(), filter_out->GetErrorString());

		boca.DeleteComponent(filter_out);

		filter_out = NIL;

		delete f_out;
		f_out = NIL;

		return False;
	}

	return True;
}

Bool BonkEnc::Encoder::Destroy()
{
	if (filter_out == NIL || f_out == NIL) return False;

	Registry	&boca = Registry::Get();

	filter_out->SetAudioTrackInfo(album);

	f_out->RemoveFilter(filter_out);

	if (filter_out->GetErrorState()) BoCA::Utilities::ErrorMessage("Error: %1", filter_out->GetErrorString());

	boca.DeleteComponent(filter_out);

	delete f_out;

	filter_out = NIL;
	f_out	   = NIL;

	return True;
}

Int BonkEnc::Encoder::Write(Buffer<UnsignedByte> &buffer, Int nbytes)
{
	bytes += nbytes;

	if (f_out->OutputData(buffer, nbytes) == False || filter_out->GetErrorState()) return -1;

	return bytes;
}

Void BonkEnc::Encoder::SignalChapterChange()
{
	if (album.tracks.Length() <= chapter) return;

	/* This adjusts chapter lengths in order to be able to
	 * set accurate chapter marks even in case we have tracks
	 * with only approxLength set in the first place.
	 */
	Track		&track	= album.tracks.GetNthReference(chapter);
	const Format	&format = album.GetFormat();

	track.length = bytes / format.channels / (format.bits / 8) - offset;

	offset += track.length;

	chapter++;
}
