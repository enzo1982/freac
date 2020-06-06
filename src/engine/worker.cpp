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

#include <engine/worker.h>

#include <engine/decoder.h>
#include <engine/encoder.h>
#include <engine/processor.h>
#include <engine/verifier.h>
#include <engine/locking.h>

#include <config.h>

using namespace BoCA;
using namespace BoCA::AS;

#ifdef __WIN32__
#	include <windows.h>

	/* ToDo: Move this to smooth::File with smooth 0.10.
	 */
	FILETIME DateTimeToFileTime(const DateTime &dateTime)
	{
		SYSTEMTIME	 time, utc;

		memset(&time, 0, sizeof(time));

		time.wYear   = dateTime.GetYear();
		time.wMonth  = dateTime.GetMonth();
		time.wDay    = dateTime.GetDay();

		time.wHour   = dateTime.GetHour();
		time.wMinute = dateTime.GetMinute();
		time.wSecond = dateTime.GetSecond();

		FILETIME	 fileTime;

		TzSpecificLocalTimeToSystemTime(NIL, &time, &utc);
		SystemTimeToFileTime(&utc, &fileTime);

		return fileTime;
	}

	Bool SetFileCreateTime(const File &file, const FILETIME &createTime)
	{
		if (!file.Exists()) return False;

		HANDLE	 handle = CreateFile(String(Directory::GetUnicodePathPrefix(file)).Append(file), GENERIC_WRITE, 0, NIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NIL);

		SetFileTime(handle, &createTime, NIL, NIL);
		CloseHandle(handle);

		return True;
	}
#endif

freac::ConvertWorker::ConvertWorker(const BoCA::Config *iConfiguration, Int iConversionID)
{
	configuration	= iConfiguration;

	conversionID	= iConversionID;

	logName		= "Converter log";

	numThreads	= 0;

	trackToConvert	= NIL;
	trackStartTicks	= 0;
	trackPosition	= 0;

	conversionStep	= ConversionStepNone;

	idle		= True;
	waiting		= True;
	error		= False;

	pause		= False;
	cancel		= False;
	quit		= False;

	threadMain.Connect(&ConvertWorker::Perform, this);
}

freac::ConvertWorker::~ConvertWorker()
{
}

Int freac::ConvertWorker::Perform()
{
	Bool	 enableParallel	 = configuration->GetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableParallelConversionsID, Config::ResourcesEnableParallelConversionsDefault);
	Int	 numberOfThreads = configuration->GetIntValue(Config::CategoryResourcesID, Config::ResourcesNumberOfConversionThreadsID, Config::ResourcesNumberOfConversionThreadsDefault);

	while (!quit)
	{
		if (idle) { S::System::System::Sleep(1); continue; }

		/* Do not limit parallel CD ripping jobs in automatic mode.
		 */
		Bool	 allocateThread = !(enableParallel && numberOfThreads <= 1 && trackToConvert.fileName.StartsWith("device://"));

		/* Allocate thread and run conversion.
		 */
		if (allocateThread) Locking::AllocateThread();

		if (Convert() != Success()) error = True;

		if (allocateThread) Locking::FreeThread();

		/* Return to waiting state.
		 */
		idle	= True;
		waiting	= True;
		cancel	= False;
	}

	return Success();
}

