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

#include <gui/main_joblist.h>

#include <startgui.h>
#include <joblist.h>
#include <config.h>
#include <utilities.h>

#include <jobs/job.h>
#include <jobs/engine/convert.h>

#include <gui/playback.h>
#include <gui/edit_folder.h>

#include <dialogs/config/configcomponent.h>

#include <dialogs/charset.h>

using namespace smooth::GUI::Dialogs;

using namespace BoCA;
using namespace BoCA::AS;

freac::LayerJoblist::LayerJoblist() : Layer("Joblist")
{
	BoCA::Config	*config = BoCA::Config::Get();

	dontUpdateInfo		= False;

	clicked_charset		= -1;
	clicked_case		= -1;

	activePopup		= 0;

	previousTrackSeconds	= 0;
	previousTotalSeconds	= 0;

	Point		 pos;
	Size		 size;

	pos.x = 7;
	pos.y = 96;

	txt_filename		= new Text(NIL, pos);
	txt_filename->SetOrientation(OR_LOWERLEFT);

	txt_filters		= new Text(NIL, pos);
	txt_filters->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	txt_format		= new Text(NIL, pos);
	txt_format->SetOrientation(OR_LOWERLEFT);

	txt_encoder		= new Text(NIL, pos);
	txt_encoder->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	txt_progress		= new Text(NIL, pos);
	txt_progress->SetOrientation(OR_LOWERLEFT);

	txt_time		= new Text(NIL, pos);
	txt_time->SetOrientation(OR_LOWERRIGHT);

	pos.y -= 24;

	txt_outdir		= new Text(NIL, pos);
	txt_outdir->SetOrientation(OR_LOWERLEFT);

	pos.x = 16;
	pos.y = 24;
	size.cx = config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowSizeXID, Config::SettingsWindowSizeXDefault) - 29;
	size.cy = config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowSizeYID, Config::SettingsWindowSizeYDefault) - 264 - (config->GetIntValue(Config::CategorySettingsID, Config::SettingsShowTitleInfoID, Config::SettingsShowTitleInfoDefault) ? 68 : 0);

	joblist			= new JobList(pos, size);
	joblist->getContextMenu.Connect(&LayerJoblist::GetContextMenu, this);

	shortcut_previous	= new Shortcut(0, Input::Keyboard::KeyUp, joblist);
	shortcut_previous->onKeyDown.Connect(&LayerJoblist::OnShortcutPrevious, this);

	shortcut_next		= new Shortcut(0, Input::Keyboard::KeyDown, joblist);
	shortcut_next->onKeyDown.Connect(&LayerJoblist::OnShortcutNext, this);

	shortcut_first		= new Shortcut(0, Input::Keyboard::KeyHome, joblist);
	shortcut_first->onKeyDown.Connect(&LayerJoblist::OnShortcutFirst, this);

	shortcut_last		= new Shortcut(0, Input::Keyboard::KeyEnd, joblist);
	shortcut_last->onKeyDown.Connect(&LayerJoblist::OnShortcutLast, this);

	shortcut_move_up	= new Shortcut(SC_SHIFT, Input::Keyboard::KeyUp, joblist);
	shortcut_move_up->onKeyDown.Connect(&LayerJoblist::OnShortcutMoveUp, this);

	shortcut_move_down	= new Shortcut(SC_SHIFT, Input::Keyboard::KeyDown, joblist);
	shortcut_move_down->onKeyDown.Connect(&LayerJoblist::OnShortcutMoveDown, this);

	shortcut_move_top	= new Shortcut(SC_SHIFT, Input::Keyboard::KeyHome, joblist);
	shortcut_move_top->onKeyDown.Connect(&LayerJoblist::OnShortcutMoveTop, this);

	shortcut_move_bottom	= new Shortcut(SC_SHIFT, Input::Keyboard::KeyEnd, joblist);
	shortcut_move_bottom->onKeyDown.Connect(&LayerJoblist::OnShortcutMoveBottom, this);

	shortcut_toggle_mark	= new Shortcut(0, Input::Keyboard::KeySpace, joblist);
	shortcut_toggle_mark->onKeyDown.Connect(&LayerJoblist::OnShortcutToggleMark, this);

	shortcut_remove		= new Shortcut(0, Input::Keyboard::KeyDelete, joblist);
	shortcut_remove->onKeyDown.Connect(&LayerJoblist::OnShortcutRemove, this);

	pos.x = 200;
	pos.y += size.cy + 4;
	size.cx = 90;
	size.cy = 0;

	check_single		= new CheckBox(NIL, pos, size, (Bool *) &config->GetPersistentIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault));
	check_single->SetOrientation(OR_UPPERRIGHT);

	pos.x -= 100;

	check_playlist		= new CheckBox(NIL, pos, size, (Bool *) &config->GetPersistentIntValue(Config::CategoryPlaylistID, Config::PlaylistCreatePlaylistID, Config::PlaylistCreatePlaylistDefault));
	check_playlist->SetOrientation(OR_UPPERRIGHT);

	pos.x -= 100;

	check_cuesheet		= new CheckBox(NIL, pos, size, (Bool *) &config->GetPersistentIntValue(Config::CategoryPlaylistID, Config::PlaylistCreateCueSheetID, Config::PlaylistCreateCueSheetDefault));
	check_cuesheet->SetOrientation(OR_UPPERRIGHT);

	Registry	&boca = Registry::Get();

	if (boca.GetNumberOfComponentsOfType(COMPONENT_TYPE_OUTPUT) > 0)
	{
		pos.x = 242;
		pos.y = 0;

		player	= new PlaybackWidget(freacGUI::Get()->GetPlayer(), joblist);
		player->SetPosition(pos);
		player->SetOrientation(OR_UPPERRIGHT);
	}
	else
	{
		player	= NIL;
	}

	info_divider		= new Divider(113 + (config->GetIntValue(Config::CategorySettingsID, Config::SettingsShowTitleInfoID, Config::SettingsShowTitleInfoDefault) ? 68 : 0), OR_HORZ | OR_BOTTOM);
	info_bottom		= new Divider(113, OR_HORZ | OR_BOTTOM);

	pos.y = 121 + (config->GetIntValue(Config::CategorySettingsID, Config::SettingsShowTitleInfoID, Config::SettingsShowTitleInfoDefault) ? 68 : 0);
	pos.x = 5;
	size.cx = 90;
	size.cy = 17;

	info_background		= new Layer();
	info_background->SetMetrics(pos, size);
	info_background->SetOrientation(OR_LOWERLEFT);
	info_background->SetBackgroundColor(Setup::BackgroundColor);

	pos.y = 0;
	pos.x = 2;
	size.cx = 86;
	size.cy = 0;

	info_checkbox = new CheckBox(NIL, pos, size, (Bool *) &config->GetPersistentIntValue(Config::CategorySettingsID, Config::SettingsShowTitleInfoID, Config::SettingsShowTitleInfoDefault));
	info_checkbox->onAction.Connect(&LayerJoblist::ShowHideTitleInfo, this);

	info_area_cover	= new ActiveArea(Setup::BackgroundColor, Point(7, 167), Size(46, 46));
	info_area_cover->SetOrientation(OR_LOWERLEFT);
	info_area_cover->SetColor(Setup::ClientColor);
	info_area_cover->Hide();

	info_image_cover = new Image(NIL, Point(8, 166), Size(44, 44));
	info_image_cover->SetOrientation(OR_LOWERLEFT);
	info_image_cover->Hide();

	Add(info_area_cover);
	Add(info_image_cover);

	pos.x = 7;
	pos.y = 164;

	info_text_artist = new Text(NIL, pos);
	info_text_artist->SetOrientation(OR_LOWERLEFT);

	pos.y -= 27;

	info_text_album = new Text(NIL, pos);
	info_text_album->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + (Int) Math::Max(info_text_album->GetUnscaledTextWidth(), info_text_artist->GetUnscaledTextWidth()));
	pos.y += 30;
	size.cx = 180;
	size.cy = 0;

	info_edit_artist = new EditBox(pos, size, 0);
	info_edit_artist->onInput.Connect(&LayerJoblist::UpdateTitleInfo, this);
	info_edit_artist->onEnter.Connect(&LayerJoblist::OnEditBoxEnterArtist, this);
	info_edit_artist->SetOrientation(OR_LOWERLEFT);

	menu_edit_artist = new MicroMenu(Point(13, -7), Size(), OR_VERT);
	menu_edit_artist->SetOrientation(OR_UPPERRIGHT);
	menu_edit_artist->Hide();
	menu_edit_artist->onOpenPopupMenu.Connect(&LayerJoblist::ToggleEditPopup, this);
	menu_edit_artist->onClosePopupMenu.Connect(&LayerJoblist::ToggleEditPopup, this);

	htsp_edit_artist = new Hotspot(Point(0, -7), Size(info_edit_artist->GetWidth(), info_edit_artist->GetHeight() + 7));
	htsp_edit_artist->SetIndependent(True);
	htsp_edit_artist->onMouseOver.Connect(&MicroMenu::Show, menu_edit_artist);
	htsp_edit_artist->onMouseOut.Connect(&MicroMenu::Hide, menu_edit_artist);
	htsp_edit_artist->onActivate.Connect(&MicroMenu::Hide, menu_edit_artist);

	pos.y -= 27;

	info_edit_album = new EditBox(pos, size, 0);
	info_edit_album->onInput.Connect(&LayerJoblist::UpdateTitleInfo, this);
	info_edit_album->onEnter.Connect(&LayerJoblist::OnEditBoxEnterAlbum, this);
	info_edit_album->SetOrientation(OR_LOWERLEFT);

	menu_edit_album = new MicroMenu(Point(13, -7), Size(), OR_VERT);
	menu_edit_album->SetOrientation(OR_UPPERRIGHT);
	menu_edit_album->Hide();
	menu_edit_album->onOpenPopupMenu.Connect(&LayerJoblist::ToggleEditPopup, this);
	menu_edit_album->onClosePopupMenu.Connect(&LayerJoblist::ToggleEditPopup, this);

	htsp_edit_album = new Hotspot(Point(0, -7), Size(info_edit_album->GetWidth(), info_edit_album->GetHeight() + 7));
	htsp_edit_album->SetIndependent(True);
	htsp_edit_album->onMouseOver.Connect(&MicroMenu::Show, menu_edit_album);
	htsp_edit_album->onMouseOut.Connect(&MicroMenu::Hide, menu_edit_album);
	htsp_edit_album->onActivate.Connect(&MicroMenu::Hide, menu_edit_album);

	pos.x += (7 + size.cx);
	pos.y += 24;

	info_text_title = new Text(NIL, pos);
	info_text_title->SetOrientation(OR_LOWERLEFT);

	pos.y -= 27;

	info_text_track = new Text(NIL, pos);
	info_text_track->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + (Int) Math::Max(info_text_title->GetUnscaledTextWidth(), info_text_track->GetUnscaledTextWidth()));
	pos.y += 30;
	size.cx = 100;

	info_edit_title = new EditBox(pos, size, 0);
	info_edit_title->onInput.Connect(&LayerJoblist::UpdateTitleInfo, this);
	info_edit_title->onEnter.Connect(&LayerJoblist::OnEditBoxEnterTitle, this);
	info_edit_title->SetOrientation(OR_LOWERLEFT);

	menu_edit_title = new MicroMenu(Point(13, -7), Size(), OR_VERT);
	menu_edit_title->SetOrientation(OR_UPPERRIGHT);
	menu_edit_title->Hide();
	menu_edit_title->onOpenPopupMenu.Connect(&LayerJoblist::ToggleEditPopup, this);
	menu_edit_title->onClosePopupMenu.Connect(&LayerJoblist::ToggleEditPopup, this);

	htsp_edit_title = new Hotspot(Point(0, -7), Size(info_edit_title->GetWidth(), info_edit_title->GetHeight() + 7));
	htsp_edit_title->SetIndependent(True);
	htsp_edit_title->onMouseOver.Connect(&MicroMenu::Show, menu_edit_title);
	htsp_edit_title->onMouseOut.Connect(&MicroMenu::Hide, menu_edit_title);
	htsp_edit_title->onActivate.Connect(&MicroMenu::Hide, menu_edit_title);

	pos.y -= 27;
	size.cx = Font().GetUnscaledTextSizeX("000") + 7;

	info_edit_track = new EditBox(pos, size, 3);
	info_edit_track->SetFlags(EDB_NUMERIC);
	info_edit_track->onInput.Connect(&LayerJoblist::UpdateTitleInfo, this);
	info_edit_track->onEnter.Connect(&LayerJoblist::OnEditBoxEnterTrack, this);
	info_edit_track->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + size.cx);
	pos.y -= 3;

	info_text_year = new Text(NIL, pos);
	info_text_year->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + info_text_year->GetUnscaledTextWidth());
	pos.y += 3;
	size.cx = Font().GetUnscaledTextSizeX("0000") + 7;

	info_edit_year = new EditBox(pos, size, 4);
	info_edit_year->SetFlags(EDB_NUMERIC);
	info_edit_year->onInput.Connect(&LayerJoblist::UpdateTitleInfo, this);
	info_edit_year->onEnter.Connect(&LayerJoblist::OnEditBoxEnterYear, this);
	info_edit_year->SetOrientation(OR_LOWERLEFT);

	menu_edit_year = new MicroMenu(Point(13, -7), Size(), OR_VERT);
	menu_edit_year->SetOrientation(OR_UPPERRIGHT);
	menu_edit_year->Hide();
	menu_edit_year->onOpenPopupMenu.Connect(&LayerJoblist::ToggleEditPopup, this);
	menu_edit_year->onClosePopupMenu.Connect(&LayerJoblist::ToggleEditPopup, this);

	htsp_edit_year = new Hotspot(Point(0, -7), Size(info_edit_year->GetWidth(), info_edit_year->GetHeight() + 7));
	htsp_edit_year->SetIndependent(True);
	htsp_edit_year->onMouseOver.Connect(&MicroMenu::Show, menu_edit_year);
	htsp_edit_year->onMouseOut.Connect(&MicroMenu::Hide, menu_edit_year);
	htsp_edit_year->onActivate.Connect(&MicroMenu::Hide, menu_edit_year);

	pos.x += (7 + size.cx);
	pos.y -= 3;

	info_text_genre = new Text(NIL, pos);
	info_text_genre->SetOrientation(OR_LOWERLEFT);

	info_list_genre = new List();
	Utilities::FillGenreList(info_list_genre);

	pos.x += (7 + info_text_genre->GetUnscaledTextWidth());
	pos.y += 3;
	size.cx = 135;

	info_edit_genre = new EditBox(pos, size, 0);
	info_edit_genre->onInput.Connect(&LayerJoblist::UpdateTitleInfo, this);
	info_edit_genre->onEnter.Connect(&LayerJoblist::OnEditBoxEnterGenre, this);
	info_edit_genre->SetOrientation(OR_LOWERLEFT);
	info_edit_genre->SetDropDownList(info_list_genre);

	menu_edit_genre = new MicroMenu(Point(13, -7), Size(), OR_VERT);
	menu_edit_genre->SetOrientation(OR_UPPERRIGHT);
	menu_edit_genre->Hide();
	menu_edit_genre->onOpenPopupMenu.Connect(&LayerJoblist::ToggleEditPopup, this);
	menu_edit_genre->onClosePopupMenu.Connect(&LayerJoblist::ToggleEditPopup, this);

	menu_charsets		= new PopupMenu();
	menu_charsets_all	= new PopupMenu();
	menu_case		= new PopupMenu();
	menu_case_all		= new PopupMenu();

	htsp_edit_genre = new Hotspot(Point(0, -7), Size(info_edit_genre->GetWidth(), info_edit_genre->GetHeight() + 7));
	htsp_edit_genre->SetIndependent(True);
	htsp_edit_genre->onMouseOver.Connect(&MicroMenu::Show, menu_edit_genre);
	htsp_edit_genre->onMouseOut.Connect(&MicroMenu::Hide, menu_edit_genre);
	htsp_edit_genre->onActivate.Connect(&MicroMenu::Hide, menu_edit_genre);

	info_edit_artist->Deactivate();
	info_edit_title->Deactivate();
	info_edit_album->Deactivate();
	info_edit_track->Deactivate();
	info_edit_year->Deactivate();
	info_edit_genre->Deactivate();

	edb_filename = new EditBox(Point(0, 99), Size(0, 0), 1024);
	edb_filename->SetOrientation(OR_LOWERLEFT);
	edb_filename->Deactivate();

	btn_skip = new Button(Bitmap(), Point(0, 103), Size(27, 27));
	btn_skip->SetOrientation(OR_LOWERLEFT);
	btn_skip->SetFlags(BF_NOFRAME);
	btn_skip->onAction.Connect(&onRequestSkipTrack);
	btn_skip->Deactivate();

	edb_filters = new EditBox(Point(0, 99), Size(0, 0), 1024);
	edb_filters->SetOrientation(OR_LOWERLEFT);
	edb_filters->Deactivate();

	btn_configure_dsp = new Button(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/settings/settings-dsp.png")), Point(30, 103), Size(27, 27));
	btn_configure_dsp->SetOrientation(OR_LOWERRIGHT);
	btn_configure_dsp->SetFlags(BF_NOFRAME);
	btn_configure_dsp->onAction.Connect(&freacGUI::ConfigureProcessing, freacGUI::Get());

	edb_format = new EditBox(Point(0, 75), Size(0, 0), 4);
	edb_format->SetOrientation(OR_LOWERLEFT);
	edb_format->Deactivate();

	combo_encoder = new ComboBox(Point(0, 75), Size(0, 0));
	combo_encoder->SetOrientation(OR_LOWERLEFT);
	combo_encoder->onSelectEntry.Connect(&LayerJoblist::OnSelectEncoder, this);

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_ENCODER) continue;

		combo_encoder->AddEntry(boca.GetComponentName(i));
	}

	UpdateEncoderText();

	btn_configure_encoder = new Button(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/settings/settings-codec.png")), Point(30, 79), Size(27, 27));
	btn_configure_encoder->SetOrientation(OR_LOWERRIGHT);
	btn_configure_encoder->SetFlags(BF_NOFRAME);
	btn_configure_encoder->onAction.Connect(&freacGUI::ConfigureEncoder, freacGUI::Get());

	edb_outdir = new FolderEditBox(Point(0, 27), Size(0, 0), 1024);
	edb_outdir->SetOrientation(OR_LOWERLEFT);
	edb_outdir->onSelectEntry.Connect(&LayerJoblist::OnSelectFolder, this);
	list_outdir = new List();

	UpdateOutputDir();

	btn_open = new Button(String(), Point(173, 28), Size(0, 0));
	btn_open->SetOrientation(OR_LOWERRIGHT);
	btn_open->onAction.Connect(&LayerJoblist::OnOpenFolder, this);

	btn_browse = new Button(String(), Point(87, 28), Size(0, 0));
	btn_browse->SetOrientation(OR_LOWERRIGHT);
	btn_browse->onAction.Connect(&LayerJoblist::OnBrowseForFolder, this);

	progress = new Progressbar(Point(0, 51), Size(0, 10), OR_HORZ, PB_NOTEXT, 0, 1000, 0);
	progress->SetOrientation(OR_LOWERLEFT);
	progress->Deactivate();

	progress_total = new Progressbar(Point(0, 42), Size(0, 10), OR_HORZ, PB_NOTEXT, 0, 1000, 0);
	progress_total->SetOrientation(OR_LOWERLEFT);
	progress_total->Deactivate();

	edb_trackPercent = new EditBox(Point(0, 51), Size(33, 0), 4);
	edb_trackPercent->SetOrientation(OR_LOWERLEFT);
	edb_trackPercent->Deactivate();

	txt_splitPercent		= new Text("/", Point(0, 48));
	txt_splitPercent->SetOrientation(OR_LOWERLEFT);

	edb_totalPercent = new EditBox(Point(0, 51), Size(33, 0), 4);
	edb_totalPercent->SetOrientation(OR_LOWERLEFT);
	edb_totalPercent->Deactivate();

	edb_trackTime = new EditBox("00:00", Point(87, 51), Size(34, 0), 5);
	edb_trackTime->SetWidth(Math::Max(34, edb_trackTime->GetUnscaledTextWidth() + 6));
	edb_trackTime->SetOrientation(OR_LOWERRIGHT);
	edb_trackTime->Deactivate();

	txt_splitTime		= new Text("/", Point(50, 48));
	txt_splitTime->SetOrientation(OR_LOWERRIGHT);

	edb_totalTime = new EditBox("00:00", Point(41, 51), Size(34, 0), 5);
	edb_totalTime->SetWidth(Math::Max(34, edb_totalTime->GetUnscaledTextWidth() + 6));
	edb_totalTime->SetOrientation(OR_LOWERRIGHT);
	edb_totalTime->Deactivate();

	menu_trackmenu		= new PopupMenu();

	Add(joblist);

	Add(shortcut_previous);
	Add(shortcut_next);
	Add(shortcut_move_up);
	Add(shortcut_move_down);

	Add(check_single);

	if (boca.ComponentExists("cuesheet-playlist")) Add(check_cuesheet);

	if ( boca.GetNumberOfComponentsOfType(COMPONENT_TYPE_PLAYLIST) > 1 ||
	    (boca.GetNumberOfComponentsOfType(COMPONENT_TYPE_PLAYLIST) > 0 && !boca.ComponentExists("cuesheet-playlist"))) Add(check_playlist);

	if (player != NIL) Add(player);

	Add(info_divider);
	Add(info_bottom);
	Add(info_background);
	Add(info_text_artist);
	Add(info_edit_artist);
	Add(info_text_title);
	Add(info_edit_title);
	Add(info_text_album);
	Add(info_edit_album);
	Add(info_text_track);
	Add(info_edit_track);
	Add(info_text_year);
	Add(info_edit_year);
	Add(info_text_genre);
	Add(info_edit_genre);

	Add(txt_filename);
	Add(edb_filename);
	Add(btn_skip);

	Add(txt_filters);
	Add(edb_filters);
	Add(btn_configure_dsp);

	Add(txt_format);
	Add(edb_format);

	Add(txt_encoder);
	Add(combo_encoder);
	Add(btn_configure_encoder);

	Add(txt_progress);
	Add(progress_total);
	Add(progress);
	Add(edb_trackPercent);
	Add(txt_splitPercent);
	Add(edb_totalPercent);

	Add(txt_time);
	Add(edb_trackTime);
	Add(txt_splitTime);
	Add(edb_totalTime);

	Add(txt_outdir);
	Add(edb_outdir);
	Add(btn_open);
	Add(btn_browse);

	Add(shortcut_first);
	Add(shortcut_last);
	Add(shortcut_move_top);
	Add(shortcut_move_bottom);
	Add(shortcut_toggle_mark);
	Add(shortcut_remove);

	info_edit_artist->Add(menu_edit_artist);
	info_edit_artist->Add(htsp_edit_artist);
	info_edit_title->Add(menu_edit_title);
	info_edit_title->Add(htsp_edit_title);
	info_edit_album->Add(menu_edit_album);
	info_edit_album->Add(htsp_edit_album);
	info_edit_year->Add(menu_edit_year);
	info_edit_year->Add(htsp_edit_year);
	info_edit_genre->Add(menu_edit_genre);
	info_edit_genre->Add(htsp_edit_genre);

	info_background->Add(info_checkbox);

	if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsShowTitleInfoID, Config::SettingsShowTitleInfoDefault))
	{
		info_bottom->Hide();
		info_text_artist->Hide();
		info_edit_artist->Hide();
		info_text_title->Hide();
		info_edit_title->Hide();
		info_text_album->Hide();
		info_edit_album->Hide();
		info_text_track->Hide();
		info_edit_track->Hide();
		info_text_year->Hide();
		info_edit_year->Hide();
		info_text_genre->Hide();
		info_edit_genre->Hide();
	}

	OnChangeConfigurationSettings();

	/* Listen to configuration changes.
	 */
	BoCA::Settings::Get()->onChangeConfigurationSettings.Connect(&LayerJoblist::OnChangeConfigurationSettings, this);
	BoCA::Settings::Get()->onChangeLanguageSettings.Connect(&LayerJoblist::OnChangeLanguageSettings, this);

	/* Listen to joblist changes.
	 */
	BoCA::JobList::Get()->onApplicationModifyTrack.Connect(&LayerJoblist::OnJoblistModifyTrack, this);
	BoCA::JobList::Get()->onApplicationRemoveTrack.Connect(&LayerJoblist::OnJoblistRemoveTrack, this);
	BoCA::JobList::Get()->onApplicationSelectTrack.Connect(&LayerJoblist::OnJoblistSelectTrack, this);

	BoCA::JobList::Get()->onApplicationRemoveAllTracks.Connect(&LayerJoblist::OnJoblistRemoveAllTracks, this);

	/* Listen to conversion progress updates.
	 */
	JobConvert::onStartEncoding.Connect(&LayerJoblist::OnEncoderStartEncoding, this);
	JobConvert::onFinishEncoding.Connect(&LayerJoblist::OnEncoderFinishEncoding, this);

	JobConvert::onEncodeTrack.Connect(&LayerJoblist::OnEncoderEncodeTrack, this);
	JobConvert::onReportProgress.Connect(&LayerJoblist::OnEncoderReportProgress, this);

	/* Connect other slots.
	 */
	onChangeSize.Connect(&LayerJoblist::OnChangeSize, this);
}

