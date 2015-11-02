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

#include <Cocoa/Cocoa.h>

#include <IOKit/IOKitLib.h>
#include <IOKit/storage/IOBlockStorageDevice.h>

#include <support/notification.h>

#include <boca.h>

using namespace BoCA;
using namespace BoCA::AS;

using namespace BonkEnc;

@interface NotificationReceiver : NSObject
{
	@private
		Array<String>	 knownDevices;
}

	- (id)	 init;

	- (void) didMount:   (NSNotification *) notification;
	- (void) didUnmount: (NSNotification *) notification;

	- (void) updateKnownDevices;
@end

@implementation NotificationReceiver
	- (id) init
	{
		[super init];

		[self updateKnownDevices];

		return self;
	}

	- (void) didMount: (NSNotification *) notification
	{
		/* 100ms delay to allow the system to update the media list
		 * (without this, the mounted medium might not be reported yet).
		 */
		S::System::System::Sleep(100);

		/* Compare known devices to new device list to find mounted device.
		 */
		Registry		&boca = Registry::Get();
		DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

		if (info != NIL)
		{
			if (info->GetNumberOfDevices() > knownDevices.Length())
			{
				for (Int drive = 0; drive < info->GetNumberOfDevices(); drive++)
				{
					const Device	&device	= info->GetNthDeviceInfo(drive);

					if (device.path != knownDevices.GetNth(drive))
					{
						Notification::Get()->onDriveChange.Emit();
						Notification::Get()->onDiscInsert.Emit(drive);

						break;
					}
				}
			}

			boca.DeleteComponent(info);
		}

		[self updateKnownDevices];
	}

	- (void) didUnmount: (NSNotification *) notification
	{
		/* 100ms delay to allow the system to update the media list
		 * (without this, the unmounted medium might still be reported).
		 */
		S::System::System::Sleep(100);

		/* Compare known devices to new device list to find unmounted device.
		 */
		Registry		&boca = Registry::Get();
		DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

		if (info != NIL)
		{
			if (info->GetNumberOfDevices() < knownDevices.Length())
			{
				for (Int drive = 0; drive <= info->GetNumberOfDevices(); drive++)
				{
					if (drive == info->GetNumberOfDevices())
					{
						Notification::Get()->onDiscRemove.Emit(drive);
						Notification::Get()->onDriveChange.Emit();

						break;
					}

					const Device	&device	= info->GetNthDeviceInfo(drive);

					if (device.path != knownDevices.GetNth(drive))
					{
						Notification::Get()->onDiscRemove.Emit(drive);
						Notification::Get()->onDriveChange.Emit();

						break;
					}
				}
			}

			boca.DeleteComponent(info);
		}

		[self updateKnownDevices];
	}

	- (void) updateKnownDevices
	{
		knownDevices.RemoveAll();

		/* Get list of devices known to
		 * the device info component.
		 */
		Registry		&boca = Registry::Get();
		DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

		if (info != NIL)
		{
			for (Int drive = 0; drive < info->GetNumberOfDevices(); drive++)
			{
				const Device	&device	= info->GetNthDeviceInfo(drive);

				knownDevices.Add(device.path);
			}

			boca.DeleteComponent(info);
		}
	}
@end

BonkEnc::Notification *BonkEnc::Notification::instance = NIL;

BonkEnc::Notification::Notification()
{
	/* Create notification receiver instance.
	 */
	privateData = [[NotificationReceiver alloc] init];

	/* Register for mount and unmount notifications.
	 */
	NSNotificationCenter	*notificationCenter = [[NSWorkspace sharedWorkspace] notificationCenter];

	[notificationCenter addObserver: (id) privateData
			       selector: @selector(didMount:)
				   name: NSWorkspaceDidMountNotification
				 object: [NSWorkspace sharedWorkspace]];

	[notificationCenter addObserver: (id) privateData
			       selector: @selector(didUnmount:)
				   name: NSWorkspaceDidUnmountNotification
				 object: [NSWorkspace sharedWorkspace]];
}

BonkEnc::Notification::~Notification()
{
	/* Unregister mount and unmount notification handlers.
	 */
	NSNotificationCenter	*notificationCenter = [[NSWorkspace sharedWorkspace] notificationCenter];

	[notificationCenter removeObserver: (id) privateData
				      name: NSWorkspaceDidMountNotification
				    object: [NSWorkspace sharedWorkspace]];

	[notificationCenter removeObserver: (id) privateData
				      name: NSWorkspaceDidUnmountNotification
				    object: [NSWorkspace sharedWorkspace]];

	/* Release notification receiver instance.
	 */
	[(NotificationReceiver *) privateData release];
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
