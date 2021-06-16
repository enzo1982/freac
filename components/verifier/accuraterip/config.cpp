 /* fre:ac - free audio converter
  * Copyright (C) 2001-2021 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "config.h"
#include "database.h"
#include "drive.h"
#include "driveoffsets.h"

const String	 BoCA::ConfigureAccurateRip::ConfigID		      = "AccurateRip";
const String	 BoCA::ConfigureAccurateRip::ConfigCategoryRipperID   = "Ripper";

const String	 BoCA::ConfigureAccurateRip::ConfigEnabledID          = "Enabled";
const Bool	 BoCA::ConfigureAccurateRip::ConfigEnabledDefault     = True;

const String	 BoCA::ConfigureAccurateRip::ConfigPathID	      = "Path";
const String	 BoCA::ConfigureAccurateRip::ConfigPathDefault	      = "http://accuraterip.com/accuraterip/";

const String	 BoCA::ConfigureAccurateRip::ConfigEnableCacheID      = "EnableCache";
const Bool	 BoCA::ConfigureAccurateRip::ConfigEnableCacheDefault = True;

const String	 BoCA::ConfigureAccurateRip::ConfigExpireCacheID      = "ExpireCache";
const Int	 BoCA::ConfigureAccurateRip::ConfigExpireCacheDefault = 30;

const String	 BoCA::ConfigureAccurateRip::ResourcesFolder	      = "freac.verifier.accuraterip";

BoCA::ConfigureAccurateRip::ConfigureAccurateRip()
{
	const Config	*config = Config::Get();
	I18n		*i18n	= I18n::Get();

	i18n->SetContext("Verifiers::AccurateRip");

	/* Get configuration.
	 */
	enableAccurateRip = config->GetIntValue(ConfigID, ConfigEnabledID, ConfigEnabledDefault);

	enableCache	  = config->GetIntValue(ConfigID, ConfigEnableCacheID, ConfigEnableCacheDefault);
	expireCache	  = CacheDaysToIndex(config->GetIntValue(ConfigID, ConfigExpireCacheID, ConfigExpireCacheDefault));

	/* General settings group.
	 */
	group_settings		= new GroupBox(i18n->TranslateString("General settings"), Point(7, 11), Size(450, 67));

	check_enabled		= new CheckBox(i18n->TranslateString("Enable AccurateRip"), Point(10, 13), Size(200, 0), &enableAccurateRip);
	check_enabled->onAction.Connect(&ConfigureAccurateRip::ToggleEnabled, this);

	text_path		= new Text(i18n->AddColon(i18n->TranslateString("Database URL")), Point(27, 41));
	edit_path		= new EditBox(config->GetStringValue(ConfigID, ConfigPathID, ConfigPathDefault), Point(text_path->GetUnscaledTextWidth() + 34, 38), Size(100, 0));

	list_path		= new List();
	list_path->AddEntry(ConfigPathDefault);

	edit_path->SetDropDownList(list_path);

	group_settings->Add(check_enabled);

	group_settings->Add(text_path);
	group_settings->Add(edit_path);

	/* Drives group.
	 */
	group_drives		= new GroupBox(i18n->TranslateString("Drives"), Point(7, 90), Size(450, 113));

	text_drive		= new Text(i18n->AddColon(i18n->TranslateString("Drive")), Point(9, 13));
	combo_drive		= new ComboBox(Point(text_drive->GetUnscaledTextWidth() + 16, 10), Size(100, 0));

	UpdateDriveList();

	combo_drive->SelectNthEntry(config->GetIntValue(ConfigCategoryRipperID, "ActiveDrive", 0));
	combo_drive->onSelectEntry.Connect(&ConfigureAccurateRip::SelectDrive, this);

	text_status		= new Text(i18n->AddColon(i18n->TranslateString("Offset configuration")), Point(27, 36));
	text_drivedb		= new Text(i18n->AddColon(i18n->TranslateString("Offset in drive database")), Point(27, 54));
	text_detected		= new Text(i18n->AddColon(i18n->TranslateString("Detected offset")), Point(27, 72));
	text_configured		= new Text(i18n->AddColon(i18n->TranslateString("Configured offset")), Point(27, 90));

	Int	 maxTextSize = Math::Max(Math::Max(text_status->GetUnscaledTextWidth(), text_drivedb->GetUnscaledTextWidth()),
					 Math::Max(text_detected->GetUnscaledTextWidth(), text_configured->GetUnscaledTextWidth()));

	text_status_value	= new Text(NIL, Point(maxTextSize + 33, 36));
	text_drivedb_offset	= new Text(NIL, Point(maxTextSize + 33, 54));
	text_detected_offset	= new Text(NIL, Point(maxTextSize + 33, 72));
	text_configured_offset	= new Text(NIL, Point(maxTextSize + 33, 90));

	button_configure	= new Button(i18n->TranslateString("Detect offset now"), Point(88, 68), Size(0, 0));
	button_configure->SetOrientation(OR_UPPERRIGHT);
	button_configure->SetWidth(Math::Max(80, button_configure->GetUnscaledTextWidth() + 16));
	button_configure->SetX(button_configure->GetWidth() + 10);
	button_configure->onAction.Connect(&ConfigureAccurateRip::DetectOffset, this);

	group_drives->Add(text_drive);
	group_drives->Add(combo_drive);
	group_drives->Add(text_status);
	group_drives->Add(text_status_value);
	group_drives->Add(text_drivedb);
	group_drives->Add(text_drivedb_offset);
	group_drives->Add(text_detected);
	group_drives->Add(text_detected_offset);
	group_drives->Add(text_configured);
	group_drives->Add(text_configured_offset);
	group_drives->Add(button_configure);

	/* Cache settings group.
	 */
	group_cache		= new GroupBox(i18n->TranslateString("Cache"), Point(7, 215), Size(450, 65));

	check_cache		= new CheckBox(i18n->TranslateString("Enable cache"), Point(10, 13), Size(200, 0), &enableCache);
	check_cache->onAction.Connect(&ConfigureAccurateRip::ToggleCache, this);

	text_expire		= new Text(i18n->AddColon(i18n->TranslateString("Expires after")), Point(27, 40));

	slider_expire		= new Slider(Point(text_expire->GetUnscaledTextWidth() + 34, 38), Size(200, 0), OR_HORZ, &expireCache, 0, 7);
	slider_expire->onValueChange.Connect(&ConfigureAccurateRip::SetExpireTime, this);

	text_expire_value	= new Text(NIL, Point(0, 40));

	group_cache->Add(check_cache);

	group_cache->Add(text_expire);
	group_cache->Add(slider_expire);
	group_cache->Add(text_expire_value);

	Add(group_settings);
	Add(group_drives);
	Add(group_cache);

	ToggleEnabled();
	SetExpireTime();
	SelectDrive();

	/* Adjust element widths.
	 */
	Font	 font	   = text_status_value->GetFont();

	maxTextSize = Math::Max(Math::Max(font.GetUnscaledTextSizeX(i18n->TranslateString("not configured")), font.GetUnscaledTextSizeX(i18n->TranslateString("automatically configured"))),
				Math::Max(font.GetUnscaledTextSizeX(i18n->TranslateString("configured by offset detection")), font.GetUnscaledTextSizeX(i18n->TranslateString("manually configured"))));

	Int	 line1Size = maxTextSize + 10;
	Int	 line3Size = font.GetUnscaledTextSizeX(i18n->TranslateString("not detected")) + button_configure->GetWidth() + 18;

	group_settings->SetWidth(Math::Max(450, text_status_value->GetX() + Math::Max(line1Size, line3Size)));
	group_drives->SetWidth(group_settings->GetWidth());
	group_cache->SetWidth(group_settings->GetWidth());

	edit_path->SetWidth(group_settings->GetWidth() - text_path->GetUnscaledTextWidth() - 44);
	combo_drive->SetWidth(group_drives->GetWidth() - text_drive->GetUnscaledTextWidth() - 26);

	maxTextSize = Math::Max(Math::Max(font.GetUnscaledTextSizeX(i18n->TranslateString("%1 day(s)").Replace("%1", "0")),   font.GetUnscaledTextSizeX(i18n->TranslateString("%1 week(s)").Replace("%1", "0"))),
		      Math::Max(Math::Max(font.GetUnscaledTextSizeX(i18n->TranslateString("%1 month(s)").Replace("%1", "0")), font.GetUnscaledTextSizeX(i18n->TranslateString("%1 year(s)").Replace("%1", "0"))),
					  font.GetUnscaledTextSizeX(i18n->TranslateString("never"))));

	text_expire_value->SetX(group_cache->GetWidth() - maxTextSize - 10);
	slider_expire->SetWidth(group_cache->GetWidth() - maxTextSize - slider_expire->GetX() - 17);

	maxTextSize = Math::Max(check_enabled->GetUnscaledTextWidth(), check_cache->GetUnscaledTextWidth());

	check_enabled->SetWidth(maxTextSize + 20);
	check_cache->SetWidth(maxTextSize + 20);

	SetSize(Size(group_cache->GetWidth() + 14, 287));
}

