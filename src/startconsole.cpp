 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <startconsole.h>
#include <joblist.h>
#include <dllinterfaces.h>

#include <jobs/job_addfiles.h>

using namespace smooth::IO;
using namespace smooth::System;

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
	currentConfig->enable_console = true;

	BoCA::Config	*componentsConfig = BoCA::Config::Get();

	if (componentsConfig == NIL) i18n->ActivateLanguage("internal");

	i18n->ActivateLanguage(componentsConfig->language);

	/* Don't save configuration settings set via command line.
	 */
	componentsConfig->SetSaveSettingsOnExit(False);

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
	ScanForParameter("-cd", &cdDrive);
	ScanForParameter("-track", &tracks);
	ScanForParameter("-t", &timeout);

	ScanForFiles(&files);

	Registry	&boca = Registry::Get();

	if (componentsConfig->cdrip_numdrives > 0)
	{
		componentsConfig->SetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, cdDrive.ToInt());

		if (componentsConfig->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault) >= componentsConfig->cdrip_numdrives)
		{
			Console::OutputString(String("Warning: Drive #").Append(cdDrive).Append(" does not exist. Using first drive.\n"));

			componentsConfig->SetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, 0);
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
	    !(encoderID == "LAME" || encoderID == "VORBIS" || encoderID == "BONK" || encoderID == "BLADE" || encoderID == "FAAC" || encoderID == "FLAC" || encoderID == "TVQ" || encoderID == "WAVE") ||
	    (files.Length() > 1 && outfile != NIL))
	{
		ShowHelp(helpenc);

		return;
	}

	joblist	= new JobList(Point(0, 0), Size(0, 0));

	Bool	 broken = false;

	if (((encoderID == "LAME")	&& !boca.ComponentExists("lame-out"))	  ||
	    ((encoderID == "VORBIS")	&& !boca.ComponentExists("vorbis-out"))   ||
	    ((encoderID == "BONK")	&& !boca.ComponentExists("bonk-out"))	  ||
	    ((encoderID == "BLADE")	&& !boca.ComponentExists("bladeenc-out")) ||
	    ((encoderID == "FAAC")	&& !boca.ComponentExists("faac-out"))	  ||
	    ((encoderID == "FLAC")	&& !boca.ComponentExists("flac-out"))	  ||
	    ((encoderID == "TVQ")	&& !boca.ComponentExists("twinvq-out")))
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

		componentsConfig->SetIntValue("LAME", "Preset", 0);
		componentsConfig->SetIntValue("LAME", "SetBitrate", True);

		componentsConfig->SetIntValue("LAME", "Bitrate", Math::Max(0, Math::Min(320, bitrate.ToInt())));
		componentsConfig->SetIntValue("LAME", "ABRBitrate", Math::Max(0, Math::Min(320, bitrate.ToInt())));
		componentsConfig->SetIntValue("LAME", "VBRQuality", Math::Max(0, Math::Min(9, quality.ToInt())) * 10);

		if	(mode == "VBR" || mode == "vbr") componentsConfig->SetIntValue("LAME", "VBRMode", 4);
		else if (mode == "ABR" || mode == "abr") componentsConfig->SetIntValue("LAME", "VBRMode", 3);
		else if (mode == "CBR" || mode == "cbr") componentsConfig->SetIntValue("LAME", "VBRMode", 0);

		componentsConfig->encoderID = "lame-out";
	}
	else if (encoderID == "VORBIS")
	{
		String	 bitrate = "192";
		String	 quality = "60";

		if (ScanForParameter("-b", &bitrate))		componentsConfig->SetIntValue("Vorbis", "Mode", 1);
		else if (ScanForParameter("-q", &quality))	componentsConfig->SetIntValue("Vorbis", "Mode", 0);
		else						componentsConfig->SetIntValue("Vorbis", "Mode", 0);

		componentsConfig->SetIntValue("Vorbis", "Quality", Math::Max(0, Math::Min(100, quality.ToInt())));
		componentsConfig->SetIntValue("Vorbis", "Bitrate", Math::Max(45, Math::Min(500, bitrate.ToInt())));

		componentsConfig->encoderID = "vorbis-out";
	}
	else if (encoderID == "BONK")
	{
		String	 quantization = "0.4";
		String	 predictor    = "32";
		String	 downsampling = "2";

		ScanForParameter("-q", &quantization);
		ScanForParameter("-p", &predictor);
		ScanForParameter("-r", &downsampling);

		componentsConfig->SetIntValue("Bonk", "JointStereo", ScanForParameter("-js", NULL));
		componentsConfig->SetIntValue("Bonk", "Lossless", ScanForParameter("-lossless", NULL));

		componentsConfig->SetIntValue("Bonk", "Quantization", Math::Max(0, Math::Min(40, Math::Round(quantization.ToFloat() * 20))));
		componentsConfig->SetIntValue("Bonk", "Predictor", Math::Max(0, Math::Min(512, predictor.ToInt())));
		componentsConfig->SetIntValue("Bonk", "Downsampling", Math::Max(0, Math::Min(10, downsampling.ToInt())));

		componentsConfig->encoderID = "bonk-out";
	}
	else if (encoderID == "BLADE")
	{
		String	 bitrate = "192";

		ScanForParameter("-b", &bitrate);

		componentsConfig->SetIntValue("BladeEnc", "Bitrate", Math::Max(32, Math::Min(320, bitrate.ToInt())));

		componentsConfig->encoderID = "blade-out";
	}
	else if (encoderID == "FAAC")
	{
		String	 bitrate = "64";
		String	 quality = "100";

		if (ScanForParameter("-b", &bitrate))		componentsConfig->SetIntValue("FAAC", "SetQuality", False);
		else if (ScanForParameter("-q", &quality))	componentsConfig->SetIntValue("FAAC", "SetQuality", True);
		else						componentsConfig->SetIntValue("FAAC", "SetQuality", True);

		componentsConfig->SetIntValue("FAAC", "MP4Container", ScanForParameter("-mp4", NULL));

		componentsConfig->SetIntValue("FAAC", "AACQuality", Math::Max(10, Math::Min(500, quality.ToInt())));
		componentsConfig->SetIntValue("FAAC", "Bitrate", Math::Max(8, Math::Min(256, bitrate.ToInt())));

		componentsConfig->encoderID = "faac-out";
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

		componentsConfig->SetIntValue("FLAC", "Preset", -1);

		componentsConfig->SetIntValue("FLAC", "DoMidSideStereo", ScanForParameter("-ms", NULL));
		componentsConfig->SetIntValue("FLAC", "DoExhaustiveModelSearch", ScanForParameter("-e", NULL));
		componentsConfig->SetIntValue("FLAC", "DoQLPCoeffPrecSearch", ScanForParameter("-p", NULL));

		componentsConfig->SetIntValue("FLAC", "Blocksize", Math::Max(192, Math::Min(32768, blocksize.ToInt())));
		componentsConfig->SetIntValue("FLAC", "MaxLPCOrder", Math::Max(0, Math::Min(32, lpc.ToInt())));
		componentsConfig->SetIntValue("FLAC", "QLPCoeffPrecision", Math::Max(0, Math::Min(16, qlp.ToInt())));
		componentsConfig->SetIntValue("FLAC", "MinResidualPartitionOrder", Math::Max(0, Math::Min(16, minrice.ToInt())));
		componentsConfig->SetIntValue("FLAC", "MaxResidualPartitionOrder", Math::Max(0, Math::Min(16, maxrice.ToInt())));

		componentsConfig->encoderID = "flac-out";
	}
	else if (encoderID == "TVQ")
	{
		String	 bitrate    = "48";
		String	 candidates = "32";

		ScanForParameter("-b", &bitrate);
		ScanForParameter("-c", &candidates);

		componentsConfig->SetIntValue("TwinVQ", "PreselectionCandidates", Math::Max(4, Math::Min(32, candidates.ToInt())));
		componentsConfig->SetIntValue("TwinVQ", "Bitrate", Math::Max(24, Math::Min(48, bitrate.ToInt())));

		componentsConfig->encoderID = "twinvq-out";
	}
	else if (encoderID == "WAVE")
	{
		componentsConfig->encoderID = "wave-out";
	}
	else
	{
		Console::OutputString(String("Encoder ").Append(encoderID).Append(" is not supported by BonkEnc!\n\n"));

		broken = true;
	}

	if (!broken)
	{
		componentsConfig->enc_outdir = outdir;
		componentsConfig->enc_filePattern = pattern;

		componentsConfig->enable_auto_cddb = cddb;
		componentsConfig->enable_cddb_cache = True;

		componentsConfig->SetIntValue("CDRip", "LockTray", False);
		componentsConfig->cdrip_timeout = timeout.ToInt();

		componentsConfig->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, False);
		componentsConfig->SetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, False);

		if (!componentsConfig->enc_outdir.EndsWith(Directory::GetDirectoryDelimiter())) componentsConfig->enc_outdir.Append(Directory::GetDirectoryDelimiter());

		for (Int i = 0; i < files.Length(); i++)
		{
			InStream	*in = new InStream(STREAM_FILE, files.GetNth(i), IS_READ);
			String		 currentFile = files.GetNth(i);

			if (currentFile.StartsWith("cdda://"))
			{
				currentFile = String("Audio CD ").Append(String::FromInt(componentsConfig->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault))).Append(" - Track ").Append(currentFile.Tail(currentFile.Length() - 4));
			}

			if (in->GetLastError() != IO_ERROR_OK && !files.GetNth(i).StartsWith("cdda://"))
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

			if ((files.GetNth(i).EndsWith(".mp3") && !boca.ComponentExists("lame-in"))   ||
			    (files.GetNth(i).EndsWith(".ogg") && !boca.ComponentExists("vorbis-in")) ||
			    (files.GetNth(i).EndsWith(".cda") && !boca.ComponentExists("cdrip-in")))
			{
				Console::OutputString(String("Cannot process file: ").Append(currentFile).Append("\n"));

				broken = true;

				continue;
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

				encoder->Encode(joblist, False);

				joblist->RemoveNthTrack(0);

				if (!quiet) Console::OutputString("done.\n");
			}
		}
	}

	delete joblist;
}

