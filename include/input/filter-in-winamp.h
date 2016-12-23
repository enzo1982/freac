 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_FILTER_IN_WINAMP
#define H_FREAC_FILTER_IN_WINAMP

#include "inputfilter.h"

#include <windows.h>
#include <3rdparty/winamp/in2.h>

namespace freac
{
	class FilterInWinamp : public InputFilter
	{
		friend int	 dsp_dosamples(short int *, int, int, int, int);
		friend void	 VSASetInfo(int, int);
		friend int	 Out_Open(int, int, int, int, int);
		friend int	 Out_CanWrite();
		friend int	 Out_Write(char *, int);

		private:
			Buffer<Byte>	 samplesBuffer;
			Threads::Mutex	*samplesBufferMutex;
			Int64		 samplesDone;

			Track		*infoTrack;

			In_Module	*plugin;

			String		 GetTempFile(const String &);
		public:
					 FilterInWinamp(Config *, Track *, In_Module *);
					~FilterInWinamp();

			Bool		 Activate();
			Bool		 Deactivate();

			Int		 ReadData(Buffer<UnsignedByte> &, Int);

			Track		*GetFileInfo(const String &);
	};
};

#endif
