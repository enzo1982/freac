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

BonkEnc::ConvertWorker::ConvertWorker()
{
	SetFlags(Threads::THREAD_WAITFLAG_START);

	trackToConvert	  = NIL;
	trackStartTicks	  = 0;
	trackPosition	  = 0;

	conversionStep	  = CONVERTER_STEP_ON_THE_FLY;

	idle		  = True;
	waiting		  = True;

	pause		  = False;
	cancel		  = False;
	quit		  = False;

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
	BoCA::Config	*config	= BoCA::Config::Get();
	Registry	&boca	= Registry::Get();

	/* Get config values.
	 */
	Bool	 encodeOnTheFly		= config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault);
	Bool	 keepWaveFiles		= config->GetIntValue(Config::CategorySettingsID, Config::SettingsKeepWaveFilesID, Config::SettingsKeepWaveFilesDefault);

	Bool	 writeToInputDirectory	= config->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault);
	Bool	 allowOverwriteSource	= config->GetIntValue(Config::CategorySettingsID, Config::SettingsAllowOverwriteSourceID, Config::SettingsAllowOverwriteSourceDefault);

#ifdef __APPLE__
	Int	 sndFileFormat		= config->GetIntValue("SndFile", "Format", 0x020000);
	Int	 sndFileSubFormat	= config->GetIntValue("SndFile", "SubFormat", 0x000000);
#else
	Int	 sndFileFormat		= config->GetIntValue("SndFile", "Format", 0x010000);
	Int	 sndFileSubFormat	= config->GetIntValue("SndFile", "SubFormat", 0x000000);
#endif

	/* Find encoder to use.
	 */
	String	 selectedEncoderID	= config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);
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
	for (Int step = 0; step < conversionSteps && !cancel; step++)
	{
		/* Setup track and file names.
		 */
		Track	 track  = trackToConvert;
		Format	 format = trackToConvert.GetFormat();

		String	 in_filename  = trackToConvert.origFilename;
		String	 out_filename = trackToConvert.outfile;

		if (out_filename.ToLower() == in_filename.ToLower()) out_filename.Append(".temp");

		/* Set conversion step in non-on-the-fly mode.
		 */
		if (!encodeOnTheFly)
		{
			if (step == 0) conversionStep = CONVERTER_STEP_DECODE;
			else	       conversionStep = CONVERTER_STEP_ENCODE;
		}

		/* Setup intermediate encoder in non-on-the-fly mode
		 */
		if (conversionStep == CONVERTER_STEP_DECODE)
		{
			activeEncoderID = "wave-enc";

			if (!boca.ComponentExists("wave-enc"))
			{
				activeEncoderID = "sndfile-enc";

				config->SetIntValue("SndFile", "Format", 0x010000);
				config->SetIntValue("SndFile", "SubFormat", 0x000000);
			}

			out_filename.Append(".wav");
		}
		else if (conversionStep == CONVERTER_STEP_ENCODE)
		{
			activeEncoderID = selectedEncoderID;

			in_filename = out_filename;
			in_filename.Append(".wav");
		}

		/* Create decoder.
		 */
		Decoder	*decoder = new Decoder();

		if (!decoder->Create(in_filename, trackToConvert))
		{
			delete decoder;

			return Error();
		}

		decoderName = decoder->GetDecoderName();

		/* Create encoder.
		 */
		Encoder	*encoder = new Encoder();

		if (!encoder->Create(activeEncoderID, out_filename, trackToConvert))
		{
			delete decoder;
			delete encoder;

			File(out_filename).Delete();

			return Error();
		}

		/* Run main conversion loop.
		 */
		log->Write(String("\tEncoding from: ").Append(in_filename));
		log->Write(String("\t         to:   ").Append(out_filename));

		Int64	 trackLength = Loop(decoder, encoder);

		if (cancel) log->WriteWarning(String("\tCancelled encoding: ").Append(in_filename));
		else	    log->Write(String("\tFinished encoding:" ).Append(in_filename));

		/* Free encoder and decoder.
		 */
		delete encoder;
		delete decoder;

		/* Delete output file if it doesn't look sane.
		 */
		if (File(out_filename).GetFileSize() <= 0 || cancel) File(out_filename).Delete();

		/* Reset SndFile configuration after decoding.
		 */
		if (conversionStep == CONVERTER_STEP_DECODE && !boca.ComponentExists("wave-enc"))
		{
			config->SetIntValue("SndFile", "Format", sndFileFormat);
			config->SetIntValue("SndFile", "SubFormat", sndFileSubFormat);
		}

		/* Delete input file if requested or not in on-the-fly mode.
		 */
		if (conversionStep == CONVERTER_STEP_ENCODE)
		{
			if (!keepWaveFiles || cancel) File(in_filename).Delete();

			if (in_filename.EndsWith(".temp.wav")) in_filename[in_filename.Length() - 9] = 0;
		}
		else if (Config::Get()->deleteAfterEncoding && !cancel)
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

		/* Revert to waiting state when not encoding on-the-fly.
		 */
		if (conversionStep == CONVERTER_STEP_DECODE) waiting = True;

		/* Report finished conversion.
		 */
		onFinishTrack.Emit(trackToConvert);

		/* Update track length and offset.
		 */
		trackToConvert.sampleOffset = 0;
		trackToConvert.length	    = trackLength;

		/* Fix total samples value when not encoding on-the-fly.
		 */
		if (conversionStep == CONVERTER_STEP_DECODE) onFixTotalSamples.Emit(track, trackToConvert);
	}

	return Success();
}

Int64 BonkEnc::ConvertWorker::Loop(Decoder *decoder, Encoder *encoder)
{
	BoCA::Config	*config	= BoCA::Config::Get();

	/* Get config values.
	 */
	Int	 ripperTimeout	 = config->GetIntValue(Config::CategoryRipperID, Config::RipperTimeoutID, Config::RipperTimeoutDefault);

	/* Find system byte order.
	 */
	Int	 systemByteOrder = CPU().GetEndianness() == EndianLittle ? BYTE_INTEL : BYTE_RAW;

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

		/* Read samples from decoder.
		 */
		Int	 bytes = decoder->Read(buffer, step * bytesPerSample * format.channels);

		if	(bytes == -1) { cancel = True; break; }
		else if (bytes ==  0)		       break;

		/* Switch byte order to native.
		 */
		if (format.order != BYTE_NATIVE && format.order != systemByteOrder) BoCA::Utilities::SwitchBufferByteOrder(buffer, format.bits / 8);

		/* Pass samples to encoder.
		 */
		if (encoder->Write(buffer, bytes) == -1) { cancel = True; break; }

		/* Update length and position info.
		 */
		trackLength += (bytes / bytesPerSample / format.channels);

		if (trackToConvert.length >= 0) trackPosition += (bytes / bytesPerSample / format.channels);
		else				trackPosition = decoder->GetInBytes();

		/* Check for ripper timeout.
		 */
		if (trackToConvert.isCDTrack && ripperTimeout > 0 && S::System::System::Clock() - trackStartTicks > (UnsignedInt) ripperTimeout * 1000)
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
