 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/jobmanager.h>
#include <jobs/job.h>
#include <config.h>

BonkEnc::JobManager	*BonkEnc::JobManager::instance = NIL;

BonkEnc::JobManager::JobManager()
{
	exitThread = False;

	managerThread = NonBlocking0<>(&JobManager::ManagerThread, this).Call();
}

BonkEnc::JobManager::~JobManager()
{
	exitThread = True;

	while (managerThread->GetStatus() == THREAD_RUNNING)
	{
		System::System::Sleep(10);
	}
}

Void BonkEnc::JobManager::ManagerThread()
{
	while (!exitThread)
	{
		if (Job::GetPlannedJobs().Length() > 0 && Job::GetRunningJobs().Length() < Config::Get()->maxActiveJobs)
		{
			Job	*job = Job::GetPlannedJobs().GetFirst();

			NonBlocking0<>(&Job::Run, job).Call();
		}

		System::System::Sleep(10);
	}
}

Error BonkEnc::JobManager::Start()
{
	if (instance != NIL) return Error();

	instance = new JobManager();

	return Success();
}

Error BonkEnc::JobManager::Quit()
{
	if (instance == NIL) return Error();

	delete instance;

	instance = NIL;

	return Success();
}