freac::LayerJoblist::~LayerJoblist()
{
	/* Disconnect configuration change slots.
	 */
	BoCA::Settings::Get()->onChangeConfigurationSettings.Disconnect(&LayerJoblist::OnChangeConfigurationSettings, this);
	BoCA::Settings::Get()->onChangeLanguageSettings.Disconnect(&LayerJoblist::OnChangeLanguageSettings, this);

	/* Disconnect joblist change slots.
	 */
	BoCA::JobList::Get()->onApplicationModifyTrack.Disconnect(&LayerJoblist::OnJoblistModifyTrack, this);
	BoCA::JobList::Get()->onApplicationRemoveTrack.Disconnect(&LayerJoblist::OnJoblistRemoveTrack, this);
	BoCA::JobList::Get()->onApplicationSelectTrack.Disconnect(&LayerJoblist::OnJoblistSelectTrack, this);

	BoCA::JobList::Get()->onApplicationRemoveAllTracks.Disconnect(&LayerJoblist::OnJoblistRemoveAllTracks, this);

	/* Disconnect conversion progress update slots.
	 */
	JobConvert::onStartEncoding.Disconnect(&LayerJoblist::OnEncoderStartEncoding, this);
	JobConvert::onFinishEncoding.Disconnect(&LayerJoblist::OnEncoderFinishEncoding, this);

	JobConvert::onEncodeTrack.Disconnect(&LayerJoblist::OnEncoderEncodeTrack, this);
	JobConvert::onReportProgress.Disconnect(&LayerJoblist::OnEncoderReportProgress, this);

	/* Clear tracks.
	 */
	joblist->RemoveAllTracks();

	/* Delete widgets.
	 */
	if (player != NIL) DeleteObject(player);

	DeleteObject(joblist);

	DeleteObject(shortcut_previous);
	DeleteObject(shortcut_next);
	DeleteObject(shortcut_first);
	DeleteObject(shortcut_last);
	DeleteObject(shortcut_move_up);
	DeleteObject(shortcut_move_down);
	DeleteObject(shortcut_move_top);
	DeleteObject(shortcut_move_bottom);
	DeleteObject(shortcut_toggle_mark);
	DeleteObject(shortcut_remove);

	DeleteObject(check_single);
	DeleteObject(check_cuesheet);
	DeleteObject(check_playlist);

	DeleteObject(info_divider);
	DeleteObject(info_bottom);
	DeleteObject(info_background);
	DeleteObject(info_checkbox);
	DeleteObject(info_area_cover);
	DeleteObject(info_image_cover);
	DeleteObject(info_text_artist);
	DeleteObject(info_edit_artist);
	DeleteObject(info_text_title);
	DeleteObject(info_edit_title);
	DeleteObject(info_text_album);
	DeleteObject(info_edit_album);
	DeleteObject(info_text_track);
	DeleteObject(info_edit_track);
	DeleteObject(info_text_year);
	DeleteObject(info_edit_year);
	DeleteObject(info_text_genre);
	DeleteObject(info_edit_genre);
	DeleteObject(info_list_genre);

	DeleteObject(txt_filename);
	DeleteObject(edb_filename);
	DeleteObject(btn_skip);

	DeleteObject(txt_filters);
	DeleteObject(edb_filters);
	DeleteObject(btn_configure_dsp);

	DeleteObject(txt_format);
	DeleteObject(edb_format);

	DeleteObject(txt_encoder);
	DeleteObject(combo_encoder);
	DeleteObject(btn_configure_encoder);

	DeleteObject(txt_progress);
	DeleteObject(progress_total);
	DeleteObject(progress);
	DeleteObject(edb_trackPercent);
	DeleteObject(txt_splitPercent);
	DeleteObject(edb_totalPercent);

	DeleteObject(txt_time);
	DeleteObject(edb_trackTime);
	DeleteObject(txt_splitTime);
	DeleteObject(edb_totalTime);

	DeleteObject(txt_outdir);
	DeleteObject(edb_outdir);
	DeleteObject(btn_open);
	DeleteObject(btn_browse);

	DeleteObject(menu_edit_artist);
	DeleteObject(htsp_edit_artist);
	DeleteObject(menu_edit_title);
	DeleteObject(htsp_edit_title);
	DeleteObject(menu_edit_album);
	DeleteObject(htsp_edit_album);
	DeleteObject(menu_edit_year);
	DeleteObject(htsp_edit_year);
	DeleteObject(menu_edit_genre);
	DeleteObject(htsp_edit_genre);

	DeleteObject(menu_trackmenu);

	DeleteObject(menu_charsets);
	DeleteObject(menu_charsets_all);
	DeleteObject(menu_case);
	DeleteObject(menu_case_all);
}

