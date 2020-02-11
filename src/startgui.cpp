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

#include <startgui.h>
#include <joblist.h>
#include <playback.h>
#include <config.h>
#include <utilities.h>
#include <resources.h>

#include <engine/converter.h>

#include <jobs/engine/convert.h>
#include <jobs/joblist/addtracks.h>
#include <jobs/joblist/removedisc.h>
#include <jobs/other/checkforupdates.h>

#include <dialogs/config/config.h>
#include <dialogs/config/configcomponent.h>

#include <dialogs/adddirectory.h>
#include <dialogs/addpattern.h>

#include <dialogs/error.h>

#include <cddb/cddbremote.h>
#include <cddb/cddbcache.h>

#include <dialogs/cddb/query.h>
#include <dialogs/cddb/submit.h>
#include <dialogs/cddb/manage.h>
#include <dialogs/cddb/managequeries.h>
#include <dialogs/cddb/managesubmits.h>

#include <gui/main_joblist.h>
#include <gui/main_threads.h>

#include <support/notification.h>

#ifdef __WIN32__
#	include <smooth/init.win32.h>
#endif

using namespace smooth::GUI::Dialogs;
using namespace smooth::I18n;
using namespace smooth::Input;

using namespace BoCA;
using namespace BoCA::AS;

Int StartGUI(const Array<String> &args)
{
	freac::freacGUI	*app = freac::freacGUI::Get();

	app->Loop();

	freac::freacGUI::Free();

	return 0;
}

freac::freacGUI *freac::freacGUI::Get()
{
	if (instance == NIL) new freacGUI();

	return (freacGUI *) instance;
}

Void freac::freacGUI::Free()
{
	if (instance != NIL) delete (freacGUI *) instance;
}

freac::freacGUI::freacGUI()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	/* Setup Notification instance.
	 */
	notification = Notification::Get();

	if (notification != NIL)
	{
		notification->onDriveChange.Connect(&freacGUI::OnDriveChange, this);
		notification->onDiscInsert.Connect(&freacGUI::OnDiscInsert, this);
		notification->onDiscRemove.Connect(&freacGUI::OnDiscRemove, this);
	}

	config->GetPersistentIntValue(Config::CategorySettingsID, Config::SettingsNotificationAvailableID, False) = notification != NIL && notification->IsNotificationAvailable();

	/* Setup attributes.
	 */
	clicked_configuration = -1;
	clicked_drive	      = -1;
	clicked_encoder	      = -1;
	clicked_processor     = -1;

	Rect	 workArea    = Screen::GetActiveScreenWorkArea();
	Float	 scaleFactor = Surface().GetSurfaceDPI() / 96.0;

	Point	 wndPos	     = Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault));
	Size	 wndSize     = Size(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowSizeXID, Config::SettingsWindowSizeXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowSizeYID, Config::SettingsWindowSizeYDefault));

	if (wndPos.x + wndSize.cx * scaleFactor > workArea.right + 2 ||
	    wndPos.y + wndSize.cy * scaleFactor > workArea.bottom + 2)
	{
		wndPos.x = (Int) Math::Min(workArea.right - 10 - Math::Round(wndSize.cx * scaleFactor), (Int64) wndPos.x);
		wndPos.y = (Int) Math::Min(workArea.bottom - 10 - Math::Round(wndSize.cy * scaleFactor), (Int64) wndPos.y);
	}

	if (wndPos.x < workArea.left - 2 ||
	    wndPos.y < workArea.top - 2)
	{
		wndPos.x = (Int) Math::Max(workArea.left + 10, wndPos.x);
		wndPos.y = (Int) Math::Max(workArea.top + 10, wndPos.y);

		wndSize.cx = (Int) Math::Min(Math::Round((workArea.right - 20) / scaleFactor), (Int64) wndSize.cx);
		wndSize.cy = (Int) Math::Min(Math::Round((workArea.bottom - 20) / scaleFactor), (Int64) wndSize.cy);
	}

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, wndPos.x);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, wndPos.y);

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWindowSizeXID, wndSize.cx);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWindowSizeYID, wndSize.cy);

	mainWnd			= new Window(String(freac::appLongName).Append(" ").Append(freac::version), wndPos, wndSize);
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar();
	mainWnd_menubar		= new Menubar();
	mainWnd_iconbar		= new Menubar();
	mainWnd_statusbar	= new Statusbar(String(freac::appLongName).Append(" ").Append(freac::version).Append(" - ").Append(freac::copyright));

	menu_file		= new PopupMenu();
	menu_addsubmenu		= new PopupMenu();
	menu_files		= new PopupMenu();
	menu_drives		= new PopupMenu();

	menu_database		= new PopupMenu();
	menu_database_query	= new PopupMenu();

	menu_options		= new PopupMenu();
	menu_configurations	= new PopupMenu();
	menu_seldrive		= new PopupMenu();

	menu_processing		= new PopupMenu();
	menu_processors		= new PopupMenu();

	menu_encode		= new PopupMenu();
	menu_encoders		= new PopupMenu();
	menu_encoder_options	= new PopupMenu();

	menu_help		= new PopupMenu();

	hyperlink		= new Hyperlink(String(freac::website).Replace("https://", NIL).Replace("/", NIL), freac::website, Point(91, -22));
	hyperlink->SetOrientation(OR_UPPERRIGHT);
	hyperlink->SetX(hyperlink->GetUnscaledTextWidth() + 4);
	hyperlink->SetIndependent(True);

	/* Build tabbed interface.
	 */
	tabs_main		= new TabWidget(Point(6, 7), Size(700, 500));

	/* Add joblist tab.
	 */
	tab_layer_joblist	= new LayerJoblist();
	tab_layer_joblist->onRequestSkipTrack.Connect(&JobConvert::Skip);

#ifndef BUILD_VIDEO_DOWNLOADER
	tabs_main->Add(tab_layer_joblist);
#endif

	/* Add extension component tabs.
	 */
	InitExtensionComponents();

	foreach (ExtensionComponent *extensionComponent, extensionComponents)
	{
		Layer	*mainTabLayer	= extensionComponent->getMainTabLayer.Emit();
		Layer	*statusBarLayer = extensionComponent->getStatusBarLayer.Emit();

		if (mainTabLayer   != NIL) tabs_main->Add(mainTabLayer);
		if (statusBarLayer != NIL) mainWnd_statusbar->Add(statusBarLayer);
	}

#ifdef BUILD_VIDEO_DOWNLOADER
	tabs_main->Add(tab_layer_joblist);
#endif

	/* Add threads tab.
	 */
	tab_layer_threads	= new LayerThreads();

	if (config->GetIntValue(Config::CategorySettingsID,
				Config::SettingsShowJobsTabID,
				Config::SettingsShowJobsTabDefault)) tabs_main->Add(tab_layer_threads);

	joblist			= tab_layer_joblist->GetJoblist();

	Add(mainWnd);

	SetLanguage();

	mainWnd->Add(hyperlink);
	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(mainWnd_statusbar);
	mainWnd->Add(mainWnd_menubar);
	mainWnd->Add(mainWnd_iconbar);
	mainWnd->Add(tabs_main);

	/* Do not use tab widget if there would be only one tab.
	 */
	if (tabs_main->GetNOfObjects() == 1)
	{
		Widget	*tab = tabs_main->GetNthObject(0);

		tabs_main->Remove(tab);

		tab->SetOrientation(OR_CENTER);

		mainWnd->Remove(tabs_main);
		mainWnd->Add(tab);
	}

	mainWnd->SetIcon(ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/freac.png")));

#ifdef __WIN32__
	mainWnd->SetIconDirect(LoadImageA(hInstance, MAKEINTRESOURCEA(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
#endif

	mainWnd->onChangePosition.Connect(&freacGUI::OnChangePosition, this);
	mainWnd->onChangeSize.Connect(&freacGUI::OnChangeSize, this);

	mainWnd->onEvent.Connect(&freacGUI::MessageProc, this);

	if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsShowTipsID, Config::SettingsShowTipsDefault)) mainWnd->onShow.Connect(&freacGUI::ShowTipOfTheDay, this);

	mainWnd->doClose.Connect(&freacGUI::ExitProc, this);
	mainWnd->SetMinimumSize(Size(600, 400 + (config->GetIntValue(Config::CategorySettingsID, Config::SettingsShowTitleInfoID, Config::SettingsShowTitleInfoDefault) ? 68 : 0)));

	if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowMaximizedID, Config::SettingsWindowMaximizedDefault)) mainWnd->Maximize();

	/* Run update check.
	 */
	if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsCheckForUpdatesID, Config::SettingsCheckForUpdatesDefault)) (new JobCheckForUpdates(True))->Schedule();
}

