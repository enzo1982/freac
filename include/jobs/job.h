 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_JOB
#define H_FREAC_JOB

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BonkEnc
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

			static Array<Job *>		 planned;
			static Array<Job *>		 running;

			static Array<Job *>		 all;
		protected:
			Array<String>			 errors;
			Array<String>			 warnings;
		public:
							 Job();
			virtual				~Job();

			virtual Int			 Paint(Int);

			Int				 Schedule();
			Int				 Run();

			virtual Bool			 ReadyToRun();
			virtual Error			 Perform() = 0;
		accessors:
			Int				 SetText(const String &);

			Int				 SetProgress(Int);
			Int				 GetProgress();

			const Array<String>		&GetErrors()		{ return errors; }
			const Array<String>		&GetWarnings()		{ return warnings; }

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
