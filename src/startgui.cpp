 /* fre:ac - free audio converter
  * Copyright (C) 2001-2017 Robert Kausch <robert.kausch@freac.org>
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

	config->enable_console = False;

	/* Setup Notification instance.
	 */
	notification = Notification::Get();

	if (notification != NIL)
	{
		notification->onDriveChange.Connect(&freacGUI::OnDriveChange, this);
		notification->onDiscInsert.Connect(&freacGUI::OnDiscInsert, this);
		notification->onDiscRemove.Connect(&freacGUI::OnDiscRemove, this);
	}

	/* Set interface language.
	 */
	String	 language = config->GetStringValue(Config::CategorySettingsID, Config::SettingsLanguageID, Config::SettingsLanguageDefault);

	if (language != NIL) i18n->ActivateLanguage(language);
	else		     i18n->SelectUserDefaultLanguage();

	config->SetStringValue(Config::CategorySettingsID, Config::SettingsLanguageID, i18n->GetActiveLanguageID());

	/* Setup attributes.
	 */
	clicked_configuration = -1;
	clicked_drive	      = -1;
	clicked_encoder	      = -1;

	Rect	 workArea = Screen::GetVirtualScreenMetrics();

	Point	 wndPos = Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault));
	Size	 wndSize = Size(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowSizeXID, Config::SettingsWindowSizeXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowSizeYID, Config::SettingsWindowSizeYDefault));

	if (wndPos.x + wndSize.cx > workArea.right + 2 ||
	    wndPos.y + wndSize.cy > workArea.bottom + 2)
	{
		wndPos.x = (Int) Math::Min(workArea.right - 10 - wndSize.cx, wndPos.x);
		wndPos.y = (Int) Math::Min(workArea.bottom - 10 - wndSize.cy, wndPos.y);
	}

	if (wndPos.x < workArea.left - 2 ||
	    wndPos.y < workArea.top - 2)
	{
		wndPos.x = (Int) Math::Max(workArea.left + 10, wndPos.x);
		wndPos.y = (Int) Math::Max(workArea.top + 10, wndPos.y);

		wndSize.cx = (Int) Math::Min(workArea.right - 20, wndSize.cx);
		wndSize.cy = (Int) Math::Min(workArea.bottom - 20, wndSize.cy);
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

	menu_encode		= new PopupMenu();
	menu_encoders		= new PopupMenu();
	menu_encoder_options	= new PopupMenu();

	menu_help		= new PopupMenu();

	hyperlink		= new Hyperlink(String(freac::website).Replace("https://", NIL).Replace("/", NIL), NIL, freac::website, Point(91, -22));
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

	for (Int i = 0; i < extensionComponents.Length(); i++)
	{
		Layer	*mainTabLayer	= extensionComponents.GetNth(i)->getMainTabLayer.Emit();
		Layer	*statusBarLayer = extensionComponents.GetNth(i)->getStatusBarLayer.Emit();

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

	mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));

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

	JobConvert::onStartEncoding.Connect(&LayerJoblist::OnEncoderStartEncoding, tab_layer_joblist);
	JobConvert::onFinishEncoding.Connect(&LayerJoblist::OnEncoderFinishEncoding, tab_layer_joblist);

	JobConvert::onEncodeTrack.Connect(&LayerJoblist::OnEncoderEncodeTrack, tab_layer_joblist);

	JobConvert::onTrackProgress.Connect(&LayerJoblist::OnEncoderTrackProgress, tab_layer_joblist);
	JobConvert::onTotalProgress.Connect(&LayerJoblist::OnEncoderTotalProgress, tab_layer_joblist);

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

	DeleteObject(menu_encode);
	DeleteObject(menu_encoders);
	DeleteObject(menu_encoder_options);

	DeleteObject(menu_help);

	foreach (PopupMenu *menu_format, formatMenus) delete menu_format;

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

	for (Int i = 0; i < extensionComponents.Length(); i++)
	{
		boca.DeleteComponent(extensionComponents.GetNth(i));
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

	QuickMessage(String(freac::appLongName).Append(" ").Append(freac::version).Append(" (").Append(freac::architecture).Append(")\n").Append(freac::copyright).Append("\n\n").Append(i18n->TranslateString("Translated by %1.").Replace("%1", i18n->GetActiveLanguageAuthor())).Append("\n\n").Append(i18n->TranslateString("This program is being distributed under the terms\nof the GNU General Public License (GPL).")), i18n->TranslateString("About %1").Replace("%1", freac::appName), Message::Buttons::Ok, (wchar_t *) IDI_ICON);
}

