 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#define __THROW_BAD_ALLOC exit(1)
#define MAKEUNICODESTR(x) L##x

#include <bonkenc.h>
#include <resources.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <time.h>

#include <dllinterfaces.h>

#include <genconfig.h>
#include <bonkconfig.h>
#include <bladeconfig.h>
#include <lameconfig.h>
#include <vorbisconfig.h>
#include <faacconfig.h>
#include <tvqconfig.h>

#include <cddb.h>
#include <cddb_multimatch.h>
#include <cddb_submit.h>

#include <language.h>

#include <input/filter-in-cdrip.h>
#include <input/filter-in-wave.h>
#include <input/filter-in-voc.h>
#include <input/filter-in-aiff.h>
#include <input/filter-in-au.h>
#include <input/filter-in-lame.h>
#include <input/filter-in-vorbis.h>
#include <input/filter-in-bonk.h>
#include <input/filter-in-winamp.h>

Int	 ENCODER_BONKENC	= -1;
Int	 ENCODER_BLADEENC	= -1;
Int	 ENCODER_LAMEENC	= -1;
Int	 ENCODER_VORBISENC	= -1;
Int	 ENCODER_FAAC		= -1;
Int	 ENCODER_TVQ		= -1;
Int	 ENCODER_WAVE		= -1;

bonkEncConfig	*bonkEnc::currentConfig	= NIL;
bonkTranslator	*bonkEnc::i18n		= NIL;

String	 bonkEnc::version = "CVS-20030608";
String	 bonkEnc::cddbVersion = "v0.9";
String	 bonkEnc::shortVersion = "v0.9";

