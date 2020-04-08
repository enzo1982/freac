 /* fre:ac - free audio converter
  * Copyright (C) 2001-2020 Robert Kausch <robert.kausch@freac.org>
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

#include <config.h>

using namespace smooth::Threads;

using namespace BoCA;

Int		 freac::Locking::allocatedThreads = 0;

Array<Bool>	 freac::Locking::deviceLocked;
Array<Bool>	 freac::Locking::outputLocked;

Mutex		 freac::Locking::managementMutex;

Bool freac::Locking::AllocateThread()
{
	/* Get number of available threads.
	 */
	BoCA::Config	*config = BoCA::Config::Get();

	Bool	 enableParallel	 = config->GetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableParallelConversionsID, Config::ResourcesEnableParallelConversionsDefault);
	Int	 numberOfThreads = config->GetIntValue(Config::CategoryResourcesID, Config::ResourcesNumberOfConversionThreadsID, Config::ResourcesNumberOfConversionThreadsDefault);

	if	(!enableParallel)      numberOfThreads = 1;
	else if (numberOfThreads <= 1) numberOfThreads = CPU().GetNumCores() + (CPU().GetNumLogicalCPUs() - CPU().GetNumCores()) / 2;

	/* Allocate a thread and return.
	 */
	while (True)
	{
		if (allocatedThreads >= numberOfThreads) { S::System::System::Sleep(1); continue; }

		Lock	 lock(managementMutex);

		if (allocatedThreads < numberOfThreads)
		{
			allocatedThreads++;

			return True;
		}
	}
}

Bool freac::Locking::FreeThread()
{
	/* Free a thread and return.
	 */
	Lock	 lock(managementMutex);

	if (allocatedThreads)
	{
		allocatedThreads--;

		return True;
	}

	return False;
}

Bool freac::Locking::LockDeviceForTrack(const Track &track)
{
	/* Check if the track is on a locked device.
	 */
	if (track.fileName.StartsWith("device://"))
	{
		String	 device = track.fileName.SubString(9, track.fileName.Tail(track.fileName.Length() - 9).Find("/"));
		Lock	 lock(managementMutex);

		if (deviceLocked.Get(device.ComputeCRC32())) return False;

		deviceLocked.Add(True, device.ComputeCRC32());
	}

	/* Treat .cda files and CDDA volumes like CD tracks.
	 */
	String	 filePath = File(track.fileName).GetFilePath();

#if defined __APPLE__
	if (track.fileName.EndsWith(".aiff") && Directory(filePath).GetDirectoryPath() == "/Volumes" && File(String(filePath).Append("/.TOC.plist")).Exists())
#elif defined __HAIKU__
	if (track.fileName.EndsWith(".wav") && filePath != NIL && Directory(filePath).GetDirectoryPath() == NIL)
#else
	if (track.fileName.EndsWith(".cda"))
#endif
	{
		Lock	 lock(managementMutex);

		if (deviceLocked.Get(filePath.ComputeCRC32())) return False;

		deviceLocked.Add(True, filePath.ComputeCRC32());
	}

	return True;
}

Bool freac::Locking::UnlockDeviceForTrack(const Track &track)
{
	/* Unlock track device if necessary.
	 */
	if (track.fileName.StartsWith("device://"))
	{
		String	 device = track.fileName.SubString(9, track.fileName.Tail(track.fileName.Length() - 9).Find("/"));
		Lock	 lock(managementMutex);

		deviceLocked.Remove(device.ComputeCRC32());
	}

	/* Treat .cda files and CDDA volumes like CD tracks.
	 */
	String	 filePath = File(track.fileName).GetFilePath();

#if defined __APPLE__
	if (track.fileName.EndsWith(".aiff") && Directory(filePath).GetDirectoryPath() == "/Volumes" && File(String(filePath).Append("/.TOC.plist")).Exists())
#elif defined __HAIKU__
	if (track.fileName.EndsWith(".wav") && filePath != NIL && Directory(filePath).GetDirectoryPath() == NIL)
#else
	if (track.fileName.EndsWith(".cda"))
#endif
	{
		Lock	 lock(managementMutex);

		deviceLocked.Remove(filePath.ComputeCRC32());
	}

	return True;
}

Bool freac::Locking::LockOutputForTrack(const Track &track)
{
	/* Check if the track output file is currently locked.
	 */
	if (track.outputFile != NIL)
	{
		Lock	 lock(managementMutex);

		if (outputLocked.Get(track.outputFile.ComputeCRC32())) return False;

		outputLocked.Add(True, track.outputFile.ComputeCRC32());
	}

	return True;
}

Bool freac::Locking::UnlockOutputForTrack(const Track &track)
{
	/* Unlock track output file if necessary.
	 */
	if (track.outputFile != NIL)
	{
		Lock	 lock(managementMutex);

		outputLocked.Remove(track.outputFile.ComputeCRC32());
	}

	return True;
}
