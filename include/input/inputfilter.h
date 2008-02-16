 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_INPUTFILTER_
#define _H_INPUTFILTER_

#include <smooth/io/filter.h>
#include <smooth/io/driver.h>
#include "../bonkenc.h"

using namespace smooth::IO;

namespace BonkEnc
{
	class InputFilter : public Filter
	{
		protected:
			Int		 inBytes;

			Bool		 errorState;
			String		 errorString;

			UnsignedInt	 fileSize;
			Track		*format;
			Config		*currentConfig;
		public:
					 InputFilter(Track *);
			virtual		~InputFilter();

			virtual Int	 ReadData(Buffer<UnsignedByte> &, Int) = 0;

			virtual Track	*GetFileInfo(const String &) = 0;
			virtual Bool	 SetFileSize(UnsignedInt);

			Int		 GetInBytes()		{ return inBytes; }

			Bool		 GetErrorState()	{ return errorState; }
			const String	&GetErrorString()	{ return errorString; }
	};
};

#endif
