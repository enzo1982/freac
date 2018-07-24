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

#ifndef H_DONATE_METHOD
#define H_DONATE_METHOD

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA;

namespace BoCA
{
	class PaymentMethod
	{
		private:
			String		 id;
		protected:
			Bitmap		 logo;
		public:
					 PaymentMethod(const String &);
			virtual		~PaymentMethod();

			virtual Layer	*GetLayer() = 0;
		accessors:
			const String	&GetID()	{ return id; }

			const Bitmap	&GetLogo()	{ return logo; }
	};
};

#endif
