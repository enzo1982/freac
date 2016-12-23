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

#include "layer.h"
#include "video.h"
#include "videosite.h"
#include "videolist.h"
#include "videolistentry.h"

using namespace smooth::GUI::Dialogs;
using namespace smooth::Net;

BoCA::LayerYouTube::LayerYouTube() : Layer("Video")
{
	Config	*config = Config::Get();

	missingDecoders = False;

	tracks.EnableLocking();

	/* Load video sites.
	 */
	LoadVideoSites();

	text_url		= new Text(NIL, Point(7, 10));

	Add(text_url);

	edit_url		= new EditBox(NIL, Point(15, 7), Size(100, 0));
	edit_url->onInput.Connect(&LayerYouTube::OnEditDownloadURL, this);

	button_add_url		= new Button(NIL, NIL, Point(88, 6), Size());
	button_add_url->SetOrientation(OR_UPPERRIGHT);
	button_add_url->Deactivate();
	button_add_url->onAction.Connect(&LayerYouTube::OnDownloadTrack, this);

	Add(edit_url);
	Add(button_add_url);

	check_auto_download	= new CheckBox(NIL, Point(8, 34), Size(300, 0), (Bool *) &config->GetPersistentIntValue("YouTube", "AutoDownload", False));
	check_auto_download->SetOrientation(OR_UPPERRIGHT);

	check_keep_files	= new CheckBox(NIL, Point(8, 34), Size(300, 0), (Bool *) &config->GetPersistentIntValue("YouTube", "SaveVideoFiles", False));
	check_keep_files->SetOrientation(OR_UPPERRIGHT);

	if (missingDecoders) check_keep_files->Deactivate();

	Add(check_auto_download);
	Add(check_keep_files);

	text_downloads		= new Text(NIL, Point(7, 57));

	list_downloads		= new VideoList(Point(7, 76), Size(100, 75));

	Add(text_downloads);
	Add(list_downloads);

	text_tracks		= new Text(NIL, Point(7, 157));

	list_tracks		= new ListBox(Point(7, 176), Size(100, 150));
	list_tracks->onSelectEntry.Connect(&LayerYouTube::OnSelectTrack, this);

	list_tracks->EnableLocking();

	Add(text_tracks);
	Add(list_tracks);

	area_cover		= new ActiveArea(Setup::BackgroundColor, Point(7, 112), Size(114, 73));
	area_cover->SetOrientation(OR_LOWERLEFT);

	image_cover		= new Image(NIL, Point(8, 111), Size(112, 71));
	image_cover->SetOrientation(OR_LOWERLEFT);

	Add(area_cover);
	Add(image_cover);

	text_source		= new Text(NIL, Point(129, 109));
	text_source->SetOrientation(OR_LOWERLEFT);

	link_source		= new Hyperlink(NIL, NIL, NIL, Point(129, 109));
	link_source->SetOrientation(OR_LOWERLEFT);

	text_title		= new Text(NIL, Point(129, 82));
	text_title->SetOrientation(OR_LOWERLEFT);

	edit_title		= new EditBox(NIL, Point(129, 85), Size(0, 0));
	edit_title->SetOrientation(OR_LOWERLEFT);
	edit_title->onInput.Connect(&LayerYouTube::OnEditMetadata, this);

	text_description	= new Text(NIL, Point(129, 55));
	text_description->SetOrientation(OR_LOWERLEFT);

	edit_description	= new MultiEdit(NIL, Point(129, 58), Size(0, 50));
	edit_description->SetOrientation(OR_LOWERLEFT);
	edit_description->onInput.Connect(&LayerYouTube::OnEditMetadata, this);

	Add(text_source);
	Add(link_source);

	Add(text_title);
	Add(edit_title);

	Add(text_description);
	Add(edit_description);

	text_site		= new Text(NIL, Point(200, 109));
	text_site->SetOrientation(OR_LOWERRIGHT);

	text_site_value		= new Text(NIL, Point(200, 109));
	text_site_value->SetOrientation(OR_LOWERRIGHT);

	text_uploader		= new Text(NIL, Point(200, 82));
	text_uploader->SetOrientation(OR_LOWERRIGHT);

	text_uploader_value	= new Text(NIL, Point(200, 82));
	text_uploader_value->SetOrientation(OR_LOWERRIGHT);

	text_date		= new Text(NIL, Point(200, 55));
	text_date->SetOrientation(OR_LOWERRIGHT);

	text_date_value		= new Text(NIL, Point(200, 55));
	text_date_value->SetOrientation(OR_LOWERRIGHT);

	Add(text_site);
	Add(text_site_value);

	Add(text_uploader);
	Add(text_uploader_value);

	Add(text_date);
	Add(text_date_value);

	OnSelectNone();

	/* Connect slots.
	 */
	onShow.Connect(&LayerYouTube::OnShowLayer, this);
	onChangeSize.Connect(&LayerYouTube::OnChangeSize, this);

	Settings::Get()->onChangeLanguageSettings.Connect(&LayerYouTube::OnChangeLanguageSettings, this);

	JobList::Get()->onApplicationModifyTrack.Connect(&LayerYouTube::OnApplicationModifyTrack, this);
	JobList::Get()->onApplicationRemoveTrack.Connect(&LayerYouTube::OnApplicationRemoveTrack, this);
	JobList::Get()->onApplicationSelectTrack.Connect(&LayerYouTube::OnApplicationSelectTrack, this);

	JobList::Get()->onApplicationRemoveAllTracks.Connect(&LayerYouTube::OnApplicationRemoveAllTracks, this);

	Application::Get()->onQuit.Connect(&LayerYouTube::OnQuit, this);

	/* Create and start clipboard checking timer.
	 */
	timer_check_clipboard	= new S::System::Timer();
	timer_check_clipboard->onInterval.Connect(&LayerYouTube::OnTimerCheckClipboard, this);

	timer_check_clipboard->Start(500);
}

