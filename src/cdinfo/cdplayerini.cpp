 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cdinfo/cdplayerini.h>
#include <cddb/cddb.h>
#include <utilities.h>
#include <freac.h>
#include <dllinterfaces.h>

freac::CDPlayerIni::CDPlayerIni()
{
}

freac::CDPlayerIni::~CDPlayerIni()
{
}

Int freac::CDPlayerIni::ReadCDInfo()
{
	if (!freac::currentConfig->cdrip_read_cdplayerini) return Success();

	cdInfo.Clear();

	Int	 numTocEntries = ex_CR_GetNumTocEntries();
	Int	 discID = 0;

	for (Int i = 0; i < numTocEntries; i++)
	{
		int	 startSector = ex_CR_GetTocEntry(i).dwStartSector + 150;

		int	 minutes = startSector / 75 / 60;
		int	 seconds = startSector / 75 % 60;
		int	 frames	 = startSector % 75;

		discID += minutes * 0x10000 + seconds * 0x100 + frames;
	}

	if (numTocEntries < 3)
	{
		discID += ex_CR_GetTocEntry(0).dwStartSector;
		discID += ex_CR_GetTocEntry(numTocEntries).dwStartSector;
	}

	String	 discIDString = CDDB::DiscIDToString(discID);

	for (Int j = 0; j < discIDString.Length(); j++)
	{
		if (discIDString[0] == '0')
		{
			for (Int k = 0; k < 7 - j; k++) discIDString[k] = discIDString[k + 1];

			discIDString[7 - j] = 0;
		}
	}

	// Open cdplayer.ini
	InStream	*in = new InStream(STREAM_FILE, S::System::System::GetWindowsRootDirectory().Append("cdplayer.ini"), IS_READ);
	String		 idString = String("[").Append(discIDString).Append("]");
	String		 result;

	while (in->GetPos() < in->Size())
	{
		if (in->InputLine().ToLower() == idString)
		{
			while (in->GetPos() < in->Size())
			{
				String	 line = in->InputLine();

				if (line.ToLower().StartsWith("artist="))
				{
					String	 artist;

					for (Int i = 7; i < line.Length(); i++) artist[i - 7] = line[i];

					cdInfo.SetArtist(artist);
				}
				else if (line.ToLower().StartsWith("title="))
				{
					String	 title;

					for (Int i = 6; i < line.Length(); i++) title[i - 6] = line[i];

					cdInfo.SetTitle(title);
				}
				else if (line.StartsWith("0=") || line.ToInt() > 0)
				{
					String	 title;
					Int	 length = 2 + (line.ToInt() >= 10 ? 1 : 0);

					for (Int i = length; i < line.Length(); i++) title[i - length] = line[i];

					cdInfo.SetTrackTitle(line.ToInt() + 1, title);
				}
				else if (line.StartsWith("["))
				{
					break;
				}
			}

			break;
		}
	}

	delete in;

	return Success();
}

Int freac::CDPlayerIni::ClearCDInfo()
{
	cdInfo.Clear();

	return Success();
}

const freac::CDInfo &freac::CDPlayerIni::GetCDInfo()
{
	return cdInfo;
}
