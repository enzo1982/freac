 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <startconsole.h>
#include <joblist.h>
#include <config.h>

#include <engine/converter.h>

#include <jobs/job_addfiles.h>

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

	Bool		 quiet		= ScanForParameter("-quiet", NULL);
	Bool		 cddb		= ScanForParameter("-cddb", NULL);
	Array<String>	 files;
	String		 encoderID	= "LAME";
	String		 helpenc;
	String		 outdir		= ".";
	String		 outfile;
	String		 pattern	= "<artist> - <title>";
	String		 cdDrive	= "0";
	String		 tracks;
	String		 timeout	= "120";

	ScanForParameter("-e", &encoderID);
	ScanForParameter("-h", &helpenc);
	ScanForParameter("-d", &outdir);
	ScanForParameter("-o", &outfile);
	ScanForParameter("-p", &pattern);

	if (config->cdrip_numdrives > 0)
	{
		ScanForParameter("-cd", &cdDrive);
		ScanForParameter("-track", &tracks);
		ScanForParameter("-t", &timeout);
	}

	ScanForFiles(&files);

	if (config->cdrip_numdrives > 0)
	{
		config->SetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, cdDrive.ToInt());

		if (config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault) >= config->cdrip_numdrives)
		{
			Console::OutputString(String("Warning: Drive #").Append(cdDrive).Append(" does not exist. Using first drive.\n"));

			config->SetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, 0);
		}

		if (!TracksToFiles(tracks, &files))
		{
			Console::OutputString("Error: Invalid track(s) specified after -track.\n");

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

	joblist	= new JobList(Point(0, 0), Size(0, 0));

	Bool	 broken = false;

	if (((encoderID == "LAME")   && !boca.ComponentExists("lame-out"))     ||
	    ((encoderID == "VORBIS") && !boca.ComponentExists("vorbis-out"))   ||
	    ((encoderID == "BONK")   && !boca.ComponentExists("bonk-out"))     ||
	    ((encoderID == "BLADE")  && !boca.ComponentExists("bladeenc-out")) ||
	    ((encoderID == "FAAC")   && !boca.ComponentExists("faac-out"))     ||
	    ((encoderID == "FLAC")   && !boca.ComponentExists("flac-out"))     ||
	    ((encoderID == "TVQ")    && !boca.ComponentExists("twinvq-out")))
	{
		Console::OutputString(String("Encoder ").Append(encoderID).Append(" is not available!\n\n"));

		broken = true;
	}

	if (encoderID == "LAME")
	{
		String	 bitrate = "192";
		String	 quality = "5";
		String	 mode	 = "VBR";

		ScanForParameter("-b", &bitrate);
		ScanForParameter("-q", &quality);
		ScanForParameter("-m", &mode);

		config->SetIntValue("LAME", "Preset", 0);
		config->SetIntValue("LAME", "SetBitrate", True);

		config->SetIntValue("LAME", "Bitrate", Math::Max(0, Math::Min(320, (Int) bitrate.ToInt())));
		config->SetIntValue("LAME", "ABRBitrate", Math::Max(0, Math::Min(320, (Int) bitrate.ToInt())));
		config->SetIntValue("LAME", "VBRQuality", Math::Max(0, Math::Min(9, (Int) quality.ToInt())) * 10);

		if	(mode == "VBR" || mode == "vbr") config->SetIntValue("LAME", "VBRMode", 4);
		else if (mode == "ABR" || mode == "abr") config->SetIntValue("LAME", "VBRMode", 3);
		else if (mode == "CBR" || mode == "cbr") config->SetIntValue("LAME", "VBRMode", 0);

		config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, "lame-out");
	}
	else if (encoderID == "VORBIS")
	{
		String	 bitrate = "192";
		String	 quality = "60";

		if	(ScanForParameter("-b", &bitrate)) config->SetIntValue("Vorbis", "Mode", 1);
		else if (ScanForParameter("-q", &quality)) config->SetIntValue("Vorbis", "Mode", 0);
		else					   config->SetIntValue("Vorbis", "Mode", 0);

		config->SetIntValue("Vorbis", "Quality", Math::Max(0, Math::Min(100, (Int) quality.ToInt())));
		config->SetIntValue("Vorbis", "Bitrate", Math::Max(45, Math::Min(500, (Int) bitrate.ToInt())));

		config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, "vorbis-out");
	}
	else if (encoderID == "BONK")
	{
		String	 quantization = "0.4";
		String	 predictor    = "32";
		String	 downsampling = "2";

		ScanForParameter("-q", &quantization);
		ScanForParameter("-p", &predictor);
		ScanForParameter("-r", &downsampling);

		config->SetIntValue("Bonk", "JointStereo", ScanForParameter("-js", NULL));
		config->SetIntValue("Bonk", "Lossless", ScanForParameter("-lossless", NULL));

		config->SetIntValue("Bonk", "Quantization", Math::Max(0, Math::Min(40, (Int) Math::Round(quantization.ToFloat() * 20))));
		config->SetIntValue("Bonk", "Predictor", Math::Max(0, Math::Min(512, (Int) predictor.ToInt())));
		config->SetIntValue("Bonk", "Downsampling", Math::Max(0, Math::Min(10, (Int) downsampling.ToInt())));

		config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, "bonk-out");
	}
	else if (encoderID == "BLADE")
	{
		String	 bitrate = "192";

		ScanForParameter("-b", &bitrate);

		config->SetIntValue("BladeEnc", "Bitrate", Math::Max(32, Math::Min(320, (Int) bitrate.ToInt())));

		config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, "blade-out");
	}
	else if (encoderID == "FAAC")
	{
		String	 bitrate = "64";
		String	 quality = "100";

		if	(ScanForParameter("-b", &bitrate)) config->SetIntValue("FAAC", "SetQuality", False);
		else if (ScanForParameter("-q", &quality)) config->SetIntValue("FAAC", "SetQuality", True);
		else					   config->SetIntValue("FAAC", "SetQuality", True);

		config->SetIntValue("FAAC", "MP4Container", ScanForParameter("-mp4", NULL));

		config->SetIntValue("FAAC", "AACQuality", Math::Max(10, Math::Min(500, (Int) quality.ToInt())));
		config->SetIntValue("FAAC", "Bitrate", Math::Max(8, Math::Min(256, (Int) bitrate.ToInt())));

		config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, "faac-out");
	}
	else if (encoderID == "FLAC")
	{
		String	 blocksize = "4608";
		String	 lpc = "8";
		String	 qlp = "0";
		String	 rice = "3,3";
		String	 minrice;
		String	 maxrice;

		ScanForParameter("-b", &blocksize);
		ScanForParameter("-l", &lpc);
		ScanForParameter("-q", &qlp);
		ScanForParameter("-r", &rice);

		Int	 i = 0;
		Int	 j = 0;

		for (i = 0; i < rice.Length(); i++)	{ if (rice[i] == ',') break; minrice[i] = rice[i]; }
		for (j = i + 1; j < rice.Length(); j++)	{ maxrice[j - i - 1] = rice[j]; }

		config->SetIntValue("FLAC", "Preset", -1);

		config->SetIntValue("FLAC", "DoMidSideStereo", ScanForParameter("-ms", NULL));
		config->SetIntValue("FLAC", "DoExhaustiveModelSearch", ScanForParameter("-e", NULL));
		config->SetIntValue("FLAC", "DoQLPCoeffPrecSearch", ScanForParameter("-p", NULL));

		config->SetIntValue("FLAC", "Blocksize", Math::Max(192, Math::Min(32768, (Int) blocksize.ToInt())));
		config->SetIntValue("FLAC", "MaxLPCOrder", Math::Max(0, Math::Min(32, (Int) lpc.ToInt())));
		config->SetIntValue("FLAC", "QLPCoeffPrecision", Math::Max(0, Math::Min(16, (Int) qlp.ToInt())));
		config->SetIntValue("FLAC", "MinResidualPartitionOrder", Math::Max(0, Math::Min(16, (Int) minrice.ToInt())));
		config->SetIntValue("FLAC", "MaxResidualPartitionOrder", Math::Max(0, Math::Min(16, (Int) maxrice.ToInt())));

		config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, "flac-out");
	}
	else if (encoderID == "TVQ")
	{
		String	 bitrate    = "48";
		String	 candidates = "32";

		ScanForParameter("-b", &bitrate);
		ScanForParameter("-c", &candidates);

		config->SetIntValue("TwinVQ", "PreselectionCandidates", Math::Max(4, Math::Min(32, (Int) candidates.ToInt())));
		config->SetIntValue("TwinVQ", "Bitrate", Math::Max(24, Math::Min(48, (Int) bitrate.ToInt())));

		config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, "twinvq-out");
	}
	else if (encoderID == "WAVE")
	{
		config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, "wave-out");
	}
	else
	{
		Console::OutputString(String("Encoder ").Append(encoderID).Append(" is not supported by ").Append(BonkEnc::appName).Append("!\n\n"));

		broken = true;
	}

	if (!broken)
	{
		if (!outdir.EndsWith(Directory::GetDirectoryDelimiter())) outdir.Append(Directory::GetDirectoryDelimiter());

		config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, outdir);
		config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, pattern);

		config->SetIntValue(Config::CategoryFreedbID, Config::FreedbAutoQueryID, cddb);
		config->SetIntValue(Config::CategoryFreedbID, Config::FreedbEnableCacheID, True);

		config->SetIntValue("CDRip", "LockTray", False);
		config->cdrip_timeout = timeout.ToInt();

		config->SetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, False);
		config->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, False);

		if (files.Length() > 1 && outfile != NIL)
		{
			config->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, True);
			config->SetStringValue(Config::CategorySettingsID, Config::SettingsSingleFilenameID, outfile);

			encoder->onEncodeTrack.Connect(&BonkEncCommandline::OnEncodeTrack, this);

			Array<String>	 jobFiles;

			for (Int i = 0; i < files.Length(); i++)
			{
				InStream	*in = new InStream(STREAM_FILE, files.GetNth(i), IS_READ);
				String		 currentFile = files.GetNth(i);

				if (currentFile.StartsWith("device://cdda:"))
				{
					currentFile = String("Audio CD ").Append(String::FromInt(config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault))).Append(" - Track ").Append(currentFile.Tail(currentFile.Length() - 16));
				}

				if (in->GetLastError() != IO_ERROR_OK && !files.GetNth(i).StartsWith("device://"))
				{
					delete in;

					Console::OutputString(String("File not found: ").Append(files.GetNth(i)).Append("\n"));

					broken = true;

					continue;
				}
				else
				{
					delete in;
				}

				jobFiles.Add(files.GetNth(i));
			}

			Job	*job = new JobAddFiles(jobFiles);

			job->Schedule();

			while (Job::GetPlannedJobs().Length() > 0) S::System::System::Sleep(10);
			while (Job::GetRunningJobs().Length() > 0) S::System::System::Sleep(10);

			if (joblist->GetNOfTracks() > 0)
			{
				encoder->Convert(joblist, False);

				if (!quiet) Console::OutputString("done.\n");
			}
			else
			{
				Console::OutputString("Could not process input files!\n");

				broken = true;
			}
		}
		else
		{
			for (Int i = 0; i < files.Length(); i++)
			{
				InStream	*in = new InStream(STREAM_FILE, files.GetNth(i), IS_READ);
				String		 currentFile = files.GetNth(i);

				if (currentFile.StartsWith("device://cdda:"))
				{
					currentFile = String("Audio CD ").Append(String::FromInt(config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault))).Append(" - Track ").Append(currentFile.Tail(currentFile.Length() - 16));
				}

				if (in->GetLastError() != IO_ERROR_OK && !files.GetNth(i).StartsWith("device://"))
				{
					delete in;

					Console::OutputString(String("File not found: ").Append(files.GetNth(i)).Append("\n"));

					broken = true;

					continue;
				}
				else
				{
					delete in;
				}

				Array<String>	 jobFiles;

				jobFiles.Add(files.GetNth(i));

				Job	*job = new JobAddFiles(jobFiles);

				job->Schedule();

				while (Job::GetPlannedJobs().Length() > 0) S::System::System::Sleep(10);
				while (Job::GetRunningJobs().Length() > 0) S::System::System::Sleep(10);

				if (joblist->GetNOfTracks() > 0)
				{
					if (!quiet) Console::OutputString(String("Processing file: ").Append(currentFile).Append("..."));

					Track	 track = joblist->GetNthTrack(0);

					track.outfile = outfile;

					joblist->UpdateTrackInfo(track);

					encoder->Convert(joblist, False);

					joblist->RemoveNthTrack(0);

					if (!quiet) Console::OutputString("done.\n");
				}
				else
				{
					Console::OutputString(String("Could not process file: ").Append(currentFile).Append("\n"));

					broken = true;

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

Void BonkEnc::BonkEncCommandline::OnEncodeTrack(const Track &track, const String &decoderName, Int mode)
{
	static Bool	 firstTime = True;

	Bool	 quiet = ScanForParameter("-quiet", NULL);

	if (!quiet)
	{
		if (!firstTime) Console::OutputString("done.\n");
		else		firstTime = False;

		Console::OutputString(String("Processing file: ").Append(track.origFilename).Append("..."));
	}
}

Bool BonkEnc::BonkEncCommandline::ScanForParameter(const String &param, String *option)
{
	for (Int i = 0; i < args.Length(); i++)
	{
		if (args.GetNth(i) == param)
		{
			if (option != NULL) *option = args.GetNth(i + 1);

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

		if (param[0] != '-' && (prevParam[0] != '-'	 ||
					prevParam == "-quiet"	 ||
					prevParam == "-cddb"	 ||
					prevParam == "-js"	 ||
					prevParam == "-lossless" ||
					prevParam == "-mp4"	 ||
					prevParam == "-ms"	 ||
					prevParam == "-extc"	 ||
					prevParam == "-extm"))
		{
			if (param.Find("*") != -1 || param.Find("?") != -1)
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
	if (tracks == "all")
	{
		Registry		&boca = Registry::Get();
		DeviceInfoComponent	*info = (DeviceInfoComponent *) boca.CreateComponentByID("cdrip-info");

		if (info != NIL)
		{
			const Array<String>	&tracks = info->GetNthDeviceTrackList(BoCA::Config::Get()->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault));

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

		if (rest.Find(",") != -1)
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

		if (current.Find("-") != -1)
		{
			Int	 dash = current.Find("-");
			Int	 first = current.Head(dash).ToInt();
			Int	 last = current.Tail(current.Length() - dash - 1).ToInt();

			for (Int i = first; i <= last; i++) (*files).Add(String("device://cdda:")
									.Append(String::FromInt(BoCA::Config::Get()->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault)))
									.Append("/")
									.Append(String::FromInt(i)));
		}
		else
		{
			(*files).Add(String("device://cdda:")
				    .Append(String::FromInt(BoCA::Config::Get()->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault)))
				    .Append("/")
				    .Append(current));
		}
	}

	return True;
}

Void BonkEnc::BonkEncCommandline::ShowHelp(const String &helpenc)
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (helpenc == NIL)
	{
		Console::OutputString(String(BonkEnc::appLongName).Append(" ").Append(BonkEnc::version).Append(" command line interface\nCopyright (C) 2001-2013 Robert Kausch\n\n"));
		Console::OutputString("Usage:\tfreaccmd [options] [file(s)]\n\n");
		Console::OutputString("\t-e <encoder>\tSpecify the encoder to use (default is LAME)\n");
		Console::OutputString("\t-h <encoder>\tPrint help for encoder specific options\n\n");
		Console::OutputString("\t-d <outdir>\tSpecify output directory for encoded files\n");
		Console::OutputString("\t-o <outfile>\tSpecify output file name in single file mode\n");
		Console::OutputString("\t-p <pattern>\tSpecify output file name pattern\n\n");

		if (config->cdrip_numdrives > 0)
		{
			Console::OutputString("\t-cd <drive>\tSpecify active CD drive (0..n)\n");
			Console::OutputString("\t-track <track>\tSpecify input track(s) to rip (e.g. 1-5,7,9 or 'all')\n");
			Console::OutputString("\t-t <timeout>\tTimeout for CD track ripping (default is 120 seconds)\n");
			Console::OutputString("\t-cddb\t\tEnable CDDB database lookup\n\n");
		}

		Console::OutputString("\t-quiet\t\tDo not print any messages\n\n");
		Console::OutputString("<encoder> can be one of LAME, VORBIS, BONK, BLADE, FAAC, FLAC, TVQ or WAVE.\n\n");
		Console::OutputString("Default for <pattern> is \"<artist> - <title>\".\n\n");
	}
	else
	{
		Console::OutputString(String(BonkEnc::appLongName).Append(" ").Append(BonkEnc::version).Append(" command line interface\nCopyright (C) 2001-2013 Robert Kausch\n\n"));

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
