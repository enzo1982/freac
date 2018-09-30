 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <engine/worker_singlefile.h>

#include <engine/decoder.h>
#include <engine/encoder.h>
#include <engine/processor.h>
#include <engine/verifier.h>

#include <config.h>

using namespace BoCA;
using namespace BoCA::AS;

freac::ConvertWorkerSingleFile::ConvertWorkerSingleFile(const BoCA::Config *iConfiguration, Int iConversionID, const BoCA::Format &iFormat, Processor *iProcessor, Encoder *iEncoder) : ConvertWorker(iConfiguration, iConversionID)
{
	format	       = iFormat;

	processor      = iProcessor;
	encoder	       = iEncoder;

	encodedSamples = 0;
}

freac::ConvertWorkerSingleFile::~ConvertWorkerSingleFile()
{
}

Int freac::ConvertWorkerSingleFile::Convert()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	Registry	&boca = Registry::Get();

	/* Get config values.
	 */
	Int	 singleFileMode	= configuration->GetIntValue(Config::CategoryProcessingID, Config::ProcessingSingleFileModeID, Config::ProcessingSingleFileModeDefault);

	Bool	 verifyInput	= configuration->GetIntValue(Config::CategoryVerificationID, Config::VerificationVerifyInputID, Config::VerificationVerifyInputDefault);

	/* Setup conversion log.
	 */
	BoCA::Protocol	*log = BoCA::Protocol::Get(logName);

	/* Set conversion step.
	 */
	if (conversionStep == ConversionStepNone) conversionStep = ConversionStepOnTheFly;

	/* Check format of output file in verification step.
	 */
	if (conversionStep == ConversionStepVerify)
	{
		DecoderComponent	*decoder = boca.CreateDecoderForStream(trackToConvert.origFilename);

		if (decoder != NIL)
		{
			Track	 outTrack;

			decoder->GetStreamInfo(trackToConvert.origFilename, outTrack);

			boca.DeleteComponent(decoder);

			Format		 format	   = trackToConvert.GetFormat();
			const Format	&outFormat = outTrack.GetFormat();

			if (format != outFormat)
			{
				onReportWarning.Emit(i18n->TranslateString(String("Skipped verification due to format mismatch: %1\n\n")
									  .Append("Original format: %2 Hz, %3 bit, %4 channels\n")
									  .Append("Output format: %5 Hz, %6 bit, %7 channels"), "Messages").Replace("%1", File(trackToConvert.origFilename).GetFileName()).Replace("%2", String::FromInt(format.rate)).Replace("%3", String::FromInt(format.bits)).Replace("%4", String::FromInt(format.channels))
																									  .Replace("%5", String::FromInt(outFormat.rate)).Replace("%6", String::FromInt(outFormat.bits)).Replace("%7", String::FromInt(outFormat.channels)));

				log->Write(String("\tSkipping verification due to format mismatch: ").Append(trackToConvert.origFilename), MessageTypeWarning);

				trackPosition = trackToConvert.length;

				onFinishTrack.Emit(trackToConvert, False);

				return Success();
			}

			/* Update track format with decoder format.
			 */
			trackToConvert.SetFormat(outFormat);
		}
	}

	/* Create decoder.
	 */
	Decoder	*decoder = new Decoder(configuration);

	if (!decoder->Create(trackToConvert.origFilename, trackToConvert))
	{
		onReportError.Emit(decoder->GetErrorString());

		delete decoder;

		return Error();
	}

	decoderName = decoder->GetDecoderName();

	/* Create verifier.
	 */
	Verifier	*verifier = new Verifier(configuration);
	Bool		 verify	  = False;

	if ((conversionStep == ConversionStepOnTheFly ||
	     conversionStep == ConversionStepDecode) && verifyInput) verify = verifier->Create(trackToConvert);

	/* Create format converter.
	 */
	FormatConverter	*converter = new FormatConverter(trackToConvert.GetFormat(), format);

	/* Create processor.
	 */
	if (singleFileMode == 0)
	{
		Track	 trackToProcess = trackToConvert;

		trackToProcess.SetFormat(format);

		processor = new Processor(configuration);

		if ((conversionStep == ConversionStepOnTheFly ||
		     conversionStep == ConversionStepDecode) && !processor->Create(trackToProcess))
		{
			delete decoder;
			delete verifier;
			delete converter;
			delete processor;

			return Error();
		}
	}

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
	Int64	 trackLength = Loop(decoder, verifier, converter, processor, encoder);

	/* Verify input.
	 */
	if (!cancel && verify && verifier->Verify())
	{
		log->Write(String("\tSuccessfully verified input file: ").Append(trackToConvert.origFilename));
	}
	else if (!cancel && verify)
	{
		onReportError.Emit(i18n->TranslateString("Failed to verify input file: %1", "Messages").Replace("%1", File(trackToConvert.origFilename).GetFileName()));

		log->Write(String("\tFailed to verify input file: ").Append(trackToConvert.origFilename), MessageTypeError);
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
			if (cancel) log->Write(String("\tCancelled converting: ").Append(trackToConvert.origFilename), MessageTypeWarning);
			else	    log->Write(String("\tFinished converting: ").Append(trackToConvert.origFilename));

			break;
		case ConversionStepVerify:
			if (!cancel && encodeChecksum != verifyChecksum) onReportError.Emit(i18n->TranslateString("Checksum mismatch verifying output file: %1\n\nEncode checksum: %2\nVerify checksum: %3", "Messages").Replace("%1", File(trackToConvert.origFilename).GetFileName()).Replace("%2", encodeChecksum).Replace("%3", verifyChecksum));

			if	(cancel)			   log->Write(String("\tCancelled verifying output file: ").Append(trackToConvert.origFilename), MessageTypeWarning);
			else if (encodeChecksum != verifyChecksum) log->Write(String("\tChecksum mismatch verifying output file: ").Append(trackToConvert.origFilename));
			else					   log->Write(String("\tSuccessfully verified output file: ").Append(trackToConvert.origFilename));

			break;
	}

	/* Get output format info.
	 */
	Format	 format = trackToConvert.GetFormat();

	if (processor != NIL) format = processor->GetFormatInfo();

	/* Free decoder and verifier.
	 */
	decoder->Destroy();
	verifier->Destroy();

	if (decoder->GetErrorState())  onReportError.Emit(decoder->GetErrorString());
	if (verifier->GetErrorState()) onReportError.Emit(verifier->GetErrorString());

	delete decoder;
	delete verifier;
	delete converter;

	if (singleFileMode == 0) delete processor;

	/* Signal next chapter.
	 */
	if (!cancel)
	{
		encoder->SignalChapterChange();
		encodedSamples += trackLength;
	}

	/* Report finished conversion.
	 */
	onFinishTrack.Emit(trackToConvert, False);

	/* Update track length and offset.
	 */
	Track	 track	= trackToConvert;

	trackToConvert.sampleOffset = Math::Round((Float) (encodedSamples - trackLength) / format.rate * 75);
	trackToConvert.length	    = trackLength;

	/* Fix total samples value in case we had
	 * a wrong or uncertain track length before.
	 */
	onFixTotalSamples.Emit(track, trackToConvert);

	return Success();
}
