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

#include <startconsole.h>
#include <joblist.h>
#include <config.h>

#include <engine/converter.h>

#include <jobs/engine/convert.h>
#include <jobs/joblist/addfiles.h>
#include <jobs/joblist/addtracks.h>

using namespace smooth::IO;

using namespace BoCA;
using namespace BoCA::AS;

Int StartConsole(const Array<String> &args)
{
	freac::freacCommandline::Get(args);
	freac::freacCommandline::Free();

	return 0;
}

freac::freacCommandline *freac::freacCommandline::Get(const Array<String> &args)
{
	if (instance == NIL) new freacCommandline(args);

	return (freacCommandline *) instance;
}

Void freac::freacCommandline::Free()
{
	if (instance != NIL) delete (freacCommandline *) instance;
}

freac::freacCommandline::freacCommandline(const Array<String> &arguments) : args(arguments)
{
	Registry	&boca	= Registry::Get();

	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	config->enable_console = True;

	/* Set interface language.
	 */
	String	 language = config->GetStringValue(Config::CategorySettingsID, Config::SettingsLanguageID, Config::SettingsLanguageDefault);

	if (language != NIL) i18n->ActivateLanguage(language);
	else		     i18n->SelectUserDefaultLanguage();

	/* Don't save configuration settings set via command line.
	 */
	config->SetSaveSettingsOnExit(False);

	/* List configurations if requested.
	 */
	if (ScanForProgramOption("--list-configs"))
	{
		Console::OutputString(String(freac::appLongName).Append(" ").Append(freac::version).Append(" (").Append(freac::architecture).Append(") command line interface\n").Append(freac::copyright).Append("\n\n"));
		Console::OutputString("Available configurations:\n\n");

		for (Int i = 0; i < config->GetNOfConfigurations(); i++)
		{
			if (i == 0) Console::OutputString(String("\t").Append(i18n->TranslateString("Default configuration", "Configuration")).Append("\n"));
			else	    Console::OutputString(String("\t").Append(config->GetNthConfigurationName(i)).Append("\n"));
		}

		Console::OutputString("\n");

		return;
	}

	/* Set active configuration.
	 */
	String		 configName;

	config->SetActiveConfiguration("default");

	if (ScanForProgramOption("--config=%VALUE", &configName) && configName != "default" && configName != "Default configuration" && configName != i18n->TranslateString("Default configuration", "Configuration"))
	{
		Bool	 foundConfig = False;

		for (Int i = 1; i < config->GetNOfConfigurations(); i++)
		{
			if (configName == config->GetNthConfigurationName(i)) { foundConfig = True; break; }
		}

		if (!foundConfig)
		{
			Console::OutputString(String("Error: No such configuration: ").Append(configName).Append("\n"));

			return;
		}

		config->SetActiveConfiguration(configName);
	}

	/* Configure the converter.
	 */
	Bool		 quiet		= ScanForProgramOption("--quiet");
	Bool		 cddb		= ScanForProgramOption("--cddb");
	Array<String>	 files;
	String		 encoderID	= "lame";
	String		 helpenc;
	String		 outdir		= Directory::GetActiveDirectory();
	String		 outfile;
	String		 pattern	= "<filename>";
	String		 cdDrive	= "0";
	String		 tracks;
	String		 timeout	= "120";

	if (!ScanForProgramOption("--encoder=%VALUE", &encoderID)) ScanForProgramOption("-e %VALUE", &encoderID);
	if (!ScanForProgramOption("--help=%VALUE",    &helpenc))   ScanForProgramOption("-h %VALUE", &helpenc);
								   ScanForProgramOption("-d %VALUE", &outdir);
								   ScanForProgramOption("-o %VALUE", &outfile);
	if (!ScanForProgramOption("--pattern=%VALUE", &pattern))   ScanForProgramOption("-p %VALUE", &pattern);

	DeviceInfoComponent	*info	   = boca.CreateDeviceInfoComponent();
	Int			 numDrives = 0;

	if (info != NIL)
	{
		if (info->GetNumberOfDevices() > 0)
		{
			if (!ScanForProgramOption("--drive=%VALUE", &cdDrive)) ScanForProgramOption("-cd %VALUE", &cdDrive);
			if (!ScanForProgramOption("--track=%VALUE", &tracks))  ScanForProgramOption("-t %VALUE",  &tracks);

			ScanForProgramOption("--timeout=%VALUE", &timeout);
		}

		numDrives = info->GetNumberOfDevices();

		boca.DeleteComponent(info);
	}

	ScanForFiles(&files);

	if (numDrives > 0)
	{
		config->SetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, cdDrive.ToInt());

		if (config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault) >= numDrives)
		{
			Console::OutputString(String("Warning: Drive #").Append(cdDrive).Append(" does not exist. Using first drive.\n"));

			config->SetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, 0);
		}

		if (!TracksToFiles(tracks, &files))
		{
			Console::OutputString("Error: Invalid track(s) specified after --track.\n");

			return;
		}
	}
	else if (tracks != NIL)
	{
		Console::OutputString("Error: CD ripping disabled!");
	}

	Console::SetTitle(String(freac::appName).Append(" ").Append(freac::version));

	encoderID = encoderID.ToLower();
	helpenc	  = helpenc.ToLower();

	if (files.Length() == 0 || helpenc != NIL)
	{
		ShowHelp(helpenc);

		return;
	}

	if (!boca.ComponentExists(String(encoderID).Append("-enc")))
	{
		Console::OutputString(String("Encoder '").Append(encoderID).Append("' is not supported by ").Append(freac::appName).Append("!\n\n"));

		return;
	}

	/* Check encoder parameters.
	 */
	if (encoderID == "lame")
	{
		String	 bitrate = "192";
		String	 quality = "5";
		String	 mode	 = "VBR";

		ScanForEncoderOption("-b %VALUE", &bitrate);
		ScanForEncoderOption("-q %VALUE", &quality);
		ScanForEncoderOption("-m %VALUE", &mode);

		config->SetIntValue("LAME", "Preset", 0);
		config->SetIntValue("LAME", "SetBitrate", True);

		config->SetIntValue("LAME", "Bitrate", Math::Max(0, Math::Min(320, (Int) bitrate.ToInt())));
		config->SetIntValue("LAME", "ABRBitrate", Math::Max(0, Math::Min(320, (Int) bitrate.ToInt())));
		config->SetIntValue("LAME", "VBRQuality", Math::Max(0, Math::Min(9, (Int) quality.ToInt())) * 10);

		if	(mode.ToUpper() == "VBR") config->SetIntValue("LAME", "VBRMode", 4);
		else if (mode.ToUpper() == "ABR") config->SetIntValue("LAME", "VBRMode", 3);
		else if (mode.ToUpper() == "CBR") config->SetIntValue("LAME", "VBRMode", 0);
	}
	else if (encoderID == "vorbis")
	{
		String	 bitrate = "192";
		String	 quality = "60";

		if (ScanForEncoderOption("-b %VALUE", &bitrate)) config->SetIntValue("Vorbis", "Mode", 1);
		else						 config->SetIntValue("Vorbis", "Mode", 0);

		config->SetIntValue("Vorbis", "Quality", Math::Max(0, Math::Min(100, (Int) quality.ToInt())));
		config->SetIntValue("Vorbis", "Bitrate", Math::Max(45, Math::Min(500, (Int) bitrate.ToInt())));
	}
	else if (encoderID == "bonk")
	{
		String	 quantization = "0.4";
		String	 predictor    = "32";
		String	 downsampling = "2";

		ScanForEncoderOption("-q %VALUE", &quantization);
		ScanForEncoderOption("-p %VALUE", &predictor);
		ScanForEncoderOption("-r %VALUE", &downsampling);

		config->SetIntValue("Bonk", "JointStereo", ScanForEncoderOption("-js"));
		config->SetIntValue("Bonk", "Lossless", ScanForEncoderOption("-lossless"));

		config->SetIntValue("Bonk", "Quantization", Math::Max(0, Math::Min(40, (Int) Math::Round(quantization.ToFloat() * 20))));
		config->SetIntValue("Bonk", "Predictor", Math::Max(0, Math::Min(512, (Int) predictor.ToInt())));
		config->SetIntValue("Bonk", "Downsampling", Math::Max(0, Math::Min(10, (Int) downsampling.ToInt())));
	}
	else if (encoderID == "bladeenc")
	{
		String	 bitrate = "192";

		ScanForEncoderOption("-b %VALUE", &bitrate);

		config->SetIntValue("BladeEnc", "Bitrate", Math::Max(32, Math::Min(320, (Int) bitrate.ToInt())));
	}
	else if (encoderID == "faac")
	{
		String	 bitrate = "64";
		String	 quality = "100";

		if (ScanForEncoderOption("-b %VALUE", &bitrate)) config->SetIntValue("FAAC", "SetQuality", False);
		else						 config->SetIntValue("FAAC", "SetQuality", True);

		config->SetIntValue("FAAC", "MP4Container", ScanForEncoderOption("-mp4"));

		config->SetIntValue("FAAC", "AACQuality", Math::Max(10, Math::Min(500, (Int) quality.ToInt())));
		config->SetIntValue("FAAC", "Bitrate", Math::Max(8, Math::Min(256, (Int) bitrate.ToInt())));
	}
	else if (encoderID == "flac")
	{
		String	 blocksize = "4608";
		String	 lpc = "8";
		String	 qlp = "0";
		String	 rice = "3,3";
		String	 minrice;
		String	 maxrice;

		ScanForEncoderOption("-b %VALUE", &blocksize);
		ScanForEncoderOption("-l %VALUE", &lpc);
		ScanForEncoderOption("-q %VALUE", &qlp);
		ScanForEncoderOption("-r %VALUE", &rice);

		Int	 i = 0;
		Int	 j = 0;

		for (i = 0; i < rice.Length(); i++)	{ if (rice[i] == ',') break; minrice[i] = rice[i]; }
		for (j = i + 1; j < rice.Length(); j++)	{ maxrice[j - i - 1] = rice[j]; }

		config->SetIntValue("FLAC", "Preset", -1);

		config->SetIntValue("FLAC", "DoMidSideStereo", ScanForEncoderOption("-ms"));
		config->SetIntValue("FLAC", "DoExhaustiveModelSearch", ScanForEncoderOption("-e"));
		config->SetIntValue("FLAC", "DoQLPCoeffPrecSearch", ScanForEncoderOption("-p"));

		config->SetIntValue("FLAC", "Blocksize", Math::Max(192, Math::Min(32768, (Int) blocksize.ToInt())));
		config->SetIntValue("FLAC", "MaxLPCOrder", Math::Max(0, Math::Min(32, (Int) lpc.ToInt())));
		config->SetIntValue("FLAC", "QLPCoeffPrecision", Math::Max(0, Math::Min(16, (Int) qlp.ToInt())));
		config->SetIntValue("FLAC", "MinResidualPartitionOrder", Math::Max(0, Math::Min(16, (Int) minrice.ToInt())));
		config->SetIntValue("FLAC", "MaxResidualPartitionOrder", Math::Max(0, Math::Min(16, (Int) maxrice.ToInt())));
	}
	else if (encoderID == "twinvq")
	{
		String	 bitrate    = "48";
		String	 candidates = "32";

		ScanForEncoderOption("-b %VALUE", &bitrate);
		ScanForEncoderOption("-c %VALUE", &candidates);

		config->SetIntValue("TwinVQ", "PreselectionCandidates", Math::Max(4, Math::Min(32, (Int) candidates.ToInt())));
		config->SetIntValue("TwinVQ", "Bitrate", Math::Max(24, Math::Min(48, (Int) bitrate.ToInt())));
	}

	/* Check dynamic encoder parameters.
	 */
	Component	*component = boca.CreateComponentByID(String(encoderID).Append("-enc"));

	if (component == NIL)
	{
		Console::OutputString(String("Encoder '").Append(helpenc).Append("' could not be initialized!\n\n"));

		return;
	}

	Bool				 broken	    = False;
	const Array<Parameter *>	&parameters = component->GetParameters();

	foreach (Parameter *parameter, parameters)
	{
		ParameterType		 type	 = parameter->GetType();
		String			 name	 = parameter->GetName();
		String			 spec	 = parameter->GetArgument();
		const Array<Option *>	&options = parameter->GetOptions();
		Float			 step	 = parameter->GetStepSize();
		String			 def	 = parameter->GetDefault();

		if (type == PARAMETER_TYPE_SWITCH)
		{
			/* Enable switch if given on command line.
			 */
			config->SetIntValue(component->GetID(), name, ScanForEncoderOption(spec));
		}
		else if (type == PARAMETER_TYPE_SELECTION)
		{
			/* Set selection to given value.
			 */
			String	 value;
			Bool	 present = ScanForEncoderOption(String(spec).Trim(), &value);

			config->SetIntValue(component->GetID(), String("Set ").Append(name), present);
			config->SetStringValue(component->GetID(), name, value);

			/* Check if given value is allowed.
			 */
			Bool	 found = False;

			foreach (Option *option, options) if (option->GetValue() == value) found = True;

			if (present && !found) broken = True;
		}
		else if (type == PARAMETER_TYPE_RANGE)
		{
			/* Set range parameter to given value.
			 */
			String	 value;
			Bool	 present = ScanForEncoderOption(String(spec).Trim(), &value);

			config->SetIntValue(component->GetID(), String("Set ").Append(name), present);
			config->SetIntValue(component->GetID(), name, Math::Round(value.ToFloat() / step));

			/* Check if given value is valid.
			 */
			Bool	 valid = True;

			foreach (Option *option, options) if (option->GetType() == OPTION_TYPE_MIN && value.ToFloat() < option->GetValue().ToFloat()) valid = False;
			foreach (Option *option, options) if (option->GetType() == OPTION_TYPE_MAX && value.ToFloat() > option->GetValue().ToFloat()) valid = False;

			if (present && !valid) broken = True;
		}
	}

	boca.DeleteComponent(component);

	if (broken)
	{
		Console::OutputString(String("Invalid arguments for encoder '").Append(helpenc).Append("'!\n\n"));

		return;
	}

	/* Perform actual conversion.
	 */
	if (!outdir.EndsWith(Directory::GetDirectoryDelimiter())) outdir.Append(Directory::GetDirectoryDelimiter());

	config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, String(encoderID).Append("-enc"));

	config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, outdir);
	config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, pattern);

	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbAutoQueryID, cddb);
	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbAutoSelectID, True);
	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbEnableCacheID, True);

	config->SetIntValue(Config::CategoryRipperID, Config::RipperLockTrayID, Config::RipperLockTrayDefault);
	config->SetIntValue(Config::CategoryRipperID, Config::RipperTimeoutID, Config::RipperTimeoutDefault);

	config->SetIntValue(Config::CategoryTagsID, Config::TagsReadChaptersID, False);

	config->SetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreatePlaylistID, False);
	config->SetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreateCueSheetID, False);

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, False);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, False);

	if (files.Length() > 1 && outfile != NIL)
	{
		config->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, True);
		config->SetStringValue(Config::CategorySettingsID, Config::SettingsSingleFilenameID, outfile);

		JobConvert::onEncodeTrack.Connect(&freacCommandline::OnEncodeTrack, this);

		Array<String>	 jobFiles;
		Bool		 addCDTracks = False;

		foreach (const String &file, files)
		{
			if (file.StartsWith("device://cdda:")) addCDTracks = True;

			InStream	 in(STREAM_FILE, file, IS_READ);

			if (in.GetLastError() != IO_ERROR_OK && !file.StartsWith("device://"))
			{
				Console::OutputString(String("File not found: ").Append(file).Append("\n"));

				continue;
			}

			jobFiles.Add(file);
		}

		JobList	*joblist = new JobList(Point(0, 0), Size(0, 0));
		Job	*job	 = addCDTracks ? (Job *) new JobAddTracks(jobFiles) : (Job *) new JobAddFiles(jobFiles);

		job->Schedule();

		while (Job::GetScheduledJobs().Length()	> 0) S::System::System::Sleep(10);
		while (Job::GetPlannedJobs().Length()	> 0) S::System::System::Sleep(10);
		while (Job::GetRunningJobs().Length()	> 0) S::System::System::Sleep(10);

		if (joblist->GetNOfTracks() > 0)
		{
			Converter().Convert(joblist, False);

			if (!quiet) Console::OutputString("done.\n");
		}
		else
		{
			Console::OutputString("Could not process input files!\n");
		}

		delete joblist;
	}
	else
	{
		JobList	*joblist = new JobList(Point(0, 0), Size(0, 0));

		foreach (const String &file, files)
		{
			String	 currentFile = file;
			Bool	 addCDTrack  = False;

			if (currentFile.StartsWith("device://cdda:"))
			{
				currentFile = String("Audio CD ").Append(String::FromInt(config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault))).Append(" - Track ").Append(currentFile.Tail(currentFile.Length() - 16));
				addCDTrack  = True;
			}

			InStream	 in(STREAM_FILE, file, IS_READ);

			if (in.GetLastError() != IO_ERROR_OK && !file.StartsWith("device://"))
			{
				Console::OutputString(String("File not found: ").Append(file).Append("\n"));

				continue;
			}

			Array<String>	 jobFiles;

			jobFiles.Add(file);

			Job	*job = addCDTrack ? (Job *) new JobAddTracks(jobFiles) : (Job *) new JobAddFiles(jobFiles);

			job->Schedule();

			while (Job::GetScheduledJobs().Length()	> 0) S::System::System::Sleep(10);
			while (Job::GetPlannedJobs().Length()	> 0) S::System::System::Sleep(10);
			while (Job::GetRunningJobs().Length()	> 0) S::System::System::Sleep(10);

			if (joblist->GetNOfTracks() > 0)
			{
				if (!quiet) Console::OutputString(String("Processing file: ").Append(currentFile).Append("..."));

				Track	 track = joblist->GetNthTrack(0);

				track.outfile = outfile;

				joblist->UpdateTrackInfo(track);

				Converter().Convert(joblist, False);

				joblist->RemoveNthTrack(0);

				if (!quiet) Console::OutputString("done.\n");
			}
			else
			{
				Console::OutputString(String("Could not process file: ").Append(currentFile).Append("\n"));

				continue;
			}
		}

		delete joblist;
	}
}

