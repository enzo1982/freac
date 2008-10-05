 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/config_cdrip.h>

BonkEnc::ConfigureCDRip::ConfigureCDRip()
{
	Point		 pos;
	Size		 size;

	BoCA::Config	*config = BoCA::Config::Get();

	cdparanoia	= config->cdrip_paranoia;
	jitter		= config->cdrip_jitter;
	swapchannels	= config->cdrip_swapchannels;
	locktray	= config->cdrip_locktray;
	ntscsi		= config->cdrip_ntscsi;
	autoRead	= config->cdrip_autoRead;
	autoRip		= config->cdrip_autoRip;
	autoEject	= config->cdrip_autoEject;
	readCDText	= config->cdrip_read_cdtext;
	readCDPlayerIni	= config->cdrip_read_cdplayerini;

	pos.x	= 7;
	pos.y	= 11;
	size.cx	= 344;
	size.cy	= 43;

	group_drive	= new GroupBox(BonkEnc::i18n->TranslateString("Active CD-ROM drive"), pos, size);

	pos.x	= 17;
	pos.y	= 23;
	size.cx	= 324;
	size.cy	= 0;

	combo_drive	= new ComboBox(pos, size);

	for (Int j = 0; j < config->cdrip_numdrives; j++)
	{
		combo_drive->AddEntry(config->cdrip_drives.GetNth(j));
	}

	combo_drive->SelectNthEntry(config->cdrip_activedrive);

	pos.x	= 7;
	pos.y	= 66;
	size.cx	= 344;
	size.cy	= 39;

	group_cdinfo	= new GroupBox(BonkEnc::i18n->TranslateString("CD information"), pos, size);

	pos.x	+= 10;
	pos.y	+= 11;
	size.cx	= 157;
	size.cy	= 0;

	check_readCDText	= new CheckBox(BonkEnc::i18n->TranslateString("Read CD Text"), pos, size, &readCDText);

	pos.x += 166;

	check_readCDPlayerIni	= new CheckBox(BonkEnc::i18n->TranslateString("Read cdplayer.ini"), pos, size, &readCDPlayerIni);

	pos.x	= 7;
	pos.y	= 117;
	size.cx	= 344;
	size.cy	= 68;

	group_ripping	= new GroupBox(BonkEnc::i18n->TranslateString("Ripper settings"), pos, size);

	pos.x	+= 10;
	pos.y	+= 14;
	size.cx	= 157;
	size.cy	= 0;

	check_paranoia	= new CheckBox(BonkEnc::i18n->TranslateString("Activate cdparanoia mode:"), pos, size, &cdparanoia);
	check_paranoia->onAction.Connect(&ConfigureCDRip::SetParanoia, this);

	pos.x	+= 166;
	pos.y	-= 1;
	size.cx	= 158;
	size.cy	= 0;

	combo_paranoia_mode= new ComboBox(pos, size);
	combo_paranoia_mode->AddEntry(BonkEnc::i18n->TranslateString("Overlap only"));
	combo_paranoia_mode->AddEntry(BonkEnc::i18n->TranslateString("No verify"));
	combo_paranoia_mode->AddEntry(BonkEnc::i18n->TranslateString("No scratch repair"));
	combo_paranoia_mode->AddEntry(BonkEnc::i18n->TranslateString("Full cdparanoia mode"));
	combo_paranoia_mode->SelectNthEntry(config->cdrip_paranoia_mode);

	if (!cdparanoia) combo_paranoia_mode->Deactivate();

	pos.x	-= 166;
	pos.y	+= 27;
	size.cx	= 157;
	size.cy	= 0;

	check_jitter	= new CheckBox(BonkEnc::i18n->TranslateString("Activate jitter correction"), pos, size, &jitter);

	pos.x += 166;

	check_swapchannels	= new CheckBox(BonkEnc::i18n->TranslateString("Swap left/right channel"), pos, size, &swapchannels);

	pos.x	= 359;
	pos.y	= 11;
	size.cx	= 178;
	size.cy	= 94;

	group_automatization	= new GroupBox(BonkEnc::i18n->TranslateString("Automatization"), pos, size);

	pos.x	+= 10;
	pos.y	+= 14;
	size.cx	= 157;
	size.cy	= 0;

	check_autoRead	= new CheckBox(BonkEnc::i18n->TranslateString("Read CD contents on insert"), pos, size, &autoRead);
	check_autoRead->onAction.Connect(&ConfigureCDRip::ToggleAutoRead, this);
	pos.y += 26;

	check_autoRip	= new CheckBox(BonkEnc::i18n->TranslateString("Start ripping automatically"), pos, size, &autoRip);

	pos.y += 26;

	check_autoEject	= new CheckBox(BonkEnc::i18n->TranslateString("Eject disk after ripping"), pos, size, &autoEject);

	pos.x	= 359;
	pos.y	= 117;
	size.cx	= 178;
	size.cy	= 68;

	group_cdoptions	= new GroupBox(BonkEnc::i18n->TranslateString("CD options"), pos, size);

	pos.x	+= 10;
	pos.y	+= 14;
	size.cx	= 157;
	size.cy	= 0;

	check_locktray	= new CheckBox(BonkEnc::i18n->TranslateString("Lock CD tray while ripping"), pos, size, &locktray);

	pos.y += 26;

	check_ntscsi	= new CheckBox(BonkEnc::i18n->TranslateString("Use native NT SCSI library"), pos, size, &ntscsi);

	OSVERSIONINFOA	 vInfo;

	vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

	GetVersionExA(&vInfo);

	if (vInfo.dwPlatformId != VER_PLATFORM_WIN32_NT) check_ntscsi->Deactivate();

	ToggleAutoRead();

	Add(group_drive);
	Add(combo_drive);
	Add(group_ripping);
	Add(check_paranoia);
	Add(combo_paranoia_mode);
	Add(check_jitter);
	Add(check_swapchannels);
	Add(group_automatization);
	Add(check_autoRead);
	Add(check_autoRip);
	Add(check_autoEject);
	Add(group_cdoptions);
	Add(check_locktray);
	Add(check_ntscsi);
	Add(group_cdinfo);
	Add(check_readCDText);
	Add(check_readCDPlayerIni);

	SetSize(Size(544, 192));
}