BoCA::ConfigureAccurateRip::~ConfigureAccurateRip()
{
	DeleteObject(group_settings);
	DeleteObject(check_enabled);

	DeleteObject(edit_path);
	DeleteObject(text_path);
	DeleteObject(list_path);

	DeleteObject(group_drives);
	DeleteObject(text_drive);
	DeleteObject(combo_drive);
	DeleteObject(text_status);
	DeleteObject(text_status_value);
	DeleteObject(text_drivedb);
	DeleteObject(text_drivedb_offset);
	DeleteObject(text_detected);
	DeleteObject(text_detected_offset);
	DeleteObject(text_configured);
	DeleteObject(text_configured_offset);
	DeleteObject(button_configure);

	DeleteObject(group_cache);
	DeleteObject(check_cache);

	DeleteObject(text_expire);
	DeleteObject(slider_expire);
	DeleteObject(text_expire_value);
}

Int BoCA::ConfigureAccurateRip::CacheDaysToIndex(Int days)
{
	if	(days ==  0) return 7;
	else if	(days <   4) return 0;
	else if	(days <  11) return 1;
	else if	(days <  22) return 2;
	else if	(days <  60) return 3;
	else if	(days < 135) return 4;
	else if	(days < 270) return 5;
	else		     return 6;
}

