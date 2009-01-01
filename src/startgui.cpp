 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <startgui.h>
#include <resources.h>
#include <dbt.h>

#include <dllinterfaces.h>
#include <joblist.h>
#include <utilities.h>

#include <dialogs/config/config.h>
#include <dialogs/config/configcomponent.h>

#include <dialogs/adddirectory.h>
#include <dialogs/addpattern.h>

#include <cddb/cddb.h>
#include <cddb/cddbremote.h>
#include <cddb/cddbbatch.h>
#include <cddb/cddbcache.h>

#include <dialogs/cddb/query.h>
#include <dialogs/cddb/submit.h>
#include <dialogs/cddb/manage.h>
#include <dialogs/cddb/managequeries.h>
#include <dialogs/cddb/managesubmits.h>

Int StartGUI(const Array<String> &args)
{
	BoCA::Protocol	*debug = BoCA::Protocol::Get("Debug Output");

	debug->Write("");
	debug->Write("=======================");
	debug->Write("= Starting BonkEnc... =");
	debug->Write("=======================");
	debug->Write("");

	BonkEnc::BonkEncGUI	*app = BonkEnc::BonkEncGUI::Get();

	debug->Write("Entering method: smooth::Loop()");

	app->Loop();

	debug->Write("Leaving method.");

	BonkEnc::BonkEncGUI::Free();

	debug->Write("");
	debug->Write("====================");
	debug->Write("= Leaving BonkEnc! =");
	debug->Write("====================");

	BoCA::Protocol::Free();

	return 0;
}

BonkEnc::BonkEncGUI *BonkEnc::BonkEncGUI::Get()
{
	if (instance == NIL) new BonkEncGUI();

	return (BonkEncGUI *) instance;
}

Void BonkEnc::BonkEncGUI::Free()
{
	if (instance != NIL) delete (BonkEncGUI *) instance;
}

