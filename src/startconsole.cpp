 /* fre:ac - free audio converter
  * Copyright (C) 2001-2021 Robert Kausch <robert.kausch@freac.org>
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
#include <utilities.h>
#include <config.h>

#include <engine/converter.h>

#include <jobs/engine/convert.h>
#include <jobs/joblist/addfiles.h>
#include <jobs/joblist/addtracks.h>

#ifdef __WIN32__
#	include <windows.h>
#else
#	include <unistd.h>
#	include <signal.h>
#	include <limits.h>

#	ifndef PATH_MAX
#		define PATH_MAX 32768
#	endif
#endif

using namespace smooth::IO;

using namespace BoCA;
using namespace BoCA::AS;

#ifdef __WIN32__
static BOOL WINAPI ConsoleCtrlHandler(DWORD ctrlType)
{
	if (ctrlType != CTRL_C_EVENT	  && ctrlType != CTRL_CLOSE_EVENT &&
	    ctrlType != CTRL_LOGOFF_EVENT && ctrlType != CTRL_SHUTDOWN_EVENT) return FALSE;

	freac::freacCommandline::Get()->Stop();

	return TRUE;
}
#else
static void ConsoleSignalHandler(int signal)
{
	freac::freacCommandline::Get()->Stop();
}
#endif

Int StartConsole(const Array<String> &args)
{
#ifdef __WIN32__
	SetConsoleCtrlHandler(&ConsoleCtrlHandler, TRUE);
#else
	signal(SIGINT, &ConsoleSignalHandler);
	signal(SIGTERM, &ConsoleSignalHandler);
#endif

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

freac::freacCommandline::freacCommandline(const Array<String> &arguments) : args(arguments), stopped(False)
{
	Registry	&boca	= Registry::Get();

	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

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

#ifndef __WIN32__
		Console::OutputString("\n");
#endif

		return;
	}

	/* Find configuration to use.
	 */
	String		 configName = "default";
	Bool		 userConfig = ScanForProgramOption("--config=%VALUE", &configName);

	if (configName ==			"Default configuration"			  ||
	    configName == i18n->TranslateString("Default configuration", "Configuration")) configName = "default";

	/* Check whether configuration exists.
	 */
	if (configName != "default")
	{
		Bool	 foundConfig = False;

		for (Int i = 1; i < config->GetNOfConfigurations() && !foundConfig; i++)
		{
			if (configName == config->GetNthConfigurationName(i)) foundConfig = True;
		}

		if (!foundConfig)
		{
			Console::OutputString(String("Error: No such configuration: ").Append(configName).Append("\n"));

			return;
		}
	}

	/* Activate configuration.
	 */
	config->SetActiveConfiguration(configName);

	SetConfigDefaults(config, userConfig);

	/* Activate English language.
	 */
	i18n->ActivateLanguage("internal");

	/* Configure the converter.
	 */
	String		 helpenc;

	Array<String>	 files;
	String		 pattern;
	String		 outputFile;
	String		 outputFolder	 = Directory::GetActiveDirectory();

	Bool		 enableSuperFast = ScanForProgramOption("--superfast");
	String		 numberOfThreads;

	String		 cdDrive	 = "0";
	String		 tracks;
	String		 ripperTimeout;
	Bool		 enableCDDB	 = ScanForProgramOption("--cddb");

	Bool		 splitChapters	 = ScanForProgramOption("--split-chapters");

	Bool		 ignoreChapters	 = ScanForProgramOption("--ignore-chapters");
	Bool		 ignoreCoverArt	 = ScanForProgramOption("--ignore-coverart");

	Bool		 quiet		 = ScanForProgramOption("--quiet");

	encoderID = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);
	encoderID = encoderID.Head(encoderID.Length() - 4);

	if (!ScanForProgramOption("--encoder=%VALUE", &encoderID)) ScanForProgramOption("-e %VALUE", &encoderID);
	if (!ScanForProgramOption("--help=%VALUE",    &helpenc))   ScanForProgramOption("-h %VALUE", &helpenc);
								   ScanForProgramOption("-d %VALUE", &outputFolder);
								   ScanForProgramOption("-o %VALUE", &outputFile);
	if (!ScanForProgramOption("--pattern=%VALUE", &pattern))   ScanForProgramOption("-p %VALUE", &pattern);

	ScanForProgramOption("--threads=%VALUE", &numberOfThreads);

	encoderID = encoderID.ToLower();
	helpenc	  = helpenc.ToLower();

	DeviceInfoComponent	*info	   = boca.CreateDeviceInfoComponent();
	Int			 numDrives = 0;

	if (info != NIL)
	{
		numDrives = info->GetNumberOfDevices();

		/* List drives if requested.
		 */
		if (ScanForProgramOption("--list-drives"))
		{
			Console::OutputString(String(freac::appLongName).Append(" ").Append(freac::version).Append(" (").Append(freac::architecture).Append(") command line interface\n").Append(freac::copyright).Append("\n\n"));
			Console::OutputString("Available CD drives:\n\n");
			Console::OutputString("\tNumber\tDevice path\tDesignation\n");
			Console::OutputString("\t------------------------------------------------\n");

			for (Int i = 0; i < numDrives; i++)
			{
				const Device	&device = info->GetNthDeviceInfo(i);

				Console::OutputString(String("\t").Append(String::FromInt(i)).Append("\t").Append(device.path).Append(device.path.Length() < 8 ? "\t\t" : "\t").Append(String(device.vendor).Append(" ").Append(device.model).Append(" ").Append(device.revision).Trim()).Append("\n"));
			}

#ifndef __WIN32__
			Console::OutputString("\n");
#endif

			boca.DeleteComponent(info);

			return;
		}

		/* Scan for CD ripping options.
		 */
		if (numDrives > 0)
		{
			if (!ScanForProgramOption("--drive=%VALUE", &cdDrive)) ScanForProgramOption("-cd %VALUE", &cdDrive);
			if (!ScanForProgramOption("--track=%VALUE", &tracks))  ScanForProgramOption("-t %VALUE",  &tracks);

			ScanForProgramOption("--timeout=%VALUE", &ripperTimeout);

#ifndef __WIN32__
			/* Resolve links to devices.
			 */
			Buffer<char>	 buffer(PATH_MAX + 1);

			buffer.Zero();

			while (readlink(cdDrive, buffer, buffer.Size() - 1) != -1)
			{
				if (buffer[0] == '/') cdDrive = buffer;
				else		      cdDrive = File(cdDrive).GetFilePath().Append("/").Append(String(buffer));

				buffer.Zero();
			}

			cdDrive = File(cdDrive);
#endif

			/* Find drive number for path.
			 */
			for (Int i = 0; i < numDrives; i++)
			{
				const Device	&device = info->GetNthDeviceInfo(i);

				if (device.path != cdDrive) continue;

				cdDrive = String::FromInt(i);

				break;
			}
		}

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
	else if (tracks != NIL || ScanForProgramOption("--list-drives"))
	{
		Console::OutputString("Error: CD ripping disabled!");
	}

	Console::SetTitle(String(freac::appName).Append(" ").Append(freac::version));

	if ((files.Length() == 0 && !ScanForProgramOption("--eject")) || helpenc != NIL)
	{
		ShowHelp(helpenc);

		return;
	}

	if (!boca.ComponentExists(String(encoderID).Append("-enc")))
	{
		Console::OutputString(String("Encoder '").Append(encoderID).Append("' is not supported by ").Append(freac::appName).Append("!\n"));

		return;
	}

	/* Evaluate dynamic encoder parameters.
	 */
	if (!SetEncoderDefaults(config, userConfig)) return;

	/* Set config values.
	 */
				    config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, String(encoderID).Append("-enc"));

				    config->SetStringValue(Config::CategorySettingsID, Config::SettingsSingleFilenameID, outputFile);
	if (pattern	    != NIL) config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, pattern);
				    config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Directory(outputFolder));

	if (enableSuperFast	  ) config->SetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableSuperFastModeID, True);
	if (numberOfThreads != NIL) config->SetIntValue(Config::CategoryResourcesID, Config::ResourcesNumberOfConversionThreadsID, numberOfThreads.ToInt());

	if (enableCDDB		  ) config->SetIntValue(Config::CategoryFreedbID, Config::FreedbAutoQueryID, True);

	if (ripperTimeout   != NIL) config->SetIntValue(Config::CategoryRipperID, Config::RipperTimeoutID, ripperTimeout.ToInt ());

	if (ignoreChapters)
	{
		config->SetIntValue(Config::CategoryTagsID, Config::TagsReadChaptersID, False);
		config->SetIntValue(Config::CategoryTagsID, Config::TagsWriteChaptersID, False);

		config->SetIntValue(Config::CategoryTagsID, Config::TagsReadEmbeddedCueSheetsID, False);
	}

	if (ignoreCoverArt)
	{
		config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtReadFromTagsID, False);
		config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToTagsID, False);
	}

	/* Perform actual conversion.
	 */
	if (files.Length() > 1 && outputFile != NIL)
	{
		JobConvert::onEncodeTrack.Connect(&freacCommandline::OnEncodeTrack, this);

		/* Check if input files exist.
		 */
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

		/* Add files to joblist.
		 */
		JobList	*joblist = new JobList(Point(0, 0), Size(0, 0));
		Job	*job	 = addCDTracks ? (Job *) new JobAddTracks(jobFiles) : (Job *) new JobAddFiles(jobFiles);

		job->Schedule();

		while (Job::GetScheduledJobs().Length()	> 0) S::System::System::Sleep(10);
		while (Job::GetPlannedJobs().Length()	> 0) S::System::System::Sleep(10);
		while (Job::GetRunningJobs().Length()	> 0) S::System::System::Sleep(10);

		if (joblist->GetNOfTracks() > 0)
		{
			/* Convert tracks in joblist.
			 */
			Converter().Convert(*joblist->GetTrackList(), False, False);

			if (!quiet)
			{
				if (!stopped) Console::OutputString("done.\n");
				else	      Console::OutputString("aborted.\n");
			}
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

			/* Check if input file exists.
			 */
			InStream	 in(STREAM_FILE, file, IS_READ);

			if (in.GetLastError() != IO_ERROR_OK && !file.StartsWith("device://"))
			{
				Console::OutputString(String("File not found: ").Append(file).Append("\n"));

				continue;
			}

			in.Close();

			/* Handle splitting chapters and cue sheets.
			 */
			Bool	 splitFile = (splitChapters || currentFile.EndsWith(".cue")) && outputFile == NIL;
			String	 pattern   = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, "<filename>");

			config->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, !splitFile);

			if (splitFile && pattern == "<filename>") config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, "<filename> - <track> - <title>");
			else					  config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, pattern);

			/* Add file to joblist.
			 */
			Array<String>	 jobFiles;

			jobFiles.Add(file);

			Job	*job = addCDTrack ? (Job *) new JobAddTracks(jobFiles) : (Job *) new JobAddFiles(jobFiles);

			job->Schedule();

			while (Job::GetScheduledJobs().Length()	> 0) S::System::System::Sleep(10);
			while (Job::GetPlannedJobs().Length()	> 0) S::System::System::Sleep(10);
			while (Job::GetRunningJobs().Length()	> 0) S::System::System::Sleep(10);

			if (joblist->GetNOfTracks() == 0)
			{
				Console::OutputString(String("Could not process file: ").Append(currentFile).Append("\n"));

				continue;
			}

			/* Set output file name.
			 */
			if (outputFile == NIL)
			{
				Track	 track = joblist->GetNthTrack(0);

				config->SetStringValue(Config::CategorySettingsID, Config::SettingsSingleFilenameID, Utilities::GetOutputFileName(config, track));
			}

			/* Convert tracks in joblist.
			 */
			if (!quiet) Console::OutputString(String("Processing file: ").Append(currentFile).Append("..."));

			Converter().Convert(*joblist->GetTrackList(), False, False);

			joblist->RemoveAllTracks();

			if (!quiet)
			{
				if (!stopped) Console::OutputString("done.\n");
				else	      Console::OutputString("aborted.\n");
			}

			if (stopped) break;
		}

		delete joblist;
	}

	/* Eject disc if requested.
	 */
	if (ScanForProgramOption("--eject") && !stopped)
	{
		DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

		if (info != NIL)
		{
			info->OpenNthDeviceTray(cdDrive.ToInt() < numDrives ? cdDrive.ToInt() : 0);

			boca.DeleteComponent(info);
		}
	}

	/* Wait for jobs to finish.
	 */
	const Array<Job *>	&jobs = Job::GetRunningJobs();

	while (jobs.Length() > 0) S::System::System::Sleep(10);
}

