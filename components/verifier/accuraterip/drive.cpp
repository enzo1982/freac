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

#include "drive.h"
#include "driveoffsets.h"
#include "algorithm.h"
#include "database.h"
#include "config.h"

using namespace BoCA::AS;

using namespace smooth::GUI::Dialogs;

BoCA::Drive::Drive(const Config *iConfig, Int driveNumber)
{
	config = iConfig;

	Registry		&boca = Registry::Get();
	DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

	if (info != NIL)
	{
		device = info->GetNthDeviceInfo(driveNumber);

		boca.DeleteComponent(info);
	} 
}

BoCA::Drive::~Drive()
{
}

Int BoCA::Drive::FindCurrentDriveNumber() const
{
	Registry		&boca = Registry::Get();
	DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

	if (info != NIL)
	{
		for (Int i = 0; i < info->GetNumberOfDevices(); i++)
		{
			if (info->GetNthDeviceInfo(i).path != device.path) continue;

			boca.DeleteComponent(info);

			return i;
		}

		boca.DeleteComponent(info);
	} 

	return -1;
}

BoCA::ConfigurationLevel BoCA::Drive::GetConfigurationLevel(Bool ignoreManual) const
{
	String			 driveID	    = device.GetID();
	ConfigurationLevel	 configurationLevel = (ConfigurationLevel) config->GetIntValue(ConfigureAccurateRip::ConfigID, String("DriveLevel-").Append(driveID), ConfigurationLevelUnconfigured);

	if (ignoreManual) return configurationLevel;

	DriveOffsets		*offsets	    = DriveOffsets::Get();

	Int			 detectedOffset	    = config->GetIntValue(ConfigureAccurateRip::ConfigID, String("DriveOffset-").Append(driveID), DriveOffset::Invalid);
	Int			 drivedbOffset	    = offsets->FindDriveOffset(device).offset;

	Bool			 ripperOffsetUsed   = config->GetIntValue(ConfigureAccurateRip::ConfigCategoryRipperID, String("UseOffsetDrive-").Append(driveID), False);
	Int			 ripperOffset	    = config->GetIntValue(ConfigureAccurateRip::ConfigCategoryRipperID, String("ReadOffsetDrive-").Append(driveID), 0);

	if (configurationLevel == ConfigurationLevelVerified)
	{
		if ((detectedOffset != 0 && !ripperOffsetUsed) || (ripperOffsetUsed && ripperOffset != detectedOffset)) return ConfigurationLevelManualOverride;
	}
	else if (configurationLevel == ConfigurationLevelAutomatic)
	{
		if ((drivedbOffset  != 0 && !ripperOffsetUsed) || (ripperOffsetUsed && ripperOffset != drivedbOffset )) return ConfigurationLevelManual;
	}
	else if (configurationLevel == ConfigurationLevelUnconfigured)
	{
		if (ripperOffsetUsed) return ConfigurationLevelManual;
	}

	return configurationLevel;
}

Int BoCA::Drive::GetDetectedOffset() const
{
	String	 driveID = device.GetID();

	return config->GetIntValue(ConfigureAccurateRip::ConfigID, String("DriveOffset-").Append(driveID), DriveOffset::Invalid);
}

