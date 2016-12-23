 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_MAIN_THREADS
#define H_FREAC_MAIN_THREADS

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

namespace freac
{
	class LayerThreads : public Layer
	{
		private:
			Text		*text_progress;
			ListBox		*list_threads;

			Text		*text_errors;
			EditBox		*edit_errors;
			ComboBox	*combo_errors;
			Button		*button_details;
		slots:
			Void		 OnChangeSize(const Size &);
			Void		 OnChangeLanguageSettings();

			Void		 OnChangeJobs();
			Void		 OnSelectJob(ListEntry *);

			Void		 ShowDetails();
		public:
					 LayerThreads();
					~LayerThreads();
	};
};

#endif