Int freac::ConvertWorker::Convert()
{
	Registry	&boca = Registry::Get();

	/* Get config values.
	 */
	Bool	 encodeOnTheFly		= configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault);
	Bool	 keepWaveFiles		= configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsKeepWaveFilesID, Config::SettingsKeepWaveFilesDefault);

	Bool	 keepTimeStamps		= configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesKeepTimeStampsID, Config::SettingsFilenamesKeepTimeStampsDefault);

	Bool	 verifyInput		= configuration->GetIntValue(Config::CategoryVerificationID, Config::VerificationVerifyInputID, Config::VerificationVerifyInputDefault);
	Bool	 verifyOutput		= configuration->GetIntValue(Config::CategoryVerificationID, Config::VerificationVerifyOutputID, Config::VerificationVerifyOutputDefault);

	Bool	 writeToInputDirectory	= configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault);
	Bool	 allowOverwriteSource	= configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsAllowOverwriteSourceID, Config::SettingsAllowOverwriteSourceDefault);

	/* Find encoder to use.
	 */
	String	 selectedEncoderID	= configuration->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);
	String	 activeEncoderID	= selectedEncoderID;

	/* Do not verify output if meh! encoder selected.
	 */
	if (selectedEncoderID == "meh-enc") verifyOutput = False;

	/* We always convert on the fly when outputting simple audio files.
	 */
	if (selectedEncoderID == "wave-enc" ||
	    selectedEncoderID == "sndfile-enc") encodeOnTheFly = True;

	/* Set initial number of conversion passes.
	 */
	Int	 conversionSteps	= encodeOnTheFly ? 1 : 2;

	/* Loop over conversion passes.
	 */
	Track		 trackToEncode	= trackToConvert;
	Bool		 error		= False;

	DateTime	 createTime	= File(trackToConvert.fileName).GetCreationTime();
	DateTime	 modifyTime	= File(trackToConvert.fileName).GetWriteTime();
	Bool		 isFile		= !trackToConvert.fileName.StartsWith("device://");

	UnsignedInt32	 rippingCRC	= 0;

	String		 encodeChecksum;
	String		 verifyChecksum;

	for (Int step = 0; step < conversionSteps && !cancel; step++)
	{
		/* Setup file names.
		 */
		File	 inFile	    = trackToConvert.fileName;
		File	 outFile    = trackToConvert.outputFile;
		Bool	 replace    = outFile.Exists();

		String	 inFileName = trackToConvert.fileName;

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

		/* Setup intermediate encoder in non-on-the-fly mode.
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

			outFile = String(outFile).Append(".wav");
		}
		else if (conversionStep == ConversionStepEncode)
		{
			activeEncoderID = selectedEncoderID;

			inFile	   = String(outFile).Append(".wav");
			inFileName = inFile;
		}

		/* Create temporary output file when replacing input.
		 */
		if (String(outFile).ToLower() == String(inFile).ToLower()) outFile = String(outFile).Append(".temp");

		/* Set number of threads to requested value.
		 */
		if	(numThreads == 1) encoderConfig->SetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableParallelConversionsID, False);
		else if (numThreads >= 2) encoderConfig->SetIntValue(Config::CategoryResourcesID, Config::ResourcesNumberOfConversionThreadsID, numThreads);

		/* Setup input file name in verification step.
		 */
		if (conversionStep == ConversionStepVerify)
		{
			if	(String(outFile).ToLower() != String(inFile).ToLower().Append(".temp"))			inFile = outFile;
			else if (writeToInputDirectory && !allowOverwriteSource && !Config::Get()->deleteAfterEncoding) inFile = String(inFile).Append(".new");
			else												inFile = String(outFile).Head(String(outFile).Length() - 5);

			inFileName = inFile;

			if (!inFile.Exists())
			{
				BoCA::I18n	*i18n = BoCA::I18n::Get();
				BoCA::Protocol	*log  = BoCA::Protocol::Get(logName);

				onReportWarning.Emit(i18n->TranslateString("Skipped verification due to non existing output file: %1", "Messages").Replace("%1", inFile.GetFileName()));

				log->Lock();
				log->Write(String("    Skipping verification due to non existing output file: ").Append(inFileName), MessageTypeWarning);
				log->Write(NIL);
				log->Release();

				trackPosition = trackToConvert.length;

				onFinishTrack.Emit(trackToConvert, False);

				BoCA::Config::Free(encoderConfig);

				break;
			}
		}

		/* Check format of output file in verification step.
		 */
		if (conversionStep == ConversionStepVerify)
		{
			DecoderComponent	*decoder = boca.CreateDecoderForStream(inFileName, configuration);

			if (decoder != NIL)
			{
				Track	 outTrack;

				decoder->GetStreamInfo(inFileName, outTrack);

				boca.DeleteComponent(decoder);

				Format		 format	   = trackToEncode.GetFormat();
				const Format	&outFormat = outTrack.GetFormat();

				if (format != outFormat)
				{
					BoCA::I18n	*i18n = BoCA::I18n::Get();
					BoCA::Protocol	*log  = BoCA::Protocol::Get(logName);

					onReportWarning.Emit(i18n->TranslateString(String("Skipped verification due to format mismatch: %1\n\n")
										  .Append("Original format: %2 Hz, %3 bit, %4 channels\n")
										  .Append("Output format: %5 Hz, %6 bit, %7 channels"), "Messages").Replace("%1", inFile.GetFileName()).Replace("%2", String::FromInt(format.rate)).Replace("%3", String::FromInt(format.bits)).Replace("%4", String::FromInt(format.channels))
																						       .Replace("%5", String::FromInt(outFormat.rate)).Replace("%6", String::FromInt(outFormat.bits)).Replace("%7", String::FromInt(outFormat.channels)));

					log->Lock();
					log->Write(String("    Skipping verification due to format mismatch: ").Append(inFileName), MessageTypeWarning);
					log->Write(NIL);
					log->Release();

					trackPosition = trackToConvert.length;

					onFinishTrack.Emit(trackToConvert, False);

					BoCA::Config::Free(encoderConfig);

					break;
				}

				/* Update track format with decoder format.
				 */
				trackToConvert.SetFormat(outFormat);
			}
		}

		/* Create decoder.
		 */
		Decoder	*decoder  = new Decoder(configuration);

		if (!decoder->Create(inFileName, trackToConvert))
		{
			onReportError.Emit(decoder->GetErrorString());

			delete decoder;

			BoCA::Config::Free(encoderConfig);

			return Error();
		}

		if (conversionStep != ConversionStepEncode &&
		    conversionStep != ConversionStepVerify) decoderName = decoder->GetDecoderName();

		/* Create verifier.
		 */
		Verifier	*verifier = new Verifier(configuration);
		Bool		 verify	  = False;

		if ((conversionStep == ConversionStepOnTheFly ||
		     conversionStep == ConversionStepDecode) && verifyInput) verify = verifier->Create(trackToConvert);

		/* Create processor.
		 */
		Processor	*processor = new Processor(configuration);

		if (conversionStep == ConversionStepOnTheFly ||
		    conversionStep == ConversionStepDecode)
		{
			if (!processor->Create(trackToEncode))
			{
				onReportError.Emit(processor->GetErrorString());

				delete decoder;
				delete verifier;
				delete processor;

				BoCA::Config::Free(encoderConfig);

				return Error();
			}

			trackToEncode.SetFormat(processor->GetFormatInfo());
		}

		/* Create encoder.
		 */
		Encoder	*encoder = new Encoder(encoderConfig);

		if (conversionStep != ConversionStepVerify && !encoder->Create(activeEncoderID, outFile, trackToEncode))
		{
			onReportError.Emit(encoder->GetErrorString());

			delete decoder;
			delete verifier;
			delete processor;
			delete encoder;

			outFile.Delete();

			BoCA::Config::Free(encoderConfig);

			return Error();
		}

		if (conversionStep == ConversionStepOnTheFly ||
		    conversionStep == ConversionStepEncode) encoderName = encoder->GetEncoderName();

		trackToEncode.SetFormat(encoder->GetTargetFormat());

		/* Enable CRC calculation when ripping.
		 */
		if (inFileName.StartsWith("device://") && (conversionStep == ConversionStepOnTheFly ||
							   conversionStep == ConversionStepDecode)) decoder->SetCalculateCRC(True);

		/* Enable MD5 and add a verification step if we are to verify the output.
		 */
		if	(verifyOutput && (conversionStep == ConversionStepOnTheFly ||
					  conversionStep == ConversionStepEncode) && encoder->IsLossless()) { encoder->SetCalculateMD5(True); conversionSteps += 1; }
		else if	(verifyOutput &&  conversionStep == ConversionStepVerify			  )   decoder->SetCalculateMD5(True);

		/* Output log messages.
		 */
		LogConversionStart(decoder, inFileName, outFile, replace);

		/* Run main conversion loop.
		 */
		Int64	 trackLength = Loop(decoder, verifier, NIL, processor, encoder);

		/* Verify input.
		 */
		if (!cancel && verify) VerifyInput(inFileName, verifier);

		/* Get CRC checksums when ripping.
		 */
		if (inFileName.StartsWith("device://") && (conversionStep == ConversionStepOnTheFly ||
							   conversionStep == ConversionStepDecode)) rippingCRC = decoder->GetCRCChecksum();

		/* Get MD5 checksums if we are to verify the output.
		 */
		if	(verifyOutput && (conversionStep == ConversionStepOnTheFly ||
					  conversionStep == ConversionStepEncode) && encoder->IsLossless()) encodeChecksum = encoder->GetMD5Checksum();
		else if	(verifyOutput &&  conversionStep == ConversionStepVerify			  ) verifyChecksum = decoder->GetMD5Checksum();

		/* Free decoder, verifier, processor and encoder.
		 */
		decoder->Destroy();
		verifier->Destroy();
		processor->Destroy();
		encoder->Destroy();

		if (decoder->GetErrorState())	{ error = True; onReportError.Emit(decoder->GetErrorString());	 }
		if (verifier->GetErrorState())	{ error = True; onReportError.Emit(verifier->GetErrorString());	 }
		if (processor->GetErrorState())	{ error = True; onReportError.Emit(processor->GetErrorString()); }
		if (encoder->GetErrorState())	{ error = True; onReportError.Emit(encoder->GetErrorString());	 }

		delete decoder;
		delete verifier;
		delete processor;
		delete encoder;

		BoCA::Config::Free(encoderConfig);

		/* Output log messages.
		 */
		LogConversionEnd(inFileName, trackLength, rippingCRC, encodeChecksum, verifyChecksum);

		/* Delete output file if it doesn't look sane.
		 */
		if (outFile.GetFileSize() <= 0 || cancel || error) outFile.Delete();

		/* Delete intermediate file in non-on-the-fly mode.
		 */
		if (conversionStep == ConversionStepEncode)
		{
			if (!keepWaveFiles || cancel || error) inFile.Delete();

			if (String(inFile).EndsWith(".temp.wav")) inFile = String(inFile).Head(String(inFile).Length() - 9);
		}

		/* Move output file if temporary.
		 */
		if (String(outFile).ToLower() == String(inFile).ToLower().Append(".temp") && outFile.Exists())
		{
			File	 targetOutFile = String(outFile).Head(String(outFile).Length() - 5);

			if (!writeToInputDirectory || allowOverwriteSource || Config::Get()->deleteAfterEncoding || !targetOutFile.Exists())
			{
				targetOutFile.Delete();
				outFile.Move(targetOutFile);

				outFile = targetOutFile;
			}
			else
			{
				File(String(targetOutFile).Append(".new")).Delete();
				outFile.Move(String(targetOutFile).Append(".new"));

				outFile = String(targetOutFile).Append(".new");
			}
		}

		/* Set file time stamp to that of original file if requested.
		 */
		if ((conversionStep == ConversionStepOnTheFly || conversionStep == ConversionStepEncode) && keepTimeStamps && isFile && outFile.Exists())
		{
#ifdef __WIN32__
			SetFileCreateTime(outFile, DateTimeToFileTime(createTime));
#endif

			outFile.SetWriteTime(modifyTime);
		}

		/* Revert to waiting state when there are more steps left.
		 */
		if (step < conversionSteps - 1) waiting = True;

		/* Report finished conversion.
		 */
		onFinishTrack.Emit(trackToConvert, waiting);

		/* Update track length and offset.
		 */
		Track	 track = trackToConvert;

		trackToConvert.sampleOffset = 0;
		trackToConvert.length	    = trackLength;

		/* Fix total samples value in case we had
		 * a wrong or uncertain track length before.
		 */
		onFixTotalSamples.Emit(track, trackToConvert);
	}

	if (error) return Error();
	else	   return Success();
}

