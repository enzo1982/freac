 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_GENCONFIG_CDRIP_
#define _H_GENCONFIG_CDRIP_

#include <smooth.h>
#include <main.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BonkEnc
{
	class GeneralSettingsLayerCDRip : public Layer
	{
		private:
			GroupBox	*group_drive;
			ComboBox	*combo_drive;
			GroupBox	*group_ripping;
			CheckBox	*check_paranoia;
			ComboBox	*combo_paranoia_mode;
			CheckBox	*check_jitter;
			CheckBox	*check_swapchannels;
			GroupBox	*group_cdoptions;
			CheckBox	*check_locktray;
			CheckBox	*check_ntscsi;
			CheckBox	*check_autoRead;
			CheckBox	*check_autoEject;

			Bool		 cdparanoia;
			Bool		 jitter;
			Bool		 swapchannels;
			Bool		 locktray;
			Bool		 ntscsi;
			Bool		 autoRead;
			Bool		 autoEject;
 
			Config		*currentConfig;
		slots:
			Void		 SetParanoia();
		public:
					 GeneralSettingsLayerCDRip();
					~GeneralSettingsLayerCDRip();

			Int		 GetActiveDrive();
			Int		 GetCDParanoiaMode();

			Bool		 GetJitter();
			Bool		 GetSwapChannels();
			Bool		 GetLockTray();
			Bool		 GetNTSCSI();
			Bool		 GetAutoRead();
			Bool		 GetAutoEject();
	};
};

#endif
