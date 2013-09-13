 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CMDMAIN
#define H_FREAC_CMDMAIN

#include <smooth.h>
#include "bonkenc.h"

using namespace smooth;
using namespace smooth::GUI;

namespace BonkEnc
{
	class BonkEncCommandline : public BonkEnc
	{
		private:
			const Array<String>	&args; 

			Bool			 ScanForParameter(const String &, String *);
			Void			 ScanForFiles(Array<String> *);
			Bool			 TracksToFiles(const String &, Array<String> *);
		public:
						 BonkEncCommandline(const Array<String> &);
						~BonkEncCommandline();

			Void			 ShowHelp(const String &);
	};
};

#endif
