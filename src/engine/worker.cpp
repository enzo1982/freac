 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <engine/worker.h>

#include <config.h>

using namespace BoCA;
using namespace BoCA::AS;

BonkEnc::ConvertWorker::ConvertWorker(const BoCA::Config *iConfiguration)
{
	SetFlags(Threads::THREAD_WAITFLAG_START);

	configuration	= iConfiguration;

	trackToConvert	= NIL;
	trackStartTicks	= 0;
	trackPosition	= 0;

	conversionStep	= ConversionStepNone;

	idle		= True;
	waiting		= True;

	pause		= False;
	cancel		= False;
	quit		= False;

	threadMain.Connect(&ConvertWorker::Perform, this);
}

BonkEnc::ConvertWorker::~ConvertWorker()
{
}

Int BonkEnc::ConvertWorker::Perform()
{
	while (!quit)
	{
		if (idle) { S::System::System::Sleep(100); continue; }

		Convert();

		idle	= True;
		waiting	= True;
		cancel	= False;
	}

	return Success();
}

Int BonkEnc::ConvertWorker::Convert()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	Registry	&boca = Registry::Get();

	/* Get config values.
	 */
	Bool	 encodeOnTheFly		= configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault);
	Bool	 keepWaveFiles		= configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsKeepWaveFilesID, Config::SettingsKeepWaveFilesDefault);

	Bool	 verifyInput		= configuration->GetIntValue(Config::CategoryVerificationID, Config::VerificationVerifyInputID, Config::VerificationVerifyInputDefault);
	Bool	 verifyOutput		= configuration->GetIntValue(Config::CategoryVerificationID, Config::VerificationVerifyOutputID, Config::VerificationVerifyOutputDefault);

	Bool	 writeToInputDirectory	= configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault);
	Bool	 allowOverwriteSource	= configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsAllowOverwriteSourceID, Config::SettingsAllowOverwriteSourceDefault);

	/* Find encoder to use.
	 */
	String	 selectedEncoderID	= configuration->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);
	String	 activeEncoderID	= selectedEncoderID;

	/* We always convert on the fly when outputting simple audio files.
	 */
	if (selectedEncoderID == "wave-enc" ||
	    selectedEncoderID == "sndfile-enc") encodeOnTheFly = True;

	/* Set initial number of conversion passes.
	 */
	Int	 conversionSteps	= encodeOnTheFly ? 1 : 2;

	/* Setup conversion log.
	 */
	BoCA::Protocol	*log = BoCA::Protocol::Get("Converter log");

	/* Loop over conversion passes.
	 */
	String	 encodeChecksum;
	String	 verifyChecksum;

	for (Int step = 0; step < conversionSteps && !cancel; step++)
	{
		/* Setup track and file names.
		 */
		Track	 track  = trackToConvert;
		Format	 format = trackToConvert.GetFormat();

		String	 in_filename  = trackToConvert.origFilename;
		String	 out_filename = trackToConvert.outfile;

		if (out_filename.ToLower() == in_filename.ToLower()) out_filename.Append(".temp");

		/* Set conversion step.
		 */
		if (encodeOnTheFly)
		{
			if	(step == 0) conversionStep = ConversionStepOnTheFly;
			else		    conversionStep = ConversionStepVerify;
		}
		else
		{
			if	(step == 0) conversionStep = ConversionStepDecode;
			else if (step == 1) conversionStep = ConversionStepEncode;
			else		    conversionStep = ConversionStepVerify;
		}

		/* Setup intermediate encoder in non-on-the-fly mode
		 */
		BoCA::Config	*encoderConfig = BoCA::Config::Copy(configuration);

		if (conversionStep == ConversionStepDecode)
		{
			activeEncoderID = "wave-enc";

			if (!boca.ComponentExists("wave-enc"))
			{
				activeEncoderID = "sndfile-enc";

				encoderConfig->SetIntValue("SndFile", "Format", 0x010000);
				encoderConfig->SetIntValue("SndFile", "SubFormat", 0x000000);
			}

			out_filename.Append(".wav");
		}
		else if (conversionStep == ConversionStepEncode)
		{
			activeEncoderID = selectedEncoderID;

			in_filename = out_filename;
			in_filename.Append(".wav");
		}

		/* Setup input file name in verification step.
		 */
		if (conversionStep == ConversionStepVerify) in_filename = out_filename;

		/* Check format of output file in verification step.
		 */
		if (conversionStep == ConversionStepVerify)
		{
			DecoderComponent	*decoder = boca.CreateDecoderForStream(in_filename);

			if (decoder != NIL)
			{
				Track	 outTrack;

				decoder->GetStreamInfo(in_filename, outTrack);

				boca.DeleteComponent(decoder);

				const Format	&outFormat = outTrack.GetFormat();

				if (outFormat.rate     != format.rate ||
				    outFormat.bits     != format.bits ||
				    outFormat.channels != format.channels)
				{
					onReportWarning.Emit(i18n->TranslateString(String("Skipped verification due to format mismatch: %1\n\n")
										  .Append("Original format: %2 Hz, %3 bit, %4 channels\n")
										  .Append("Output format: %5 Hz, %6 bit, %7 channels"), "Messages").Replace("%1", File(in_filename).GetFileName()).Replace("%2", String::FromInt(format.rate)).Replace("%3", String::FromInt(format.bits)).Replace("%4", String::FromInt(format.channels))
																								  .Replace("%5", String::FromInt(outFormat.rate)).Replace("%6", String::FromInt(outFormat.bits)).Replace("%7", String::FromInt(outFormat.channels)));

					log->Write(String("\tSkipping verification due to format mismatch: ").Append(in_filename), MessageTypeWarning);

					trackPosition = trackToConvert.length;

					onFinishTrack.Emit(trackToConvert, False);

					break;
				}

				/* Update track format with decoder format.
				 */
				format.order = outFormat.order;

				trackToConvert.SetFormat(format);
			}
		}

		/* Create decoder.
		 */
		Decoder	*decoder = new Decoder(configuration);

		if (!decoder->Create(in_filename, trackToConvert))
		{
			delete decoder;

			return Error();
		}

		decoderName = decoder->GetDecoderName();

		/* Create encoder.
		 */
		Encoder	*encoder = new Encoder(encoderConfig);

		if (conversionStep != ConversionStepVerify && !encoder->Create(activeEncoderID, out_filename, trackToConvert))
		{
			delete decoder;
			delete encoder;

			File(out_filename).Delete();

			return Error();
		}

		/* Create verifier.
		 */
		Verifier	*verifier = new Verifier(configuration);
		Bool		 verify	  = False;

		if (verifyInput && (conversionStep == ConversionStepOnTheFly ||
				    conversionStep == ConversionStepDecode)) verify = verifier->Create(trackToConvert);

		/* Enable MD5 and add a verification step if we are to verify the output.
		 */
		if	(verifyOutput && (conversionStep == ConversionStepOnTheFly ||
					  conversionStep == ConversionStepEncode) && encoder->IsLossless()) { encoder->SetCalculateMD5(True); conversionSteps += 1; }
		else if	(verifyOutput &&  conversionStep == ConversionStepVerify			  )   decoder->SetCalculateMD5(True);

		/* Output log messages.
		 */
		switch (conversionStep)
		{
			default:
			case ConversionStepOnTheFly:
				log->Write(String("\tConverting from: ").Append(in_filename));
				log->Write(String("\t           to:   ").Append(out_filename));

				break;
			case ConversionStepDecode:
				log->Write(String("\tDecoding from: ").Append(in_filename));
				log->Write(String("\t         to:   ").Append(out_filename));

				break;
			case ConversionStepEncode:
				log->Write(String("\tEncoding from: ").Append(in_filename));
				log->Write(String("\t         to:   ").Append(out_filename));

				break;
			case ConversionStepVerify:
				log->Write(String("\tVerifying: ").Append(in_filename));

				break;
		}

		/* Run main conversion loop.
		 */
		Int64	 trackLength = Loop(decoder, verifier, encoder);

		/* Verify input.
		 */
		if (!cancel && verify && verifier->Verify())
		{
			log->Write(String("\tSuccessfully verified input file: ").Append(in_filename));
		}
		else if (!cancel && verify)
		{
			onReportError.Emit(i18n->TranslateString("Failed to verify input file: %1", "Messages").Replace("%1", in_filename.Contains("://") ? in_filename : File(in_filename).GetFileName()));

			log->Write(String("\tFailed to verify input file: ").Append(in_filename), MessageTypeError);
		}

		/* Get MD5 checksums if we are to verify the output.
		 */
		if	(verifyOutput && (conversionStep == ConversionStepOnTheFly ||
					  conversionStep == ConversionStepEncode) && encoder->IsLossless()) encodeChecksum = encoder->GetMD5Checksum();
		else if	(verifyOutput &&  conversionStep == ConversionStepVerify			  ) verifyChecksum = decoder->GetMD5Checksum();

		/* Output log messages.
		 */
		switch (conversionStep)
		{
			default:
			case ConversionStepOnTheFly:
				if (cancel) log->Write(String("\tCancelled converting: ").Append(in_filename), MessageTypeWarning);
				else	    log->Write(String("\tFinished converting:" ).Append(in_filename));

				break;
			case ConversionStepDecode:
				if (cancel) log->Write(String("\tCancelled decoding: ").Append(in_filename), MessageTypeWarning);
				else	    log->Write(String("\tFinished decoding:" ).Append(in_filename));

				break;
			case ConversionStepEncode:
				if (cancel) log->Write(String("\tCancelled encoding: ").Append(in_filename), MessageTypeWarning);
				else	    log->Write(String("\tFinished encoding:" ).Append(in_filename));

				break;
			case ConversionStepVerify:
				if (!cancel && encodeChecksum != verifyChecksum) onReportError.Emit(i18n->TranslateString("Checksum mismatch verifying output file: %1\n\nEncode checksum: %2\nVerify checksum: %3", "Messages").Replace("%1", File(in_filename).GetFileName()).Replace("%2", encodeChecksum).Replace("%3", verifyChecksum));

				if	(cancel)			   log->Write(String("\tCancelled verifying output file: ").Append(in_filename), MessageTypeWarning);
				else if (encodeChecksum != verifyChecksum) log->Write(String("\tChecksum mismatch verifying output file:" ).Append(in_filename), MessageTypeError);
				else					   log->Write(String("\tSuccessfully verified output file:" ).Append(in_filename));

				break;
		}

		/* Free encoder, decoder and verifier.
		 */
		delete encoder;
		delete decoder;
		delete verifier;

		BoCA::Config::Free(encoderConfig);

		/* Delete output file if it doesn't look sane.
		 */
		if (File(out_filename).GetFileSize() <= 0 || cancel) File(out_filename).Delete();

		/* Delete input file if requested or not in on-the-fly mode.
		 */
		if (conversionStep == ConversionStepEncode)
		{
			if (!keepWaveFiles || cancel) File(in_filename).Delete();

			if (in_filename.EndsWith(".temp.wav")) in_filename[in_filename.Length() - 9] = 0;
		}
		else if (conversionStep != ConversionStepVerify && Config::Get()->deleteAfterEncoding && !cancel)
		{
			File(in_filename).Delete();
		}

		/* Move output file if temporary.
		 */
		if (out_filename.ToLower() == String(in_filename.ToLower()).Append(".temp") && File(out_filename).Exists())
		{
			if (!writeToInputDirectory || allowOverwriteSource || !File(in_filename).Exists())
			{
				File(in_filename).Delete();
				File(out_filename).Move(in_filename);

				out_filename = in_filename;
			}
			else
			{
				File(String(in_filename).Append(".new")).Delete();
				File(out_filename).Move(String(in_filename).Append(".new"));

				out_filename = String(in_filename).Append(".new");
			}
		}

		/* Revert to waiting state when there are more steps left.
		 */
		if (step < conversionSteps - 1) waiting = True;

		/* Report finished conversion.
		 */
		onFinishTrack.Emit(trackToConvert, waiting);

		/* Update track length and offset.
		 */
		trackToConvert.sampleOffset = 0;
		trackToConvert.length	    = trackLength;

		/* Fix total samples value in case we had
		 * a wrong or uncertain track length before.
		 */
		onFixTotalSamples.Emit(track, trackToConvert);
	}

	return Success();
}