BonkEnc::BonkEncGUI::BonkEncGUI()
{
	currentConfig->enable_console = false;

	dontUpdateInfo = False;

	clicked_drive = -1;
	clicked_encoder = -1;

	if (currentConfig->language == NIL && i18n->GetNOfLanguages() > 1)
	{
		switch (PRIMARYLANGID(GetUserDefaultLangID()))
		{
			default:
			case LANG_ARABIC:
				currentConfig->language = "bonkenc_ar.xml";
				break;
			case LANG_CATALAN:
				currentConfig->language = "bonkenc_ca.xml";
				break;
			case LANG_CHINESE:
				currentConfig->language = "bonkenc_zh_CN.xml";

				if (SUBLANGID(GetUserDefaultLangID()) == SUBLANG_CHINESE_SIMPLIFIED) currentConfig->language = "bonkenc_zh_CN.xml";
				if (SUBLANGID(GetUserDefaultLangID()) == SUBLANG_CHINESE_TRADITIONAL) currentConfig->language = "bonkenc_zh_TW.xml";
				break;
			case LANG_CZECH:
				currentConfig->language = "bonkenc_cz.xml";
				break;
			case LANG_DANISH:
				currentConfig->language = "bonkenc_dk.xml";
				break;
			case LANG_DUTCH:
				currentConfig->language = "bonkenc_nl.xml";
				break;
			case LANG_ENGLISH:
				currentConfig->language = "internal";
				break;
			case LANG_ESTONIAN:
				currentConfig->language = "bonkenc_ee.xml";
				break;
			case LANG_FINNISH:
				currentConfig->language = "bonkenc_fi.xml";
				break;
			case LANG_FRENCH:
				currentConfig->language = "bonkenc_fr.xml";
				break;
			case LANG_GERMAN:
				currentConfig->language = "bonkenc_de.xml";
				break;
			case LANG_GREEK:
				currentConfig->language = "bonkenc_gr.xml";
				break;
			case LANG_HUNGARIAN:
				currentConfig->language = "bonkenc_hu.xml";
				break;
			case LANG_ITALIAN:
				currentConfig->language = "bonkenc_it.xml";
				break;
			case LANG_JAPANESE:
				currentConfig->language = "bonkenc_ja.xml";
				break;
			case LANG_KOREAN:
				currentConfig->language = "bonkenc_ko.xml";
				break;
			case LANG_LITHUANIAN:
				currentConfig->language = "bonkenc_lt.xml";
				break;
			case LANG_NORWEGIAN:
				currentConfig->language = "bonkenc_no.xml";
				break;
			case LANG_POLISH:
				currentConfig->language = "bonkenc_pl.xml";
				break;
			case LANG_PORTUGUESE:
				currentConfig->language = "bonkenc_pt.xml";

				if (SUBLANGID(GetUserDefaultLangID()) == SUBLANG_PORTUGUESE) currentConfig->language = "bonkenc_pt.xml";
				if (SUBLANGID(GetUserDefaultLangID()) == SUBLANG_PORTUGUESE_BRAZILIAN) currentConfig->language = "bonkenc_pt_BR.xml";
				break;
			case LANG_ROMANIAN:
				currentConfig->language = "bonkenc_ro.xml";
				break;
			case LANG_RUSSIAN:
				currentConfig->language = "bonkenc_ru.xml";
				break;
			case LANG_SERBIAN:
				currentConfig->language = "bonkenc_sr.xml";
				break;
			case LANG_SLOVAK:
				currentConfig->language = "bonkenc_sk.xml";
				break;
			case LANG_SPANISH:
				currentConfig->language = "bonkenc_es.xml";

				if (SUBLANGID(GetUserDefaultLangID()) == SUBLANG_SPANISH) currentConfig->language = "bonkenc_es.xml";
				if (SUBLANGID(GetUserDefaultLangID()) == SUBLANG_SPANISH_ARGENTINA) currentConfig->language = "bonkenc_es_AR.xml";
				break;
			case LANG_SWEDISH:
				currentConfig->language = "bonkenc_sv.xml";
				break;
			case LANG_TURKISH:
				currentConfig->language = "bonkenc_tr.xml";
				break;
			case LANG_UKRAINIAN:
				currentConfig->language = "bonkenc_ua.xml";
				break;
		}
	}

	if (currentConfig->language == NIL) currentConfig->language = "internal";

	i18n->ActivateLanguage(currentConfig->language);

	Rect	 workArea = MultiMonitor::GetVirtualScreenMetrics();

	if (currentConfig->wndPos.x + currentConfig->wndSize.cx > workArea.right + 2 ||
	    currentConfig->wndPos.y + currentConfig->wndSize.cy > workArea.bottom + 2)
	{
		currentConfig->wndPos.x = (Int) Math::Min(workArea.right - 10 - currentConfig->wndSize.cx, currentConfig->wndPos.x);
		currentConfig->wndPos.y = (Int) Math::Min(workArea.bottom - 10 - currentConfig->wndSize.cy, currentConfig->wndPos.y);
	}

	if (currentConfig->wndPos.x < workArea.left - 2 ||
	    currentConfig->wndPos.y < workArea.top - 2)
	{
		currentConfig->wndPos.x = (Int) Math::Max(workArea.left + 10, currentConfig->wndPos.x);
		currentConfig->wndPos.y = (Int) Math::Max(workArea.top + 10, currentConfig->wndPos.y);

		currentConfig->wndSize.cx = (Int) Math::Min(workArea.right - 20, currentConfig->wndSize.cx);
		currentConfig->wndSize.cy = (Int) Math::Min(workArea.bottom - 20, currentConfig->wndSize.cy);
	}

	mainWnd			= new Window(String("BonkEnc ").Append(BonkEnc::version), currentConfig->wndPos, currentConfig->wndSize);
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar();
	mainWnd_menubar		= new Menubar();
	mainWnd_iconbar		= new Menubar();
	mainWnd_statusbar	= new Statusbar(String("BonkEnc ").Append(BonkEnc::version).Append(" - Copyright (C) 2001-2009 Robert Kausch"));
	menu_file		= new PopupMenu();
	menu_options		= new PopupMenu();
	menu_addsubmenu		= new PopupMenu();
	menu_encode		= new PopupMenu();
	menu_drives		= new PopupMenu();
	menu_files		= new PopupMenu();
	menu_seldrive		= new PopupMenu();
	menu_database		= new PopupMenu();
	menu_database_query	= new PopupMenu();
	menu_help		= new PopupMenu();
	menu_encoders		= new PopupMenu();
	menu_encoder_options	= new PopupMenu();

	hyperlink		= new Hyperlink("www.bonkenc.org", NIL, "http://www.bonkenc.org/", Point(91, -22));
	hyperlink->SetOrientation(OR_UPPERRIGHT);
	hyperlink->SetIndependent(True);

	tabs_main		= new TabWidget(Point(6, 7), Size(700, 500));

	tab_layer_joblist	= new LayerJoblist();
	tab_layer_joblist->onRequestSkipTrack.Connect(&Encoder::SkipTrack, encoder);

	tabs_main->Add(tab_layer_joblist);

	InitExtensionComponents();

	for (Int i = 0; i < extensionComponents.Length(); i++)
	{
		Layer	*mainTabLayer	= extensionComponents.GetNth(i)->getMainTabLayer.Emit();
		Layer	*statusBarLayer = extensionComponents.GetNth(i)->getStatusBarLayer.Emit();

		if (mainTabLayer   != NIL) tabs_main->Add(mainTabLayer);
		if (statusBarLayer != NIL) mainWnd_statusbar->Add(statusBarLayer);
	}

	tab_layer_threads	= new LayerThreads();
	tab_layer_protocol	= new LayerProtocol();

	tabs_main->Add(tab_layer_threads);
	tabs_main->Add(tab_layer_protocol);

	joblist			= tab_layer_joblist->GetJoblist();

	Add(mainWnd);

	SetLanguage();

	mainWnd->Add(hyperlink);
	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(mainWnd_statusbar);
	mainWnd->Add(mainWnd_menubar);
	mainWnd->Add(mainWnd_iconbar);
	mainWnd->Add(tabs_main);

	mainWnd->SetIcon(ImageLoader::Load("BonkEnc.pci:0"));

	mainWnd->onChangePosition.Connect(&BonkEncGUI::OnChangePosition, this);
	mainWnd->onChangeSize.Connect(&BonkEncGUI::OnChangeSize, this);

	mainWnd->onEvent.Connect(&BonkEncGUI::MessageProc, this);

	if (currentConfig->showTips) mainWnd->onShow.Connect(&BonkEncGUI::ShowTipOfTheDay, this);

	mainWnd->doClose.Connect(&BonkEncGUI::ExitProc, this);
	mainWnd->SetMinimumSize(Size(530, 340 + (currentConfig->showTitleInfo ? 68 : 0)));

	if (currentConfig->maximized) mainWnd->Maximize();

	encoder->onStartEncoding.Connect(&LayerJoblist::OnEncoderStartEncoding, tab_layer_joblist);
	encoder->onFinishEncoding.Connect(&LayerJoblist::OnEncoderFinishEncoding, tab_layer_joblist);

	encoder->onEncodeTrack.Connect(&LayerJoblist::OnEncoderEncodeTrack, tab_layer_joblist);

	encoder->onTrackProgress.Connect(&LayerJoblist::OnEncoderTrackProgress, tab_layer_joblist);
	encoder->onTotalProgress.Connect(&LayerJoblist::OnEncoderTotalProgress, tab_layer_joblist);

	if (currentConfig->checkUpdatesAtStartup) NonBlocking1<Bool>(&BonkEncGUI::CheckForUpdates, this).Call(True);
}

BonkEnc::BonkEncGUI::~BonkEncGUI()
{
	FreeExtensionComponents();

	DeleteObject(mainWnd_menubar);
	DeleteObject(mainWnd_iconbar);
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd_statusbar);
	DeleteObject(mainWnd);

	DeleteObject(tabs_main);

	DeleteObject(tab_layer_joblist);
	DeleteObject(tab_layer_threads);
	DeleteObject(tab_layer_protocol);

	DeleteObject(hyperlink);

	DeleteObject(menu_file);
	DeleteObject(menu_options);
	DeleteObject(menu_addsubmenu);
	DeleteObject(menu_encode);
	DeleteObject(menu_drives);
	DeleteObject(menu_files);
	DeleteObject(menu_seldrive);
	DeleteObject(menu_encoders);
	DeleteObject(menu_encoder_options);
	DeleteObject(menu_database);
	DeleteObject(menu_database_query);
	DeleteObject(menu_help);
}