freac::freacCommandline::~freacCommandline()
{
}

Void freac::freacCommandline::OnEncodeTrack(const Track &track, const String &decoderName, const String &encoderName, ConversionStep mode)
{
	static Bool	 firstTime = True;

	BoCA::Config	*config = BoCA::Config::Get();
	Bool		 quiet	= ScanForProgramOption("--quiet");

	if (!quiet)
	{
		if (!firstTime) Console::OutputString("done.\n");
		else		firstTime = False;

		String	 currentFile = track.fileName;

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

		if (option.StartsWith("-") && option.EndsWith("%VALUE") && !option.Contains(" ") && value != NIL && arg.StartsWith(option.Head(option.Find("%"))))
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

		if (option.StartsWith("-") && option.EndsWith("%VALUE") && !option.Contains(" ") && value != NIL && arg.StartsWith(option.Head(option.Find("%"))))
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
	Bool	 encoderOptionsOnly = False;

	foreach (const String &arg, args)
	{
		/* Skip non-file arguments.
		 */
		if (arg == "--") encoderOptionsOnly = True;

		if (arg.StartsWith("-"))
		{
			if (ParamHasArguments(arg, encoderOptionsOnly)) foreachindex++;

			continue;
		}

		/* Add file argument to list.
		 */
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
}

Bool freac::freacCommandline::ParamHasArguments(const String &arg, Bool encoderOptionsOnly)
{
	if (!encoderOptionsOnly && (arg == "-e" || arg == "-h" || arg == "-d" || arg == "-o" || arg == "-p" || arg == "-cd" || arg == "-t")) return True;

	Registry	&boca	   = Registry::Get();
	Component	*component = boca.CreateComponentByID(String(encoderID).Append("-enc"));

	if (component == NIL) return False;

	/* Check dynamic encoder parameters.
	 */
	Bool				 hasArgs    = False;
	const Array<Parameter *>	&parameters = component->GetParameters();

	foreach (Parameter *parameter, parameters)
	{
		ParameterType		 type	 = parameter->GetType();
		String			 spec	 = parameter->GetArgument();
		const Array<Option *>	&options = parameter->GetOptions();

		if (type == PARAMETER_TYPE_SWITCH)
		{
			if (spec == arg) break;
		}
		else if (type == PARAMETER_TYPE_SELECTION)
		{
			if (spec.StartsWith(arg.Append(" "))) { hasArgs = True; break; }
			if (spec == arg.Append("%VALUE"))			break;

			if (spec == "%VALUE")
			{
				Bool	 found = False;

				foreach (Option *option, options) if (option->GetValue() == arg) found = True;

				if (found) break;
			}
		}
		else if (type == PARAMETER_TYPE_RANGE)
		{
			if (spec.StartsWith(arg.Append(" "))) { hasArgs = True; break; }
			if (spec == arg.Append("%VALUE"))			break;
		}
	}

	boca.DeleteComponent(component);

	return hasArgs;
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

Void freac::freacCommandline::Stop()
{
	stopped = True;

	JobConvert::Stop();
}

Bool freac::freacCommandline::SetConfigDefaults(BoCA::Config *config, Bool userConfig)
{
	/* Make mandatory config adjustments.
	 */
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsEnableConsoleID, True);

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, True);

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, True);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsDeleteAfterEncodingID, False);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsAddEncodedTracksID, False);

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, False);

	config->SetIntValue(Config::CategoryProcessingID, Config::ProcessingEnableProcessingID, False);

	config->SetIntValue(Config::CategoryVerificationID, Config::VerificationVerifyInputID, False);
	config->SetIntValue(Config::CategoryVerificationID, Config::VerificationVerifyOutputID, False);

	config->SetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableParallelConversionsID, True);

	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbAutoSelectID, True);
	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbEnableCacheID, True);

	config->SetIntValue(Config::CategoryRipperID, Config::RipperEjectAfterRippingID, False);

	config->SetIntValue(Config::CategoryTagsID, Config::TagsPreferCueSheetsToChaptersID, True);

	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtReadFromFilesID, False);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToFilesID, False);

	config->SetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreatePlaylistID, False);
	config->SetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreateCueSheetID, False);

	/* Don't set defaults for user-provided configs.
	 */
	if (userConfig) return True;

	/* Set configurable values to defaults.
	 */
	config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, "lame-enc");
	config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, "<filename>");

	config->SetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableSuperFastModeID, False);
	config->SetIntValue(Config::CategoryResourcesID, Config::ResourcesNumberOfConversionThreadsID, 0);

	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbAutoQueryID, False);

	config->SetIntValue(Config::CategoryRipperID, Config::RipperTimeoutID, 0);

	config->SetIntValue(Config::CategoryTagsID, Config::TagsReadChaptersID, True);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsWriteChaptersID, True);

	config->SetIntValue(Config::CategoryTagsID, Config::TagsReadEmbeddedCueSheetsID, True);

	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtReadFromTagsID, True);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToTagsID, True);

	return True;
}

