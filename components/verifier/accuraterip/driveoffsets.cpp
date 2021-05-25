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

#include "driveoffsets.h"
#include "config.h"

using namespace smooth::IO;
using namespace smooth::Net;

BoCA::DriveOffsets	*BoCA::DriveOffsets::instance = NIL;

BoCA::DriveOffsets::DriveOffsets()
{
}

BoCA::DriveOffsets::~DriveOffsets()
{
}

BoCA::DriveOffsets *BoCA::DriveOffsets::Get()
{
	if (instance == NIL) instance = new DriveOffsets();

	return instance;
}

Void BoCA::DriveOffsets::Free()
{
	if (instance != NIL) delete instance;

	instance = NIL;
}

Bool BoCA::DriveOffsets::LoadDriveOffsets()
{
	Config	*config	     = Config::Get();
	String	 cacheDir    = config->cacheDir;

	if (cacheDir == GUI::Application::GetApplicationDirectory()) cacheDir.Append("boca/");
	else							     cacheDir.Append("freac/");

	String	 offsetsFile = String(cacheDir).Append(ConfigureAccurateRip::ResourcesFolder).Append("/DriveOffsets.bin");

	Utilities::CreateDirectoryForFile(offsetsFile);

	/* Download offsets file if necessary.
	 */
	if (!File(offsetsFile).Exists())
	{
		String		 base = config->GetStringValue(ConfigureAccurateRip::ConfigID, ConfigureAccurateRip::ConfigPathID, ConfigureAccurateRip::ConfigPathDefault);

		if (!base.EndsWith("/")) base.Append("/");

		String		 url  = String(base).Append("DriveOffsets.bin");
		Protocols::HTTP	 protocol(url);

		protocol.DownloadToFile(offsetsFile);

		if (!protocol.GetResponseHeaderField("Status-Code").StartsWith("200")) File(offsetsFile).Delete();
	}

	if (!File(offsetsFile).Exists()) return False;

	/* Parse offsets file.
	 */
	InStream	 in(STREAM_FILE, offsetsFile);

	while (in.GetPos() < in.Size())
	{
		DriveOffset	 offset;

		/* Read drive's sample offset.
		 */
		offset.offset = (Int16) in.InputNumber(2);

		/* Read drive ID string.
		 */
		String		 id = in.InputString(32);

		offset.vendor = id.Head(id.Find("- ")).Trim();
		offset.model  = id.Tail(id.Length() - id.Find("- ") - 2).Trim();

		/* Skip rest of chunk.
		 */
		in.RelSeek(1);	// Drive ID terminator
		in.RelSeek(1);	// Number of submissions
		in.RelSeek(32);	// Unknown 32 byte string
		in.RelSeek(1);	// Unknown string terminator

		/* Add drive to list of offsets.
		 */
		driveOffsets.Add(offset);
	}

	return True;
}

const BoCA::DriveOffset &BoCA::DriveOffsets::FindDriveOffset(const Device &device)
{
	static DriveOffset	 invalidOffset;

	if (driveOffsets.Length() == 0) LoadDriveOffsets();

	foreach (const DriveOffset &offset, driveOffsets)
	{
		if (offset.vendor == device.vendor &&
		    offset.model  == device.model) return offset;
	}

	return invalidOffset;
}