Void BonkEnc::BonkEncGUI::InitExtensionComponents()
{
	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_EXTENSION) continue;

		ExtensionComponent	*component = (ExtensionComponent *) boca.CreateComponentByID(boca.GetComponentID(i));

		if (component != NIL) extensionComponents.Add(component);
	}
}

Void BonkEnc::BonkEncGUI::FreeExtensionComponents()
{
	Registry	&boca = Registry::Get();

	for (Int i = 0; i < extensionComponents.Length(); i++)
	{
		boca.DeleteComponent(extensionComponents.GetNth(i));
	}

	extensionComponents.RemoveAll();
}

Bool BonkEnc::BonkEncGUI::ExitProc()
{
	if (encoder->IsEncoding())
	{
		if (IDNO == QuickMessage(i18n->TranslateString("The encoding thread is still running! Do you really want to quit?"), i18n->TranslateString("Currently encoding"), MB_YESNO, IDI_QUESTION)) return False;

		encoder->Stop();
	}

	/* Stop playback if playing a track
	 */
	tab_layer_joblist->StopPlayback();

	Rect	 wndRect = mainWnd->GetRestoredWindowRect();

	currentConfig->wndPos = Point(wndRect.left, wndRect.top);
	currentConfig->wndSize = Size(wndRect.right - wndRect.left, wndRect.bottom - wndRect.top);
	currentConfig->maximized = mainWnd->IsMaximized();

	currentConfig->tab_width_track = joblist->GetNthTabWidth(1);
	currentConfig->tab_width_length = joblist->GetNthTabWidth(2);
	currentConfig->tab_width_size = joblist->GetNthTabWidth(3);

	currentConfig->SaveSettings();

	return True;
}

Void BonkEnc::BonkEncGUI::MessageProc(Int message, Int wParam, Int lParam)
{
	switch (message)
	{
		case WM_DEVICECHANGE:
			if (wParam == DBT_DEVICEARRIVAL && currentConfig->enable_cdrip && BoCA::Config::Get()->cdrip_autoRead)
			{
				if (((DEV_BROADCAST_HDR *) lParam)->dbch_devicetype != DBT_DEVTYP_VOLUME || !(((DEV_BROADCAST_VOLUME *) lParam)->dbcv_flags & DBTF_MEDIA)) break;

				String	 trackCDA = String(" ").Append(":\\track01.cda");

				for (Int drive = 0; drive < 26; drive++)
				{
					if (((DEV_BROADCAST_VOLUME *) lParam)->dbcv_unitmask >> drive & 1)
					{
						trackCDA[0] = drive + 'A';

						break;
					}
				}

				if (trackCDA[0] == ' ') break;

				// Read length of track from .cda file
				InStream	*in = new InStream(STREAM_FILE, trackCDA, IS_READONLY);

				in->Seek(32);

				Int	 trackLength = in->InputNumber(4);

				delete in;

				if (trackLength > 0)
				{
					Bool	 ok = False;
					Int	 drive = 0;

					for (drive = 0; drive < BoCA::Config::Get()->cdrip_numdrives; drive++)
					{
						ex_CR_SetActiveCDROM(drive);

						ex_CR_ReadToc();

						TOCENTRY	 entry = ex_CR_GetTocEntry(0);
						TOCENTRY	 nextentry = ex_CR_GetTocEntry(1);
						Int		 length = nextentry.dwStartSector - entry.dwStartSector;

						if (!(entry.btFlag & CDROMDATAFLAG) && length == trackLength) { ok = True; break; }
					}

					if (ok)
					{
						BoCA::Config::Get()->cdrip_activedrive = drive;
						BoCA::Config::Get()->cdrip_autoRead_active = True;

						ReadCD();

						BoCA::Config::Get()->cdrip_autoRead_active = False;

						if (BoCA::Config::Get()->cdrip_autoRip) Encode();
					}
				}
			}

			break;
	}
}

Void BonkEnc::BonkEncGUI::OnChangePosition(const Point &nPos)
{
	currentConfig->wndPos = mainWnd->GetPosition();
}

Void BonkEnc::BonkEncGUI::OnChangeSize(const Size &nSize)
{
	currentConfig->wndSize = mainWnd->GetSize();

	mainWnd->SetStatusText(String("BonkEnc ").Append(BonkEnc::version).Append(" - Copyright (C) 2001-2009 Robert Kausch"));

	Rect	 clientRect = mainWnd->GetClientRect();
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	tabs_main->SetSize(clientSize - Size(12, 15));
}

Void BonkEnc::BonkEncGUI::Close()
{
	mainWnd->Close();
}

Void BonkEnc::BonkEncGUI::About()
{
	QuickMessage(String("BonkEnc ").Append(BonkEnc::version).Append("\nCopyright (C) 2001-2009 Robert Kausch\n\n").Append(String(i18n->TranslateString("Translated by %1.")).Replace("%1", i18n->GetActiveLanguageAuthor())).Append("\n\n").Append(i18n->TranslateString("This program is being distributed under the terms\nof the GNU General Public License (GPL).")), i18n->TranslateString("About BonkEnc"), MB_OK, MAKEINTRESOURCE(IDI_ICON));
}

Void BonkEnc::BonkEncGUI::ConfigureEncoder()
{
	if (!currentConfig->CanChangeConfig()) return;

	Registry	&boca = Registry::Get();
	Component	*component = boca.CreateComponentByID(currentConfig->encoderID);
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
		QuickMessage(String(i18n->TranslateString("No configuration dialog available for:\n\n%1")).Replace("%1", component->GetName()), BonkEnc::i18n->TranslateString("Error"), MB_OK, IDI_INFORMATION);
	}

	component->FreeConfigurationLayer();

	boca.DeleteComponent(component);
}

