 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_PLAYBACK_
#define _H_PLAYBACK_

#include <smooth.h>

#include "bonkenc.h"

using namespace smooth;
using namespace BoCA::AS;

namespace BonkEnc
{
	class JobList;
};

namespace BonkEnc
{
	class Playback
	{
		protected:
			Bool					 stop_playback;

			Int					 player_activedrive;
			OutputComponent				*output;

			Int					 player_entry;

			JobList					*joblist;

			Int					 PlayThread();
		public:
			Bool					 playing;
			Bool					 paused;

								 Playback();
								~Playback();

			Void					 Play(Int, JobList *);

			Void					 Pause();
			Void					 Resume();

			Void					 Previous();
			Void					 Next();

			Void					 Stop();
		signals:
			Signal2<Void, const Track *, Int>	 onPlayTrack;
			Signal0<Void>				 onFinishTrack;
	};
};

#endif
