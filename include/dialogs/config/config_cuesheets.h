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

#ifndef H_FREAC_CONFIG_CUE_SHEETS
#define H_FREAC_CONFIG_CUE_SHEETS

#include <smooth.h>
#include <boca.h>

namespace freac
{
	class ConfigureCueSheets : public BoCA::ConfigLayer
	{
		private:
			GroupBox	*group_cueSheets;
			CheckBox	*check_createCueSheets;

			GroupBox	*group_outdir;
			Text		*text_note;
			Text		*text_outdir;

			GroupBox	*group_embedded;
			CheckBox	*check_readEmbedded;
			CheckBox	*check_preferCueSheets;

			Bool		 createCueSheets;

			Bool		 readEmbedded;
			Bool		 preferCueSheets;
		slots:
			Void		 ToggleCreateCueSheets();
			Void		 ToggleReadEmbedded();
		public:
					 ConfigureCueSheets();
					~ConfigureCueSheets();

			Int		 SaveSettings();
	};
};

#endif
