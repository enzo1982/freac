 /* fre:ac - free audio converter
  * Copyright (C) 2001-2020 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_PLAYER
#define H_FREAC_PLAYER

#include <smooth.h>
#include <boca.h>

using namespace smooth;

namespace freac
{
	class Decoder;
	class Processor;

	class Player
	{
		private:
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
								 Player();
								~Player();

			Void					 Play(const BoCA::Track &);

			Void					 Pause();
			Void					 Resume();

			Void					 SetPosition(Int);

			Void					 Stop();
		accessors:
			Bool					 IsPlaying() const		{ return playing; }
			Bool					 IsPaused() const		{ return paused; }

			const BoCA::Track			&GetPlayingTrack() const	{ return track; }
		signals:
			Signal1<Void, const BoCA::Track &>	 onPlay;
			Signal1<Void, Int>			 onProgress;
			Signal1<Void, const BoCA::Track &>	 onFinish;
	};
};

#endif
