 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CDDB_SUBMIT_
#define _H_CDDB_SUBMIT_

#include <smooth.h>
#include <main.h>

using namespace smooth;
using namespace smooth::GUI;

class cddbSubmitDlg : public Dialog
{
	private:
		Divider		*divbar;

		Window		*mainWnd;
		Titlebar	*mainWnd_titlebar;

		GroupBox	*group_drive;
		ComboBox	*combo_drive;

		Text		*text_artist;
		EditBox		*edit_artist;
		Text		*text_album;
		EditBox		*edit_album;
		Text		*text_year;
		EditBox		*edit_year;
		Text		*text_genre;
		EditBox		*edit_genre;
		ListBox		*list_genre;

		ListBox		*list_tracks;
		Text		*text_track;
		EditBox		*edit_track;
		Text		*text_title;
		EditBox		*edit_title;

		Text		*text_cdstatus;
		Text		*text_status;

		Button		*btn_cancel;
		Button		*btn_submit;

		bonkEncConfig	*currentConfig;

		Int		 activedrive;

		Bool		 dontUpdateInfo;

		CDDBInfo	*cddbInfo;

		String		 discid;
		String		 genre;
		Array<String>	 titles;

		Void		 Submit();
		Void		 Cancel();
		Void		 ChangeDrive();
		Void		 SelectTrack();
		Void		 UpdateTrack();
		String		 GetCDDBGenre(String);
	public:
				 cddbSubmitDlg();
				~cddbSubmitDlg();

		Int		 ShowDialog();
};

#endif
