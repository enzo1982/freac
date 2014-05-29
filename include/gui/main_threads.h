 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_MAIN_THREADS
#define H_FREAC_MAIN_THREADS

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BonkEnc
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
			Void		 ShowDetails();

			Void		 OnChangeJobs();
			Void		 OnSelectJob(ListEntry *);

			Void		 OnChangeSize(const Size &);
		public:
					 LayerThreads();
					~LayerThreads();
	};
};

#endif
