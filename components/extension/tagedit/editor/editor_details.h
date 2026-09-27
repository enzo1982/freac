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

			Text				*text_albumartist;
			MultiValueEditBox		*edit_albumartist;

			Text				*text_band;
			MultiValueEditBox		*edit_band;

			Text				*text_conductor;
			MultiValueEditBox		*edit_conductor;

			Text				*text_remix;
			MultiValueEditBox		*edit_remix;

			Text				*text_composer;
			MultiValueEditBox		*edit_composer;

			Text				*text_textwriter;
			MultiValueEditBox		*edit_textwriter;

			GroupBox			*group_original;

			Text				*text_oartist;
			MultiValueEditBox		*edit_oartist;

			Text				*text_oalbum;
			MultiValueEditBox		*edit_oalbum;

			Text				*text_otextwriter;
			MultiValueEditBox		*edit_otextwriter;

			Text				*text_oyear;
			EditBox				*edit_oyear;

			GroupBox			*group_musical;

			Text				*text_initialkey;
			MultiValueEditBox		*edit_initialkey;

			Text				*text_tempo;
			EditBox				*edit_tempo;
			Text				*text_tempo_bpm;

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