Int64 BonkEnc::ConvertWorker::Loop(Decoder *decoder, Verifier *verifier, Encoder *encoder)
{
	/* Get config values.
	 */
	Int	 ripperTimeout	 = configuration->GetIntValue(Config::CategoryRipperID, Config::RipperTimeoutID, Config::RipperTimeoutDefault);

	/* Enter conversion loop.
	 */
	Format		 format	     = trackToConvert.GetFormat();

	Int64		 trackLength = 0;
	UnsignedLong	 samplesSize = 512;

	trackStartTicks = S::System::System::Clock();
	trackPosition	= 0;

	waiting		= False;

	Int			 bytesPerSample = format.bits / 8;
	Buffer<UnsignedByte>	 buffer(samplesSize * bytesPerSample * format.channels);

	while (!cancel)
	{
		Int	 step = samplesSize;

		if (trackToConvert.length >= 0)
		{
			if (trackPosition >= trackToConvert.length) break;

			if (trackPosition + step > trackToConvert.length) step = trackToConvert.length - trackPosition;
		}

		buffer.Resize(step * bytesPerSample * format.channels);

		/* Read samples from decoder.
		 */
		Int	 bytes = decoder->Read(buffer);

		if	(bytes == -1) { cancel = True; break; }
		else if (bytes ==  0)		       break;

		/* Pass samples to verifier.
		 */
		verifier->Process(buffer);

		/* Pass samples to encoder.
		 */
		if (encoder->Write(buffer) == -1) { cancel = True; break; }

		/* Update length and position info.
		 */
		trackLength += (bytes / bytesPerSample / format.channels);

		if (trackToConvert.length >= 0) trackPosition += (bytes / bytesPerSample / format.channels);
		else				trackPosition = decoder->GetInBytes();

		/* Check for ripper timeout.
		 */
		if (trackToConvert.isCDTrack && ripperTimeout > 0 && S::System::System::Clock() - trackStartTicks > (UnsignedInt64) ripperTimeout * 1000)
		{
			BoCA::Utilities::WarningMessage("CD ripping timeout after %1 seconds. Skipping track.", String::FromInt(ripperTimeout));

			cancel = True;
		}

		/* Pause if requested.
		 */
		while (pause && !cancel) S::System::System::Sleep(50);
	}

	return trackLength;
}

Void BonkEnc::ConvertWorker::SetTrackToConvert(const BoCA::Track &nTrack)
{
	trackToConvert	= nTrack;
	trackStartTicks	= 0;
	trackPosition	= 0;

	idle		= False;
	waiting		= True;
}

Int BonkEnc::ConvertWorker::Pause(Bool value)
{
	pause = value;

	return Success();
}

Int BonkEnc::ConvertWorker::Cancel()
{
	cancel = True;

	return Success();
}

Int BonkEnc::ConvertWorker::Quit()
{
	cancel = True;
	quit   = True;

	return Success();
}
