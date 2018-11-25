 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_NOTIFIER_CONFIG
#define H_NOTIFIER_CONFIG

#include <smooth.h>
#include <boca.h>

#include "player.h"

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA;

namespace BoCA
{
	class ConfigureNotifier : public ConfigLayer
	{
		private:
			Player			 player;

			GroupBox		*group_notify;
			CheckBox		*check_notify;

			CheckBox		*check_sound;
			EditBox			*edit_soundfile;
			List			*list_soundfile;

			Button			*btn_browse;

			Button			*btn_preview;
			Button			*btn_stop;

			CheckBox		*check_message;
			EditBox			*edit_message;

			CheckBox		*check_time;
			EditBox			*edit_amount;
			List			*list_amount;
			ComboBox		*combo_units;

			Bool			 enableNotifications;

			Bool			 playSound;
			Bool			 displayMessage;

			Bool			 minimumDuration;
		public:
			static const String	 ConfigID;

			static const String	 ResourcesFolder;
			static const String	 DefaultMessage;

						 ConfigureNotifier();
						~ConfigureNotifier();

			Int			 SaveSettings();
		slots:
			Void			 OnToggleNotifications();

			Void			 OnBrowse();

			Void			 OnPreview();
			Void			 OnStop();
	};
};

#endif
