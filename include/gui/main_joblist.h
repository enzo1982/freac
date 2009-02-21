 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_MAIN_JOBLIST_
#define _H_MAIN_JOBLIST_

namespace BonkEnc
{
	class JobList;
	class Playback;
};

#include <bonkenc.h>

namespace BonkEnc
{
	const Int	 CHARSET_ISO_8859_1	= 0;
	const Int	 CHARSET_ISO_8859_2	= 1;
	const Int	 CHARSET_ISO_8859_5	= 2;
	const Int	 CHARSET_ISO_8859_7	= 3;
	const Int	 CHARSET_CP1251		= 4;
	const Int	 CHARSET_SHIFT_JIS	= 5;
	const Int	 CHARSET_GBK		= 6;
	const Int	 CHARSET_BIG_5		= 7;

	class LayerJoblist : public Layer
	{
		private:
			BoCA::I18n		*i18n;
			Config			*currentConfig;

			CheckBox		*check_single;
			CheckBox		*check_playlist;
			CheckBox		*check_cuesheet;

			Divider			*info_divider;
			Layer			*info_background;
			CheckBox		*info_checkbox;
			Divider			*info_bottom;
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
			ListBox			*info_list_genre;

			Text			*txt_filename;
			EditBox			*edb_filename;

			Text			*txt_format;
			EditBox			*edb_format;

			Text			*txt_encoder;
			ComboBox		*combo_encoder;

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

			Button			*btn_skip;
			Button			*btn_outdir;

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

			Button			*button_play;
			Button			*button_pause;
			Button			*button_stop;
			Button			*button_prev;
			Button			*button_next;
			Button			*button_open;

			PopupMenu		*menu_trackmenu;

			JobList			*joblist;
			Playback		*player;

			Bool			 dontUpdateInfo;

			Int			 clicked_charset;
			Int			 clicked_case;

			Int			 activePopup;

			Bool			 SetLanguage();
			Void			 FillMenus();

			String			 SecondsToString(Int);

			String			 AdjustCaseFirstCapital(const String &);
			String			 AdjustCaseWordsFirstCapital(const String &);
			String			 AdjustCaseLongWordsFirstCapital(const String &);
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

			Void			 OnJoblistSelectTrack(const Track &);
			Void			 OnJoblistModifyTrack(const Track &);
			Void			 OnJoblistRemoveTrack(const Track &);

			Void			 OnJoblistRemoveAllTracks();

			Void			 OnEncoderStartEncoding();
			Void			 OnEncoderFinishEncoding(Bool);

			Void			 OnEncoderEncodeTrack(const Track &, Int);

			Void			 OnEncoderTrackProgress(Int, Int);
			Void			 OnEncoderTotalProgress(Int, Int);

			PopupMenu		*GetContextMenu();

			Void			 ShowHideTitleInfo();
			Void			 UpdateTitleInfo();

			Void			 OnSelectDir();
			Void			 OnSelectEncoder();

			Void			 ToggleEditPopup();

			Void			 PlaySelectedItem();
			Void			 PauseResumePlayback();
			Void			 StopPlayback();
			Void			 OpenCDTray();

			Void			 UseStringForSelectedTracks();
			Void			 InterpretStringAs();
			Void			 InterpretStringAsAll();
			Void			 AdjustStringCase();
			Void			 AdjustStringCaseAll();
	};
};

#endif
