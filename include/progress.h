 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_PROGRESS_
#define _H_PROGRESS_

#include <smooth.h>
#include <time.h>

#include "bonkenc.h"

using namespace smooth;

namespace BonkEnc
{
	class JobList;
};

namespace BonkEnc
{
	class Progress
	{
		protected:
			Int64			 totalSamples;
			Float			 totalSamplesDone;

			Int			 startTicks;
		public:
						 Progress();
						~Progress();

			Void			 ComputeTotalSamples(JobList *);
			Void			 FixTotalSamples(Track *, Track *);

			Int64			 GetTotalSamples()			{ return totalSamples; }
			Int			 GetTimePassed()			{ return clock() - startTicks; }

			Void			 InitProgressValues();
			Void			 UpdateProgressValues(Track *, Int);
			Void			 FinishProgressValues(Track *);
		signals:
			Signal2<Void, Int, Int>	 onTrackProgress;
			Signal2<Void, Int, Int>	 onTotalProgress;
	};
};

#endif