Void freac::freacGUI::ConfigureEncoder()
{
	BoCA::Config	*config	= BoCA::Config::Get();

	Registry	&boca = Registry::Get();
	Component	*component = boca.CreateComponentByID(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault));

	if (component != NIL)
	{
		ConfigLayer	*layer = component->GetConfigurationLayer();

		if (layer != NIL)
		{
			ConfigComponentDialog	*dlg = new ConfigComponentDialog(layer);

			dlg->SetParentWindow(mainWnd);
			dlg->ShowDialog();

			DeleteObject(dlg);
		}
		else
		{
			BoCA::Utilities::ErrorMessage("No configuration dialog available for:\n\n%1", component->GetName());
		}

		boca.DeleteComponent(component);
	}
}

Void freac::freacGUI::ConfigureSettings()
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
	ConfigDialog	*dialog	 = new ConfigDialog();

	dialog->ShowDialog();

	DeleteObject(dialog);

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

	/* Show/hide Jobs tab.
	 */
	if (config->GetIntValue(Config::CategorySettingsID,
				Config::SettingsShowJobsTabID,
				Config::SettingsShowJobsTabDefault)) tabs_main->Add(tab_layer_threads);
	else							     tabs_main->Remove(tab_layer_threads);

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

		Job	*job = new JobAddTracks(urls, autoCDRead);

		if (config->GetIntValue(Config::CategoryRipperID, Config::RipperAutoRipID, Config::RipperAutoRipDefault)) job->onFinish.Connect(&freacGUI::Convert, this);

		job->Schedule();

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
				cddbQueryDlg	*dlg = new cddbQueryDlg(queryString);

				if (dlg->ShowDialog() == Error())
				{
					/* Ask whether to perform this query later.
					 */
					CDDBBatch	*queries = new CDDBBatch();

					if (i == queryStrings.Length() - 1 && QuickMessage(dlg->GetErrorString().Append("\n\n").Append(i18n->TranslateString("Would you like to perform this query again later?", "CDDB::Query::Errors")), i18n->TranslateString("Error"), Message::Buttons::YesNo, Message::Icon::Error) == Message::Button::Yes)
					{
						queries->AddQuery(queryString);
					}
					else if (i < queryStrings.Length() - 1 && QuickMessage(dlg->GetErrorString().Append("\n\n").Append(i18n->TranslateString("Would you like to perform the remaining queries again later?", "CDDB::Query::Errors")), i18n->TranslateString("Error"), Message::Buttons::YesNo, Message::Icon::Error) == Message::Button::Yes)
					{
						for (Int j = i; j < queryStrings.Length(); j++) queries->AddQuery(queryStrings.GetNth(j));
					}

					delete queries;

					/* Temporarily disable remote CDDB queries if connection failed.
					 */
					config->SetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, False);
				}
				else if (dlg->GetErrorString() != NIL)
				{
					/* If there's an info message, add it to the list of errors.
					 */
					queryErrors.Add(dlg->GetErrorString());
				}

				cdInfo = dlg->GetCDDBInfo();

				DeleteObject(dlg);
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

				info.artist	= (cdInfo.dArtist == "Various" ? cdInfo.trackArtists.GetNth(trackNumber - 1) : cdInfo.dArtist);
				info.title	= cdInfo.trackTitles.GetNth(trackNumber - 1);
				info.album	= cdInfo.dTitle;
				info.genre	= cdInfo.dGenre;
				info.year	= cdInfo.dYear;
				info.track	= trackNumber;

				track.SetInfo(info);
				track.SetOriginalInfo(info);

				track.outfile	= NIL;

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
		CDDBBatch	*queries = new CDDBBatch();

		foreach (const String &queryString, queryStrings) queries->AddQuery(queryString);

		delete queries;
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

	cddbSubmitDlg	*dlg = new cddbSubmitDlg();

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Void freac::freacGUI::ManageCDDBData()
{
	cddbManageDlg	*dlg = new cddbManageDlg();

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Void freac::freacGUI::ManageCDDBBatchData()
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (!config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, Config::FreedbEnableRemoteDefault))
	{
		BoCA::Utilities::ErrorMessage("Remote CDDB support is disabled! Please enable\nremote CDDB support in the configuration dialog.");

		return;
	}

	cddbManageSubmitsDlg	*dlg = new cddbManageSubmitsDlg();

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Void freac::freacGUI::ManageCDDBBatchQueries()
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (!config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, Config::FreedbEnableRemoteDefault))
	{
		BoCA::Utilities::ErrorMessage("Remote CDDB support is disabled! Please enable\nremote CDDB support in the configuration dialog.");

		return;
	}

	cddbManageQueriesDlg	*dlg = new cddbManageQueriesDlg();

	dlg->ShowDialog();

	DeleteObject(dlg);
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

	surface->StartPaint(Rect(Point(0, 0), Size(mainWnd_iconbar->GetX(), mainWnd_iconbar->GetY()) + mainWnd_iconbar->GetSize()));

	mainWnd_menubar->Hide();
	mainWnd_iconbar->Hide();

	menu_file->RemoveAllEntries();
	menu_addsubmenu->RemoveAllEntries();
	menu_files->RemoveAllEntries();
	menu_drives->RemoveAllEntries();

	menu_database->RemoveAllEntries();
	menu_database_query->RemoveAllEntries();

	menu_options->RemoveAllEntries();
	menu_configurations->RemoveAllEntries();
	menu_seldrive->RemoveAllEntries();

	menu_encode->RemoveAllEntries();
	menu_encoders->RemoveAllEntries();
	menu_encoder_options->RemoveAllEntries();

	menu_help->RemoveAllEntries();

	foreach (PopupMenu *menu_format, formatMenus) delete menu_format;

	formatMenus.RemoveAll();

	MenuEntry	*entry	     = NIL;
	Config		*freacConfig = Config::Get();

	i18n->SetContext("Menu::File");

	menu_file->AddEntry(i18n->TranslateString("Add"), ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:21")), menu_addsubmenu);

	entry = menu_file->AddEntry(i18n->TranslateString("Remove"), ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:24")));
	entry->onAction.Connect(&JobList::RemoveSelectedTrack, joblist);
	entry->SetShortcut(SC_CONTROL, Keyboard::KeyR, mainWnd);

	if (boca.GetNumberOfComponentsOfType(COMPONENT_TYPE_PLAYLIST) > 1 || (boca.GetNumberOfComponentsOfType(COMPONENT_TYPE_PLAYLIST) > 0 && !boca.ComponentExists("cuesheet-playlist")))
	{
		menu_file->AddEntry();

		menu_file->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Load joblist")))->onAction.Connect(&JobList::LoadList, joblist);
		menu_file->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Save joblist")))->onAction.Connect(&JobList::SaveList, joblist);
	}

	menu_file->AddEntry();

	entry = menu_file->AddEntry(i18n->TranslateString("Clear joblist"), ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:25")));
	entry->onAction.Connect(&JobList::StartJobRemoveAllTracks, joblist);
	entry->SetShortcut(SC_CONTROL | SC_SHIFT, Keyboard::KeyR, mainWnd);

	menu_file->AddEntry();

	entry = menu_file->AddEntry(i18n->TranslateString("Exit"), ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:36")));
	entry->onAction.Connect(&freacGUI::Close, this);

#ifdef __WIN32__
	entry->SetShortcut(SC_ALT, Keyboard::KeyF4, mainWnd);
#else
	entry->SetShortcut(SC_CONTROL, Keyboard::KeyQ, mainWnd);
#endif

	entry = menu_addsubmenu->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Audio file(s)")), ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:22")));
	entry->onAction.Connect(&JobList::AddTrackByDialog, joblist);
	entry->SetShortcut(SC_CONTROL, Keyboard::KeyF, mainWnd);

	DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

	if (info != NIL)
	{
		for (Int i = 0; i < info->GetNumberOfDevices(); i++)
		{
			const Device	&device = info->GetNthDeviceInfo(i);
			String		 name	= String(device.vendor).Append(" ").Append(device.model).Append(" ").Append(device.revision).Trim();

			menu_drives->AddEntry(name, NIL, NIL, NIL, &clicked_drive, i)->onAction.Connect(&freacGUI::ReadCD, this);
			menu_seldrive->AddEntry(name, NIL, NIL, NIL, &config->GetPersistentIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault), i);
		}

		boca.DeleteComponent(info);
	}

	if (menu_drives->GetNOfEntries() >= 1)
	{
		entry = menu_addsubmenu->AddEntry(i18n->TranslateString("Audio CD contents"), ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:23")));
		entry->onAction.Connect(&freacGUI::ReadCD, this);
		entry->SetShortcut(SC_CONTROL, Keyboard::KeyD, mainWnd);
	}

	menu_files->AddEntry(i18n->AddEllipsis(i18n->TranslateString("By pattern")))->onAction.Connect(&freacGUI::AddFilesByPattern, this);
	menu_files->AddEntry(i18n->AddEllipsis(i18n->TranslateString("From folder")))->onAction.Connect(&freacGUI::AddFilesFromDirectory, this);

	menu_addsubmenu->AddEntry();
	menu_addsubmenu->AddEntry(i18n->TranslateString("Audio file(s)"), NIL, menu_files);

	if (menu_drives->GetNOfEntries() > 1)
	{
		menu_addsubmenu->AddEntry(i18n->TranslateString("Audio CD contents"), NIL, menu_drives);
	}

	i18n->SetContext("Menu::Database");

	entry = menu_database->AddEntry(i18n->TranslateString("Query CDDB database"), ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:26")));
	entry->onAction.Connect(&freacGUI::QueryCDDB, this);
	entry->SetShortcut(SC_CONTROL | SC_SHIFT, Keyboard::KeyQ, mainWnd);

	entry = menu_database->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Submit CDDB data")), ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:27")));
	entry->onAction.Connect(&freacGUI::SubmitCDDBData, this);
	entry->SetShortcut(SC_CONTROL | SC_SHIFT, Keyboard::KeyS, mainWnd);

	menu_database->AddEntry();

	menu_database->AddEntry(i18n->TranslateString("Query CDDB database later"))->onAction.Connect(&freacGUI::QueryCDDBLater, this);

	menu_database->AddEntry();

	menu_database->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Show queued CDDB entries")))->onAction.Connect(&freacGUI::ManageCDDBBatchData, this);
	menu_database->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Show queued CDDB queries")))->onAction.Connect(&freacGUI::ManageCDDBBatchQueries, this);

	menu_database->AddEntry();

	menu_database->AddEntry(i18n->TranslateString("Enable CDDB cache"), NIL, NIL, (Bool *) &config->GetPersistentIntValue(Config::CategoryFreedbID, Config::FreedbEnableCacheID, Config::FreedbEnableCacheDefault));
	menu_database->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Manage CDDB cache entries")))->onAction.Connect(&freacGUI::ManageCDDBData, this);

	menu_database->AddEntry();

	menu_database->AddEntry(i18n->TranslateString("Automatic CDDB queries"), NIL, NIL, (Bool *) &config->GetPersistentIntValue(Config::CategoryFreedbID, Config::FreedbAutoQueryID, Config::FreedbAutoQueryDefault));

	menu_database_query->AddEntry(i18n->TranslateString("Query CDDB database"), ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:26")))->onAction.Connect(&freacGUI::QueryCDDB, this);
	menu_database_query->AddEntry(i18n->TranslateString("Query CDDB database later"))->onAction.Connect(&freacGUI::QueryCDDBLater, this);

	i18n->SetContext("Menu::Options");

	entry = menu_options->AddEntry(i18n->AddEllipsis(i18n->TranslateString("General settings")), ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:28")));
	entry->onAction.Connect(&freacGUI::ConfigureSettings, this);
	entry->SetShortcut(SC_CONTROL | SC_SHIFT, Keyboard::KeyC, mainWnd);

	entry = menu_options->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Configure selected encoder")), ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:29")));
	entry->onAction.Connect(&freacGUI::ConfigureEncoder, this);
	entry->SetShortcut(SC_CONTROL | SC_SHIFT, Keyboard::KeyE, mainWnd);

	if (config->GetNOfConfigurations() > 1)
	{
		for (Int i = 0; i < config->GetNOfConfigurations(); i++)
		{
			menu_configurations->AddEntry(config->GetNthConfigurationName(i) == "default" ? i18n->TranslateString("Default configuration") : config->GetNthConfigurationName(i), NIL, NIL, NIL, &clicked_configuration, i)->onAction.Connect(&freacGUI::OnSelectConfiguration, this);

			if (config->GetNthConfigurationName(i) == config->GetConfigurationName()) clicked_configuration = i;
		}

		menu_options->AddEntry();
		menu_options->AddEntry(i18n->TranslateString("Active configuration"), NIL, menu_configurations);
	}

	if (menu_seldrive->GetNOfEntries() > 1)
	{
		menu_options->AddEntry();
		menu_options->AddEntry(i18n->TranslateString("Active CD-ROM drive"), ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:30")), menu_seldrive);
	}

	i18n->SetContext("Menu::Encode");

	entry = menu_encode->AddEntry(i18n->TranslateString("Start encoding"), ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:31")));
	entry->onAction.Connect(&freacGUI::Convert, this);
	entry->SetShortcut(SC_CONTROL, Keyboard::KeyE, mainWnd);

	menu_encode->AddEntry(i18n->TranslateString("Pause/resume encoding"), ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:32")))->onAction.Connect(&freacGUI::PauseResumeEncoding, this);
	menu_encode->AddEntry(i18n->TranslateString("Stop encoding"), ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:33")))->onAction.Connect(&freacGUI::StopEncoding, this);

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

				menu_formats->AddEntry(name, NIL, NIL, NIL, &clicked_encoder, 100 * i + j)->onAction.Connect(&freacGUI::Convert, this);
			}

			menu_encoders->AddEntry(boca.GetComponentName(i), NIL, menu_formats);

			formatMenus.Add(menu_formats);
		}
		else
		{
			menu_encoders->AddEntry(boca.GetComponentName(i), NIL, NIL, NIL, &clicked_encoder, 100 * i)->onAction.Connect(&freacGUI::Convert, this);
		}
	}

	if (boca.GetNumberOfComponentsOfType(COMPONENT_TYPE_ENCODER) > 0)
	{
		menu_encode->AddEntry();
		menu_encode->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Start encoding with")), NIL, menu_encoders);
	}

	menu_encoder_options->AddEntry(i18n->TranslateString("Encode to a single file"), NIL, NIL, (Bool *) &config->GetPersistentIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault));

	menu_encoder_options->AddEntry();

	menu_encoder_options->AddEntry(i18n->TranslateString("Encode to input file folder if possible"), NIL, NIL, (Bool *) &config->GetPersistentIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault))->onAction.Connect(&freacGUI::ToggleUseInputDirectory, this);

	allowOverwriteMenuEntry = menu_encoder_options->AddEntry(i18n->TranslateString("Allow overwriting input file"), NIL, NIL, (Bool *) &config->GetPersistentIntValue(Config::CategorySettingsID, Config::SettingsAllowOverwriteSourceID, Config::SettingsAllowOverwriteSourceDefault));

	menu_encoder_options->AddEntry();

	menu_encoder_options->AddEntry(i18n->TranslateString("Delete original files after encoding"), NIL, NIL, &currentConfig->deleteAfterEncoding)->onAction.Connect(&freacGUI::ConfirmDeleteAfterEncoding, this);

	menu_encoder_options->AddEntry();

	menu_encoder_options->AddEntry(i18n->TranslateString("Shutdown after encoding"), NIL, NIL, &currentConfig->shutdownAfterEncoding);

	menu_encode->AddEntry();

	menu_encode->AddEntry(i18n->TranslateString("Encoder options"), ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:29")), menu_encoder_options);

	ToggleUseInputDirectory();

	i18n->SetContext("Menu::Help");

	entry = menu_help->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Help topics")), ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:34")));
	entry->onAction.Connect(&freacGUI::ShowHelp, this);
	entry->SetShortcut(0, Keyboard::KeyF1, mainWnd);

	menu_help->AddEntry();

	entry = menu_help->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Show Tip of the Day")));
	entry->onAction.Connect(&freacGUI::ShowTipOfTheDay, this);
	entry->SetShortcut(0, Keyboard::KeyF10, mainWnd);

	if (currentConfig->enable_eUpdate)
	{
		menu_help->AddEntry();
		menu_help->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Check for updates now")))->onAction.Connect(&freacGUI::CheckForUpdates, this);
		menu_help->AddEntry(i18n->TranslateString("Check for updates at startup"), NIL, NIL, (Bool *) &config->GetPersistentIntValue(Config::CategorySettingsID, Config::SettingsCheckForUpdatesID, Config::SettingsCheckForUpdatesDefault));
	}

	menu_help->AddEntry();
	menu_help->AddEntry(i18n->AddEllipsis(i18n->TranslateString("About %1").Replace("%1", freac::appName)), ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:35")))->onAction.Connect(&freacGUI::About, this);

	i18n->SetContext("Menu");

	mainWnd_menubar->RemoveAllEntries();

	mainWnd_menubar->AddEntry(i18n->TranslateString("File"), NIL, menu_file);

	if (menu_drives->GetNOfEntries() >= 1) mainWnd_menubar->AddEntry(i18n->TranslateString("Database"), NIL, menu_database);

	mainWnd_menubar->AddEntry(i18n->TranslateString("Options"), NIL, menu_options);
	mainWnd_menubar->AddEntry(i18n->TranslateString("Encode"), NIL, menu_encode);

	mainWnd_menubar->AddEntry()->SetOrientation(OR_RIGHT);

	mainWnd_menubar->AddEntry(i18n->TranslateString("Help"), NIL, menu_help)->SetOrientation(OR_RIGHT);

	i18n->SetContext("Toolbar");

	mainWnd_iconbar->RemoveAllEntries();

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:1")), menu_files);
	entry->onAction.Connect(&JobList::AddTrackByDialog, joblist);
	entry->SetTooltipText(i18n->TranslateString("Add audio file(s) to the joblist"));

	if (menu_drives->GetNOfEntries() >= 1)
	{
		entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:2")), menu_drives->GetNOfEntries() > 1 ? menu_drives : NIL);
		entry->onAction.Connect(&freacGUI::ReadCD, this);
		entry->SetTooltipText(i18n->TranslateString("Add audio CD contents to the joblist"));
	}

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:3")));
	entry->onAction.Connect(&JobList::RemoveSelectedTrack, joblist);
	entry->SetTooltipText(i18n->TranslateString("Remove the selected entry from the joblist"));

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:4")));
	entry->onAction.Connect(&JobList::StartJobRemoveAllTracks, joblist);
	entry->SetTooltipText(i18n->TranslateString("Clear the entire joblist"));

	if (menu_drives->GetNOfEntries() >= 1)
	{
		mainWnd_iconbar->AddEntry();

		entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:5")), menu_database_query);
		entry->onAction.Connect(&freacGUI::QueryCDDB, this);
		entry->SetTooltipText(i18n->TranslateString("Query CDDB database"));

		entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:6")));
		entry->onAction.Connect(&freacGUI::SubmitCDDBData, this);
		entry->SetTooltipText(i18n->AddEllipsis(i18n->TranslateString("Submit CDDB data")));
	}

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:7")), config->GetNOfConfigurations() > 1 ? menu_configurations : NIL);
	entry->onAction.Connect(&freacGUI::ConfigureSettings, this);
	entry->SetTooltipText(i18n->TranslateString("Configure general settings"));

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:8")));
	entry->onAction.Connect(&freacGUI::ConfigureEncoder, this);
	entry->SetTooltipText(i18n->TranslateString("Configure the selected audio encoder"));

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:9")), boca.GetNumberOfComponentsOfType(COMPONENT_TYPE_ENCODER) > 0 ? menu_encoders : NIL);
	entry->onAction.Connect(&freacGUI::Convert, this);
	entry->SetTooltipText(i18n->TranslateString("Start the encoding process"));

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:10")));
	entry->onAction.Connect(&freacGUI::PauseResumeEncoding, this);
	entry->SetTooltipText(i18n->TranslateString("Pause/resume encoding"));

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load(String(freacConfig->resourcesPath).Append("freac.pci:11")));
	entry->onAction.Connect(&freacGUI::StopEncoding, this);
	entry->SetTooltipText(i18n->TranslateString("Stop encoding"));

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

		clicked_encoder = -1;

		OptionBox::internalCheckValues.Emit();
	}

	/* Return if there are no tracks to convert.
	 */
	if (joblist->Length() == 0) return;

	/* Detect accidental double clicks.
	 */
	static Int64	 previousTicks = 0;

	if (S::System::System::Clock() - previousTicks < 250) return;

	/* Warn about active conversion.
	 */
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	if (JobConvert::IsConverting() && QuickMessage(i18n->TranslateString("A conversion process is already active!\n\nWould you like to enqueue this conversion?", "Messages"), i18n->TranslateString("Info"), Message::Buttons::YesNo, Message::Icon::Question) != Message::Button::Yes) return;

	/* Start conversion.
	 */
	Converter().Convert(joblist);

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
	AddPatternDialog	*dialog = new AddPatternDialog();

	if (dialog->ShowDialog() == Success()) joblist->AddTracksByPattern(dialog->GetDirectory(), dialog->GetPattern(), dialog->GetSearchSubFolders());

	DeleteObject(dialog);
}