freac::freacGUI::~freacGUI()
{
	/* Free Notification instance.
	 */
	Notification::Free();

	/* Free BoCA components.
	 */
	FreeExtensionComponents();

	/* Delete GUI widgets.
	 */
	DeleteObject(mainWnd_menubar);
	DeleteObject(mainWnd_iconbar);
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd_statusbar);
	DeleteObject(mainWnd);

	DeleteObject(tabs_main);

	DeleteObject(tab_layer_joblist);
	DeleteObject(tab_layer_threads);

	DeleteObject(hyperlink);

	DeleteObject(menu_file);
	DeleteObject(menu_addsubmenu);
	DeleteObject(menu_files);
	DeleteObject(menu_drives);

	DeleteObject(menu_database);
	DeleteObject(menu_database_query);

	DeleteObject(menu_options);
	DeleteObject(menu_configurations);
	DeleteObject(menu_seldrive);

	DeleteObject(menu_processing);
	DeleteObject(menu_processors);

	DeleteObject(menu_encode);
	DeleteObject(menu_encoders);
	DeleteObject(menu_encoder_options);

	DeleteObject(menu_help);

	foreach (PopupMenu *menu_format, formatMenus) DeleteObject(menu_format);

	formatMenus.RemoveAll();
}

Void freac::freacGUI::InitExtensionComponents()
{
	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_EXTENSION) continue;

		ExtensionComponent	*component = (ExtensionComponent *) boca.CreateComponentByID(boca.GetComponentID(i));

		if (component != NIL) extensionComponents.Add(component);
	}
}

Void freac::freacGUI::FreeExtensionComponents()
{
	Registry	&boca = Registry::Get();

	foreach (ExtensionComponent *extensionComponent, extensionComponents)
	{
		boca.DeleteComponent(extensionComponent);
	}

	extensionComponents.RemoveAll();
}

Bool freac::freacGUI::ExitProc()
{
	BoCA::Config	*config = BoCA::Config::Get();

	/* Check if encoder is running.
	 */
	if (JobConvert::IsConverting())
	{
		BoCA::I18n	*i18n = BoCA::I18n::Get();

		i18n->SetContext("Messages");

		if (Message::Button::No == QuickMessage(i18n->TranslateString("A conversion process is still active! Do you really want to quit?"), i18n->TranslateString("Currently converting"), Message::Buttons::YesNo, Message::Icon::Question)) return False;

		JobConvert::Stop();
	}

	/* Stop playback if playing a track
	 */
	Playback::Get()->Stop();

	/* Notify components that we are about to quit.
	 */
	BoCA::Application::Get()->onQuit.Emit();

	/* Save main window position.
	 */
	Rect	 wndRect = mainWnd->GetRestoredWindowRect() - mainWnd->GetSizeModifier();

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, wndRect.left);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, wndRect.top);

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWindowSizeXID, wndRect.right - wndRect.left);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWindowSizeYID, wndRect.bottom - wndRect.top);

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWindowMaximizedID, mainWnd->IsMaximized());

	config->SaveSettings();

	return True;
}

Void freac::freacGUI::MessageProc(Int message, Int wParam, Int lParam)
{
	notification->ProcessSystemMessage(message, wParam, lParam);
}

Void freac::freacGUI::OnChangePosition(const Point &nPos)
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, mainWnd->GetPosition().x);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, mainWnd->GetPosition().y);
}

Void freac::freacGUI::OnChangeSize(const Size &nSize)
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWindowSizeXID, mainWnd->GetSize().cx);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWindowSizeYID, mainWnd->GetSize().cy);

	mainWnd->SetStatusText(String(freac::appLongName).Append(" ").Append(freac::version).Append(" - ").Append(freac::copyright));

	Rect	 clientRect = mainWnd->GetClientRect();
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	tabs_main->SetSize(clientSize - Size(12, 14));
}

Void freac::freacGUI::Close()
{
	mainWnd->Close();
}

Void freac::freacGUI::About()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("About");

	QuickMessage(String(freac::appLongName).Append(" ").Append(freac::version).Append(" (").Append(freac::architecture).Append(")\n").Append(freac::copyright).Append("\n\n").Append(i18n->TranslateString("Translated by %1.").Replace("%1", i18n->GetActiveLanguageAuthor())).Append("\n\n").Append(i18n->TranslateString("This program is being distributed under the terms\nof the GNU General Public License (GPL).")), i18n->TranslateString("About %1").Replace("%1", freac::appName), Message::Buttons::Ok, ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/freac.png")));
}

Void freac::freacGUI::ConfigureSettings(ConfigurePage page)
{
	BoCA::Config	*config = BoCA::Config::Get();

	/* Save joblist tab field sizes.
	 */
	String	 fieldSizes;

	for (Int i = 0; i < joblist->GetNOfTabs(); i++)
	{
		if (i > 0) fieldSizes.Append(",");

		if (joblist->GetNthTabWidth(i) <= 0) fieldSizes.Append("*");
		else				     fieldSizes.Append(String::FromInt(joblist->GetNthTabWidth(i)));
	}

	config->SetStringValue(Config::CategoryJoblistID, Config::JoblistFieldSizesID, fieldSizes);

	/* Show configuration dialog.
	 */
	ConfigDialog().ShowDialog(page);

	/* Select active configuration if we have multiple.
	 */
	for (Int i = 0; i < config->GetNOfConfigurations(); i++)
	{
		if (config->GetNthConfigurationName(i) == config->GetConfigurationName()) clicked_configuration = i;
	}

	/* Update widgets and notify components.
	 */
	OnChangeConfiguration();
}

Void freac::freacGUI::ConfigureSettings()
{
	ConfigureSettings(ConfigurePageDefault);
}

Void freac::freacGUI::ConfigureProcessing()
{
	ConfigureSettings(ConfigurePageProcessing);
}

Void freac::freacGUI::ConfigureComponent(const String &id)
{
	Registry	&boca = Registry::Get();
	Component	*component = boca.CreateComponentByID(id);

	if (component != NIL)
	{
		ConfigComponentDialog	 dlg(component);

		dlg.SetParentWindow(mainWnd);

		if (dlg.ShowDialog() == Error()) BoCA::Utilities::ErrorMessage("No configuration dialog available for:\n\n%1", component->GetName());

		boca.DeleteComponent(component);
	}
}

Void freac::freacGUI::ConfigureEncoder()
{
	BoCA::Config	*config	= BoCA::Config::Get();

	ConfigureComponent(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault));
}