Void freac::LayerJoblist::OnChangeSize(const Size &nSize)
{
	BoCA::Config	*config = BoCA::Config::Get();

	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	/* Update title info checkbox.
	 */
	Surface	*surface = GetDrawSurface();

	surface->StartPaint(Rect(info_background->GetRealPosition(), info_background->GetRealSize()));

	info_background->Hide();
	info_divider->SetPos(113 + (config->GetIntValue(Config::CategorySettingsID, Config::SettingsShowTitleInfoID, Config::SettingsShowTitleInfoDefault) ? 68 : 0));
	info_background->SetY(121 + (config->GetIntValue(Config::CategorySettingsID, Config::SettingsShowTitleInfoID, Config::SettingsShowTitleInfoDefault) ? 68 : 0));
	info_background->Show();

	surface->EndPaint();

	/* Update title info area.
	 */
	info_edit_title->SetX(clientSize.cx - 170 - info_text_genre->GetUnscaledTextWidth() - info_text_year->GetUnscaledTextWidth() - info_edit_year->GetWidth() - info_edit_track->GetWidth());
	info_edit_title->SetWidth(163 + info_text_genre->GetUnscaledTextWidth() + info_text_year->GetUnscaledTextWidth() + info_edit_year->GetWidth() + info_edit_track->GetWidth());
	info_edit_track->SetX(info_edit_title->GetX());
	info_text_year->SetX(info_edit_track->GetX() + info_edit_track->GetWidth() + 7);
	info_edit_year->SetX(info_text_year->GetX() + info_text_year->GetUnscaledTextWidth() + 7);
	info_text_genre->SetX(info_edit_year->GetX() + info_edit_year->GetWidth() + 7);
	info_text_title->SetX(info_edit_title->GetX() - (Int) Math::Max(info_text_title->GetUnscaledTextWidth(), info_text_track->GetUnscaledTextWidth()) - 7);
	info_text_track->SetX(info_text_title->GetX());
	info_edit_artist->SetWidth(clientSize.cx - 21 - info_edit_artist->GetX() - (Int) Math::Max(info_text_title->GetUnscaledTextWidth(), info_text_track->GetUnscaledTextWidth()) - info_edit_title->GetWidth());
	info_edit_album->SetWidth(info_edit_artist->GetWidth());
	info_edit_genre->SetX(clientSize.cx - 142);

	htsp_edit_title->SetWidth(info_edit_title->GetWidth());
	htsp_edit_artist->SetWidth(info_edit_artist->GetWidth());
	htsp_edit_album->SetWidth(info_edit_album->GetWidth());

	/* Update status area.
	 */
	Int	 maxTextSize = (Int) Math::Max(Math::Max(txt_progress->GetUnscaledTextWidth(), txt_outdir->GetUnscaledTextWidth()), Math::Max(txt_filename->GetUnscaledTextWidth(), txt_format->GetUnscaledTextWidth()));
	Int	 maxTextSize2 = (Int) Math::Max(txt_filters->GetUnscaledTextWidth(), txt_encoder->GetUnscaledTextWidth());

	txt_filters->SetX(clientSize.cx / 2 + maxTextSize2 - txt_filters->GetUnscaledTextWidth() + 7);
	edb_filters->SetX(clientSize.cx / 2 + maxTextSize2 + 14);

	txt_encoder->SetX(clientSize.cx / 2 + maxTextSize2 - txt_encoder->GetUnscaledTextWidth() + 7);
	combo_encoder->SetX(clientSize.cx / 2 + maxTextSize2 + 14);

	edb_filename->SetWidth(clientSize.cx / 2 - 10 - maxTextSize - btn_skip->GetWidth());
	edb_format->SetWidth(clientSize.cx / 2 - 14 - maxTextSize);
	edb_filters->SetWidth(Math::Ceil(Float(clientSize.cx) / 2.0) - 44 - maxTextSize2);
	combo_encoder->SetWidth(Math::Ceil(Float(clientSize.cx) / 2.0) - 44 - maxTextSize2);
	edb_outdir->SetWidth(clientSize.cx - 33 - maxTextSize - btn_browse->GetWidth() - btn_open->GetWidth());

	btn_skip->SetX(edb_filename->GetX() + edb_filename->GetWidth());

	/* Update progress bar and time-left display.
	 */
	surface->StartPaint(Rect::EncloseRect(progress->GetVisibleArea(), edb_totalTime->GetVisibleArea()));

	progress->Hide();
	progress_total->Hide();

	edb_trackPercent->Hide();
	txt_splitPercent->Hide();
	edb_totalPercent->Hide();

	txt_time->Hide();
	edb_trackTime->Hide();
	txt_splitTime->Hide();
	edb_totalTime->Hide();

	edb_totalTime->SetX(edb_totalTime->GetWidth() + 7);
	txt_splitTime->SetX(edb_totalTime->GetX() + 9);
	edb_trackTime->SetX(txt_splitTime->GetX() + edb_trackTime->GetWidth() + 3);
	txt_time->SetX(edb_trackTime->GetX() + txt_time->GetUnscaledTextWidth() + 7);

	progress->SetWidth(clientSize.cx - 125 - maxTextSize - txt_time->GetUnscaledTextWidth() - edb_trackTime->GetWidth() - edb_totalTime->GetWidth());
	progress_total->SetWidth(clientSize.cx - 125 - maxTextSize - txt_time->GetUnscaledTextWidth() - edb_trackTime->GetWidth() - edb_totalTime->GetWidth());

	edb_trackPercent->SetX(progress->GetX() + progress->GetWidth());
	txt_splitPercent->SetX(progress->GetX() + progress->GetWidth() + 36);
	edb_totalPercent->SetX(progress->GetX() + progress->GetWidth() + 45);

	progress_total->Show();
	progress->Show();

	edb_trackPercent->Show();
	txt_splitPercent->Show();
	edb_totalPercent->Show();

	txt_time->Show();
	edb_trackTime->Show();
	txt_splitTime->Show();
	edb_totalTime->Show();

	surface->EndPaint();

	/* Update joblist size and checkbox positions.
	 */
	joblist->SetSize(Size(clientSize.cx - 23, clientSize.cy - 162 - (config->GetIntValue(Config::CategorySettingsID, Config::SettingsShowTitleInfoID, Config::SettingsShowTitleInfoDefault) ? 68 : 0)));

	check_single->SetMetrics(Point(check_single->GetUnscaledTextWidth() + 28, joblist->GetY() + joblist->GetHeight() + 4), Size(check_single->GetUnscaledTextWidth() + 21, check_single->GetHeight()));
	check_cuesheet->SetMetrics(Point(check_single->GetUnscaledTextWidth() + check_cuesheet->GetUnscaledTextWidth() + 53, joblist->GetY() + joblist->GetHeight() + 4), Size(check_cuesheet->GetUnscaledTextWidth() + 21, check_cuesheet->GetHeight()));

	if (Registry::Get().ComponentExists("cuesheet-playlist")) check_playlist->SetMetrics(Point(check_single->GetUnscaledTextWidth() + check_cuesheet->GetUnscaledTextWidth() + check_playlist->GetUnscaledTextWidth() + 78, joblist->GetY() + joblist->GetHeight() + 4), Size(check_playlist->GetUnscaledTextWidth() + 21, check_playlist->GetHeight()));
	else							  check_playlist->SetMetrics(Point(check_single->GetUnscaledTextWidth() + check_playlist->GetUnscaledTextWidth() + 53, joblist->GetY() + joblist->GetHeight() + 4), Size(check_playlist->GetUnscaledTextWidth() + 21, check_playlist->GetHeight()));
}

