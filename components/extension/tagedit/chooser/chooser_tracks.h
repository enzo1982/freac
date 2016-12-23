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

#ifndef H_TAGEDIT_CHOOSER_TRACKS
#define H_TAGEDIT_CHOOSER_TRACKS

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA;

#include "chooser.h"

namespace BoCA
{
	class ChooserTracks : public Chooser
	{
		private:
			Array<Track>	 tracks;

			ListBox		*list_tracks;

			Shortcut	*shortcut_previous;
			Shortcut	*shortcut_next;
			Shortcut	*shortcut_first;
			Shortcut	*shortcut_last;
			Shortcut	*shortcut_remove;
		slots:
			Void		 OnApplicationAddTrack(const Track &);
			Void		 OnApplicationModifyTrack(const Track &);
			Void		 OnApplicationRemoveTrack(const Track &);
			Void		 OnApplicationSelectTrack(const Track &);

			Void		 OnApplicationRemoveAllTracks();

			Void		 OnSelectTrack();

			Void		 OnShortcutPrevious();
			Void		 OnShortcutNext();
			Void		 OnShortcutFirst();
			Void		 OnShortcutLast();
			Void		 OnShortcutRemove();

			Void		 OnChangeSize(const Size &);
			Void		 OnChangeLanguageSettings();
		public:
					 ChooserTracks();
					~ChooserTracks();

			Void		 ReselectEntry();
	};
};

#endif
