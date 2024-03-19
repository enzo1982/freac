 /* fre:ac - free audio converter
  * Copyright (C) 2001-2024 Robert Kausch <robert.kausch@freac.org>
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

	freac::freacCommandline	*application = freac::freacCommandline::Get(args);
	Int			 errorCode   = application->GetErrorCode();

	freac::freacCommandline::Free();

	return errorCode;
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

freac::freacCommandline::freacCommandline(const Array<String> &arguments) : args(arguments), firstFile(True), stopped(False), errorCode(0)
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

			errorCode = -2;

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
	Bool		 ignoreAlbumArt	 = ScanForProgramOption("--ignore-albumart") || ScanForProgramOption("--ignore-coverart");

	String		 cueSheetFile;
	String		 playlistFile;

	String		 playlistFormat	 = Config::PlaylistFormatDefault;

	encoderID = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);
	encoderID = encoderID.Head(encoderID.Length() - 4);

	if (!ScanForProgramOption("--encoder=%VALUE", &encoderID)) ScanForProgramOption("-e %VALUE", &encoderID);
	if (!ScanForProgramOption("--help=%VALUE",    &helpenc))   ScanForProgramOption("-h %VALUE", &helpenc);

	ScanForProgramOption("-d %VALUE", &outputFolder);
	ScanForProgramOption("-o %VALUE", &outputFile);

	if (outputFolder != NIL && BoCA::Utilities::IsRelativePath(outputFolder)) outputFolder = GetAbsolutePathName(outputFolder);
	if (outputFile	 != NIL && BoCA::Utilities::IsRelativePath(outputFile))	  outputFile   = GetAbsolutePathName(outputFile);

	if (!ScanForProgramOption("--pattern=%VALUE", &pattern))   ScanForProgramOption("-p %VALUE", &pattern);

	ScanForProgramOption("--threads=%VALUE", &numberOfThreads);

	if (numberOfThreads != NIL) numberOfThreads = String::FromInt(numberOfThreads.ToInt());

	ScanForProgramOption("--cuesheet=%VALUE", &cueSheetFile);
	ScanForProgramOption("--playlist=%VALUE", &playlistFile);

	if (cueSheetFile == NIL && ScanForProgramOption("--cuesheet")) cueSheetFile = "<default>";
	if (playlistFile == NIL && ScanForProgramOption("--playlist")) playlistFile = "<default>";

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

				Console::OutputString(String("\t").Append(String::FromInt(i)).Append("\t").Append(device.path).Append(device.path.Length() < 8 ? "\t\t" : "\t").Append(device.GetName()).Append("\n"));
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

			errorCode = -2;

			return;
		}
	}
	else if (tracks != NIL || ScanForProgramOption("--list-drives"))
	{
		Console::OutputString("Error: CD ripping disabled!");

		errorCode = -4;

		return;
	}

	Console::SetTitle(String(freac::appName).Append(" ").Append(freac::version));

	if ((files.Length() == 0 && !ScanForProgramOption("--eject")) || helpenc != NIL)
	{
		ShowHelp(helpenc);

		if (helpenc == NIL) errorCode = -2;

		return;
	}

	if (!boca.ComponentExists(String(encoderID).Append("-enc")))
	{
		Console::OutputString(String("Encoder '").Append(encoderID).Append("' is not supported by ").Append(freac::appName).Append("!\n"));

		errorCode = -2;

		return;
	}

	/* Load album art files.
	 */
	String	 coverFrontFile;
	String	 coverBackFile;

	ScanForProgramOption("--add-cover=%VALUE", &coverFrontFile);
	ScanForProgramOption("--add-cover-back=%VALUE", &coverBackFile);

	if (coverFrontFile != NIL)
	{
		coverFront = LoadAlbumArt(coverFrontFile, 3);

		if (coverFront == Picture()) return;
	}

	if (coverBackFile != NIL)
	{
		coverBack = LoadAlbumArt(coverBackFile, 4);

		if (coverBack == Picture()) return;
	}

	/* Evaluate dynamic encoder parameters.
	 */
	if (!SetEncoderDefaults(config, userConfig)) return;

	/* Set config values.
	 */
				    config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, String(encoderID).Append("-enc"));
				    config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Directory(outputFolder));

				    config->SetStringValue(Config::CategorySettingsID, Config::SettingsSingleFilenameID, outputFile);

	if (pattern	    != NIL) config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, pattern);

	if (enableSuperFast	  ) config->SetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableSuperFastModeID, True);
	if (numberOfThreads == "1") config->SetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableParallelConversionsID, False);
	if (numberOfThreads != NIL) config->SetIntValue(Config::CategoryResourcesID, Config::ResourcesNumberOfConversionThreadsID, numberOfThreads.ToInt());
	
	if (enableCDDB		  ) config->SetIntValue(Config::CategoryFreedbID, Config::FreedbAutoQueryID, True);

	if (ripperTimeout   != NIL) config->SetIntValue(Config::CategoryRipperID, Config::RipperTimeoutID, ripperTimeout.ToInt ());

	if (cueSheetFile != NIL)
	{
		config->SetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreateCueSheetID, True);

		if (cueSheetFile != "<default>")
		{
			if (!cueSheetFile.ToLower().EndsWith(".cue")) cueSheetFile.Append(".cue");

			config->SetStringValue(Config::CategoryPlaylistID, Config::PlaylistCueSheetID, File(cueSheetFile));
		}
		else
		{
			cueSheetFile = String(Directory(outputFolder)).Append(Directory::GetDirectoryDelimiter()).Append("cuesheet.cue");
		}
	}

	if (playlistFile != NIL)
	{
		config->SetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreatePlaylistID, True);

		if (playlistFile != "<default>")
		{
			if (!playlistFile.Contains(".")) playlistFile.Append(".m3u8");

			config->SetStringValue(Config::CategoryPlaylistID, Config::PlaylistFilenameID, File(playlistFile));

			/* Set playlist format based on file extension.
			 */
			for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
			{
				if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_PLAYLIST) continue;

				const Array<FileFormat *>	&formats = boca.GetComponentFormats(i);

				foreach (FileFormat *format, formats)
				{
					const Array<String>	&formatExtensions = format->GetExtensions();

					foreach (const String &formatExtension, formatExtensions)
					{
						if (playlistFile.ToLower().EndsWith(String(".").Append(formatExtension))) playlistFormat = boca.GetComponentID(i).Append("-").Append(formatExtension);
					}
				}
			}

			config->SetStringValue(Config::CategoryPlaylistID, Config::PlaylistFormatID, playlistFormat);
		}
		else
		{
			playlistFile = String(Directory(outputFolder)).Append(Directory::GetDirectoryDelimiter()).Append("playlist.").Append(playlistFormat.Tail(playlistFormat.Length() - playlistFormat.FindLast("-") - 1));
		}
	}

	if (ignoreChapters)
	{
		config->SetIntValue(Config::CategoryTagsID, Config::TagsReadChaptersID, False);
		config->SetIntValue(Config::CategoryTagsID, Config::TagsWriteChaptersID, False);

		config->SetIntValue(Config::CategoryTagsID, Config::TagsReadEmbeddedCueSheetsID, False);
	}

	if (ignoreAlbumArt)
	{
		config->SetIntValue(Config::CategoryTagsID, Config::TagsAlbumArtReadFromTagsID, False);
		config->SetIntValue(Config::CategoryTagsID, Config::TagsAlbumArtWriteToTagsID, False);
	}

	/* Perform actual conversion.
	 */
	JobConvert::onEncodeTrack.Connect(&freacCommandline::OnEncodeTrack, this);
	JobConvert::onFinishEncoding.Connect(&freacCommandline::OnFinishEncoding, this);

	if (outputFile != NIL)
	{
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

				errorCode = -3;

				continue;
			}

			jobFiles.Add(file);
		}

		/* Add files to joblist.
		 */
		JobList	*joblist = new JobList(Point(0, 0), Size(0, 0));

		if (AddToJoblist(jobFiles, addCDTracks))
		{
			/* Convert tracks in joblist.
			 */
			Converter().Convert(*joblist->GetTrackList(), False, False);
		}
		else
		{
			Console::OutputString("Could not process input files!\n");

			errorCode = -1;
		}

		DeleteObject(joblist);
	}
	else
	{
		Array<Track>	 playlistTracks;

		config->SetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreatePlaylistID, False);
		config->SetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreateCueSheetID, False);

		JobList		*joblist = new JobList(Point(0, 0), Size(0, 0));

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

				errorCode = -3;

				continue;
			}

			in.Close();

			/* Handle splitting chapters and cue sheets.
			 */
			Bool	 splitFile = splitChapters || currentFile.EndsWith(".cue");
			String	 pattern   = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, "<filename>");

			config->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, !splitFile);

			if (splitFile && pattern == "<filename>") config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, "<filename> - <track> - <title>");
			else					  config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, pattern);

			/* Add file to joblist.
			 */
			Array<String>	 jobFiles;

			jobFiles.Add(file);

			if (!AddToJoblist(jobFiles, addCDTrack))
			{
				Console::OutputString(String("Could not process file: ").Append(currentFile).Append("\n"));

				errorCode = -1;

				continue;
			}

			/* Set output file name.
			 */
			Track	 track = joblist->GetNthTrack(0);

			config->SetStringValue(Config::CategorySettingsID, Config::SettingsSingleFilenameID, Utilities::GetOutputFileName(config, track));

			/* Add tracks to playlist tracks.
			 */
			const Array<Track>	*joblistTracks = joblist->GetTrackList();

			foreach (Track playlistTrack, *joblistTracks)
			{
				playlistTrack.fileName	= Utilities::GetOutputFileName(config, playlistTrack);
				playlistTrack.isCDTrack = False;

				playlistTracks.Add(playlistTrack);
			}

			/* Convert tracks in joblist.
			 */
			firstFile = True;

			Converter().Convert(*joblist->GetTrackList(), False, False);

			joblist->RemoveAllTracks();

			if (stopped) break;
		}

		/* Create playlist and cue sheet.
		 */
		if (!stopped && playlistTracks.Length() > 0 && playlistFile != NIL)
		{
			/* Write playlist.
			 */
			PlaylistComponent	*playlist = (PlaylistComponent *) boca.CreateComponentByID(playlistFormat.Head(playlistFormat.FindLast("-")));

			if (playlist != NIL)
			{
				playlist->SetTrackList(playlistTracks);
				playlist->WritePlaylist(File(playlistFile));

				boca.DeleteComponent(playlist);
			}
		}

		if (!stopped && playlistTracks.Length() > 0 && cueSheetFile != NIL)
		{
			/* Write cue sheet.
			 */
			PlaylistComponent	*cuesheet = (PlaylistComponent *) boca.CreateComponentByID("cuesheet-playlist");

			if (cuesheet != NIL)
			{
				cuesheet->SetTrackList(playlistTracks);
				cuesheet->WritePlaylist(File(cueSheetFile));

				boca.DeleteComponent(cuesheet);
			}
		}

		DeleteObject(joblist);
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
	BoCA::Config	*config = BoCA::Config::Get();
	Bool		 quiet	= ScanForProgramOption("--quiet");

	if (!quiet)
	{
		static String	 previousFile;
		String		 currentFile = track.fileName;

		if (currentFile == previousFile) return;
		else				 previousFile = currentFile;

		if (currentFile.StartsWith("device://cdda:")) currentFile = String("Audio CD ").Append(String::FromInt(config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault))).Append(" - Track ").Append(currentFile.Tail(currentFile.Length() - 16));

		if (!firstFile) Console::OutputString("done.\n");
		else		firstFile = False;

		Console::OutputString(String("Processing file: ").Append(currentFile).Append("..."));
	}
}

