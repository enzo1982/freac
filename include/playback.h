 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@bonkenc.org>
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
#include <boca.h>

using namespace smooth;

namespace BonkEnc
{
	class JobList;

	class Playback
	{
		private:
			/* Singleton class, therefore private constructor/destructor
			 */
			static Playback				*instance;

								 Playback();
								~Playback();

			BoCA::AS::OutputComponent		*output;

			JobList					*joblist;

			Bool					 playing;
			Bool					 paused;

			Int					 newPosition;

			Bool					 stop_playback;

			Int					 player_activedrive;

			Int					 PlayThread();
		public:
			Int					 player_entry;
			Int					 player_entry_id;

			Void					 Play(Int, JobList *);

			Void					 Pause();
			Void					 Resume();

			Void					 SetPosition(Int);

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
			Signal1<Void, const BoCA::Track &>	 onPlay;
			Signal1<Void, Int>			 onProgress;
			Signal1<Void, const BoCA::Track &>	 onFinish;
	};
};

#endif
