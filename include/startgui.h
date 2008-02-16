 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_MAIN_
#define _H_MAIN_

#include <smooth.h>
#include "bonkenc.h"
#include "playback.h"

using namespace smooth;
using namespace smooth::GUI;

extern "C"
{
	Int __declspec (dllexport) StartGUI(const Array<String> &);
}

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

	class BonkEncGUI : public BonkEnc
	{
		private:
			/* Singleton class, therefore protected constructor/destructor
			 */
						 BonkEncGUI();
						~BonkEncGUI();

			PopupMenu		*menu_file;
			PopupMenu		*menu_options;
			PopupMenu		*menu_addsubmenu;
			PopupMenu		*menu_encode;
			PopupMenu		*menu_drives;
			PopupMenu		*menu_files;
			PopupMenu		*menu_seldrive;
			PopupMenu		*menu_database;
			PopupMenu		*menu_database_query;
			PopupMenu		*menu_trackmenu;
			PopupMenu		*menu_help;
			PopupMenu		*menu_encoders;
			PopupMenu		*menu_encoder_options;
			PopupMenu		*menu_charsets;
			PopupMenu		*menu_charsets_all;
			PopupMenu		*menu_case;
			PopupMenu		*menu_case_all;

			MenuEntry		*allowOverwriteMenuEntry;

			Menubar			*mainWnd_menubar;
			Menubar			*mainWnd_iconbar;
			Titlebar		*mainWnd_titlebar;

			MicroMenu		*menu_edit_artist;
			MicroMenu		*menu_edit_title;
			MicroMenu		*menu_edit_album;
			MicroMenu		*menu_edit_year;
			MicroMenu		*menu_edit_genre;

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

			Int			 clicked_drive;
			Int			 clicked_encoder;
			Int			 clicked_charset;
			Int			 clicked_case;

			Int			 activePopup;

			Playback		*player;

			Void			 Close();
			Void			 About();
			Void			 ConfigureEncoder();
			Void			 ConfigureSettings();
			Void			 ShowHideTitleInfo();
			Void			 UpdateTitleInfo();
			Void			 ReadSpecificCD();

			Void			 EncodeSpecific();
			PopupMenu		*GetContextMenu();
			Void			 ShowHelp();
			Void			 ShowTipOfTheDay();

			Void			 CheckForUpdates();
			Int			 CheckForUpdates(Bool);

			Bool			 SetLanguage();
			Void			 FillMenus();

			Void			 SetEncoderText();

			Void			 Encode();

			Void			 PauseResumeEncoding();
			Void			 StopEncoding();

			String			 AdjustCaseFirstCapital(const String &);
			String			 AdjustCaseWordsFirstCapital(const String &);
			String			 AdjustCaseLongWordsFirstCapital(const String &);
		signals:
			Signal0<Void>		 onChangeLanguageSettings;
		slots:
			Bool			 ExitProc();

			Void			 MessageProc(Int, Int, Int);

			Void			 OnChangePosition(const Point &);
			Void			 OnChangeSize(const Size &);

			Void			 PlaySelectedItem();
			Void			 PauseResumePlayback();
			Void			 StopPlayback();
			Void			 OpenCDTray();

			Void			 QueryCDDB();
			Void			 QueryCDDBLater();
			Void			 SubmitCDDBData();

			Void			 ManageCDDBData();
			Void			 ManageCDDBBatchData();
			Void			 ManageCDDBBatchQueries();

			Void			 SelectDir();
			Void			 SkipTrack();

			Void			 AddFilesFromDirectory();
			Void			 AddFilesByPattern();

			Void			 ToggleEditPopup();
			Void			 ToggleUseInputDirectory();
			Void			 ToggleEncodeToSingleFile();

			Void			 UseStringForSelectedTracks();
			Void			 InterpretStringAs();
			Void			 InterpretStringAsAll();
			Void			 AdjustStringCase();
			Void			 AdjustStringCaseAll();

			Void			 ConfirmDeleteAfterEncoding();

			Void			 OnJoblistSelectTrack(Track *);
			Void			 OnJoblistSelectNone();

			Void			 OnEncoderStartEncoding();
			Void			 OnEncoderFinishEncoding(Bool);

			Void			 OnEncoderEncodeTrack(const Track *, Int);

			Void			 OnEncoderTrackProgress(Int, Int);
			Void			 OnEncoderTotalProgress(Int, Int);
		public:
			CDDBInfo		 GetCDDBData();

			/* Returns a new or existing instance of BonkEncGUI
			 */
			static BonkEncGUI	*Get();

			/* Destroys an existing instance of BonkEncGUI
			 */
			static Void		 Free();
	};
};

#endif
