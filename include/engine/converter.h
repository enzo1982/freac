 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CONVERTER
#define H_FREAC_CONVERTER

#include <smooth.h>
#include <boca.h>

using namespace smooth;

namespace BonkEnc
{
	class JobList;
	class Progress;

	const Int	 ENCODER_MODE_ON_THE_FLY = 0;
	const Int	 ENCODER_MODE_DECODE	 = 1;
	const Int	 ENCODER_MODE_ENCODE	 = 2;

	class Converter
	{
		protected:
			Bool							 encoding;
			Bool							 paused;

			Bool							 stop;
			Bool							 skip;

			Bool							 overwriteAll;

			Array<BoCA::Track>					 tracks;

			JobList							*joblist;

			Bool							 CheckSingleFileSampleFormat();

			String							 GetPlaylistFileName(const BoCA::Track &);

			String							 GetOutputFileName(const BoCA::Track &);
			String							 GetSingleOutputFileName(const BoCA::Track &);

			Int							 ConverterThread();
		public:
										 Converter();
										~Converter();

			Void							 Convert(JobList *, Bool = True);

			Void							 Pause();
			Void							 Resume();

			Void							 Stop();
		accessors:
			Bool							 IsEncoding() const	{ return encoding; }
			Bool							 IsPaused() const	{ return paused; }
		signals:
			Signal0<Void>						 onStartEncoding;
			Signal1<Void, Bool>					 onFinishEncoding;

			Signal3<Void, const BoCA::Track &, const String &, Int>	 onEncodeTrack;
			Signal0<Void>						 onFinishTrack;

			Signal2<Void, Int, Int>					 onTrackProgress;
			Signal2<Void, Int, Int>					 onTotalProgress;
		slots:
			Void							 SkipTrack();
	};
};

#endif
