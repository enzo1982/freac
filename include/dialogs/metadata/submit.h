 /* fre:ac - free audio converter
  * Copyright (C) 2001-2026 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_METADATA_SUBMIT
#define H_FREAC_METADATA_SUBMIT

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

#include <boca.h>

namespace freac
{
	class MetadataSubmitDlg : public Dialogs::Dialog
	{
		private:
			Divider		*divbar;

			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			GroupBox	*group_drive;
			ComboBox	*combo_drive;

			GroupBox	*group_provider;
			ComboBox	*combo_provider;
			Text		*text_provider_note;

			Text		*text_artist;
			EditBox		*edit_artist;
			List		*list_artist;
			Text		*text_album;
			EditBox		*edit_album;
			Text		*text_year;
			EditBox		*edit_year;
			Text		*text_genre;
			EditBox		*edit_genre;
			List		*list_genre;
			Text		*text_disccomment;
			MultiEdit	*edit_disccomment;

			ListBox		*list_tracks;
			Text		*text_track;
			EditBox		*edit_track;
			Text		*text_trackartist;
			EditBox		*edit_trackartist;
			Text		*text_title;
			EditBox		*edit_title;
			Text		*text_comment;
			MultiEdit	*edit_comment;

			Shortcut	*shortcut_previous;
			Shortcut	*shortcut_next;
			Shortcut	*shortcut_first;
			Shortcut	*shortcut_last;

			Text		*text_cdstatus;
			Text		*text_status;

			CheckBox	*check_updateJoblist;

			Button		*btn_cancel;
			Button		*btn_submit;

			Int		 activedrive;

			Bool		 dontUpdateInfo;
			Bool		 edited;
			Bool		 finishedArtist;
			Bool		 updateJoblist;

			Array<String>	 artists;
			Array<String>	 titles;
			Array<String>	 comments;
			Array<Bool>	 data;

			Array<String>	 providerIDs;
			Array<Int>	 providerSubmitData;
			Array<Int>	 providerSubmitMethods;

			BoCA::Info		 disc;
			BoCA::DiscMetadata	 prefill;
			String			 prefillProvider;

			Void		 UpdateTrackList();
			Widget		*GetActiveEditBox();

			Bool		 IsVariousArtists() const;

			Bool		 IsDataValid() const;
			static Bool	 IsStringValid(const String &);

			Bool		 IsRecordEditable() const;
			Bool		 IsSubmitByUser() const;

			Void		 UpdateFieldState();

			BoCA::DiscMetadata	 BuildRecord() const;
		slots:
			Void		 OnChangeSize(const Size &);

			Void		 Submit();
			Void		 Cancel();

			Void		 ReadDisc();
			Void		 SelectProvider();
			Void		 OnEdit();
			Void		 SelectTrack();
			Void		 UpdateTrack();
			Void		 FinishTrack();
			Void		 FinishArtist();
			Void		 UpdateComment();

			Void		 SetArtist();

			Void		 OnShortcutPrevious();
			Void		 OnShortcutNext();
			Void		 OnShortcutFirst();
			Void		 OnShortcutLast();
		public:
					 MetadataSubmitDlg();
					~MetadataSubmitDlg();

			const Error	&ShowDialog();
	};
};

#endif