BonkEnc::BonkEncCommandline::~BonkEncCommandline()
{
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

				foreach (File entry, array) (*files).Add(entry);
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

			foreach (String track, tracks) (*files).Add(track);

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

			for (Int i = first; i <= last; i++) (*files).Add(String("cdda://")
									.Append(String::FromInt(BoCA::Config::Get()->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault)))
									.Append("/")
									.Append(String::FromInt(i)));
		}
		else
		{
			(*files).Add(String("cdda://")
				    .Append(String::FromInt(BoCA::Config::Get()->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault)))
				    .Append("/")
				    .Append(current));
		}
	}

	return True;
}

Void BonkEnc::BonkEncCommandline::ShowHelp(const String &helpenc)
{
	if (helpenc == NIL)
	{
		Console::OutputString(String(BonkEnc::appLongName).Append(" ").Append(BonkEnc::version).Append(" command line interface\nCopyright (C) 2001-2009 Robert Kausch\n\n"));
		Console::OutputString("Usage:\tBEcmd [options] [file(s)]\n\n");
		Console::OutputString("\t-e <encoder>\tSpecify the encoder to use (default is LAME)\n");
		Console::OutputString("\t-h <encoder>\tPrint help for encoder specific options\n\n");
		Console::OutputString("\t-d <outdir>\tSpecify output directory for encoded files\n");
		Console::OutputString("\t-o <outfile>\tSpecify output file name in single file mode\n");
		Console::OutputString("\t-p <pattern>\tSpecify output file name pattern\n\n");
		Console::OutputString("\t-cd <drive>\tSpecify active CD drive (0..n)\n");
		Console::OutputString("\t-track <track>\tSpecify input track(s) to rip (e.g. 1-5,7,9 or 'all')\n");
		Console::OutputString("\t-t <timeout>\tTimeout for CD track ripping (default is 120 seconds)\n");
		Console::OutputString("\t-cddb\t\tEnable CDDB database lookup\n\n");
		Console::OutputString("\t-quiet\t\tDo not print any messages\n\n");
		Console::OutputString("<encoder> can be one of LAME, VORBIS, BONK, BLADE, FAAC, FLAC, TVQ or WAVE.\n\n");
		Console::OutputString("Default for <pattern> is \"<artist> - <title>\".\n\n");
	}
	else
	{
		Console::OutputString(String(BonkEnc::appLongName).Append(" ").Append(BonkEnc::version).Append(" command line interface\nCopyright (C) 2001-2009 Robert Kausch\n\n"));

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
			Console::OutputString(String("Encoder ").Append(helpenc).Append(" is not supported by BonkEnc!\n\n"));
		}
	}
}
