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

#include <iolib/drivers/driver_posix.h>
#include <iolib/drivers/driver_unicode.h>

OutputFilter::OutputFilter(bonkEncConfig *config, bonkEncTrack *iformat)
{
	error		= 0;
	lastPacket	= false;
	format		= iformat;
	currentConfig	= config;
}

OutputFilter::~OutputFilter()
{
}

void OutputFilter::PrepareLastPacket()
{
	lastPacket = true;
}

OutStream *OutputFilter::CreateFile(String fileName)
{
	if (Setup::enableUnicode)	iolibDriver = new IOLibDriverUnicode(fileName, OS_OVERWRITE);
	else				iolibDriver = new IOLibDriverPOSIX(fileName, OS_OVERWRITE);

	return new OutStream(STREAM_DRIVER, iolibDriver);
}

S::Int OutputFilter::CloseFile(OutStream *stream)
{
	delete stream;
	delete iolibDriver;

	return Success;
}
