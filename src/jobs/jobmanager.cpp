 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
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
#include <utilities.h>

#include <boca.h>

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
		S::System::System::Sleep(100);
	}
}

Void BonkEnc::JobManager::ManagerThread()
{
	while (!exitThread)
	{
		if (Job::GetPlannedJobs().Length() > 0 && Job::GetRunningJobs().Length() < Config::Get()->maxActiveJobs)
		{
			const Array<Job *>	&planned = Job::GetPlannedJobs();

			foreach (Job *job, planned)
			{
				/* Check if the job is ready to run and if so start it.
				 */
				if (!job->ReadyToRun()) continue;

				job->onFinishJob.Connect(&JobManager::OnFinishJob, this);

				NonBlocking0<>(&Job::Run, job).Call();

				break;
			}
		}

		S::System::System::Sleep(100);
	}
}

Void BonkEnc::JobManager::OnFinishJob(Job *job)
{
	job->onFinishJob.Disconnect(&JobManager::OnFinishJob, this);

	if (job->GetErrors().Length() > 0)
	{
		if (BoCA::Config::Get()->GetIntValue(Config::CategorySettingsID, Config::SettingsDisplayErrorsID, Config::SettingsDisplayErrorsDefault))
		{
			const Array<String>	&errors = job->GetErrors();

			foreach (String error, errors) Utilities::ErrorMessage(error);
		}

		return;
	}

	Object::DeleteObject(job);
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