freac::freacCommandline::~freacCommandline()
{
}

Void freac::freacCommandline::OnEncodeTrack(const Track &track, const String &decoderName, ConversionStep mode)
{
	static Bool	 firstTime = True;

	BoCA::Config	*config = BoCA::Config::Get();
	Bool		 quiet	= ScanForProgramOption("--quiet");

	if (!quiet)
	{
		if (!firstTime) Console::OutputString("done.\n");
		else		firstTime = False;

		String	 currentFile = track.origFilename;

		if (currentFile.StartsWith("device://cdda:")) currentFile = String("Audio CD ").Append(String::FromInt(config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault))).Append(" - Track ").Append(currentFile.Tail(currentFile.Length() - 16));

		Console::OutputString(String("Processing file: ").Append(currentFile).Append("..."));
	}
}

Bool freac::freacCommandline::ScanForProgramOption(const String &option, String *value)
{
	/* Scan all arguments.
	 */
	foreach (const String &arg, args)
	{
		/* Stop upon encountering separator.
		 */
		if (arg == "--") break;

		if (option.StartsWith("--") && option.EndsWith("=%VALUE") && value != NIL && arg.StartsWith(option.Head(option.Find("=") + 1)))
		{
			*value = arg.Tail(arg.Length() - option.Length() + 6);

			return True;
		}
		else if (option.StartsWith("--") && arg == option)
		{
			return True;
		}
		else if (option.StartsWith("-") && option.EndsWith("%VALUE") && !option.Contains(" ") && value != NIL && arg.StartsWith(option.Head(option.Find("%"))))
		{
			*value = arg.Tail(arg.Length() - option.Length() + 6);

			return True;
		}
		else if (option.StartsWith("-") && option.EndsWith(" %VALUE") && value != NIL && arg == option.Head(option.Find(" ")))
		{
			*value = args.GetNth(foreachindex + 1);

			return True;
		}
		else if (option.StartsWith("-") && arg == option)
		{
			return True;
		}
	}

	return False;
}

