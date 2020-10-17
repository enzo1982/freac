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

#include <gui/playback.h>

#include <joblist.h>
#include <player.h>
#include <config.h>

#include <jobs/engine/convert.h>

using namespace BoCA;
using namespace BoCA::AS;

freac::PlaybackWidget::PlaybackWidget(Player *iPlayer, JobList *iJoblist)
{
	player	= iPlayer;
	joblist = iJoblist;

	Config		*config = Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	Point		 pos;
	Size		 size;

	pos.x = -1 + (i18n->IsActiveLanguageRightToLeft() ? 215 : 0);
	pos.y = -1;
	size.cx = 25;
	size.cy = 25;

	button_play	= new Button(ImageLoader::Load(String(config->resourcesPath).Append("icons/player/player-play.png")), pos, size);
	button_play->onAction.Connect(&PlaybackWidget::PlaySelectedItem, this);
	button_play->SetFlags(BF_NOFRAME);

	pos.x += 22 - (i18n->IsActiveLanguageRightToLeft() ? 44 : 0);

	button_pause	= new Button(ImageLoader::Load(String(config->resourcesPath).Append("icons/player/player-pause.png")), pos, size);
	button_pause->onAction.Connect(&PlaybackWidget::PauseResumePlayback, this);
	button_pause->SetFlags(BF_NOFRAME);

	pos.x += 22 - (i18n->IsActiveLanguageRightToLeft() ? 44 : 0);

	button_stop	= new Button(ImageLoader::Load(String(config->resourcesPath).Append("icons/player/player-stop.png")), pos, size);
	button_stop->onAction.Connect(&PlaybackWidget::StopPlayback, this);
	button_stop->SetFlags(BF_NOFRAME);

	pos.x += 127 - (i18n->IsActiveLanguageRightToLeft() ? 254 : 0);

	button_prev	= new Button(ImageLoader::Load(String(config->resourcesPath).Append("icons/player/player-previous.png")), pos, size);
	button_prev->onAction.Connect(&PlaybackWidget::PlayPreviousItem, this);
	button_prev->SetFlags(BF_NOFRAME);

	pos.x += 22 - (i18n->IsActiveLanguageRightToLeft() ? 44 : 0);

	button_next	= new Button(ImageLoader::Load(String(config->resourcesPath).Append("icons/player/player-next.png")), pos, size);
	button_next->onAction.Connect(&PlaybackWidget::PlayNextItem, this);
	button_next->SetFlags(BF_NOFRAME);

	pos.x += 22 - (i18n->IsActiveLanguageRightToLeft() ? 44 : 0);

	button_open	= new Button(ImageLoader::Load(String(config->resourcesPath).Append("icons/player/player-eject.png")), pos, size);
	button_open->onAction.Connect(&PlaybackWidget::OpenCDTray, this);
	button_open->SetFlags(BF_NOFRAME);

	pos.x = 69;
	pos.y += 4;
	size.cx = 100;

	slider_play	= new Slider(pos, size, OR_HORZ, NIL, 0, 1000);
	slider_play->onMouseDragEnd.Connect(&PlaybackWidget::OnChangePlayPosition, this);
	slider_play->SetValue(i18n->IsActiveLanguageRightToLeft() ? 1000 : 0);
	slider_play->Deactivate();

	player->onProgress.Connect(&Slider::SetValue, slider_play);

	Add(button_play);
	Add(button_pause);
	Add(button_stop);
	Add(button_prev);
	Add(button_next);
	Add(button_open);

	Add(slider_play);

	player->onPlay.Connect(&PlaybackWidget::OnPlay, this);
	player->onFinish.Connect(&PlaybackWidget::OnFinish, this);

	BoCA::JobList::Get()->onApplicationRemoveTrack.Connect(&PlaybackWidget::OnJoblistRemoveTrack, this);
	BoCA::JobList::Get()->onApplicationRemoveAllTracks.Connect(&PlaybackWidget::OnJoblistRemoveAllTracks, this);

	BoCA::Settings::Get()->onChangeLanguageSettings.Connect(&PlaybackWidget::OnChangeLanguageSettings, this);

	SetSize(Size(243, 21));
}

freac::PlaybackWidget::~PlaybackWidget()
{
	player->onPlay.Disconnect(&PlaybackWidget::OnPlay, this);
	player->onFinish.Disconnect(&PlaybackWidget::OnFinish, this);

	BoCA::JobList::Get()->onApplicationRemoveTrack.Disconnect(&PlaybackWidget::OnJoblistRemoveTrack, this);
	BoCA::JobList::Get()->onApplicationRemoveAllTracks.Disconnect(&PlaybackWidget::OnJoblistRemoveAllTracks, this);

	BoCA::Settings::Get()->onChangeLanguageSettings.Disconnect(&PlaybackWidget::OnChangeLanguageSettings, this);

	DeleteObject(button_play);
	DeleteObject(button_pause);
	DeleteObject(button_stop);
	DeleteObject(button_prev);
	DeleteObject(button_next);
	DeleteObject(button_open);

	player->onProgress.Disconnect(&Slider::SetValue, slider_play);

	DeleteObject(slider_play);
}

