 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_MAIN
#define H_FREAC_MAIN

#include <smooth.h>
#include "freac.h"

#include <support/notification.h>

#include <gui/main_joblist.h>
#include <gui/main_threads.h>

#ifdef __WIN32__
#	define DLLEXPORT __declspec (dllexport)
#else
#	define DLLEXPORT __attribute__ ((visibility ("default")))
#endif

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA::AS;

extern "C"
{
	Int DLLEXPORT StartGUI(const Array<String> &);
}

namespace freac
{
	class freacGUI : public freac
	{
		private:
			/* Singleton class, therefore protected constructor/destructor
			 */
							 freacGUI();
							~freacGUI();

			Window				*mainWnd;

			Titlebar			*mainWnd_titlebar;
			Statusbar			*mainWnd_statusbar;

			Menubar				*mainWnd_menubar;
			Menubar				*mainWnd_iconbar;

			PopupMenu			*menu_file;
			PopupMenu			*menu_addsubmenu;
			PopupMenu			*menu_files;
			PopupMenu			*menu_drives;

			PopupMenu			*menu_database;
			PopupMenu			*menu_database_query;

			PopupMenu			*menu_options;
			PopupMenu			*menu_configurations;
			PopupMenu			*menu_seldrive;

			PopupMenu			*menu_encode;
			PopupMenu			*menu_encoders;
			PopupMenu			*menu_encoder_options;

			PopupMenu			*menu_help;

			MenuEntry			*allowOverwriteMenuEntry;

			Hyperlink			*hyperlink;

			TabWidget			*tabs_main;
			LayerJoblist			*tab_layer_joblist;
			LayerThreads			*tab_layer_threads;

			JobList				*joblist;

			Notification			*notification;

			Int				 clicked_configuration;
			Int				 clicked_drive;
			Int				 clicked_encoder;

			Array<ExtensionComponent *>	 extensionComponents;

			Void				 InitExtensionComponents();
			Void				 FreeExtensionComponents();

			Void				 Close();
			Void				 About();

			Void				 ConfigureEncoder();
			Void				 ConfigureSettings();

			Void				 ShowHelp();
			Void				 ShowTipOfTheDay();

			Void				 CheckForUpdates();

			Bool				 SetLanguage();
			Void				 FillMenus();

			Void				 Convert();

			Void				 PauseResumeEncoding();
			Void				 StopEncoding();
		slots:
			Bool				 ExitProc();

			Void				 MessageProc(Int, Int, Int);

			Void				 OnChangePosition(const Point &);
			Void				 OnChangeSize(const Size &);

			Void				 OnSelectConfiguration();
			Void				 OnChangeConfiguration();

			Void				 OnDriveChange();
			Void				 OnDiscInsert(Int);
			Void				 OnDiscRemove(Int);

			Void				 ReadCD()				{ ReadCD(False); }
			Void				 ReadCD(Bool);

			Void				 QueryCDDB();
			Void				 QueryCDDBLater();
			Void				 SubmitCDDBData();

			Void				 ManageCDDBData();
			Void				 ManageCDDBBatchData();
			Void				 ManageCDDBBatchQueries();

			Void				 AddFilesFromDirectory();
			Void				 AddFilesByPattern();

			Void				 ToggleUseInputDirectory();
			Void				 ToggleEncodeToSingleFile();

			Void				 ConfirmDeleteAfterEncoding();
		public:
			/* Returns a new or existing instance of freacGUI
			 */
			static freacGUI			*Get();

			/* Destroys an existing instance of freacGUI
			 */
			static Void			 Free();
	};
};

#endif