BoCA::ConfigurationLevel BoCA::Drive::ConfigureOffset(Bool automatic, Bool askToDetect) const
{
	ConfigurationLevel	 configurationLevel = GetConfigurationLevel(True);

	if (configurationLevel > ConfigurationLevelUnconfigured && automatic) return configurationLevel;

	AS::Registry	&boca = AS::Registry::Get();
	I18n		*i18n = I18n::Get();

	i18n->SetContext("Verifiers::AccurateRip::Messages");

	/* Automatic configuration using drive database. 
	 */
	DriveOffsets	*offsets       = DriveOffsets::Get();
	Int		 drivedbOffset = offsets->FindDriveOffset(device).offset;

	if (configurationLevel < ConfigurationLevelAutomatic)
	{
		if (drivedbOffset != DriveOffset::Invalid) configurationLevel = ConfigurationLevelAutomatic;
	}

	/* Ask to detect drive offset if not listed in drive database.
	 */
	Bool	 detectOffset = False;

	if (configurationLevel < ConfigurationLevelAutomatic && automatic && askToDetect)
	{
		Config	*config	       = Config::Get();
		Bool	 doNotAskAgain = !config->GetIntValue(ConfigureAccurateRip::ConfigID, "AskToConfigure", True);

		if (doNotAskAgain) return configurationLevel;

		MessageDlg	 messageBox(i18n->TranslateString("Your CD drive could not be found in the AccurateRip database.\n\nDrive name: %1\n\nWould you like to run an automatic drive offset detection now?").Replace("%1", device.GetName()), i18n->TranslateString("AccurateRip configuration"), Message::Buttons::YesNo, Message::Icon::Question, i18n->TranslateString("Do not ask again"), &doNotAskAgain);

		messageBox.ShowDialog();

		config->SetIntValue(ConfigureAccurateRip::ConfigID, "AskToConfigure", !doNotAskAgain);

		if (messageBox.GetButtonCode() != Message::Button::Yes) return configurationLevel;

		detectOffset = True;
	}

	/* Perform offset detection.
	 */
	Int	 detectedOffset = DriveOffset::Invalid;

	if (!automatic || detectOffset)
	{
		DatabaseEntry	 entry = GetDatabaseEntry();

		if (!entry.IsValid()) return configurationLevel;

		/* Disable automatic handling of inserted CDs.
		 */
		Bool	 autoReadContents = ToggleAutoRead(False);

		/* Read samples for offset detection.
		 */
		Config	*config	     = Config::Copy();

		String	 driveID     = device.GetID();
		Int	 driveNumber = FindCurrentDriveNumber();

		config->SetIntValue(ConfigureAccurateRip::ConfigCategoryRipperID, String("UseOffsetDrive-").Append(driveID), False);
		config->SetIntValue(ConfigureAccurateRip::ConfigCategoryRipperID, String("SpinUpTimeDrive-").Append(driveID), -1);

		config->SetIntValue(ConfigureAccurateRip::ConfigCategoryRipperID, "ReadCDText", False);
		config->SetIntValue(ConfigureAccurateRip::ConfigCategoryRipperID, "ReadISRC", False);

		config->SetIntValue(ConfigureAccurateRip::ConfigCategoryRipperID, "JitterCorrection", False);
		config->SetIntValue(ConfigureAccurateRip::ConfigCategoryRipperID, "CDParanoia", False);

		Array<Int>	 candidates;

		while (True)
		{
			Array<Int>	 offsets;

			Int			 trackNumber = 1;
			String			 trackURI    = String("device://cdda:").Append(String::FromInt(driveNumber).Append("/").Append(String::FromInt(trackNumber)));
			Buffer<UnsignedByte>	 samples;

			AS::DecoderComponent	*decoder = boca.CreateDecoderForStream(trackURI, config);

			if (decoder == NIL) break;

			Track	 track;

			decoder->GetStreamInfo(trackURI, track);
			decoder->SetAudioTrackInfo(track);

			decoder->Activate();
			decoder->Seek(445 * samplesPerSector);

			Int	 bytes = decoder->ReadData(samples);

			if (bytes >= 11 * bytesPerSector)
			{
				Algorithm	 ar;

				for (Int i = 0; i < 10 * bytesPerSector; i += 4)
				{
					Buffer<UnsignedByte>	 sector(bytesPerSector);

					memcpy(sector, samples + i, sector.Size());

					if (track.cdTrack == 1) track.cdTrack = 2; // To avoid skipping first 5 sectors in CRC calculation.

					ar.Init(track);
					ar.ProcessData(sector);

					if (entry.VerifyOffsetDetection(trackNumber, ar.GetCRCv1())) offsets.Add(i / 4 - 5 * samplesPerSector);
				}
			}

			decoder->Deactivate();

			boca.DeleteComponent(decoder);

			/* Check candidates against newly detected offsets.
			 */
			if (candidates.Length() == 0)
			{
				foreach (Int offset, offsets)
				{
					/* Accept offset if it matches the one found in the drive database.
					 */
					if (offset == drivedbOffset)
					{
						candidates.RemoveAll();
						candidates.Add(drivedbOffset);

						break;
					}

					candidates.Add(offset);
				}
			}
			else
			{
				/* Check each previous candidate and remove it if
				 * it is not among the newly detected offsets.
				 */
				foreachreverse (Int candidate, candidates)
				{
					Bool	 found = False;

					foreach (Int offset, offsets)
					{
						if (offset != candidate) continue;

						found = True;
						break;
					}

					if (!found) candidates.RemoveNth(foreachindex);
				}
			}

			if (candidates.Length() == 1) break;

			/* Open drive tray.
			 */
			AS::DeviceInfoComponent	*deviceInfo = boca.CreateDeviceInfoComponent();

			if (deviceInfo == NIL) break;

			deviceInfo->OpenNthDeviceTray(driveNumber);

			boca.DeleteComponent(deviceInfo);

			/* Ask for another disc.
			 */
			String	 message = i18n->TranslateString("Offset detection needs to check another disc.").Append("\n\n").Append(i18n->TranslateString("Please insert another disc into the CD drive and click OK.\n\nThe disc must be present in the AccurateRip database. The chance for this is\nhighest for popular CDs that have likely been ripped by other users before."));

			if (QuickMessage(message, i18n->TranslateString("AccurateRip offset detection"), Message::Buttons::OkCancel, Message::Icon::Information) != Message::Button::Ok) break;

			entry	    = GetDatabaseEntry();
			driveNumber = FindCurrentDriveNumber();

			if (!entry.IsValid()) break;
		}

		Config::Free(config);

		/* Restore automatic handling of inserted CDs.
		 */
		ToggleAutoRead(autoReadContents);

		/* Check configuration result.
		 */
		if (candidates.Length() != 1) return configurationLevel;

		configurationLevel = ConfigurationLevelVerified;
		detectedOffset	   = candidates.GetFirst();

		QuickMessage(i18n->TranslateString("The drive offset has been successfully detected and configured to %1 samples.").Replace("%1", String::FromInt(detectedOffset)), i18n->TranslateString("AccurateRip offset detection"), Message::Buttons::Ok, Message::Icon::Information);
	}

	/* Save configuration level.
	 */
	Config	*config	 = Config::Get();
	String	 driveID = device.GetID();

	Bool	 ripperOffsetUsed = config->GetIntValue(ConfigureAccurateRip::ConfigCategoryRipperID, String("UseOffsetDrive-").Append(driveID), False);

	config->SetIntValue(ConfigureAccurateRip::ConfigID, String("DriveLevel-").Append(driveID), configurationLevel);

	if (configurationLevel == ConfigurationLevelAutomatic && !ripperOffsetUsed)
	{
		config->SetIntValue(ConfigureAccurateRip::ConfigCategoryRipperID, String("UseOffsetDrive-").Append(driveID), True);
		config->SetIntValue(ConfigureAccurateRip::ConfigCategoryRipperID, String("ReadOffsetDrive-").Append(driveID), drivedbOffset);
	}
	else if (configurationLevel == ConfigurationLevelVerified)
	{
		config->SetIntValue(ConfigureAccurateRip::ConfigID, String("DriveOffset-").Append(driveID), detectedOffset);

		config->SetIntValue(ConfigureAccurateRip::ConfigCategoryRipperID, String("UseOffsetDrive-").Append(driveID), True);
		config->SetIntValue(ConfigureAccurateRip::ConfigCategoryRipperID, String("ReadOffsetDrive-").Append(driveID), detectedOffset);
	}

	/* Notify of ripper configuration change.
	 */	
	if	(boca.ComponentExists("cdio-dec"))	 Settings::onChangeComponentSettings.Emit("cdio-dec");
	else if (boca.ComponentExists("cdparanoia-dec")) Settings::onChangeComponentSettings.Emit("cdparanoia-dec");
	else if (boca.ComponentExists("cdrip-dec"))	 Settings::onChangeComponentSettings.Emit("cdrip-dec");
	else if (boca.ComponentExists("akrip-dec"))	 Settings::onChangeComponentSettings.Emit("akrip-dec");

	return configurationLevel;
}

