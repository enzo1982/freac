 /* fre:ac - free audio converter
  * Copyright (C) 2001-2021 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_TAGEDIT_LAYER_TAG_LYRICS
#define H_TAGEDIT_LAYER_TAG_LYRICS

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA;

#include "editor.h"

namespace BoCA
{
	class LayerTagLyrics : public Editor
	{
		private:
			GroupBox	*group_lyrics;

			Text		*text_lyrics;
			MultiEdit	*edit_lyrics;

			Track		 track;
			Bool		 ignoreSelect;
		slots:
			Void		 OnSelectTrack(const Track &);
			Void		 OnSelectAlbum(const Track &);
			Void		 OnSelectNone();

			Void		 OnModifyTrack();

			Void		 OnChangeSize(const Size &);
			Void		 OnChangeLanguageSettings();

			Bool		 AllowTrackChangeByArrowKey();
			Bool		 AllowTrackRemoveByDeleteKey();
		public:
					 LayerTagLyrics();
					~LayerTagLyrics();
	};
};

#endif
