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

#ifndef H_TAGEDIT_LAYER_TAG_PUBLISHER
#define H_TAGEDIT_LAYER_TAG_PUBLISHER

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA;

#include "editor.h"

namespace BoCA
{
	class LayerTagPublisher : public Editor
	{
		private:
			GroupBox			*group_publisher;

			Text				*text_publisher;
			EditBox				*edit_publisher;

			Text				*text_mediatype;
			EditBox				*edit_mediatype;
			List				*list_mediatype;

			Text				*text_catalog;
			EditBox				*edit_catalog;

			Text				*text_barcode;
			EditBox				*edit_barcode;

			Text				*text_isrc;
			EditBox				*edit_isrc;

			GroupBox			*group_web;

			Text				*text_wartist;
			EditBox				*edit_wartist;

			Text				*text_wpublisher;
			EditBox				*edit_wpublisher;

			Text				*text_wradio;
			EditBox				*edit_wradio;

			Text				*text_wsource;
			EditBox				*edit_wsource;

			Text				*text_wcopyright;
			EditBox				*edit_wcopyright;

			Text				*text_wcommercial;
			EditBox				*edit_wcommercial;

			Track				 track;
			Bool				 ignoreSelect;

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
							 LayerTagPublisher();
							~LayerTagPublisher();
	};
};

#endif
