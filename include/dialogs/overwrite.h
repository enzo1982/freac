 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_DIALOG_CONFIRM_OVERWRITE
#define H_FREAC_DIALOG_CONFIRM_OVERWRITE

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BonkEnc
{
	namespace ConfirmOverwrite
	{
		namespace Action
		{
			const Int	 None	   = 0;
			const Int	 Confirm   = 1;
			const Int	 Overwrite = 2;
			const Int	 Skip	   = 3;
			const Int	 SkipAll   = 4;
			const Int	 Cancel	   = 5;
		};
	};

	class DialogConfirmOverwrite : public Dialogs::Dialog
	{
		private:
			Divider			*divbar;

			Window			*mainWnd;
			Titlebar		*mainWnd_titlebar;

			Text			*text_files;

			Button			*button_select_all;
			Button			*button_select_none;
			Button			*button_select_toggle;

			ListBox			*list_files;

			Button			*button_skip;
			Button			*button_overwrite;
			Button			*button_cancel;

			Int			 userAction;
		public:
						 DialogConfirmOverwrite(const Array<BoCA::Track> &);
						~DialogConfirmOverwrite();

			const Error		&ShowDialog();

			Int			 GetUserAction();
			const Array<Int>	&GetUserActions();
		slots:
			Void			 OnChangeSize(const Size &);

			Void			 SelectAll();
			Void			 SelectNone();
			Void			 ToggleSelection();

			Void			 OnMarkEntry(ListEntry *);

			Void			 OnSkip();
			Void			 OnOverwrite();
			Void			 OnCancel();
	};
};

#endif
