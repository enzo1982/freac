 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_JOB_CONVERT
#define H_FREAC_JOB_CONVERT

#include <boca.h>

#include "../job.h"

namespace BonkEnc
{
	const Int	 CONVERTER_MODE_ON_THE_FLY = 0;
	const Int	 CONVERTER_MODE_DECODE	   = 1;
	const Int	 CONVERTER_MODE_ENCODE	   = 2;

	class JobConvert : public Job
	{
		private:
			static Bool							 conversionRunning;
			static Bool							 conversionPaused;

			static Bool							 skipTrack;
			static Bool							 stopConversion;

			Array<BoCA::Track>						 tracks;

			Void								 UpdateProgress(Int, Int);

			Bool								 CheckSingleFileSampleFormat();
			BoCA::Track							 ConsolidateTrackInfo();

			String								 GetPlaylistFileName(const BoCA::Track &);

			String								 GetOutputFileName(const BoCA::Track &);
			String								 GetSingleOutputFileName(const BoCA::Track &);
		public:
			static Bool							 IsConverting()	{ return conversionRunning; }
			static Bool							 IsPaused()	{ return conversionPaused;  }

			static Void							 Pause()	{ if (conversionRunning) conversionPaused = True;  }
			static Void							 Resume()	{ if (conversionRunning) conversionPaused = False; }

			static Void							 Skip()		{ if (conversionRunning) skipTrack = True; }
			static Void							 Stop();

											 JobConvert(Array<BoCA::Track> &);
			virtual								~JobConvert();

			virtual Bool							 ReadyToRun();
		signals:
			static Signal0<Void>						 onStartEncoding;
			static Signal1<Void, Bool>					 onFinishEncoding;

			static Signal3<Void, const BoCA::Track &, const String &, Int>	 onEncodeTrack;
			static Signal0<Void>						 onFinishTrack;

			static Signal2<Void, Int, Int>					 onTrackProgress;
			static Signal2<Void, Int, Int>					 onTotalProgress;
		slots:
			virtual Error							 Perform();
	};
};

#endif
