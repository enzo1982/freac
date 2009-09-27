 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_CONFIG_DLG
#define H_BONKENC_CONFIG_DLG

#include <bonkenc.h>
#include "configentry.h"

namespace BonkEnc
{
	class ConfigDialog : public Dialog
	{
		private:
			Array<AS::Component *>	 components;
			Array<ConfigEntry *>	 entries;
			Array<ConfigLayer *>	 layers;

			ConfigLayer		*selectedLayer;

			GUI::Window		*mainWnd;
			Titlebar		*mainWnd_titlebar;

			Button			*btn_cancel;
			Button			*btn_ok;

			Divider			*divbar;

			Text			*text_config;
			ComboBox		*combo_config;
			Button			*button_config_new;
			Button			*button_config_delete;
			Divider			*divider_top;

			ListBox			*list_layers;
			Tree			*tree_bonkenc;
			Tree			*tree_components;
			Tree			*tree_encoders;
			Tree			*tree_decoders;
			Tree			*tree_output;
			Tree			*tree_dsp;
			Tree			*tree_extension;
			Tree			*tree_other;

	 		Config			*currentConfig;
		slots:
			Void			 OK();
			Void			 Cancel();

			Void			 OnChangeSize();
			Void			 OnSelectEntry(ConfigLayer *);

			Void			 OnChangeEncoderSettings(const String &);
		public:
						 ConfigDialog();
						~ConfigDialog();

			const Error		&ShowDialog();
	};
};

#endif
