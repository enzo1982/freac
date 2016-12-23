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

#ifndef H_TAGEDIT_LAYER_TAG_DETAILS
#define H_TAGEDIT_LAYER_TAG_DETAILS

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA;

#include "editor.h"

namespace BoCA
{
	class LayerTagDetails : public Editor
	{
		private:
			GroupBox			*group_details;

			Text				*text_band;
			EditBox				*edit_band;

			Text				*text_conductor;
			EditBox				*edit_conductor;

			Text				*text_remix;
			EditBox				*edit_remix;

			Text				*text_composer;
			EditBox				*edit_composer;

			Text				*text_textwriter;
			EditBox				*edit_textwriter;

			GroupBox			*group_publisher;

			Text				*text_publisher;
			EditBox				*edit_publisher;

			Text				*text_isrc;
			EditBox				*edit_isrc;

			GroupBox			*group_tempo;

			Text				*text_bpm;
			EditBox				*edit_bpm;

			Track				 track;

			EditBox				*GetActiveEditBox();
		slots:
			Void				 OnSelectTrack(const Track &);
			Void				 OnSelectAlbum(const Track &);
			Void				 OnSelectNone();

			Void				 OnModifyTrack();

			Void				 OnChangeSize(const Size &);
			Void				 OnChangeLanguageSettings();

			Bool				 AllowTrackRemoveByDeleteKey();
		public:
							 LayerTagDetails();
							~LayerTagDetails();
	};
};

#endif