BoCA::LayerYouTube::~LayerYouTube()
{
	/* Stop clipboard checking timer.
	 */
	timer_check_clipboard->Stop();

	DeleteObject(timer_check_clipboard);

	/* Remove all tracks from track list.
	 */
	OnApplicationRemoveAllTracks();

	/* Disconnect slots.
	 */
	Settings::Get()->onChangeLanguageSettings.Disconnect(&LayerYouTube::OnChangeLanguageSettings, this);

	JobList::Get()->onApplicationModifyTrack.Disconnect(&LayerYouTube::OnApplicationModifyTrack, this);
	JobList::Get()->onApplicationRemoveTrack.Disconnect(&LayerYouTube::OnApplicationRemoveTrack, this);
	JobList::Get()->onApplicationSelectTrack.Disconnect(&LayerYouTube::OnApplicationSelectTrack, this);

	JobList::Get()->onApplicationRemoveAllTracks.Disconnect(&LayerYouTube::OnApplicationRemoveAllTracks, this);

	Application::Get()->onQuit.Disconnect(&LayerYouTube::OnQuit, this);

	/* Delete widgets.
	 */
	DeleteObject(text_url);

	DeleteObject(edit_url);
	DeleteObject(button_add_url);

	DeleteObject(check_auto_download);

	DeleteObject(text_downloads);
	DeleteObject(list_downloads);

	DeleteObject(check_keep_files);

	DeleteObject(text_tracks);
	DeleteObject(list_tracks);

	DeleteObject(area_cover);
	DeleteObject(image_cover);

	DeleteObject(text_site);
	DeleteObject(text_site_value);

	DeleteObject(text_source);
	DeleteObject(link_source);

	DeleteObject(text_title);
	DeleteObject(edit_title);

	DeleteObject(text_description);
	DeleteObject(edit_description);

	DeleteObject(text_uploader);
	DeleteObject(text_uploader_value);

	DeleteObject(text_date);
	DeleteObject(text_date_value);

	/* Free video sites.
	 */
	FreeVideoSites();
}

