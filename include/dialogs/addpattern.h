 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_DIALOG_PATTERN
#define H_FREAC_DIALOG_PATTERN

#include <main.h>

namespace freac
{
	class AddPatternDialog : public Dialogs::Dialog
	{
		private:
			Divider		*divbar;

			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			GroupBox	*group_pattern;
			Text		*text_directory;
			EditBox		*edit_directory;
			Button		*btn_browse;

			Text		*text_pattern;
			EditBox		*edit_pattern;

			Button		*btn_ok;
			Button		*btn_cancel;

			Config		*currentConfig;
		public:
					 AddPatternDialog();
					~AddPatternDialog();

			const Error	&ShowDialog();
		accessors:
			String		 GetDirectory();
			String		 GetPattern();
		slots:
			Void		 OK();
			Void		 Cancel();

			Void		 Browse();
	};
};

#endif