Void freac::LayerJoblist::OnChangeConfigurationSettings()
{
	/* Get configuration.
	 */
	BoCA::Config	*config = BoCA::Config::Get();

	Bool	 enableProcessing = config->GetIntValue(Config::CategoryProcessingID, Config::ProcessingEnableProcessingID, Config::ProcessingEnableProcessingDefault);
	String	 selectedFilters  = config->GetStringValue(Config::CategoryProcessingID, Config::ProcessingComponentsID, Config::ProcessingComponentsDefault);

	/* Update title info area state.
	 */
	ShowHideTitleInfo();

	/* Update list of filters in status area.
	 */
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Joblist::Filters");

	String	 componentNames = i18n->TranslateString("disabled");

	if (enableProcessing)
	{
		componentNames = i18n->TranslateString("none");

		if (selectedFilters != NIL)
		{
			Registry	&boca = Registry::Get();

			componentNames = NIL;

			const Array<String>	&components = selectedFilters.Explode(",");

			foreach (const String &component, components)
			{
				AS::Component	*dsp = boca.CreateComponentByID(component);

				if (dsp == NIL) continue;

				componentNames.Append(componentNames != NIL ? ", " : NIL).Append(dsp->GetName());

				boca.DeleteComponent(dsp);
			}
		}
	}

	edb_filters->SetText(componentNames);
}

Void freac::LayerJoblist::OnChangeLanguageSettings()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Joblist");

	SetText(i18n->TranslateString("Joblist"));

	/* Hide all affected widgets prior to changing
	 * labels to avoid flickering.
	 */
	Bool	 prevVisible = IsVisible();

	if (prevVisible) Hide();

	/* Change labels of joblist  widgets.
	 */
	txt_filename->SetText(i18n->AddColon(i18n->TranslateString("Current file", "Joblist::File")));
	txt_filters->SetText(i18n->AddColon(i18n->TranslateString("Selected filters", "Joblist::Filters")));
	txt_format->SetText(i18n->AddColon(i18n->TranslateString("Active decoder", "Joblist::Decoder")));
	txt_encoder->SetText(i18n->AddColon(i18n->TranslateString("Selected encoder", "Joblist::Encoder")));
	txt_progress->SetText(i18n->AddColon(i18n->TranslateString("File progress")));
	txt_time->SetText(i18n->AddColon(i18n->TranslateString("Time left")));
	txt_outdir->SetText(i18n->AddColon(i18n->TranslateString("Output folder")));

	edb_filename->SetText(i18n->TranslateString("none", "Joblist::File"));
	edb_format->SetText(i18n->TranslateString("none", "Joblist::Decoder"));

	btn_skip->SetTooltipText(i18n->TranslateString("Skip current file", "Joblist::File"));

	btn_configure_dsp->SetTooltipText(i18n->TranslateString("Configure signal processing", "Joblist::Filters"));
	btn_configure_encoder->SetTooltipText(i18n->TranslateString("Configure selected encoder", "Joblist::Encoder"));

	btn_open->SetText(i18n->TranslateString("Open"));
	btn_browse->SetText(i18n->TranslateString("Select"));

	check_single->SetText(i18n->TranslateString("Encode to a single file"));
	check_cuesheet->SetText(i18n->TranslateString("Create cue sheet"));
	check_playlist->SetText(i18n->TranslateString("Create playlist"));

	info_text_artist->SetText(i18n->AddColon(i18n->TranslateString("Artist")));
	info_text_album->SetText(i18n->AddColon(i18n->TranslateString("Album")));
	info_text_title->SetText(i18n->AddColon(i18n->TranslateString("Title")));
	info_text_track->SetText(i18n->AddColon(i18n->TranslateString("Track")));
	info_text_year->SetText(i18n->AddColon(i18n->TranslateString("Year")));
	info_text_genre->SetText(i18n->AddColon(i18n->TranslateString("Genre")));

	/* Now correct position and size of affected widgets.
	 */
	Int	 maxTextSize = (Int) Math::Max(Math::Max(txt_progress->GetUnscaledTextWidth(), txt_outdir->GetUnscaledTextWidth()), Math::Max(txt_filename->GetUnscaledTextWidth(), txt_format->GetUnscaledTextWidth()));

	txt_progress->SetX(maxTextSize + 7 - txt_progress->GetUnscaledTextWidth());
	txt_outdir->SetX(maxTextSize + 7 - txt_outdir->GetUnscaledTextWidth());
	txt_filename->SetX(maxTextSize + 7 - txt_filename->GetUnscaledTextWidth());
	txt_format->SetX(maxTextSize + 7 - txt_format->GetUnscaledTextWidth());
	txt_time->SetX(edb_trackTime->GetX() + txt_time->GetUnscaledTextWidth() + 7);

	edb_filename->SetX(maxTextSize + 14);
	edb_format->SetX(maxTextSize + 14);
	edb_outdir->SetX(maxTextSize + 14);

	edb_trackPercent->SetText(i18n->TranslateString("%1%", "Technical").Replace("%1", "0"));
	edb_totalPercent->SetText(i18n->TranslateString("%1%", "Technical").Replace("%1", "0"));

	progress->SetX(maxTextSize + 14);
	progress_total->SetX(maxTextSize + 14);

	btn_browse->SetWidth(Math::Max(80, btn_browse->GetUnscaledTextWidth() + 13));

	Bitmap	 skipIcon = ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/other/skip.png"));

	if (i18n->IsActiveLanguageRightToLeft()) Utilities::MirrorBitmap(skipIcon);

	btn_skip->SetBitmap(skipIcon);
	btn_skip->SetX(btn_skip->GetWidth() + 7);
	btn_browse->SetX(btn_browse->GetWidth() + 7);

	btn_open->SetWidth(Math::Max(80, btn_open->GetUnscaledTextWidth() + 13));
	btn_open->SetX(btn_browse->GetWidth() + btn_open->GetWidth() + 13);

	if (progress_total->GetRealPosition().y < progress->GetRealPosition().y + progress->GetRealSize().cy - 1) progress_total->SetMetrics(progress_total->GetPosition() - Point(0, 1), progress_total->GetSize() - Size(0, 1));

	info_checkbox->SetText(i18n->TranslateString("Show title info"));
	info_checkbox->SetWidth(info_checkbox->GetUnscaledTextWidth() + 20);

	info_background->Hide();
	info_divider->Paint(SP_PAINT);
	info_background->SetWidth(info_checkbox->GetUnscaledTextWidth() + 24);
	info_background->Show();

	info_edit_artist->SetX(info_text_artist->GetX() + (Int) Math::Max(info_text_artist->GetUnscaledTextWidth(), info_text_album->GetUnscaledTextWidth()) + 8);
	info_edit_album->SetX(info_text_album->GetX() + (Int) Math::Max(info_text_artist->GetUnscaledTextWidth(), info_text_album->GetUnscaledTextWidth()) + 8);

	/* OnChangeSize will correct sizes of any other widgets.
	 */
	OnChangeSize(GetSize());

	/* Show all widgets again.
	 */
	if (prevVisible) Show();

	FillMenus();
}