Void BoCA::LayerYouTube::LoadVideoSites()
{
	Directory		 dir(Utilities::GetBoCADirectory().Append("../freac/freac.extension.youtube"));
	const Array<File>	&files	= dir.GetFilesByPattern("videosite_*.xml");

	Config			*config = Config::Get();
	AS::Registry		&boca	= AS::Registry::Get();

	foreach (const File &file, files)
	{
		VideoSite	*site = new VideoSite(file);

		if (site->IsSane())
		{
			sites.Add(site);

			const Array<String>	&decoders = site->GetDecoders();

			foreach (const String &decoder, decoders)
			{
				if (!boca.ComponentExists(decoder)) missingDecoders = True;
			}
		}
		else
		{
			delete site;
		}
	}

	if (missingDecoders) config->SetIntValue("YouTube", "SaveVideoFiles", True);

	config->SetIntValue("YouTube", "DisableSaveOption", missingDecoders);
}

Void BoCA::LayerYouTube::FreeVideoSites()
{
	foreach (VideoSite *site, sites) delete site;

	sites.RemoveAll();
}

/* Get VideoSite object that can handle the URL.
 * ----
 */
VideoSite *BoCA::LayerYouTube::GetVideoSiteForURL(const String &URL)
{
	if (!(URL.StartsWith("http://") || URL.StartsWith("https://")) || URL.Length() < 11) return NIL;

	VideoSite	*videoSite = NIL;

	foreach (VideoSite *site, sites)
	{
		if (site->CanHandleURL(URL)) { videoSite = site; break; }
	}

	return videoSite;
}

/* Get text from clipboard.
 * ----
 */
String BoCA::LayerYouTube::GetClipboardText()
{
	if (container->GetContainerWindow() == NIL) return NIL;

	String	 clipboardText = Clipboard(container->GetContainerWindow()).GetClipboardText();

	/* Save clipboard text to compare later.
	 */
	previousClipboardText = clipboardText;

	return clipboardText;
}

/* Called when component is displayed.
 * ----
 */
Void BoCA::LayerYouTube::OnShowLayer()
{
	static Bool	 initialized = False;

	if (initialized)		 return;
	if (GetContainerWindow() == NIL) return;

	Config	*config = Config::Get();

	if (missingDecoders && !config->GetIntValue("YouTube", "DoNotShowDecoderWarning", False))
	{
		BoCA::I18n	*i18n = BoCA::I18n::Get();

		i18n->SetContext("Extensions::Video Downloader::Errors");

		Bool		 doNotShowAgain = False;
		MessageDlg	*messageBox = new MessageDlg(i18n->TranslateString("Some required video decoders could not be found. Video files\ncannot be added to the joblist for conversion to audio files.\n\nPlease install avconv or FFmpeg to fix this problem!"), i18n->TranslateString("Note"), Message::Buttons::Ok, Message::Icon::Warning, i18n->TranslateString("Do not display this note again"), &doNotShowAgain);

		messageBox->ShowDialog();

		DeleteObject(messageBox);

		config->SetIntValue("YouTube", "DoNotShowDecoderWarning", doNotShowAgain);
	}

	initialized = True;
}

/* Called when component canvas size changes.
 * ----
 */
