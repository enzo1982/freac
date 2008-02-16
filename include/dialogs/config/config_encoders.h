 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CONFIG_ENCODERS_
#define _H_CONFIG_ENCODERS_

#include <bonkenc.h>

namespace BonkEnc
{
	class ConfigureEncoders : public ConfigLayer
	{
		private:
			GroupBox	*group_encoder;
			ComboBox	*combo_encoder;
			Button		*button_config;
			GroupBox	*group_outdir;
			CheckBox	*check_useInputDir;
			CheckBox	*check_allowOverwrite;
			EditBox		*edit_outdir;
			Button		*button_outdir_browse;
			GroupBox	*group_filename;
			EditBox		*edit_filename;
			ListBox		*list_filename;
			GroupBox	*group_options;
			CheckBox	*check_onTheFly;
			CheckBox	*check_keepWaves;
			CheckBox	*check_singleFile;
			GroupBox	*group_unicode;
			CheckBox	*check_unicode_files;

			Bool		 onTheFly;
			Bool		 keepWaves;
			Bool		 useInputDir;
			Bool		 allowOverwrite;
			Bool		 singleFile;

			Bool		 unicode_files;
 
			Config		*currentConfig;
		slots:
			Void		 SelectDir();
			Void		 ConfigureEncoder();

			Void		 ToggleOnTheFly();
			Void		 ToggleUseInputDir();
			Void		 ToggleEncodeToSingleFile();
		public:
					 ConfigureEncoders();
					~ConfigureEncoders();

			Int		 SaveSettings();
	};
};

#endif