Void BonkEnc::BonkEncGUI::ConfigureSettings()
{
	if (!currentConfig->CanChangeConfig()) return;

	ConfigDialog	*dlg = new ConfigDialog();

	dlg->ShowDialog();

	DeleteObject(dlg);

	if (currentConfig->languageChanged)
	{
		SetLanguage();

		currentConfig->languageChanged = false;
	}

	tab_layer_joblist->UpdateEncoderText();
	tab_layer_joblist->UpdateOutputDir();

	CheckBox::internalCheckValues.Emit();
	ToggleUseInputDirectory();

	currentConfig->SaveSettings();
}


Void BonkEnc::BonkEncGUI::ReadCD()
{
	if (!joblist->CanModifyJobList()) return;

	ex_CR_SetActiveCDROM(BoCA::Config::Get()->cdrip_activedrive);

	ex_CR_ReadToc();

	Int	 numTocEntries = ex_CR_GetNumTocEntries();

	for (Int i = 0; i < numTocEntries; i++)
	{
		TOCENTRY entry = ex_CR_GetTocEntry(i);

		if (!(entry.btFlag & CDROMDATAFLAG) && entry.btTrackNumber == i + 1)
		{
			/* Add CD track to joblist using a cdda:// URI
			 */
			joblist->AddTrackByFileName(
				String("cdda://")
					.Append(String::FromInt(BoCA::Config::Get()->cdrip_activedrive))
					.Append("/")
					.Append(String::FromInt(entry.btTrackNumber))
			);
		}
	}

	if (currentConfig->enable_auto_cddb) QueryCDDB();
}

Void BonkEnc::BonkEncGUI::ReadSpecificCD()
{
	BoCA::Config::Get()->cdrip_activedrive = clicked_drive;

	clicked_drive = -1;

	ReadCD();
}

Void BonkEnc::BonkEncGUI::QueryCDDB()
{
	if (!currentConfig->enable_local_cddb && !currentConfig->enable_remote_cddb)
	{
		Utilities::ErrorMessage("CDDB support is disabled! Please enable local or\nremote CDDB support in the configuration dialog.");

		return;
	}

	Array<Int>	 discIDs;
	Array<String>	 discIDStrings;

	for (Int i = 0; i < joblist->GetNOfTracks(); i++)
	{
		const Track	&track = joblist->GetNthTrack(i);
		Int		 discID = CDDB::StringToDiscID(track.discid);

		if (track.isCDTrack)
		{
			discIDs.Add(discID, track.drive);
			discIDStrings.Add(track.discid, track.drive);
		}
	}

	for (Int j = 0; j < discIDs.Length(); j++)
	{
		Int	 oDrive = BoCA::Config::Get()->cdrip_activedrive;

		BoCA::Config::Get()->cdrip_activedrive = discIDs.GetNthIndex(j);

		Int	 discID = discIDs.GetNth(j);
		String	 discIDString = discIDStrings.GetNth(j);
		CDDBInfo cdInfo;

		if (currentConfig->enable_cddb_cache) cdInfo = CDDBCache::Get()->GetCacheEntry(discID);

		if (cdInfo == NIL)
		{
			cdInfo = GetCDDBData();

			if (cdInfo != NIL) CDDBCache::Get()->AddCacheEntry(cdInfo);
		}

		BoCA::Config::Get()->cdrip_activedrive = oDrive;

		if (cdInfo != NIL)
		{
			for (Int k = 0; k < joblist->GetNOfTracks(); k++)
			{
				Track	 track = joblist->GetNthTrack(k);

				if (track.isCDTrack && track.discid == discIDString)
				{
					track.track	= track.cdTrack;
					track.outfile	= NIL;
					track.artist	= (cdInfo.dArtist == "Various" ? cdInfo.trackArtists.GetNth(track.cdTrack - 1) : cdInfo.dArtist);
					track.title	= cdInfo.trackTitles.GetNth(track.cdTrack - 1);
					track.album	= cdInfo.dTitle;
					track.genre	= cdInfo.dGenre;
					track.year	= cdInfo.dYear;

					track.oArtist	= track.artist;
					track.oTitle	= track.title;
					track.oAlbum	= track.album;
					track.oGenre	= track.genre;

					BoCA::JobList::Get()->onComponentModifyTrack.Emit(track);
				}
			}
		}
	}

	joblist->Paint(SP_PAINT);

	if (joblist->GetSelectedTrack() != NIL) joblist->OnSelectEntry();
}

Void BonkEnc::BonkEncGUI::QueryCDDBLater()
{
	Array<Int>	 drives;

	for (Int i = 0; i < joblist->GetNOfTracks(); i++)
	{
		const Track	&track = joblist->GetNthTrack(i);

		if (track.isCDTrack) drives.Add(track.drive, track.drive);
	}

	if (drives.Length() > 0)
	{
		CDDBBatch	*queries = new CDDBBatch();

		for (Int j = 0; j < drives.Length(); j++)
		{
			Int		 drive = drives.GetNth(j);
			CDDBRemote	 cddb;

			cddb.SetActiveDrive(drive);

			queries->AddQuery(cddb.GetCDDBQueryString());
		}

		delete queries;
	}
}

BonkEnc::CDDBInfo BonkEnc::BonkEncGUI::GetCDDBData()
{
	cddbQueryDlg	*dlg	  = new cddbQueryDlg();
	CDDBInfo	 cddbInfo = dlg->QueryCDDB(True);

	DeleteObject(dlg);

	return cddbInfo;
}