Void BoCA::LayerYouTube::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	edit_url->SetWidth(clientSize.cx - text_url->GetUnscaledTextWidth() - button_add_url->GetWidth() - 30);

	list_downloads->SetWidth(clientSize.cx - 15);

	list_tracks->SetSize(Size(clientSize.cx - 15, clientSize.cy - 296));

	/* Shorten link text.
	 */
	Font	 font;
	String	 text = link_source->GetURL();

	if (font.GetUnscaledTextSizeX(text) >= clientSize.cx - Math::Max(text_source->GetUnscaledTextWidth(), Math::Max(text_title->GetUnscaledTextWidth(), text_description->GetUnscaledTextWidth())) - 344)
	{
		for (Int i = text.Length() - 1; i >= 13; i--)
		{
			if (font.GetUnscaledTextSizeX(text.Head(i).Append("...")) >= clientSize.cx - Math::Max(text_source->GetUnscaledTextWidth(), Math::Max(text_title->GetUnscaledTextWidth(), text_description->GetUnscaledTextWidth())) - 344) continue;

			link_source->SetText(text.Head(i).Append("..."));

			break;
		}
	}
	else
	{
		link_source->SetText(text);
	}

	/* Video title edit box.
	 */
	edit_title->SetWidth(clientSize.cx - Math::Max(text_source->GetUnscaledTextWidth(), Math::Max(text_title->GetUnscaledTextWidth(), text_description->GetUnscaledTextWidth())) - 344);
	edit_description->SetWidth(clientSize.cx - Math::Max(text_source->GetUnscaledTextWidth(), Math::Max(text_title->GetUnscaledTextWidth(), text_description->GetUnscaledTextWidth())) - 344);
}

/* Called when application language is changed.
 * ----
 */
Void BoCA::LayerYouTube::OnChangeLanguageSettings()
{
	I18n	*i18n	= I18n::Get();

	i18n->SetContext("Extensions::Video Downloader");

	SetText(i18n->TranslateString("Video"));

	/* Hide all affected widgets prior to changing
	 * labels to avoid flickering.
	 */
	Bool	 prevVisible = IsVisible();

	if (prevVisible) Hide();

	/* Set texts and positions.
	 */
	text_url->SetText(i18n->AddColon(i18n->TranslateString("Enter video URL here")));

	edit_url->SetX(text_url->GetUnscaledTextWidth() + 15);

	button_add_url->SetText(i18n->TranslateString("Download"));
	button_add_url->SetWidth(Math::Max(80, button_add_url->GetUnscaledTextWidth() + 13));
	button_add_url->SetX(button_add_url->GetWidth() + 8);

	check_auto_download->SetText(i18n->TranslateString("Automatically download URLs copied to clipboard"));
	check_auto_download->SetWidth(check_auto_download->GetUnscaledTextWidth() + 20);

	check_keep_files->SetText(i18n->TranslateString("Save downloaded video files"));
	check_keep_files->SetWidth(check_keep_files->GetUnscaledTextWidth() + 20);

	check_auto_download->SetX(check_auto_download->GetWidth() + check_keep_files->GetWidth() + 16);
	check_keep_files->SetX(check_keep_files->GetWidth() + 8);

	text_downloads->SetText(i18n->AddColon(i18n->TranslateString("Video downloads")));
	text_tracks->SetText(i18n->AddColon(i18n->TranslateString("Downloaded tracks")));

	list_tracks->RemoveAllTabs();

	list_tracks->AddTab(i18n->TranslateString("Uploader"), 150);
	list_tracks->AddTab(i18n->TranslateString("Title"));
	list_tracks->AddTab(i18n->TranslateString("Length"), 80, OR_RIGHT);
	list_tracks->AddTab(i18n->TranslateString("Size"), 80, OR_RIGHT);

	text_source->SetText(i18n->AddColon(i18n->TranslateString("Video URL")));
	text_title->SetText(i18n->AddColon(i18n->TranslateString("Video title")));
	text_description->SetText(i18n->AddColon(i18n->TranslateString("Video description")));

	link_source->SetX(129 + Math::Max(text_source->GetUnscaledTextWidth(), Math::Max(text_title->GetUnscaledTextWidth(), text_description->GetUnscaledTextWidth())) + 8);
	edit_title->SetX(129 + Math::Max(text_source->GetUnscaledTextWidth(), Math::Max(text_title->GetUnscaledTextWidth(), text_description->GetUnscaledTextWidth())) + 8);
	edit_description->SetX(129 + Math::Max(text_source->GetUnscaledTextWidth(), Math::Max(text_title->GetUnscaledTextWidth(), text_description->GetUnscaledTextWidth())) + 8);

	text_site->SetText(i18n->AddColon(i18n->TranslateString("Site name")));
	text_uploader->SetText(i18n->AddColon(i18n->TranslateString("Uploaded by")));
	text_date->SetText(i18n->AddColon(i18n->TranslateString("Uploaded on")));

	text_site_value->SetX(200 - Math::Max(text_site->GetUnscaledTextWidth(), Math::Max(text_uploader->GetUnscaledTextWidth(), text_date->GetUnscaledTextWidth())) - 8);
	text_uploader_value->SetX(200 - Math::Max(text_site->GetUnscaledTextWidth(), Math::Max(text_uploader->GetUnscaledTextWidth(), text_date->GetUnscaledTextWidth())) - 8);
	text_date_value->SetX(200 - Math::Max(text_site->GetUnscaledTextWidth(), Math::Max(text_uploader->GetUnscaledTextWidth(), text_date->GetUnscaledTextWidth())) - 8);

	/* OnChangeSize will correct sizes of any other widgets.
	 */
	OnChangeSize(GetSize());

	/* Show all widgets again.
	 */
	if (prevVisible) Show();
}

