 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_GENCONFIG_CDRIP
#define H_FREAC_GENCONFIG_CDRIP

#include <main.h>

namespace freac
{
	class GeneralSettingsLayerCDRip : public Layer
	{
		private:
			GroupBox	*group_drive;
			ComboBox	*combo_drive;
			CheckBox	*check_speed;
			ComboBox	*combo_speed;

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

			Bool		 setspeed;
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
 
			Config		*currentConfig;
		slots:
			Void		 ToggleSetSpeed();

			Void		 SetParanoia();

			Void		 ToggleAutoRead();
		public:
					 GeneralSettingsLayerCDRip();
					~GeneralSettingsLayerCDRip();

			Int		 GetActiveDrive();
			Int		 GetCDParanoiaMode();

			Int		 GetSpeed();
			Bool		 GetJitter();
			Bool		 GetSwapChannels();
			Bool		 GetLockTray();
			Bool		 GetNTSCSI();
			Bool		 GetAutoRead();
			Bool		 GetAutoRip();
			Bool		 GetAutoEject();
			Bool		 GetReadCDText();
			Bool		 GetReadCDPlayerIni();
	};
};

#endif
