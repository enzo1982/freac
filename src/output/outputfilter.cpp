 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <output/outputfilter.h>

OutputFilter::OutputFilter(bonkEncConfig *config, bonkFormatInfo *iformat)
{
	error = 0;
	lastPacket = false;
	format = iformat;
	currentConfig = config;
}

OutputFilter::~OutputFilter()
{
}

void OutputFilter::PrepareLastPacket()
{
	lastPacket = true;
}