Void freac::freacGUI::ConfigureProcessor()
{
	BoCA::Config	*config	= BoCA::Config::Get();

	/* Open configuration dialog for selected processor.
	 */
	const Array<String>	&ids = config->GetStringValue(Config::CategoryProcessingID, Config::ProcessingComponentsID, Config::ProcessingComponentsDefault).Explode(",");

	ConfigureComponent(ids.GetNth(clicked_processor));

	/* Reset selected processor.
	 */
	clicked_processor = -1;

	OptionBox::internalCheckValues.Emit();
}

Void freac::freacGUI::OnSelectConfiguration()
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SaveSettings();
	config->SetActiveConfiguration(config->GetNthConfigurationName(clicked_configuration));

	/* Update widgets and notify components.
	 */
	OnChangeConfiguration();
}

Void freac::freacGUI::OnChangeConfiguration()
{
	BoCA::Config	*config  = BoCA::Config::Get();
	BoCA::I18n	*i18n	 = BoCA::I18n::Get();

	Surface		*surface = mainWnd->GetDrawSurface();

	surface->StartPaint(mainWnd->GetVisibleArea());

	/* Update language and menu bars.
	 */
	if (i18n->GetActiveLanguageID() != config->GetStringValue(Config::CategorySettingsID,
								  Config::SettingsLanguageID,
								  Config::SettingsLanguageDefault)) SetLanguage();
	else											    FillMenus();

	/* Notify components of configuration change.
	 */
	BoCA::Settings::Get()->onChangeConfigurationSettings.Emit();

	/* Update tabs for new configuration.
	 */
	const Widget	*selectedTab = tabs_main->GetSelectedTab();

	for (Int i = 0; i < tabs_main->GetNOfObjects(); i++)
	{
		if (tabs_main->GetNthObject(i)->GetObjectType() != Layer::classID) continue;

		tabs_main->Remove(tabs_main->GetNthObject(i--));
	}

#ifndef BUILD_VIDEO_DOWNLOADER
	tabs_main->Add(tab_layer_joblist);
#endif

	foreach (ExtensionComponent *extensionComponent, extensionComponents)
	{
		Layer	*mainTabLayer = extensionComponent->getMainTabLayer.Emit();

		if (mainTabLayer != NIL) tabs_main->Add(mainTabLayer);
	}

#ifdef BUILD_VIDEO_DOWNLOADER
	tabs_main->Add(tab_layer_joblist);
#endif

	if (config->GetIntValue(Config::CategorySettingsID,
				Config::SettingsShowJobsTabID,
				Config::SettingsShowJobsTabDefault)) tabs_main->Add(tab_layer_threads);

	tabs_main->SelectTab(selectedTab);
	tabs_main->Paint(SP_PAINT);

	/* Do not use tab widget if there would be only one tab.
	 */
	if (tabs_main->GetNOfObjects() == 1)
	{
		Widget	*tab = tabs_main->GetNthObject(0);

		if (tabs_main->GetContainer() != NIL)
		{
			tabs_main->Remove(tab);

			tab->SetOrientation(OR_CENTER);

			mainWnd->Remove(tabs_main);
			mainWnd->Add(tab);
		}
		else
		{
			Widget	*mainTab = NIL;

			for (Int i = mainWnd->GetNOfObjects() - 1; i >= 0; i--)
			{
				if (mainWnd->GetNthObject(i)->GetObjectType() == Layer::classID) { mainTab = mainWnd->GetNthObject(i); break; }
			}

			mainWnd->Remove(mainTab);
			mainWnd->Add(tabs_main);

			if (mainTab != NIL)
			{
				mainTab->SetOrientation(OR_UPPERLEFT);

				tabs_main->Remove(tab);
				tabs_main->Add(mainTab);
				tabs_main->Add(tab);
			}
		}

		mainWnd->Paint(SP_PAINT);
	}

	/* Update joblist layer.
	 */
	tab_layer_joblist->UpdateEncoderText();
	tab_layer_joblist->UpdateOutputDir();

	CheckBox::internalCheckValues.Emit();

	ToggleSignalProcessing();
	ToggleUseInputDirectory();

	surface->EndPaint();

	config->SaveSettings();
}

Void freac::freacGUI::OnDriveChange()
{
	/* Update menus to reflect drive change.
	 */
	FillMenus();
}

Void freac::freacGUI::OnDiscInsert(Int drive)
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (config->GetIntValue(Config::CategoryRipperID, Config::RipperAutoReadContentsID, Config::RipperAutoReadContentsDefault))
	{
		Int	 activeDrive = config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault);

		config->SetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, drive);

		ReadCD(True);

		config->SetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, activeDrive);
	}
}

Void freac::freacGUI::OnDiscRemove(Int drive)
{
	(new JobRemoveDiscTracks(drive))->Schedule();
}

Void freac::freacGUI::ReadCD(Bool autoCDRead)
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (clicked_drive >= 0)
	{
		config->SetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, clicked_drive);

		clicked_drive = -1;

		OptionBox::internalCheckValues.Emit();
	}

	Registry		&boca = Registry::Get();
	DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

	if (info != NIL)
	{
		const Array<String>	&urls = info->GetNthDeviceTrackList(config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault));

		(new JobAddTracks(urls, autoCDRead))->Schedule();

		boca.DeleteComponent(info);
	}
}

Void freac::freacGUI::QueryCDDB()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	/* Check if CDDB support is enabled.
	 */
	if (!config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableLocalID, Config::FreedbEnableLocalDefault) && !config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, Config::FreedbEnableRemoteDefault))
	{
		BoCA::Utilities::ErrorMessage("CDDB support is disabled! Please enable local or\nremote CDDB support in the configuration dialog.");

		return;
	}

	/* Collect CDDB query strings.
	 */
	Array<String>	 queryStrings;

	for (Int i = 0; i < joblist->GetNOfTracks(); i++)
	{
		const Track	&track = joblist->GetNthTrack(i);
		const Info	&info  = track.GetInfo();

		if (info.mcdi.GetData().Size() > 0)
		{
			String	 queryString = CDDB::QueryStringFromMCDI(info.mcdi);

			queryStrings.Add(queryString, queryString.ComputeCRC32());
		}
		else if (info.offsets != NIL)
		{
			String	 queryString = CDDB::QueryStringFromOffsets(info.offsets);

			queryStrings.Add(queryString, queryString.ComputeCRC32());
		}
	}

	/* Query database using each query string.
	 */
	Bool		 remoteCDDBEnabled = config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, Config::FreedbEnableRemoteDefault);
	Array<String>	 queryErrors;

	for (Int i = 0; i < queryStrings.Length(); i++)
	{
		const String	&queryString = queryStrings.GetNth(i);
		CDDBInfo	 cdInfo;

		if (config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableCacheID, Config::FreedbEnableCacheDefault)) cdInfo = CDDBCache::Get()->GetCacheEntry(queryString);

		if (cdInfo == NIL)
		{
			if (config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableLocalID, Config::FreedbEnableLocalDefault) ||
			    config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, Config::FreedbEnableRemoteDefault))
			{
				cddbQueryDlg	 dlg(queryString);

				if (dlg.ShowDialog() == Error())
				{
					/* Ask whether to perform this query later.
					 */
					CDDBBatch	 queries;

					if (i == queryStrings.Length() - 1 && QuickMessage(dlg.GetErrorString().Append("\n\n").Append(i18n->TranslateString("Would you like to perform this query again later?", "CDDB::Query::Errors")), i18n->TranslateString("Error"), Message::Buttons::YesNo, Message::Icon::Error) == Message::Button::Yes)
					{
						queries.AddQuery(queryString);
					}
					else if (i < queryStrings.Length() - 1 && QuickMessage(dlg.GetErrorString().Append("\n\n").Append(i18n->TranslateString("Would you like to perform the remaining queries again later?", "CDDB::Query::Errors")), i18n->TranslateString("Error"), Message::Buttons::YesNo, Message::Icon::Error) == Message::Button::Yes)
					{
						for (Int j = i; j < queryStrings.Length(); j++) queries.AddQuery(queryStrings.GetNth(j));
					}

					/* Temporarily disable remote CDDB queries if connection failed.
					 */
					config->SetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, False);
				}
				else if (dlg.GetErrorString() != NIL)
				{
					/* If there's an info message, add it to the list of errors.
					 */
					queryErrors.Add(dlg.GetErrorString());
				}

				cdInfo = dlg.GetCDDBInfo();
			}

			if (cdInfo != NIL) CDDBCache::Get()->AddCacheEntry(cdInfo);
		}

		/* Continue with next query if we couldn't find anything.
		 */
		if (cdInfo == NIL) continue;

		/* Update tracks with CDDB data.
		 */
		for (Int j = 0; j < joblist->GetNOfTracks(); j++)
		{
			Track	 track = joblist->GetNthTrack(j);
			Info	 info  = track.GetInfo();

			if ((info.mcdi.GetData().Size() > 0 && queryString == CDDB::QueryStringFromMCDI(info.mcdi)) ||
			    (info.offsets != NIL && queryString == CDDB::QueryStringFromOffsets(info.offsets)))
			{
				Int	 trackNumber = -1;

				if (track.isCDTrack) trackNumber = track.cdTrack;
				else		     trackNumber = info.track;

				if (trackNumber == -1) continue;

				info.artist	 = (cdInfo.dArtist == "Various" ? cdInfo.trackArtists.GetNth(trackNumber - 1) : cdInfo.dArtist);
				info.title	 = cdInfo.trackTitles.GetNth(trackNumber - 1);
				info.album	 = cdInfo.dTitle;
				info.genre	 = cdInfo.dGenre;
				info.year	 = cdInfo.dYear;
				info.track	 = trackNumber;

				track.SetInfo(info);
				track.SetOriginalInfo(info);

				track.outputFile = NIL;

				BoCA::JobList::Get()->onComponentModifyTrack.Emit(track);
			}
		}
	}

	/* Display errors.
	 */
	if (queryErrors.Length() > 0)
	{
		ErrorDialog	 dialog(queryErrors);

		dialog.ShowDialog();
	}

	/* Restore previous remote CDDB state.
	 */
	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, remoteCDDBEnabled);
}

