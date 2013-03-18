 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2012 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_CONFIG_DLG_ENTRY
#define H_BONKENC_CONFIG_DLG_ENTRY

#include <smooth.h>
#include <boca.h>

namespace BonkEnc
{
	class ConfigEntry : public ListEntry
	{
		private:
			BoCA::ConfigLayer			*layer;
		public:
								 ConfigEntry(const String &, BoCA::ConfigLayer *);
								~ConfigEntry();
		accessors:
			BoCA::ConfigLayer			*GetLayer()				{ return layer; }
			Void					 SetLayer(BoCA::ConfigLayer *nLayer)	{ layer = nLayer; }
		slots:
			Void					 OnSelect();
		signals:
			Signal1<Void, BoCA::ConfigLayer *>	 onChangeLayer;
	};
};

#endif
