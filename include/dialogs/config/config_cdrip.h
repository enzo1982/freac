 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CONFIG_CDRIP_
#define _H_CONFIG_CDRIP_

#include <bonkenc.h>

namespace BonkEnc
{
	class ConfigureCDRip : public ConfigLayer
	{
		private:
			GroupBox	*group_drive;
			ComboBox	*combo_drive;

			GroupBox	*group_ripping;
			CheckBox	*check_paranoia;
			ComboBox	*combo_paranoia_mode;
			CheckBox	*check_jitter;
			CheckBox	*check_swapchannels;

			GroupBox	*group_automatization;
			CheckBox	*check_autoRead;
			CheckBox	*check_autoRip;
			CheckBox	*check_autoEject;

			GroupBox	*group_cdoptions;
			CheckBox	*check_locktray;
			CheckBox	*check_ntscsi;

			GroupBox	*group_cdinfo;
			CheckBox	*check_readCDText;
			CheckBox	*check_readCDPlayerIni;

			Bool		 cdparanoia;
			Bool		 jitter;
			Bool		 swapchannels;
			Bool		 locktray;
			Bool		 ntscsi;
			Bool		 autoRead;
			Bool		 autoRip;
			Bool		 autoEject;
			Bool		 readCDText;
			Bool		 readCDPlayerIni;
 		slots:
			Void		 SetParanoia();

			Void		 ToggleAutoRead();
		public:
					 ConfigureCDRip();
					~ConfigureCDRip();

			Int		 SaveSettings();
	};
};

#endif