/* Called when clipboard contents need to be checked for an update.
 * ----
 * Initiates download of URL in clipboard.
 */
Void BoCA::LayerYouTube::OnTimerCheckClipboard()
{
	if (container->GetContainerWindow() == NIL) return;

	/* Initialize clipboard text when running the first time.
	 */
	static Bool	 initialized = False;

	if (!initialized) { GetClipboardText(); initialized = True; }

	Config	*config = Config::Get();

	/* Quit if auto download is disabled.
	 */
	if (!config->GetIntValue("YouTube", "AutoDownload", False)) { previousClipboardText = NIL; return; }

	/* Get text from clipboard.
	 */
	String	 previousText = previousClipboardText;
	String	 clipboardText = GetClipboardText();

	if (previousText == clipboardText) return;

	/* Check if it's a valid URL.
	 */
	if (GetVideoSiteForURL(clipboardText) == NIL) return;

	/* Start download.
	 */
	StartDownload(clipboardText);
}

/* Called when the download URL is modified.
 * ----
 * Checks for a valid URL and toggles download button state.
 */
Void BoCA::LayerYouTube::OnEditDownloadURL()
{
	if ((edit_url->GetText().StartsWith("http://") ||
	     edit_url->GetText().StartsWith("https://")) &&
	     edit_url->GetText().Length() >= 11) button_add_url->Activate();
	else					 button_add_url->Deactivate();
}

/* Called when the download button is pressed.
 * ----
 * Starts the download of the video.
 */
Void BoCA::LayerYouTube::OnDownloadTrack()
{
	I18n	*i18n	= I18n::Get();

	i18n->SetContext("Extensions::Video Downloader::Errors");

	/* Let's see if we are already downloading this video.
	 */
	for (Int i = 0; i < list_downloads->Length(); i++)
	{
		VideoListEntry	*entry = (VideoListEntry *) list_downloads->GetNthEntry(i);
		Video		*video = entry->GetVideo();

		if (video->GetVideoURL() == edit_url->GetText() && !video->IsDownloadFinished() && !video->IsDownloadCancelled()) { Utilities::ErrorMessage(i18n->TranslateString("You are already downloading this video!")); return; }
	}

	/* Find a video site that can handle the URL.
	 */
	VideoSite	*videoSite = GetVideoSiteForURL(edit_url->GetText());

	if (videoSite == NIL) { Utilities::ErrorMessage(i18n->TranslateString("This URL is not supported or not a video page URL!")); return; }

	/* Start download.
	 */
	StartDownload(edit_url->GetText());

	/* Clean up.
	 */
	edit_url->SetText(NIL);
	button_add_url->Deactivate();
}

/* Called when the download is finished.
 * ----
 * Checks if download succeeded and finishes download.
 */
