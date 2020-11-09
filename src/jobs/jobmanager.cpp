 /* fre:ac - free audio converter
  * Copyright (C) 2001-2019 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/jobmanager.h>

#include <config.h>
#include <utilities.h>

#include <support/autorelease.h>

#include <dialogs/error.h>

freac::JobManager	*freac::JobManager::instance = NIL;

freac::JobManager::JobManager()
{
	exitThread = False;

	/* Enable R/W locking for job lists.
	 */
	Job::GetScheduledJobs().EnableLocking();
	Job::GetPlannedJobs().EnableLocking();
	Job::GetRunningJobs().EnableLocking();

	Job::GetAllJobs().EnableLocking();

	finished.EnableLocking();

	/* Connect slots.
	 */
	Job::onFinishJob.Connect(&JobManager::OnFinishJob, this);

	/* Start job manager thread.
	 */
	managerThread = NonBlocking0<>(&JobManager::ManagerThread, this).Call();
}

freac::JobManager::~JobManager()
{
	exitThread = True;

	/* Wait for manager thread to exit.
	 */
	managerThread->Wait();

	/* Disconnect slots.
	 */
	Job::onFinishJob.Disconnect(&JobManager::OnFinishJob, this);
}

Void freac::JobManager::ManagerThread()
{
	BoCA::Config	*config = BoCA::Config::Get();

	while (!exitThread)
	{
		const Array<Job *>	&scheduled = Job::GetScheduledJobs();
		const Array<Job *>	&planned   = Job::GetPlannedJobs();

		if (scheduled.Length() > 0)
		{
			AutoRelease	 autoRelease;

			foreachreverse (Job *job, scheduled)
			{
				/* Call the job's precheck method.
				 */
				if (job->RunPrecheck() != Success()) Object::DeleteObject(job);
			}

			continue;
		}

		if (planned.Length() > 0)
		{
			AutoRelease	 autoRelease;

			foreach (Job *job, planned)
			{
				/* Check if the job is ready to run and if so start it.
				 */
				if (!job->ReadyToRun()) continue;

				NonBlocking0<>(&Job::Run, job).Call();

				while (planned.Get(job->GetHandle()) == job) S::System::System::Sleep(0);

				break;
			}
		}

		if (finished.Length() > 0)
		{
			AutoRelease	 autoRelease;

			Bool	 enableConsole = config->GetIntValue(Config::CategorySettingsID, Config::SettingsEnableConsoleID, Config::SettingsEnableConsoleDefault);
			Bool	 displayErrors = config->GetIntValue(Config::CategorySettingsID, Config::SettingsDisplayErrorsID, Config::SettingsDisplayErrorsDefault);

			foreachreverse (Job *job, finished)
			{
				finished.Remove(job->GetHandle());

				/* Check for errors and display error list if any.
				 */
				if (job->GetErrors().Length() > 0 && !enableConsole)
				{
					if (displayErrors)
					{
						ErrorDialog	 dialog(job->GetErrors());

						dialog.ShowDialog();
					}

					continue;
				}

				/* Check for warnings and keep job object alive if any.
				 */
				if (job->GetWarnings().Length() > 0 && !enableConsole)
				{
					continue;
				}

				Object::DeleteObject(job);
			}
		}

		S::System::System::Sleep(100);
	}
}

Void freac::JobManager::OnFinishJob(Job *job)
{
	/* Add job object to finished list.
	 */
	finished.Add(job, job->GetHandle());
}

Error freac::JobManager::Start()
{
	if (instance != NIL) return Error();

	instance = new JobManager();

	return Success();
}

Error freac::JobManager::Quit()
{
	if (instance == NIL) return Error();

	delete instance;

	instance = NIL;

	return Success();
}
