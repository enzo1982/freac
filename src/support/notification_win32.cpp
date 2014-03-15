 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <support/notification.h>

#include <config.h>
#include <boca.h>

#include <windows.h>
#include <dbt.h>

using namespace BoCA;
using namespace BoCA::AS;

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
	BoCA::Config	*config = BoCA::Config::Get();

	switch (message)
	{
		case WM_DEVICECHANGE:
			if (wParam == DBT_DEVICEARRIVAL && config->GetIntValue(Config::CategoryRipperID, Config::RipperAutoReadContentsID, Config::RipperAutoReadContentsDefault))
			{
				if (((DEV_BROADCAST_HDR *) lParam)->dbch_devicetype != DBT_DEVTYP_VOLUME || !(((DEV_BROADCAST_VOLUME *) lParam)->dbcv_flags & DBTF_MEDIA)) break;

				/* Get drive letter from message.
				 */
				String	 driveLetter = String(" :");

				for (Int drive = 0; drive < 26; drive++)
				{
					if (((DEV_BROADCAST_VOLUME *) lParam)->dbcv_unitmask >> drive & 1)
					{
						driveLetter[0] = drive + 'A';

						break;
					}
				}

				if (driveLetter[0] == ' ') break;

				/* Read length of first track using MCI.
				 */
				Int	 trackLength = 0;

				MCI_OPEN_PARMSA		 openParms  = { 0 };
				MCI_SET_PARMS		 setParms   = { 0 };
				MCI_GENERIC_PARMS	 closeParms = { 0 };

				openParms.lpstrDeviceType  = (LPSTR) MCI_DEVTYPE_CD_AUDIO;
				openParms.lpstrElementName = driveLetter;

				if (mciSendCommandA(NIL, MCI_OPEN, MCI_WAIT | MCI_OPEN_SHAREABLE | MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT, (DWORD_PTR) &openParms) != 0) break;

				setParms.dwTimeFormat	   = MCI_FORMAT_MSF;

				if (mciSendCommandA(openParms.wDeviceID, MCI_SET, MCI_WAIT | MCI_SET_TIME_FORMAT, (DWORD_PTR) &setParms) == 0)
				{
					MCI_STATUS_PARMS	 statusParms = { 0 };

					statusParms.dwItem  = MCI_STATUS_LENGTH;
					statusParms.dwTrack = 1;

					if (mciSendCommandA(openParms.wDeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM | MCI_TRACK, (DWORD_PTR) &statusParms) == 0)
					{
						trackLength = MCI_MSF_MINUTE(statusParms.dwReturn) * 60 * 75 +
							      MCI_MSF_SECOND(statusParms.dwReturn) * 75      +
							      MCI_MSF_FRAME (statusParms.dwReturn);
					}
				}

				mciSendCommandA(openParms.wDeviceID, MCI_CLOSE, MCI_WAIT, (DWORD_PTR) &closeParms);

				if (trackLength == 0) break;

				/* Look for the actual drive using
				 * the length of the first track.
				 */
				Registry		&boca = Registry::Get();
				DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

				if (info != NIL)
				{
					for (Int drive = 0; drive < info->GetNumberOfDevices(); drive++)
					{
						const MCDI	&mcdi	= info->GetNthDeviceMCDI(drive);
						Int		 length = mcdi.GetNthEntryOffset(1) - mcdi.GetNthEntryOffset(0);

						if (mcdi.GetNthEntryType(0) == ENTRY_AUDIO && length == trackLength)
						{
							onDiscInsert.Emit(drive);

							break;
						}
					}

					boca.DeleteComponent(info);
				}
			}

			break;
	}
}