Void freac::LayerJoblist::FillMenus()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Joblist");

	/* Fill track context menu.
	 */
	menu_trackmenu->RemoveAllEntries();

	if (Registry::Get().GetNumberOfComponentsOfType(COMPONENT_TYPE_OUTPUT) > 0)
	{
		menu_trackmenu->AddEntry(i18n->TranslateString("Play"))->onAction.Connect(&PlaybackWidget::PlaySelectedItem, player);
		menu_trackmenu->AddEntry(i18n->TranslateString("Stop"))->onAction.Connect(&PlaybackWidget::StopPlayback, player);
		menu_trackmenu->AddEntry();
	}

	menu_trackmenu->AddEntry(i18n->TranslateString("Remove"))->onAction.Connect(&JobList::RemoveSelectedTrack, joblist);
	menu_trackmenu->AddEntry();
	menu_trackmenu->AddEntry(i18n->TranslateString("Clear joblist"))->onAction.Connect(&JobList::StartJobRemoveAllTracks, joblist);
	menu_trackmenu->AddEntry();
	menu_trackmenu->AddEntry(i18n->TranslateString("Select all"))->onAction.Connect(&JobList::SelectAll, joblist);
	menu_trackmenu->AddEntry(i18n->TranslateString("Select none"))->onAction.Connect(&JobList::SelectNone, joblist);
	menu_trackmenu->AddEntry(i18n->TranslateString("Toggle selection"))->onAction.Connect(&JobList::ToggleSelection, joblist);

	/* Emit overlay signal.
	 */
	BoCA::Menu	*menu = BoCA::Menu::Get();

	menu->doContextMenuOverlay.Emit(menu_trackmenu);

	/* Fill character sets menu.
	 */
	menu_charsets->RemoveAllEntries();
	menu_charsets_all->RemoveAllEntries();

	menu_charsets->AddEntry("UTF-8", &clicked_charset, CHARSET_UTF_8)->onAction.Connect(&LayerJoblist::InterpretStringAs, this);
	menu_charsets->AddEntry();
	menu_charsets->AddEntry("ISO-8859-1", &clicked_charset, CHARSET_ISO_8859_1)->onAction.Connect(&LayerJoblist::InterpretStringAs, this);
	menu_charsets->AddEntry("ISO-8859-2", &clicked_charset, CHARSET_ISO_8859_2)->onAction.Connect(&LayerJoblist::InterpretStringAs, this);
	menu_charsets->AddEntry("ISO-8859-5", &clicked_charset, CHARSET_ISO_8859_5)->onAction.Connect(&LayerJoblist::InterpretStringAs, this);
	menu_charsets->AddEntry("ISO-8859-7", &clicked_charset, CHARSET_ISO_8859_7)->onAction.Connect(&LayerJoblist::InterpretStringAs, this);
	menu_charsets->AddEntry();
	menu_charsets->AddEntry("CP1251", &clicked_charset, CHARSET_CP1251)->onAction.Connect(&LayerJoblist::InterpretStringAs, this);
	menu_charsets->AddEntry();
	menu_charsets->AddEntry("SHIFT-JIS", &clicked_charset, CHARSET_SHIFT_JIS)->onAction.Connect(&LayerJoblist::InterpretStringAs, this);
	menu_charsets->AddEntry("GBK", &clicked_charset, CHARSET_GBK)->onAction.Connect(&LayerJoblist::InterpretStringAs, this);
	menu_charsets->AddEntry("BIG-5", &clicked_charset, CHARSET_BIG_5)->onAction.Connect(&LayerJoblist::InterpretStringAs, this);
	menu_charsets->AddEntry();
	menu_charsets->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Other")), &clicked_charset, CHARSET_OTHER)->onAction.Connect(&LayerJoblist::InterpretStringAs, this);

	menu_charsets_all->AddEntry("UTF-8", &clicked_charset, CHARSET_UTF_8)->onAction.Connect(&LayerJoblist::InterpretStringAsAll, this);
	menu_charsets_all->AddEntry();
	menu_charsets_all->AddEntry("ISO-8859-1", &clicked_charset, CHARSET_ISO_8859_1)->onAction.Connect(&LayerJoblist::InterpretStringAsAll, this);
	menu_charsets_all->AddEntry("ISO-8859-2", &clicked_charset, CHARSET_ISO_8859_2)->onAction.Connect(&LayerJoblist::InterpretStringAsAll, this);
	menu_charsets_all->AddEntry("ISO-8859-5", &clicked_charset, CHARSET_ISO_8859_5)->onAction.Connect(&LayerJoblist::InterpretStringAsAll, this);
	menu_charsets_all->AddEntry("ISO-8859-7", &clicked_charset, CHARSET_ISO_8859_7)->onAction.Connect(&LayerJoblist::InterpretStringAsAll, this);
	menu_charsets_all->AddEntry();
	menu_charsets_all->AddEntry("CP1251", &clicked_charset, CHARSET_CP1251)->onAction.Connect(&LayerJoblist::InterpretStringAsAll, this);
	menu_charsets_all->AddEntry();
	menu_charsets_all->AddEntry("SHIFT-JIS", &clicked_charset, CHARSET_SHIFT_JIS)->onAction.Connect(&LayerJoblist::InterpretStringAsAll, this);
	menu_charsets_all->AddEntry("GBK", &clicked_charset, CHARSET_GBK)->onAction.Connect(&LayerJoblist::InterpretStringAsAll, this);
	menu_charsets_all->AddEntry("BIG-5", &clicked_charset, CHARSET_BIG_5)->onAction.Connect(&LayerJoblist::InterpretStringAsAll, this);
	menu_charsets_all->AddEntry();
	menu_charsets_all->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Other")), &clicked_charset, CHARSET_OTHER)->onAction.Connect(&LayerJoblist::InterpretStringAsAll, this);

	/* Fill metadata edit menus.
	 */
	menu_edit_artist->RemoveAllEntries();
	menu_edit_title->RemoveAllEntries();
	menu_edit_album->RemoveAllEntries();
	menu_edit_year->RemoveAllEntries();
	menu_edit_genre->RemoveAllEntries();

	menu_edit_artist->AddEntry(i18n->TranslateString("Use for all selected tracks"))->onAction.Connect(&LayerJoblist::UseStringForSelectedTracks, this);
	menu_edit_artist->AddEntry();
	menu_edit_artist->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Adjust upper/lower case")), menu_case);
	menu_edit_artist->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Adjust upper/lower case")).Append(" (").Append(i18n->TranslateString("selected tracks")).Append(")"), NIL, menu_case_all);
	menu_edit_artist->AddEntry();
	menu_edit_artist->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Interpret string as")), menu_charsets);
	menu_edit_artist->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Interpret string as")).Append(" (").Append(i18n->TranslateString("selected tracks")).Append(")"), NIL, menu_charsets_all);

	menu_edit_title->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Adjust upper/lower case")), menu_case);
	menu_edit_title->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Adjust upper/lower case")).Append(" (").Append(i18n->TranslateString("selected tracks")).Append(")"), NIL, menu_case_all);
	menu_edit_title->AddEntry();
	menu_edit_title->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Interpret string as")), menu_charsets);
	menu_edit_title->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Interpret string as")).Append(" (").Append(i18n->TranslateString("selected tracks")).Append(")"), NIL, menu_charsets_all);

	menu_edit_album->AddEntry(i18n->TranslateString("Use for all selected tracks"))->onAction.Connect(&LayerJoblist::UseStringForSelectedTracks, this);
	menu_edit_album->AddEntry();
	menu_edit_album->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Adjust upper/lower case")), menu_case);
	menu_edit_album->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Adjust upper/lower case")).Append(" (").Append(i18n->TranslateString("selected tracks")).Append(")"), NIL, menu_case_all);
	menu_edit_album->AddEntry();
	menu_edit_album->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Interpret string as")), menu_charsets);
	menu_edit_album->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Interpret string as")).Append(" (").Append(i18n->TranslateString("selected tracks")).Append(")"), NIL, menu_charsets_all);

	menu_edit_genre->AddEntry(i18n->TranslateString("Use for all selected tracks"))->onAction.Connect(&LayerJoblist::UseStringForSelectedTracks, this);
	menu_edit_genre->AddEntry();
	menu_edit_genre->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Interpret string as")), menu_charsets);
	menu_edit_genre->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Interpret string as")).Append(" (").Append(i18n->TranslateString("selected tracks")).Append(")"), NIL, menu_charsets_all);

	menu_edit_year->AddEntry(i18n->TranslateString("Use for all selected tracks"))->onAction.Connect(&LayerJoblist::UseStringForSelectedTracks, this);
}

Void freac::LayerJoblist::UpdateEncoderText()
{
	if (!combo_encoder->IsActive()) return;

	BoCA::Config	*config = BoCA::Config::Get();
	Registry	&boca = Registry::Get();
	Component	*component = boca.CreateComponentByID(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault));

	if (component == NIL) component = boca.CreateComponentByID(Config::SettingsEncoderDefault);

	if (component != NIL)
	{
		combo_encoder->SelectEntry(component->GetName());

		boca.DeleteComponent(component);
	}
}

Void freac::LayerJoblist::OnJoblistSelectTrack(const Track &track)
{
	Surface	*surface = GetDrawSurface();

	surface->StartPaint(GetVisibleArea());

	if (track.pictures.Length() > 0) ShowCoverArea();
	else				 HideCoverArea();

	info_edit_artist->Activate();
	info_edit_title->Activate();
	info_edit_album->Activate();
	info_edit_track->Activate();
	info_edit_year->Activate();
	info_edit_genre->Activate();

	OnJoblistModifyTrack(track);

	surface->EndPaint();
}

Void freac::LayerJoblist::OnJoblistModifyTrack(const Track &track)
{
	if (joblist->GetSelectedTrack().GetTrackID() != track.GetTrackID()) return;

	const Info	&info = track.GetInfo();

	dontUpdateInfo = True;

	if (track.pictures.Length() > 0)
	{
		if (currentCover != track.pictures.GetFirst())
		{
			currentCover = track.pictures.GetFirst();

			info_image_cover->SetBitmap(currentCover.GetBitmap());
		}

		ShowCoverArea();
	}
	else
	{
		HideCoverArea();
	}

	info_edit_artist->SetText(info.artist);
	info_edit_title->SetText(info.title);
	info_edit_album->SetText(info.album);

	if	(info.track <=  0) info_edit_track->SetText(NIL);
	else if	(info.track <  10) info_edit_track->SetText(String("0").Append(String::FromInt(info.track)));
	else			   info_edit_track->SetText(String::FromInt(info.track));

	if (info.year <= 0) info_edit_year->SetText(NIL);
	else		    info_edit_year->SetText(String::FromInt(info.year));

	info_edit_genre->SetText(info.genre);

	dontUpdateInfo = False;
}

Void freac::LayerJoblist::OnJoblistRemoveTrack(const Track &track)
{
	/* Clear and deactivate edit boxes if the selected track is removed.
	 */
	if (joblist->GetSelectedTrack().GetTrackID() == track.GetTrackID())
	{
		dontUpdateInfo = True;

		Surface	*surface = GetDrawSurface();

		surface->StartPaint(GetVisibleArea());

		HideCoverArea();

		info_edit_artist->SetText(NIL);
		info_edit_title->SetText(NIL);
		info_edit_album->SetText(NIL);
		info_edit_track->SetText(NIL);
		info_edit_year->SetText(NIL);
		info_edit_genre->SetText(NIL);

		info_edit_artist->Deactivate();
		info_edit_title->Deactivate();
		info_edit_album->Deactivate();
		info_edit_track->Deactivate();
		info_edit_year->Deactivate();
		info_edit_genre->Deactivate();

		surface->EndPaint();

		dontUpdateInfo = False;
	}
}

Void freac::LayerJoblist::OnJoblistRemoveAllTracks()
{
	/* Clear and deactivate edit boxes.
	 */
	dontUpdateInfo = True;

	Surface	*surface = GetDrawSurface();

	surface->StartPaint(GetVisibleArea());

	HideCoverArea();

	info_edit_artist->SetText(NIL);
	info_edit_title->SetText(NIL);
	info_edit_album->SetText(NIL);
	info_edit_track->SetText(NIL);
	info_edit_year->SetText(NIL);
	info_edit_genre->SetText(NIL);

	info_edit_artist->Deactivate();
	info_edit_title->Deactivate();
	info_edit_album->Deactivate();
	info_edit_track->Deactivate();
	info_edit_year->Deactivate();
	info_edit_genre->Deactivate();

	surface->EndPaint();

	dontUpdateInfo = False;
}

EditBox *freac::LayerJoblist::GetActiveEditBox()
{
	if	(info_edit_artist->IsFocussed()) return info_edit_artist;
	else if	(info_edit_title->IsFocussed())	 return info_edit_title;
	else if	(info_edit_album->IsFocussed())	 return info_edit_album;
	else if	(info_edit_track->IsFocussed())	 return info_edit_track;
	else if	(info_edit_year->IsFocussed())	 return info_edit_year;
	else if	(info_edit_genre->IsFocussed())	 return info_edit_genre;

	return NIL;
}

Void freac::LayerJoblist::FocusEditBox(EditBox *editBox)
{
	editBox->SetFocus();
	editBox->MarkAll();
}

Void freac::LayerJoblist::ShowCoverArea()
{
	if (info_text_artist->GetX() > 7) return;

	htsp_edit_artist->SetWidth(htsp_edit_artist->GetWidth() - 54);
	htsp_edit_album->SetWidth(htsp_edit_album->GetWidth() - 54);

	info_edit_artist->SetWidth(info_edit_artist->GetWidth() - 54);
	info_edit_album->SetWidth(info_edit_album->GetWidth() - 54);

	info_edit_artist->SetX(info_edit_artist->GetX() + 54);
	info_edit_album->SetX(info_edit_album->GetX() + 54);

	info_text_artist->SetX(info_text_artist->GetX() + 54);
	info_text_album->SetX(info_text_album->GetX() + 54);

	BoCA::Config	*config = BoCA::Config::Get();

	if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsShowTitleInfoID, Config::SettingsShowTitleInfoDefault))
	{
		info_area_cover->Show();
		info_image_cover->Show();
	}
}