Int BoCA::ConfigureAccurateRip::CacheIndexToDays(Int index)
{
	if	(index == 0) return 1;
	else if	(index == 1) return 7;
	else if	(index == 2) return 14;
	else if	(index == 3) return 30;
	else if	(index == 4) return 90;
	else if	(index == 5) return 180;
	else if	(index == 6) return 365;
	else		     return 0;
}

Void BoCA::ConfigureAccurateRip::ToggleEnabled()
{
	if (enableAccurateRip)
	{
		text_path->Activate();
		edit_path->Activate();

		group_drives->Activate();
		group_cache->Activate();
	}
	else
	{
		text_path->Deactivate();
		edit_path->Deactivate();

		group_drives->Deactivate();
		group_cache->Deactivate();
	}
}

Void BoCA::ConfigureAccurateRip::ToggleCache()
{
	if (enableCache)
	{
		text_expire->Activate();
		slider_expire->Activate();
		text_expire_value->Activate();
	}
	else
	{
		text_expire->Deactivate();
		slider_expire->Deactivate();
		text_expire_value->Deactivate();
	}
}

Void BoCA::ConfigureAccurateRip::SetExpireTime()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Verifiers::AccurateRip");

	switch (expireCache)
	{
		case 0: text_expire_value->SetText(i18n->TranslateString("%1 day(s)").Replace("%1", "1"));   break;
		case 1: text_expire_value->SetText(i18n->TranslateString("%1 week(s)").Replace("%1", "1"));  break;
		case 2: text_expire_value->SetText(i18n->TranslateString("%1 week(s)").Replace("%1", "2"));  break;
		case 3: text_expire_value->SetText(i18n->TranslateString("%1 month(s)").Replace("%1", "1")); break;
		case 4: text_expire_value->SetText(i18n->TranslateString("%1 month(s)").Replace("%1", "3")); break;
		case 5: text_expire_value->SetText(i18n->TranslateString("%1 month(s)").Replace("%1", "6")); break;
		case 6: text_expire_value->SetText(i18n->TranslateString("%1 year(s)").Replace("%1", "1"));  break;
		case 7: text_expire_value->SetText(i18n->TranslateString("never"));			     break;
	}
}

