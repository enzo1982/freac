 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_ENCODER_
#define _H_ENCODER_

#include <smooth.h>

#include "bonkenc.h"

using namespace smooth;
using namespace BoCA;

namespace BonkEnc
{
	class JobList;
	class Progress;
};

namespace BonkEnc
{
	const Int	 ENCODER_MODE_ON_THE_FLY = 0;
	const Int	 ENCODER_MODE_DECODE	 = 1;
	const Int	 ENCODER_MODE_ENCODE	 = 2;

	class Encoder
	{
		protected:
			Bool					 encoding;
			Bool					 paused;

			Bool					 stop;
			Bool					 skip;

			Int					 encoder_activedrive;

			Bool					 overwriteAll;

			JobList					*joblist;
			Progress				*progress;

			String					 GetPlaylistFileName(const Track &);
			String					 GetRelativeFileName(const String &, const String &);

			String					 GetOutputFileName(const Track &);
			String					 GetSingleOutputFileName(const Track &);

			Int					 EncoderThread();
		public:
								 Encoder();
								~Encoder();

			Void					 Encode(JobList *, Bool = True);

			Void					 Pause();
			Void					 Resume();

			Void					 Stop();
		accessors:
			Bool					 IsEncoding() const	{ return encoding; }
			Bool					 IsPaused() const	{ return paused; }
		signals:
			Signal0<Void>				 onStartEncoding;
			Signal1<Void, Bool>			 onFinishEncoding;

			Signal2<Void, const Track &, Int>	 onEncodeTrack;
			Signal0<Void>				 onFinishTrack;

			Signal2<Void, Int, Int>			 onTrackProgress;
			Signal2<Void, Int, Int>			 onTotalProgress;
		slots:
			Void					 SkipTrack();
	};
};

#endif