Int64 freac::ConvertWorker::Loop(Decoder *decoder, Verifier *verifier, FormatConverter *converter, Processor *processor, Encoder *encoder)
{
	/* Get config values.
	 */
	Int	 ripperTimeout	 = configuration->GetIntValue(Config::CategoryRipperID, Config::RipperTimeoutID, Config::RipperTimeoutDefault);

	/* Inform components about starting conversion.
	 */
	BoCA::Engine	*engine = BoCA::Engine::Get();

	engine->onStartTrackConversion.Emit(conversionID, trackToConvert);

	/* Enter conversion loop.
	 */
	Format		 format	     = trackToConvert.GetFormat();

	Int64		 trackOffset = encoder->GetEncodedSamples();
	UnsignedLong	 samplesSize = 512;

	trackStartTicks = S::System::System::Clock();
	trackPosition	= 0;

	waiting		= False;

	Int			 bytesPerSample = format.bits / 8 * format.channels;
	Buffer<UnsignedByte>	 buffer(samplesSize * bytesPerSample);

	while (!cancel)
	{
		Int	 step = samplesSize;

		if (trackToConvert.length >= 0)
		{
			if (trackPosition >= trackToConvert.length) break;

			if (trackPosition + step > trackToConvert.length) step = trackToConvert.length - trackPosition;
		}

		buffer.Resize(step * bytesPerSample);

		/* Read samples from decoder.
		 */
		Int	 bytes = decoder->Read(buffer);

		if	(bytes == -1) { cancel = True; break; }
		else if (bytes ==  0)		       break;

		/* Pass samples to verifier.
		 */
		if (verifier != NIL) verifier->Process(buffer);

		/* Convert format and process samples.
		 */
		if (converter != NIL) converter->Transform(buffer);
		if (processor != NIL) processor->Transform(buffer);

		/* Pass samples to encoder.
		 */
		if (encoder->Write(buffer) == -1) { cancel = True; break; }

		/* Update position info.
		 */
		if (trackToConvert.length >= 0) trackPosition += (bytes / bytesPerSample);
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

	/* Finish format conversion and processing.
	 */
	for (Int i = 0; i < 2 && !cancel; i++)
	{
		buffer.Resize(0);

		if (i == 0 && converter != NIL) converter->Finish(buffer);
		if (i == 0 && processor != NIL) processor->Transform(buffer);

		if (i == 1 && processor != NIL) processor->Finish(buffer);

		if (encoder->Write(buffer) == -1) cancel = True;
	}

	/* Inform components about finished/cancelled conversion.
	 */
	if (cancel) engine->onCancelTrackConversion.Emit(conversionID, trackToConvert);
	else	    engine->onFinishTrackConversion.Emit(conversionID, trackToConvert);

	if (encoder->GetEncodedSamples() > 0) return encoder->GetEncodedSamples() - trackOffset;
	else				      return decoder->GetDecodedSamples();
}

Void freac::ConvertWorker::VerifyInput(const String &uri, Verifier *verifier)
{
	BoCA::Protocol	*log = BoCA::Protocol::Get(logName);

	log->Lock();

	String	 uriType = uri.StartsWith("device://") ? "track" : "file";

	if (verifier->Verify())
	{
		log->Write(String("    Successfully verified input ").Append(uriType).Append(": ").Append(uri));
	}
	else
	{
		BoCA::I18n	*i18n = BoCA::I18n::Get();

		onReportError.Emit(i18n->TranslateString(String("Failed to verify input ").Append(uriType).Append(": %1"), "Errors").Replace("%1", uri.Contains("://") ? uri : File(uri).GetFileName()).Append("\n\n").Append(verifier->GetErrorString()));

		log->Write(String("    Failed to verify input ").Append(uriType).Append(": ").Append(uri), MessageTypeError);
	}

	/* Print messages generated by verifier component.
	 */
	const Array<String>	&messages = verifier->GetErrorString().Explode("\n");

	foreach (const String &message, messages) log->Write(String("        ").Append(message));

	log->Write(NIL);
	log->Release();
}

Void freac::ConvertWorker::LogConversionStart(Decoder *decoder, const String &uri, const String &outFile, Bool replace) const
{
	String	 verb = "Converting";

	if	(uri.StartsWith("device://"))		 verb = "Ripping";
	else if	(conversionStep == ConversionStepDecode) verb = "Decoding";
	else if (conversionStep == ConversionStepEncode) verb = "Encoding";
	else if (conversionStep == ConversionStepVerify) verb = "Verifying";

	BoCA::Protocol	*log = BoCA::Protocol::Get(logName);

	log->Lock();
	log->Write(String("    ").Append(verb).Append(": ").Append(uri));

	/* Log output file.
	 */
	String	 outputFile = outFile;

	if (String(outFile).ToLower() == String(uri).ToLower().Append(".temp")) outputFile = uri;

	if (outFile != NIL && conversionStep != ConversionStepVerify)
	{
		log->Write(String("    ").Append(String().FillN(' ', verb.Length() - 2)).Append("to: ").Append(outputFile));
	}

	/* Log overwriting of files.
	 */
	if (outFile != NIL && (conversionStep == ConversionStepOnTheFly || conversionStep == ConversionStepEncode))
	{
		if	(outputFile == uri) log->Write("        Replacing input file with conversion result");
		else if	(replace)	    log->Write("        Replacing existing file with conversion result");
	}

	/* Log used decoder.
	 */
	if (conversionStep == ConversionStepOnTheFly || conversionStep == ConversionStepDecode)
	{
		log->Write(String("        Decoder: ").Append(decoder->GetDecoderName()));
	}

	log->Write(NIL);
	log->Release();
}

Void freac::ConvertWorker::LogConversionEnd(const String &uri, Int64 trackLength, UnsignedInt32 rippingCRC, const String &encodeChecksum, const String &verifyChecksum) const
{
	String	 verb = "converting";

	if	(uri.StartsWith("device://"))		 verb = "ripping";
	else if	(conversionStep == ConversionStepDecode) verb = "decoding";
	else if (conversionStep == ConversionStepEncode) verb = "encoding";

	BoCA::Protocol	*log = BoCA::Protocol::Get(logName);

	log->Lock();

	if (conversionStep != ConversionStepVerify)
	{
		/* Log conversion result.
		 */
		if (cancel)
		{
			log->Write(String("    Cancelled ").Append(verb).Append(": ").Append(uri), MessageTypeWarning);
		}
		else
		{
			log->Write(String("    Finished ").Append(verb).Append(": ").Append(uri));

			if (uri.StartsWith("device://") && (conversionStep == ConversionStepOnTheFly ||
							    conversionStep == ConversionStepDecode)) log->Write(String("        CRC checksum: ").Append(Number(Int64(rippingCRC)).ToHexString(8)));
		}
	}
	else
	{
		/* Log verification result.
		 */
		if (cancel)
		{
			log->Write(String("    Cancelled verifying output file: %1").Replace("%1", uri), MessageTypeWarning);
		}
		else if (encodeChecksum != verifyChecksum)
		{
			BoCA::I18n	*i18n = BoCA::I18n::Get();

			onReportError.Emit(i18n->TranslateString("Checksum mismatch verifying output file: %1\n\nEncode checksum: %2\nVerify checksum: %3", "Errors").Replace("%1", File(uri).GetFileName()).Replace("%2", encodeChecksum).Replace("%3", verifyChecksum));

			log->Write(String("    Checksum mismatch verifying output file: %1").Replace("%1", uri), MessageTypeError);
			log->Write(String("        Encode MD5: %1").Replace("%1", encodeChecksum));
			log->Write(String("        Verify MD5: %1").Replace("%1", verifyChecksum));
		}
		else
		{
			log->Write(String("    Successfully verified output file: %1").Replace("%1", uri));
			log->Write(String("        MD5 hash: %1").Replace("%1", verifyChecksum));
		}
	}

	/* Log duration.
	 */
	if (!cancel)
	{
		UnsignedInt64	 ticks	  = S::System::System::Clock() - trackStartTicks;
		String		 duration = String(ticks / 1000 / 60 % 60 <  10 ?			      "0"  : "").Append(String::FromInt(ticks / 1000 / 60 % 60)).Append(":")
					   .Append(ticks / 1000 % 60	  <  10 ?			      "0"  : "").Append(String::FromInt(ticks / 1000 % 60     )).Append(".")
					   .Append(ticks % 1000		  < 100 ? (ticks % 1000 < 10 ? "00" : "0") : "").Append(String::FromInt(ticks % 1000	      ));
		String		 speed	  = String::FromFloat(Math::Round(Float(trackLength) / trackToConvert.GetFormat().rate / (Float(ticks) / 1000.0) * 10.0) / 10.0);

		if (!speed.Contains(".")) speed.Append(".0");

		log->Write(String("        Duration: ").Append(duration).Append(" (").Append(speed).Append("x speed)"));
	}

	log->Write(NIL);
	log->Release();
}

Void freac::ConvertWorker::SetTrackToConvert(const BoCA::Track &nTrack)
{
	trackToConvert	= nTrack;
	trackStartTicks	= 0;
	trackPosition	= 0;

	idle		= False;
	waiting		= True;
	error		= False;
}

Int freac::ConvertWorker::Pause(Bool value)
{
	pause = value;

	return Success();
}

Int freac::ConvertWorker::Cancel()
{
	cancel = True;

	return Success();
}

Int freac::ConvertWorker::Quit()
{
	cancel = True;
	quit   = True;

	return Success();
}
