 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_DIALOG_DIRECTORY
#define H_BONKENC_DIALOG_DIRECTORY

#include <bonkenc.h>

namespace BonkEnc
{
	class AddDirectoryDialog : public Dialog
	{
		private:
			Divider		*divbar;

			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			GroupBox	*group_directory;
			Text		*text_directory;
			EditBox		*edit_directory;
			Button		*btn_browse;

			Button		*btn_ok;
			Button		*btn_cancel;
		public:
					 AddDirectoryDialog();
					~AddDirectoryDialog();

			const Error	&ShowDialog();
		accessors:
			String		 GetDirectory();
		slots:
			Void		 OK();
			Void		 Cancel();

			Void		 Browse();
	};
};

#endif
