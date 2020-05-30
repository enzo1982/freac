 /* fre:ac - free audio converter
  * Copyright (C) 2001-2020 Robert Kausch <robert.kausch@freac.org>
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
	Registry	&boca = Registry::Get();

	/* Get config values.
	 */
	Int	 singleFileMode	= configuration->GetIntValue(Config::CategoryProcessingID, Config::ProcessingSingleFileModeID, Config::ProcessingSingleFileModeDefault);

	Bool	 verifyInput	= configuration->GetIntValue(Config::CategoryVerificationID, Config::VerificationVerifyInputID, Config::VerificationVerifyInputDefault);

	/* Set conversion step.
	 */
	if (conversionStep == ConversionStepNone) conversionStep = ConversionStepOnTheFly;

	/* Check format of output file in verification step.
	 */
	if (conversionStep == ConversionStepVerify)
	{
		DecoderComponent	*decoder = boca.CreateDecoderForStream(trackToConvert.fileName, configuration);

		if (decoder != NIL)
		{
			Track	 outTrack;

			decoder->GetStreamInfo(trackToConvert.fileName, outTrack);

			boca.DeleteComponent(decoder);

			Format		 format	   = trackToConvert.GetFormat();
			const Format	&outFormat = outTrack.GetFormat();

			if (format != outFormat)
			{
				BoCA::I18n	*i18n = BoCA::I18n::Get();
				BoCA::Protocol	*log  = BoCA::Protocol::Get(logName);

				onReportWarning.Emit(i18n->TranslateString(String("Skipped verification due to format mismatch: %1\n\n")
									  .Append("Original format: %2 Hz, %3 bit, %4 channels\n")
									  .Append("Output format: %5 Hz, %6 bit, %7 channels"), "Messages").Replace("%1", File(trackToConvert.fileName).GetFileName()).Replace("%2", String::FromInt(format.rate)).Replace("%3", String::FromInt(format.bits)).Replace("%4", String::FromInt(format.channels))
																									  .Replace("%5", String::FromInt(outFormat.rate)).Replace("%6", String::FromInt(outFormat.bits)).Replace("%7", String::FromInt(outFormat.channels)));

				log->Lock();
				log->Write(String("    Skipping verification due to format mismatch: ").Append(trackToConvert.fileName), MessageTypeWarning);
				log->Write(NIL);
				log->Release();

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

	if (!decoder->Create(trackToConvert.fileName, trackToConvert))
	{
		onReportError.Emit(decoder->GetErrorString());

		delete decoder;

		return Error();
	}

	decoderName = decoder->GetDecoderName();
	encoderName = encoder->GetEncoderName();

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
			onReportError.Emit(processor->GetErrorString());

			delete decoder;
			delete verifier;
			delete converter;
			delete processor;

			return Error();
		}
	}

	/* Enable CRC calculation when ripping.
	 */
	if (trackToConvert.fileName.StartsWith("device://") && (conversionStep == ConversionStepOnTheFly ||
								conversionStep == ConversionStepDecode)) decoder->SetCalculateCRC(True);

	/* Enable MD5 if we are to verify the output.
	 */
	if (conversionStep == ConversionStepVerify) decoder->SetCalculateMD5(True);

	/* Output log messages.
	 */
	LogConversionStart(decoder, trackToConvert.fileName);

	/* Run main conversion loop.
	 */
	Int64	 trackLength = Loop(decoder, verifier, converter, processor, encoder);

	/* Verify input.
	 */
	if (!cancel && verify) VerifyInput(trackToConvert.fileName, verifier);

	/* Get CRC checksums when ripping.
	 */
	UnsignedInt32	 rippingCRC = 0;

	if (trackToConvert.fileName.StartsWith("device://") && (conversionStep == ConversionStepOnTheFly ||
								conversionStep == ConversionStepDecode)) rippingCRC = decoder->GetCRCChecksum();

	/* Get MD5 checksum if we are to verify the output.
	 */
	String	 verifyChecksum;

	if (conversionStep == ConversionStepVerify) verifyChecksum = decoder->GetMD5Checksum();

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

	/* Output log messages.
	 */
	LogConversionEnd(trackToConvert.fileName, trackLength, rippingCRC, encodeChecksum, verifyChecksum);

	/* Report finished conversion.
	 */
	onFinishTrack.Emit(trackToConvert, False);

	/* Update track length and offset.
	 */
	Track	 track	= trackToConvert;

	trackToConvert.sampleOffset = encodedSamples - trackLength;
	trackToConvert.length	    = trackLength;

	/* Fix total samples value in case we had
	 * a wrong or uncertain track length before.
	 */
	onFixTotalSamples.Emit(track, trackToConvert);

	return Success();
}
