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

#ifndef H_NOTIFIER_PLAYER
#define H_NOTIFIER_PLAYER

#include <smooth.h>
#include <boca.h>

using namespace smooth;

using namespace BoCA;

namespace BoCA
{
	class Player
	{
		private:
			AS::OutputComponent	*output;

			Bool			 playing;
			Bool			 stop;

			Int			 PlayThread(String);

			Void			 Loop(const Track &, IO::InStream *);
			Void			 Write(Buffer<UnsignedByte> &, Int);
		public:
						 Player();
						~Player();
		slots:
			Void			 Play(const String &);
			Void			 Stop();
	};
};

#endif
