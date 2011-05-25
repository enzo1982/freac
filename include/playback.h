 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_PLAYBACK
#define H_BONKENC_PLAYBACK

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
		private:
			/* Singleton class, therefore private constructor/destructor
			 */
			static Playback				*instance;

								 Playback();
								~Playback();

			OutputComponent				*output;

			JobList					*joblist;

			Bool					 playing;
			Bool					 paused;

			Bool					 stop_playback;

			Int					 player_activedrive;

			Int					 PlayThread();
		public:
			Int					 player_entry;
			Int					 player_entry_id;

			Void					 Play(Int, JobList *);

			Void					 Pause();
			Void					 Resume();

			Void					 Previous();
			Void					 Next();

			Void					 Stop();

			/* Returns a new or existing instance of Playback
			 */
			static Playback				*Get();

			/* Destroys an existing instance of Playback
			 */
			static Void				 Free();
		accessors:
			Bool					 IsPlaying() const	{ return playing; }
			Bool					 IsPaused() const	{ return paused; }
		signals:
			Signal2<Void, const Track *, Int>	 onPlayTrack;
			Signal0<Void>				 onFinishTrack;
	};
};

#endif
