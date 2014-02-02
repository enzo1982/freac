 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <gui/player.h>

#include <joblist.h>
#include <playback.h>
#include <config.h>

using namespace BoCA;
using namespace BoCA::AS;

BonkEnc::LayerPlayer::LayerPlayer(JobList *iJoblist)
{
	joblist = iJoblist;

	Config		*config = Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	Playback	*player	= Playback::Get();

	Point		 pos;
	Size		 size;

	pos.x = -1 + (i18n->IsActiveLanguageRightToLeft() ? 215 : 0);
	pos.y = -1;
	size.cx = 25;
	size.cy = 25;

	button_play	= new Button(NIL, ImageLoader::Load(String(config->resourcesPath).Append("freac.pci:12")), pos, size);
	button_play->onAction.Connect(&LayerPlayer::PlaySelectedItem, this);
	button_play->SetFlags(BF_NOFRAME);

	pos.x += 22 - (i18n->IsActiveLanguageRightToLeft() ? 44 : 0);

	button_pause	= new Button(NIL, ImageLoader::Load(String(config->resourcesPath).Append("freac.pci:13")), pos, size);
	button_pause->onAction.Connect(&LayerPlayer::PauseResumePlayback, this);
	button_pause->SetFlags(BF_NOFRAME);

	pos.x += 22 - (i18n->IsActiveLanguageRightToLeft() ? 44 : 0);

	button_stop	= new Button(NIL, ImageLoader::Load(String(config->resourcesPath).Append("freac.pci:14")), pos, size);
	button_stop->onAction.Connect(&LayerPlayer::StopPlayback, this);
	button_stop->SetFlags(BF_NOFRAME);

	pos.x += 127 - (i18n->IsActiveLanguageRightToLeft() ? 254 : 0);

	button_prev	= new Button(NIL, ImageLoader::Load(String(config->resourcesPath).Append("freac.pci:15")), pos, size);
	button_prev->onAction.Connect(&LayerPlayer::PlayPreviousItem, this);
	button_prev->SetFlags(BF_NOFRAME);

	pos.x += 22 - (i18n->IsActiveLanguageRightToLeft() ? 44 : 0);

	button_next	= new Button(NIL, ImageLoader::Load(String(config->resourcesPath).Append("freac.pci:16")), pos, size);
	button_next->onAction.Connect(&LayerPlayer::PlayNextItem, this);
	button_next->SetFlags(BF_NOFRAME);

	pos.x += 22 - (i18n->IsActiveLanguageRightToLeft() ? 44 : 0);

	button_open	= new Button(NIL, ImageLoader::Load(String(config->resourcesPath).Append("freac.pci:17")), pos, size);
	button_open->onAction.Connect(&LayerPlayer::OpenCDTray, this);
	button_open->SetFlags(BF_NOFRAME);

	pos.x = 69;
	pos.y += 4;
	size.cx = 100;

	slider_play	= new Slider(pos, size, OR_HORZ, NIL, 0, 1000);
	slider_play->onMouseDragEnd.Connect(&LayerPlayer::OnChangePlayPosition, this);
	slider_play->SetValue(i18n->IsActiveLanguageRightToLeft() ? 1000 : 0);

	player->onProgress.Connect(&Slider::SetValue, slider_play);

	Add(button_play);
	Add(button_pause);
	Add(button_stop);
	Add(button_prev);
	Add(button_next);
	Add(button_open);

	Add(slider_play);

	Playback::Get()->onFinish.Connect(&LayerPlayer::StopPlayback, this);

	BoCA::JobList::Get()->onApplicationRemoveTrack.Connect(&LayerPlayer::OnJoblistRemoveTrack, this);
	BoCA::JobList::Get()->onApplicationRemoveAllTracks.Connect(&LayerPlayer::OnJoblistRemoveAllTracks, this);

	BoCA::Settings::Get()->onChangeLanguageSettings.Connect(&LayerPlayer::OnChangeLanguageSettings, this);

	SetSize(Size(243, 21));
}

BonkEnc::LayerPlayer::~LayerPlayer()
{
	Playback::Get()->onFinish.Disconnect(&LayerPlayer::StopPlayback, this);

	BoCA::JobList::Get()->onApplicationRemoveTrack.Disconnect(&LayerPlayer::OnJoblistRemoveTrack, this);
	BoCA::JobList::Get()->onApplicationRemoveAllTracks.Disconnect(&LayerPlayer::OnJoblistRemoveAllTracks, this);

	BoCA::Settings::Get()->onChangeLanguageSettings.Disconnect(&LayerPlayer::OnChangeLanguageSettings, this);

	DeleteObject(button_play);
	DeleteObject(button_pause);
	DeleteObject(button_stop);
	DeleteObject(button_prev);
	DeleteObject(button_next);
	DeleteObject(button_open);

	Playback::Get()->onProgress.Disconnect(&Slider::SetValue, slider_play);

	DeleteObject(slider_play);
}

