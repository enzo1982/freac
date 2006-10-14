 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_DIALOG_DIRECTORY_
#define _H_DIALOG_DIRECTORY_

#include <main.h>

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

			Config		*currentConfig;
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
