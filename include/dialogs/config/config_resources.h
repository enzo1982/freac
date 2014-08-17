 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CONFIG_RESOURCES
#define H_FREAC_CONFIG_RESOURCES

#include <smooth.h>
#include <boca.h>

namespace BonkEnc
{
	class ConfigureResources : public BoCA::ConfigLayer
	{
		private:
			GroupBox	*group_cpu;
			CheckBox	*check_enable_openmp;
			Text		*text_threads;
			Slider		*slider_threads;
			Text		*text_threads_value;

			Bool		 enableOpenMP;
 		slots:
			Void		 ToggleOpenMP();

			Void		 ChangeOpenMPThreads();
		public:
					 ConfigureResources();
					~ConfigureResources();

			Int		 SaveSettings();
	};
};

#endif
