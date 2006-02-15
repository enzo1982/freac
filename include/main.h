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

			Menubar		*mainWnd_menubar;
			Menubar		*mainWnd_iconbar;
			Titlebar	*mainWnd_titlebar;

			Button		*button_play;
			Button		*button_pause;
			Button		*button_stop;
			Button		*button_prev;
			Button		*button_next;
			Button		*button_open;

			Int		 clicked_drive;
			Int		 clicked_encoder;

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

			Void		 OnJoblistSelectTrack(Track *);
			Void		 OnJoblistSelectNone();
		public:
					 BonkEncGUI();
					~BonkEncGUI();

			CDDBInfo	*GetCDDBData();
	};
};

#endif
