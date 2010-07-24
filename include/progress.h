 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_PROGRESS
#define H_BONKENC_PROGRESS

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

			Int			 trackStartTicks;
			Int			 trackPauseTicks;

			Int			 totalStartTicks;
			Int			 totalPauseTicks;
		public:
						 Progress();
						~Progress();

			Void			 ComputeTotalSamples(JobList *);
			Void			 FixTotalSamples(Track &, const Track &);

			Int64			 GetTotalSamples()			{ return totalSamples; }
			Int			 GetTrackTimePassed()			{ return clock() - trackStartTicks; }
			Int			 GetTotalTimePassed()			{ return clock() - totalStartTicks; }

			Void			 InitTrackProgressValues();

			Void			 PauseTrackProgress();
			Void			 ResumeTrackProgress();

			Void			 InitTotalProgressValues();

			Void			 PauseTotalProgress();
			Void			 ResumeTotalProgress();

			Void			 UpdateProgressValues(const Track &, Int);

			Void			 FinishTrackProgressValues(const Track &);
		signals:
			Signal2<Void, Int, Int>	 onTrackProgress;
			Signal2<Void, Int, Int>	 onTotalProgress;
	};
};

#endif
