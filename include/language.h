 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_LANGUAGE_
#define _H_LANGUAGE_

#include <smooth.h>
#include <main.h>

using namespace smooth;
using namespace smooth::GUI;

class languageDlg : public Application
{
	private:
		Divider		*divbar;

		Window		*mainWnd;
		Titlebar	*mainWnd_titlebar;

		Text		*text_language;
		ListBox		*list_language;

		Button		*btn_ok;

		bonkEncConfig	*currentConfig;

		Void		 OK();
	public:
				 languageDlg();
				~languageDlg();

		Int		 ShowDialog();
};

#endif
