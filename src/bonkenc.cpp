 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <bonkenc.h>
#include <resources.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <time.h>

#include <dllinterfaces.h>

#include <iolib/drivers/driver_posix.h>
#include <iolib/drivers/driver_unicode.h>

#include <dialogs/genconfig/genconfig.h>
#include <dialogs/bonkconfig.h>
#include <dialogs/bladeconfig.h>
#include <dialogs/lameconfig.h>
#include <dialogs/vorbisconfig.h>
#include <dialogs/faacconfig.h>
#include <dialogs/tvqconfig.h>

#include <cddb.h>
#include <dialogs/cddb/query.h>
#include <dialogs/cddb/submit.h>

#include <dialogs/language.h>

#include <input/filter-in-cdrip.h>
#include <input/filter-in-wave.h>
#include <input/filter-in-voc.h>
#include <input/filter-in-aiff.h>
#include <input/filter-in-au.h>
#include <input/filter-in-lame.h>
#include <input/filter-in-mp4.h>
#include <input/filter-in-vorbis.h>
#include <input/filter-in-bonk.h>
#include <input/filter-in-faad2.h>
#include <input/filter-in-flac.h>
#include <input/filter-in-winamp.h>

Int	 ENCODER_BONKENC	= -1;
Int	 ENCODER_BLADEENC	= -1;
Int	 ENCODER_LAMEENC	= -1;
Int	 ENCODER_VORBISENC	= -1;
Int	 ENCODER_FAAC		= -1;
Int	 ENCODER_TVQ		= -1;
Int	 ENCODER_WAVE		= -1;

bonkEncConfig		*bonkEnc::currentConfig	= NIL;
I18n::Translator	*bonkEnc::i18n		= NIL;

bonkEncDebug	*debug_out;

String	 bonkEnc::version = "v1.0 Release Candidate 1";
String	 bonkEnc::cddbVersion = "v1.0rc1";
String	 bonkEnc::shortVersion = "v1.0";