Void freac::freacGUI::AddFilesFromDirectory()
{
	AddDirectoryDialog	*dialog = new AddDirectoryDialog();

	if (dialog->ShowDialog() == Success())
	{
		Array<String>	 directories;

		directories.Add(dialog->GetDirectory());

		joblist->AddTracksByDragAndDrop(directories);
	}

	DeleteObject(dialog);
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
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Messages");

	if (currentConfig->deleteAfterEncoding)
	{
		if (Message::Button::No == QuickMessage(i18n->TranslateString("This option will remove the original files from your computer\nafter the encoding process!\n\nAre you sure you want to activate this function?"), i18n->TranslateString("Delete original files after encoding"), Message::Buttons::YesNo, Message::Icon::Question)) currentConfig->deleteAfterEncoding = False;
	}
}

Void freac::freacGUI::ShowHelp()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Menu::Help");

	S::System::System::OpenURL(String("file://").Append(Config::Get()->documentationPath).Append("manual/").Append(i18n->TranslateString("index_en.html")));
}

Void freac::freacGUI::ShowTipOfTheDay()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Tips");

	Bool		 showTips = config->GetIntValue(Config::CategorySettingsID, Config::SettingsShowTipsID, Config::SettingsShowTipsDefault);
	TipOfTheDay	*dialog = new TipOfTheDay(&showTips);

	dialog->AddTip(i18n->TranslateString("%1 is available in %2 languages. If your language is\nnot available, you can easily translate %1 using the\n\'smooth Translator\' application.").Replace("%1", freac::appName).Replace("%2", String::FromInt(Math::Max(36, i18n->GetNOfLanguages()))));
	dialog->AddTip(i18n->TranslateString("%1 comes with support for the LAME, Ogg Vorbis, FAAC,\nFLAC and Bonk encoders. An encoder for the VQF format is\navailable at the %1 website: %2").Replace("%1", freac::appName).Replace("%2", freac::website));

