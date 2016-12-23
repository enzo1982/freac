 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
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

#include <libudev.h>

using namespace BoCA;
using namespace BoCA::AS;

using namespace smooth::System;

static DynamicLoader		*loader	     = NIL;

/* udev status variables.
 */
static struct udev		*udev	     = NIL;
static struct udev_monitor	*udevMonitor = NIL;
static int			 udevFd	     = 0;

/* udev function pointers.
 */
static struct udev *		(*ex_udev_new)(void)											 = NIL;
static void			(*ex_udev_unref)(struct udev *)										 = NIL;

static const char *		(*ex_udev_device_get_devnode)(struct udev_device *)							 = NIL;
static const char *		(*ex_udev_device_get_property_value)(struct udev_device *, const char *)				 = NIL;
static void			(*ex_udev_device_unref)(struct udev_device *)								 = NIL;

static struct udev_monitor *	(*ex_udev_monitor_new_from_netlink)(struct udev *, const char *)					 = NIL;
static int			(*ex_udev_monitor_filter_add_match_subsystem_devtype)(struct udev_monitor *, const char *, const char *) = NIL;
static int			(*ex_udev_monitor_enable_receiving)(struct udev_monitor *)						 = NIL;
static int			(*ex_udev_monitor_get_fd)(struct udev_monitor *)							 = NIL;
static struct udev_device *	(*ex_udev_monitor_receive_device)(struct udev_monitor *)						 = NIL;
static void			(*ex_udev_monitor_unref)(struct udev_monitor *)								 = NIL;

static Void FreeLibrary()
{
	if (loader != NIL)
	{
		delete loader;

		loader = NIL;
	}
}

static Bool LoadLibrary()
{
	if (loader == NIL)
	{
		loader = new DynamicLoader("udev");

		if (loader->GetSystemModuleHandle() == NIL) { FreeLibrary(); return False; }

		ex_udev_new					   = (struct udev *(*)(void)) loader->GetFunctionAddress("udev_new");
		ex_udev_unref					   = (void (*)(struct udev *)) loader->GetFunctionAddress("udev_unref");

		ex_udev_device_get_devnode			   = (const char *(*)(struct udev_device *)) loader->GetFunctionAddress("udev_device_get_devnode");
		ex_udev_device_get_property_value		   = (const char *(*)(struct udev_device *, const char *)) loader->GetFunctionAddress("udev_device_get_property_value");
		ex_udev_device_unref				   = (void (*)(struct udev_device *)) loader->GetFunctionAddress("udev_device_unref");

		ex_udev_monitor_new_from_netlink		   = (struct udev_monitor *(*)(struct udev *, const char *)) loader->GetFunctionAddress("udev_monitor_new_from_netlink");
		ex_udev_monitor_filter_add_match_subsystem_devtype = (int (*)(struct udev_monitor *, const char *, const char *)) loader->GetFunctionAddress("udev_monitor_filter_add_match_subsystem_devtype");
		ex_udev_monitor_enable_receiving		   = (int (*)(struct udev_monitor *)) loader->GetFunctionAddress("udev_monitor_enable_receiving");
		ex_udev_monitor_get_fd				   = (int (*)(struct udev_monitor *)) loader->GetFunctionAddress("udev_monitor_get_fd");
		ex_udev_monitor_receive_device			   = (struct udev_device *(*)(struct udev_monitor *)) loader->GetFunctionAddress("udev_monitor_receive_device");
		ex_udev_monitor_unref				   = (void (*)(struct udev_monitor *)) loader->GetFunctionAddress("udev_monitor_unref");

		if (ex_udev_new						== NIL ||
		    ex_udev_unref					== NIL ||
		    ex_udev_device_get_devnode				== NIL ||
		    ex_udev_device_get_property_value			== NIL ||
		    ex_udev_device_unref				== NIL ||
		    ex_udev_monitor_new_from_netlink			== NIL ||
		    ex_udev_monitor_filter_add_match_subsystem_devtype	== NIL ||
		    ex_udev_monitor_enable_receiving			== NIL ||
		    ex_udev_monitor_get_fd				== NIL ||
		    ex_udev_monitor_receive_device			== NIL ||
		    ex_udev_monitor_unref				== NIL) { FreeLibrary(); return False; }
	}

	return True;
}

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
	struct udev_device	*device = ex_udev_monitor_receive_device(udevMonitor);

	if (device == NIL) return;

	/* Look for the drive number using the device path.
	 */
	Registry		&boca = Registry::Get();
	DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

	if (info != NIL)
	{
		for (Int drive = 0; drive < info->GetNumberOfDevices(); drive++)
		{
			if (info->GetNthDeviceInfo(drive).path == ex_udev_device_get_devnode(device))
			{
				if (String(ex_udev_device_get_property_value(device, "ID_CDROM_MEDIA")) == "1") freac::Notification::Get()->onDiscInsert.Emit(drive);
				else										freac::Notification::Get()->onDiscRemove.Emit(drive);

				break;
			}
		}

		boca.DeleteComponent(info);
	}

	ex_udev_device_unref(device);
}

freac::Notification	*freac::Notification::instance = NIL;

freac::Notification::Notification()
{
	/* Setup udev monitor.
	 */
	udev	    = ex_udev_new();
	udevMonitor = ex_udev_monitor_new_from_netlink(udev, "udev");

	ex_udev_monitor_filter_add_match_subsystem_devtype(udevMonitor, "block", "disk");
	ex_udev_monitor_enable_receiving(udevMonitor);

	udevFd	    = ex_udev_monitor_get_fd(udevMonitor);

	/* Create a timer to fire every half second.
	 */
	privateData = new Timer();

	((Timer *) privateData)->onInterval.Connect(&CheckNotification);
	((Timer *) privateData)->Start(500);
}

freac::Notification::~Notification()
{
	/* Stop timer.
	 */
	((Timer *) privateData)->Stop();

	Object::DeleteObject((Timer *) privateData);

	/* Clean up udev monitor.
	 */
	ex_udev_monitor_unref(udevMonitor);
	ex_udev_unref(udev);
}

freac::Notification *freac::Notification::Get()
{
	if (instance == NIL)
	{
		if (!LoadLibrary()) return NIL;

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

		FreeLibrary();
	}
}

Void freac::Notification::ProcessSystemMessage(Int message, Int wParam, Int lParam)
{
}