Bool freac::freacCommandline::SetEncoderDefaults(BoCA::Config *config, Bool userConfig)
{
	/* Check dynamic encoder parameters.
	 */
	Registry	&boca	   = Registry::Get();
	Component	*component = boca.CreateComponentByID(String(encoderID).Append("-enc"));

	if (component == NIL)
	{
		Console::OutputString(String("Encoder '").Append(encoderID).Append("' could not be initialized!\n"));

		return False;
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
			Bool	 enabled = ScanForEncoderOption(spec);

			if (!userConfig || enabled) config->SetIntValue(component->GetID(), name, enabled);
		}
		else if (type == PARAMETER_TYPE_SELECTION)
		{
			/* Get selection value.
			 */
			String	 value;
			Bool	 present = ScanForEncoderOption(String(spec).Trim(), &value);

			/* Check if given value is allowed.
			 */
			if (present)
			{
				Bool	 found = False;

				/* Check case sensitive.
				 */
				foreach (Option *option, options)
				{
					if (found || option->GetValue() != value) continue;

					found = True;
				}

				/* Check ignoring case.
				 */
				value = value.ToLower();

				foreach (Option *option, options)
				{
					if (found || option->GetValue().ToLower() != value) continue;

					found = True;
					value = option->GetValue();
				}

				if (!found) broken = True;
			}

			/* Set selection to given value.
			 */
			if (!userConfig || present)
			{
				config->SetIntValue(component->GetID(), String("Set ").Append(name), present);
				config->SetStringValue(component->GetID(), name, value);
			}
		}
		else if (type == PARAMETER_TYPE_RANGE)
		{
			/* Set range parameter to given value.
			 */
			String	 value;
			Bool	 present = ScanForEncoderOption(String(spec).Trim(), &value);

			if (!userConfig || present)
			{
				config->SetIntValue(component->GetID(), String("Set ").Append(name), present);
				config->SetIntValue(component->GetID(), name, Math::Round(value.ToFloat() / step));
			}

			/* Check if given value is valid.
			 */
			Bool	 valid = True;

			foreach (Option *option, options) if (option->GetType() == OPTION_TYPE_MIN && value.ToFloat() < option->GetValue().ToFloat()) valid = False;
			foreach (Option *option, options) if (option->GetType() == OPTION_TYPE_MAX && value.ToFloat() > option->GetValue().ToFloat()) valid = False;

			if (present && !valid) broken = True;
		}
	}

	boca.DeleteComponent(component);

	if (broken) Console::OutputString(String("Invalid arguments for encoder '").Append(encoderID).Append("'!\n"));

	return !broken;
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
				Console::OutputString("  --list-drives\t\t\tPrint a list of available CD drives\n\n");
				Console::OutputString("  --drive=<n|id>  | -cd <n|id>\tSpecify active CD drive (0..n or device path)\n");
				Console::OutputString("  --track=<n>     | -t <n>\tSpecify input track(s) to rip (e.g. 1-5,7,9 or 'all')\n");
				Console::OutputString("  --timeout=<s>\t\t\tTimeout for CD track ripping (disabled by default)\n");
				Console::OutputString("  --cddb\t\t\tEnable CDDB database lookup\n");
				Console::OutputString("  --eject\t\t\tEject disc after ripping\n\n");
			}

			boca.DeleteComponent(info);
		}

		Console::OutputString("  --superfast\t\t\tEnable SuperFast mode\n");
		Console::OutputString("  --threads=<n>\t\t\tSpecify number of threads to use in SuperFast mode\n\n");

		Console::OutputString("  --split-chapters\t\tSplit to individual chapters if chapter tags are present\n\n");

		Console::OutputString("  --ignore-chapters\t\tDo not write chapter information to tags\n");
		Console::OutputString("  --ignore-coverart\t\tDo not write cover images to tags\n\n");

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
			Console::OutputString(String("Encoder '").Append(helpenc).Append("' is not supported by ").Append(freac::appName).Append("!\n"));

			return;
		}

		Component	*component = boca.CreateComponentByID(String(helpenc).Append("-enc"));

		if (component == NIL)
		{
			Console::OutputString(String("Encoder '").Append(helpenc).Append("' could not be initialized!\n"));

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

				maxTabs = Math::Max(maxTabs, Math::Ceil((spec.Length() + 1) / 8.0));
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

							Console::OutputString(option->GetValue().Append(option->GetAlias() != option->GetValue() ? String(" (").Append(option->GetAlias().Replace("%1", option->GetValue())).Append(")") : String()).Append(def == option->GetValue() ? ", default" : NIL).Append("\n"));
						}
					}

					if (foreachindex < parameters.Length() - 1) Console::OutputString("\n");
				}
				else if (type == PARAMETER_TYPE_RANGE)
				{
					Console::OutputString(String("\t").Append(spec).Append(String().FillN('\t', maxTabs - Math::Floor(spec.Length() / 8.0))).Append(name).Append(": "));

					foreach (Option *option, options) if (option->GetType() == OPTION_TYPE_MIN) Console::OutputString(String(option->GetValue()).Append(option->GetAlias() != option->GetValue() ? String(" (").Append(option->GetAlias().Replace("%1", option->GetValue())).Append(")") : String()).Append(" - "));
					foreach (Option *option, options) if (option->GetType() == OPTION_TYPE_MAX) Console::OutputString(String(option->GetValue()).Append(option->GetAlias() != option->GetValue() ? String(" (").Append(option->GetAlias().Replace("%1", option->GetValue())).Append(")") : String()));

					if (def != NIL) Console::OutputString(String(", default ").Append(def));

					Console::OutputString("\n");
				}
			}
		}
		else
		{
			Console::OutputString(String("\tno options for ").Append(component->GetName()).Append("\n"));
		}

		boca.DeleteComponent(component);
	}
}
