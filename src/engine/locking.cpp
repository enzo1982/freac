 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <engine/locking.h>

using namespace smooth::Threads;

using namespace BoCA;

Array<Bool>	 freac::Locking::deviceLocked;
Array<Bool>	 freac::Locking::outputLocked;

Mutex		 freac::Locking::managementMutex;

Bool freac::Locking::LockDeviceForTrack(const Track &track)
{
	/* Check if the track is on a locked device.
	 */
	if (track.origFilename.StartsWith("device://"))
	{
		String	 device = track.origFilename.SubString(9, track.origFilename.Tail(track.origFilename.Length() - 9).Find("/"));
		Lock	 lock(managementMutex);

		if (deviceLocked.Get(device.ComputeCRC32())) return False;

		deviceLocked.Add(True, device.ComputeCRC32());
	}

#if defined __APPLE__ || defined __HAIKU__
	/* On macOS and Haiku, treat CDDA volumes like CD tracks.
	 */
	String	 filePath = File(track.origFilename).GetFilePath();

#if defined __APPLE__
	if (track.origFilename.EndsWith(".aiff") && Directory(filePath).GetDirectoryPath() == "/Volumes" && File(String(filePath).Append("/.TOC.plist")).Exists())
#else
	if (track.origFilename.EndsWith(".wav") && filePath != NIL && Directory(filePath).GetDirectoryPath() == NIL)
#endif
	{
		Lock	 lock(managementMutex);

		if (deviceLocked.Get(filePath.ComputeCRC32())) return False;

		deviceLocked.Add(True, filePath.ComputeCRC32());
	}
#endif

	return True;
}

Bool freac::Locking::UnlockDeviceForTrack(const Track &track)
{
	/* Unlock track device if necessary.
	 */
	if (track.origFilename.StartsWith("device://"))
	{
		String	 device = track.origFilename.SubString(9, track.origFilename.Tail(track.origFilename.Length() - 9).Find("/"));
		Lock	 lock(managementMutex);

		deviceLocked.Remove(device.ComputeCRC32());
	}

#if defined __APPLE__ || defined __HAIKU__
	/* On macOS and Haiku, treat CDDA volumes like CD tracks.
	 */
	String	 filePath = File(track.origFilename).GetFilePath();

#if defined __APPLE__
	if (track.origFilename.EndsWith(".aiff") && Directory(filePath).GetDirectoryPath() == "/Volumes" && File(String(filePath).Append("/.TOC.plist")).Exists())
#else
	if (track.origFilename.EndsWith(".wav") && filePath != NIL && Directory(filePath).GetDirectoryPath() == NIL)
#endif
	{
		Lock	 lock(managementMutex);

		deviceLocked.Remove(filePath.ComputeCRC32());
	}
#endif

	return True;
}

Bool freac::Locking::LockOutputForTrack(const Track &track)
{
	/* Check if the track output file is currently locked.
	 */
	if (track.outfile != NIL)
	{
		Lock	 lock(managementMutex);

		if (outputLocked.Get(track.outfile.ComputeCRC32())) return False;

		outputLocked.Add(True, track.outfile.ComputeCRC32());
	}

	return True;
}

Bool freac::Locking::UnlockOutputForTrack(const Track &track)
{
	/* Unlock track output file if necessary.
	 */
	if (track.outfile != NIL)
	{
		Lock	 lock(managementMutex);

		outputLocked.Remove(track.outfile.ComputeCRC32());
	}

	return True;
}
