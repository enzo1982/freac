 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-flac.h>
#include <dllinterfaces.h>

FilterInFLAC::FilterInFLAC(bonkEncConfig *config, bonkEncTrack *format) : InputFilter(config, format)
{
	packageSize = 0;
}

FilterInFLAC::~FilterInFLAC()
{
}

bool FilterInFLAC::Activate()
{
	return true;
}

bool FilterInFLAC::Deactivate()
{
	return true;
}

int FilterInFLAC::ReadData(unsigned char **data, int size)
{
	return 0;
}

bonkEncTrack *FilterInFLAC::GetFileInfo(String inFile)
{
	return NIL;
}
