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
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	Registry	&boca	= Registry::Get();

	/* Get config values.
	 */
	Bool	 verifyInput	= config->GetIntValue(Config::CategoryVerificationID, Config::VerificationVerifyInputID, Config::VerificationVerifyInputDefault);

	/* Setup conversion log.
	 */
	BoCA::Protocol	*log = BoCA::Protocol::Get("Converter log");

	/* Check format of output file in verification step.
	 */
	Format	 format = trackToConvert.GetFormat();

	if (conversionStep == ConversionStepVerify)
	{
		DecoderComponent	*decoder = boca.CreateDecoderForStream(trackToConvert.origFilename);

		if (decoder != NIL)
		{
			Track	 outTrack;

			decoder->GetStreamInfo(trackToConvert.origFilename, outTrack);

			boca.DeleteComponent(decoder);

			const Format	&outFormat = outTrack.GetFormat();

			if (outFormat.rate     != format.rate ||
			    outFormat.bits     != format.bits ||
			    outFormat.channels != format.channels)
			{
				onReportWarning.Emit(i18n->TranslateString(String("Skipped verification due to format mismatch: %1\n\n")
									  .Append("Original format: %2 Hz, %3 bit, %4 channels\n")
									  .Append("Output format: %5 Hz, %6 bit, %7 channels"), "Messages").Replace("%1", File(trackToConvert.origFilename).GetFileName()).Replace("%2", String::FromInt(format.rate)).Replace("%3", String::FromInt(format.bits)).Replace("%4", String::FromInt(format.channels))
																									  .Replace("%5", String::FromInt(outFormat.rate)).Replace("%6", String::FromInt(outFormat.bits)).Replace("%7", String::FromInt(outFormat.channels)));

				log->WriteWarning(String("\tSkipping verification due to format mismatch: ").Append(trackToConvert.origFilename));

				trackPosition = trackToConvert.length;

				onFinishTrack.Emit(trackToConvert);

				return Success();
			}
		}
	}

	/* Create decoder.
	 */
	Decoder	*decoder = new Decoder();

	if (!decoder->Create(trackToConvert.origFilename, trackToConvert))
	{
		delete decoder;

		return Error();
	}

	decoderName = decoder->GetDecoderName();

	/* Create verifier.
	 */
	Verifier	*verifier = new Verifier();

	if (verifyInput && conversionStep == ConversionStepOnTheFly) verifier->Create(trackToConvert);

	/* Enable MD5 if we are to verify the output.
	 */
	if (conversionStep == ConversionStepVerify) decoder->SetCalculateMD5(True);

	/* Output log messages.
	 */
	switch (conversionStep)
	{
		default:
		case ConversionStepOnTheFly:
			log->Write(String("\tConverting from: ").Append(trackToConvert.origFilename));

			break;
		case ConversionStepVerify:
			log->Write(String("\tVerifying: ").Append(trackToConvert.origFilename));

			break;
	}

	/* Run main conversion loop.
	 */
	Int64	 trackLength = Loop(decoder, verifier, encoder);

	/* Verify input.
	 */
	if (!cancel && verifier->Verify())
	{
		log->Write(String("\tSuccessfully verified input file: ").Append(trackToConvert.origFilename));
	}
	else if (!cancel)
	{
		onReportError.Emit(i18n->TranslateString("Failed to verify input file: %1", "Messages").Replace("%1", File(trackToConvert.origFilename).GetFileName()));

		log->WriteError(String("\tFailed to verify input file: ").Append(trackToConvert.origFilename));
	}

	/* Get MD5 checksum if we are to verify the output.
	 */
	String	 verifyChecksum;

	if (conversionStep == ConversionStepVerify) verifyChecksum = decoder->GetMD5Checksum();

	/* Output log messages.
	 */
	switch (conversionStep)
	{
		default:
		case ConversionStepOnTheFly:
			if (cancel) log->WriteWarning(String("\tCancelled converting: ").Append(trackToConvert.origFilename));
			else	    log->Write(String("\tFinished converting:" ).Append(trackToConvert.origFilename));

			break;
		case ConversionStepVerify:
			if (!cancel && encodeChecksum != verifyChecksum) onReportError.Emit(i18n->TranslateString("Checksum mismatch verifying output file: %1\n\nEncode checksum: %2\nVerify checksum: %3", "Messages").Replace("%1", File(trackToConvert.origFilename).GetFileName()).Replace("%2", encodeChecksum).Replace("%3", verifyChecksum));

			if	(cancel)			   log->WriteWarning(String("\tCancelled verifying output file: ").Append(trackToConvert.origFilename));
			else if (encodeChecksum != verifyChecksum) log->Write(String("\tChecksum mismatch verifying output file:" ).Append(trackToConvert.origFilename));
			else					   log->Write(String("\tSuccessfully verified output file:" ).Append(trackToConvert.origFilename));

			break;
	}

	/* Free decoder and verifier.
	 */
	delete decoder;
	delete verifier;

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
	trackToConvert.sampleOffset = Math::Round((Float) (encodedSamples - trackLength) / format.rate * 75);
	trackToConvert.length	    = trackLength;

	return Success();
}
