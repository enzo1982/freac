 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
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
			Progressbar			*progress;

			static Array<Job *>		 planned;
			static Array<Job *>		 running;

			static Array<Job *>		 all;
		public:
							 Job();
			virtual				~Job();

			Int				 Run();
		accessors:
			Int				 SetProgress(Int);

			static const Array<Job *>	&GetPlannedJobs()	{ return planned; }
			static const Array<Job *>	&GetRunnningJobs()	{ return running; }

			static const Array<Job *>	&GetAllJobs()		{ return all; }
		slots:
			virtual Void			 Perform() = 0;
		signals:
			static Signal0<Void>		 onChange;

			static Signal1<Void, Job *>	 onPlanJob;
			static Signal1<Void, Job *>	 onRunJob;
			static Signal1<Void, Job *>	 onFinishJob;
	};
};

#endif
