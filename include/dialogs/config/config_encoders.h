 /* fre:ac - free audio converter
  * Copyright (C) 2001-2023 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CONFIG_ENCODERS
#define H_FREAC_CONFIG_ENCODERS

#include <smooth.h>
#include <boca.h>

namespace freac
{
	class ConfigureEncoders : public BoCA::ConfigLayer
	{
		private:
			GroupBox	*group_encoder;
			ComboBox	*combo_encoder;
			Button		*button_config;

			GroupBox	*group_options;
			CheckBox	*check_singleFile;
			CheckBox	*check_onTheFly;
			CheckBox	*check_keepWaves;
			CheckBox	*check_removeTracks;
			CheckBox	*check_addEncodedTracks;

			Bool		 singleFile;
			Bool		 onTheFly;
			Bool		 keepWaves;
			Bool		 removeTracks;
			Bool		 addEncodedTracks;

			String		 GetSelectedEncoder() const;
		slots:
			Void		 SelectEncoder();
			Void		 ConfigureEncoder();

			Void		 ToggleOnTheFly();
			Void		 ToggleRemoveTracks();
			Void		 ToggleEncodeToSingleFile();
		public:
					 ConfigureEncoders();
					~ConfigureEncoders();

			Int		 SaveSettings();
	};
};

#endif