Void BoCA::ConfigureAccurateRip::UpdateDriveList()
{
	AS::Registry		&boca = AS::Registry::Get();
	AS::DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

	if (info != NIL)
	{
		if (info->GetNumberOfDevices() == 0) group_drives->Deactivate();
		else				     group_drives->Activate();

		String	 selectedPath = selectedDrivePath;

		combo_drive->RemoveAllEntries();

		for (Int i = 0; i < info->GetNumberOfDevices(); i++)
		{
			const Device	&device	  = info->GetNthDeviceInfo(i);
			Bool		 selected = (device.path == selectedPath);

			combo_drive->AddEntry(device.GetName());

			if (selected) combo_drive->SelectNthEntry(i);
		}

		boca.DeleteComponent(info);
	}
}

Void BoCA::ConfigureAccurateRip::SelectDrive()
{
	AS::Registry		&boca = AS::Registry::Get();
	AS::DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

	if (info == NIL || info->GetNumberOfDevices() == 0)
	{
		if (info != NIL) boca.DeleteComponent(info);

		group_drives->Deactivate();

		return;
	}

	Config	*config = Config::Get();

	/* Find configuration level.
	 */
	Int			 driveNumber = combo_drive->GetSelectedEntryNumber();
	Drive			 drive(config, driveNumber);
	ConfigurationLevel	 configurationLevel = drive.GetConfigurationLevel();

	if (configurationLevel == ConfigurationLevelUnconfigured) configurationLevel = drive.ConfigureOffset(True, False);

	/* Assign configuration level status.
	 */
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Verifiers::AccurateRip");

	switch (configurationLevel)
	{
		case ConfigurationLevelUnconfigured:   text_status_value->SetText(i18n->TranslateString("not configured"));		    break;
		case ConfigurationLevelAutomatic:      text_status_value->SetText(i18n->TranslateString("automatically configured"));	    break;
		case ConfigurationLevelVerified:       text_status_value->SetText(i18n->TranslateString("configured by offset detection")); break;
		case ConfigurationLevelManual:
		case ConfigurationLevelManualOverride: text_status_value->SetText(i18n->TranslateString("manually configured"));	    break;
	}

	/* Find offset in database.
	 */
	DriveOffsets	*offsets       = DriveOffsets::Get();
	const Device	&device	       = info->GetNthDeviceInfo(driveNumber);
	Int		 drivedbOffset = offsets->FindDriveOffset(device).offset;

	if (drivedbOffset == DriveOffset::Invalid) text_drivedb_offset->SetText(i18n->TranslateString("not present"));
	else					   text_drivedb_offset->SetText(String::FromInt(drivedbOffset));

	/* Find detected offset.
	 */
	Int		 detectedOffset = drive.GetDetectedOffset();

	if (detectedOffset == DriveOffset::Invalid) text_detected_offset->SetText(i18n->TranslateString("not detected"));
	else					    text_detected_offset->SetText(String::FromInt(detectedOffset));

	/* Find configured offset.
	 */
	String	 driveID   = device.GetID();

	Bool	 useOffset = config->GetIntValue(ConfigCategoryRipperID, String("UseOffsetDrive-").Append(driveID), False);
	Int	 offset	   = config->GetIntValue(ConfigCategoryRipperID, String("ReadOffsetDrive-").Append(driveID), 0);

	if (useOffset) text_configured_offset->SetText(String::FromInt(offset));
	else	       text_configured_offset->SetText(i18n->TranslateString("disabled"));

	/* Save device path.
	 */
	selectedDrivePath = device.path;

	boca.DeleteComponent(info);
}

Void BoCA::ConfigureAccurateRip::DetectOffset()
{
	Config	*config = Config::Get();

	Drive	 drive(config, combo_drive->GetSelectedEntryNumber());

	drive.ConfigureOffset();

	/* Update offset configuration.
	 */
	UpdateDriveList();
}

Int BoCA::ConfigureAccurateRip::SaveSettings()
{
	Config	*config = Config::Get();

	config->SetIntValue(ConfigID, ConfigEnabledID, enableAccurateRip);

	String	 path = edit_path->GetText();

	if (!path.EndsWith("/")) path.Append("/");
	if (path.StartsWith("https://")) path.Replace("https://", "http://");

	if (config->GetStringValue(ConfigID, ConfigPathID, ConfigPathDefault) != path) Database::InitializeSessionCache();

	config->SetStringValue(ConfigID, ConfigPathID, path);

	config->SetIntValue(ConfigID, ConfigEnableCacheID, enableCache);
	config->SetIntValue(ConfigID, ConfigExpireCacheID, CacheIndexToDays(expireCache));

	return Success();
}
