 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <engine/encoder.h>
#include <utilities.h>

using namespace smooth::IO;

using namespace BoCA::AS;

BonkEnc::Encoder::Encoder()
{
	f_out	   = NIL;
	filter_out = NIL;
}

BonkEnc::Encoder::~Encoder()
{
	Destroy();
}

Bool BonkEnc::Encoder::Create(const String &encoderID, const String &fileName, const Track &track)
{
	Registry	&boca = Registry::Get();

	f_out = new OutStream(STREAM_FILE, fileName, OS_REPLACE);
	f_out->SetPackageSize(196608);

	if (f_out->GetLastError() != IO_ERROR_OK)
	{
		BoCA::Utilities::ErrorMessage("Cannot create output file: %1", File(fileName).GetFileName());

		delete f_out;
		f_out = NIL;

		return False;
	}

	filter_out = (EncoderComponent *) boca.CreateComponentByID(encoderID);
	filter_out->SetAudioTrackInfo(track);

	if (f_out->AddFilter(filter_out) == False)
	{
		BoCA::Utilities::ErrorMessage("Cannot set up encoder for output file:%1\n\nError: %2", File(fileName).GetFileName(), filter_out->GetErrorString());

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

	f_out->RemoveFilter(filter_out);

	if (filter_out->GetErrorState()) BoCA::Utilities::ErrorMessage("Error: %1", filter_out->GetErrorString());

	boca.DeleteComponent(filter_out);

	delete f_out;

	filter_out = NIL;
	f_out	   = NIL;

	return True;
}

Int BonkEnc::Encoder::Write(Buffer<UnsignedByte> &buffer, Int bytes)
{
	return f_out->OutputData(buffer, bytes);
}
