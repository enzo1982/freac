 /* fre:ac - free audio converter
  * Copyright (C) 2001-2017 Robert Kausch <robert.kausch@freac.org>
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

freac::Notification	*freac::Notification::instance = NIL;

freac::Notification::Notification()
{
	privateData = NIL;
}

freac::Notification::~Notification()
{
}

freac::Notification *freac::Notification::Get()
{
	if (instance == NIL)
	{
		instance = new Notification();
	}

	return instance;
}

Void freac::Notification::Free()
{
	if (instance != NIL)
	{
		delete instance;

		instance = NIL;
	}
}

Void freac::Notification::ProcessSystemMessage(Int message, Int wParam, Int lParam)
{
	if (message != WM_DEVICECHANGE) return;

	static UnsignedInt64	 lastTime  = 0;
	static Int		 lastDrive = -1;

	switch (wParam)
	{
		case DBT_DEVICEARRIVAL:
			if (((DEV_BROADCAST_HDR *) lParam)->dbch_devicetype != DBT_DEVTYP_VOLUME || !(((DEV_BROADCAST_VOLUME *) lParam)->dbcv_flags & DBTF_MEDIA)) break;

			/* Get drive number from message.
			 */
			{
				Int	 drive = GetDriveNumber(*(DEV_BROADCAST_VOLUME *) lParam);

				if (drive >= 0)
				{
					/* Guard against double sent notifications.
					 */
					if (drive == lastDrive && S::System::System::Clock() - lastTime <= 15000) break;

					/* Fire disc insert event.
					 */
					onDiscInsert.Emit(drive);

					/* Save message time and drive.
					 */
					lastTime  = S::System::System::Clock();
					lastDrive = drive;
				}
			}

			break;
		case DBT_DEVICEREMOVECOMPLETE:
			if (((DEV_BROADCAST_HDR *) lParam)->dbch_devicetype != DBT_DEVTYP_VOLUME || !(((DEV_BROADCAST_VOLUME *) lParam)->dbcv_flags & DBTF_MEDIA)) break;

			/* Get drive number from message.
			 */
			{
				Int	 drive = GetDriveNumber(*(DEV_BROADCAST_VOLUME *) lParam);

				if (drive >= 0)
				{
					/* Fire disc removal event.
					 */
					onDiscRemove.Emit(drive);

					/* Reset message time.
					 */
					if (drive == lastDrive) lastTime = 0;
				}
			}

			break;
	}
}
