 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_ENCODING
#define H_BONKENC_ENCODING

#include <smooth.h>

using namespace smooth;

namespace BonkEnc
{
	class Encoding
	{
		private:
			static Bool	 IsUTF8(const char *);

			static Void	 InitArray(char[256], const char *);
			static Int	 Sum(char[256], Int, Int);
		public:
			static String	 GuessEncoding(const char *);
	};
};

#endif
