 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_VERIFIER
#define H_FREAC_VERIFIER

#include <smooth.h>
#include <boca.h>

using namespace smooth;

namespace BonkEnc
{
	class Verifier
	{
		protected:
			const BoCA::Config		*configuration;

			BoCA::AS::VerifierComponent	*verifier;
		public:
							 Verifier(const BoCA::Config *);
			virtual				~Verifier();

			Bool				 Create(const BoCA::Track &);
			Bool				 Destroy();

			Int				 Process(Buffer<UnsignedByte> &);

			Bool				 Verify();
	};
};

#endif
