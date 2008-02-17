 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CONFIG_DLG_ENTRY_
#define _H_CONFIG_DLG_ENTRY_

#include <bonkenc.h>

namespace BonkEnc
{
	class ConfigEntry : public ListEntry
	{
		private:
			ConfigLayer			*layer;
		public:
							 ConfigEntry(const String &, ConfigLayer *);
							~ConfigEntry();
		slots:
			Void				 OnSelect();
		signals:
			Signal1<Void, ConfigLayer *>	 onChangeLayer;
	};
};

#endif
