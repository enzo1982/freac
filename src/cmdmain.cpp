 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#define __THROW_BAD_ALLOC exit(1)
#define MAKEUNICODESTR(x) L##x

#include <smooth/main.h>
#include <cmdmain.h>
#include <console.h>

Int smooth::Main()
{
	bonkEncCommandline	*app = new bonkEncCommandline();

	delete app;

	return 0;
}

bonkEncCommandline::bonkEncCommandline()
{
	currentConfig->enable_console = true;
	currentConfig->appMain = this;

	joblist = new ListBox(Point(0, 0), Size(0, 0));

	bool		 quiet = ScanForParameter("-q", NULL);
	Array<String>	 files;
	String		 encoder = "BONK";
	String		 helpenc = "";
	String		 outdir = ".";
	String		 outfile = "";

	ScanForParameter("-e", &encoder);
	ScanForParameter("-h", &helpenc);
	ScanForParameter("-d", &outdir);
	ScanForParameter("-o", &outfile);

	ScanForFiles(&files);

	bonkEncConsole	*con;

	if (!quiet)	con = new bonkEncConsole(String("BonkEnc ").Append(bonkEnc::version));
	else		con = new bonkEncConsole((char *) NIL);

	con->OutputString(String("BonkEnc Audio Encoder ").Append(bonkEnc::version).Append(" command line interface\nCopyright (C) 2001-2004 Robert Kausch\n\n"));

	if ((files.GetNOfEntries() == 0 && helpenc == "") || !(encoder == "LAME" || encoder == "VORBIS" || encoder == "BONK" || encoder == "BLADE" || encoder == "FAAC" || encoder == "TVQ" || encoder == "WAVE") || (files.GetNOfEntries() > 1 && outfile != ""))
	{
		con->OutputString("Usage:\tBEcmd [options] [file(s)]\n\n");
		con->OutputString("\t-e <encoder>\tSpecify the encoder to use (default is BONK)\n");
		con->OutputString("\t-d <outdir>\tSpecify output directory for encoded files\n");
		con->OutputString("\t-o <outfile>\tSpecify output file name in single file mode\n");
		con->OutputString("\t-h <encoder>\tPrint help for encoder specific options\n");
		con->OutputString("\t-q\t\tDo not print any messages\n\n");
		con->OutputString("<encoder> can be one of LAME, VORBIS, BONK, BLADE, FAAC, TVQ or WAVE.\n\n");
	}
	else if (helpenc != "")
	{
		if (helpenc == "LAME")		con->OutputString("There are no LAME options on the BonkEnc console interface!\n\n");
		else if (helpenc == "VORBIS")	con->OutputString("There are no Ogg Vorbis options on the BonkEnc console interface!\n\n");
		else if (helpenc == "BONK")	con->OutputString("There are no BonkEnc options on the BonkEnc console interface!\n\n");
		else if (helpenc == "BLADE")	con->OutputString("There are no BladeEnc options on the BonkEnc console interface!\n\n");
		else if (helpenc == "FAAC")	con->OutputString("There are no FAAC options on the BonkEnc console interface!\n\n");
		else if (helpenc == "TVQ")	con->OutputString("There are no TwinVQ options on the BonkEnc console interface!\n\n");
		else if (helpenc == "WAVE")	con->OutputString("No options can be configured for the WAVE Out filter!\n\n");
		else				con->OutputString(String("Encoder ").Append(helpenc).Append(" is not supported by BonkEnc!\n\n"));
	}
	else
	{
		bool	 broken = false;

		if ((encoder == "LAME" && !currentConfig->enable_lame) ||
		    (encoder == "VORBIS" && !currentConfig->enable_vorbis) ||
		    (encoder == "BONK" && !currentConfig->enable_bonk) ||
		    (encoder == "BLADE" && !currentConfig->enable_blade) ||
		    (encoder == "FAAC" && !currentConfig->enable_faac) ||
		    (encoder == "TVQ" && !currentConfig->enable_tvq))
		{
			con->OutputString(String("Encoder ").Append(encoder).Append(" is not available!\n\n"));

			broken = true;
		}

		if (encoder == "LAME")		currentConfig->encoder = ENCODER_LAMEENC;
		else if (encoder == "VORBIS")	currentConfig->encoder = ENCODER_VORBISENC;
		else if (encoder == "BONK")	currentConfig->encoder = ENCODER_BONKENC;
		else if (encoder == "BLADE")	currentConfig->encoder = ENCODER_BLADEENC;
		else if (encoder == "FAAC")	currentConfig->encoder = ENCODER_FAAC;
		else if (encoder == "TVQ")	currentConfig->encoder = ENCODER_TVQ;
		else if (encoder == "WAVE")	currentConfig->encoder = ENCODER_WAVE;
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

				InStream	*in = new InStream(STREAM_FILE, files.GetNthEntry(i));

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
	for (int i = 0; i < szCmdLine.Length(); i++)
	{
		bool	 gotit = true;

		for (int j = 0; j < param.Length(); j++)
		{
			if (szCmdLine[i + j] != param[j])
			{
				gotit = false;

				break;
			}
		}

		if (gotit)
		{
			if (option != NULL)
			{
				*option = "";

				int	 sval = 0;

				for (sval = 0; szCmdLine[i + param.Length() + sval] == ' '; sval++);

				for (int j = sval; j > 0; j++)
				{
					if ((i + param.Length() + j) >= szCmdLine.Length()) break;
					if (szCmdLine[i + param.Length() + j] == ' ') break;

					(*option)[j - sval] = szCmdLine[i + param.Length() + j];
				}
			}

			return true;
		}
	}

	return false;
}

Void bonkEncCommandline::ScanForFiles(Array<String> *files)
{
	String	 param;
	String	 prevParam;
	bool	 done = true;
	int	 len = szCmdLine.Length();

	for (int i = 0; i < len; i++)
	{
		if (done)
		{
			prevParam = param;

			param = "";

			done = false;
		}

		if (szCmdLine[i] != ' ')
		{
			for (int j = 0; j < szCmdLine.Length() - i; j++)
			{
				if (szCmdLine[i + j] == ' ' || i + j == len - 1)
				{
					if (szCmdLine[i + j] != ' ') param[j] = szCmdLine[i + j];

					done = true;

					i += j;

					break;
				}

				param[j] = szCmdLine[i + j];
			}

			if (param[0] != '-' && (prevParam[0] != '-' || prevParam == "-q" || prevParam == "--console")) (*files).AddEntry(param);
		}
	}
}
