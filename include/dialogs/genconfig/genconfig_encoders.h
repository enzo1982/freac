 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_GENCONFIG_ENCODERS_
#define _H_GENCONFIG_ENCODERS_

#include <smooth.h>
#include <main.h>

using namespace smooth;
using namespace smooth::GUI;

class configureGeneralSettingsLayerEncoders : public Layer
{
	private:
		GroupBox	*group_encoder;
		ComboBox	*combo_encoder;
		Button		*button_config;
		GroupBox	*group_outdir;
		EditBox		*edit_outdir;
		Button		*button_outdir_browse;
		GroupBox	*group_filename;
		EditBox		*edit_filename;
		ListBox		*list_filename;
		GroupBox	*group_options;
		CheckBox	*check_onTheFly;
		CheckBox	*check_keepWaves;
		GroupBox	*group_unicode;
		CheckBox	*check_unicode_files;

		Bool		 onTheFly;
		Bool		 keepWaves;

		Bool		 unicode_files;
 
		bonkEncConfig	*currentConfig;
	slots:
		Void		 SelectDir();
		Void		 ConfigureEncoder();
		Void		 ToggleOnTheFly();
	public:
				 configureGeneralSettingsLayerEncoders();
				~configureGeneralSettingsLayerEncoders();

		Int		 GetSelectedEncoder();

		Bool		 GetOnTheFly();
		Bool		 GetKeepWaveFiles();
		Bool		 GetUnicodeFilenames();

		String		 GetOutputDirectory();
		String		 GetFilenamePattern();
};

#endif