Void freac::LayerJoblist::HideCoverArea()
{
	if (info_text_artist->GetX() == 7) return;

	BoCA::Config	*config = BoCA::Config::Get();

	if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsShowTitleInfoID, Config::SettingsShowTitleInfoDefault))
	{
		info_area_cover->Hide();
		info_image_cover->Hide();
	}

	info_text_artist->SetX(info_text_artist->GetX() - 54);
	info_text_album->SetX(info_text_album->GetX() - 54);

	info_edit_artist->SetX(info_edit_artist->GetX() - 54);
	info_edit_album->SetX(info_edit_album->GetX() - 54);

	info_edit_artist->SetWidth(info_edit_artist->GetWidth() + 54);
	info_edit_album->SetWidth(info_edit_album->GetWidth() + 54);

	htsp_edit_artist->SetWidth(htsp_edit_artist->GetWidth() + 54);
	htsp_edit_album->SetWidth(htsp_edit_album->GetWidth() + 54);
}

Void freac::LayerJoblist::OnShortcutPrevious()
{
	if (!IsVisible()) return;

	EditBox	*activeEditBox = GetActiveEditBox();

	if (joblist->GetSelectedEntryNumber() == -1) joblist->SelectNthEntry(joblist->Length()		       - 1);
	else					     joblist->SelectNthEntry(joblist->GetSelectedEntryNumber() - 1);

	if (activeEditBox != NIL) FocusEditBox(activeEditBox);
	else			  FocusEditBox(info_edit_artist);
}

Void freac::LayerJoblist::OnShortcutNext()
{
	if (!IsVisible()) return;

	EditBox	*activeEditBox = GetActiveEditBox();

	joblist->SelectNthEntry(joblist->GetSelectedEntryNumber() + 1);

	if (activeEditBox != NIL) FocusEditBox(activeEditBox);
	else			  FocusEditBox(info_edit_artist);
}

Void freac::LayerJoblist::OnShortcutFirst()
{
	if (!IsVisible() || GetActiveEditBox() != NIL) return;

	EditBox	*activeEditBox = GetActiveEditBox();

	joblist->SelectNthEntry(0);

	if (activeEditBox != NIL) FocusEditBox(activeEditBox);
	else			  FocusEditBox(info_edit_artist);
}

Void freac::LayerJoblist::OnShortcutLast()
{
	if (!IsVisible() || GetActiveEditBox() != NIL) return;

	EditBox	*activeEditBox = GetActiveEditBox();

	joblist->SelectNthEntry(joblist->Length() - 1);

	if (activeEditBox != NIL) FocusEditBox(activeEditBox);
	else			  FocusEditBox(info_edit_artist);
}

Void freac::LayerJoblist::OnShortcutMoveUp()
{
	if (!IsVisible() || joblist->GetSelectedEntryNumber() == -1) return;

	if (joblist->GetSelectedEntryNumber() > 0)
	{
		Surface	*surface = GetDrawSurface();

		surface->StartPaint(Rect(joblist->GetRealPosition(), joblist->GetRealSize()));

		joblist->MoveEntry(joblist->GetSelectedEntryNumber(), joblist->GetSelectedEntryNumber() - 1);
		joblist->Paint(SP_PAINT);

		while (joblist->GetSelectedEntry()->GetVisibleArea().GetHeight() < joblist->GetSelectedEntry()->GetRealSize().cy) joblist->ScrollUp(1);

		surface->EndPaint();
	}
}

Void freac::LayerJoblist::OnShortcutMoveDown()
{
	if (!IsVisible() || joblist->GetSelectedEntryNumber() == -1) return;

	if (joblist->GetSelectedEntryNumber() < joblist->Length() - 1)
	{
		Surface	*surface = GetDrawSurface();

		surface->StartPaint(Rect(joblist->GetRealPosition(), joblist->GetRealSize()));

		joblist->MoveEntry(joblist->GetSelectedEntryNumber(), joblist->GetSelectedEntryNumber() + 1);
		joblist->Paint(SP_PAINT);

		while (joblist->GetSelectedEntry()->GetVisibleArea().GetHeight() < joblist->GetSelectedEntry()->GetRealSize().cy) joblist->ScrollDown(1);

		surface->EndPaint();
	}
}

Void freac::LayerJoblist::OnShortcutMoveTop()
{
	if (!IsVisible() || GetActiveEditBox() != NIL || joblist->GetSelectedEntryNumber() == -1) return;

	if (joblist->GetSelectedEntryNumber() > 0)
	{
		Surface	*surface = GetDrawSurface();

		surface->StartPaint(Rect(joblist->GetRealPosition(), joblist->GetRealSize()));

		joblist->MoveEntry(joblist->GetSelectedEntryNumber(), 0);
		joblist->ScrollUp();
		joblist->Paint(SP_PAINT);

		surface->EndPaint();
	}
}

Void freac::LayerJoblist::OnShortcutMoveBottom()
{
	if (!IsVisible() || GetActiveEditBox() != NIL || joblist->GetSelectedEntryNumber() == -1) return;

	if (joblist->GetSelectedEntryNumber() < joblist->Length() - 1)
	{
		Surface	*surface = GetDrawSurface();

		surface->StartPaint(Rect(joblist->GetRealPosition(), joblist->GetRealSize()));

		joblist->MoveEntry(joblist->GetSelectedEntryNumber(), joblist->Length() - 1);
		joblist->ScrollDown();
		joblist->Paint(SP_PAINT);

		surface->EndPaint();
	}
}

Void freac::LayerJoblist::OnShortcutToggleMark()
{
	if (!IsVisible() || GetActiveEditBox() != NIL) return;

	ListEntry	*entry = joblist->GetSelectedEntry();

	if (entry != NIL)
	{
		entry->SetMark(!entry->IsMarked());
	}
}

Void freac::LayerJoblist::OnShortcutRemove()
{
	if (!IsVisible() || GetActiveEditBox() != NIL) return;

	Int	 entryNumber = joblist->GetSelectedEntryNumber();

	if (entryNumber >= 0)
	{
		joblist->RemoveSelectedTrack();
		joblist->SelectNthEntry(entryNumber);
	}
}

Void freac::LayerJoblist::OnEditBoxEnter(EditBox *activeEditBox)
{
	joblist->SelectNthEntry(joblist->GetSelectedEntryNumber() + 1);

	if (activeEditBox != NIL) FocusEditBox(activeEditBox);
}

Void freac::LayerJoblist::UpdateTitleInfo()
{
	if (dontUpdateInfo) return;

	Track	 track = joblist->GetSelectedTrack();
	Info	 info = track.GetInfo();

	if (track == NIL) return;

	if (info.genre != info_edit_genre->GetText() && info_list_genre->GetEntry(info_edit_genre->GetText()) != NIL)
	{
		Utilities::UpdateGenreList(info_list_genre, info_edit_genre->GetText());

		info_edit_genre->SetDropDownList(info_list_genre);
	}

	info.artist	= info_edit_artist->GetText();
	info.title	= info_edit_title->GetText();
	info.album	= info_edit_album->GetText();
	info.track	= info_edit_track->GetText().ToInt();
	info.year	= info_edit_year->GetText().ToInt();
	info.genre	= info_edit_genre->GetText();

	track.SetInfo(info);

	BoCA::JobList::Get()->onComponentModifyTrack.Emit(track);
}

PopupMenu *freac::LayerJoblist::GetContextMenu()
{
	/* Select the entry under the cursor before opening the context menu.
	 */
	Process(SM_LBUTTONDOWN, 0, 0);
	Process(SM_LBUTTONUP, 0, 0);

	if (joblist->GetSelectedTrack() != NIL) return menu_trackmenu;

	return NIL;
}

Void freac::LayerJoblist::OnEncoderStartEncoding()
{
	OnEncoderFinishEncoding(True);

	BoCA::Config	*config	= BoCA::Config::Get();

	if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault)) btn_skip->Activate();

	combo_encoder->Deactivate();
}

Void freac::LayerJoblist::OnEncoderFinishEncoding(Bool success)
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Joblist");

	edb_trackPercent->SetText(i18n->TranslateString("%1%", "Technical").Replace("%1", "0"));
	edb_trackTime->SetText("00:00");

	edb_totalPercent->SetText(i18n->TranslateString("%1%", "Technical").Replace("%1", "0"));
	edb_totalTime->SetText("00:00");

	if (edb_trackTime->GetWidth() != Math::Max(34, edb_trackTime->GetUnscaledTextWidth() + 6) ||
	    edb_totalTime->GetWidth() != Math::Max(34, edb_totalTime->GetUnscaledTextWidth() + 6))
	{
		edb_trackTime->SetWidth(Math::Max(34, edb_trackTime->GetUnscaledTextWidth() + 6));
		edb_totalTime->SetWidth(Math::Max(34, edb_totalTime->GetUnscaledTextWidth() + 6));

		OnChangeSize(GetSize());
	}

	progress->SetValue(0);
	progress_total->SetValue(0);

	previousTrackSeconds = -10;
	previousTotalSeconds = -10;

	if (JobConvert::IsConverting()) return;

	edb_filename->SetText(i18n->TranslateString("none", "Joblist::File"));
	edb_format->SetText(i18n->TranslateString("none", "Joblist::Decoder"));

	btn_skip->Deactivate();

	combo_encoder->Activate();

	UpdateEncoderText();

	/* Shutdown system if requested.
	 */
	if (success && Job::GetPlannedJobs().Length() == 0 && Config::Get()->shutdownAfterEncoding) S::System::System::Shutdown();
}

Void freac::LayerJoblist::OnEncoderEncodeTrack(const Track &track, const String &decoderName, const String &encoderName, ConversionStep mode)
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Joblist");

	Surface	*surface = GetDrawSurface();

	surface->StartPaint(Rect::EncloseRect(edb_filename->GetVisibleArea(), edb_totalTime->GetVisibleArea()));

	edb_trackPercent->SetText(i18n->TranslateString("%1%", "Technical").Replace("%1", "0"));
	edb_trackTime->SetText("00:00");

	if (edb_trackTime->GetWidth() != Math::Max(34, edb_trackTime->GetUnscaledTextWidth() + 6))
	{
		edb_trackTime->SetWidth(Math::Max(34, edb_trackTime->GetUnscaledTextWidth() + 6));

		OnChangeSize(GetSize());
	}

	progress->SetValue(0);

	previousTrackSeconds = -10;

	const Info	&info	  = track.GetInfo();
	String		 fileInfo = track.fileName;

	if (info.artist.Length() > 0 ||
	    info.title.Length()	 > 0) fileInfo = String(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist")).Append(" - ").Append(info.title.Length() > 0 ? info.title : i18n->TranslateString("unknown title"));

	switch (mode)
	{
		default:
		case ConversionStepOnTheFly:
			// nothing special in this case
			break;
		case ConversionStepDecode:
			fileInfo = fileInfo.Append(" (").Append(i18n->TranslateString("ripping/decoding")).Append(")");
			break;
		case ConversionStepEncode:
			fileInfo = fileInfo.Append(" (").Append(i18n->TranslateString("encoding")).Append(")");
			break;
		case ConversionStepVerify:
			fileInfo = fileInfo.Append(" (").Append(i18n->TranslateString("verifying")).Append(")");
			break;
	}

	edb_filename->SetText(fileInfo);
	edb_format->SetText(decoderName);

	combo_encoder->SelectEntry(encoderName);

	surface->EndPaint();
}