Void freac::PlaybackWidget::OnChangeLanguageSettings()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	static Bool	 prevRTL = i18n->IsActiveLanguageRightToLeft();

	if (i18n->IsActiveLanguageRightToLeft() == prevRTL) return;

	/* Rearrange playback buttons if language direction changed.
	 */
	if (Registry::Get().GetNumberOfComponentsOfType(COMPONENT_TYPE_OUTPUT) > 0)
	{
		button_play->Hide();
		button_pause->Hide();
		button_stop->Hide();
		button_prev->Hide();
		button_next->Hide();
		button_open->Hide();

		Point	 pos;

		pos.x = -1 + (i18n->IsActiveLanguageRightToLeft() ? 215 : 0);
		pos.y = -1;

		button_play->SetPosition(pos);

		pos.x += 22 - (i18n->IsActiveLanguageRightToLeft() ? 44 : 0);

		button_pause->SetPosition(pos);

		pos.x += 22 - (i18n->IsActiveLanguageRightToLeft() ? 44 : 0);

		button_stop->SetPosition(pos);

		pos.x += 127 - (i18n->IsActiveLanguageRightToLeft() ? 254 : 0);

		button_prev->SetPosition(pos);

		pos.x += 22 - (i18n->IsActiveLanguageRightToLeft() ? 44 : 0);

		button_next->SetPosition(pos);

		pos.x += 22 - (i18n->IsActiveLanguageRightToLeft() ? 44 : 0);

		button_open->SetPosition(pos);

		button_play->Show();
		button_pause->Show();
		button_stop->Show();
		button_prev->Show();
		button_next->Show();
		button_open->Show();

		slider_play->SetValue(1000 - slider_play->GetValue());
	}

	prevRTL = i18n->IsActiveLanguageRightToLeft();
}

Void freac::PlaybackWidget::OnJoblistRemoveTrack(const Track &track)
{
	if (player->IsPlaying() && playingTrack.GetTrackID() == track.GetTrackID()) StopPlayback();
}

Void freac::PlaybackWidget::OnJoblistRemoveAllTracks()
{
	if (player->IsPlaying()) StopPlayback();
}

Void freac::PlaybackWidget::PlaySelectedItem()
{
	const Track	&track = joblist->GetSelectedTrack();

	if (track != NIL) Play(track);
}

Void freac::PlaybackWidget::PlayPreviousItem()
{
	for (Int i = 1; i < joblist->Length(); i++)
	{
		const Track	&track = joblist->GetNthTrack(i);

		if (track.GetTrackID() != playingTrack.GetTrackID()) continue;

		StopPlayback();

		Play(joblist->GetNthTrack(i - 1));

		break;
	}
}

Void freac::PlaybackWidget::PlayNextItem()
{
	for (Int i = 0; i < joblist->Length() - 1; i++)
	{
		const Track	&track = joblist->GetNthTrack(i);

		if (track.GetTrackID() != playingTrack.GetTrackID()) continue;

		StopPlayback();

		Play(joblist->GetNthTrack(i + 1));

		break;
	}
}

Void freac::PlaybackWidget::Play(const Track &track)
{
	/* Play track.
	 */
	player->Play(track);

	if (!player->IsPlaying()) return;

	playingTrack = track;
}

Void freac::PlaybackWidget::PauseResumePlayback()
{
	if (!player->IsPlaying()) return;

	if (player->IsPaused()) player->Resume();
	else			player->Pause();
}

Void freac::PlaybackWidget::StopPlayback()
{
	if (!player->IsPlaying()) return;

	player->Stop();

	playingTrack = NIL;
}

Void freac::PlaybackWidget::OnPlay(const Track &track)
{
	/* Activate slider.
	 */
	slider_play->Activate();

	/* Set joblist entry color.
	 */
	ListEntry	*entry = joblist->GetEntryByTrack(track);

	if (entry == NIL) return;

	GUI::Font	 font = entry->GetFont();

	font.SetColor(Color(255, 0, 0));

	entry->SetFont(font);
}

Void freac::PlaybackWidget::OnFinish(const Track &track)
{
	/* Reset slider.
	 */
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	slider_play->SetValue(i18n->IsActiveLanguageRightToLeft() ? 1000 : 0);
	slider_play->Deactivate();

	/* Reset joblist entry color.
	 */
	ListEntry	*entry = joblist->GetEntryByTrack(track);

	if (entry == NIL) return;

	GUI::Font	 font = entry->GetFont();

	font.SetColor(Setup::ClientTextColor);

	entry->SetFont(font);
}

Void freac::PlaybackWidget::OpenCDTray()
{
	Registry		&boca = Registry::Get();
	DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

	if (info == NIL) return;

	BoCA::Config	*config = BoCA::Config::Get();
	Int		 drive	= config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault);

	JobConvert::StopRipping(String("cdda:").Append(String::FromInt(drive)));

	if (info->IsNthDeviceTrayOpen(drive)) info->CloseNthDeviceTray(drive);
	else				      info->OpenNthDeviceTray(drive);

	boca.DeleteComponent(info);
}

Void freac::PlaybackWidget::OnChangePlayPosition()
{
	if (!player->IsPlaying()) return;

	player->onFinish.Disconnect(&PlaybackWidget::OnFinish, this);

	player->Stop();
	player->Play(playingTrack);

	player->onFinish.Connect(&PlaybackWidget::OnFinish, this);

	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	if (i18n->IsActiveLanguageRightToLeft()) player->SetPosition(1000 - slider_play->GetValue());
	else					 player->SetPosition(slider_play->GetValue());
}