Void freac::freacGUI::QueryCDDBLater()
{
	/* Collect CDDB query strings.
	 */
	Array<String>	 queryStrings;

	for (Int i = 0; i < joblist->GetNOfTracks(); i++)
	{
		const Track	&track = joblist->GetNthTrack(i);
		const Info	&info  = track.GetInfo();

		if (track.isCDTrack)
		{
			String	 queryString = CDDB::QueryStringFromMCDI(info.mcdi);

			queryStrings.Add(queryString, queryString.ComputeCRC32());
		}
	}

	/* Add queries to batch.
	 */
	if (queryStrings.Length() > 0)
	{
		CDDBBatch	 queries;

		foreach (const String &queryString, queryStrings) queries.AddQuery(queryString);
	}
}

Void freac::freacGUI::SubmitCDDBData()
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (!config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableLocalID, Config::FreedbEnableLocalDefault) && !config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, Config::FreedbEnableRemoteDefault))
	{
		BoCA::Utilities::ErrorMessage("CDDB support is disabled! Please enable local or\nremote CDDB support in the configuration dialog.");

		return;
	}

	cddbSubmitDlg().ShowDialog();
}

Void freac::freacGUI::ManageCDDBData()
{
	cddbManageDlg().ShowDialog();
}

Void freac::freacGUI::ManageCDDBBatchData()
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (!config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, Config::FreedbEnableRemoteDefault))
	{
		BoCA::Utilities::ErrorMessage("Remote CDDB support is disabled! Please enable\nremote CDDB support in the configuration dialog.");

		return;
	}

	cddbManageSubmitsDlg().ShowDialog();
}

Void freac::freacGUI::ManageCDDBBatchQueries()
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (!config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, Config::FreedbEnableRemoteDefault))
	{
		BoCA::Utilities::ErrorMessage("Remote CDDB support is disabled! Please enable\nremote CDDB support in the configuration dialog.");

		return;
	}

	cddbManageQueriesDlg().ShowDialog();
}

Bool freac::freacGUI::SetLanguage()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	Bool		 prevRTL = i18n->IsActiveLanguageRightToLeft();

	i18n->ActivateLanguage(config->GetStringValue(Config::CategorySettingsID, Config::SettingsLanguageID, Config::SettingsLanguageDefault));

	MessageDlg::SetDefaultRightToLeft(i18n->IsActiveLanguageRightToLeft());

	if (i18n->IsActiveLanguageRightToLeft() != prevRTL)
	{
		mainWnd->SetUpdateRect(Rect(Point(0, 0), mainWnd->GetRealSize()));
		mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());
		mainWnd->Paint(SP_PAINT);
	}

	BoCA::Settings::Get()->onChangeLanguageSettings.Emit();

	FillMenus();

	tabs_main->Paint(SP_PAINT);

	return true;
}