Void freac::LayerJoblist::OnEncoderReportProgress(Int trackProgressValue, Int trackSecondsLeft, Int totalProgressValue, Int totalSecondsLeft)
{
	Surface	*surface = GetDrawSurface();

	surface->StartPaint(Rect::EncloseRect(progress->GetVisibleArea(), edb_totalTime->GetVisibleArea()));

	/* Update seconds only if estimate is less or
	 * at least two seconds more than before.
	 */
	if (trackSecondsLeft < previousTrackSeconds || trackSecondsLeft >= previousTrackSeconds + 2 || previousTrackSeconds == 0)
	{
		/* Set track time string.
		 */
		String	 secondsString = Job::SecondsToString(trackSecondsLeft);

		edb_trackTime->SetText(secondsString);

		if (edb_trackTime->GetWidth() != Math::Max(34, edb_trackTime->GetUnscaledTextWidth() + 6))
		{
			edb_trackTime->SetWidth(Math::Max(34, edb_trackTime->GetUnscaledTextWidth() + 6));

			OnChangeSize(GetSize());
		}

		previousTrackSeconds = trackSecondsLeft;
	}

	if (totalSecondsLeft < previousTotalSeconds || totalSecondsLeft >= previousTotalSeconds + 2 || previousTotalSeconds == 0)
	{
		/* Set total time string.
		 */
		String	 secondsString = Job::SecondsToString(totalSecondsLeft);

		edb_totalTime->SetText(secondsString);

		if (edb_totalTime->GetWidth() != Math::Max(34, edb_totalTime->GetUnscaledTextWidth() + 6))
		{
			edb_totalTime->SetWidth(Math::Max(34, edb_totalTime->GetUnscaledTextWidth() + 6));

			OnChangeSize(GetSize());
		}

		previousTotalSeconds = totalSecondsLeft;
	}

	/* Set percent values.
	 */
	if (trackProgressValue > progress->GetValue())
	{
		edb_trackPercent->SetText(BoCA::I18n::Get()->TranslateString("%1%", "Technical").Replace("%1", String::FromInt(Math::Round(trackProgressValue / 10.0))));

		progress->SetValue(trackProgressValue);
	}

	if (totalProgressValue > progress_total->GetValue())
	{
		edb_totalPercent->SetText(BoCA::I18n::Get()->TranslateString("%1%", "Technical").Replace("%1", String::FromInt(Math::Round(totalProgressValue / 10.0))));

		progress_total->SetValue(totalProgressValue);
	}

	surface->EndPaint();
}

Void freac::LayerJoblist::ShowHideTitleInfo()
{
	BoCA::Config	*config = BoCA::Config::Get();

	Bool	 showTitleInfo = config->GetIntValue(Config::CategorySettingsID, Config::SettingsShowTitleInfoID, Config::SettingsShowTitleInfoDefault);

	/* Check if visibility state actually changed.
	 */
	if (( showTitleInfo && info_divider->GetPos() != 113) ||
	    (!showTitleInfo && info_divider->GetPos() == 113)) return;

	/* Toggle info area visibility state.
	 */
	Surface	*surface = GetDrawSurface();

	surface->StartPaint(GetVisibleArea());

	if (!showTitleInfo)
	{
		info_bottom->Hide();

		if (info_text_artist->GetX() > 7)
		{
			info_area_cover->Hide();
			info_image_cover->Hide();
		}

		info_text_artist->Hide();
		info_edit_artist->Hide();
		info_text_title->Hide();
		info_edit_title->Hide();
		info_text_album->Hide();
		info_edit_album->Hide();
		info_text_track->Hide();
		info_edit_track->Hide();
		info_text_year->Hide();
		info_edit_year->Hide();
		info_text_genre->Hide();
		info_edit_genre->Hide();
	}

	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	joblist->SetSize(Size(clientSize.cx - 23, clientSize.cy - 162 - (showTitleInfo ? 68 : 0)));

	check_single->SetMetrics(Point(check_single->GetUnscaledTextWidth() + 28, joblist->GetY() + joblist->GetHeight() + 4), Size(check_single->GetUnscaledTextWidth() + 21, check_single->GetHeight()));
	check_cuesheet->SetMetrics(Point(check_single->GetUnscaledTextWidth() + check_cuesheet->GetUnscaledTextWidth() + 53, joblist->GetY() + joblist->GetHeight() + 4), Size(check_cuesheet->GetUnscaledTextWidth() + 21, check_cuesheet->GetHeight()));
	check_playlist->SetMetrics(Point(check_single->GetUnscaledTextWidth() + check_cuesheet->GetUnscaledTextWidth() + check_playlist->GetUnscaledTextWidth() + 78, joblist->GetY() + joblist->GetHeight() + 4), Size(check_playlist->GetUnscaledTextWidth() + 21, check_playlist->GetHeight()));

	info_divider->SetPos(113 + (showTitleInfo ? 68 : 0));
	info_background->SetY(121 + (showTitleInfo ? 68 : 0));

	joblist->Paint(SP_PAINT);

	if (showTitleInfo)
	{
		info_bottom->Show();

		if (info_text_artist->GetX() > 7)
		{
			info_area_cover->Show();
			info_image_cover->Show();
		}

		info_text_artist->Show();
		info_edit_artist->Show();
		info_text_title->Show();
		info_edit_title->Show();
		info_text_album->Show();
		info_edit_album->Show();
		info_text_track->Show();
		info_edit_track->Show();
		info_text_year->Show();
		info_edit_year->Show();
		info_text_genre->Show();
		info_edit_genre->Show();
	}

	surface->EndPaint();
}

Void freac::LayerJoblist::UpdateOutputDir()
{
	BoCA::Config	*config = BoCA::Config::Get();

	edb_outdir->SetText(BoCA::Utilities::GetAbsolutePathName(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault)));

	/* Update combo box.
	 */
	list_outdir->RemoveAllEntries();

	for (Int i = 1; i <= 5; i++)
	{
		if (config->GetStringValue(Config::CategorySettingsID, String(Config::SettingsLastOutputDirectoryID).Append(String::FromInt(i)), NIL) != NIL)
		{
			list_outdir->AddEntry(config->GetStringValue(Config::CategorySettingsID, String(Config::SettingsLastOutputDirectoryID).Append(String::FromInt(i)), NIL));
		}
	}

	edb_outdir->SetDropDownList(list_outdir);
}

Void freac::LayerJoblist::OnOpenFolder()
{
	Directory	 outputDirectory(BoCA::Utilities::GetAbsolutePathName(edb_outdir->GetText()));

	if (!outputDirectory.Exists())
	{
		BoCA::Utilities::ErrorMessage("The selected folder does not exist anymore!");

		return;
	}

	S::System::System::OpenURL(edb_outdir->GetText());
}

Void freac::LayerJoblist::OnBrowseForFolder()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Joblist");

	/* Find main window and create dialog.
	 */
	Window		*mainWnd = Window::GetNthWindow(0);
	DirSelection	 dialog;

	dialog.SetParentWindow(mainWnd);
	dialog.SetCaption(String("\n").Append(i18n->AddColon(i18n->TranslateString("Select the folder in which the encoded files will be placed"))));
	dialog.SetDirName(BoCA::Utilities::GetAbsolutePathName(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault)));

	if (dialog.ShowDialog() == Success())
	{
		edb_outdir->SetText(dialog.GetDirName());

		OnSelectFolder();
	}
}

Void freac::LayerJoblist::OnSelectFolder()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Encoders::Errors");

	/* Check if output folder exists.
	 */
	Directory	 outputDirectory(BoCA::Utilities::GetAbsolutePathName(edb_outdir->GetText()));

	if (Directory::SetActiveDirectory(outputDirectory) != Success())
	{
		Int	 selection = QuickMessage(i18n->TranslateString("The output folder does not exist! Do you want to create it?"), i18n->TranslateString("Error"), Message::Buttons::YesNoCancel, Message::Icon::Question);

		if	(selection == Message::Button::Yes)	outputDirectory.Create();
		else if (selection == Message::Button::Cancel)	{ UpdateOutputDir(); return; }
	}

	Directory::SetActiveDirectory(GUI::Application::GetApplicationDirectory());

	/* Save output directory and list of last used folders.
	 */
	Int	 outputDirNumber = 5;

	for (Int i = 1; i <= 5; i++)
	{
		if (config->GetStringValue(Config::CategorySettingsID, String(Config::SettingsLastOutputDirectoryID).Append(String::FromInt(i)), NIL) == edb_outdir->GetText())
		{
			outputDirNumber = i;

			break;
		}
	}

	for (Int i = outputDirNumber; i > 1; i--)
	{
		config->SetStringValue(Config::CategorySettingsID, String(Config::SettingsLastOutputDirectoryID).Append(String::FromInt(i)), config->GetStringValue(Config::CategorySettingsID, String(Config::SettingsLastOutputDirectoryID).Append(String::FromInt(i - 1)), NIL));
	}

	config->SetStringValue(Config::CategorySettingsID, String(Config::SettingsLastOutputDirectoryID).Append(String::FromInt(1)), edb_outdir->GetText());
	config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, edb_outdir->GetText());

	/* Update combo box.
	 */
	UpdateOutputDir();
}

Void freac::LayerJoblist::OnSelectEncoder()
{
	BoCA::Config	*config = BoCA::Config::Get();
	Registry	&boca	= Registry::Get();

	for (Int i = 0, n = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_ENCODER) continue;

		if (combo_encoder->GetSelectedEntryNumber() == n++)
		{
			config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, boca.GetComponentID(i));

			break;
		}
	}

	/* Update joblist entries if output file name might need to be changed.
	 */
	if (config->GetStringValue(Config::CategoryJoblistID, Config::JoblistFieldsID, Config::JoblistFieldsDefault).Contains("<outputfile>")) joblist->OnChangeHeaderColumns();
}

Void freac::LayerJoblist::ToggleEditPopup()
{
	String	 string;

	if (menu_edit_artist->IsVisible())
	{
		activePopup = menu_edit_artist->GetHandle();

		if (htsp_edit_artist->IsActive()) { htsp_edit_artist->Deactivate(); string = info_edit_artist->GetText(); }
		else				  htsp_edit_artist->Activate();
	}
	else if (menu_edit_title->IsVisible())
	{
		activePopup = menu_edit_title->GetHandle();

		if (htsp_edit_title->IsActive())  { htsp_edit_title->Deactivate(); string = info_edit_title->GetText(); }
		else				  htsp_edit_title->Activate();
	}
	else if (menu_edit_album->IsVisible())
	{
		activePopup = menu_edit_album->GetHandle();

		if (htsp_edit_album->IsActive())  { htsp_edit_album->Deactivate(); string = info_edit_album->GetText(); }
		else				  htsp_edit_album->Activate();
	}
	else if (menu_edit_genre->IsVisible())
	{
		activePopup = menu_edit_genre->GetHandle();

		if (htsp_edit_genre->IsActive())  htsp_edit_genre->Deactivate();
		else				  htsp_edit_genre->Activate();
	}
	else if (menu_edit_year->IsVisible())
	{
		activePopup = menu_edit_year->GetHandle();

		if (htsp_edit_year->IsActive())	  htsp_edit_year->Deactivate();
		else				  htsp_edit_year->Activate();
	}
	else
	{
		return;
	}

	if (string == NIL) string = "?";

	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Joblist");

	menu_case->RemoveAllEntries();
	menu_case_all->RemoveAllEntries();

	menu_case->AddEntry(AdjustCaseWordsFirstCapital(string).Append(" (").Append(i18n->TranslateString("all words upper case")).Append(")"), &clicked_case, 0)->onAction.Connect(&LayerJoblist::AdjustStringCase, this);
	menu_case->AddEntry(AdjustCaseLongWordsFirstCapital(string).Append(" (").Append(i18n->TranslateString("long words upper case")).Append(")"), &clicked_case, 1)->onAction.Connect(&LayerJoblist::AdjustStringCase, this);
	menu_case->AddEntry(AdjustCaseFirstCapital(string).Append(" (").Append(i18n->TranslateString("first letter upper case")).Append(")"), &clicked_case, 2)->onAction.Connect(&LayerJoblist::AdjustStringCase, this);
	menu_case->AddEntry(string.ToLower().Append(" (").Append(i18n->TranslateString("all lower case")).Append(")"), &clicked_case, 3)->onAction.Connect(&LayerJoblist::AdjustStringCase, this);
	menu_case->AddEntry(string.ToUpper().Append(" (").Append(i18n->TranslateString("all upper case")).Append(")"), &clicked_case, 4)->onAction.Connect(&LayerJoblist::AdjustStringCase, this);

	menu_case_all->AddEntry(AdjustCaseWordsFirstCapital(string).Append(" (").Append(i18n->TranslateString("all words upper case")).Append(")"), &clicked_case, 0)->onAction.Connect(&LayerJoblist::AdjustStringCaseAll, this);
	menu_case_all->AddEntry(AdjustCaseLongWordsFirstCapital(string).Append(" (").Append(i18n->TranslateString("long words upper case")).Append(")"), &clicked_case, 1)->onAction.Connect(&LayerJoblist::AdjustStringCaseAll, this);
	menu_case_all->AddEntry(AdjustCaseFirstCapital(string).Append(" (").Append(i18n->TranslateString("first letter upper case")).Append(")"), &clicked_case, 2)->onAction.Connect(&LayerJoblist::AdjustStringCaseAll, this);
	menu_case_all->AddEntry(string.ToLower().Append(" (").Append(i18n->TranslateString("all lower case")).Append(")"), &clicked_case, 3)->onAction.Connect(&LayerJoblist::AdjustStringCaseAll, this);
	menu_case_all->AddEntry(string.ToUpper().Append(" (").Append(i18n->TranslateString("all upper case")).Append(")"), &clicked_case, 4)->onAction.Connect(&LayerJoblist::AdjustStringCaseAll, this);
}

