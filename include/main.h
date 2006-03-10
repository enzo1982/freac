 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
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

using namespace smooth;
using namespace smooth::GUI;

namespace BonkEnc
{
	const Int	 CHARSET_ISO_8859_1	= 0;
	const Int	 CHARSET_ISO_8859_2	= 1;
	const Int	 CHARSET_ISO_8859_5	= 2;
	const Int	 CHARSET_ISO_8859_7	= 3;
	const Int	 CHARSET_CP1251		= 4;
	const Int	 CHARSET_UTF_8		= 5;
	const Int	 CHARSET_UTF_16LE	= 6;
	const Int	 CHARSET_UTF_16BE	= 7;

	class BonkEncGUI : public BonkEnc
	{
		private:
			PopupMenu	*menu_file;
			PopupMenu	*menu_options;
			PopupMenu	*menu_addsubmenu;
			PopupMenu	*menu_encode;
			PopupMenu	*menu_drives;
			PopupMenu	*menu_files;
			PopupMenu	*menu_seldrive;
			PopupMenu	*menu_database;
			PopupMenu	*menu_trackmenu;
			PopupMenu	*menu_help;
			PopupMenu	*menu_encoders;
			PopupMenu	*menu_charsets;
			PopupMenu	*menu_charsets_all;
			PopupMenu	*menu_case;
			PopupMenu	*menu_case_all;

			Menubar		*mainWnd_menubar;
			Menubar		*mainWnd_iconbar;
			Titlebar	*mainWnd_titlebar;

			MicroMenu	*menu_edit_artist;
			MicroMenu	*menu_edit_title;
			MicroMenu	*menu_edit_album;
			MicroMenu	*menu_edit_year;
			MicroMenu	*menu_edit_genre;

			Hotspot		*htsp_edit_artist;
			Hotspot		*htsp_edit_title;
			Hotspot		*htsp_edit_album;
			Hotspot		*htsp_edit_year;
			Hotspot		*htsp_edit_genre;

			Button		*button_play;
			Button		*button_pause;
			Button		*button_stop;
			Button		*button_prev;
			Button		*button_next;
			Button		*button_open;

			Int		 clicked_drive;
			Int		 clicked_encoder;
			Int		 clicked_charset;
			Int		 clicked_case;

			Int		 activePopup;

			Thread		*checkForUpdates;

			Void		 Close();
			Void		 About();
			Void		 ConfigureEncoder();
			Void		 ConfigureGeneral();
			Void		 ShowHideTitleInfo();
			Void		 UpdateTitleInfo();
			Void		 ReadSpecificCD();

			Void		 QueryCDDB();
			Void		 SubmitCDDBData();

			Void		 EncodeSpecific();
			PopupMenu	*GetTrackMenu(Int, Int);
			Void		 ShowHelp();
			Void		 ShowTipOfTheDay();

			Void		 CheckForUpdates();
			Int		 CheckForUpdatesThread(Thread *);

			Bool		 SetLanguage();
			Void		 FillMenus();

			Void		 SetEncoderText();

			Void		 PlayItem(Int);
			Int		 PlayThread(Thread *);

			String		 AdjustCaseFirstCapital(const String &);
			String		 AdjustCaseWordsFirstCapital(const String &);
			String		 AdjustCaseLongWordsFirstCapital(const String &);
		signals:
			Signal0<Void>	 onChangeLanguageSettings;
		slots:
			Bool		 ExitProc();

			Void		 MessageProc(Int, Int, Int);
			Void		 ResizeProc();

			Void		 PlaySelectedItem();
			Void		 PausePlayback();
			Void		 StopPlayback();
			Void		 PlayPrevious();
			Void		 PlayNext();
			Void		 OpenCDTray();

			Void		 SelectDir();

			Void		 AddFilesFromDirectory();
			Void		 AddFilesByPattern();

			Void		 ToggleEditPopup();

			Void		 UseStringForSelectedTracks();
			Void		 InterpretStringAs();
			Void		 InterpretStringAsAll();
			Void		 AdjustStringCase();
			Void		 AdjustStringCaseAll();

			Void		 OnJoblistSelectTrack(Track *);
			Void		 OnJoblistSelectNone();
		public:
					 BonkEncGUI();
					~BonkEncGUI();

			CDDBInfo	*GetCDDBData();
	};
};

#endif
