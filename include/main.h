 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
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

class bonkEncGUI : public bonkEnc
{
	private:
		Menu		*menu_file;
		Menu		*menu_options;
		Menu		*menu_addsubmenu;
		Menu		*menu_encode;
		Menu		*menu_drives;
		Menu		*menu_seldrive;
		Menu		*menu_database;
		Menu		*menu_trackmenu;
		Menu		*menu_help;
		Menu		*menu_encoders;

		Menubar		*mainWnd_menubar;
		Menubar		*mainWnd_iconbar;
		Titlebar	*mainWnd_titlebar;

		Button		*button_play;
		Button		*button_pause;
		Button		*button_stop;
		Button		*button_prev;
		Button		*button_next;

		Button		*button_sel_all;
		Button		*button_sel_none;
		Button		*button_sel_toggle;

		Int		 clicked_drive;
		Int		 clicked_encoder;

		Thread		*checkForUpdates;

		Bool		 ExitProc();
		Void		 ResizeProc();
		Void		 Close();
		Void		 About();
		Void		 ConfigureEncoder();
		Void		 ConfigureGeneral();
		Void		 AddFile();
		Void		 AddDragDropFile(String *);
		Void		 RemoveFile();
		Void		 SelectJoblistEntry();
		Void		 JoblistSelectAll();
		Void		 JoblistSelectNone();
		Void		 JoblistToggleSelection();
		Void		 ShowHideTitleInfo();
		Void		 UpdateTitleInfo();
		Void		 ReadSpecificCD();
		Void		 QueryCDDB();
		Void		 SubmitCDDBData();
		Void		 EncodeSpecific();
		Menu		*GetTrackMenu(Int, Int);
		Void		 ShowHelp();
		Void		 ShowTipOfTheDay();

		Void		 CheckForUpdates();
		Int		 CheckForUpdatesThread(Thread *);

		Bool		 SetLanguage(String);
	public:
				 bonkEncGUI();
				~bonkEncGUI();
};

#endif
