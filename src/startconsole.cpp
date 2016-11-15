 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
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
	BonkEnc::BonkEncCommandline::Get(args);
	BonkEnc::BonkEncCommandline::Free();

	return 0;
}

BonkEnc::BonkEncCommandline *BonkEnc::BonkEncCommandline::Get(const Array<String> &args)
{
	if (instance == NIL) new BonkEncCommandline(args);

	return (BonkEncCommandline *) instance;
}

Void BonkEnc::BonkEncCommandline::Free()
{
	if (instance != NIL) delete (BonkEncCommandline *) instance;
}

BonkEnc::BonkEncCommandline::BonkEncCommandline(const Array<String> &arguments) : args(arguments)
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
	if (ScanForOption("--list-configs"))
	{
		Console::OutputString(String(BonkEnc::appLongName).Append(" ").Append(BonkEnc::version).Append(" (").Append(BonkEnc::architecture).Append(") command line interface\n").Append(BonkEnc::copyright).Append("\n\n"));
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

	if (ScanForOption("--config", &configName) && configName != "default" && configName != "Default configuration" && configName != i18n->TranslateString("Default configuration", "Configuration"))
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
	Bool		 quiet		= ScanForOption("--quiet");
	Bool		 cddb		= ScanForOption("--cddb");
	Array<String>	 files;
	String		 encoderID	= "LAME";
	String		 helpenc;
	String		 outdir		= Directory::GetActiveDirectory();
	String		 outfile;
	String		 pattern	= "<artist> - <title>";
	String		 cdDrive	= "0";
	String		 tracks;
	String		 timeout	= "120";

	if (!ScanForOption("--encoder", &encoderID)) ScanForOption("-e", &encoderID);
	if (!ScanForOption("--help",	&helpenc))   ScanForOption("-h", &helpenc);
						     ScanForOption("-d", &outdir);
						     ScanForOption("-o", &outfile);
	if (!ScanForOption("--pattern", &pattern))   ScanForOption("-p", &pattern);

	DeviceInfoComponent	*info	   = boca.CreateDeviceInfoComponent();
	Int			 numDrives = 0;

	if (info != NIL)
	{
		if (info->GetNumberOfDevices() > 0)
		{
			if (!ScanForOption("--drive", &cdDrive)) ScanForOption("-cd", &cdDrive);
			if (!ScanForOption("--track", &tracks))	 ScanForOption("-t",  &tracks);

			ScanForOption("--timeout", &timeout);
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

	Console::SetTitle(String(BonkEnc::appName).Append(" ").Append(BonkEnc::version));

	encoderID = encoderID.ToUpper();

	if (files.Length() == 0 ||
	    helpenc != NIL ||
	    !(encoderID == "LAME" || encoderID == "VORBIS" || encoderID == "BONK" || encoderID == "BLADE" || encoderID == "FAAC" || encoderID == "FLAC" || encoderID == "TVQ" || encoderID == "WAVE"))
	{
		ShowHelp(helpenc);

		return;
	}

	JobList	*joblist = new JobList(Point(0, 0), Size(0, 0));
	Bool	 broken	 = False;

	if (((encoderID == "LAME")   && !boca.ComponentExists("lame-enc"))     ||
	    ((encoderID == "VORBIS") && !boca.ComponentExists("vorbis-enc"))   ||
	    ((encoderID == "BONK")   && !boca.ComponentExists("bonk-enc"))     ||
	    ((encoderID == "BLADE")  && !boca.ComponentExists("bladeenc-enc")) ||
	    ((encoderID == "FAAC")   && !boca.ComponentExists("faac-enc"))     ||
	    ((encoderID == "FLAC")   && !boca.ComponentExists("flac-enc"))     ||
	    ((encoderID == "TVQ")    && !boca.ComponentExists("twinvq-enc")))
	{
		Console::OutputString(String("Encoder ").Append(encoderID).Append(" is not available!\n\n"));

		broken = True;
	}

	if (encoderID == "LAME")
	{
		String	 bitrate = "192";
		String	 quality = "5";
		String	 mode	 = "VBR";

		ScanForOption("-b", &bitrate);
		ScanForOption("-q", &quality);
		ScanForOption("-m", &mode);

		config->SetIntValue("LAME", "Preset", 0);
		config->SetIntValue("LAME", "SetBitrate", True);

		config->SetIntValue("LAME", "Bitrate", Math::Max(0, Math::Min(320, (Int) bitrate.ToInt())));
		config->SetIntValue("LAME", "ABRBitrate", Math::Max(0, Math::Min(320, (Int) bitrate.ToInt())));
		config->SetIntValue("LAME", "VBRQuality", Math::Max(0, Math::Min(9, (Int) quality.ToInt())) * 10);

		if	(mode == "VBR" || mode == "vbr") config->SetIntValue("LAME", "VBRMode", 4);
		else if (mode == "ABR" || mode == "abr") config->SetIntValue("LAME", "VBRMode", 3);
		else if (mode == "CBR" || mode == "cbr") config->SetIntValue("LAME", "VBRMode", 0);

		config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, "lame-enc");
	}
	else if (encoderID == "VORBIS")
	{
		String	 bitrate = "192";
		String	 quality = "60";

		if (ScanForOption("-b", &bitrate)) config->SetIntValue("Vorbis", "Mode", 1);
		else				   config->SetIntValue("Vorbis", "Mode", 0);

		config->SetIntValue("Vorbis", "Quality", Math::Max(0, Math::Min(100, (Int) quality.ToInt())));
		config->SetIntValue("Vorbis", "Bitrate", Math::Max(45, Math::Min(500, (Int) bitrate.ToInt())));

		config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, "vorbis-enc");
	}
	else if (encoderID == "BONK")
	{
		String	 quantization = "0.4";
		String	 predictor    = "32";
		String	 downsampling = "2";

		ScanForOption("-q", &quantization);
		ScanForOption("-p", &predictor);
		ScanForOption("-r", &downsampling);

		config->SetIntValue("Bonk", "JointStereo", ScanForOption("-js"));
		config->SetIntValue("Bonk", "Lossless", ScanForOption("-lossless"));

		config->SetIntValue("Bonk", "Quantization", Math::Max(0, Math::Min(40, (Int) Math::Round(quantization.ToFloat() * 20))));
		config->SetIntValue("Bonk", "Predictor", Math::Max(0, Math::Min(512, (Int) predictor.ToInt())));
		config->SetIntValue("Bonk", "Downsampling", Math::Max(0, Math::Min(10, (Int) downsampling.ToInt())));

		config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, "bonk-enc");
	}
	else if (encoderID == "BLADE")
	{
		String	 bitrate = "192";

		ScanForOption("-b", &bitrate);

		config->SetIntValue("BladeEnc", "Bitrate", Math::Max(32, Math::Min(320, (Int) bitrate.ToInt())));

		config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, "blade-enc");
	}
	else if (encoderID == "FAAC")
	{
		String	 bitrate = "64";
		String	 quality = "100";

		if (ScanForOption("-b", &bitrate)) config->SetIntValue("FAAC", "SetQuality", False);
		else				   config->SetIntValue("FAAC", "SetQuality", True);

		config->SetIntValue("FAAC", "MP4Container", ScanForOption("-mp4"));

		config->SetIntValue("FAAC", "AACQuality", Math::Max(10, Math::Min(500, (Int) quality.ToInt())));
		config->SetIntValue("FAAC", "Bitrate", Math::Max(8, Math::Min(256, (Int) bitrate.ToInt())));

		config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, "faac-enc");
	}
	else if (encoderID == "FLAC")
	{
		String	 blocksize = "4608";
		String	 lpc = "8";
		String	 qlp = "0";
		String	 rice = "3,3";
		String	 minrice;
		String	 maxrice;

		ScanForOption("-b", &blocksize);
		ScanForOption("-l", &lpc);
		ScanForOption("-q", &qlp);
		ScanForOption("-r", &rice);

		Int	 i = 0;
		Int	 j = 0;

		for (i = 0; i < rice.Length(); i++)	{ if (rice[i] == ',') break; minrice[i] = rice[i]; }
		for (j = i + 1; j < rice.Length(); j++)	{ maxrice[j - i - 1] = rice[j]; }

		config->SetIntValue("FLAC", "Preset", -1);

		config->SetIntValue("FLAC", "DoMidSideStereo", ScanForOption("-ms"));
		config->SetIntValue("FLAC", "DoExhaustiveModelSearch", ScanForOption("-e"));
		config->SetIntValue("FLAC", "DoQLPCoeffPrecSearch", ScanForOption("-p"));

		config->SetIntValue("FLAC", "Blocksize", Math::Max(192, Math::Min(32768, (Int) blocksize.ToInt())));
		config->SetIntValue("FLAC", "MaxLPCOrder", Math::Max(0, Math::Min(32, (Int) lpc.ToInt())));
		config->SetIntValue("FLAC", "QLPCoeffPrecision", Math::Max(0, Math::Min(16, (Int) qlp.ToInt())));
		config->SetIntValue("FLAC", "MinResidualPartitionOrder", Math::Max(0, Math::Min(16, (Int) minrice.ToInt())));
		config->SetIntValue("FLAC", "MaxResidualPartitionOrder", Math::Max(0, Math::Min(16, (Int) maxrice.ToInt())));

		config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, "flac-enc");
	}
	else if (encoderID == "TVQ")
	{
		String	 bitrate    = "48";
		String	 candidates = "32";

		ScanForOption("-b", &bitrate);
		ScanForOption("-c", &candidates);

		config->SetIntValue("TwinVQ", "PreselectionCandidates", Math::Max(4, Math::Min(32, (Int) candidates.ToInt())));
		config->SetIntValue("TwinVQ", "Bitrate", Math::Max(24, Math::Min(48, (Int) bitrate.ToInt())));

		config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, "twinvq-enc");
	}
	else if (encoderID == "WAVE")
	{
		config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, "wave-enc");

		if (!boca.ComponentExists("wave-enc"))
		{
			config->SetIntValue("SndFile", "Format", 0x010000);
			config->SetIntValue("SndFile", "SubFormat", 0x000000);

			config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, "sndfile-enc");
		}
	}
	else
	{
		Console::OutputString(String("Encoder ").Append(encoderID).Append(" is not supported by ").Append(BonkEnc::appName).Append("!\n\n"));

		broken = True;
	}

	if (!broken)
	{
		if (!outdir.EndsWith(Directory::GetDirectoryDelimiter())) outdir.Append(Directory::GetDirectoryDelimiter());

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

			JobConvert::onEncodeTrack.Connect(&BonkEncCommandline::OnEncodeTrack, this);

			Array<String>	 jobFiles;
			Bool		 addCDTracks = False;

			for (Int i = 0; i < files.Length(); i++)
			{
				InStream	*in = new InStream(STREAM_FILE, files.GetNth(i), IS_READ);
				String		 currentFile = files.GetNth(i);

				if (currentFile.StartsWith("device://cdda:"))
				{
					currentFile = String("Audio CD ").Append(String::FromInt(config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault))).Append(" - Track ").Append(currentFile.Tail(currentFile.Length() - 16));
					addCDTracks = True;
				}

				if (in->GetLastError() != IO_ERROR_OK && !files.GetNth(i).StartsWith("device://"))
				{
					delete in;

					Console::OutputString(String("File not found: ").Append(files.GetNth(i)).Append("\n"));

					broken = True;

					continue;
				}
				else
				{
					delete in;
				}

				jobFiles.Add(files.GetNth(i));
			}

			Job	*job = addCDTracks ? (Job *) new JobAddTracks(jobFiles) : (Job *) new JobAddFiles(jobFiles);

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

				broken = True;
			}
		}
		else
		{
			for (Int i = 0; i < files.Length(); i++)
			{
				InStream	*in	     = new InStream(STREAM_FILE, files.GetNth(i), IS_READ);
				String		 currentFile = files.GetNth(i);
				Bool		 addCDTrack  = False;

				if (currentFile.StartsWith("device://cdda:"))
				{
					currentFile = String("Audio CD ").Append(String::FromInt(config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault))).Append(" - Track ").Append(currentFile.Tail(currentFile.Length() - 16));
					addCDTrack  = True;
				}

				if (in->GetLastError() != IO_ERROR_OK && !files.GetNth(i).StartsWith("device://"))
				{
					delete in;

					Console::OutputString(String("File not found: ").Append(files.GetNth(i)).Append("\n"));

					broken = True;

					continue;
				}
				else
				{
					delete in;
				}

				Array<String>	 jobFiles;

				jobFiles.Add(files.GetNth(i));

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

					broken = True;

					continue;
				}
			}
		}
	}

	delete joblist;
}

