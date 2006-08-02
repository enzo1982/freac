 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cddb/cddbinfo.h>
#include <cddb/cddb.h>

BonkEnc::CDDBInfo::CDDBInfo()
{
	charset = "UTF-8";
}

String BonkEnc::CDDBInfo::DiscIDToString()
{
	return CDDB::DiscIDToString(discID);
}
