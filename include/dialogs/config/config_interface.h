 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CONFIG_INTERFACE
#define H_FREAC_CONFIG_INTERFACE

#include <smooth.h>
#include <boca.h>

namespace BonkEnc
{
	class ConfigureInterface : public BoCA::ConfigLayer
	{
		private:
			GroupBox	*group_joblist;
			Text		*text_fields;
			ListBox		*list_fields;
			Button		*button_up;
			Button		*button_down;

			Void		 FillJoblistFieldsList();
		public:
					 ConfigureInterface();
					~ConfigureInterface();

			Int		 SaveSettings();
		slots:
			Void		 OnSelectJoblistField();

			Void		 JoblistFieldMoveUp();
			Void		 JoblistFieldMoveDown();
	};
};

#endif