BonkEnc::BonkEncCommandline::~BonkEncCommandline()
{
}

Void BonkEnc::BonkEncCommandline::OnEncodeTrack(const Track &track, const String &decoderName, ConversionStep mode)
{
	static Bool	 firstTime = True;

	BoCA::Config	*config = BoCA::Config::Get();
	Bool		 quiet	= ScanForOption("--quiet");

	if (!quiet)
	{
		if (!firstTime) Console::OutputString("done.\n");
		else		firstTime = False;

		String	 currentFile = track.origFilename;

		if (currentFile.StartsWith("device://cdda:")) currentFile = String("Audio CD ").Append(String::FromInt(config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault))).Append(" - Track ").Append(currentFile.Tail(currentFile.Length() - 16));

		Console::OutputString(String("Processing file: ").Append(currentFile).Append("..."));
	}
}

Bool BonkEnc::BonkEncCommandline::ScanForOption(const String &option, String *value)
{
	for (Int i = 0; i < args.Length(); i++)
	{
		if (option.StartsWith("--") && value != NIL && args.GetNth(i).StartsWith(String(option).Append("=")))
		{
			*value = args.GetNth(i).Tail(args.GetNth(i).Length() - option.Length() - 1);

			return True;
		}
		else if (option.StartsWith("--") && args.GetNth(i) == option)
		{
			return True;
		}
		else if (option.StartsWith("-") && args.GetNth(i) == option)
		{
			if (value != NIL) *value = args.GetNth(i + 1);

			return True;
		}
	}

	return False;
}

