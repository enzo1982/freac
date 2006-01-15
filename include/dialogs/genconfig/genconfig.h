 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_GENCONFIG_
#define _H_GENCONFIG_

namespace BonkEnc
{
	class GeneralSettingsLayerEncoders;
	class GeneralSettingsLayerLanguage;
	class GeneralSettingsLayerCDRip;
	class GeneralSettingsLayerCDDB;
	class GeneralSettingsLayerPlugins;
	class GeneralSettingsLayerTags;
};

#include <smooth.h>
#include <main.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BonkEnc
{
	class GeneralSettingsDialog : public Dialog
	{
		private:
			TabWidget			*reg_register;

			GeneralSettingsLayerEncoders	*register_layer_encoders;
			GeneralSettingsLayerLanguage	*register_layer_language;
			GeneralSettingsLayerCDRip	*register_layer_cdrip;
			GeneralSettingsLayerCDDB	*register_layer_cddb;
			GeneralSettingsLayerPlugins	*register_layer_plugins;
			GeneralSettingsLayerTags	*register_layer_tags;

			Divider				*divbar;

			Window				*mainWnd;
			Titlebar			*mainWnd_titlebar;

			Button				*btn_cancel;
			Button				*btn_ok;

	 		Config				*currentConfig;
		slots:
			Void				 OK();
			Void				 Cancel();
		public:
							 GeneralSettingsDialog();
							~GeneralSettingsDialog();

			const Error			&ShowDialog();
	};
};

#endif
