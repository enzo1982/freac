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

#include <boca.h>

#include <libudev.h>

using namespace BoCA::AS;

using namespace BonkEnc;

static struct udev		*udev	     = NIL;
static struct udev_monitor	*udevMonitor = NIL;
static int			 udevFd	     = 0;

static Void CheckNotification()
{
	/* Check for available notifications.
	 */
	struct timeval	 timeval = { 0 };
	fd_set		 fdset;

	FD_ZERO(&fdset);
	FD_SET(udevFd, &fdset);
	
	if (select(udevFd + 1, &fdset, NIL, NIL, &timeval) <= 0) return;

	/* Receive device notification.
	 */
	struct udev_device	*device = udev_monitor_receive_device(udevMonitor);

	if (device == NIL) return;

	/* Look for the drive number using the device path.
	 */
	Registry		&boca = Registry::Get();
	DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

	if (info != NIL)
	{
		for (Int drive = 0; drive < info->GetNumberOfDevices(); drive++)
		{
			if (info->GetNthDeviceInfo(drive).path == udev_device_get_devnode(device))
			{
				if (String(udev_device_get_property_value(device, "ID_CDROM_MEDIA")) == "1") Notification::Get()->onDiscInsert.Emit(drive);
				else									     Notification::Get()->onDiscRemove.Emit(drive);

				break;
			}
		}

		boca.DeleteComponent(info);
	}

	udev_device_unref(device);
}

BonkEnc::Notification *BonkEnc::Notification::instance = NIL;

BonkEnc::Notification::Notification()
{
	/* Setup udev monitor.
	 */
	udev	    = udev_new();
	udevMonitor = udev_monitor_new_from_netlink(udev, "udev");

	udev_monitor_filter_add_match_subsystem_devtype(udevMonitor, "block", "disk");
	udev_monitor_enable_receiving(udevMonitor);

	udevFd	    = udev_monitor_get_fd(udevMonitor);

	/* Create a timer to fire every half second.
	 */
	privateData = new System::Timer();

	((System::Timer *) privateData)->onInterval.Connect(&CheckNotification);
	((System::Timer *) privateData)->Start(500);
}

BonkEnc::Notification::~Notification()
{
	/* Stop timer.
	 */
	((System::Timer *) privateData)->Stop();

	Object::DeleteObject((System::Timer *) privateData);

	/* Clean up udev monitor.
	 */
	udev_monitor_unref(udevMonitor);
	udev_unref(udev);
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
}