Bool freac::freacCommandline::ScanForEncoderOption(const String &option, String *value)
{
	/* Check if we have a separator for program and encoder options.
	 */
	Int	 separatorindex = -1;

	foreach (const String &arg, args)
	{
		if (arg != "--") continue;

		separatorindex = foreachindex;

		break;
	}

	/* Scan all arguments.
	 */
	foreach (const String &arg, args)
	{
		/* Discard arguments before separator.
		 */
		if (foreachindex <= separatorindex) continue;

		if (option.StartsWith("--") && option.EndsWith("=%VALUE") && value != NIL && arg.StartsWith(option.Head(option.Find("=") + 1)))
		{
			*value = arg.Tail(arg.Length() - option.Length() + 6);

			return True;
		}
		else if (option.StartsWith("--") && arg == option)
		{
			return True;
		}
		else if (option.StartsWith("-") && option.EndsWith("%VALUE") && !option.Contains(" ") && value != NIL && arg.StartsWith(option.Head(option.Find("%"))))
		{
			*value = arg.Tail(arg.Length() - option.Length() + 6);

			return True;
		}
		else if (option.StartsWith("-") && option.EndsWith(" %VALUE") && value != NIL && arg == option.Head(option.Find(" ")))
		{
			*value = args.GetNth(foreachindex + 1);

			return True;
		}
		else if (option.StartsWith("-") && arg == option)
		{
			return True;
		}
	}

	return False;
}

