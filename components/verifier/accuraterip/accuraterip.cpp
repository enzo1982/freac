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

#include <smooth.h>
#include <smooth/dll.h>

#include "accuraterip.h"
#include "database.h"
#include "drive.h"
#include "driveoffsets.h"
#include "config.h"

const String &BoCA::VerifierAccurateRip::GetComponentSpecs()
{
	Database::InitializeSessionCache();

	I18n	*i18n = I18n::Get();

	i18n->SetContext("Components::Verifiers");

	static String	 componentSpecs = String("						\
												\
	  <?xml version=\"1.0\" encoding=\"UTF-8\"?>						\
	  <component>										\
	    <name>").Append(i18n->TranslateString("AccurateRip Verifier")).Append("</name>	\
	    <version>1.0</version>								\
	    <id>accuraterip-verify</id>								\
	    <type>verifier</type>								\
	  </component>										\
												\
	");

	return componentSpecs;
}

Void smooth::AttachDLL(Void *instance)
{
}

Void smooth::DetachDLL()
{
	BoCA::DriveOffsets::Free();
}

using namespace smooth::IO;
using namespace smooth::Net;

BoCA::VerifierAccurateRip::VerifierAccurateRip()
{
	configLayer = NIL;
}

BoCA::VerifierAccurateRip::~VerifierAccurateRip()
{
	if (configLayer != NIL) Object::DeleteObject(configLayer);
}

Bool BoCA::VerifierAccurateRip::CanVerifyTrack(const Track &track)
{
	const Config	*config = GetConfiguration();

	if (!config->GetIntValue(ConfigureAccurateRip::ConfigID, ConfigureAccurateRip::ConfigEnabledID, ConfigureAccurateRip::ConfigEnabledDefault)) return False;

	if (track.isCDTrack) return True;
	else		     return False;
}

Bool BoCA::VerifierAccurateRip::Activate()
{
	return algorithm.Init(track);
}

Int BoCA::VerifierAccurateRip::ProcessData(Buffer<UnsignedByte> &data)
{
	return algorithm.ProcessData(data);
}

Bool BoCA::VerifierAccurateRip::Verify()
{
	/* Check if we are invoked in response to a disc insert.
	 */
	const Config	*config = GetConfiguration();

	if (config->GetIntValue(ConfigureAccurateRip::ConfigID, "DiscInsert", -1) != -1) return OnDiscInsert();

	/* Calculate AccurateRip disc ID and query database.
	 */
	String			 discID	= Algorithm::CalculateDiscID(track.GetInfo().mcdi);
	const DatabaseEntry	&entry	= Database::Query(config, discID);

	if (!entry.IsValid())
	{
		/* Cannot verify track if not found in AccurateRip database.
		 */
		errorString = String("Disc ID not found in AccurateRip database: ").Append(discID);

		return False;
	}

	/* Verify checksums and construct result message.
	 */
	UnsignedInt32	 arCRCv1 = algorithm.GetCRCv1();
	UnsignedInt32	 arCRCv2 = algorithm.GetCRCv2();

	Int	 v1Confidence = entry.Verify(track.cdTrack, arCRCv1);
	Int	 v2Confidence = entry.Verify(track.cdTrack, arCRCv2);

	errorState  = False;
	errorString = NIL;

	if (v1Confidence >= 0 || v2Confidence >= 0)
	{
		errorString.Append("Track has been accurately ripped:");

		if (v1Confidence >= 0) errorString.Append("\n    Checksum (AccurateRip v1): %1").Append(", Confidence: %2").Replace("%1", Number(Int64(arCRCv1)).ToHexString(8)).Replace("%2", String::FromInt(v1Confidence));
		if (v2Confidence >= 0) errorString.Append("\n    Checksum (AccurateRip v2): %1").Append(", Confidence: %2").Replace("%1", Number(Int64(arCRCv2)).ToHexString(8)).Replace("%2", String::FromInt(v2Confidence));

		return True;
	}

	Drive			 drive(config, track.drive);
	ConfigurationLevel	 configurationLevel = drive.GetConfigurationLevel();

	if (configurationLevel < ConfigurationLevelAutomatic)
	{
		errorString.Append("AccurateRip verification skipped as drive offset is not configured.");

		return False;
	}

	errorState = True;
	errorString.Append("Track could not be verified as accurate:");

	errorString.Append("\n    Checksum (AccurateRip v1): %1").Replace("%1", Number(Int64(arCRCv1)).ToHexString(8));
	errorString.Append("\n    Checksum (AccurateRip v2): %1").Replace("%1", Number(Int64(arCRCv2)).ToHexString(8));

	return False;
}

Bool BoCA::VerifierAccurateRip::OnDiscInsert()
{
	const Config	*config = GetConfiguration();

	Int	 driveNumber = config->GetIntValue(ConfigureAccurateRip::ConfigID, "DiscInsert", -1);

	NonBlocking1<Int>(&ConfigureDrive).Call(driveNumber);

	return True;
}

Void BoCA::VerifierAccurateRip::ConfigureDrive(Int driveNumber)
{
	const Config* config = Config::Get();

	Drive	 drive(config, driveNumber);

	if (drive.GetConfigurationLevel() < ConfigurationLevelAutomatic) drive.ConfigureOffset(True);
}

ConfigLayer *BoCA::VerifierAccurateRip::GetConfigurationLayer()
{
	if (configLayer != NIL) return configLayer;

	AS::Registry		&boca = AS::Registry::Get();
	AS::DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

	if (info != NIL)
	{
		configLayer = new ConfigureAccurateRip();

		boca.DeleteComponent(info);
	}

	return configLayer;
}
