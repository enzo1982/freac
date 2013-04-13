 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_CONFIG_DLG
#define H_BONKENC_CONFIG_DLG

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

#include "configentry.h"

namespace BonkEnc
{
	class ConfigDialog : public Dialogs::Dialog
	{
		private:
			String				 initialConfig;

			Array<BoCA::AS::Component *>	 components;
			Array<ConfigEntry *>		 entries;
			Array<BoCA::ConfigLayer *>	 layers;
			Array<BoCA::ConfigLayer *>	 createdLayers;

			BoCA::ConfigLayer		*selectedLayer;

			Window				*mainWnd;
			Titlebar			*mainWnd_titlebar;

			Button				*btn_cancel;
			Button				*btn_ok;

			Divider				*divbar;

			Text				*text_config;
			ComboBox			*combo_config;
			EditBox				*edit_config;
			Button				*button_config_new;
			Button				*button_config_delete;
			Divider				*divider_top;

			ListBox				*list_layers;
			Tree				*tree_bonkenc;
			Tree				*tree_ripper;
			Tree				*tree_interface;

			Tree				*tree_components;
			Tree				*tree_encoders;
			Tree				*tree_decoders;
			Tree				*tree_output;
			Tree				*tree_dsp;
			Tree				*tree_extension;
			Tree				*tree_other;

			Void				 AddLayers();
			Void				 DeleteLayers();
		slots:
			Void				 OK();
			Void				 Cancel();

			Void				 OnChangeSize(const Size &);

			Void				 OnSelectConfiguration();
			Void				 OnSelectConfigurationByName();

			Void				 OnEditConfigurationName(const String &);

			Void				 OnCreateConfig();
			Void				 OnDeleteConfig();

			Void				 OnSelectEntry(BoCA::ConfigLayer *);

			Void				 OnChangeComponentSettings(const String &);
		public:
							 ConfigDialog();
							~ConfigDialog();

			const Error			&ShowDialog();
	};
};

#endif
