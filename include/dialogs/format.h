 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_DIALOG_SELECT_FORMAT
#define H_FREAC_DIALOG_SELECT_FORMAT

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

namespace freac
{
	class DialogSelectFormat : public Dialogs::Dialog
	{
		private:
			Divider			*divbar;

			Window			*mainWnd;
			Titlebar		*mainWnd_titlebar;

			Text			*text_formats;
			ListBox			*list_formats;

			Button			*button_confirm;
			Button			*button_cancel;
		public:
						 DialogSelectFormat(const Array<BoCA::Format> &);
						~DialogSelectFormat();

			const Error		&ShowDialog();

			Int			 GetSelectedEntryNumber() const;
		slots:
			Void			 OnChangeSize(const Size &);

			Void			 OnConfirm();
			Void			 OnCancel();
	};
};

#endif
