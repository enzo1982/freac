 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
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
#include <cddbinfo.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BonkEnc
{
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

			CheckBox	*check_updateJoblist;

			Button		*btn_cancel;
			Button		*btn_submit;

			Config		*currentConfig;

			Int		 activedrive;

			Bool		 dontUpdateInfo;
			Bool		 updateJoblist;

			CDDBInfo	*cddbInfo;
			Bool		 ownCddbInfo;

			String		 discid;
			String		 genre;
			Array<String>	 titles;

			Array<String>	 cdText;

			Int		 ReadCDText();
			Int		 FreeCDText();

			Void		 Submit();
			Void		 Cancel();
			Void		 ChangeDrive();
			Void		 SelectTrack();
			Void		 UpdateTrack();
			Void		 FinishTrack();
			String		 GetCDDBGenre(const String &);
		public:
					 cddbSubmitDlg();
					~cddbSubmitDlg();

			const Error	&ShowDialog();
	};
};

#endif