Void freac::freacCommandline::ScanForFiles(Array<String> *files)
{
	String	 prevArg;

	foreach (const String &arg, args)
	{
		if (arg[0] != '-' && (prevArg.StartsWith("--")  ||
				      prevArg[0] != '-'		||
				      prevArg    == "-js"	||
				      prevArg    == "-lossless" ||
				      prevArg    == "-mp4"	||
				      prevArg    == "-ms"	||
				      prevArg    == "-extc"	||
				      prevArg    == "-extm"))
		{
			if (arg.Contains("*") || arg.Contains("?"))
			{
				File			 file(arg);
				Directory		 dir(file.GetFilePath());
				const Array<File>	&array = dir.GetFilesByPattern(file.GetFileName());

				foreach (const File &entry, array) (*files).Add(entry);
			}
			else
			{
				(*files).Add(arg);
			}
		}

		prevArg = arg;
	}
}

Bool freac::freacCommandline::TracksToFiles(const String &tracks, Array<String> *files)
{
	BoCA::Config	*config	= BoCA::Config::Get();

	if (tracks == "all")
	{
		Registry		&boca = Registry::Get();
		DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

		if (info != NIL)
		{
			const Array<String>	&tracks = info->GetNthDeviceTrackList(config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault));

			foreach (const String &track, tracks) (*files).Add(track);

			boca.DeleteComponent(info);
		}

		return True;
	}

	for (Int i = 0; i < tracks.Length(); i++)
	{
		if ((tracks[i] >= 'a' && tracks[i] <= 'z') ||
		    (tracks[i] >= 'A' && tracks[i] <= 'Z')) return False;
	}

	String	 rest = tracks;

	while (rest.Length() > 0)
	{
		String	 current;

		if (rest.Contains(","))
		{
			Int	 comma = rest.Find(",");

			current = rest.Head(comma);
			rest = rest.Tail(rest.Length() - comma - 1);
		}
		else
		{
			current = rest;
			rest = NIL;
		}

		if (current.Contains("-"))
		{
			Int	 dash  = current.Find("-");
			Int	 first = current.Head(dash).ToInt();
			Int	 last  = current.Tail(current.Length() - dash - 1).ToInt();

			for (Int i = first; i <= last; i++) (*files).Add(String("device://cdda:")
									.Append(String::FromInt(config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault)))
									.Append("/")
									.Append(String::FromInt(i)));
		}
		else
		{
			(*files).Add(String("device://cdda:")
				    .Append(String::FromInt(config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault)))
				    .Append("/")
				    .Append(current));
		}
	}

	return True;
}

