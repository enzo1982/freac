 /* fre:ac - free audio converter
  * Copyright (C) 2001-2017 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_PROGRESS
#define H_FREAC_PROGRESS

#include <smooth.h>
#include <boca.h>

using namespace smooth;

namespace freac
{
	class Progress
	{
		private:
			Threads::Mutex		 mutex;

			Window			*window;
		protected:
			const BoCA::Config	*configuration;

			UnsignedInt64		 lastInvoked;

			Int64			 totalSamplesLeft;
			Int			 totalSamplesMultiplier;

			Float			 totalProgressDone;

			Array<BoCA::Track>	 trackList;
			Array<UnsignedInt64>	 trackStartTicks;
			Array<Int64>		 trackPositions;

			UnsignedInt64		 startTicks;
			UnsignedInt64		 pauseTicks;
		public:
						 Progress(const BoCA::Config *);
						~Progress();

			Void			 ComputeTotalSamples(const Array<BoCA::Track> &);
			Void			 FixTotalSamples(const BoCA::Track &, const BoCA::Track &);

			Int64			 GetTotalSamples() const;

			Void			 Start();
			Void			 Pause();
			Void			 Resume();

			Void			 StartTrack(const BoCA::Track &);
			Void			 UpdateTrack(const BoCA::Track &, Int64);
			Void			 FinishTrack(const BoCA::Track &, Bool);
		signals:
			Signal2<Void, Int, Int>	 onTrackProgress;
			Signal2<Void, Int, Int>	 onTotalProgress;
	};
};

#endif
