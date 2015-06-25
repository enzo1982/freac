 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/engine/convertworker.h>

#include <engine/decoder.h>
#include <engine/encoder.h>

#include <config.h>

using namespace BoCA;
using namespace BoCA::AS;

BonkEnc::JobConvertWorker::JobConvertWorker()
{
	SetFlags(Threads::THREAD_WAITFLAG_START);

	trackToConvert	  = NIL;
	trackStartTicks	  = 0;
	trackPosition	  = 0;

	singleFileEncoder = NIL;
	encodedSamples	  = 0;

	conversionStep	  = CONVERTER_STEP_ON_THE_FLY;

	idle		  = True;
	waiting		  = True;

	pause		  = False;
	cancel		  = False;
	quit		  = False;

	threadMain.Connect(&JobConvertWorker::Perform, this);
}

BonkEnc::JobConvertWorker::~JobConvertWorker()
{
}

Int BonkEnc::JobConvertWorker::Perform()
{
	while (!quit)
	{
		if (idle) { S::System::System::Sleep(100); continue; }

		Convert(trackToConvert);

		idle	= True;
		waiting	= True;
		cancel	= False;
	}

	return Success();
}

Int BonkEnc::JobConvertWorker::Convert(const Track &iTrack)
{
	BoCA::Config	*config	= BoCA::Config::Get();
	Registry	&boca	= Registry::Get();

	/* Get config values.
	 */
	Bool	 encodeOnTheFly		= config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault);
	Bool	 keepWaveFiles		= config->GetIntValue(Config::CategorySettingsID, Config::SettingsKeepWaveFilesID, Config::SettingsKeepWaveFilesDefault);

	Bool	 writeToInputDirectory	= config->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault);
	Bool	 allowOverwriteSource	= config->GetIntValue(Config::CategorySettingsID, Config::SettingsAllowOverwriteSourceID, Config::SettingsAllowOverwriteSourceDefault);

	Int	 ripperTimeout		= config->GetIntValue(Config::CategoryRipperID, Config::RipperTimeoutID, Config::RipperTimeoutDefault);

#ifdef __APPLE__
	Int	 sndFileFormat		= config->GetIntValue("SndFile", "Format", 0x020000);
	Int	 sndFileSubFormat	= config->GetIntValue("SndFile", "SubFormat", 0x000000);
#else
	Int	 sndFileFormat		= config->GetIntValue("SndFile", "Format", 0x010000);
	Int	 sndFileSubFormat	= config->GetIntValue("SndFile", "SubFormat", 0x000000);
#endif

	/* Find system byte order.
	 */
	Int	 systemByteOrder	= CPU().GetEndianness() == EndianLittle ? BYTE_INTEL : BYTE_RAW;

	/* Find encoder to use.
	 */
	String	 selectedEncoderID	= config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);
	String	 activeEncoderID	= selectedEncoderID;

	/* We always convert on the fly when encoding to a
	 * single file or when outputting simple audio files.
	 */
	if (singleFileEncoder != NIL || selectedEncoderID == "wave-enc" ||
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
		Track	 track  = iTrack;
		Format	 format = track.GetFormat();

		String	 in_filename  = track.origFilename;
		String	 out_filename = track.outfile;

		if (out_filename.ToLower() == in_filename.ToLower()) out_filename.Append(".temp");

		track.outfile = out_filename;
		trackPosition = 0;

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

		if (!decoder->Create(in_filename, track))
		{
			delete decoder;

			return Error();
		}

		decoderName = decoder->GetDecoderName();

		/* Create encoder.
		 */
		Encoder	*encoder = singleFileEncoder;

		if (singleFileEncoder == NIL)
		{
			encoder = new Encoder();

			if (!encoder->Create(activeEncoderID, out_filename, track))
			{
				delete decoder;
				delete encoder;

				File(out_filename).Delete();

				return Error();
			}
		}

		log->Write(String("\tEncoding from: ").Append(in_filename));
		log->Write(String("\t         to:   ").Append(out_filename));

		/* Enter main conversion loop.
		 */
		Int64		 trackLength  = 0;
		UnsignedLong	 samples_size = 512;

		trackStartTicks = S::System::System::Clock();

		waiting		= False;

		Int			 bytesPerSample = format.bits / 8;
		Buffer<UnsignedByte>	 buffer(samples_size * bytesPerSample * format.channels);

		while (!cancel)
		{
			Int	 step = samples_size;

			if (track.length >= 0)
			{
				if (trackPosition >= track.length) break;

				if (trackPosition + step > track.length) step = track.length - trackPosition;
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

			if (track.length >= 0) trackPosition += (bytes / bytesPerSample / format.channels);
			else		       trackPosition = decoder->GetInBytes();

			/* Check for ripper timeout.
			 */
			if (track.isCDTrack && ripperTimeout > 0 && S::System::System::Clock() - trackStartTicks > (UnsignedInt) ripperTimeout * 1000)
			{
				BoCA::Utilities::WarningMessage("CD ripping timeout after %1 seconds. Skipping track.", String::FromInt(ripperTimeout));

				cancel = True;
			}

			/* Pause if requested.
			 */
			while (pause && !cancel) S::System::System::Sleep(50);
		}

		/* Free encoder and decoder.
		 */
		if (singleFileEncoder == NIL)
		{
			delete encoder;

			/* Delete output file if it doesn't look sane.
			 */
			if (File(out_filename).GetFileSize() <= 0 || cancel) File(out_filename).Delete();
		}

		delete decoder;

		if (cancel) log->WriteWarning(String("\tCancelled encoding: ").Append(in_filename));
		else	    log->Write(String("\tFinished encoding:" ).Append(in_filename));

		/* Signal next chapter.
		 */
		if (singleFileEncoder != NIL && !cancel)
		{
			singleFileEncoder->SignalChapterChange();
			encodedSamples += trackLength;
		}

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
		onFinishTrack.Emit(track);

		/* Update track length and offset.
		 */
		trackToConvert.sampleOffset = (singleFileEncoder == NIL ? 0 : Math::Round((Float) (encodedSamples - trackLength) / format.rate * 75));
		trackToConvert.length	    = trackLength;

		/* Fix total samples value when not encoding on-the-fly.
		 */
		if (conversionStep == CONVERTER_STEP_DECODE) onFixTotalSamples.Emit(track, trackToConvert);
	}

	return Error();
}

Int BonkEnc::JobConvertWorker::Pause(Bool value)
{
	pause = value;

	return Success();
}

Int BonkEnc::JobConvertWorker::Cancel()
{
	cancel = True;

	return Success();
}

Int BonkEnc::JobConvertWorker::Quit()
{
	cancel = True;
	quit   = True;

	return Success();
}