#ifdef __WIN32__
	dialog->AddTip(i18n->TranslateString("%1 can use Winamp 2 input plug-ins to support more file\nformats. Copy the in_*.dll files to the %1/plugins directory to\nenable %1 to read these formats.").Replace("%1", freac::appName));
#endif

	dialog->AddTip(i18n->TranslateString("With %1 you can submit freedb CD database entries\ncontaining Unicode characters. So if you have any CDs with\nnon-Latin artist or title names, you can submit the correct\nfreedb entries with %1.").Replace("%1", freac::appName));
	dialog->AddTip(i18n->TranslateString("To correct reading errors while ripping you can enable\nJitter correction in the Ripper tab of %1's configuration\ndialog. If that does not help, try using one of the Paranoia modes.").Replace("%1", freac::appName));
	dialog->AddTip(i18n->TranslateString("Do you have any suggestions on how to improve %1?\nYou can submit any ideas through the Tracker on the %1\nSourceForge project page - %2\nor send an eMail to %3.").Replace("%1", freac::appName).Replace("%2", "https://sf.net/projects/bonkenc").Replace("%3", "suggestions@freac.org"));
	dialog->AddTip(i18n->TranslateString("Do you like %1? %1 is available for free, but you can\nhelp fund the development by donating to the %1 project.\nYou can send money to %2 through PayPal.\nSee %3 for more details.").Replace("%1", freac::appName).Replace("%2", "donate@freac.org").Replace("%3", String(freac::website).Append("donating.php")));

	dialog->SetMode(TIP_ORDERED, config->GetIntValue(Config::CategorySettingsID, Config::SettingsNextTipID, Config::SettingsNextTipDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsShowTipsID, Config::SettingsShowTipsDefault));

	dialog->ShowDialog();

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsShowTipsID, showTips);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsNextTipID, dialog->GetOffset());

	DeleteObject(dialog);
}

Void freac::freacGUI::CheckForUpdates()
{
	(new JobCheckForUpdates(False))->Schedule();
}