Bool freac::LayerJoblist::IsWordBreakingCharacter(Int character)
{
	if (character == 0   ||
	    character == ' ' ||
	    character == '(' || character == ')'  ||
	    character == '[' || character == ']'  ||
	    character == '<' || character == '>'  ||
	    character == '?' || character == 0xBF || // inverted question mark
	    character == '!' || character == 0xA1 || // inverted exclamation mark
	    character == '-' ||
	    character == '+' ||
	    character == '&' ||
	    character == '.' ||
	    character == ',' ||
	    character == ':' ||
	    character == ';' ||
	    character == '\"') return True;

	return False;
}

String freac::LayerJoblist::AdjustCaseFirstCapital(const String &string)
{
	String	 value = String(string).ToLower();
	String	 character;

	for (Int i = 0; i < value.Length(); i++)
	{
		if (IsWordBreakingCharacter(value[i]) || value[i] == '\'') continue;

		character[0] = value[i];
		value[i]     = character.ToTitle()[0];

		break;
	}

	return value;
}

String freac::LayerJoblist::AdjustCaseWordsFirstCapital(const String &string)
{
	String	 value = String(string).ToLower();
	String	 character;

	for (Int i = 0; i < value.Length(); i++)
	{
		character[0] = value[i];

		if ( IsWordBreakingCharacter(value[i - 1]) || (value[i - 1] == '\'' &&
		    (IsWordBreakingCharacter(value[i - 2]) ||  value[i - 2] == '\''))) value[i] = character.ToTitle()[0];
	}

	return value;
}

String freac::LayerJoblist::AdjustCaseLongWordsFirstCapital(const String &string)
{
	String	 value = AdjustCaseWordsFirstCapital(string);
	String	 character;

	for (Int i = 0; i < value.Length(); i++)
	{
		if (IsWordBreakingCharacter(value[i]) || value[i] == '\'') continue;

		for (Int j = i + 1; j < value.Length(); j++)
		{
			character[0] = value[j];

			if ((IsWordBreakingCharacter(value[j + 1]) ||
			     IsWordBreakingCharacter(value[j + 2]) ||
			     IsWordBreakingCharacter(value[j + 3])) &&
			    ( IsWordBreakingCharacter(value[j - 1]) || (value[j - 1] == '\'' &&
			     (IsWordBreakingCharacter(value[j - 2]) ||  value[j - 2] == '\'')))) value[j] = character.ToLower()[0];
		}

		break;
	}

	return value;
}

Void freac::LayerJoblist::UseStringForSelectedTracks()
{
	for (Int i = 0; i < joblist->GetNOfTracks(); i++)
	{
		ListEntry	*entry = joblist->GetNthEntry(i);

		if (entry->IsMarked())
		{
			Track	 track = joblist->GetNthTrack(i);
			Info	 info = track.GetInfo();

			if	(activePopup == menu_edit_artist->GetHandle()) info.artist = info_edit_artist->GetText();
			else if (activePopup == menu_edit_album->GetHandle())  info.album = info_edit_album->GetText();
			else if (activePopup == menu_edit_genre->GetHandle())  info.genre = info_edit_genre->GetText();
			else if (activePopup == menu_edit_year->GetHandle())   info.year = info_edit_year->GetText().ToInt();

			track.SetInfo(info);

			BoCA::JobList::Get()->onComponentModifyTrack.Emit(track);
		}
	}
}

Void freac::LayerJoblist::InterpretStringAs()
{
	String	 charset;

	switch (clicked_charset)
	{
		case CHARSET_UTF_8:	 charset = "UTF-8";	 break;
		case CHARSET_ISO_8859_1: charset = "ISO-8859-1"; break;
		case CHARSET_ISO_8859_2: charset = "ISO-8859-2"; break;
		case CHARSET_ISO_8859_5: charset = "ISO-8859-5"; break;
		case CHARSET_ISO_8859_7: charset = "ISO-8859-7"; break;
		case CHARSET_CP1251:	 charset = "CP1251";	 break;
		case CHARSET_SHIFT_JIS:	 charset = "SHIFT-JIS";	 break;
		case CHARSET_GBK:	 charset = "GBK";	 break;
		case CHARSET_BIG_5:	 charset = "BIG-5";	 break;

		case CHARSET_OTHER:
			{
				ChooseCharsetDialog	 dialog;

				if (dialog.ShowDialog() == Success()) charset = dialog.GetCharset();
			}

			break;
	}

	if (charset == NIL) return;

	Track		 track = joblist->GetSelectedTrack();
	Info		 info = track.GetInfo();
	const Info	&oInfo = track.GetOriginalInfo();

	if	(activePopup == menu_edit_artist->GetHandle()) info.artist.ImportFrom(charset, oInfo.artist.ConvertTo("ISO-8859-1"));
	else if (activePopup == menu_edit_title->GetHandle())  info.title.ImportFrom(charset, oInfo.title.ConvertTo("ISO-8859-1"));
	else if (activePopup == menu_edit_album->GetHandle())  info.album.ImportFrom(charset, oInfo.album.ConvertTo("ISO-8859-1"));
	else if (activePopup == menu_edit_genre->GetHandle())  info.genre.ImportFrom(charset, oInfo.genre.ConvertTo("ISO-8859-1"));

	track.SetInfo(info);

	BoCA::JobList::Get()->onComponentModifyTrack.Emit(track);

	clicked_charset = -1;
}

Void freac::LayerJoblist::InterpretStringAsAll()
{
	String	 charset;

	switch (clicked_charset)
	{
		case CHARSET_UTF_8:	 charset = "UTF-8";	 break;
		case CHARSET_ISO_8859_1: charset = "ISO-8859-1"; break;
		case CHARSET_ISO_8859_2: charset = "ISO-8859-2"; break;
		case CHARSET_ISO_8859_5: charset = "ISO-8859-5"; break;
		case CHARSET_ISO_8859_7: charset = "ISO-8859-7"; break;
		case CHARSET_CP1251:	 charset = "CP1251";	 break;
		case CHARSET_SHIFT_JIS:	 charset = "SHIFT-JIS";	 break;
		case CHARSET_GBK:	 charset = "GBK";	 break;
		case CHARSET_BIG_5:	 charset = "BIG-5";	 break;

		case CHARSET_OTHER:
			{
				ChooseCharsetDialog	 dialog;

				if (dialog.ShowDialog() == Success()) charset = dialog.GetCharset();
			}

			break;
	}

	if (charset == NIL) return;

	for (Int i = 0; i < joblist->GetNOfTracks(); i++)
	{
		ListEntry	*entry = joblist->GetNthEntry(i);

		if (entry->IsMarked())
		{
			Track		 track = joblist->GetNthTrack(i);
			Info		 info = track.GetInfo();
			const Info	&oInfo = track.GetOriginalInfo();

			if	(activePopup == menu_edit_artist->GetHandle()) info.artist.ImportFrom(charset, oInfo.artist.ConvertTo("ISO-8859-1"));
			else if (activePopup == menu_edit_title->GetHandle())  info.title.ImportFrom(charset, oInfo.title.ConvertTo("ISO-8859-1"));
			else if (activePopup == menu_edit_album->GetHandle())  info.album.ImportFrom(charset, oInfo.album.ConvertTo("ISO-8859-1"));
			else if (activePopup == menu_edit_genre->GetHandle())  info.genre.ImportFrom(charset, oInfo.genre.ConvertTo("ISO-8859-1"));

			track.SetInfo(info);

			BoCA::JobList::Get()->onComponentModifyTrack.Emit(track);
		}
	}

	clicked_charset = -1;
}

Void freac::LayerJoblist::AdjustStringCase()
{
	String	 string;

	if	(activePopup == menu_edit_artist->GetHandle()) string = info_edit_artist->GetText();
	else if (activePopup == menu_edit_title->GetHandle())  string = info_edit_title->GetText();
	else if (activePopup == menu_edit_album->GetHandle())  string = info_edit_album->GetText();

	switch (clicked_case)
	{
		case 0: string = AdjustCaseWordsFirstCapital(string);	  break;
		case 1: string = AdjustCaseLongWordsFirstCapital(string); break;
		case 2: string = AdjustCaseFirstCapital(string);	  break;
		case 3: string = string.ToLower();			  break;
		case 4:	string = string.ToUpper();			  break;
	}

	Track	 track = joblist->GetSelectedTrack();
	Info	 info = track.GetInfo();

	if	(activePopup == menu_edit_artist->GetHandle()) info.artist = string;
	else if (activePopup == menu_edit_title->GetHandle())  info.title  = string;
	else if (activePopup == menu_edit_album->GetHandle())  info.album  = string;

	track.SetInfo(info);

	BoCA::JobList::Get()->onComponentModifyTrack.Emit(track);

	clicked_case = -1;
}

Void freac::LayerJoblist::AdjustStringCaseAll()
{
	for (Int i = 0; i < joblist->GetNOfTracks(); i++)
	{
		ListEntry	*entry = joblist->GetNthEntry(i);

		if (entry->IsMarked())
		{
			String	 string;
			Track	 track = joblist->GetNthTrack(i);
			Info	 info = track.GetInfo();

			if	(activePopup == menu_edit_artist->GetHandle()) string = info.artist;
			else if (activePopup == menu_edit_title->GetHandle())  string = info.title;
			else if (activePopup == menu_edit_album->GetHandle())  string = info.album;

			switch (clicked_case)
			{
				case 0: string = AdjustCaseWordsFirstCapital(string);	  break;
				case 1: string = AdjustCaseLongWordsFirstCapital(string); break;
				case 2: string = AdjustCaseFirstCapital(string);	  break;
				case 3: string = string.ToLower();			  break;
				case 4:	string = string.ToUpper();			  break;
			}

			if	(activePopup == menu_edit_artist->GetHandle()) info.artist = string;
			else if (activePopup == menu_edit_title->GetHandle())  info.title  = string;
			else if (activePopup == menu_edit_album->GetHandle())  info.album  = string;

			track.SetInfo(info);

			BoCA::JobList::Get()->onComponentModifyTrack.Emit(track);
		}
	}

	clicked_case = -1;
}
