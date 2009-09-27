 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_CONFIG_INTERFACE
#define H_BONKENC_CONFIG_INTERFACE

#include <bonkenc.h>

namespace BonkEnc
{
	class ConfigureInterface : public ConfigLayer
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
