 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_INPUTFILTER_
#define _H_INPUTFILTER_

#include <iolib-cxx.h>
#include "../bonkenc.h"
#include "../dllinterfaces.h"

class InputFilter : public IOLibFilter
{
	private:
		Bool			 ParseID3V2Tag(ID3Tag *, bonkEncTrack *);
	protected:
		Int			 inBytes;

		UnsignedInt		 fileSize;
		bonkEncTrack		*format;
		bonkEncConfig		*currentConfig;

		Bool			 ParseID3V2Tag(unsigned char *, Int, bonkEncTrack *);
		Bool			 ParseID3V2Tag(String, bonkEncTrack *);

		String			 GetID3CategoryName(Int);

		IOLibDriver		*iolibDriver;

		InStream		*OpenFile(String);
		Int			 CloseFile(InStream *);
	public:
		Int			 error;

					 InputFilter(bonkEncConfig *, bonkEncTrack *);
					~InputFilter();

		virtual int		 ReadData(unsigned char **, int) = 0;

		virtual bonkEncTrack	*GetFileInfo(String) = 0;
		virtual Bool		 SetFileSize(UnsignedInt);

		Int			 GetInBytes();
};

#endif
