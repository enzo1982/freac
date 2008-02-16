 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-boca.h>

BonkEnc::FilterInBoCA::FilterInBoCA(Track *format, const String &componentID) : InputFilter(format)
{
	component = (DecoderComponent *) Registry::Get().CreateComponentByID(componentID);

	packageSize = component->GetPackageSize();
}

BonkEnc::FilterInBoCA::~FilterInBoCA()
{
	Registry::Get().DeleteComponent(component);
}

Bool BonkEnc::FilterInBoCA::Activate()
{
	component->SetInputFormat(*format);
	component->SetDriver(driver);

	return component->Activate();
}

Bool BonkEnc::FilterInBoCA::Deactivate()
{
	return component->Deactivate();
}

Int BonkEnc::FilterInBoCA::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	return component->ReadData(data, size);
}

BoCA::Track *BonkEnc::FilterInBoCA::GetFileInfo(const String &inFile)
{
	Track		*track = new Track();

	component->GetStreamInfo(inFile, *track);

	return track;
}
