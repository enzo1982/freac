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

#ifndef H_FREAC_JOB
#define H_FREAC_JOB

#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

namespace freac
{
	abstract class Job : public ListEntry
	{
		private:
			Text				*progressLabel;
			Progressbar			*progress;
			EditBox				*progressValue;

			Text				*timeLabel;
			EditBox				*timeValue;

			Hotspot				*closeHotspot;

			UnsignedInt64			 startTicks;
			Int				 previousSecondsLeft;

			static Array<Job *>		 scheduled;
			static Array<Job *>		 planned;
			static Array<Job *>		 running;

			static Array<Job *>		 all;

			static Threads::Mutex		 mutex;
		protected:
			BoCA::Config			*configuration;

			Array<String>			 errors;
			Array<String>			 warnings;

			Bool				 waiting;
			Bool				 abort;
		public:
							 Job();
			virtual				~Job();

			static String			 SecondsToString(UnsignedInt);

			virtual Int			 Paint(Int);

			Int				 Schedule();

			Int				 RunPrecheck();
			Int				 Run();

			Int				 RequestAbort();

			/* Overwrite Precheck to run checks before
			 * the job is actually scheduled.
			 */
			virtual Error			 Precheck();

			/* Overwrite ReadyToRun to check conditions
			 * before running the job.
			 */
			virtual Bool			 ReadyToRun();

			/* Implement your actual job in Perform.
			 */
			virtual Error			 Perform() = 0;
		accessors:
			Int				 SetText(const String &);

			Int				 SetProgress(Int);
			Int				 GetProgress() const;

			Bool				 IsWaiting() const	{ return waiting; }

			const Array<String>		&GetErrors() const	{ return errors; }
			const Array<String>		&GetWarnings() const	{ return warnings; }

			static const Array<Job *>	&GetScheduledJobs()	{ return scheduled; }
			static const Array<Job *>	&GetPlannedJobs()	{ return planned; }
			static const Array<Job *>	&GetRunningJobs()	{ return running; }

			static const Array<Job *>	&GetAllJobs()		{ return all; }
		slots:
			Void				 OnChangeSize(const Size &);

			Void				 OnDoubleClick();
			Void				 OnClickToClose();
		signals:
			static Signal0<Void>		 onChange;

			Signal0<Void>			 onRun;
			Signal0<Void>			 onFinish;

			static Signal1<Void, Job *>	 onPlanJob;
			static Signal1<Void, Job *>	 onRunJob;
			static Signal1<Void, Job *>	 onFinishJob;
	};
};

#endif
