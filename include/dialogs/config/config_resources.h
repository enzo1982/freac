 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CONFIG_RESOURCES
#define H_FREAC_CONFIG_RESOURCES

#include <smooth.h>
#include <boca.h>

namespace freac
{
	class ConfigureResources : public BoCA::ConfigLayer
	{
		private:
			GroupBox	*group_cpu;
			CheckBox	*check_enable_parallel;
			CheckBox	*check_enable_superfast;
			Text		*text_threads;
			Slider		*slider_threads;
			Text		*text_threads_value;

			GroupBox	*group_priority;
			Text		*text_priority;
			Slider		*slider_priority;
			Text		*text_priority_value;

			Bool		 enableParallel;
			Bool		 enableSuperFast;
		slots:
			Void		 ToggleParallel();

			Void		 ChangeConversionThreads();
			Void		 ChangePriority();
		public:
					 ConfigureResources();
					~ConfigureResources();

			Int		 SaveSettings();
	};
};

#endif
