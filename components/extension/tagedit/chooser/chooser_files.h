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

#ifndef H_TAGEDIT_CHOOSER_FILES
#define H_TAGEDIT_CHOOSER_FILES

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA;

#include "chooser.h"

namespace BoCA
{
	class ChooserFiles : public Chooser
	{
		private:
			Array<Track>	 tracks;
			Array<Int>	 modified;

			Array<String>	 extensions;

			Array<Tree *>	 trees;

			ListBox		*list_directories;

			Divider		*div_split;

			EditBox		*edit_directory;
			ListBox		*list_files;
			Text		*text_nofiles;

			Shortcut	*shortcut_previous;
			Shortcut	*shortcut_next;
			Shortcut	*shortcut_first;
			Shortcut	*shortcut_last;

			Button		*btn_save;
			Button		*btn_saveall;

			Int		 SaveFileTag(const Track &);

			Void		 GetSupportedFileExtensions();
		slots:
			Void		 OnDragDivider(Int);

			Void		 OnSelectDirectory(const Directory &);
			Void		 OnSelectFile(ListEntry *);

			Void		 OnShortcutPrevious();
			Void		 OnShortcutNext();
			Void		 OnShortcutFirst();
			Void		 OnShortcutLast();

			Void		 OnModifyTrack(const Track &);

			Void		 OnSave();
			Void		 OnSaveAll();

			Void		 OnChangeSize(const Size &);
			Void		 OnChangeLanguageSettings();
		public:
					 ChooserFiles();
					~ChooserFiles();

			Void		 ReselectEntry();
	};
};

#endif