Void freac::freacCommandline::ShowHelp(const String &helpenc)
{
	Console::OutputString(String(freac::appLongName).Append(" ").Append(freac::version).Append(" (").Append(freac::architecture).Append(") command line interface\n").Append(freac::copyright).Append("\n\n"));

	Registry	&boca = Registry::Get();

	if (helpenc == NIL)
	{
		Console::OutputString("Usage:\tfreaccmd [options] [--] [encoder options] [file(s)]\n\n");
		Console::OutputString("  --encoder=<id>  | -e <id>\tSpecify the encoder to use (default is LAME)\n");
		Console::OutputString("  --help=<id>     | -h <id>\tPrint help for encoder specific options\n\n");
		Console::OutputString("                    -d <dir>\tSpecify output directory for encoded files\n");
		Console::OutputString("                    -o <file>\tSpecify output file name in single file mode\n");
		Console::OutputString("  --pattern=<pat> | -p <pat>\tSpecify output file name pattern\n\n");

		DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

		if (info != NIL)
		{
			if (info->GetNumberOfDevices() > 0)
			{
				Console::OutputString("  --drive=<n>     | -cd <n>\tSpecify active CD drive (0..n)\n");
				Console::OutputString("  --track=<n>     | -t <n>\tSpecify input track(s) to rip (e.g. 1-5,7,9 or 'all')\n");
				Console::OutputString("  --timeout=<s>\t\t\tTimeout for CD track ripping (default is 120 seconds)\n");
				Console::OutputString("  --cddb\t\t\tEnable CDDB database lookup\n\n");
			}

			boca.DeleteComponent(info);
		}

		Console::OutputString("  --list-configs\t\tPrint a list of available configurations\n");
		Console::OutputString("  --config=<cfg>\t\tSpecify configuration to use\n\n");
		Console::OutputString("  --quiet\t\t\tDo not print any messages\n\n");
		Console::OutputString("Use -- to separate freaccmd options from encoder options if both have the same name.\n\n");
		Console::OutputString("Encoder <id> can be one of:\n\n");

		String	 list;
		Bool	 first = True;

		for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
		{
			if (boca.GetComponentType(i) != COMPONENT_TYPE_ENCODER) continue;

			if (list.Length() + boca.GetComponentID(i).Length() > 80)
			{
				Console::OutputString(list.Append(",\n"));

				list  = NIL;
				first = True;
			}

			if (first) list.Append("\t");
			else	   list.Append(", ");

			first = False;

			list.Append(boca.GetComponentID(i).Head(boca.GetComponentID(i).FindLast("-enc")));
		}

		Console::OutputString(list.Append("\n\n"));
		Console::OutputString("Default for <pat> is \"<filename>\".\n");
	}
	else
	{
		if (!boca.ComponentExists(String(helpenc).Append("-enc")))
		{
			Console::OutputString(String("Encoder '").Append(helpenc).Append("' is not supported by ").Append(freac::appName).Append("!\n\n"));

			return;
		}

		Component	*component = boca.CreateComponentByID(String(helpenc).Append("-enc"));

		if (component == NIL)
		{
			Console::OutputString(String("Encoder '").Append(helpenc).Append("' could not be initialized!\n\n"));

			return;
		}

		Console::OutputString(String("Options for ").Append(component->GetName()).Append(":\n\n"));

		if (component->GetParameters().Length() > 0)
		{
			const Array<Parameter *>	&parameters = component->GetParameters();

			/* Get required number of tabs for formatting.
			 */
			Int	 maxTabs = 0;

			foreach (Parameter *parameter, parameters)
			{
				String	 spec = parameter->GetArgument().Replace("%VALUE", "<val>");

				maxTabs = Math::Max((Int64) maxTabs, Math::Ceil((spec.Length() + 1) / 8.0));
			}

			/* Print formatted parameter list.
			 */
			foreach (Parameter *parameter, parameters)
			{
				ParameterType		 type	 = parameter->GetType();
				String			 name	 = parameter->GetName();
				String			 spec	 = parameter->GetArgument().Replace("%VALUE", "<val>");
				const Array<Option *>	&options = parameter->GetOptions();
				String			 def	 = parameter->GetDefault();

				if (type == PARAMETER_TYPE_SWITCH)
				{
					Console::OutputString(String("\t").Append(spec).Append(String().FillN('\t', maxTabs - Math::Floor(spec.Length() / 8.0))).Append(name).Append("\n"));
				}
				else if (type == PARAMETER_TYPE_SELECTION)
				{
					Console::OutputString(String("\t").Append(spec).Append(String().FillN('\t', maxTabs - Math::Floor(spec.Length() / 8.0))).Append(name).Append(": "));

					{
						foreach (Option *option, options)
						{
							if (foreachindex > 0) Console::OutputString(String().FillN('\t', maxTabs + 1).Append(String().FillN(' ', name.Length() + 2)));

							Console::OutputString(option->GetValue().Append(option->GetAlias() != option->GetValue() ? String(" (").Append(option->GetAlias()).Append(")") : String()).Append(def == option->GetValue() ? ", default" : NIL).Append("\n"));
						}
					}

					if (foreachindex < parameters.Length() - 1) Console::OutputString("\n");
				}
				else if (type == PARAMETER_TYPE_RANGE)
				{
					Console::OutputString(String("\t").Append(spec).Append(String().FillN('\t', maxTabs - Math::Floor(spec.Length() / 8.0))).Append(name).Append(": "));

					foreach (Option *option, options) if (option->GetType() == OPTION_TYPE_MIN) Console::OutputString(String(option->GetValue()).Append(option->GetAlias() != option->GetValue() ? String(" (").Append(option->GetAlias()).Append(")") : String()).Append(" - "));
					foreach (Option *option, options) if (option->GetType() == OPTION_TYPE_MAX) Console::OutputString(String(option->GetValue()).Append(option->GetAlias() != option->GetValue() ? String(" (").Append(option->GetAlias()).Append(")") : String()).Append(", "));

					Console::OutputString(String("default ").Append(def).Append("\n"));
				}
			}
		}
		else
		{
			Console::OutputString(String("\tno options for ").Append(component->GetName()).Append("\n\n"));
		}

		boca.DeleteComponent(component);
	}
}
