 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_JOB_
#define _H_JOB_

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

			Int				 startTicks;

			static Array<Job *>		 planned;
			static Array<Job *>		 running;

			static Array<Job *>		 all;
		protected:
			Array<String>			 errors;
		public:
							 Job();
			virtual				~Job();

			Int				 Schedule();
			Int				 Run();

			virtual Bool			 ReadyToRun();
			virtual Error			 Perform() = 0;
		accessors:
			Int				 SetText(const String &);

			Int				 SetProgress(Int);
			Int				 GetProgress();

			const Array<String>		&GetErrors()		{ return errors; }

			static const Array<Job *>	&GetPlannedJobs()	{ return planned; }
			static const Array<Job *>	&GetRunningJobs()	{ return running; }

			static const Array<Job *>	&GetAllJobs()		{ return all; }
		slots:
			Void				 OnChangeSize(const Size &);

			Void				 OnMouseOver();
			Void				 OnMouseOut();
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