BonkEnc::ConfigureCDRip::~ConfigureCDRip()
{
	DeleteObject(group_drive);
	DeleteObject(combo_drive);
	DeleteObject(group_ripping);
	DeleteObject(check_paranoia);
	DeleteObject(combo_paranoia_mode);
	DeleteObject(check_jitter);
	DeleteObject(check_swapchannels);
	DeleteObject(group_automatization);
	DeleteObject(check_autoRead);
	DeleteObject(check_autoRip);
	DeleteObject(check_autoEject);
	DeleteObject(group_cdoptions);
	DeleteObject(check_locktray);
	DeleteObject(check_ntscsi);
	DeleteObject(group_cdinfo);
	DeleteObject(check_readCDText);
	DeleteObject(check_readCDPlayerIni);
}

Void BonkEnc::ConfigureCDRip::SetParanoia()
{
	if (cdparanoia)	combo_paranoia_mode->Activate();
	else		combo_paranoia_mode->Deactivate();
}

Void BonkEnc::ConfigureCDRip::ToggleAutoRead()
{
	if (autoRead)	check_autoRip->Activate();
	else		check_autoRip->Deactivate();
}

Int BonkEnc::ConfigureCDRip::SaveSettings()
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (Config::Get()->enable_cdrip && config->cdrip_numdrives >= 1) config->cdrip_activedrive = combo_drive->GetSelectedEntryNumber();

	config->cdrip_paranoia		= cdparanoia;
	config->cdrip_paranoia_mode	= combo_paranoia_mode->GetSelectedEntryNumber();
	config->cdrip_jitter		= jitter;
	config->cdrip_swapchannels	= swapchannels;
	config->cdrip_locktray		= locktray;
	config->cdrip_ntscsi		= ntscsi;
	config->cdrip_autoRead		= autoRead;
	config->cdrip_autoRip		= autoRip;
	config->cdrip_autoEject		= autoEject;
	config->cdrip_read_cdtext	= readCDText;
	config->cdrip_read_cdplayerini	= readCDPlayerIni;

	return Success();
}