Void BoCA::LayerYouTube::OnFinishDownload(Video *video)
{
	if (video->IsDownloadCancelled()) return;

	I18n	*i18n	= I18n::Get();

	i18n->SetContext("Extensions::Video Downloader::Errors");

	if (!File(video->GetVideoFile()).Exists()) { Utilities::ErrorMessage(i18n->TranslateString("Error downloading video!")); return; }

	/* Finish download.
	 */
	FinishDownload(video);
}

/* Called when a list entry is selected.
 * ----
 * Finds the corresponding track and emits onSelectTrack.
 */
Void BoCA::LayerYouTube::OnSelectTrack()
{
	const Track	&track = tracks.GetNth(list_tracks->GetSelectedEntryNumber());
	const Info	&info = track.GetInfo();

	edit_title->Activate();
	edit_title->SetText(info.title);

	edit_description->Activate();
	edit_description->SetText(info.comment);

	link_source->SetText(NIL);
	link_source->SetURL(NIL);

	text_site_value->SetText(NIL);
	text_uploader_value->SetText(NIL);
	text_date_value->SetText(NIL);

	foreach (const String &value, info.other)
	{
		if	(value.StartsWith(String(INFO_WEB_SOURCE).Append(":")))	  { link_source->SetURL(value.Tail(value.Length() - value.Find(":") - 1)); OnChangeSize(GetSize()); }

		else if	(value.StartsWith(String("Video site").Append(":")))	    text_site_value->SetText(value.Tail(value.Length() - value.Find(":") - 1));
		else if	(value.StartsWith(String("Video uploader").Append(":")))    text_uploader_value->SetText(value.Tail(value.Length() - value.Find(":") - 1));
		else if	(value.StartsWith(String("Video upload date").Append(":"))) text_date_value->SetText(value.Tail(value.Length() - value.Find(":") - 1));
	}

	if (track.pictures.Length() > 0) image_cover->SetBitmap(track.pictures.GetFirst().GetBitmap());

	area_cover->SetColor(Color(255, 255, 255));
	image_cover->Paint(SP_PAINT);

	Font	 font;

	font.SetColor(Setup::TextColor);

	text_site->SetFont(font);
	text_source->SetFont(font);
	text_title->SetFont(font);
	text_description->SetFont(font);

	text_uploader->SetFont(font);
	text_date->SetFont(font);

	JobList::Get()->onComponentSelectTrack.Emit(track);
}

/* Called when no list entry is selected.
 * ----
 * Clears up all info fields.
 */
Void BoCA::LayerYouTube::OnSelectNone()
{
	edit_title->SetText(NIL);
	edit_title->Deactivate();

	edit_description->SetText(NIL);
	edit_description->Deactivate();

	link_source->SetText(NIL);
	link_source->SetURL(NIL);

	text_site_value->SetText(NIL);
	text_uploader_value->SetText(NIL);
	text_date_value->SetText(NIL);

	image_cover->SetBitmap(NIL);
	area_cover->SetColor(Setup::BackgroundColor);

	Font	 font;

	font.SetColor(Setup::InactiveTextColor);

	text_site->SetFont(font);
	text_source->SetFont(font);
	text_title->SetFont(font);
	text_description->SetFont(font);

	text_uploader->SetFont(font);
	text_date->SetFont(font);
}

/* Called when info for the selected track is edited.
 * ----
 * Updates the selected track.
 */
Void BoCA::LayerYouTube::OnEditMetadata()
{
	Track	&track = tracks.GetNthReference(list_tracks->GetSelectedEntryNumber());
	Info	 info = track.GetInfo();

	info.title	= edit_title->GetText();
	info.comment	= edit_description->GetText();

	track.SetInfo(info);

	JobList::Get()->onComponentModifyTrack.Emit(track);
}

/* Called when a track is modified by the application.
 * ----
 * Modifies our corresponding entry accordingly.
 */
