 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_GENCONFIG_
#define _H_GENCONFIG_

class configureGeneralSettingsLayerEncoders;
class configureGeneralSettingsLayerLanguage;
class configureGeneralSettingsLayerCDRip;
class configureGeneralSettingsLayerCDDB;
class configureGeneralSettingsLayerPlugins;
class configureGeneralSettingsLayerTags;

#include <smooth.h>
#include <main.h>

using namespace smooth;
using namespace smooth::GUI;

class configureGeneralSettings : public Dialog
{
	private:
		TabWidget				*reg_register;

		configureGeneralSettingsLayerEncoders	*register_layer_encoders;
		configureGeneralSettingsLayerLanguage	*register_layer_language;
		configureGeneralSettingsLayerCDRip	*register_layer_cdrip;
		configureGeneralSettingsLayerCDDB	*register_layer_cddb;
		configureGeneralSettingsLayerPlugins	*register_layer_plugins;
		configureGeneralSettingsLayerTags	*register_layer_tags;

		Divider					*divbar;

		Window					*mainWnd;
		Titlebar				*mainWnd_titlebar;

		Button					*btn_cancel;
		Button					*btn_ok;

 		bonkEncConfig				*currentConfig;
	slots:
		Void					 OK();
		Void					 Cancel();
	public:
							 configureGeneralSettings();
							~configureGeneralSettings();

		Int					 ShowDialog();
};

#endif