Void freac::freacCommandline::OnFinishEncoding(Bool success)
{
	Bool	 quiet = ScanForProgramOption("--quiet");

	if (!quiet)
	{
		if (success) Console::OutputString("done.\n");
		else	     Console::OutputString("aborted.\n");
	}

	if (!success) errorCode = -1;
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
		if (parameter->GetHidden()) continue;

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

String freac::freacCommandline::GetAbsolutePathName(const String &path) const
{
	String	 pathName = path;

	/* Convert relative to absolute paths.
	 */
	if (BoCA::Utilities::IsRelativePath(pathName)) pathName = String(Directory::GetActiveDirectory()).Append(Directory::GetDirectoryDelimiter()).Append(pathName);

	if (!pathName.EndsWith(Directory::GetDirectoryDelimiter())) pathName.Append(Directory::GetDirectoryDelimiter());

	return pathName;
}

Picture freac::freacCommandline::LoadAlbumArt(const String &file, Int type)
{
	if (!File(file).Exists())
	{
		Console::OutputString(String("Album art file not found: ").Append(file).Append("\n"));

		errorCode = -3;

		return Picture();
	}

	Picture	 picture;

	picture.LoadFromFile(file);

	if (picture.GetBitmap() == NIL)
	{
		Console::OutputString(String("Invalid album art file format: ").Append(file).Append("\n"));

		errorCode = -1;

		return Picture();
	}

	picture.type = type;

	return picture;
}

Bool freac::freacCommandline::AddToJoblist(const Array<String> &files, Bool cdTracks)
{
	/* Schedule job to add tracks to joblist.
	 */
	Job	*job = cdTracks ? (Job *) new JobAddTracks(files) : (Job *) new JobAddFiles(files);

	job->Schedule();

	while (Job::GetScheduledJobs().Length()	> 0) S::System::System::Sleep(10);
	while (Job::GetPlannedJobs().Length()	> 0) S::System::System::Sleep(10);
	while (Job::GetRunningJobs().Length()	> 0) S::System::System::Sleep(10);

	/* Check track list.
	 */
	BoCA::JobList		*joblist = BoCA::JobList::Get();
	const Array<Track>	*tracks	 = joblist->getTrackList.Call();

	if (tracks->Length() == 0) return False;

	/* Add album art.
	 */
	if (coverFront != Picture() || coverBack != Picture())
	{
		foreach (Track track, *tracks)
		{
			if (coverFront != Picture()) track.pictures.Add(coverFront);
			if (coverBack  != Picture()) track.pictures.Add(coverBack);

			joblist->onComponentModifyTrack.Emit(track);
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
	/* Set user specified config flag.
	 */
	config->SetIntValue(Config::CategorySettingsID, "UserSpecifiedConfig", userConfig);

	/* Make mandatory config adjustments.
	 */
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsEnableConsoleID, True);

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, True);

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, True);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsDeleteAfterEncodingID, False);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsAddEncodedTracksID, False);

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, False);

	config->SetIntValue(Config::CategoryVerificationID, Config::VerificationVerifyInputID, False);
	config->SetIntValue(Config::CategoryVerificationID, Config::VerificationVerifyOutputID, False);

	config->SetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableParallelConversionsID, True);

	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbAutoSelectID, True);
	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbEnableCacheID, True);

	config->SetIntValue(Config::CategoryRipperID, Config::RipperEjectAfterRippingID, False);

	config->SetIntValue(Config::CategoryTagsID, Config::TagsPreferCueSheetsToChaptersID, True);

	config->SetIntValue(Config::CategoryTagsID, Config::TagsAlbumArtReadFromFilesID, False);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsAlbumArtWriteToFilesID, False);

	config->SetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreatePlaylistID, False);
	config->SetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreateCueSheetID, False);

	/* Don't set defaults for user-provided configs.
	 */
	if (userConfig) return True;

	/* Set configurable values to defaults.
	 */
	config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, "lame-enc");
	config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, "<filename>");

	config->SetIntValue(Config::CategoryProcessingID, Config::ProcessingEnableProcessingID, False);

	config->SetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableSuperFastModeID, False);
	config->SetIntValue(Config::CategoryResourcesID, Config::ResourcesNumberOfConversionThreadsID, 0);

	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbAutoQueryID, False);

	config->SetIntValue(Config::CategoryRipperID, Config::RipperTimeoutID, 0);

	config->SetIntValue(Config::CategoryTagsID, Config::TagsReadChaptersID, True);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsWriteChaptersID, True);

	config->SetIntValue(Config::CategoryTagsID, Config::TagsReadEmbeddedCueSheetsID, True);

	config->SetIntValue(Config::CategoryTagsID, Config::TagsAlbumArtReadFromTagsID, True);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsAlbumArtWriteToTagsID, True);

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

		errorCode = -1;

		return False;
	}

	Bool				 broken	    = False;
	const Array<Parameter *>	&parameters = component->GetParameters();

	foreach (Parameter *parameter, parameters)
	{
		if (parameter->GetHidden()) continue;

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

	if (broken)
	{
		Console::OutputString(String("Invalid arguments for encoder '").Append(encoderID).Append("'!\n"));

		errorCode = -2;
	}

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
		Console::OutputString("  --pattern=<pat> | -p <pat>\tSpecify output file name pattern (default is \"<filename>\")\n\n");

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

		Console::OutputString("  --add-cover=<file>\t\tAdd front cover image from file (.png or .jpg)\n");
		Console::OutputString("  --add-cover-back=<file>\tAdd back cover image from file (.png or .jpg)\n\n");

		if (boca.GetNumberOfComponentsOfType(COMPONENT_TYPE_PLAYLIST) > 0)
		{
			if (								      boca.ComponentExists("cuesheet-playlist")) Console::OutputString("  --cuesheet[=<file>]\t\tCreate a cue sheet of the conversion results\n");
			if (boca.GetNumberOfComponentsOfType(COMPONENT_TYPE_PLAYLIST) > 1 || !boca.ComponentExists("cuesheet-playlist")) Console::OutputString("  --playlist[=<file>]\t\tCreate a playlist containing the conversion results\n");

			Console::OutputString("\n");
		}

		Console::OutputString("  --ignore-chapters\t\tDo not write chapter information to tags\n");
		Console::OutputString("  --ignore-albumart\t\tDo not write album images to tags\n\n");

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

		Console::OutputString(list.Append("\n"));
	}
	else
	{
		if (!boca.ComponentExists(String(helpenc).Append("-enc")))
		{
			Console::OutputString(String("Encoder '").Append(helpenc).Append("' is not supported by ").Append(freac::appName).Append("!\n"));

			errorCode = -2;

			return;
		}

		Component	*component = boca.CreateComponentByID(String(helpenc).Append("-enc"));

		if (component == NIL)
		{
			Console::OutputString(String("Encoder '").Append(helpenc).Append("' could not be initialized!\n"));

			errorCode = -1;

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
				if (parameter->GetHidden()) continue;

				String	 spec = parameter->GetArgument().Replace("%VALUE", "<val>");

				maxTabs = Math::Max(maxTabs, Math::Ceil((spec.Length() + 1) / 8.0));
			}

			/* Print formatted parameter list.
			 */
			foreach (Parameter *parameter, parameters)
			{
				if (parameter->GetHidden()) continue;

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