Void BonkEnc::LayerPlayer::OnChangeLanguageSettings()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	static Bool	 prevRTL = i18n->IsActiveLanguageRightToLeft();

	/* Rearrange playback buttons if language direction changed.
	 */
	if (i18n->IsActiveLanguageRightToLeft() != prevRTL)
	{
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
}

Void BonkEnc::LayerPlayer::OnJoblistRemoveTrack(const Track &track)
{
	Playback	*player = Playback::Get();

	if (player->IsPlaying() && playingTrack.GetTrackID() == track.GetTrackID()) StopPlayback();
}

Void BonkEnc::LayerPlayer::OnJoblistRemoveAllTracks()
{
	if (Playback::Get()->IsPlaying()) StopPlayback();
}

Void BonkEnc::LayerPlayer::PlaySelectedItem()
{
	const Track	&track = joblist->GetSelectedTrack();

	if (track != NIL)
	{
		StopPlayback();

		Play(track);
	}
}

Void BonkEnc::LayerPlayer::PlayPreviousItem()
{
	const Array<Track>	*tracks = joblist->GetTrackList();

	for (Int i = 1; i < tracks->Length(); i++)
	{
		const Track	&track = tracks->GetNth(i);

		if (track.GetTrackID() == playingTrack.GetTrackID())
		{
			StopPlayback();

			Play(tracks->GetNth(i - 1));

			break;
		}
	}
}

Void BonkEnc::LayerPlayer::PlayNextItem()
{
	const Array<Track>	*tracks = joblist->GetTrackList();

	for (Int i = 0; i < tracks->Length() - 1; i++)
	{
		const Track	&track = tracks->GetNth(i);

		if (track.GetTrackID() == playingTrack.GetTrackID())
		{
			StopPlayback();

			Play(tracks->GetNth(i + 1));

			break;
		}
	}
}

Void BonkEnc::LayerPlayer::Play(const Track &track)
{
	playingTrack = track;

	/* Set joblist entry color.
	 */
	ListEntry	*entry = joblist->GetEntryByTrack(playingTrack);

	if (entry != NIL)
	{
		GUI::Font	 font = entry->GetFont();

		font.SetColor(Color(255, 0, 0));

		entry->SetFont(font);
	}

	/* Play track.
	 */
	Playback	*player = Playback::Get();

	player->Play(playingTrack);
}

Void BonkEnc::LayerPlayer::PauseResumePlayback()
{
	Playback	*player = Playback::Get();

	if (!player->IsPlaying()) return;

	if (player->IsPaused()) player->Resume();
	else			player->Pause();
}

Void BonkEnc::LayerPlayer::StopPlayback()
{
	Playback	*player = Playback::Get();

	if (!player->IsPlaying()) return;

	player->Stop();

	/* Reset slider.
	 */
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	slider_play->SetValue(i18n->IsActiveLanguageRightToLeft() ? 1000 : 0);

	/* Reset joblist entry color.
	 */
	ListEntry	*entry = joblist->GetEntryByTrack(playingTrack);

	if (entry != NIL)
	{
		GUI::Font	 font = entry->GetFont();

		font.SetColor(Setup::ClientTextColor);

		entry->SetFont(font);
	}
}

Void BonkEnc::LayerPlayer::OpenCDTray()
{
	Registry		&boca = Registry::Get();
	DeviceInfoComponent	*info = (DeviceInfoComponent *) boca.CreateComponentByID("cdrip-info");

	if (info != NIL)
	{
		info->OpenNthDeviceTray(BoCA::Config::Get()->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault));

		boca.DeleteComponent(info);
	}
}

Void BonkEnc::LayerPlayer::OnChangePlayPosition()
{
	Playback	*player = Playback::Get();

	if (!player->IsPlaying()) return;

	player->onFinish.Disconnect(&LayerPlayer::StopPlayback, this);

	player->Stop();
	player->Play(playingTrack);

	player->onFinish.Connect(&LayerPlayer::StopPlayback, this);

	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	if (i18n->IsActiveLanguageRightToLeft()) player->SetPosition(1000 - slider_play->GetValue());
	else					 player->SetPosition(slider_play->GetValue());
}
