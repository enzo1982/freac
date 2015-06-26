 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <engine/worker_singlefile.h>

#include <config.h>

using namespace BoCA;
using namespace BoCA::AS;

BonkEnc::ConvertWorkerSingleFile::ConvertWorkerSingleFile(Encoder *iEncoder)
{
	encoder	       = iEncoder;
	encodedSamples = 0;
}

BonkEnc::ConvertWorkerSingleFile::~ConvertWorkerSingleFile()
{
}

Int BonkEnc::ConvertWorkerSingleFile::Convert()
{
	/* Setup conversion log.
	 */
	BoCA::Protocol	*log = BoCA::Protocol::Get("Converter log");

	/* Create decoder.
	 */
	Decoder	*decoder = new Decoder();

	if (!decoder->Create(trackToConvert.origFilename, trackToConvert))
	{
		delete decoder;

		return Error();
	}

	decoderName = decoder->GetDecoderName();

	/* Run main conversion loop.
	 */
	log->Write(String("\tEncoding from: ").Append(trackToConvert.origFilename));

	Int64	 trackLength = Loop(decoder, encoder);

	if (cancel) log->WriteWarning(String("\tCancelled encoding: ").Append(trackToConvert.origFilename));
	else	    log->Write(String("\tFinished encoding:" ).Append(trackToConvert.origFilename));

	/* Free decoder.
	 */
	delete decoder;

	/* Signal next chapter.
	 */
	if (!cancel)
	{
		encoder->SignalChapterChange();
		encodedSamples += trackLength;
	}

	/* Delete input file if requested.
	 */
	if (Config::Get()->deleteAfterEncoding && !cancel) File(trackToConvert.origFilename).Delete();

	/* Report finished conversion.
	 */
	onFinishTrack.Emit(trackToConvert);

	/* Update track length and offset.
	 */
	Format	 format = trackToConvert.GetFormat();

	trackToConvert.sampleOffset = Math::Round((Float) (encodedSamples - trackLength) / format.rate * 75);
	trackToConvert.length	    = trackLength;

	return Success();
}
