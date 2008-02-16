 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/inputfilter.h>

#include <dllinterfaces.h>
#include <utilities.h>

BonkEnc::InputFilter::InputFilter(Track *iFormat)
{
	errorState	= False;
	errorString	= "Unknown error";

	inBytes		= 0;
	fileSize	= 0;
	format		= iFormat;
	currentConfig	= Config::Get();
}

BonkEnc::InputFilter::~InputFilter()
{
}

Bool BonkEnc::InputFilter::SetFileSize(UnsignedInt newFileSize)
{
	fileSize = newFileSize;

	return true;
}
