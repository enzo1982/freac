 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <support/notification.h>

#include <boca.h>

#include <windows.h>
#include <dbt.h>

using namespace BoCA;
using namespace BoCA::AS;

static Int GetDriveNumber(const DEV_BROADCAST_VOLUME &dbcv)
{
	/* Get drive number from broadcast structure.
	 */
	Int	 number = 0;

	for (Int drive = 0; drive < 26; drive++)
	{
		if (dbcv.dbcv_unitmask >> drive & 1) return number;

		String	 driveLetter = String(" :");

		driveLetter[0] = drive + 'A';

		if (GetDriveType(driveLetter) == DRIVE_CDROM) number++;
	}

	return -1;
}

BonkEnc::Notification *BonkEnc::Notification::instance = NIL;

BonkEnc::Notification::Notification()
{
	privateData = NIL;
}

BonkEnc::Notification::~Notification()
{
}

BonkEnc::Notification *BonkEnc::Notification::Get()
{
	if (instance == NIL)
	{
		instance = new Notification();
	}

	return instance;
}

Void BonkEnc::Notification::Free()
{
	if (instance != NIL)
	{
		delete instance;

		instance = NIL;
	}
}

Void BonkEnc::Notification::ProcessSystemMessage(Int message, Int wParam, Int lParam)
{
	if (message != WM_DEVICECHANGE) return;

	switch (wParam)
	{
		case DBT_DEVICEARRIVAL:
			if (((DEV_BROADCAST_HDR *) lParam)->dbch_devicetype != DBT_DEVTYP_VOLUME || !(((DEV_BROADCAST_VOLUME *) lParam)->dbcv_flags & DBTF_MEDIA)) break;

			/* Get drive number from message.
			 */
			{
				Int	 drive = GetDriveNumber(*(DEV_BROADCAST_VOLUME *) lParam);

				if (drive >= 0) onDiscInsert.Emit(drive);
			}

			break;
		case DBT_DEVICEREMOVECOMPLETE:
			if (((DEV_BROADCAST_HDR *) lParam)->dbch_devicetype != DBT_DEVTYP_VOLUME || !(((DEV_BROADCAST_VOLUME *) lParam)->dbcv_flags & DBTF_MEDIA)) break;

			/* Get drive number from message.
			 */
			{
				Int	 drive = GetDriveNumber(*(DEV_BROADCAST_VOLUME *) lParam);

				if (drive >= 0) onDiscRemove.Emit(drive);
			}

			break;
	}
}
