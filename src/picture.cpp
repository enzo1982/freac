 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2007 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <picture.h>

BonkEnc::Picture::Picture()
{
	type = 0;
}

BonkEnc::Picture::Picture(const Picture &oPicture)
{
	type = oPicture.type;
	mime = oPicture.mime;
	description = oPicture.description;

	data.Resize(oPicture.data.Size());

	memcpy(data, oPicture.data, data.Size());
}

BonkEnc::Picture::~Picture()
{
}
