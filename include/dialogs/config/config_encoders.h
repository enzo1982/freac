 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2012 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_CONFIG_ENCODERS
#define H_BONKENC_CONFIG_ENCODERS

#include <smooth.h>
#include <boca.h>

namespace BonkEnc
{
	class ConfigureEncoders : public BoCA::ConfigLayer
	{
		private:
			GroupBox			*group_encoder;
			ComboBox			*combo_encoder;
			Button				*button_config;

			GroupBox			*group_options;
			CheckBox			*check_singleFile;
			CheckBox			*check_onTheFly;
			CheckBox			*check_keepWaves;
			CheckBox			*check_removeTracks;
			CheckBox			*check_addEncodedTracks;

			GroupBox			*group_outdir;
			CheckBox			*check_useInputDir;
			CheckBox			*check_allowOverwrite;
			EditBox				*edit_outdir;
			Button				*button_outdir_browse;

			GroupBox			*group_filename;
			Text				*text_filename;
			EditBox				*edit_filename;
			List				*list_filename;
			CheckBox			*check_unicode_files;
			CheckBox			*check_replace_spaces;

			Bool				 singleFile;
			Bool				 onTheFly;
			Bool				 keepWaves;
			Bool				 removeTracks;
			Bool				 addEncodedTracks;
			Bool				 useInputDir;
			Bool				 allowOverwrite;

			Bool				 unicode_files;
 			Bool				 replace_spaces;
 		slots:
			Void				 SelectDir();
			Void				 ConfigureEncoder();

			Void				 ToggleOnTheFly();
			Void				 ToggleRemoveTracks();
			Void				 ToggleUseInputDir();
			Void				 ToggleEncodeToSingleFile();
		public:
							 ConfigureEncoders();
							~ConfigureEncoders();

			Int				 SaveSettings();
		signals:
			Signal1<Void, const String &>	 onChangeEncoderSettings;
	};
};

#endif