BoCA::DatabaseEntry BoCA::Drive::GetDatabaseEntry() const
{
	AS::Registry		&boca	    = AS::Registry::Get();
	AS::DeviceInfoComponent	*deviceInfo = boca.CreateDeviceInfoComponent();

	if (deviceInfo == NIL) return DatabaseEntry();

	/* Disable automatic handling of inserted CDs.
	 */
	Bool	 autoReadContents = ToggleAutoRead(False);

	/* Get inserted CD database entry.
	 */
	DatabaseEntry	 entry;
	I18n		*i18n = I18n::Get();

	while (True)
	{
		/* Wait up to 10 seconds for a disc to be inserted.
		 */
		Int	 driveNumber = -1;
		MCDI	 mcdi	     = MCDI(Buffer<UnsignedByte>());

		for (Int i = 0; i < 20; i++)
		{
			driveNumber = FindCurrentDriveNumber();

			if (driveNumber >= 0)
			{
				mcdi = deviceInfo->GetNthDeviceMCDI(driveNumber);

				if (mcdi.IsValid()) break;
			}

			S::System::System::Sleep(500);
		}

		/* Query entry if a disc was found.
		 */
		if (mcdi.IsValid())
		{
			/* Query AccurateRip database.
			 */
			String	 discID = Algorithm::CalculateDiscID(mcdi);

			entry = Database::Query(config, discID);

			if (entry.IsValid()) break;

			/* Open drive tray.
			 */
			deviceInfo->OpenNthDeviceTray(driveNumber);
		}

		/* Ask for another disc.
		 */
		String	 message = i18n->TranslateString("The inserted disc could not be found in the AccurateRip database.").Append("\n\n").Append(i18n->TranslateString("Please insert another disc into the CD drive and click OK.\n\nThe disc must be present in the AccurateRip database. The chance for this is\nhighest for popular CDs that have likely been ripped by other users before."));

		if (QuickMessage(message, i18n->TranslateString("AccurateRip offset detection"), Message::Buttons::OkCancel, Message::Icon::Information) != Message::Button::Ok) break;
	}

	boca.DeleteComponent(deviceInfo);

	/* Restore automatic handling of inserted CDs.
	 */
	ToggleAutoRead(autoReadContents);

	return entry;
}

Bool BoCA::Drive::ToggleAutoRead(Bool newValue) const
{
	/* Configure automatic handling of inserted CDs.
	 */
	Config	*config	= Config::Get();

	Bool	 previousValue = config->GetIntValue(ConfigureAccurateRip::ConfigCategoryRipperID, "AutoReadContents", True);

	config->SetIntValue(ConfigureAccurateRip::ConfigCategoryRipperID, "AutoReadContents", newValue);

	return previousValue;
}
