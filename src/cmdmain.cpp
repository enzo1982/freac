 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/main.h>
#include <smooth/args.h>

#include <cmdmain.h>
#include <joblist.h>
#include <dllinterfaces.h>

using namespace smooth::System;

Int smooth::Main(const Array<String> &args)
{
	BonkEnc::BonkEncCommandline	*app = new BonkEnc::BonkEncCommandline(args);

	Object::DeleteObject(app);

	return 0;
}

BonkEnc::BonkEncCommandline::BonkEncCommandline(const Array<String> &arguments) : args(arguments)
{
	currentConfig->enable_console = true;
	currentConfig->appMain = this;

	if (currentConfig->language == NIL) i18n->ActivateLanguage("internal");

	i18n->ActivateLanguage(currentConfig->language);

	InitCDRip();

	Bool		 quiet		= ScanForParameter("-quiet", NULL);
	Bool		 cddb		= ScanForParameter("-cddb", NULL);
	Array<String>	 files;
	String		 encoder	= "LAME";
	String		 helpenc;
	String		 outdir		= ".";
	String		 outfile;
	String		 pattern	= "<artist> - <title>";
	String		 cdDrive	= "0";
	String		 tracks;
	String		 timeout	= "120";

	ScanForParameter("-e", &encoder);
	ScanForParameter("-h", &helpenc);
	ScanForParameter("-d", &outdir);
	ScanForParameter("-o", &outfile);
	ScanForParameter("-p", &pattern);
	ScanForParameter("-cd", &cdDrive);
	ScanForParameter("-track", &tracks);
	ScanForParameter("-t", &timeout);

	ScanForFiles(&files);

	if (currentConfig->enable_cdrip && ex_CR_GetNumCDROM() > 0)
	{
		currentConfig->cdrip_activedrive = cdDrive.ToInt();

		if (currentConfig->cdrip_activedrive >= ex_CR_GetNumCDROM())
		{
			Console::OutputString(String("Warning: Drive #").Append(cdDrive).Append(" does not exist. Using first drive.\n"));

			currentConfig->cdrip_activedrive = 0;
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

	encoder = encoder.ToUpper();

	if (files.Length() == 0 ||
	    helpenc != NIL ||
	    !(encoder == "LAME" || encoder == "VORBIS" || encoder == "BONK" || encoder == "BLADE" || encoder == "FAAC" || encoder == "FLAC" || encoder == "TVQ" || encoder == "WAVE") ||
	    (files.Length() > 1 && outfile != NIL))
	{
		ShowHelp(helpenc);

		return;
	}

	joblist	= new JobList(Point(0, 0), Size(0, 0));

	bool	 broken = false;

	if (((encoder == "LAME" || encoder == "lame")	  && !currentConfig->enable_lame)   ||
	    ((encoder == "VORBIS" || encoder == "vorbis") && !currentConfig->enable_vorbis) ||
	    ((encoder == "BONK" || encoder == "bonk")	  && !currentConfig->enable_bonk)   ||
	    ((encoder == "BLADE" || encoder == "blade")	  && !currentConfig->enable_blade)  ||
	    ((encoder == "FAAC" || encoder == "faac")	  && !currentConfig->enable_faac)   ||
	    ((encoder == "FLAC" || encoder == "flac")	  && !currentConfig->enable_flac)   ||
	    ((encoder == "TVQ" || encoder == "tvq")	  && !currentConfig->enable_tvq))
	{
		Console::OutputString(String("Encoder ").Append(encoder).Append(" is not available!\n\n"));

		broken = true;
	}

	if (encoder == "LAME" || encoder == "lame")
	{
		String	 bitrate = "192";
		String	 quality = "5";
		String	 mode	 = "VBR";

		ScanForParameter("-b", &bitrate);
		ScanForParameter("-q", &quality);
		ScanForParameter("-m", &mode);

		currentConfig->lame_preset = 0;
		currentConfig->lame_set_bitrate = True;

		currentConfig->lame_bitrate    = Math::Max(0, Math::Min(320, (Int) bitrate.ToInt()));
		currentConfig->lame_abrbitrate = Math::Max(0, Math::Min(320, (Int) bitrate.ToInt()));
		currentConfig->lame_vbrquality = Math::Max(0, Math::Min(9, (Int) quality.ToInt())) * 10;

		if (mode == "VBR" || mode == "vbr")	 currentConfig->lame_vbrmode = vbr_mtrh;
		else if (mode == "ABR" || mode == "abr") currentConfig->lame_vbrmode = vbr_abr;
		else if (mode == "CBR" || mode == "cbr") currentConfig->lame_vbrmode = vbr_off;

		currentConfig->encoder = ENCODER_LAMEENC;
	}
	else if (encoder == "VORBIS" || encoder == "vorbis")
	{
		String	 bitrate = "192";
		String	 quality = "60";

		if (ScanForParameter("-b", &bitrate))		currentConfig->vorbis_mode = 1;
		else if (ScanForParameter("-q", &quality))	currentConfig->vorbis_mode = 0;
		else						currentConfig->vorbis_mode = 0;

		currentConfig->vorbis_quality = Math::Max(0, Math::Min(100, (Int) quality.ToInt()));
		currentConfig->vorbis_bitrate = Math::Max(45, Math::Min(500, (Int) bitrate.ToInt()));

		currentConfig->encoder = ENCODER_VORBISENC;
	}
	else if (encoder == "BONK" || encoder == "bonk")
	{
		String	 quantization = "0.4";
		String	 predictor    = "32";
		String	 downsampling = "2";

		ScanForParameter("-q", &quantization);
		ScanForParameter("-p", &predictor);
		ScanForParameter("-r", &downsampling);

		currentConfig->bonk_jstereo	 = ScanForParameter("-js", NULL);
		currentConfig->bonk_lossless	 = ScanForParameter("-lossless", NULL);

		currentConfig->bonk_quantization = Math::Max(0, Math::Min(40, (Int) Math::Round(quantization.ToFloat() * 20)));
		currentConfig->bonk_predictor	 = Math::Max(0, Math::Min(512, (Int) predictor.ToInt()));
		currentConfig->bonk_downsampling = Math::Max(0, Math::Min(10, (Int) downsampling.ToInt()));

		currentConfig->encoder = ENCODER_BONKENC;
	}
	else if (encoder == "BLADE" || encoder == "blade")
	{
		String	 bitrate = "192";

		ScanForParameter("-b", &bitrate);

		currentConfig->blade_bitrate = Math::Max(32, Math::Min(320, (Int) bitrate.ToInt()));

		currentConfig->encoder = ENCODER_BLADEENC;
	}
	else if (encoder == "FAAC" || encoder == "faac")
	{
		String	 bitrate = "64";
		String	 quality = "100";

		if (ScanForParameter("-b", &bitrate))		currentConfig->faac_set_quality = False;
		else if (ScanForParameter("-q", &quality))	currentConfig->faac_set_quality = True;
		else						currentConfig->faac_set_quality = True;

		currentConfig->faac_enable_mp4	= ScanForParameter("-mp4", NULL);

		currentConfig->faac_aac_quality	= Math::Max(10, Math::Min(500, (Int) quality.ToInt()));
		currentConfig->faac_bitrate	= Math::Max(8, Math::Min(256, (Int) bitrate.ToInt()));

		currentConfig->encoder = ENCODER_FAAC;
	}
	else if (encoder == "FLAC" || encoder == "flac")
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

		currentConfig->flac_preset = -1;

		currentConfig->flac_do_mid_side_stereo		 = ScanForParameter("-ms", NULL);
		currentConfig->flac_do_exhaustive_model_search	 = ScanForParameter("-e", NULL);
		currentConfig->flac_do_qlp_coeff_prec_search	 = ScanForParameter("-p", NULL);

		currentConfig->flac_blocksize			 = Math::Max(192, Math::Min(32768, (Int) blocksize.ToInt()));
		currentConfig->flac_max_lpc_order		 = Math::Max(0, Math::Min(32, (Int) lpc.ToInt()));
		currentConfig->flac_qlp_coeff_precision		 = Math::Max(0, Math::Min(16, (Int) qlp.ToInt()));
		currentConfig->flac_min_residual_partition_order = Math::Max(0, Math::Min(16, (Int) minrice.ToInt()));
		currentConfig->flac_max_residual_partition_order = Math::Max(0, Math::Min(16, (Int) maxrice.ToInt()));

		currentConfig->encoder = ENCODER_FLAC;
	}
	else if (encoder == "TVQ" || encoder == "tvq")
	{
		String	 bitrate    = "48";
		String	 candidates = "32";

		ScanForParameter("-b", &bitrate);
		ScanForParameter("-c", &candidates);

		currentConfig->tvq_presel_candidates = Math::Max(4, Math::Min(32, (Int) candidates.ToInt()));
		currentConfig->tvq_bitrate	     = Math::Max(24, Math::Min(48, (Int) bitrate.ToInt()));

		currentConfig->encoder = ENCODER_TVQ;
	}
	else if (encoder == "WAVE" || encoder == "wave")
	{
		currentConfig->encoder = ENCODER_WAVE;
	}
	else
	{
		Console::OutputString(String("Encoder ").Append(encoder).Append(" is not supported by ").Append(BonkEnc::appName).Append("!\n\n"));

		broken = true;
	}

	if (!broken)
	{
		currentConfig->enc_outdir = outdir;
		currentConfig->enc_filePattern = pattern;

		currentConfig->enable_auto_cddb = cddb;
		currentConfig->enable_cddb_cache = True;
		currentConfig->cdrip_locktray = False;
		currentConfig->cdrip_timeout = timeout.ToInt();

		currentConfig->encodeToSingleFile = False;
		currentConfig->writeToInputDir = False;

		if (!currentConfig->enc_outdir.EndsWith(Directory::GetDirectoryDelimiter())) currentConfig->enc_outdir.Append(Directory::GetDirectoryDelimiter());

		for (Int i = 0; i < files.Length(); i++)
		{
			InStream	*in = new InStream(STREAM_FILE, files.GetNth(i), IS_READ);
			String		 currentFile = files.GetNth(i);

			if (currentFile.StartsWith("/cda"))
			{
				currentFile = String("Audio CD ").Append(String::FromInt(currentConfig->cdrip_activedrive)).Append(" - Track ").Append(currentFile.Tail(currentFile.Length() - 4));
			}

			if (in->GetLastError() != IO_ERROR_OK && !files.GetNth(i).StartsWith("/cda"))
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

			if ((files.GetNth(i).EndsWith(".mp3") && !currentConfig->enable_lame) || (files.GetNth(i).EndsWith(".ogg") && !currentConfig->enable_vorbis) || (files.GetNth(i).EndsWith(".cda") && !currentConfig->enable_cdrip))
			{
				Console::OutputString(String("Cannot process file: ").Append(currentFile).Append("\n"));

				broken = true;

				continue;
			}

			joblist->AddTrackByFileName(files.GetNth(i), outfile);

			if (joblist->GetNOfTracks() > 0)
			{
				if (!quiet) Console::OutputString(String("Processing file: ").Append(currentFile).Append("..."));

				Encode(False);

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

		if (param[0] != '-' && (prevParam[0] != '-' ||
					prevParam == "-quiet" ||
					prevParam == "-cddb" ||
					prevParam == "-js" ||
					prevParam == "-lossless" ||
					prevParam == "-mp4" ||
					prevParam == "-ms" ||
					prevParam == "-extc" ||
					prevParam == "-extm ||"))
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
		ex_CR_SetActiveCDROM(currentConfig->cdrip_activedrive);

		ex_CR_ReadToc();

		Int	 numTocEntries = ex_CR_GetNumTocEntries();

		for (Int i = 1; i <= numTocEntries; i++) (*files).Add(String("/cda").Append(String::FromInt(i)));

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

			for (Int i = first; i <= last; i++) (*files).Add(String("/cda").Append(String::FromInt(i)));
		}
		else
		{
			(*files).Add(String("/cda").Append(current));
		}
	}

	return True;
}

Void BonkEnc::BonkEncCommandline::ShowHelp(const String &helpenc)
{
	if (helpenc == NIL)
	{
		Console::OutputString(String(BonkEnc::appLongName).Append(" ").Append(BonkEnc::version).Append(" command line interface\nCopyright (C) 2001-2013 Robert Kausch\n\n"));
		Console::OutputString("Usage:\tfreaccmd [options] [file(s)]\n\n");
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
			Console::OutputString("No options can be configured for Windows Wave file output!\n\n");
		}
		else
		{
			Console::OutputString(String("Encoder ").Append(helpenc).Append(" is not supported by ").Append(BonkEnc::appName).Append("!\n\n"));
		}
	}
}
