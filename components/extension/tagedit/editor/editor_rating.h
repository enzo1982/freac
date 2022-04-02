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

#ifndef H_TAGEDIT_LAYER_TAG_RATING
#define H_TAGEDIT_LAYER_TAG_RATING

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA;

#include "editor.h"

namespace BoCA
{
	class LayerTagRating : public Editor
	{
		private:
			GroupBox	*group_rating;

			Text		*text_rating;
			Slider		*slider_rating;
			Text		*text_rating_value;
			ComboBox	*combo_rating;

			Int		 rating;

			Track		 track;
			Bool		 ignoreSelect;
		slots:
			Void		 OnSelectTrack(const Track &);
			Void		 OnSelectAlbum(const Track &);
			Void		 OnSelectNone();

			Void		 OnChangeRating();
			Void		 OnSelectRating();

			Void		 OnModifyTrack();

			Void		 OnChangeSize(const Size &);
			Void		 OnChangeLanguageSettings();
		public:
					 LayerTagRating();
					~LayerTagRating();
	};
};

#endif
