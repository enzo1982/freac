 /* fre:ac - free audio converter
  * Copyright (C) 2001-2022 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CONFIG_DSP
#define H_FREAC_CONFIG_DSP

#include <smooth.h>
#include <boca.h>

namespace freac
{
	class ConfigureDSP : public BoCA::ConfigLayer
	{
		private:
			GroupBox	*group_dsp;

			CheckBox	*check_enable;
			CheckBox	*check_playback;

			GroupBox	*group_components;

			Text		*text_available;
			ListBox		*list_available;
			Text		*text_selected;
			ListBox		*list_selected;

			Button		*btn_add;
			Button		*btn_remove;
			Button		*btn_configure;

			GroupBox	*group_single_file;

			OptionBox	*option_individual;
			OptionBox	*option_combined;

			Bool		 enableProcessing;
			Bool		 processPlayback;

			Int		 singleFileMode;

			Void		 AddComponents();

			String		 GetSelectedComponent() const;
		public:
					 ConfigureDSP();
					~ConfigureDSP();

			Int		 SaveSettings();
		slots:
			Void		 OnToggleProcessing();

			Void		 OnSelectAvailable();
			Void		 OnSelectComponent();

			Void		 OnAddComponent();
			Void		 OnRemoveComponent();

			Void		 OnConfigureComponent();
	};
};

#endif
