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

#ifndef H_TAGEDIT_LAYER
#define H_TAGEDIT_LAYER

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA;

#include "chooser/chooser.h"
#include "editor/editor.h"

namespace BoCA
{
	class LayerTags : public Layer
	{
		private:
			Array<Chooser *>		 choosers;
			Array<Editor *>			 editors;

			TabWidget			*tab_mode;
			TabWidget			*tab_editor;
		signals:
			Signal1<Void, const Track &>	 onSelectTrack;
			Signal1<Void, const Track &>	 onSelectAlbum;
			Signal0<Void>			 onSelectNone;
		callbacks:
			Callback0<Bool>			 allowTrackChangeByArrowKey;
			Callback0<Bool>			 allowTrackRemoveByDeleteKey;
		slots:
			Void				 OnSelectTab(const Widget *);

			Void				 OnModifyTrack(const Track &);

			Void				 OnChangeSize(const Size &);
			Void				 OnChangeLanguageSettings();

			Bool				 AllowTrackChangeByArrowKey();
			Bool				 AllowTrackRemoveByDeleteKey();
		public:
							 LayerTags();
							~LayerTags();
	};
};

#endif
