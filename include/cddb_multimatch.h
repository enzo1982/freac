 /* BonkEnc version 0.8
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CDDB_MULTIMATCH_
#define _H_CDDB_MULTIMATCH_

#include <smoothx.h>
#include <main.h>

class cddbMultiMatchDlg : public SMOOTHApplication
{
	private:
		SMOOTHGroupBox		*group_match;
		SMOOTHText		*text_match;
		SMOOTHComboBox		*combo_match;

		SMOOTHDivisionbar	*divbar;

		SMOOTHWindow		*mainWnd;
		SMOOTHTitlebar		*mainWnd_titlebar;
		SMOOTHLayer		*mainWnd_layer;

		SMOOTHButton		*btn_cancel;
		SMOOTHButton		*btn_ok;

		SMOOTHVoid		 OK();
		SMOOTHVoid		 Cancel();
	public:
					 cddbMultiMatchDlg(bonkEncConfig *, SMOOTHBool);
					~cddbMultiMatchDlg();

		SMOOTHInt		 ShowDialog();

		SMOOTHInt		 AddEntry(SMOOTHString, SMOOTHString);
};

#endif
