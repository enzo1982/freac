 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_PLAYBACK
#define H_FREAC_PLAYBACK

#include <smooth.h>
#include <boca.h>

using namespace smooth;

namespace freac
{
	class Decoder;
	class Processor;

	class Playback
	{
		private:
			/* Singleton class, therefore private constructor/destructor
			 */
			static Playback				*instance;

								 Playback();
								~Playback();

			BoCA::AS::OutputComponent		*output;

			BoCA::Track				 track;

			Bool					 playing;
			Bool					 paused;

			Bool					 stop;

			Int					 newPosition;

			Int					 PlayThread();

			Void					 Loop(Decoder *, Processor *);
			Void					 Write(Buffer<UnsignedByte> &, Int);
		public:
			Void					 Play(const BoCA::Track &);

			Void					 Pause();
			Void					 Resume();

			Void					 SetPosition(Int);

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