bonkEnc::bonkEnc()
{
	encoding = false;
	encoder_thread = NIL;

	currentConfig = new bonkEncConfig;

	currentConfig->SetIniFile(GetApplicationDirectory().Append("bonkenc.ini"));
	currentConfig->LoadSettings();

	i18n = new bonkTranslator();

	if (currentConfig->language == "" && i18n->GetNOfLanguages() > 1)
	{
		languageDlg	*dlg = new languageDlg();

		dlg->ShowDialog();

		delete dlg;
	}

	i18n->ActivateLanguage(currentConfig->language);

	if (LoadBonkDLL() == false)	currentConfig->enable_bonk = false;
	else				currentConfig->enable_bonk = true;

	if (LoadBladeDLL() == false)	currentConfig->enable_blade = false;
	else				currentConfig->enable_blade = true;

	if (LoadLAMEDLL() == false)	currentConfig->enable_lame = false;
	else				currentConfig->enable_lame = true;

	if (LoadVorbisDLL() == false)	currentConfig->enable_vorbis = false;
	else				currentConfig->enable_vorbis = true;

	if (LoadFAACDLL() == false)	currentConfig->enable_faac = false;
	else				currentConfig->enable_faac = true;

	if (LoadTVQDLL() == false)	currentConfig->enable_tvq = false;
	else				currentConfig->enable_tvq = true;

	if (LoadCDRipDLL() == false)	currentConfig->enable_cdrip = false;
	else				currentConfig->enable_cdrip = true;

	if (LoadID3DLL() == false)	currentConfig->enable_id3 = false;
	else				currentConfig->enable_id3 = true;

	LoadWinampDLLs();

	int	 nextEC = 0;

	if (currentConfig->enable_blade)	ENCODER_BLADEENC = nextEC++;
	if (currentConfig->enable_bonk)		ENCODER_BONKENC = nextEC++;
	if (currentConfig->enable_faac)		ENCODER_FAAC = nextEC++;
	if (currentConfig->enable_lame)		ENCODER_LAMEENC = nextEC++;
	if (currentConfig->enable_vorbis)	ENCODER_VORBISENC = nextEC++;
	if (currentConfig->enable_tvq)		ENCODER_TVQ = nextEC++;

	ENCODER_WAVE = nextEC++;

	if (currentConfig->encoder >= nextEC) currentConfig->encoder = ENCODER_WAVE;


	if (currentConfig->enable_cdrip)
	{
		Long		 error = ex_CR_Init(currentConfig->GetIniFile());
		Int		 choice = IDYES;
		OSVERSIONINFOA	 vInfo;

		vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

		GetVersionExA(&vInfo);

		if (currentConfig->cdrip_ntscsi && vInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			ex_CR_SetTransportLayer(currentConfig->cdrip_ntscsi);

			ex_CR_SaveSettings();

			error = ex_CR_Init(currentConfig->GetIniFile());
		}

		if (error != CDEX_OK)
		{
			switch (error)
			{
				case CDEX_NATIVEEASPINOTSUPPORTED:
				case CDEX_FAILEDTOLOADASPIDRIVERS:
				case CDEX_FAILEDTOGETASPISTATUS:
					SMOOTH::MessageBox(i18n->TranslateString("Unable to load ASPI drivers!").Append(" ").Append(i18n->TranslateString("CD ripping disabled!")), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

					currentConfig->enable_cdrip = false;

					break;
				case CDEX_NOCDROMDEVICES:
					if (vInfo.dwPlatformId != VER_PLATFORM_WIN32_NT)
					{
						SMOOTH::MessageBox(i18n->TranslateString("Unable to load ASPI drivers!").Append(" ").Append(i18n->TranslateString("CD ripping disabled!")), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

						currentConfig->enable_cdrip = false;

						break;
					}
				case CDEX_NATIVEEASPISUPPORTEDNOTSELECTED:
					if (error == CDEX_NATIVEEASPISUPPORTEDNOTSELECTED) choice = SMOOTH::MessageBox(i18n->TranslateString("Unable to load ASPI drivers!").Append(" ").Append(i18n->TranslateString("Do you want to use native NT SCSI instead?")), i18n->TranslateString("Error"), MB_YESNO, IDI_QUESTION);

					if (choice == IDYES)
					{
						currentConfig->cdrip_ntscsi = True;
	
						ex_CR_SetTransportLayer(currentConfig->cdrip_ntscsi);

						ex_CR_SaveSettings();

						error = ex_CR_Init(currentConfig->GetIniFile());

						if (error != CDEX_OK)
						{
							SMOOTH::MessageBox(i18n->TranslateString("Unable to load ASPI drivers!").Append(" ").Append(i18n->TranslateString("CD ripping disabled!")), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

							currentConfig->enable_cdrip = False;
							currentConfig->cdrip_ntscsi = False;
						}
					}

					break;
			}
		}

		if (error == CDEX_OK)
		{
			currentConfig->cdrip_numdrives = ex_CR_GetNumCDROM();

			if (currentConfig->cdrip_numdrives >= 1)
			{
				for (int i = 0; i < currentConfig->cdrip_numdrives; i++)
				{
					ex_CR_SetActiveCDROM(i);

					CDROMPARAMS	 params;

					ex_CR_GetCDROMParameters(&params);

					currentConfig->cdrip_drives.AddEntry(params.lpszCDROMID);
				}
			}

			if (currentConfig->cdrip_numdrives <= currentConfig->cdrip_activedrive) currentConfig->cdrip_activedrive = 0;
		}
	}

	int	 len = currentConfig->enc_outdir.Length() - 1;

	if (currentConfig->enc_outdir[len] != '\\') currentConfig->enc_outdir[++len] = '\\';
}

bonkEnc::~bonkEnc()
{
	if (currentConfig->enable_cdrip) ex_CR_DeInit();

	for (Int i = 0; i < bonkEncCDDB::titleCache.GetNOfEntries(); i++)
	{
		delete bonkEncCDDB::titleCache.GetNthEntry(i);
		delete bonkEncCDDB::infoCache.GetNthEntry(i);
	}

	if (currentConfig->enable_bonk)		FreeBonkDLL();
	if (currentConfig->enable_blade)	FreeBladeDLL();
	if (currentConfig->enable_faac)		FreeFAACDLL();
	if (currentConfig->enable_lame)		FreeLAMEDLL();
	if (currentConfig->enable_tvq)		FreeTVQDLL();
	if (currentConfig->enable_vorbis)	FreeVorbisDLL();
	if (currentConfig->enable_cdrip)	FreeCDRipDLL();

	FreeWinampDLLs();

	delete currentConfig;
}

InputFilter *bonkEnc::CreateInputFilter(String file)
{
	String	 extension2;
	String	 extension3;
	String	 extension4;

	extension2[0] = tolower(file[file.Length() - 2]);
	extension2[1] = tolower(file[file.Length() - 1]);

	extension3[0] = tolower(file[file.Length() - 3]);
	extension3[1] = tolower(file[file.Length() - 2]);
	extension3[2] = tolower(file[file.Length() - 1]);

	extension4[0] = tolower(file[file.Length() - 4]);
	extension4[1] = tolower(file[file.Length() - 3]);
	extension4[2] = tolower(file[file.Length() - 2]);
	extension4[3] = tolower(file[file.Length() - 1]);

	Array<String>	 extensions;
	Array<Int>	 indexes;

	for (Int i = 0; i < winamp_in_plugins.GetNOfEntries(); i++)
	{
		Int	 n = 1;
		Int	 k = 0;
		String	 extension;

		for (Int j = 0; true; j++)
		{
			if (!(n & 1))
			{
				if (winamp_in_modules.GetNthEntry(i)->FileExtensions[j] == 0) n++;
			}
			else
			{
				extension[k++] = winamp_in_modules.GetNthEntry(i)->FileExtensions[j];

				if (winamp_in_modules.GetNthEntry(i)->FileExtensions[j] == 0)
				{
					String	 extension2 = extension;
					Int	 o = 0;		

					for (Int m = 0; m <= extension2.Length(); m++)
					{
						if (extension2[m] == ';' || extension2[m] == 0)
						{
							extension[m - o] = 0;

							extensions.AddEntry(extension);
							indexes.AddEntry(i);

							o = m + 1;
						}
						else
						{
							extension[m - o] = tolower(extension2[m]);
						}
					}

					k = 0;
					n++;
					extension = "";
				}
			}

			if (winamp_in_modules.GetNthEntry(i)->FileExtensions[j] == 0 && winamp_in_modules.GetNthEntry(i)->FileExtensions[j + 1] == 0) break;
		}
	}

	InputFilter	*filter_in = NIL;

	if (extension3 == "cda" && currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		filter_in = new FilterInCDRip(currentConfig);
	}
	else if (extension3 == "mp3" && currentConfig->enable_lame)
	{
		filter_in = new FilterInLAME(currentConfig);
	}
	else if (extension3 == "ogg" && currentConfig->enable_vorbis)
	{
		filter_in = new FilterInVORBIS(currentConfig);
	}
	else if (extension4 == "bonk" && currentConfig->enable_bonk)
	{
		filter_in = new FilterInBONK(currentConfig);
	}
	else
	{
		Int	 found = -1;

		for (Int i = 0; i < extensions.GetNOfEntries(); i++)
		{
			switch (extensions.GetNthEntry(i).Length())
			{
				case 2:
					if (extension2 == extensions.GetNthEntry(i)) found = i;
					break;
				case 3:
					if (extension3 == extensions.GetNthEntry(i)) found = i;
					break;
				case 4:
					if (extension4 == extensions.GetNthEntry(i)) found = i;
					break;
			}

			if (found >= 0) break;
		}

		if (found == -1)
		{
			InStream	*f_in = new InStream(STREAM_FILE, file);
			Int		 magic = f_in->InputNumber(4);

			delete f_in;

			switch (magic)
			{
				case 1297239878:
					filter_in = new FilterInAIFF(currentConfig);
					break;
				case 1684960046:
					filter_in = new FilterInAU(currentConfig);
					break;
				case 1634038339:
					filter_in = new FilterInVOC(currentConfig);
					break;
				case 1179011410:
					filter_in = new FilterInWAVE(currentConfig);
					break;
			}
		}
		else
		{
			filter_in = new FilterInWinamp(currentConfig, winamp_in_modules.GetNthEntry(indexes.GetNthEntry(found)));
		}
	}

	return filter_in;
}

Void bonkEnc::ReadCD()
{
	if (encoding)
	{
		SMOOTH::MessageBox(i18n->TranslateString("Cannot modify the joblist while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	ex_CR_SetActiveCDROM(currentConfig->cdrip_activedrive);

	ex_CR_ReadToc();

	Int	 numTocEntries = ex_CR_GetNumTocEntries();

	for (Int i = 0; i < numTocEntries; i++)
	{
		TOCENTRY entry = ex_CR_GetTocEntry(i);

		if (!(entry.btFlag & CDROMDATAFLAG) && entry.btTrackNumber == i + 1) AddFileByName(String("/cda").Append(String::FromInt(i + 1)));

		cddbRetry = False;
	}

	cddbRetry = True;
}

Array<bonkFormatInfo::bonkTrackInfo *> *bonkEnc::GetCDDBData()
{
	bonkEncCDDB	 cddb(currentConfig);
	String		 result;
	String		 read = NIL;
	Bool		 fuzzy = False;

	cddb.SetActiveDrive(currentConfig->cdrip_activedrive);

	String		 discid = cddb.GetDiscIDString();

	if (discid == "ffffffff" || discid == "00000000") return NIL; // no disc in drive or read error

	if (bonkEncCDDB::requestedDiscs.GetEntry(cddb.ComputeDiscID()) == True && !cddbRetry)
	{
		cddbInfo = bonkEncCDDB::infoCache.GetEntry(cddb.ComputeDiscID());

		return bonkEncCDDB::titleCache.GetEntry(cddb.ComputeDiscID());
	}

	bonkEncCDDB::requestedDiscs.AddEntry(True, cddb.ComputeDiscID());

	mainWnd_statusbar->SetText(i18n->TranslateString("Connecting to freedb server at").Append(" ").Append(currentConfig->freedb_server).Append("..."));

	if (currentConfig->freedb_mode == FREEDB_MODE_CDDBP) cddb.ConnectToServer();

	mainWnd_statusbar->SetText(i18n->TranslateString("Requesting CD information").Append("..."));

	result = cddb.Query(discid);

	if (result == "none")
	{
		SMOOTH::MessageBox(i18n->TranslateString("No freedb entry for this disk."), i18n->TranslateString("Info"), MB_OK, IDI_INFORMATION);
	}
	else if (result == "multiple" || result == "fuzzy")
	{
		cddbMultiMatchDlg	*dlg = new cddbMultiMatchDlg(false);

		for (int i = 0; i < cddb.GetNOfMatches(); i++)
		{
			dlg->AddEntry(cddb.GetNthCategory(i), cddb.GetNthTitle(i));
		}

		if (result == "fuzzy")
		{
			dlg->AddEntry(i18n->TranslateString("none"), "");

			fuzzy = True;
		}

		Int index = dlg->ShowDialog();

		if (index < cddb.GetNOfMatches() && index >= 0)
		{
			read = String(cddb.GetNthCategory(index)).Append(" ").Append(cddb.GetNthID(index));
		}

		delete dlg;
	}
	else if (result == "error")
	{
		SMOOTH::MessageBox(i18n->TranslateString("Some error occurred trying to connect to the freedb server."), i18n->TranslateString("Error"), MB_OK, IDI_HAND);
	}
	else
	{
		read = result;
	}

	Array<bonkFormatInfo::bonkTrackInfo *>	*array = NIL;

	if (read != NIL)
	{
		cddbInfo = new CDDBInfo();

		String	 result = cddb.Read(read);
		String	 cLine;

		array = new Array<bonkFormatInfo::bonkTrackInfo *>;

		cddbInfo->discid = cddb.GetDiscIDString();
		cddbInfo->category = cddb.GetCategory();

		if (fuzzy) cddbInfo->revision = -1;

		for (Int j = 0; j < result.Length();)
		{
			for (Int i = 0; i >= 0; i++, j++)
			{
				if (result[j] == '\n' || result[j] == 0)	{ cLine[i] = 0; j++; break; }
				else						cLine[i] = result[j];
			}

			if (cLine.CompareN("DTITLE", 6) == 0)
			{
				bonkFormatInfo::bonkTrackInfo	*info = new bonkFormatInfo::bonkTrackInfo;
				Int				 k;

				for (k = 7; k >= 0; k++)
				{
					if (cLine[k] == ' ' && cLine[k + 1] == '/' && cLine[k + 2] == ' ')	break;
					else									info->artist[k - 7] = cLine[k];
				}

				for (Int l = k + 3; l < cLine.Length(); l++) info->album[l - k - 3] = cLine[l];

				info->track = -1;

				cddbInfo->artist = info->artist;
				cddbInfo->album = info->album;

				array->AddEntry(info, 0);
			}
			else if (cLine.CompareN("DGENRE", 6) == 0)
			{
				bonkFormatInfo::bonkTrackInfo	*info = array->GetEntry(0);

				for (Int l = 7; l < cLine.Length(); l++) info->genre[l - 7] = cLine[l];

				cddbInfo->genre = info->genre;
			}
			else if (cLine.CompareN("DYEAR", 5) == 0)
			{
				bonkFormatInfo::bonkTrackInfo	*info = array->GetEntry(0);
				String				 year;

				for (Int l = 6; l < cLine.Length(); l++) year[l - 6] = cLine[l];

				info->year = year.ToInt();

				cddbInfo->year = year;
			}
			else if (cLine.CompareN("TTITLE", 6) == 0)
			{
				bonkFormatInfo::bonkTrackInfo	*info = new bonkFormatInfo::bonkTrackInfo;
				String				 track;
				Int				 k;

				for (k = 6; k >= 0; k++)
				{
					if (cLine[k] == '=')	break;
					else			track[k - 6] = cLine[k];
				}

				for (Int l = k + 1; l < cLine.Length(); l++) info->title[l - k - 1] = cLine[l];

				info->track = track.ToInt() + 1;

				cddbInfo->titles.AddEntry(info->title);

				array->AddEntry(info, info->track);
			}
			else if (cLine.CompareN("EXTD", 4) == 0)
			{
				bonkFormatInfo::bonkTrackInfo	*info = array->GetEntry(0);

				for (Int k = 5; k < cLine.Length(); k++) info->comment[k - 5] = cLine[k];

				cddbInfo->comment = info->comment;
			}
			else if (cLine.CompareN("EXTT", 4) == 0)
			{
				String	 track;
				Int	 k;

				for (k = 4; k >= 0; k++)
				{
					if (cLine[k] == '=')	break;
					else			track[k - 4] = cLine[k];
				}

				bonkFormatInfo::bonkTrackInfo	*info = array->GetEntry(track.ToInt() + 1);

				if (info != NIL)
				{
					for (Int l = k + 1; l < cLine.Length(); l++) info->comment[l - k - 1] = cLine[l];
				}

				cddbInfo->comments.AddEntry(info->comment);
			}
			else if (cLine.CompareN("PLAYORDER", 9) == 0)
			{
				bonkFormatInfo::bonkTrackInfo	*info = array->GetEntry(0);

				for (Int k = 10; k < cLine.Length(); k++) info->playorder[k - 10] = cLine[k];

				cddbInfo->playorder = info->playorder;
			}
			else if (cLine.CompareN("# Revision: ", 12) == 0 && !fuzzy)
			{
				String	 revision;

				for (Int l = 12; l < cLine.Length(); l++) revision[l - 12] = cLine[l];

				cddbInfo->revision = revision.ToInt();
			}
			else if (cLine.CompareN("# Track frame offsets:", 22) == 0)
			{
				do
				{
					for (Int m = 0; m >= 0; m++, j++)
					{
						if (result[j] == '\n' || result[j] == 0)	{ cLine[m] = 0; j++; break; }
						else						cLine[m] = result[j];
					}

					if (cLine[0] == '#' && cLine.Length() <= 2) break;

					Int	 firstDigit = 0;
					String	 offset;

					for (Int n = 2; n < cLine.Length(); n++)
					{
						if (cLine[n] != ' ' && cLine[n] != '\t')
						{
							firstDigit = n;

							break;
						}
					}

					for (Int l = firstDigit; l < cLine.Length(); l++) offset[l - firstDigit] = cLine[l];

					cddbInfo->offsets.AddEntry(offset.ToInt());
				}
				while (True);
			}
			else if (cLine.CompareN("# Disc length: ", 15) == 0)
			{
				String	 disclength;

				for (Int l = 15; l < cLine.Length(); l++) disclength[l - 15] = cLine[l];

				cddbInfo->disclength = disclength.ToInt();
			}
			else if (cLine.CompareN("210 ", 4) == 0)
			{
				String	 category;

				for (Int l = 4; l < cLine.Length(); l++)
				{
					if (cLine[l] == ' ') break;

					category[l - 4] = cLine[l];
				}

				cddbInfo->category = category;
			}
		}
	}

	if (currentConfig->freedb_mode == FREEDB_MODE_CDDBP) cddb.CloseConnection();

	if (!currentConfig->enable_console) mainWnd_statusbar->SetText(String("BonkEnc ").Append(bonkEnc::version).Append(" - Copyright (C) 2001-2003 Robert Kausch"));

	return array;
}
