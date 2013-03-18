 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2012 Robert Kausch <robert.kausch@bonkenc.org>
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
#include <boca.h>

#ifdef __WIN32__
#	include <shlobj.h>
#endif

using namespace smooth;

namespace BonkEnc
{
	class Progress
	{
		private:
#ifdef __WIN32__
			HWND			 hwnd;
			ITaskbarList3		*taskbar;
#endif
		protected:
			Int64			 totalSamples;
			Float			 totalSamplesDone;

			UnsignedInt64		 trackStartTicks;
			UnsignedInt64		 trackPauseTicks;

			UnsignedInt64		 totalStartTicks;
			UnsignedInt64		 totalPauseTicks;
		public:
						 Progress();
						~Progress();

			Void			 ComputeTotalSamples(const Array<BoCA::Track> &);
			Void			 FixTotalSamples(BoCA::Track &, const BoCA::Track &);

			Int64			 GetTotalSamples()			{ return totalSamples; }
			Int			 GetTrackTimePassed()			{ return S::System::System::Clock() - trackStartTicks; }
			Int			 GetTotalTimePassed()			{ return S::System::System::Clock() - totalStartTicks; }

			Void			 InitTrackProgressValues();

			Void			 PauseTrackProgress();
			Void			 ResumeTrackProgress();

			Void			 InitTotalProgressValues();

			Void			 PauseTotalProgress();
			Void			 ResumeTotalProgress();

			Void			 UpdateProgressValues(const BoCA::Track &, Int);

			Void			 FinishTrackProgressValues(const BoCA::Track &);
		signals:
			Signal2<Void, Int, Int>	 onTrackProgress;
			Signal2<Void, Int, Int>	 onTotalProgress;
	};
};

#endif
