 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
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

// TODO: remove this line once everything is in namespace BonkEnc
using namespace BonkEnc;

class languageDlg : public Dialog
{
	private:
		Divider		*divbar;

		Window		*mainWnd;
		Titlebar	*mainWnd_titlebar;

		Text		*text_language;
		ListBox		*list_language;

		Button		*btn_ok;

		Config		*currentConfig;

		Void		 OK();
	public:
				 languageDlg();
				~languageDlg();

		Int		 ShowDialog();
};

#endif