Void freac::freacGUI::FillMenus()
{
	BoCA::Config	*config	 = BoCA::Config::Get();
	BoCA::I18n	*i18n	 = BoCA::I18n::Get();

	Registry	&boca	 = Registry::Get();
	Surface		*surface = mainWnd->GetDrawSurface();

	surface->StartPaint(Rect::EncloseRect(mainWnd_menubar->GetVisibleArea(), mainWnd_iconbar->GetVisibleArea()));

	mainWnd_menubar->Hide();
	mainWnd_iconbar->Hide();

	/* Clear menus.
	 */
	menu_file->RemoveAllEntries();
	menu_addsubmenu->RemoveAllEntries();
	menu_files->RemoveAllEntries();
	menu_drives->RemoveAllEntries();

	menu_database->RemoveAllEntries();
	menu_database_query->RemoveAllEntries();

	menu_options->RemoveAllEntries();
	menu_configurations->RemoveAllEntries();
	menu_seldrive->RemoveAllEntries();

	menu_processing->RemoveAllEntries();
	menu_processors->RemoveAllEntries();

	menu_encode->RemoveAllEntries();
	menu_encoders->RemoveAllEntries();
	menu_encoder_options->RemoveAllEntries();

	menu_help->RemoveAllEntries();

	foreach (PopupMenu *menu_format, formatMenus) DeleteObject(menu_format);

	formatMenus.RemoveAll();

	/* Fill file menu.
	 */
	i18n->SetContext("Menu::File");

	MenuEntry	*entry = NIL;

	menu_file->AddEntry(i18n->TranslateString("Add"), ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/joblist/joblist-add.png")), menu_addsubmenu);

	entry = menu_file->AddEntry(i18n->TranslateString("Remove"), ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/joblist/joblist-remove.png")));
	entry->onAction.Connect(&JobList::RemoveSelectedTrack, joblist);
	entry->SetShortcut(SC_CONTROL, Keyboard::KeyR, mainWnd);

	if (boca.GetNumberOfComponentsOfType(COMPONENT_TYPE_PLAYLIST) > 1 || (boca.GetNumberOfComponentsOfType(COMPONENT_TYPE_PLAYLIST) > 0 && !boca.ComponentExists("cuesheet-playlist")))
	{
		menu_file->AddEntry();

		menu_file->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Load joblist")))->onAction.Connect(&JobList::LoadList, joblist);
		menu_file->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Save joblist")))->onAction.Connect(&JobList::SaveList, joblist);
	}

	menu_file->AddEntry();

	entry = menu_file->AddEntry(i18n->TranslateString("Clear joblist"), ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/joblist/joblist-clear.png")));
	entry->onAction.Connect(&JobList::StartJobRemoveAllTracks, joblist);
	entry->SetShortcut(SC_CONTROL | SC_SHIFT, Keyboard::KeyR, mainWnd);

	menu_file->AddEntry();

	entry = menu_file->AddEntry(i18n->TranslateString("Exit"), ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/other/exit.png")));
	entry->onAction.Connect(&freacGUI::Close, this);

#ifdef __WIN32__
	entry->SetShortcut(SC_ALT, Keyboard::KeyF4, mainWnd);
#else
	entry->SetShortcut(SC_CONTROL, Keyboard::KeyQ, mainWnd);
#endif

	entry = menu_addsubmenu->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Audio file(s)")), ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/joblist/joblist-addfiles.png")));
	entry->onAction.Connect(&JobList::AddTrackByDialog, joblist);
	entry->SetShortcut(SC_CONTROL, Keyboard::KeyF, mainWnd);

	DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

	if (info != NIL)
	{
		for (Int i = 0; i < info->GetNumberOfDevices(); i++)
		{
			const Device	&device = info->GetNthDeviceInfo(i);
			String		 name	= String(device.vendor).Append(" ").Append(device.model).Append(" ").Append(device.revision).Trim();

			menu_drives->AddEntry(name, &clicked_drive, i)->onAction.Connect(&freacGUI::ReadCD, this);
			menu_seldrive->AddEntry(name, &config->GetPersistentIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault), i);
		}

		boca.DeleteComponent(info);
	}

	if (menu_drives->Length() >= 1)
	{
		entry = menu_addsubmenu->AddEntry(i18n->TranslateString("Audio CD contents"), ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/joblist/joblist-adddisc.png")));
		entry->onAction.Connect(&freacGUI::ReadCD, this);
		entry->SetShortcut(SC_CONTROL, Keyboard::KeyD, mainWnd);
	}

	menu_files->AddEntry(i18n->AddEllipsis(i18n->TranslateString("By pattern")))->onAction.Connect(&freacGUI::AddFilesByPattern, this);
	menu_files->AddEntry(i18n->AddEllipsis(i18n->TranslateString("From folder")))->onAction.Connect(&freacGUI::AddFilesFromDirectory, this);

	menu_addsubmenu->AddEntry();
	menu_addsubmenu->AddEntry(i18n->TranslateString("Audio file(s)"), menu_files);

	if (menu_drives->Length() > 1)
	{
		menu_addsubmenu->AddEntry(i18n->TranslateString("Audio CD contents"), menu_drives);
	}

	/* Fill database menu.
	 */
	i18n->SetContext("Menu::Database");

	entry = menu_database->AddEntry(i18n->TranslateString("Query CDDB database"), ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/freedb/freedb-query.png")));
	entry->onAction.Connect(&freacGUI::QueryCDDB, this);
	entry->SetShortcut(SC_CONTROL | SC_SHIFT, Keyboard::KeyQ, mainWnd);

	if (menu_drives->Length() >= 1)
	{
		entry = menu_database->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Submit CDDB data")), ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/freedb/freedb-edit.png")));
		entry->onAction.Connect(&freacGUI::SubmitCDDBData, this);
		entry->SetShortcut(SC_CONTROL | SC_SHIFT, Keyboard::KeyS, mainWnd);

		menu_database->AddEntry();
		menu_database->AddEntry(i18n->TranslateString("Query CDDB database later"))->onAction.Connect(&freacGUI::QueryCDDBLater, this);

		menu_database->AddEntry();
		menu_database->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Show queued CDDB entries")))->onAction.Connect(&freacGUI::ManageCDDBBatchData, this);
		menu_database->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Show queued CDDB queries")))->onAction.Connect(&freacGUI::ManageCDDBBatchQueries, this);
	}

	menu_database->AddEntry();

	menu_database->AddEntry(i18n->TranslateString("Enable CDDB cache"), (Bool *) &config->GetPersistentIntValue(Config::CategoryFreedbID, Config::FreedbEnableCacheID, Config::FreedbEnableCacheDefault));
	menu_database->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Manage CDDB cache entries")))->onAction.Connect(&freacGUI::ManageCDDBData, this);

	if (menu_drives->Length() >= 1)
	{
		menu_database->AddEntry();
		menu_database->AddEntry(i18n->TranslateString("Automatic CDDB queries"), (Bool *) &config->GetPersistentIntValue(Config::CategoryFreedbID, Config::FreedbAutoQueryID, Config::FreedbAutoQueryDefault));
	}

	menu_database_query->AddEntry(i18n->TranslateString("Query CDDB database"), ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/freedb/freedb-query.png")))->onAction.Connect(&freacGUI::QueryCDDB, this);
	menu_database_query->AddEntry(i18n->TranslateString("Query CDDB database later"))->onAction.Connect(&freacGUI::QueryCDDBLater, this);

	/* Fill options menu.
	 */
	i18n->SetContext("Menu::Options");

	entry = menu_options->AddEntry(i18n->AddEllipsis(i18n->TranslateString("General settings")), ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/settings/settings.png")));
	entry->onAction.Connect(&freacGUI::ConfigureSettings, this);
	entry->SetShortcut(SC_CONTROL | SC_SHIFT, Keyboard::KeyC, mainWnd);

	entry = menu_options->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Configure signal processing")), ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/settings/settings-dsp.png")));
	entry->onAction.Connect(&freacGUI::ConfigureProcessing, this);
	entry->SetShortcut(SC_CONTROL | SC_SHIFT, Keyboard::KeyD, mainWnd);

	entry = menu_options->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Configure selected encoder")), ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/settings/settings-codec.png")));
	entry->onAction.Connect(&freacGUI::ConfigureEncoder, this);
	entry->SetShortcut(SC_CONTROL | SC_SHIFT, Keyboard::KeyE, mainWnd);

	if (config->GetNOfConfigurations() > 1)
	{
		for (Int i = 0; i < config->GetNOfConfigurations(); i++)
		{
			menu_configurations->AddEntry(config->GetNthConfigurationName(i) == "default" ? i18n->TranslateString("Default configuration") : config->GetNthConfigurationName(i), &clicked_configuration, i)->onAction.Connect(&freacGUI::OnSelectConfiguration, this);

			if (config->GetNthConfigurationName(i) == config->GetConfigurationName()) clicked_configuration = i;
		}

		menu_options->AddEntry();
		menu_options->AddEntry(i18n->TranslateString("Active configuration"), menu_configurations);
	}

	if (menu_seldrive->Length() > 1)
	{
		menu_options->AddEntry();
		menu_options->AddEntry(i18n->TranslateString("Active CD-ROM drive"), ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/other/cd-drive.png")), menu_seldrive);
	}

	/* Fill processing menu.
	 */
	i18n->SetContext("Menu::Processing");

	entry = menu_processing->AddEntry(i18n->TranslateString("Enable signal processing"), (Bool *) &config->GetPersistentIntValue(Config::CategoryProcessingID, Config::ProcessingEnableProcessingID, Config::ProcessingEnableProcessingDefault));
	entry->onAction.Connect(&freacGUI::ToggleSignalProcessing, this);

	menu_processing->AddEntry(i18n->TranslateString("Enable processing during playback"), (Bool *) &config->GetPersistentIntValue(Config::CategoryProcessingID, Config::ProcessingProcessPlaybackID, Config::ProcessingProcessPlaybackDefault));

	const Array<String>	&components = config->GetStringValue(Config::CategoryProcessingID, Config::ProcessingComponentsID, Config::ProcessingComponentsDefault).Explode(",");

	foreach (const String &component, components)
	{
		AS::DSPComponent	*dsp = (AS::DSPComponent *) boca.CreateComponentByID(component);

		if (dsp == NIL) continue;

		menu_processors->AddEntry(dsp->GetName(), &clicked_processor, foreachindex)->onAction.Connect(&freacGUI::ConfigureProcessor, this);

		boca.DeleteComponent(dsp);
	}

	if (menu_processors->Length() > 0)
	{
		menu_processing->AddEntry();
		menu_processing->AddEntry(i18n->TranslateString("Selected signal processors"), menu_processors);
	}

	ToggleSignalProcessing();

	/* Fill conversion menu.
	 */
	i18n->SetContext("Menu::Encode");

	entry = menu_encode->AddEntry(i18n->TranslateString(currentConfig->deleteAfterEncoding ? "Start encoding (deleting original files)" : "Start encoding"), ImageLoader::Load(String(currentConfig->resourcesPath).Append(currentConfig->deleteAfterEncoding ? "icons/conversion/conversion-start-warning.png" : "icons/conversion/conversion-start.png")));
	entry->onAction.Connect(&freacGUI::Convert, this);
	entry->SetShortcut(SC_CONTROL, Keyboard::KeyE, mainWnd);

	menu_encode->AddEntry(i18n->TranslateString("Pause/resume encoding"), ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/conversion/conversion-pause.png")))->onAction.Connect(&freacGUI::PauseResumeEncoding, this);
	menu_encode->AddEntry(i18n->TranslateString("Stop encoding"), ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/conversion/conversion-stop.png")))->onAction.Connect(&freacGUI::StopEncoding, this);

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_ENCODER) continue;

		const Array<FileFormat *>	&formats = boca.GetComponentFormats(i);

		if (formats.Length() > 1)
		{
			PopupMenu	*menu_formats = new PopupMenu();

			for (Int j = 0; j < formats.Length(); j++)
			{
				FileFormat		*format	    = formats.GetNth(j);
				String			 name	    = format->GetName();
				const Array<String>	&extensions = format->GetExtensions();

				if (extensions.Length() > 0)
				{
					String	 extensionList;

					foreach (const String &extension, extensions) extensionList.Append(extensionList != NIL ? ", " : NIL).Append(".").Append(extension);

					name = i18n->AddBrackets(name, extensionList);
				}

				menu_formats->AddEntry(name, &clicked_encoder, 100 * i + j)->onAction.Connect(&freacGUI::Convert, this);
			}

			menu_encoders->AddEntry(boca.GetComponentName(i), menu_formats);

			formatMenus.Add(menu_formats);
		}
		else
		{
			menu_encoders->AddEntry(boca.GetComponentName(i), &clicked_encoder, 100 * i)->onAction.Connect(&freacGUI::Convert, this);
		}
	}

	if (boca.GetNumberOfComponentsOfType(COMPONENT_TYPE_ENCODER) > 0)
	{
		menu_encode->AddEntry();
		menu_encode->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Start encoding with")), menu_encoders);
	}

	menu_encoder_options->AddEntry(i18n->TranslateString("Encode to a single file"), (Bool *) &config->GetPersistentIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault));

	menu_encoder_options->AddEntry();
	menu_encoder_options->AddEntry(i18n->TranslateString("Encode to input file folder if possible"), (Bool *) &config->GetPersistentIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault))->onAction.Connect(&freacGUI::ToggleUseInputDirectory, this);

	allowOverwriteMenuEntry = menu_encoder_options->AddEntry(i18n->TranslateString("Allow overwriting input file"), (Bool *) &config->GetPersistentIntValue(Config::CategorySettingsID, Config::SettingsAllowOverwriteSourceID, Config::SettingsAllowOverwriteSourceDefault));

	menu_encoder_options->AddEntry();
	menu_encoder_options->AddEntry(i18n->TranslateString("Delete original files after encoding"), &currentConfig->deleteAfterEncoding)->onAction.Connect(&freacGUI::ConfirmDeleteAfterEncoding, this);

	menu_encoder_options->AddEntry();
	menu_encoder_options->AddEntry(i18n->TranslateString("Shutdown after encoding"), &currentConfig->shutdownAfterEncoding);

	menu_encode->AddEntry();
	menu_encode->AddEntry(i18n->TranslateString("Encoder options"), ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/settings/settings-codec.png")), menu_encoder_options);

	ToggleUseInputDirectory();

	/* Fill help menu.
	 */
	i18n->SetContext("Menu::Help");

	entry = menu_help->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Help topics")), ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/other/help.png")));
	entry->onAction.Connect(&freacGUI::ShowHelp, this);
	entry->SetShortcut(0, Keyboard::KeyF1, mainWnd);

	menu_help->AddEntry();

	entry = menu_help->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Show Tip of the Day")));
	entry->onAction.Connect(&freacGUI::ShowTipOfTheDay, this);
	entry->SetShortcut(0, Keyboard::KeyF10, mainWnd);

	menu_help->AddEntry();

	entry = menu_help->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Report an issue")));
	entry->onAction.Connect(&freacGUI::ReportIssue, this);

	entry = menu_help->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Suggest a feature")));
	entry->onAction.Connect(&freacGUI::SuggestFeature, this);

	if (currentConfig->enable_eUpdate)
	{
		menu_help->AddEntry();
		menu_help->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Check for updates now")))->onAction.Connect(&freacGUI::CheckForUpdates, this);
		menu_help->AddEntry(i18n->TranslateString("Check for updates at startup"), (Bool *) &config->GetPersistentIntValue(Config::CategorySettingsID, Config::SettingsCheckForUpdatesID, Config::SettingsCheckForUpdatesDefault));
	}

	menu_help->AddEntry();
	menu_help->AddEntry(i18n->AddEllipsis(i18n->TranslateString("About %1").Replace("%1", freac::appName)), ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/other/info.png")))->onAction.Connect(&freacGUI::About, this);

	/* Add entries to menubar.
	 */
	i18n->SetContext("Menu");

	mainWnd_menubar->RemoveAllEntries();

	mainWnd_menubar->AddEntry(i18n->TranslateString("File"), menu_file);
	mainWnd_menubar->AddEntry(i18n->TranslateString("Database"), menu_database);
	mainWnd_menubar->AddEntry(i18n->TranslateString("Options"), menu_options);
	mainWnd_menubar->AddEntry(i18n->TranslateString("Processing"), menu_processing);
	mainWnd_menubar->AddEntry(i18n->TranslateString("Encode"), menu_encode);

	mainWnd_menubar->AddEntry(i18n->TranslateString("Help"), menu_help)->SetOrientation(OR_RIGHT);

	/* Fill toolbar.
	 */
	i18n->SetContext("Toolbar");

	mainWnd_iconbar->RemoveAllEntries();

	entry = mainWnd_iconbar->AddEntry(ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/joblist/joblist-addfiles.png")), menu_files);
	entry->onAction.Connect(&JobList::AddTrackByDialog, joblist);
	entry->SetTooltipText(i18n->TranslateString("Add audio file(s) to the joblist"));

	if (menu_drives->Length() >= 1)
	{
		entry = mainWnd_iconbar->AddEntry(ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/joblist/joblist-adddisc.png")), menu_drives->Length() > 1 ? menu_drives : NIL);
		entry->onAction.Connect(&freacGUI::ReadCD, this);
		entry->SetTooltipText(i18n->TranslateString("Add audio CD contents to the joblist"));
	}

	entry = mainWnd_iconbar->AddEntry(ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/joblist/joblist-remove.png")));
	entry->onAction.Connect(&JobList::RemoveSelectedTrack, joblist);
	entry->SetTooltipText(i18n->TranslateString("Remove the selected entry from the joblist"));

	entry = mainWnd_iconbar->AddEntry(ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/joblist/joblist-clear.png")));
	entry->onAction.Connect(&JobList::StartJobRemoveAllTracks, joblist);
	entry->SetTooltipText(i18n->TranslateString("Clear the entire joblist"));

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/freedb/freedb-query.png")), menu_drives->Length() >= 1 ? menu_database_query : NIL);
	entry->onAction.Connect(&freacGUI::QueryCDDB, this);
	entry->SetTooltipText(i18n->TranslateString("Query CDDB database"));

	if (menu_drives->Length() >= 1)
	{
		entry = mainWnd_iconbar->AddEntry(ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/freedb/freedb-edit.png")));
		entry->onAction.Connect(&freacGUI::SubmitCDDBData, this);
		entry->SetTooltipText(i18n->AddEllipsis(i18n->TranslateString("Submit CDDB data")));
	}

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/settings/settings.png")), config->GetNOfConfigurations() > 1 ? menu_configurations : NIL);
	entry->onAction.Connect(&freacGUI::ConfigureSettings, this);
	entry->SetTooltipText(i18n->TranslateString("Configure general settings"));

	entry = mainWnd_iconbar->AddEntry(ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/settings/settings-dsp.png")), menu_processors->Length() > 0 ? menu_processors : NIL);
	entry->onAction.Connect(&freacGUI::ConfigureProcessing, this);
	entry->SetTooltipText(i18n->TranslateString("Configure signal processing"));

	entry = mainWnd_iconbar->AddEntry(ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/settings/settings-codec.png")));
	entry->onAction.Connect(&freacGUI::ConfigureEncoder, this);
	entry->SetTooltipText(i18n->TranslateString("Configure the selected audio encoder"));

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(ImageLoader::Load(String(currentConfig->resourcesPath).Append(currentConfig->deleteAfterEncoding ? "icons/conversion/conversion-start-warning.png" : "icons/conversion/conversion-start.png")), boca.GetNumberOfComponentsOfType(COMPONENT_TYPE_ENCODER) > 0 ? menu_encoders : NIL);
	entry->onAction.Connect(&freacGUI::Convert, this);
	entry->SetTooltipText(i18n->TranslateString(currentConfig->deleteAfterEncoding ? "Start the encoding process (deleting original files)" : "Start the encoding process"));

	entry = mainWnd_iconbar->AddEntry(ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/conversion/conversion-pause.png")));
	entry->onAction.Connect(&freacGUI::PauseResumeEncoding, this);
	entry->SetTooltipText(i18n->TranslateString("Pause/resume encoding"));

	entry = mainWnd_iconbar->AddEntry(ImageLoader::Load(String(currentConfig->resourcesPath).Append("icons/conversion/conversion-stop.png")));
	entry->onAction.Connect(&freacGUI::StopEncoding, this);
	entry->SetTooltipText(i18n->TranslateString("Stop encoding"));

	/* Set size of iconbar entries to 28x28 pixels.
	 */
	for (Int i = 0; i < mainWnd_iconbar->GetNOfObjects(); i++)
	{
 		if (mainWnd_iconbar->GetNthObject(i)->GetObjectType() != MenuEntry::classID) continue;

		entry = (MenuEntry *) mainWnd_iconbar->GetNthObject(i);

		if (entry->GetBitmap() != NIL) entry->SetSize(Size(28 + (entry->GetPopupMenu() != NIL ? 12 : 0), 28));
	}

	mainWnd_iconbar->SetHeight(30);

	/* Emit overlay signals.
	 */
	BoCA::Menu	*menu = BoCA::Menu::Get();

	menu->doMenubarOverlay.Emit(mainWnd_menubar);
	menu->doIconbarOverlay.Emit(mainWnd_iconbar);

	/* Show menu bars.
	 */
	mainWnd_menubar->Show();
	mainWnd_iconbar->Show();

	hyperlink->Hide();
	hyperlink->Show();

	surface->EndPaint();
}

Void freac::freacGUI::Convert()
{
	/* Resume paused conversions if any.
	 */
	if (JobConvert::IsConverting() && JobConvert::IsPaused() && clicked_encoder == -1)
	{
		JobConvert::Resume();

		return;
	}

	/* If a specific encoder was selected, activate it.
	 */
	if (clicked_encoder >= 0)
	{
		BoCA::Config		*config	   = BoCA::Config::Get();
		Registry		&boca	   = Registry::Get();
		const String		&encoderID = boca.GetComponentID(clicked_encoder / 100);
		EncoderComponent	*encoder   = (EncoderComponent *) boca.CreateComponentByID(encoderID);

		if (encoder != NIL)
		{
			encoder->SetOutputFormat(clicked_encoder % 100);

			boca.DeleteComponent(encoder);
		}

		config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, encoderID);

		tab_layer_joblist->UpdateEncoderText();

		/* Reset selected encoder.
		 */
		clicked_encoder = -1;

		OptionBox::internalCheckValues.Emit();
	}

	/* Return if there are no tracks to convert.
	 */
	if (joblist->Length() == 0) return;

	/* Detect accidental double clicks.
	 */
	static UnsignedInt64	 previousTicks = 0;

	if (S::System::System::Clock() - previousTicks < 250) return;

	/* Create array of tracks to convert.
	 */
	Array<Track>	 tracks;

	for (Int i = 0; i < joblist->GetNOfTracks(); i++)
	{
		if (!joblist->GetNthEntry(i)->IsMarked()) continue;

		tracks.Add(joblist->GetNthTrack(i));
	}

	/* Start conversion.
	 */
	Converter().Convert(tracks);

	previousTicks = S::System::System::Clock();
}

Void freac::freacGUI::PauseResumeEncoding()
{
	if (!JobConvert::IsConverting()) return;

	if (JobConvert::IsPaused()) JobConvert::Resume();
	else			    JobConvert::Pause();
}

Void freac::freacGUI::StopEncoding()
{
	JobConvert::Stop();
}

Void freac::freacGUI::AddFilesByPattern()
{
	AddPatternDialog	 dialog;

	if (dialog.ShowDialog() == Success()) joblist->AddTracksByPattern(dialog.GetDirectory(), dialog.GetPattern(), dialog.GetSearchSubFolders());
}

Void freac::freacGUI::AddFilesFromDirectory()
{
	AddDirectoryDialog	 dialog;

	if (dialog.ShowDialog() == Success())
	{
		Array<String>	 directories;

		directories.Add(dialog.GetDirectory());

		joblist->AddTracksByDragAndDrop(directories);
	}
}

Void freac::freacGUI::ToggleSignalProcessing()
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (config->GetIntValue(Config::CategoryProcessingID, Config::ProcessingEnableProcessingID, Config::ProcessingEnableProcessingDefault)) menu_processing->GetNthEntry(1)->Activate();
	else																	menu_processing->GetNthEntry(1)->Deactivate();
}

Void freac::freacGUI::ToggleUseInputDirectory()
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault)) allowOverwriteMenuEntry->Activate();
	else																	    allowOverwriteMenuEntry->Deactivate();
}

Void freac::freacGUI::ToggleEncodeToSingleFile()
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault)) config->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, True);
}

