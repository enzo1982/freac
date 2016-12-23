 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <picture.h>

freac::Picture::Picture()
{
	type = 0;
}

freac::Picture::Picture(const Picture &oPicture)
{
	type = oPicture.type;
	mime = oPicture.mime;
	description = oPicture.description;

	data.Resize(oPicture.data.Size());

	memcpy(data, oPicture.data, data.Size());
}

freac::Picture::~Picture()
{
}