bonkEnc::bonkEnc()
{
	CoInitialize(NIL);

	encoding = False;
	encoder_thread = NIL;

	playing = False;
	play_thread = NIL;

	currentConfig = new bonkEncConfig;

	currentConfig->LoadSettings();

	i18n = new I18n::Translator();
	i18n->SetInternalLanguageInfo("English", "Robert Kausch <robert.kausch@bonkenc.org>", "http://www.bonkenc.org/", False);

	if (currentConfig->language == "" && i18n->GetNOfLanguages() > 1)
	{
		languageDlg	*dlg = new languageDlg();

		dlg->ShowDialog();

		DeleteObject(dlg);
	}

	if (currentConfig->language == "") currentConfig->language = "internal";

	i18n->ActivateLanguage(currentConfig->language);

	if (bonkEncDLLInterfaces::LoadBonkDLL() == False)	currentConfig->enable_bonk = False;
	else							currentConfig->enable_bonk = True;

	if (bonkEncDLLInterfaces::LoadBladeDLL() == False)	currentConfig->enable_blade = False;
	else							currentConfig->enable_blade = True;

	if (bonkEncDLLInterfaces::LoadLAMEDLL() == False)	currentConfig->enable_lame = False;
	else							currentConfig->enable_lame = True;

	if (bonkEncDLLInterfaces::LoadVorbisDLL() == False)	currentConfig->enable_vorbis = False;
	else							currentConfig->enable_vorbis = True;

	if (bonkEncDLLInterfaces::LoadFAACDLL() == False)	currentConfig->enable_faac = False;
	else							currentConfig->enable_faac = True;

	if (bonkEncDLLInterfaces::LoadFAAD2DLL() == False)	currentConfig->enable_faad2 = False;
	else							currentConfig->enable_faad2 = True;

	if (bonkEncDLLInterfaces::LoadTVQDLL() == False)	currentConfig->enable_tvq = False;
	else							currentConfig->enable_tvq = True;

	if (bonkEncDLLInterfaces::LoadCDRipDLL() == False)	currentConfig->enable_cdrip = False;
	else							currentConfig->enable_cdrip = True;

	if (bonkEncDLLInterfaces::LoadID3DLL() == False)	currentConfig->enable_id3 = False;
	else							currentConfig->enable_id3 = True;

	if (bonkEncDLLInterfaces::LoadEUpdateDLL() == False)	currentConfig->enable_eUpdate = False;
	else							currentConfig->enable_eUpdate = True;

	if (bonkEncDLLInterfaces::LoadFLACDLL() == False)	currentConfig->enable_flac = False;
	else							currentConfig->enable_flac = True;

	if (currentConfig->enable_faac || currentConfig->enable_faad2)
	{
		if (bonkEncDLLInterfaces::LoadMP4V2DLL() == False)	currentConfig->enable_mp4 = False;
		else							currentConfig->enable_mp4 = True;
	}
	else
	{
		currentConfig->enable_mp4 = False;
	}

	bonkEncDLLInterfaces::LoadWinampDLLs();

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
		Long		 error = CDEX_OK;
		OSVERSIONINFOA	 vInfo;

		vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

		GetVersionExA(&vInfo);

		if (vInfo.dwPlatformId != VER_PLATFORM_WIN32_NT) currentConfig->cdrip_ntscsi = False;

		error = ex_CR_Init(currentConfig->cdrip_ntscsi);

		if (error != CDEX_OK && vInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			currentConfig->cdrip_ntscsi = !currentConfig->cdrip_ntscsi;

			error = ex_CR_Init(currentConfig->cdrip_ntscsi);
		}

		if (error != CDEX_OK)
		{
			QuickMessage(i18n->TranslateString("Unable to load ASPI drivers!").Append(" ").Append(i18n->TranslateString("CD ripping disabled!")), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

			currentConfig->enable_cdrip = false;
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

	if (currentConfig->enc_outdir[currentConfig->enc_outdir.Length() - 1] != '\\') currentConfig->enc_outdir.Append("\\");
}

bonkEnc::~bonkEnc()
{
	if (currentConfig->enable_cdrip) ex_CR_DeInit();

	for (Int i = 0; i < bonkEncCDDB::infoCache.GetNOfEntries(); i++)
	{
		for (Int j = 0; j < bonkEncCDDB::infoCache.GetNthEntry(i)->GetNOfEntries(); j++)
		{
			delete bonkEncCDDB::infoCache.GetNthEntry(i)->GetNthEntry(j);
		}

		delete bonkEncCDDB::infoCache.GetNthEntry(i);
	}

	if (currentConfig->enable_bonk)		bonkEncDLLInterfaces::FreeBonkDLL();
	if (currentConfig->enable_blade)	bonkEncDLLInterfaces::FreeBladeDLL();
	if (currentConfig->enable_faac)		bonkEncDLLInterfaces::FreeFAACDLL();
	if (currentConfig->enable_faad2)	bonkEncDLLInterfaces::FreeFAAD2DLL();
	if (currentConfig->enable_lame)		bonkEncDLLInterfaces::FreeLAMEDLL();
	if (currentConfig->enable_tvq)		bonkEncDLLInterfaces::FreeTVQDLL();
	if (currentConfig->enable_vorbis)	bonkEncDLLInterfaces::FreeVorbisDLL();
	if (currentConfig->enable_cdrip)	bonkEncDLLInterfaces::FreeCDRipDLL();
	if (currentConfig->enable_id3)		bonkEncDLLInterfaces::FreeID3DLL();
	if (currentConfig->enable_eUpdate)	bonkEncDLLInterfaces::FreeEUpdateDLL();
	if (currentConfig->enable_mp4)		bonkEncDLLInterfaces::FreeMP4V2DLL();
	if (currentConfig->enable_flac)		bonkEncDLLInterfaces::FreeFLACDLL();

	bonkEncDLLInterfaces::FreeWinampDLLs();

	delete i18n;
	delete currentConfig;

	CoUninitialize();
}

InputFilter *bonkEnc::CreateInputFilter(String file, bonkEncTrack *trackInfo)
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

	for (Int i = 0; i < bonkEncDLLInterfaces::winamp_in_plugins.GetNOfEntries(); i++)
	{
		Int	 n = 1;
		Int	 k = 0;
		String	 extension;

		for (Int j = 0; true; j++)
		{
			if (!(n & 1))
			{
				if (bonkEncDLLInterfaces::winamp_in_modules.GetNthEntry(i)->FileExtensions[j] == 0) n++;
			}
			else
			{
				extension[k++] = bonkEncDLLInterfaces::winamp_in_modules.GetNthEntry(i)->FileExtensions[j];

				if (bonkEncDLLInterfaces::winamp_in_modules.GetNthEntry(i)->FileExtensions[j] == 0)
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

			if (bonkEncDLLInterfaces::winamp_in_modules.GetNthEntry(i)->FileExtensions[j] == 0 && bonkEncDLLInterfaces::winamp_in_modules.GetNthEntry(i)->FileExtensions[j + 1] == 0) break;
		}
	}

	InputFilter	*filter_in = NIL;

	if (extension3 == "cda" && currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		filter_in = new FilterInCDRip(currentConfig, trackInfo);
	}
	else if (extension3 == "mp3" && currentConfig->enable_lame)
	{
		filter_in = new FilterInLAME(currentConfig, trackInfo);
	}
	else if ((extension3 == "mp4" || extension3 == "m4a" || extension3 == "m4b") && currentConfig->enable_mp4 && currentConfig->enable_faad2)
	{
		filter_in = new FilterInMP4(currentConfig, trackInfo);
	}
	else if (extension3 == "ogg" && currentConfig->enable_vorbis)
	{
		filter_in = new FilterInVORBIS(currentConfig, trackInfo);
	}
	else if (extension3 == "aac" && currentConfig->enable_faad2)
	{
		filter_in = new FilterInFAAD2(currentConfig, trackInfo);
	}
	else if (extension4 == "bonk" && currentConfig->enable_bonk)
	{
		filter_in = new FilterInBONK(currentConfig, trackInfo);
	}
	else if (extension4 == "flac" && currentConfig->enable_flac)
	{
		filter_in = new FilterInFLAC(currentConfig, trackInfo);
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
			IOLibDriver	*driver_in = NIL;

			if (Setup::enableUnicode)	driver_in = new IOLibDriverUnicode(file, IS_READONLY);
			else				driver_in = new IOLibDriverPOSIX(file, IS_READONLY);

			InStream	*f_in = new InStream(STREAM_DRIVER, driver_in);
			Int		 magic = f_in->InputNumber(4);

			delete f_in;
			delete driver_in;

			switch (magic)
			{
				case 1297239878:
					filter_in = new FilterInAIFF(currentConfig, trackInfo);
					break;
				case 1684960046:
					filter_in = new FilterInAU(currentConfig, trackInfo);
					break;
				case 1634038339:
					filter_in = new FilterInVOC(currentConfig, trackInfo);
					break;
				case 1179011410:
					filter_in = new FilterInWAVE(currentConfig, trackInfo);
					break;
			}
		}
		else
		{
			filter_in = new FilterInWinamp(currentConfig, trackInfo, bonkEncDLLInterfaces::winamp_in_modules.GetNthEntry(indexes.GetNthEntry(found)));
		}
	}

	return filter_in;
}

Void bonkEnc::ReadCD()
{
	if (encoding)
	{
		QuickMessage(i18n->TranslateString("Cannot modify the joblist while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

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

Array<bonkEncTrack *> *bonkEnc::GetCDDBData()
{
	cddbQueryDlg		*dlg	= new cddbQueryDlg();
	Array<bonkEncTrack *>	*array	= dlg->QueryCDDB();

	DeleteObject(dlg);

	return array;
}

Void bonkEnc::SelectDir()
{
	DirSelection	*dialog = new DirSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->SetCaption(String("\n").Append(bonkEnc::i18n->TranslateString("Select the folder in which the encoded files will be placed:")));
	dialog->SetDirName(currentConfig->enc_outdir);

	if (dialog->ShowDialog() == Success)
	{
		edb_outdir->SetText(dialog->GetDirName());
		currentConfig->enc_outdir = dialog->GetDirName();
	}

	DeleteObject(dialog);
}

String bonkEnc::LocalizeNumber(Int number)
{
	String	 nString = String::FromInt(number);
	String	 retVal;
	String	 separator;

	char	*buffer_a = new char [256];
	wchar_t	*buffer_w = new wchar_t [256];

	if (Setup::enableUnicode)	GetLocaleInfoW(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, buffer_w, 256);
	else				GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, buffer_a, 256);

	if (Setup::enableUnicode)	separator = buffer_w;
	else				separator = buffer_a;

	delete [] buffer_a;
	delete [] buffer_w;

	for (Int i = 0; i < nString.Length(); i++)
	{
		if ((nString.Length() - i) % 3 == 0 && i > 0) retVal.Append(separator);

		retVal[retVal.Length()] = nString[i];
	}

	return retVal;
}
