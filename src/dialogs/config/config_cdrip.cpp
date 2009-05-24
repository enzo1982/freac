 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
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
	BoCA::Config	*config = BoCA::Config::Get();

	cdparanoia	= config->cdrip_paranoia;
	jitter		= config->cdrip_jitter;
	swapchannels	= config->cdrip_swapchannels;
	locktray	= config->GetIntValue("CDRip", "LockTray", True);
	ntscsi		= config->GetIntValue("CDRip", "UseNTSCSI", True);
	autoRead	= config->GetIntValue("CDRip", "AutoReadContents", True);
	autoRip		= config->GetIntValue("CDRip", "AutoRip", False);
	autoEject	= config->GetIntValue("CDRip", "EjectAfterRipping", False);
	readCDText	= config->GetIntValue("CDRip", "ReadCDText", True);
	readCDPlayerIni	= config->GetIntValue("CDRip", "ReadCDPlayerIni", True);
	readISRC	= config->GetIntValue("CDRip", "ReadISRC", False);

	group_drive	= new GroupBox(BonkEnc::i18n->TranslateString("Active CD-ROM drive"), Point(7, 11), Size(344, 43));

	combo_drive	= new ComboBox(Point(10, 12), Size(324, 0));

	for (Int j = 0; j < config->cdrip_numdrives; j++)
	{
		combo_drive->AddEntry(config->cdrip_drives.GetNth(j));
	}

	combo_drive->SelectNthEntry(config->cdrip_activedrive);

	group_drive->Add(combo_drive);

	group_cdinfo		= new GroupBox(BonkEnc::i18n->TranslateString("CD information"), Point(7, 66), Size(344, 65));

	check_readCDText	= new CheckBox(BonkEnc::i18n->TranslateString("Read CD Text"), Point(10, 11), Size(157, 0), &readCDText);
	check_readCDPlayerIni	= new CheckBox(BonkEnc::i18n->TranslateString("Read cdplayer.ini"), Point(176, 11), Size(157, 0), &readCDPlayerIni);
	check_readISRC		= new CheckBox(BonkEnc::i18n->TranslateString("Read ISRC when adding tracks to joblist"), Point(10, 37), Size(323, 0), &readISRC);

	group_cdinfo->Add(check_readCDText);
	group_cdinfo->Add(check_readCDPlayerIni);
	group_cdinfo->Add(check_readISRC);

	group_ripping		= new GroupBox(BonkEnc::i18n->TranslateString("Ripper settings"), Point(7, 143), Size(344, 68));

	check_paranoia		= new CheckBox(BonkEnc::i18n->TranslateString("Activate cdparanoia mode:"), Point(10, 14), Size(157, 0), &cdparanoia);
	check_paranoia->onAction.Connect(&ConfigureCDRip::SetParanoia, this);

	combo_paranoia_mode= new ComboBox(Point(176, 13), Size(158, 0));
	combo_paranoia_mode->AddEntry(BonkEnc::i18n->TranslateString("Overlap only"));
	combo_paranoia_mode->AddEntry(BonkEnc::i18n->TranslateString("No verify"));
	combo_paranoia_mode->AddEntry(BonkEnc::i18n->TranslateString("No scratch repair"));
	combo_paranoia_mode->AddEntry(BonkEnc::i18n->TranslateString("Full cdparanoia mode"));
	combo_paranoia_mode->SelectNthEntry(config->cdrip_paranoia_mode);

	if (!cdparanoia) combo_paranoia_mode->Deactivate();

	check_jitter		= new CheckBox(BonkEnc::i18n->TranslateString("Activate jitter correction"), Point(10, 40), Size(157, 0), &jitter);
	check_swapchannels	= new CheckBox(BonkEnc::i18n->TranslateString("Swap left/right channel"), Point(176, 40), Size(157, 0), &swapchannels);

	group_ripping->Add(check_paranoia);
	group_ripping->Add(combo_paranoia_mode);
	group_ripping->Add(check_jitter);
	group_ripping->Add(check_swapchannels);

	group_automatization	= new GroupBox(BonkEnc::i18n->TranslateString("Automatization"), Point(359, 11), Size(178, 94));

	check_autoRead	= new CheckBox(BonkEnc::i18n->TranslateString("Read CD contents on insert"), Point(10, 14), Size(157, 0), &autoRead);
	check_autoRead->onAction.Connect(&ConfigureCDRip::ToggleAutoRead, this);

	check_autoRip	= new CheckBox(BonkEnc::i18n->TranslateString("Start ripping automatically"), check_autoRead->GetPosition() + Point(0, 26), Size(157, 0), &autoRip);
	check_autoEject	= new CheckBox(BonkEnc::i18n->TranslateString("Eject disk after ripping"), check_autoRip->GetPosition() + Point(0, 26), Size(157, 0), &autoEject);

	group_automatization->Add(check_autoRead);
	group_automatization->Add(check_autoRip);
	group_automatization->Add(check_autoEject);

	group_cdoptions	= new GroupBox(BonkEnc::i18n->TranslateString("CD options"), Point(359, 143), Size(178, 68));

	check_locktray	= new CheckBox(BonkEnc::i18n->TranslateString("Lock CD tray while ripping"), Point(10, 14), Size(157, 0), &locktray);
	check_ntscsi	= new CheckBox(BonkEnc::i18n->TranslateString("Use native NT SCSI library"), check_locktray->GetPosition() + Point(0, 26), Size(157, 0), &ntscsi);

	group_cdoptions->Add(check_locktray);
	group_cdoptions->Add(check_ntscsi);

#ifdef __WIN32__
	OSVERSIONINFOA	 vInfo;

	vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

	GetVersionExA(&vInfo);

	if (vInfo.dwPlatformId != VER_PLATFORM_WIN32_NT) check_ntscsi->Deactivate();
#else
	check_ntscsi->Deactivate();
#endif

	ToggleAutoRead();

	Add(group_drive);
	Add(group_ripping);
	Add(group_automatization);
	Add(group_cdoptions);
	Add(group_cdinfo);

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
	DeleteObject(check_readISRC);
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
	config->SetIntValue("CDRip", "LockTray", locktray);
	config->SetIntValue("CDRip", "UseNTSCSI", ntscsi);
	config->SetIntValue("CDRip", "AutoReadContents", autoRead);
	config->SetIntValue("CDRip", "AutoRip", autoRip);
	config->SetIntValue("CDRip", "EjectAfterRipping", autoEject);
	config->SetIntValue("CDRip", "ReadCDText", readCDText);
	config->SetIntValue("CDRip", "ReadCDPlayerIni", readCDPlayerIni);
	config->SetIntValue("CDRip", "ReadISRC", readISRC);

	return Success();
}
