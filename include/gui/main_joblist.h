 /* fre:ac - free audio converter
  * Copyright (C) 2001-2020 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_MAIN_JOBLIST
#define H_FREAC_MAIN_JOBLIST

#include <smooth.h>
#include <boca.h>

#include "../engine/converter.h"

using namespace smooth;
using namespace smooth::GUI;

namespace freac
{
	class JobList;
	class LayerPlayer;

	const Int	 CHARSET_ISO_8859_1	= 0;
	const Int	 CHARSET_ISO_8859_2	= 1;
	const Int	 CHARSET_ISO_8859_5	= 2;
	const Int	 CHARSET_ISO_8859_7	= 3;
	const Int	 CHARSET_CP1251		= 4;
	const Int	 CHARSET_SHIFT_JIS	= 5;
	const Int	 CHARSET_GBK		= 6;
	const Int	 CHARSET_BIG_5		= 7;
	const Int	 CHARSET_OTHER		= 255;

	class LayerJoblist : public Layer
	{
		private:
			CheckBox		*check_single;
			CheckBox		*check_playlist;
			CheckBox		*check_cuesheet;

			Divider			*info_divider;
			Layer			*info_background;
			CheckBox		*info_checkbox;
			Divider			*info_bottom;
			ActiveArea		*info_area_cover;
			Image			*info_image_cover;
			Text			*info_text_artist;
			EditBox			*info_edit_artist;
			Text			*info_text_title;
			EditBox			*info_edit_title;
			Text			*info_text_album;
			EditBox			*info_edit_album;
			Text			*info_text_track;
			EditBox			*info_edit_track;
			Text			*info_text_year;
			EditBox			*info_edit_year;
			Text			*info_text_genre;
			EditBox			*info_edit_genre;
			List			*info_list_genre;

			Text			*txt_filename;
			EditBox			*edb_filename;

			Button			*btn_skip;

			Text			*txt_filters;
			EditBox			*edb_filters;

			Button			*btn_configure_dsp;

			Text			*txt_format;
			EditBox			*edb_format;

			Text			*txt_encoder;
			ComboBox		*combo_encoder;

			Button			*btn_configure_encoder;

			Text			*txt_progress;
			Progressbar		*progress;
			Progressbar		*progress_total;
			EditBox			*edb_trackPercent;
			Text			*txt_splitPercent;
			EditBox			*edb_totalPercent;

			Text			*txt_time;
			EditBox			*edb_trackTime;
			Text			*txt_splitTime;
			EditBox			*edb_totalTime;

			Text			*txt_outdir;
			EditBox			*edb_outdir;
			List			*list_outdir;

			Button			*btn_open;
			Button			*btn_browse;

			MicroMenu		*menu_edit_artist;
			MicroMenu		*menu_edit_title;
			MicroMenu		*menu_edit_album;
			MicroMenu		*menu_edit_year;
			MicroMenu		*menu_edit_genre;

			PopupMenu		*menu_charsets;
			PopupMenu		*menu_charsets_all;
			PopupMenu		*menu_case;
			PopupMenu		*menu_case_all;

			Hotspot			*htsp_edit_artist;
			Hotspot			*htsp_edit_title;
			Hotspot			*htsp_edit_album;
			Hotspot			*htsp_edit_year;
			Hotspot			*htsp_edit_genre;

			PopupMenu		*menu_trackmenu;

			JobList			*joblist;

			Shortcut		*shortcut_previous;
			Shortcut		*shortcut_next;
			Shortcut		*shortcut_first;
			Shortcut		*shortcut_last;
			Shortcut		*shortcut_move_up;
			Shortcut		*shortcut_move_down;
			Shortcut		*shortcut_move_top;
			Shortcut		*shortcut_move_bottom;
			Shortcut		*shortcut_toggle_mark;
			Shortcut		*shortcut_remove;

			LayerPlayer		*player;

			Surface			*drawSurface;

			BoCA::Picture		 currentCover;

			Bool			 dontUpdateInfo;

			Int			 clicked_charset;
			Int			 clicked_case;

			Int			 activePopup;

			Int			 previousTrackSeconds;
			Int			 previousTotalSeconds;

			Void			 FillMenus();

			EditBox			*GetActiveEditBox();
			Void			 FocusEditBox(EditBox *);

			Void			 ShowCoverArea();
			Void			 HideCoverArea();

			static Bool		 IsWordBreakingCharacter(Int);

			static String		 AdjustCaseFirstCapital(const String &);
			static String		 AdjustCaseWordsFirstCapital(const String &);
			static String		 AdjustCaseLongWordsFirstCapital(const String &);
		public:
						 LayerJoblist();
						~LayerJoblist();

			Void			 UpdateEncoderText();
			Void			 UpdateOutputDir();
		accessors:
			JobList			*GetJoblist()			{ return joblist; }
		signals:
			Signal0<Void>		 onRequestSkipTrack;
		slots:
			Void			 OnChangeSize(const Size &);

			Void			 OnChangeConfigurationSettings();
			Void			 OnChangeLanguageSettings();

			Void			 OnJoblistSelectTrack(const BoCA::Track &);
			Void			 OnJoblistModifyTrack(const BoCA::Track &);
			Void			 OnJoblistRemoveTrack(const BoCA::Track &);

			Void			 OnJoblistRemoveAllTracks();

			Void			 OnShortcutPrevious();
			Void			 OnShortcutNext();
			Void			 OnShortcutFirst();
			Void			 OnShortcutLast();
			Void			 OnShortcutMoveUp();
			Void			 OnShortcutMoveDown();
			Void			 OnShortcutMoveTop();
			Void			 OnShortcutMoveBottom();
			Void			 OnShortcutToggleMark();
			Void			 OnShortcutRemove();

			Void			 OnEditBoxEnter(EditBox *);

			Void			 OnEditBoxEnterArtist()		{ OnEditBoxEnter(info_edit_artist); }
			Void			 OnEditBoxEnterTitle()		{ OnEditBoxEnter(info_edit_title); }
			Void			 OnEditBoxEnterAlbum()		{ OnEditBoxEnter(info_edit_album); }
			Void			 OnEditBoxEnterTrack()		{ OnEditBoxEnter(info_edit_track); }
			Void			 OnEditBoxEnterYear()		{ OnEditBoxEnter(info_edit_year); }
			Void			 OnEditBoxEnterGenre()		{ OnEditBoxEnter(info_edit_genre); }

			Void			 OnEncoderStartEncoding();
			Void			 OnEncoderFinishEncoding(Bool);

			Void			 OnEncoderEncodeTrack(const BoCA::Track &, const String &, const String &, ConversionStep);

			Void			 OnEncoderTrackProgress(Int, Int);
			Void			 OnEncoderTotalProgress(Int, Int);

			PopupMenu		*GetContextMenu();

			Void			 ShowHideTitleInfo();
			Void			 UpdateTitleInfo();

			Void			 OnOpenFolder();
			Void			 OnBrowseForFolder();

			Void			 OnSelectFolder();
			Void			 OnSelectEncoder();

			Void			 ToggleEditPopup();

			Void			 UseStringForSelectedTracks();
			Void			 InterpretStringAs();
			Void			 InterpretStringAsAll();
			Void			 AdjustStringCase();
			Void			 AdjustStringCaseAll();
	};
};

#endif
