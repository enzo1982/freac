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

#include <gui/main_joblist.h>
#include <gui/main_protocol.h>
#include <gui/main_threads.h>

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA::AS;

extern "C"
{
	Int __declspec (dllexport) StartGUI(const Array<String> &);
}

namespace BonkEnc
{
	class BonkEncGUI : public BonkEnc
	{
		private:
			/* Singleton class, therefore protected constructor/destructor
			 */
							 BonkEncGUI();
							~BonkEncGUI();

			PopupMenu			*menu_file;
			PopupMenu			*menu_options;
			PopupMenu			*menu_addsubmenu;
			PopupMenu			*menu_encode;
			PopupMenu			*menu_drives;
			PopupMenu			*menu_files;
			PopupMenu			*menu_seldrive;
			PopupMenu			*menu_database;
			PopupMenu			*menu_database_query;
			PopupMenu			*menu_help;
			PopupMenu			*menu_encoders;
			PopupMenu			*menu_encoder_options;

			MenuEntry			*allowOverwriteMenuEntry;

			Menubar				*mainWnd_menubar;
			Menubar				*mainWnd_iconbar;
			Titlebar			*mainWnd_titlebar;

			TabWidget			*tabs_main;
			LayerJoblist			*tab_layer_joblist;
			LayerThreads			*tab_layer_threads;
			LayerProtocol			*tab_layer_protocol;

			Int				 clicked_drive;
			Int				 clicked_encoder;

			Array<ExtensionComponent *>	 extensionComponents;

			Void				 InitExtensionComponents();
			Void				 FreeExtensionComponents();

			Void				 Close();
			Void				 About();
			Void				 ConfigureEncoder();
			Void				 ConfigureSettings();
			Void				 ReadSpecificCD();

			Void				 EncodeSpecific();
			Void				 ShowHelp();
			Void				 ShowTipOfTheDay();

			Void				 CheckForUpdates();
			Int				 CheckForUpdates(Bool);

			Bool				 SetLanguage();
			Void				 FillMenus();

			Void				 Encode();

			Void				 PauseResumeEncoding();
			Void				 StopEncoding();
		signals:
			Signal0<Void>			 onChangeLanguageSettings;
		slots:
			Bool				 ExitProc();

			Void				 MessageProc(Int, Int, Int);

			Void				 OnChangePosition(const Point &);
			Void				 OnChangeSize(const Size &);

			Void				 ReadCD();

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
			CDDBInfo			 GetCDDBData();

			/* Returns a new or existing instance of BonkEncGUI
			 */
			static BonkEncGUI		*Get();

			/* Destroys an existing instance of BonkEncGUI
			 */
			static Void			 Free();
	};
};

#endif