Void BoCA::LayerYouTube::OnApplicationModifyTrack(const Track &track)
{
	I18n	*i18n	= I18n::Get();

	i18n->SetContext("Extensions::Video Downloader");

	const Info	&info = track.GetInfo();
	String		 jlEntry;

	if (info.artist == NIL && info.title == NIL) jlEntry = String("\t").Append(track.origFilename).Append("\t");
	else					     jlEntry = String(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown")).Append("\t").Append(info.title.Length() > 0 ? info.title : i18n->TranslateString("unknown title")).Append("\t");

	jlEntry.Append(track.GetLengthString()).Append("\t").Append(track.GetFileSizeString());

	for (Int i = 0; i < list_tracks->Length(); i++)
	{
		if (tracks.Get(list_tracks->GetNthEntry(i)->GetHandle()).GetTrackID() == track.GetTrackID())
		{
			list_tracks->GetNthEntry(i)->SetText(jlEntry);

			tracks.GetReference(list_tracks->GetNthEntry(i)->GetHandle()) = track;

			/* Call OnSelectTrack to update info fields.
			 */
			if (list_tracks->GetSelectedEntryNumber() == i) OnSelectTrack();

			break;
		}
	}
}

/* Called when a track is removed from the application joblist.
 * ----
 * Removes our corresponding entry from tracks and list_tracks.
 */
Void BoCA::LayerYouTube::OnApplicationRemoveTrack(const Track &track)
{
	Config	*config = Config::Get();

	for (Int i = 0; i < list_tracks->Length(); i++)
	{
		if (tracks.Get(list_tracks->GetNthEntry(i)->GetHandle()).GetTrackID() == track.GetTrackID())
		{
			if (!config->GetIntValue("YouTube", "SaveVideoFiles", False) || track.origFilename.StartsWith(S::System::System::GetTempDirectory())) File(track.origFilename).Delete();

			tracks.Remove(list_tracks->GetNthEntry(i)->GetHandle());

			list_tracks->Remove(list_tracks->GetNthEntry(i));

			break;
		}
	}

	if (list_tracks->GetSelectedEntry() == NIL || list_tracks->Length() == 0) OnSelectNone();
}

/* Called when a track is selected in the application joblist.
 * ----
 * Finds and selects the corresponding entry in our track list.
 */
Void BoCA::LayerYouTube::OnApplicationSelectTrack(const Track &track)
{
	for (Int i = 0; i < list_tracks->Length(); i++)
	{
		if (tracks.Get(list_tracks->GetNthEntry(i)->GetHandle()).GetTrackID() == track.GetTrackID())
		{
			if (list_tracks->GetSelectedEntryNumber() != i) list_tracks->SelectNthEntry(i);

			break;
		}
	}
}

/* Called when all tracks are removed from the application joblist at once.
 * ----
 * Clears tracks and list_tracks.
 */
Void BoCA::LayerYouTube::OnApplicationRemoveAllTracks()
{
	Config	*config = Config::Get();

	foreach (const Track &track, tracks)
	{
		if (!config->GetIntValue("YouTube", "SaveVideoFiles", False) || track.origFilename.StartsWith(S::System::System::GetTempDirectory())) File(track.origFilename).Delete();
	}

	tracks.RemoveAll();

	list_tracks->RemoveAllEntries();

	OnSelectNone();
}

/* Called when application is about to quit.
 * ----
 * Stop downloads and clean up videolist entries.
 */
Void BoCA::LayerYouTube::OnQuit()
{
	/* Delete video list entries.
	 */
	for (Int i = 0; i < list_downloads->Length(); i++)
	{
		VideoListEntry	*entry = (VideoListEntry *) list_downloads->GetNthEntry(i);
		Video		*video = entry->GetVideo();

		list_downloads->Remove(entry);

		DeleteObject(entry);

		delete video;
	}
}

/* Start video download.
 * ----
 */
Bool BoCA::LayerYouTube::StartDownload(const String &URL)
{
	/* Find a video site that can handle the URL.
	 */
	VideoSite	*videoSite = GetVideoSiteForURL(URL);

	if (videoSite == NIL) return False;

	/* Create video object.
	 */
	Video	*video = new Video(videoSite);

	video->SetVideoURL(URL);
	video->finishDownload.Connect(&LayerYouTube::OnFinishDownload, this);

	list_downloads->Add(new VideoListEntry(video));
	list_downloads->ScrollToEndOfList();

	/* Get file extension.
	 */
	String		 fileExt = "flv";
	AS::Registry	&boca = AS::Registry::Get();

	for (Int j = 0; j < boca.GetNumberOfComponents(); j++)
	{
		if (boca.GetComponentID(j) != video->GetDecoderID()) continue;

		const Array<AS::FileFormat *>	&formats = boca.GetComponentFormats(j);

		if (formats.Length() >= 1)
		{
			const Array<String>	&extensions = formats.GetFirst()->GetExtensions();

			if (extensions.Length() >= 1) fileExt = extensions.GetFirst();
		}

		break;
	}

	/* Start download.
	 */
	Config	*config = Config::Get();
	String	 videoFile = S::System::System::GetTempDirectory().Append("video_temp_").Append(String::FromInt(S::System::System::Clock())).Append(".").Append(fileExt);

	if (config->GetIntValue("YouTube", "SaveVideoFiles", False))
	{
		video->QueryMetadata();

		videoFile = config->GetStringValue("YouTube", "VideoOutputDir", S::System::System::GetPersonalFilesDirectory(S::System::PersonalFilesMovies));

		if (!videoFile.EndsWith(Directory::GetDirectoryDelimiter())) videoFile.Append(Directory::GetDirectoryDelimiter());

		videoFile.Append(Utilities::ReplaceIncompatibleCharacters(video->GetVideoTitle()));

		Int	 count = 1;

		while (File(String(videoFile).Append(".").Append(fileExt)).Exists())
		{
			count++;

			if (!File(String(videoFile).Append(" (").Append(String::FromInt(count)).Append(").").Append(fileExt)).Exists())
			{
				videoFile.Append(" (").Append(String::FromInt(count)).Append(")");

				break;
			}
		}

		videoFile.Append(".").Append(fileExt);
	}

	video->SetVideoFile(videoFile);
	video->Download();

	return True;
}

/* Finish video download.
 * ----
 */
Bool BoCA::LayerYouTube::FinishDownload(Video *video)
{
	/* Open track and get basic info.
	 */
	AS::Registry		&boca = AS::Registry::Get();
	AS::DecoderComponent	*decoder = (AS::DecoderComponent *) boca.CreateComponentByID(video->GetDecoderID());

	if (decoder == NIL) return False;

	Track	 track;

	track.origFilename = video->GetVideoFile();

	decoder->GetStreamInfo(track.origFilename, track);

	boca.DeleteComponent(decoder);

	/* Fill in metadata.
	 */
	Info	 info;

	info.artist	= video->GetVideoUploader();
	info.title	= video->GetVideoTitle();
	info.comment	= video->GetVideoDescription();

	info.other.Add(String(INFO_WEB_SOURCE).Append(":").Append(video->GetVideoURL()));

	info.other.Add(String("Video site").Append(":").Append(video->GetVideoSiteName()));
	info.other.Add(String("Video uploader").Append(":").Append(video->GetVideoUploader()));
	info.other.Add(String("Video upload date").Append(":").Append(video->GetVideoDate()));

	if (video->GetVideoThumbnail().mime == "image/jpeg") track.pictures.Add(video->GetVideoThumbnail());

	track.SetInfo(info);

	I18n	*i18n	= I18n::Get();

	i18n->SetContext("Extensions::Video Downloader");

	/* Add track to joblist.
	 */
	JobList::Get()->onComponentAddTrack.Emit(track);

	String		 jlEntry = String(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown")).Append("\t").Append(info.title.Length() > 0 ? info.title : i18n->TranslateString("unknown title")).Append("\t").Append(track.GetLengthString()).Append("\t").Append(track.GetFileSizeString());

	tracks.Add(track, list_tracks->AddEntry(jlEntry)->GetHandle());

	return True;
}
