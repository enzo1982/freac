 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
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
#include <console.h>
#include <joblist.h>

Int smooth::Main(Array<String> &args)
{
	debug_out = new bonkEncDebug("BonkEnc.log");

	debug_out->OutputLine("");
	debug_out->OutputLine("=========================================");
	debug_out->OutputLine("= Starting BonkEnc command line tool... =");
	debug_out->OutputLine("=========================================");
	debug_out->OutputLine("");

	bonkEncCommandline	*app = new bonkEncCommandline(args);

	delete app;

	debug_out->OutputLine("");
	debug_out->OutputLine("======================================");
	debug_out->OutputLine("= Leaving BonkEnc command line tool! =");
	debug_out->OutputLine("======================================");

	delete debug_out;

	return 0;
}

bonkEncCommandline::bonkEncCommandline(Array<String> &arguments) : args(arguments)
{
	currentConfig->enable_console = true;
	currentConfig->appMain = this;

	joblist	= new JobList(Point(0, 0), Size(0, 0));

	bool		 quiet		= ScanForParameter("-quiet", NULL);
	Array<String>	 files;
	String		 encoder	= "FAAC";
	String		 helpenc	= "";
	String		 outdir		= ".";
	String		 outfile	= "";

	ScanForParameter("-e", &encoder);
	ScanForParameter("-h", &helpenc);
	ScanForParameter("-d", &outdir);
	ScanForParameter("-o", &outfile);

	ScanForFiles(&files);

	bonkEncConsole	*con;

	if (!quiet)	con = new bonkEncConsole(String("BonkEnc ").Append(bonkEnc::version));
	else		con = new bonkEncConsole((char *) NIL);

	con->OutputString(String("BonkEnc Audio Encoder ").Append(bonkEnc::version).Append(" command line interface\nCopyright (C) 2001-2005 Robert Kausch\n\n"));

	if ((files.GetNOfEntries() == 0 && helpenc == "") || !(encoder == "LAME" || encoder == "VORBIS" || encoder == "BONK" || encoder == "BLADE" || encoder == "FAAC" || encoder == "TVQ" || encoder == "WAVE" || encoder == "lame" || encoder == "vorbis" || encoder == "bonk" || encoder == "blade" || encoder == "faac" || encoder == "tvq" || encoder == "wave") || (files.GetNOfEntries() > 1 && outfile != ""))
	{
		con->OutputString("Usage:\tBEcmd [options] [file(s)]\n\n");
		con->OutputString("\t-e <encoder>\tSpecify the encoder to use (default is FAAC)\n");
		con->OutputString("\t-d <outdir>\tSpecify output directory for encoded files\n");
		con->OutputString("\t-o <outfile>\tSpecify output file name in single file mode\n");
		con->OutputString("\t-h <encoder>\tPrint help for encoder specific options\n");
		con->OutputString("\t-quiet\t\tDo not print any messages\n\n");
		con->OutputString("<encoder> can be one of LAME, VORBIS, BONK, BLADE, FAAC, TVQ or WAVE.\n\n");
	}
	else if (helpenc != "")
	{
		if (helpenc == "LAME" || helpenc == "lame")
		{
			con->OutputString("Options for LAME MP3 encoder:\n\n");
			con->OutputString("\t-m <mode>\t\t(CBR, VBR or ABR, default: VBR)\n");
			con->OutputString("\t-b <CBR/ABR bitrate>\t(8 - 320, default: 192)\n");
			con->OutputString("\t-q <VBR quality>\t(0 = best, 9 = worst, default: 5)\n\n");
		}
		else if (helpenc == "VORBIS" || helpenc == "vorbis")
		{
			con->OutputString("Options for Ogg Vorbis encoder:\n\n");
			con->OutputString("\t-q <quality>\t\t(0 - 100, default: 60, VBR mode)\n");
			con->OutputString("\t-b <target bitrate>\t(45 - 500, default: 192, ABR mode)\n\n");
		}
		else if (helpenc == "BONK" || helpenc == "bonk")
		{
			con->OutputString("Options for Bonk encoder:\n\n");
			con->OutputString("\t-q <quantization factor>\t(0 - 2, default: 0.4)\n");
			con->OutputString("\t-p <predictor size>\t\t(0 - 512, default: 32)\n");
			con->OutputString("\t-r <downsampling ratio>\t\t(1 - 10, default: 2)\n");
			con->OutputString("\t-js\t\t\t\t(use Joint Stereo)\n");
			con->OutputString("\t-lossless\t\t\t(use lossless compression)\n\n");
		}
		else if (helpenc == "BLADE" || helpenc == "blade")
		{
			con->OutputString("Options for BladeEnc encoder:\n\n");
			con->OutputString("\t-b <bitrate>\t(32, 40, 48, 56, 64, 80, 96, 112, 128,\n");
			con->OutputString("\t\t\t 160, 192, 224, 256 or 320, default: 192)\n\n");
		}
		else if (helpenc == "FAAC" || helpenc == "faac")
		{
			con->OutputString("Options for FAAC AAC/MP4 encoder:\n\n");
			con->OutputString("\t-q <quality>\t\t\t(10 - 500, default: 100, VBR mode)\n");
			con->OutputString("\t-b <bitrate per channel>\t(8 - 256, default: 64, ABR mode)\n");
			con->OutputString("\t-mp4\t\t\t\t(use MP4 container format)\n\n");
		}
		else if (helpenc == "TVQ" || helpenc == "tvq")
		{
			con->OutputString("Options for TwinVQ encoder:\n\n");
			con->OutputString("\t-b <bitrate per channel>\t(24, 32 or 48, default: 48)\n");
			con->OutputString("\t-c <preselection candidates>\t(4, 8, 16 or 32, default: 32)\n\n");
		}
		else if (helpenc == "WAVE" || helpenc == "wave")
		{
			con->OutputString("No options can be configured for the WAVE Out filter!\n\n");
		}
		else
		{
			con->OutputString(String("Encoder ").Append(helpenc).Append(" is not supported by BonkEnc!\n\n"));
		}
	}
	else
	{
		bool	 broken = false;

		if (((encoder == "LAME" || encoder == "lame")	  && !currentConfig->enable_lame)   ||
		    ((encoder == "VORBIS" || encoder == "vorbis") && !currentConfig->enable_vorbis) ||
		    ((encoder == "BONK" || encoder == "bonk")	  && !currentConfig->enable_bonk)   ||
		    ((encoder == "BLADE" || encoder == "blade")	  && !currentConfig->enable_blade)  ||
		    ((encoder == "FAAC" || encoder == "faac")	  && !currentConfig->enable_faac)   ||
		    ((encoder == "TVQ" || encoder == "tvq")	  && !currentConfig->enable_tvq))
		{
			con->OutputString(String("Encoder ").Append(encoder).Append(" is not available!\n\n"));

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

			currentConfig->lame_set_bitrate = True;

			currentConfig->lame_bitrate    = Math::Max(0, Math::Min(320, bitrate.ToInt()));
			currentConfig->lame_abrbitrate = Math::Max(0, Math::Min(320, bitrate.ToInt()));
			currentConfig->lame_vbrquality = Math::Max(0, Math::Min(9, quality.ToInt()));

			if (mode == "VBR" || mode == "vbr")	 currentConfig->lame_vbrmode = 2;
			else if (mode == "ABR" || mode == "abr") currentConfig->lame_vbrmode = 3;
			else if (mode == "CBR" || mode == "cbr") currentConfig->lame_vbrmode = 0;

			currentConfig->encoder = ENCODER_LAMEENC;
		}
		else if (encoder == "VORBIS" || encoder == "vorbis")
		{
			String	 bitrate = "192";
			String	 quality = "60";

			if (ScanForParameter("-b", &bitrate))		currentConfig->vorbis_mode = 1;
			else if (ScanForParameter("-q", &quality))	currentConfig->vorbis_mode = 0;
			else						currentConfig->vorbis_mode = 0;

			currentConfig->vorbis_quality = Math::Max(0, Math::Min(100, quality.ToInt()));
			currentConfig->vorbis_bitrate = Math::Max(45, Math::Min(500, bitrate.ToInt()));

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

			currentConfig->bonk_jstereo = ScanForParameter("-js", NULL);
			currentConfig->bonk_lossless = ScanForParameter("-lossless", NULL);

			currentConfig->bonk_quantization = Math::Max(0, Math::Min(40, Math::Round(quantization.ToFloat() * 20)));
			currentConfig->bonk_predictor	 = Math::Max(0, Math::Min(512, predictor.ToInt()));
			currentConfig->bonk_downsampling = Math::Max(0, Math::Min(10, downsampling.ToInt()));

			currentConfig->encoder = ENCODER_BONKENC;
		}
		else if (encoder == "BLADE" || encoder == "blade")
		{
			String	 bitrate = "192";

			ScanForParameter("-b", &bitrate);

			currentConfig->blade_bitrate = Math::Max(32, Math::Min(320, bitrate.ToInt()));

			currentConfig->encoder = ENCODER_BLADEENC;
		}
		else if (encoder == "FAAC" || encoder == "faac")
		{
			String	 bitrate = "64";
			String	 quality = "100";

			if (ScanForParameter("-b", &bitrate))		currentConfig->faac_set_quality = False;
			else if (ScanForParameter("-q", &quality))	currentConfig->faac_set_quality = True;
			else						currentConfig->faac_set_quality = True;

			currentConfig->faac_enable_mp4 = ScanForParameter("-mp4", NULL);

			currentConfig->faac_aac_quality = Math::Max(10, Math::Min(500, quality.ToInt()));
			currentConfig->faac_bitrate	= Math::Max(8, Math::Min(256, bitrate.ToInt()));

			currentConfig->encoder = ENCODER_FAAC;
		}
		else if (encoder == "TVQ" || encoder == "tvq")
		{
			String	 bitrate    = "48";
			String	 candidates = "32";

			ScanForParameter("-b", &bitrate);
			ScanForParameter("-c", &candidates);

			currentConfig->tvq_presel_candidates = Math::Max(4, Math::Min(32, candidates.ToInt()));
			currentConfig->tvq_bitrate	     = Math::Max(24, Math::Min(48, bitrate.ToInt()));

			currentConfig->encoder = ENCODER_TVQ;
		}
		else if (encoder == "WAVE" || encoder == "wave")
		{
			currentConfig->encoder = ENCODER_WAVE;
		}
		else
		{
			con->OutputString(String("Encoder ").Append(encoder).Append(" is not supported by BonkEnc!\n\n"));

			broken = true;
		}

		if (!broken)
		{
			currentConfig->enc_outdir = outdir;

			int	 len = currentConfig->enc_outdir.Length() - 1;

			if (currentConfig->enc_outdir[len] != '\\') currentConfig->enc_outdir[++len] = '\\';

			bool	 lferror = false;

			for (int i = 0; i < files.GetNOfEntries(); i++)
			{
				if (i != 0 && !lferror) con->OutputString("done.\n");

				lferror = false;

				InStream	*in = new InStream(STREAM_FILE, files.GetNthEntry(i), IS_READONLY);

				if (in->GetLastError() != IOLIB_ERROR_OK)
				{
					delete in;

					con->OutputString(String("File not found: ").Append(files.GetNthEntry(i)).Append("\n"));

					lferror = true;
					broken = true;

					continue;
				}
				else
				{
					delete in;
				}

				String	 extension;

				extension[0] = (files.GetNthEntry(i))[files.GetNthEntry(i).Length() - 4];
				extension[1] = (files.GetNthEntry(i))[files.GetNthEntry(i).Length() - 3];
				extension[2] = (files.GetNthEntry(i))[files.GetNthEntry(i).Length() - 2];
				extension[3] = (files.GetNthEntry(i))[files.GetNthEntry(i).Length() - 1];

				if ((extension == ".mp3" && !currentConfig->enable_lame) || (extension == ".ogg" && !currentConfig->enable_vorbis))
				{
					con->OutputString(String("Cannot process file: ").Append(files.GetNthEntry(i)).Append("\n"));

					lferror = true;
					broken = true;

					continue;
				}

				con->OutputString(String("Processing file: ").Append(files.GetNthEntry(i)).Append("..."));

				AddFileByName(files.GetNthEntry(i), outfile);

				Encode();

				while (encoding)
				{
					MSG	 msg;
					bool	 result;

					if (Setup::enableUnicode)	result = PeekMessageW(&msg, 0, 0, 0, PM_REMOVE);
					else				result = PeekMessageA(&msg, 0, 0, 0, PM_REMOVE);

					if (result)
					{
						TranslateMessage(&msg);

						if (Setup::enableUnicode)	DispatchMessageW(&msg);
						else				DispatchMessageA(&msg);
					}

					Sleep(10);
				}
			}

			if (!lferror) con->OutputString("done.\n");
		}
	}

	delete con;

	delete joblist;
}

bonkEncCommandline::~bonkEncCommandline()
{
}

Bool bonkEncCommandline::ScanForParameter(String param, String *option)
{
	for (Int i = 0; i < args.GetNOfEntries(); i++)
	{
		if (args.GetNthEntry(i) == param)
		{
			if (option != NULL) *option = args.GetNthEntry(i + 1);

			return True;
		}
	}

	return False;
}

Void bonkEncCommandline::ScanForFiles(Array<String> *files)
{
	String	 param;
	String	 prevParam;

	for (Int i = 0; i < args.GetNOfEntries(); i++)
	{
		prevParam	= param;
		param		= args.GetNthEntry(i);

		if (param[0] != '-' && (prevParam[0] != '-' || prevParam == "-q")) (*files).AddEntry(param);
	}
}