Void BonkEnc::BonkEncGUI::SubmitCDDBData()
{
	if (!currentConfig->enable_local_cddb && !currentConfig->enable_remote_cddb)
	{
		Utilities::ErrorMessage("CDDB support is disabled! Please enable local or\nremote CDDB support in the configuration dialog.");

		return;
	}

	cddbSubmitDlg	*dlg = new cddbSubmitDlg();

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Void BonkEnc::BonkEncGUI::ManageCDDBData()
{
	cddbManageDlg	*dlg = new cddbManageDlg();

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Void BonkEnc::BonkEncGUI::ManageCDDBBatchData()
{
	if (!currentConfig->enable_remote_cddb)
	{
		Utilities::ErrorMessage("Remote CDDB support is disabled! Please enable\nremote CDDB support in the configuration dialog.");

		return;
	}

	cddbManageSubmitsDlg	*dlg = new cddbManageSubmitsDlg();

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Void BonkEnc::BonkEncGUI::ManageCDDBBatchQueries()
{
	if (!currentConfig->enable_remote_cddb)
	{
		Utilities::ErrorMessage("Remote CDDB support is disabled! Please enable\nremote CDDB support in the configuration dialog.");

		return;
	}

	cddbManageQueriesDlg	*dlg = new cddbManageQueriesDlg();

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Bool BonkEnc::BonkEncGUI::SetLanguage()
{
	Bool	 prevRTL = i18n->IsActiveLanguageRightToLeft();

	i18n->ActivateLanguage(currentConfig->language);

	if (i18n->IsActiveLanguageRightToLeft() != prevRTL)
	{
		mainWnd->SetUpdateRect(Rect(Point(0, 0), mainWnd->GetSize()));
		mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());
		mainWnd->Paint(SP_PAINT);
	}

	onChangeLanguageSettings.Emit();

	FillMenus();

	hyperlink->Hide();
	hyperlink->Show();

	return true;
}

Void BonkEnc::BonkEncGUI::FillMenus()
{
	mainWnd_menubar->Hide();
	mainWnd_iconbar->Hide();

	menu_file->RemoveAllEntries();
	menu_options->RemoveAllEntries();
	menu_drives->RemoveAllEntries();
	menu_files->RemoveAllEntries();
	menu_seldrive->RemoveAllEntries();
	menu_addsubmenu->RemoveAllEntries();
	menu_encode->RemoveAllEntries();
	menu_encoders->RemoveAllEntries();
	menu_encoder_options->RemoveAllEntries();
	menu_database->RemoveAllEntries();
	menu_database_query->RemoveAllEntries();
	menu_help->RemoveAllEntries();

	MenuEntry	*entry = NIL;

	menu_file->AddEntry(i18n->TranslateString("Add"), ImageLoader::Load("BonkEnc.pci:21"), menu_addsubmenu);
	entry = menu_file->AddEntry(i18n->TranslateString("Remove"), ImageLoader::Load("BonkEnc.pci:24"));
	entry->onAction.Connect(&JobList::RemoveSelectedTrack, joblist);
	entry->SetShortcut(SC_CTRL, 'R', mainWnd);
	menu_file->AddEntry();
	menu_file->AddEntry(i18n->TranslateString("Load joblist..."))->onAction.Connect(&JobList::LoadList, joblist);
	menu_file->AddEntry(i18n->TranslateString("Save joblist..."))->onAction.Connect(&JobList::SaveList, joblist);
	menu_file->AddEntry();
	entry = menu_file->AddEntry(i18n->TranslateString("Clear joblist"), ImageLoader::Load("BonkEnc.pci:25"));
	entry->onAction.Connect(&JobList::RemoveAllTracks, joblist);
	entry->SetShortcut(SC_CTRL | SC_SHIFT, 'R', mainWnd);
	menu_file->AddEntry();
	entry = menu_file->AddEntry(i18n->TranslateString("Exit"), ImageLoader::Load("BonkEnc.pci:36"));
	entry->onAction.Connect(&BonkEncGUI::Close, this);
	entry->SetShortcut(SC_ALT, VK_F4, mainWnd);

	entry = menu_options->AddEntry(i18n->TranslateString("General settings..."), ImageLoader::Load("BonkEnc.pci:28"));
	entry->onAction.Connect(&BonkEncGUI::ConfigureSettings, this);
	entry->SetShortcut(SC_CTRL | SC_SHIFT, 'C', mainWnd);
	entry = menu_options->AddEntry(i18n->TranslateString("Configure selected encoder..."), ImageLoader::Load("BonkEnc.pci:29"));
	entry->onAction.Connect(&BonkEncGUI::ConfigureEncoder, this);
	entry->SetShortcut(SC_CTRL | SC_SHIFT, 'E', mainWnd);

	if (currentConfig->enable_cdrip && BoCA::Config::Get()->cdrip_numdrives > 1)
	{
		for (Int j = 0; j < BoCA::Config::Get()->cdrip_numdrives; j++)
		{
			menu_seldrive->AddEntry(BoCA::Config::Get()->cdrip_drives.GetNth(j), NIL, NIL, NIL, &BoCA::Config::Get()->cdrip_activedrive, j);
		}

		menu_options->AddEntry();
		menu_options->AddEntry(i18n->TranslateString("Active CD-ROM drive"), ImageLoader::Load("BonkEnc.pci:30"), menu_seldrive);
	}

	entry = menu_addsubmenu->AddEntry(String(i18n->TranslateString("Audio file(s)")).Append("..."), ImageLoader::Load("BonkEnc.pci:22"));
	entry->onAction.Connect(&JobList::AddTrackByDialog, joblist);
	entry->SetShortcut(SC_CTRL, 'A', mainWnd);

	if (currentConfig->enable_cdrip && BoCA::Config::Get()->cdrip_numdrives >= 1)
	{
		for (Int j = 0; j < BoCA::Config::Get()->cdrip_numdrives; j++)
		{
			menu_drives->AddEntry(BoCA::Config::Get()->cdrip_drives.GetNth(j), NIL, NIL, NIL, &clicked_drive, j)->onAction.Connect(&BonkEncGUI::ReadSpecificCD, this);
		}

		entry = menu_addsubmenu->AddEntry(i18n->TranslateString("Audio CD contents"), ImageLoader::Load("BonkEnc.pci:23"));
		entry->onAction.Connect(&BonkEncGUI::ReadCD, this);
		entry->SetShortcut(SC_CTRL, 'D', mainWnd);
	}

	menu_files->AddEntry(String(i18n->TranslateString("By pattern")).Append("..."))->onAction.Connect(&BonkEncGUI::AddFilesByPattern, this);
	menu_files->AddEntry(String(i18n->TranslateString("From directory")).Append("..."))->onAction.Connect(&BonkEncGUI::AddFilesFromDirectory, this);

	menu_addsubmenu->AddEntry();
	menu_addsubmenu->AddEntry(i18n->TranslateString("Audio file(s)"), NIL, menu_files);

	if (currentConfig->enable_cdrip && BoCA::Config::Get()->cdrip_numdrives > 1)
	{
		menu_addsubmenu->AddEntry(i18n->TranslateString("Audio CD contents"), NIL, menu_drives);
	}

	entry = menu_encode->AddEntry(i18n->TranslateString("Start encoding"), ImageLoader::Load("BonkEnc.pci:31"));
	entry->onAction.Connect(&BonkEncGUI::Encode, this);
	entry->SetShortcut(SC_CTRL, 'E', mainWnd);
	menu_encode->AddEntry(i18n->TranslateString("Pause/resume encoding"), ImageLoader::Load("BonkEnc.pci:32"))->onAction.Connect(&BonkEncGUI::PauseResumeEncoding, this);
	menu_encode->AddEntry(i18n->TranslateString("Stop encoding"), ImageLoader::Load("BonkEnc.pci:33"))->onAction.Connect(&BonkEncGUI::StopEncoding, this);

	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_ENCODER) continue;

		menu_encoders->AddEntry(boca.GetComponentName(i), NIL, NIL, NIL, &clicked_encoder, i)->onAction.Connect(&BonkEncGUI::EncodeSpecific, this);
	}

	if (Registry::Get().GetNumberOfComponentsOfType(COMPONENT_TYPE_ENCODER) > 0)
	{
		menu_encode->AddEntry();
		menu_encode->AddEntry(i18n->TranslateString("Start encoding"), NIL, menu_encoders);
	}

	menu_encoder_options->AddEntry(i18n->TranslateString("Encode to single file"), NIL, NIL, &currentConfig->encodeToSingleFile);

	menu_encoder_options->AddEntry();
	menu_encoder_options->AddEntry(i18n->TranslateString("Encode to input file directory if possible"), NIL, NIL, &currentConfig->writeToInputDir)->onAction.Connect(&BonkEncGUI::ToggleUseInputDirectory, this);
	allowOverwriteMenuEntry = menu_encoder_options->AddEntry(i18n->TranslateString("Allow overwriting input file"), NIL, NIL, &currentConfig->allowOverwrite);

	menu_encoder_options->AddEntry();
	menu_encoder_options->AddEntry(i18n->TranslateString("Delete original files after encoding"), NIL, NIL, &currentConfig->deleteAfterEncoding)->onAction.Connect(&BonkEncGUI::ConfirmDeleteAfterEncoding, this);

	menu_encoder_options->AddEntry();
	menu_encoder_options->AddEntry(i18n->TranslateString("Shutdown after encoding"), NIL, NIL, &currentConfig->shutdownAfterEncoding);

	menu_encode->AddEntry();
	menu_encode->AddEntry(i18n->TranslateString("Encoder options"), ImageLoader::Load("BonkEnc.pci:29"), menu_encoder_options);

	ToggleUseInputDirectory();

	entry = menu_database->AddEntry(i18n->TranslateString("Query CDDB database"), ImageLoader::Load("BonkEnc.pci:26"));
	entry->onAction.Connect(&BonkEncGUI::QueryCDDB, this);
	entry->SetShortcut(SC_CTRL, 'Q', mainWnd);
	entry = menu_database->AddEntry(i18n->TranslateString("Submit CDDB data..."), ImageLoader::Load("BonkEnc.pci:27"));
	entry->onAction.Connect(&BonkEncGUI::SubmitCDDBData, this);
	entry->SetShortcut(SC_CTRL, 'S', mainWnd);
	menu_database->AddEntry();
	menu_database->AddEntry(i18n->TranslateString("Query CDDB database later"))->onAction.Connect(&BonkEncGUI::QueryCDDBLater, this);
	menu_database->AddEntry();
	menu_database->AddEntry(i18n->TranslateString("Show queued CDDB entries..."))->onAction.Connect(&BonkEncGUI::ManageCDDBBatchData, this);
	menu_database->AddEntry(i18n->TranslateString("Show queued CDDB queries..."))->onAction.Connect(&BonkEncGUI::ManageCDDBBatchQueries, this);
	menu_database->AddEntry();
	menu_database->AddEntry(i18n->TranslateString("Enable CDDB cache"), NIL, NIL, &currentConfig->enable_cddb_cache);
	menu_database->AddEntry(i18n->TranslateString("Manage CDDB cache entries..."))->onAction.Connect(&BonkEncGUI::ManageCDDBData, this);
	menu_database->AddEntry();
	menu_database->AddEntry(i18n->TranslateString("Automatic CDDB queries"), NIL, NIL, &currentConfig->enable_auto_cddb);

	menu_database_query->AddEntry(i18n->TranslateString("Query CDDB database"), ImageLoader::Load("BonkEnc.pci:26"))->onAction.Connect(&BonkEncGUI::QueryCDDB, this);
	menu_database_query->AddEntry(i18n->TranslateString("Query CDDB database later"))->onAction.Connect(&BonkEncGUI::QueryCDDBLater, this);

	entry = menu_help->AddEntry(i18n->TranslateString("Help topics..."), ImageLoader::Load("BonkEnc.pci:34"));
	entry->onAction.Connect(&BonkEncGUI::ShowHelp, this);
	entry->SetShortcut(0, VK_F1, mainWnd);
	menu_help->AddEntry();
	entry = menu_help->AddEntry(String(i18n->TranslateString("Show Tip of the Day")).Append("..."));
	entry->onAction.Connect(&BonkEncGUI::ShowTipOfTheDay, this);
	entry->SetShortcut(0, VK_F10, mainWnd);

	if (currentConfig->enable_eUpdate)
	{
		menu_help->AddEntry();
		menu_help->AddEntry(String(i18n->TranslateString("Check for updates now")).Append("..."))->onAction.Connect(&BonkEncGUI::CheckForUpdates, this);
		menu_help->AddEntry(i18n->TranslateString("Check for updates at startup"), NIL, NIL, &currentConfig->checkUpdatesAtStartup);
	}

	menu_help->AddEntry();
	menu_help->AddEntry(String(i18n->TranslateString("About BonkEnc")).Append("..."), ImageLoader::Load("BonkEnc.pci:35"))->onAction.Connect(&BonkEncGUI::About, this);

	mainWnd_menubar->RemoveAllEntries();

	mainWnd_menubar->AddEntry(i18n->TranslateString("File"), NIL, menu_file);

	if (currentConfig->enable_cdrip && BoCA::Config::Get()->cdrip_numdrives >= 1) mainWnd_menubar->AddEntry(i18n->TranslateString("Database"), NIL, menu_database);

	mainWnd_menubar->AddEntry(i18n->TranslateString("Options"), NIL, menu_options);
	mainWnd_menubar->AddEntry(i18n->TranslateString("Encode"), NIL, menu_encode);
	mainWnd_menubar->AddEntry()->SetOrientation(OR_RIGHT);
	mainWnd_menubar->AddEntry(i18n->TranslateString("Help"), NIL, menu_help)->SetOrientation(OR_RIGHT);

	mainWnd_iconbar->RemoveAllEntries();

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:1"), menu_files);
	entry->onAction.Connect(&JobList::AddTrackByDialog, joblist);
	entry->SetTooltipText(i18n->TranslateString("Add audio file(s) to the joblist"));

	if (currentConfig->enable_cdrip && BoCA::Config::Get()->cdrip_numdrives >= 1)
	{
		entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:2"), BoCA::Config::Get()->cdrip_numdrives > 1 ? menu_drives : NIL);
		entry->onAction.Connect(&BonkEncGUI::ReadCD, this);
		entry->SetTooltipText(i18n->TranslateString("Add audio CD contents to the joblist"));
	}

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:3"));
	entry->onAction.Connect(&JobList::RemoveSelectedTrack, joblist);
	entry->SetTooltipText(i18n->TranslateString("Remove the selected entry from the joblist"));

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:4"));
	entry->onAction.Connect(&JobList::RemoveAllTracks, joblist);
	entry->SetTooltipText(i18n->TranslateString("Clear the entire joblist"));

	if (currentConfig->enable_cdrip && BoCA::Config::Get()->cdrip_numdrives >= 1)
	{
		mainWnd_iconbar->AddEntry();

		entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:5"), menu_database_query);
		entry->onAction.Connect(&BonkEncGUI::QueryCDDB, this);
		entry->SetTooltipText(i18n->TranslateString("Query CDDB database"));

		entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:6"));
		entry->onAction.Connect(&BonkEncGUI::SubmitCDDBData, this);
		entry->SetTooltipText(i18n->TranslateString("Submit CDDB data..."));
	}

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:7"));
	entry->onAction.Connect(&BonkEncGUI::ConfigureSettings, this);
	entry->SetTooltipText(i18n->TranslateString("Configure general settings"));

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:8"));
	entry->onAction.Connect(&BonkEncGUI::ConfigureEncoder, this);
	entry->SetTooltipText(i18n->TranslateString("Configure the selected audio encoder"));

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:9"), Registry::Get().GetNumberOfComponentsOfType(COMPONENT_TYPE_ENCODER) > 0 ? menu_encoders : NIL);
	entry->onAction.Connect(&BonkEncGUI::Encode, this);
	entry->SetTooltipText(i18n->TranslateString("Start the encoding process"));

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:10"));
	entry->onAction.Connect(&BonkEncGUI::PauseResumeEncoding, this);
	entry->SetTooltipText(i18n->TranslateString("Pause/resume encoding"));

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:11"));
	entry->onAction.Connect(&BonkEncGUI::StopEncoding, this);
	entry->SetTooltipText(i18n->TranslateString("Stop encoding"));

	mainWnd_menubar->Show();
	mainWnd_iconbar->Show();
}

Void BonkEnc::BonkEncGUI::EncodeSpecific()
{
	Registry	&boca = Registry::Get();

	currentConfig->encoderID = boca.GetComponentID(clicked_encoder);

	tab_layer_joblist->UpdateEncoderText();

	clicked_encoder = -1;

	Encode();
}

Void BonkEnc::BonkEncGUI::Encode()
{
	encoder->Encode(joblist);
}

Void BonkEnc::BonkEncGUI::PauseResumeEncoding()
{
	if (!encoder->IsEncoding()) return;

	if (encoder->IsPaused()) encoder->Resume();
	else			 encoder->Pause();
}

Void BonkEnc::BonkEncGUI::StopEncoding()
{
	if (encoder == NIL) return;

	encoder->Stop();
}

Void BonkEnc::BonkEncGUI::AddFilesByPattern()
{
	AddPatternDialog	*dialog = new AddPatternDialog();

	if (dialog->ShowDialog() == Success()) joblist->AddTracksByPattern(dialog->GetDirectory(), dialog->GetPattern());

	DeleteObject(dialog);
}

Void BonkEnc::BonkEncGUI::AddFilesFromDirectory()
{
	AddDirectoryDialog	*dialog = new AddDirectoryDialog();

	if (dialog->ShowDialog() == Success()) joblist->AddTrackByDragAndDrop(dialog->GetDirectory());

	DeleteObject(dialog);
}

Void BonkEnc::BonkEncGUI::ToggleUseInputDirectory()
{
	if (currentConfig->writeToInputDir) allowOverwriteMenuEntry->Activate();
	else				    allowOverwriteMenuEntry->Deactivate();
}

Void BonkEnc::BonkEncGUI::ToggleEncodeToSingleFile()
{
	if (currentConfig->encodeToSingleFile) currentConfig->enc_onTheFly = True;
}

Void BonkEnc::BonkEncGUI::ConfirmDeleteAfterEncoding()
{
	if (currentConfig->deleteAfterEncoding)
	{
		if (IDNO == QuickMessage(i18n->TranslateString("This option will remove the original files from your computer\nafter the encoding process!\n\nAre you sure you want to activate this function?"), i18n->TranslateString("Delete original files after encoding"), MB_YESNO, IDI_QUESTION)) currentConfig->deleteAfterEncoding = False;
	}
}

Void BonkEnc::BonkEncGUI::ShowHelp()
{
	ShellExecuteA(NIL, "open", String("file://").Append(GetApplicationDirectory()).Append("manual/").Append(i18n->TranslateString("index_en.html")), NIL, NIL, 0);
}

Void BonkEnc::BonkEncGUI::ShowTipOfTheDay()
{
	TipOfTheDay	*dlg = new TipOfTheDay(&currentConfig->showTips);

	dlg->AddTip(String(i18n->TranslateString("BonkEnc is available in %1 languages. If your language is\nnot available, you can easily translate BonkEnc using the\n\'smooth Translator\' application.")).Replace("%1", String::FromInt(Math::Max(32, i18n->GetNOfLanguages()))));
	dlg->AddTip(String(i18n->TranslateString("BonkEnc comes with support for the LAME, Ogg Vorbis, FAAC,\nFLAC and Bonk encoders. An encoder for the VQF format is\navailable at the BonkEnc website: %1")).Replace("%1", "http://www.bonkenc.org/"));
	dlg->AddTip(i18n->TranslateString("BonkEnc can use Winamp 2 input plug-ins to support more file\nformats. Copy the in_*.dll files to the BonkEnc/plugins directory to\nenable BonkEnc to read these formats."));
	dlg->AddTip(i18n->TranslateString("With BonkEnc you can submit freedb CD database entries\ncontaining Unicode characters. So if you have any CDs with\nnon-Latin artist or title names, you can submit the correct\nfreedb entries with BonkEnc."));
	dlg->AddTip(i18n->TranslateString("To correct reading errors while ripping you can enable\nJitter correction in the CDRip tab of BonkEnc's configuration\ndialog. If that does not help, try using one of the Paranoia modes."));
	dlg->AddTip(String(i18n->TranslateString("Do you have any suggestions on how to improve BonkEnc?\nYou can submit any ideas through the Tracker on the BonkEnc\nSourceForge project page - %1\nor send an eMail to %2.")).Replace("%1", "http://sf.net/projects/bonkenc").Replace("%2", "suggestions@bonkenc.org"));
	dlg->AddTip(String(i18n->TranslateString("Do you like BonkEnc? BonkEnc is available for free, but you can\nhelp fund the development by donating to the BonkEnc project.\nYou can send money to %1 through PayPal.\nSee %2 for more details.")).Replace("%1", "donate@bonkenc.org").Replace("%2", "http://www.bonkenc.org/donating.php"));

	dlg->SetMode(TIP_ORDERED, currentConfig->tipOffset, currentConfig->showTips);

	dlg->ShowDialog();

	currentConfig->tipOffset = dlg->GetOffset();

	DeleteObject(dlg);
}

Void BonkEnc::BonkEncGUI::CheckForUpdates()
{
	NonBlocking1<Bool>(&BonkEncGUI::CheckForUpdates, this).Call(False);
}

Int BonkEnc::BonkEncGUI::CheckForUpdates(Bool startup)
{
	if (!currentConfig->enable_eUpdate) return Success();

	if (currentConfig->firstStart)
	{
		if (QuickMessage(i18n->TranslateString("BonkEnc can perform an automatic check for online\nprogram updates at startup.\n\nWould you like BonkEnc to look for updates at startup?"), "BonkEnc easyUpdate", MB_YESNO, IDI_QUESTION) == IDNO)
		{
			currentConfig->checkUpdatesAtStartup = False;
			currentConfig->firstStart = False;

			return Success();
		}
	}

	Void	*context = ex_eUpdate_CreateUpdateContext("BonkEnc Audio Encoder", version, updatePath);

	if (currentConfig->configDir != NIL)
	{
		if (Setup::enableUnicode) ex_eUpdate_SetConfigFileW(context, String(currentConfig->configDir).Append("eUpdate.xml"));
		else			  ex_eUpdate_SetConfigFile(context, String(currentConfig->configDir).Append("eUpdate.xml"));
	}

	if (currentConfig->language != "internal")
	{
		String	 lang;

		for (Int i = 8; i < currentConfig->language.Length(); i++) lang[i - 8] = currentConfig->language[i];

		if (!ex_eUpdate_SetLanguage(context, String("eupdate_").Append(lang))) ex_eUpdate_SetLanguage(context, "internal");
	}
	else
	{
		ex_eUpdate_SetLanguage(context, "internal");
	}

	if (ex_eUpdate_CheckForNewUpdates(context, !startup) > 0)
	{
		MessageDlg	*msgBox = new MessageDlg(i18n->TranslateString("There are new updates for BonkEnc available online!\nWould you like to see a list of available updates now?"), "BonkEnc easyUpdate", MB_YESNO, IDI_QUESTION, i18n->TranslateString("Check for updates at startup"), &currentConfig->checkUpdatesAtStartup);

		msgBox->ShowDialog();

		if (msgBox->GetButtonCode() == IDYES)
		{
			currentConfig->SaveSettings();

			ex_eUpdate_AutomaticUpdate(context);
		}

		DeleteObject(msgBox);
	}
	else if (!startup)
	{
		MessageDlg	*msgBox = new MessageDlg(i18n->TranslateString("There are no updates available at the moment!"), "BonkEnc easyUpdate", MB_OK, IDI_INFORMATION, i18n->TranslateString("Check for updates at startup"), &currentConfig->checkUpdatesAtStartup);

		msgBox->ShowDialog();

		DeleteObject(msgBox);
	}
	else if (currentConfig->firstStart)
	{
		QuickMessage(i18n->TranslateString("There are no updates available at the moment!"), "BonkEnc easyUpdate", MB_OK, IDI_INFORMATION);
	}

	ex_eUpdate_FreeUpdateContext(context);

	currentConfig->firstStart = False;

	return Success();
}