Void freac::freacGUI::ConfirmDeleteAfterEncoding()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Messages");

	Bool	 keepOptionEnabled = config->GetIntValue(Config::CategorySettingsID, Config::SettingsDeleteAfterEncodingID, Config::SettingsDeleteAfterEncodingDefault);

	if (currentConfig->deleteAfterEncoding)
	{
		MessageDlg	 messageBox(i18n->TranslateString("This option will remove the original files from your computer\nafter the encoding process!\n\nAre you sure you want to activate this function?"), i18n->TranslateString("Delete original files after encoding"), Message::Buttons::YesNo, Message::Icon::Question, i18n->TranslateString("Keep this option enabled even after restarting %1").Replace("%1", appName), &keepOptionEnabled);

		messageBox.ShowDialog();

		if (messageBox.GetButtonCode() == Message::Button::No) currentConfig->deleteAfterEncoding = False;
	}

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsDeleteAfterEncodingID, currentConfig->deleteAfterEncoding && keepOptionEnabled);

	FillMenus();
}

Void freac::freacGUI::ShowHelp()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Menu::Help");

	S::System::System::OpenURL(String("file://").Append(currentConfig->documentationPath).Append("manual/").Append(i18n->TranslateString("index_en.html")));
}

Void freac::freacGUI::ShowTipOfTheDay()
{
	BoCA::Config	*config   = BoCA::Config::Get();
	BoCA::I18n	*i18n	  = BoCA::I18n::Get();

	Bool		 showTips = config->GetIntValue(Config::CategorySettingsID, Config::SettingsShowTipsID, Config::SettingsShowTipsDefault);
	String		 language = config->GetStringValue(Config::CategorySettingsID, Config::SettingsLanguageID, Config::SettingsLanguageDefault);

	TipOfTheDay	 dialog(&showTips);
	Translator	 tips("Tips");

	tips.ActivateLanguage(String(language).Replace("freac", "tips"));

	dialog.AddTip(tips.TranslateString("%1 comes with support for a wide variety of audio formats\nincluding MP3, M4A/AAC, FLAC, ALAC, Opus, Ogg Vorbis and\nWMA.\n\nAdditional formats can be added by installing command line\nencoders.\n\nPlease refer to %2\nfor details.").Replace("%1", freac::appName).Replace("%2", "https://freac.org/manual/en/howto.html#codecs"));
	dialog.AddTip(tips.TranslateString("%1 can combine multiple input files into a single output file.\n\nWhen loading such files back into %1 or another software\nsupporting chapters, the original tracks will show up there again.\n\nSelect the 'Encode to a single file' option to enable this mode.").Replace("%1", freac::appName));
	dialog.AddTip(tips.TranslateString("%1 is optimized to take advantage of modern multi-core\nprocessors and will convert multiple files in parallel to speed up\nconversions.\n\nIn addition, %1's unique SuperFast technology speeds up\nsingle file conversions for select target formats.").Replace("%1", freac::appName));
	dialog.AddTip(tips.TranslateString("%1 will verify lossless source formats like FLAC and alert you if\nsomething does not seem to be right.\n\nIn addition, it can verify lossless output files to make sure nothing\nwent wrong during the conversion process.\n\nCheck your settings in the configuration dialog if you would like\nto enable this.").Replace("%1", freac::appName));
	dialog.AddTip(tips.TranslateString("%1's gapless encoding and decoding support ensures that your\nMP3s and other files start and stop at exactly the right sample.\n\nThis means perfect quality when advancing to the next song of a\ncontinuously recorded album without any clicks or pops.").Replace("%1", freac::appName));
	dialog.AddTip(tips.TranslateString("%1 can apply filters to your audio during conversions to reduce\nnoise in voice recordings or speed up/slow down a record without\nchanging the pitch which is great for speed listening to audio\nbooks.\n\nCheck the 'Processing' page of the configuration dialog for these\nfilters.").Replace("%1", freac::appName));
	dialog.AddTip(tips.TranslateString("The 'meh! - multi encoder hub' component can convert to multiple\noutput formats at once.\n\nUsing it, you can create e.g. FLAC and MP3 files of the same source\nin one go."));

#if defined __WIN32__
	dialog.AddTip(tips.TranslateString("%1 can use the Core Audio AAC/ALAC encoder included with\niTunes if that program is installed on your computer.\n\nThe Core Audio encoder provides great M4A/AAC encoding\nquality and performance.\n\nPlease refer to %2 for installing iTunes.", "Windows").Replace("%1", freac::appName).Replace("%2", "https://apple.com/itunes"));

#if defined __i386__ || defined _M_IX86
	dialog.AddTip(tips.TranslateString("%1 can use Winamp 2 input plug-ins to support more file\nformats.\n\nCopy the in_*.dll files to the %1/plugins directory to enable\n%1 to read these formats.", "Windows").Replace("%1", freac::appName));
#endif
#endif

#if defined __linux__ || defined __FreeBSD__
#if defined __i386__ || defined __x86_64__
	dialog.AddTip(tips.TranslateString("%1 can use the Core Audio AAC/ALAC encoder if Wine and the\nApple Application Support package are installed on your computer.\n\nThe Core Audio encoder provides great M4A/AAC encoding\nquality and performance.\n\nFor instructions on installing this encoder, please refer to the tutorial\nat: %2", "Linux").Replace("%1", freac::appName).Replace("%2", "https://freac.org/manual/en/howto.html#coreaudio"));
#endif
#endif

	if (menu_drives->Length() >= 1)
	{
		dialog.AddTip(tips.TranslateString("%1 supports the freedb CD database to query artist and title\ninformation about CDs.\n\nIf you have a CD that is not in the database, you can submit new\nfreedb entries with %1.").Replace("%1", freac::appName));
		dialog.AddTip(tips.TranslateString("To correct reading errors while ripping, you can enable Jitter\ncorrection in the Ripper tab of %1's configuration dialog.\n\nIf that does not help, try using one of the Paranoia modes.").Replace("%1", freac::appName));
	}

	dialog.AddTip(tips.TranslateString("%1 is available in %2 languages. If your language is not available\nor the translation is incomplete, you can easily translate %1\nusing the \'smooth Translator\' utility.\n\nTo contribute your translations back to the %1 project, please\nrefer to %3 for additional details.").Replace("%1", freac::appName).Replace("%2", String::FromInt(Math::Max(36, i18n->GetNOfLanguages()))).Replace("%3", "https://freac.org/translations"));
	dialog.AddTip(tips.TranslateString("Do you have suggestions on how to improve %1?\n\nYou can submit your ideas through the tracker on %1's %2\nproject page at %3 or send\nan eMail to %4.").Replace("%1", freac::appName).Replace("%2", "GitHub").Replace("%3", "https://github.com/enzo1982/freac/issues").Replace("%4", "suggestions@freac.org"));
	dialog.AddTip(tips.TranslateString("Do you like %1? %1 is free software, but you can help fund\nthe development by making a donation.\n\nDonations can be sent via PayPal to %2.\n\nOr select 'Help'->'Donate to the %1 project...' from the menu\nbar to see different ways to donate.").Replace("%1", freac::appName).Replace("%2", "donate@freac.org"));

	dialog.SetMode(TIP_ORDERED, config->GetIntValue(Config::CategorySettingsID, Config::SettingsNextTipID, Config::SettingsNextTipDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsShowTipsID, Config::SettingsShowTipsDefault));

	for (Int i = 0; i < dialog.GetNOfObjects(); i++)
	{
		if (dialog.GetNthObject(i)->GetObjectType() != Window::classID) continue;

		Window	*dlgWnd = (Window *) dialog.GetNthObject(i);

		dlgWnd->SetRightToLeft(tips.IsActiveLanguageRightToLeft());

		break;
	}

	dialog.ShowDialog();

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsShowTipsID, showTips);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsNextTipID, dialog.GetOffset());
}

Void freac::freacGUI::ReportIssue()
{
	S::System::System::OpenURL(issueTracker);
}

Void freac::freacGUI::SuggestFeature()
{
	S::System::System::OpenURL(featureTracker);
}

Void freac::freacGUI::CheckForUpdates()
{
	(new JobCheckForUpdates(False))->Schedule();
}