Void BonkEnc::BonkEncCommandline::ScanForFiles(Array<String> *files)
{
	String	 param;
	String	 prevParam;

	for (Int i = 0; i < args.Length(); i++)
	{
		prevParam	= param;
		param		= args.GetNth(i);

		if (param[0] != '-' && (prevParam.StartsWith("--")  ||
					prevParam[0] != '-'	    ||
					prevParam    == "-js"	    ||
					prevParam    == "-lossless" ||
					prevParam    == "-mp4"	    ||
					prevParam    == "-ms"	    ||
					prevParam    == "-extc"	    ||
					prevParam    == "-extm"))
		{
			if (param.Contains("*") || param.Contains("?"))
			{
				File			 file(param);
				Directory		 dir(file.GetFilePath());
				const Array<File>	&array = dir.GetFilesByPattern(file.GetFileName());

				foreach (const File &entry, array) (*files).Add(entry);
			}
			else
			{
				(*files).Add(param);
			}
		}
	}
}

Bool BonkEnc::BonkEncCommandline::TracksToFiles(const String &tracks, Array<String> *files)
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

Void BonkEnc::BonkEncCommandline::ShowHelp(const String &helpenc)
{
	Console::OutputString(String(BonkEnc::appLongName).Append(" ").Append(BonkEnc::version).Append(" (").Append(BonkEnc::architecture).Append(") command line interface\n").Append(BonkEnc::copyright).Append("\n\n"));

	if (helpenc == NIL)
	{
		Console::OutputString("Usage:\tfreaccmd [options] [file(s)]\n\n");
		Console::OutputString("  --encoder=<id>  | -e <id>\tSpecify the encoder to use (default is LAME)\n");
		Console::OutputString("  --help=<id>     | -h <id>\tPrint help for encoder specific options\n\n");
		Console::OutputString("                    -d <dir>\tSpecify output directory for encoded files\n");
		Console::OutputString("                    -o <file>\tSpecify output file name in single file mode\n");
		Console::OutputString("  --pattern=<pat> | -p <pat>\tSpecify output file name pattern\n\n");

		Registry		&boca	   = Registry::Get();
		DeviceInfoComponent	*info	   = boca.CreateDeviceInfoComponent();

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
		Console::OutputString("Encoder <id> can be one of LAME, VORBIS, BONK, BLADE, FAAC, FLAC, TVQ or WAVE.\n\n");
		Console::OutputString("Default for <pat> is \"<artist> - <title>\".\n\n");
	}
	else
	{
		if (helpenc == "LAME" || helpenc == "lame")
		{
			Console::OutputString("Options for LAME MP3 encoder:\n\n");
			Console::OutputString("\t-m <mode>\t\t(CBR, VBR or ABR, default: VBR)\n");
			Console::OutputString("\t-b <CBR/ABR bitrate>\t(8 - 320, default: 192)\n");
			Console::OutputString("\t-q <VBR quality>\t(0 = best, 9 = worst, default: 5)\n\n");
		}
		else if (helpenc == "VORBIS" || helpenc == "vorbis")
		{
			Console::OutputString("Options for Ogg Vorbis encoder:\n\n");
			Console::OutputString("\t-q <quality>\t\t(0 - 100, default: 60, VBR mode)\n");
			Console::OutputString("\t-b <target bitrate>\t(45 - 500, default: 192, ABR mode)\n\n");
		}
		else if (helpenc == "BONK" || helpenc == "bonk")
		{
			Console::OutputString("Options for Bonk encoder:\n\n");
			Console::OutputString("\t-q <quantization factor>\t(0 - 2, default: 0.4)\n");
			Console::OutputString("\t-p <predictor size>\t\t(0 - 512, default: 32)\n");
			Console::OutputString("\t-r <downsampling ratio>\t\t(1 - 10, default: 2)\n");
			Console::OutputString("\t-js\t\t\t\t(use Joint Stereo)\n");
			Console::OutputString("\t-lossless\t\t\t(use lossless compression)\n\n");
		}
		else if (helpenc == "BLADE" || helpenc == "blade")
		{
			Console::OutputString("Options for BladeEnc encoder:\n\n");
			Console::OutputString("\t-b <bitrate>\t(32, 40, 48, 56, 64, 80, 96, 112, 128,\n");
			Console::OutputString("\t\t\t 160, 192, 224, 256 or 320, default: 192)\n\n");
		}
		else if (helpenc == "FAAC" || helpenc == "faac")
		{
			Console::OutputString("Options for FAAC AAC/MP4 encoder:\n\n");
			Console::OutputString("\t-q <quality>\t\t\t(10 - 500, default: 100, VBR mode)\n");
			Console::OutputString("\t-b <bitrate per channel>\t(8 - 256, default: 64, ABR mode)\n");
			Console::OutputString("\t-mp4\t\t\t\t(use MP4 container format)\n\n");
		}
		else if (helpenc == "FLAC" || helpenc == "flac")
		{
			Console::OutputString("Options for FLAC encoder:\n\n");
			Console::OutputString("\t-b <blocksize>\t\t\t(192 - 32768, default: 4608)\n");
			Console::OutputString("\t-ms\t\t\t\t(use mid-side stereo)\n");
			Console::OutputString("\t-l <max LPC order>\t\t(0 - 32, default: 8)\n");
			Console::OutputString("\t-q <QLP coeff precision>\t(0 - 16, default: 0)\n");
			Console::OutputString("\t-extc\t\t\t\t(do exhaustive QLP coeff optimization)\n");
			Console::OutputString("\t-extm\t\t\t\t(do exhaustive model search)\n");
			Console::OutputString("\t-r <min Rice>,<max Rice>\t(0 - 16, default: 3,3)\n\n");
		}
		else if (helpenc == "TVQ" || helpenc == "tvq")
		{
			Console::OutputString("Options for TwinVQ encoder:\n\n");
			Console::OutputString("\t-b <bitrate per channel>\t(24, 32 or 48, default: 48)\n");
			Console::OutputString("\t-c <preselection candidates>\t(4, 8, 16 or 32, default: 32)\n\n");
		}
		else if (helpenc == "WAVE" || helpenc == "wave")
		{
			Console::OutputString("No options can be configured for the WAVE Out filter!\n\n");
		}
		else
		{
			Console::OutputString(String("Encoder ").Append(helpenc).Append(" is not supported by ").Append(BonkEnc::appName).Append("!\n\n"));
		}
	}
}
