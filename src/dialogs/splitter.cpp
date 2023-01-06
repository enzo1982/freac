 /* fre:ac - free audio converter
  * Copyright (C) 2001-2023 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/splitter.h>

#include <engine/decoder.h>

#include <jobs/engine/convert.h>

#include <config.h>
#include <utilities.h>

using namespace BoCA;
using namespace BoCA::AS;

namespace freac
{
	class RepeatPreviousPartComponent : public Layer
	{
		private:
			CheckBox		*check_repeat;
			Text			*text_repeat;
			EditBox			*edit_repeat;
			Arrows			*arrows_repeat;
			Text			*text_repeat_sec;

			Bool			 repeatEnd;
			Int			 repeatSeconds;
		public:
						 RepeatPreviousPartComponent(const Point &, Bool, Int);
						~RepeatPreviousPartComponent();

			Void			 SetRepeat(Bool);
			Void			 SetDuration(Int);
		signals:
			Signal1<Void, Bool>	 onToggleRepeat;
			Signal1<Void, Int>	 onChangeDuration;
		slots:
			Void			 OnToggleRepeat();
			Void			 OnEditDuration();
			Void			 OnChangeDuration();
	};
};

freac::RepeatPreviousPartComponent::RepeatPreviousPartComponent(const Point &pos, Bool repeat, Int seconds)
{
	repeatEnd     = repeat;
	repeatSeconds = seconds;

	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Dialogs::Splitter");

	check_repeat	= new CheckBox(i18n->TranslateString("Repeat end of previous part at beginning of each part"), Point(0, 1), Size(140, 0), &repeatEnd);
	check_repeat->SetWidth(check_repeat->GetUnscaledTextWidth() + 21);
	check_repeat->onAction.Connect(&RepeatPreviousPartComponent::OnToggleRepeat, this);

	text_repeat	= new Text(i18n->AddColon(i18n->TranslateString("Repeat duration")), Point(check_repeat->GetWidth() + 8, 3));

	edit_repeat	= new EditBox(String::FromInt(repeatSeconds), Point(text_repeat->GetX() + text_repeat->GetUnscaledTextWidth() + 9, 0), Size(24, 0), 3);
	edit_repeat->SetFlags(EDB_NUMERIC);
 	edit_repeat->onInput.Connect(&RepeatPreviousPartComponent::OnEditDuration, this);

	arrows_repeat	= new Arrows(Point(edit_repeat->GetX() + edit_repeat->GetWidth() + 1, 1), Size(15, 18), OR_VERT, &repeatSeconds, 1, 120);
	arrows_repeat->onValueChange.Connect(&RepeatPreviousPartComponent::OnChangeDuration, this);

	text_repeat_sec	= new Text(i18n->TranslateString("second(s)"), Point(arrows_repeat->GetX() + 23, 3));

	Add(check_repeat);
	Add(text_repeat);
	Add(edit_repeat);
	Add(arrows_repeat);
	Add(text_repeat_sec);

	SetMetrics(pos, Size(text_repeat_sec->GetX() + text_repeat_sec->GetUnscaledTextWidth(), 25));
}

freac::RepeatPreviousPartComponent::~RepeatPreviousPartComponent()
{
	DeleteObject(check_repeat);
	DeleteObject(text_repeat);
	DeleteObject(edit_repeat);
	DeleteObject(arrows_repeat);
	DeleteObject(text_repeat_sec);
}

Void freac::RepeatPreviousPartComponent::SetRepeat(Bool repeat)
{
	repeatEnd = repeat;

	check_repeat->SetChecked(repeatEnd);

	if (repeatEnd)
	{
		text_repeat->Activate();
		edit_repeat->Activate();
		arrows_repeat->Activate();
		text_repeat_sec->Activate();
	}
	else
	{
		text_repeat->Deactivate();
		edit_repeat->Deactivate();
		arrows_repeat->Deactivate();
		text_repeat_sec->Deactivate();
	}
}

Void freac::RepeatPreviousPartComponent::SetDuration(Int seconds)
{
	repeatSeconds = Math::Max(1, Math::Min(seconds, 120));

	edit_repeat->SetText(String::FromInt(repeatSeconds));
	arrows_repeat->SetValue(repeatSeconds);
}

Void freac::RepeatPreviousPartComponent::OnToggleRepeat()
{
	SetRepeat(repeatEnd);

	onToggleRepeat.Emit(repeatEnd);
}

Void freac::RepeatPreviousPartComponent::OnEditDuration()
{
	SetDuration(edit_repeat->GetText().ToInt());

	onChangeDuration.Emit(repeatSeconds);
}

Void freac::RepeatPreviousPartComponent::OnChangeDuration()
{
	SetDuration(repeatSeconds);

	onChangeDuration.Emit(repeatSeconds);
}

freac::DialogSplitter::DialogSplitter(const Array<Track> &tracks)
{
	disableUpdates		  = True;

	cancelUpdateTrackLengths  = False;
	updateTrackLengthsRunning = False;

	/* Get configuration.
	 */
	BoCA::Config	*config = BoCA::Config::Get();

	splitMode      = config->GetIntValue(Config::CategorySplitterID, Config::SplitterSplitModeID, Config::SplitterSplitModeDefault);
	splitAtTracks  = config->GetIntValue(Config::CategorySplitterID, Config::SplitterSplitAtTracksID, Config::SplitterSplitAtTracksDefault);

	partDuration   = config->GetIntValue(Config::CategorySplitterID, Config::SplitterPartDurationID, Config::SplitterPartDurationDefault);
	numberOfParts  = config->GetIntValue(Config::CategorySplitterID, Config::SplitterNumberOfPartsID, Config::SplitterNumberOfPartsDefault);

	repeatEnd      = config->GetIntValue(Config::CategorySplitterID, Config::SplitterRepeatEndID, Config::SplitterRepeatEndDefault);
	repeatSeconds  = config->GetIntValue(Config::CategorySplitterID, Config::SplitterRepeatSecondsID, Config::SplitterRepeatSecondsDefault);

	/* Setup window metrics.
	 */
	Rect	 workArea      = S::System::Screen::GetActiveScreenWorkArea();
	Rect	 virtualScreen = Screen::GetVirtualScreenMetrics();
	Float	 scaleFactor   = Surface().GetSurfaceDPI() / 96.0;

	Point	 defaultPos    = workArea.GetPosition() + Point((workArea.GetSize().cx - Config::SplitterWindowSizeXDefault) / 2, (workArea.GetSize().cy - Config::SplitterWindowSizeYDefault - 50) / 2);

	Point	 wndPos	       = Point(config->GetIntValue(Config::CategorySplitterID, Config::SplitterWindowPosXID, defaultPos.x), config->GetIntValue(Config::CategorySplitterID, Config::SplitterWindowPosYID, defaultPos.y));
	Size	 wndSize       = Size(config->GetIntValue(Config::CategorySplitterID, Config::SplitterWindowSizeXID, Config::SplitterWindowSizeXDefault), config->GetIntValue(Config::CategorySplitterID, Config::SplitterWindowSizeYID, Config::SplitterWindowSizeYDefault));

	if (wndPos.x + wndSize.cx * scaleFactor > virtualScreen.right + 2 ||
	    wndPos.y + wndSize.cy * scaleFactor > virtualScreen.bottom + 2)
	{
		wndPos.x = (Int) Math::Min(virtualScreen.right - 10 - Math::Round(wndSize.cx * scaleFactor), (Int64) wndPos.x);
		wndPos.y = (Int) Math::Min(virtualScreen.bottom - 10 - Math::Round(wndSize.cy * scaleFactor), (Int64) wndPos.y);
	}

	if (wndPos.x < virtualScreen.left - 2 ||
	    wndPos.y < virtualScreen.top - 2)
	{
		wndPos.x = (Int) Math::Max(virtualScreen.left + 10, wndPos.x);
		wndPos.y = (Int) Math::Max(virtualScreen.top + 10, wndPos.y);

		wndSize.cx = (Int) Math::Min(Math::Round((virtualScreen.right - 20) / scaleFactor), (Int64) wndSize.cx);
		wndSize.cy = (Int) Math::Min(Math::Round((virtualScreen.bottom - 20) / scaleFactor), (Int64) wndSize.cy);
	}

	config->SetIntValue(Config::CategorySplitterID, Config::SplitterWindowPosXID, wndPos.x);
	config->SetIntValue(Config::CategorySplitterID, Config::SplitterWindowPosYID, wndPos.y);

	config->SetIntValue(Config::CategorySplitterID, Config::SplitterWindowSizeXID, wndSize.cx);
	config->SetIntValue(Config::CategorySplitterID, Config::SplitterWindowSizeYID, wndSize.cy);

	/* Create UI elements.
	 */
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Dialogs::Splitter");

	mainWnd			= new Window(i18n->TranslateString("Output splitting tool"), wndPos, wndSize);
	mainWnd->SetMinimumSize(Size(580, 410));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());
	mainWnd->GetMainLayer()->onChangeSize.Connect(&DialogSplitter::OnChangeSize, this);

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	button_convert		= new Button(i18n->TranslateString("Convert"), Point(87, 29), Size());
	button_convert->onAction.Connect(&DialogSplitter::OnConvert, this);
	button_convert->SetOrientation(OR_LOWERRIGHT);
	button_convert->SetWidth(Math::Max(80, button_convert->GetUnscaledTextWidth() + 14));
	button_convert->SetX(7 + button_convert->GetWidth());

	button_save		= new Button(i18n->TranslateString("Save"), Point(175, 29), Size());
	button_save->onAction.Connect(&DialogSplitter::OnSave, this);
	button_save->SetOrientation(OR_LOWERRIGHT);
	button_save->SetX(15 + button_convert->GetWidth() + button_save->GetWidth());

	button_cancel		= new Button(i18n->TranslateString("Cancel"), Point(175, 29), Size());
	button_cancel->onAction.Connect(&DialogSplitter::OnCancel, this);
	button_cancel->SetOrientation(OR_LOWERRIGHT);
	button_cancel->SetX(23 + button_convert->GetWidth() + button_save->GetWidth() + button_cancel->GetWidth());

	/* Track list.
	 */
	Config	*freacConfig	= Config::Get();

	text_tracks		= new Text(i18n->AddColon(i18n->TranslateString("%1 track(s) in joblist").Replace("%1", String::FromInt(tracks.Length()))), Point(7, 5));

	i18n->SetContext("Joblist");

	button_select_all	= new Button(ImageLoader::Load(String(freacConfig->resourcesPath).Append("icons/select/select-all.png")), Point(-2, 20), Size(21, 21));
	button_select_all->onAction.Connect(&DialogSplitter::SelectAll, this);
	button_select_all->SetFlags(BF_NOFRAME);
	button_select_all->SetTooltipText(i18n->TranslateString("Select all"));

	button_select_none	= new Button(ImageLoader::Load(String(freacConfig->resourcesPath).Append("icons/select/select-none.png")), Point(-2, 34), Size(21, 21));
	button_select_none->onAction.Connect(&DialogSplitter::SelectNone, this);
	button_select_none->SetFlags(BF_NOFRAME);
	button_select_none->SetTooltipText(i18n->TranslateString("Select none"));

	button_select_toggle	= new Button(ImageLoader::Load(String(freacConfig->resourcesPath).Append("icons/select/select-toggle.png")), Point(-2, 48), Size(21, 21));
	button_select_toggle->onAction.Connect(&DialogSplitter::ToggleSelection, this);
	button_select_toggle->SetFlags(BF_NOFRAME);
	button_select_toggle->SetTooltipText(i18n->TranslateString("Toggle selection"));

	Int	 numAlign = i18n->IsActiveLanguageRightToLeft() ? OR_LEFT : OR_RIGHT;

	list_tracks		= new ListBox(Point(17, 24), Size(378, 90));
	list_tracks->SetFlags(LF_ALLOWREORDER | LF_MULTICHECKBOX);
	list_tracks->onMarkEntry.Connect(&DialogSplitter::OnMarkEntry, this);
	list_tracks->onClickTab.Connect(&DialogSplitter::OnClickTab, this);
	list_tracks->onChangeEntryOrder.Connect(&DialogSplitter::OnChangeConfiguration, this);
	list_tracks->AddTab(i18n->TranslateString("Artist"), 100);
	list_tracks->AddTab(i18n->TranslateString("Title"));
	list_tracks->AddTab(i18n->TranslateString("Track"), 50, numAlign);
	list_tracks->AddTab(i18n->TranslateString("Length"), 80, numAlign);

	foreach (const Track &track, tracks)
	{
		ListEntry	*entry = list_tracks->AddEntry(GetJoblistEntryString(track), True);

		joblistTracks.Add(track, entry->GetHandle());
	}

	/* Result list.
	 */
	list_result		= new ListBox(Point(17, 24), Size(378, 90));
	list_result->AddTab(i18n->TranslateString("Artist"), 100);
	list_result->AddTab(i18n->TranslateString("Title"));
	list_result->AddTab(i18n->TranslateString("Length"), 80, numAlign);

	progress_length		= new Progressbar(Point(17, 96), Size(378, 10), OR_HORZ, PB_NOTEXT, 0, 1000);
	progress_length->Hide();

	/* Configuration.
	 */
	i18n->SetContext("Dialogs::Splitter");

	group_mode		= new GroupBox(i18n->TranslateString("Split mode"), Point(7, 244), Size(300, 40));
	group_mode->SetOrientation(OR_LOWERLEFT);

	option_mode_time	= new OptionBox(i18n->TranslateString("Split by duration"), Point(10, 13), Size(140, 0), &splitMode, SplitModeTime);
	option_mode_time->onAction.Connect(&DialogSplitter::OnSelectMode, this);

	option_mode_parts	= new OptionBox(i18n->TranslateString("Fixed number of parts"), Point(10, 13), Size(140, 0), &splitMode, SplitModeParts);
	option_mode_parts->onAction.Connect(&DialogSplitter::OnSelectMode, this);

	option_mode_metadata	= new OptionBox(i18n->TranslateString("Split upon metadata change"), Point(10, 13), Size(140, 0), &splitMode, SplitModeMetadata);
	option_mode_metadata->onAction.Connect(&DialogSplitter::OnSelectMode, this);

	group_mode->Add(option_mode_time);
	group_mode->Add(option_mode_parts);
	group_mode->Add(option_mode_metadata);

	/* Split by duration.
	 */
	group_time		= new GroupBox(i18n->TranslateString("Split by duration"), Point(7, 193), Size(300, 93));
	group_time->SetOrientation(OR_LOWERLEFT);

	text_time		= new Text(i18n->AddColon(i18n->TranslateString("Duration per part")), Point(10, 16));

	combo_time_mode		= new ComboBox(Point(text_time->GetUnscaledTextWidth() + 17, 13), Size(100, 0));
	combo_time_mode->AddEntry(i18n->TranslateString("at most"));
	combo_time_mode->AddEntry(i18n->TranslateString("approximately"));
	combo_time_mode->AddEntry(i18n->TranslateString("at least"));
	combo_time_mode->SelectNthEntry(config->GetIntValue(Config::CategorySplitterID, Config::SplitterPartDurationModeID, Config::SplitterPartDurationModeDefault));
	combo_time_mode->onSelectEntry.Connect(&DialogSplitter::OnChangeConfiguration, this);

	combo_time_exact	= new ComboBox(Point(text_time->GetUnscaledTextWidth() + 17, 13), Size(100, 0));
	combo_time_exact->AddEntry(i18n->TranslateString("exactly"));
	combo_time_exact->Deactivate();

	edit_time_value		= new EditBox(String::FromInt(partDuration), Point(combo_time_mode->GetX() + combo_time_mode->GetWidth() + 8, 13), Size(24, 0), 3);
	edit_time_value->SetFlags(EDB_NUMERIC);
	edit_time_value->onInput.Connect(&DialogSplitter::OnEditDuration, this);

	arrows_time_value	= new Arrows(Point(edit_time_value->GetX() + edit_time_value->GetWidth() + 1, 14), Size(15, 18), OR_VERT, &partDuration, 1, 999);
	arrows_time_value->onValueChange.Connect(&DialogSplitter::OnChangeDuration, this);

	combo_time_unit		= new ComboBox(Point(arrows_time_value->GetX() + 23, 13), Size(80, 0));
	combo_time_unit->AddEntry(i18n->TranslateString("second(s)"));
	combo_time_unit->AddEntry(i18n->TranslateString("minute(s)"));
	combo_time_unit->AddEntry(i18n->TranslateString("hour(s)"));
	combo_time_unit->SelectNthEntry(config->GetIntValue(Config::CategorySplitterID, Config::SplitterPartDurationUnitID, Config::SplitterPartDurationUnitDefault));
	combo_time_unit->onSelectEntry.Connect(&DialogSplitter::OnChangeConfiguration, this);

	check_time_tracks	= new CheckBox(i18n->TranslateString("Split at track boundaries"), Point(10, 40), Size(140, 0), &splitAtTracks);
	check_time_tracks->onAction.Connect(&DialogSplitter::OnToggleSplitAtTracks, this);

	repeat_time		= new RepeatPreviousPartComponent(Point(10, 64), repeatEnd, repeatSeconds);
	repeat_time->onToggleRepeat.Connect(&DialogSplitter::OnToggleRepeatEnd, this);
	repeat_time->onChangeDuration.Connect(&DialogSplitter::OnChangeRepeatDuration, this);

	group_time->Add(text_time);
	group_time->Add(combo_time_mode);
	group_time->Add(combo_time_exact);
	group_time->Add(edit_time_value);
	group_time->Add(arrows_time_value);
	group_time->Add(combo_time_unit);
	group_time->Add(check_time_tracks);
	group_time->Add(repeat_time);

	/* Split by number of parts.
	 */
	group_parts		= new GroupBox(i18n->TranslateString("Fixed number of parts"), Point(7, 193), Size(300, 93));
	group_parts->SetOrientation(OR_LOWERLEFT);

	text_parts		= new Text(i18n->AddColon(i18n->TranslateString("Number of parts")), Point(10, 16));

	edit_parts		= new EditBox(Point(text_parts->GetUnscaledTextWidth() + 17, 13), Size(24, 0), 3);
	edit_parts->SetFlags(EDB_NUMERIC);
 	edit_parts->onInput.Connect(&DialogSplitter::OnEditParts, this);

	arrows_parts		= new Arrows(Point(edit_parts->GetX() + edit_parts->GetWidth() + 1, 14), Size(15, 18), OR_VERT, &numberOfParts, 2, 100);
	arrows_parts->onValueChange.Connect(&DialogSplitter::OnChangeParts, this);

	check_parts_tracks	= new CheckBox(i18n->TranslateString("Split at track boundaries"), Point(10, 40), Size(140, 0), &splitAtTracks);
	check_parts_tracks->onAction.Connect(&DialogSplitter::OnToggleSplitAtTracks, this);

	repeat_parts		= new RepeatPreviousPartComponent(Point(10, 64), repeatEnd, repeatSeconds);
	repeat_parts->onToggleRepeat.Connect(&DialogSplitter::OnToggleRepeatEnd, this);
	repeat_parts->onChangeDuration.Connect(&DialogSplitter::OnChangeRepeatDuration, this);

	group_parts->Add(text_parts);
	group_parts->Add(edit_parts);
	group_parts->Add(arrows_parts);
	group_parts->Add(check_parts_tracks);
	group_parts->Add(repeat_parts);

	/* Split upon metadata change.
	 */
	group_metadata		= new GroupBox(i18n->TranslateString("Split upon metadata change"), Point(7, 193), Size(300, 93));
	group_metadata->SetOrientation(OR_LOWERLEFT);

	text_metadata		= new Text(i18n->AddColon(i18n->TranslateString("Metadata pattern to compare")), Point(10, 16));

	edit_metadata		= new EditBox(config->GetStringValue(Config::CategorySplitterID, Config::SplitterMetadataStringID, Config::SplitterMetadataStringDefault), Point(text_metadata->GetUnscaledTextWidth() + 17, 13), Size(240, 0));
 	edit_metadata->onInput.Connect(&DialogSplitter::OnEditMetadataString, this);

	list_metadata_defaults	= new List();
	list_metadata_defaults->AddEntry("<albumartist>,<album>");
	list_metadata_defaults->AddEntry("<albumartist>,<album>,<title>");
	list_metadata_defaults->AddEntry("<directory>");
	list_metadata_defaults->AddEntry("<filename>");

	edit_metadata->SetDropDownList(list_metadata_defaults);

	repeat_metadata		= new RepeatPreviousPartComponent(Point(10, 64), repeatEnd, repeatSeconds);
	repeat_metadata->onToggleRepeat.Connect(&DialogSplitter::OnToggleRepeatEnd, this);
	repeat_metadata->onChangeDuration.Connect(&DialogSplitter::OnChangeRepeatDuration, this);

	group_metadata->Add(text_metadata);
	group_metadata->Add(edit_metadata);
	group_metadata->Add(repeat_metadata);

	/* Output file name configuration.
	 */
	group_filename		= new GroupBox(i18n->TranslateString("Output filenames"), Point(7, 89), Size(300, 42));
	group_filename->SetOrientation(OR_LOWERLEFT);

	text_filename		= new Text(i18n->AddColon(i18n->TranslateString("Filename pattern")), Point(10, 16));

	edit_filename		= new EditBox(config->GetStringValue(Config::CategorySplitterID, Config::SplitterFileNamePatternID, Config::SplitterFileNamePatternDefault), Point(text_filename->GetUnscaledTextWidth() + 17, 13), Size(240, 0));

	list_filename		= new List();
	list_filename->AddEntry(String("<albumartist> - <album>"));
	list_filename->AddEntry(String("<albumartist> - <album> - <title>"));
	list_filename->AddEntry(String("<albumartist>").Append(Directory::GetDirectoryDelimiter()).Append("<album>"));
	list_filename->AddEntry(String("<albumartist>").Append(Directory::GetDirectoryDelimiter()).Append("<album> - <title>"));
	list_filename->AddEntry(String("<filename>"));

	edit_filename->SetDropDownList(list_filename);

	group_filename->Add(text_filename);
	group_filename->Add(edit_filename);

	/* Configure widgets.
	 */
	OnSelectMode();
	OnToggleSplitAtTracks();

	OnToggleRepeatEnd(repeatEnd);

	OnChangeParts();

	disableUpdates = False;

	OnChangeConfiguration();

	/* Add widgets to dialog.
	 */
	Add(mainWnd);

	mainWnd->Add(button_convert);
	mainWnd->Add(button_save);
	mainWnd->Add(button_cancel);

	mainWnd->Add(text_tracks);

	mainWnd->Add(button_select_all);
	mainWnd->Add(button_select_none);
	mainWnd->Add(button_select_toggle);

	mainWnd->Add(list_tracks);
	mainWnd->Add(list_result);

	mainWnd->Add(progress_length);

	mainWnd->Add(group_mode);
	mainWnd->Add(group_time);
	mainWnd->Add(group_parts);
	mainWnd->Add(group_metadata);
	mainWnd->Add(group_filename);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);

	mainWnd->SetFlags(WF_MODAL);
	mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));

	/* Update inexact track lengths in joblist.
	 */
	updateTrackLengthsThread = NonBlocking0<>(&DialogSplitter::UpdateTrackLengths, this).Call();
}

freac::DialogSplitter::~DialogSplitter()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);

	DeleteObject(text_tracks);

	DeleteObject(button_select_all);
	DeleteObject(button_select_none);
	DeleteObject(button_select_toggle);

	DeleteObject(list_tracks);
	DeleteObject(list_result);

	DeleteObject(progress_length);

	DeleteObject(group_mode);
	DeleteObject(option_mode_time);
	DeleteObject(option_mode_parts);
	DeleteObject(option_mode_metadata);

	DeleteObject(group_time);
	DeleteObject(text_time);
	DeleteObject(combo_time_mode);
	DeleteObject(combo_time_exact);
	DeleteObject(edit_time_value);
	DeleteObject(arrows_time_value);
	DeleteObject(combo_time_unit);
	DeleteObject(check_time_tracks);
	DeleteObject(repeat_time);

	DeleteObject(group_parts);
	DeleteObject(text_parts);
	DeleteObject(edit_parts);
	DeleteObject(arrows_parts);
	DeleteObject(check_parts_tracks);
	DeleteObject(repeat_parts);

	DeleteObject(group_metadata);
	DeleteObject(text_metadata);
	DeleteObject(edit_metadata);
	DeleteObject(list_metadata_defaults);
	DeleteObject(repeat_metadata);

	DeleteObject(group_filename);
	DeleteObject(text_filename);
	DeleteObject(edit_filename);
	DeleteObject(list_filename);

	DeleteObject(button_convert);
	DeleteObject(button_save);
	DeleteObject(button_cancel);
}

const Error &freac::DialogSplitter::ShowDialog()
{
	mainWnd->WaitUntilClosed();

	return error;
}

Void freac::DialogSplitter::SaveSettings()
{
	/* Save configuration.
	 */
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue(Config::CategorySplitterID, Config::SplitterSplitModeID, splitMode);
	config->SetIntValue(Config::CategorySplitterID, Config::SplitterSplitAtTracksID, splitAtTracks);

	config->SetIntValue(Config::CategorySplitterID, Config::SplitterPartDurationModeID, combo_time_mode->GetSelectedEntryNumber());
	config->SetIntValue(Config::CategorySplitterID, Config::SplitterPartDurationID, partDuration);
	config->SetIntValue(Config::CategorySplitterID, Config::SplitterPartDurationUnitID, combo_time_unit->GetSelectedEntryNumber());

	config->SetIntValue(Config::CategorySplitterID, Config::SplitterNumberOfPartsID, numberOfParts);

	config->SetIntValue(Config::CategorySplitterID, Config::SplitterRepeatEndID, repeatEnd);
	config->SetIntValue(Config::CategorySplitterID, Config::SplitterRepeatSecondsID, repeatSeconds);

	config->SetStringValue(Config::CategorySplitterID, Config::SplitterMetadataStringID, edit_metadata->GetText());

	config->SetStringValue(Config::CategorySplitterID, Config::SplitterFileNamePatternID, edit_filename->GetText());

	/* Save window position.
	 */
	Rect	 wndRect = mainWnd->GetRestoredWindowRect();

	config->SetIntValue(Config::CategorySplitterID, Config::SplitterWindowPosXID, wndRect.left);
	config->SetIntValue(Config::CategorySplitterID, Config::SplitterWindowPosYID, wndRect.top);

	config->SetIntValue(Config::CategorySplitterID, Config::SplitterWindowSizeXID, wndRect.right - wndRect.left);
	config->SetIntValue(Config::CategorySplitterID, Config::SplitterWindowSizeYID, wndRect.bottom - wndRect.top);
}

String freac::DialogSplitter::GetJoblistEntryString(const BoCA::Track &track) const
{
	const Info	&info = track.GetInfo();

	return String(info.artist).Append(ListEntry::tabDelimiter)
	      .Append(info.title).Append(ListEntry::tabDelimiter)
	      .Append(info.track > 0 ? (info.track < 10 ? String("0").Append(String::FromInt(info.track)) : String::FromInt(info.track)) : String()).Append(ListEntry::tabDelimiter)
	      .Append(track.GetLengthString());
}

String freac::DialogSplitter::GetResultEntryString(const BoCA::Track &track) const
{
	const Info	&info = track.GetInfo();

	return String(info.artist).Append(ListEntry::tabDelimiter)
	      .Append(info.title).Append(ListEntry::tabDelimiter)
	      .Append(track.GetLengthString());
}

Bool freac::DialogSplitter::SortsAfter(const String &str1, const String &str2) const
{
	Int	 length = Math::Min(str1.Length(), str2.Length());

	for (Int i = 0; i < length; i++)
	{
		if	(str1[i] > str2[i]) return True;
		else if	(str1[i] < str2[i]) return False;
	}

	if (str1.Length() > str2.Length()) return True;
	else				   return False;
}

Void freac::DialogSplitter::UpdateTrackLengths()
{
	BoCA::Config	*config = BoCA::Config::Get();

	Int	 trackCount = 0;

	foreach (Track &track, joblistTracks)
	{
		if (track.length >= 0) continue;

		trackCount++;
	}

	if (trackCount == 0) return;

	/* Show progress bar.
	 */
	GUI::Application::Lock::Acquire();

	updateTrackLengthsRunning = True;

	button_convert->Deactivate();
	list_tracks->SetHeight(list_result->GetHeight() - progress_length->GetHeight() - 8);
	progress_length->Show();

	GUI::Application::Lock::Release();

	/* Process tracks.
	 */
	Int	 tracksProcessed = 0;

	foreach (Track &track, joblistTracks)
	{
		if (track.length >= 0) continue;

		/* Create decoder for track.
		 */
		Decoder	*decoder  = new Decoder(config);

		if (!decoder->Create(track.fileName, track))
		{
			delete decoder;

			continue;
		}

		/* Decode whole track.
		 */
		Bool			 failed		= False;

		const Format		&format		= track.GetFormat();
		UnsignedLong		 samplesSize	= format.rate / 8;
		Int			 bytesPerSample = format.bits / 8 * format.channels;

		Buffer<UnsignedByte>	 buffer(samplesSize * bytesPerSample);

		while (!Threads::Access::Value(cancelUpdateTrackLengths))
		{
			buffer.Resize(samplesSize * bytesPerSample);

			/* Read samples from decoder.
			 */
			Int	 bytes = decoder->Read(buffer);

			if	(bytes == -1) { failed = True; break; }
			else if (bytes ==  0)		       break;
		}

		/* Update track length if successful.
		 */
		GUI::Application::Lock::Acquire();

		if (!failed)
		{
			track.length = decoder->GetDecodedSamples();

			/* Update list entry.
			 */
			ListEntry	*entry = list_tracks->GetNthEntry(foreachindex);

			entry->SetText(GetJoblistEntryString(track));

			/* Update split result.
			 */
			OnChangeConfiguration();

			/* Update track in main joblist.
			 */
			BoCA::JobList::Get()->onComponentModifyTrack.Emit(track);
		}

		progress_length->SetValue(Math::Round(1000.0 / trackCount * ++tracksProcessed));

		GUI::Application::Lock::Release();

		decoder->Destroy();

		delete decoder;

		if (Threads::Access::Value(cancelUpdateTrackLengths)) break;
	}

	/* Hide progress bar.
	 */
	GUI::Application::Lock::Acquire();

	progress_length->Hide();
	list_tracks->SetHeight(list_result->GetHeight());
	button_convert->Activate();

	updateTrackLengthsRunning = False;

	GUI::Application::Lock::Release();
}

Void freac::DialogSplitter::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(mainWnd->GetMainLayer()->GetPosition(), mainWnd->GetMainLayer()->GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	list_tracks->SetSize(Size(clientSize.cx / 2 - 16, clientSize.cy - 279));

	list_result->SetX(clientSize.cx / 2 + 9);
	list_result->SetSize(list_tracks->GetSize() + Size(clientSize.cx & 1, 0));

	if (updateTrackLengthsRunning) list_tracks->SetHeight(list_tracks->GetHeight() - progress_length->GetHeight() - 8);

	progress_length->SetPosition(list_tracks->GetPosition() + Point(0, list_result->GetHeight() - progress_length->GetHeight()));
	progress_length->SetWidth(list_tracks->GetWidth());

	group_mode->SetWidth(clientSize.cx - 14);

	option_mode_time->SetWidth(group_mode->GetWidth() / 3 - 12);

	option_mode_parts->SetX(group_mode->GetWidth() / 3 + 6);
	option_mode_parts->SetWidth(group_mode->GetWidth() / 3 - 12 + (group_mode->GetWidth() % 3));

	option_mode_metadata->SetX(group_mode->GetWidth() / 3 * 2 + 2 + (group_mode->GetWidth() % 3));
	option_mode_metadata->SetWidth(group_mode->GetWidth() / 3 - 12);

	group_time->SetWidth(clientSize.cx - 14);
	check_time_tracks->SetWidth(group_time->GetWidth() - 20);

	group_parts->SetWidth(clientSize.cx - 14);
	check_parts_tracks->SetWidth(group_parts->GetWidth() - 20);

	group_metadata->SetWidth(clientSize.cx - 14);
	edit_metadata->SetWidth(group_metadata->GetWidth() - edit_metadata->GetX() - 10);

	group_filename->SetWidth(clientSize.cx - 14);
	edit_filename->SetWidth(group_filename->GetWidth() - edit_filename->GetX() - 10);
}

Void freac::DialogSplitter::SelectAll()
{
	disableUpdates = True;

	for (Int i = 0; i < list_tracks->Length(); i++)
	{
		if (!list_tracks->GetNthEntry(i)->IsMarked()) list_tracks->GetNthEntry(i)->SetMark(True);
	}

	disableUpdates = False;

	OnChangeConfiguration();
}

Void freac::DialogSplitter::SelectNone()
{
	disableUpdates = True;

	for (Int i = 0; i < list_tracks->Length(); i++)
	{
		if (list_tracks->GetNthEntry(i)->IsMarked()) list_tracks->GetNthEntry(i)->SetMark(False);
	}

	disableUpdates = False;

	OnChangeConfiguration();
}

Void freac::DialogSplitter::ToggleSelection()
{
	disableUpdates = True;

	for (Int i = 0; i < list_tracks->Length(); i++)
	{
		if (list_tracks->GetNthEntry(i)->IsMarked()) list_tracks->GetNthEntry(i)->SetMark(False);
		else					     list_tracks->GetNthEntry(i)->SetMark(True);
	}

	disableUpdates = False;

	OnChangeConfiguration();
}

Void freac::DialogSplitter::OnMarkEntry(ListEntry *entry)
{
	OnChangeConfiguration();
}

Void freac::DialogSplitter::OnClickTab(Int n)
{
	Surface	*surface = mainWnd->GetDrawSurface();

	surface->StartPaint(Rect(list_result->GetRealPosition(), list_result->GetRealSize()));

	/* Operate on a copy of the actual track list.
	 */
	Array<Track>	 tracks = joblistTracks;

	/* Find sorting criteria.
	 */
	Bool	 sortByArtist = (n == 0);
	Bool	 sortByTitle  = (n == 1);
	Bool	 sortByTrack  = (n == 2);
	Bool	 sortByTime   = (n == 3);

	/* Check if reverse sorting is needed.
	 */
	static Int	 previousCriteria;
	static Bool	 reverse = False;

	if (n == previousCriteria)
	{
		/* Revert sort order previously reversed.
		 */
		for (Int i = 0; reverse && i < tracks.Length(); i++) list_tracks->MoveEntry(0, tracks.Length() - i - 1);

		reverse = !reverse;
	}
	else
	{
		previousCriteria = n;

		reverse = False;
	}

	/* Sort the list using insertion sort.
	 */
	for (Int i = 0; i < tracks.Length(); i++)
	{
		Track		&thisTrack  = tracks.GetReference(list_tracks->GetNthEntry(i)->GetHandle());
		Info		 thisInfo   = thisTrack.GetInfo();
		const Format	&thisFormat = thisTrack.GetFormat();

		if	(sortByArtist) thisInfo.artist = thisInfo.artist.ToLower();
		else if (sortByTitle)  thisInfo.title  = thisInfo.title.ToLower();

		thisTrack.SetInfo(thisInfo);

		/* Find position using optimized binary search.
		 */
		Int	 bottom = 0;
		Int	 top	= i - 1;

		while (top >= bottom)
		{
			/* Select element to compare optimizing for pre-sorted lists by
			 * selecting i - 1 initially before sticking to binary search.
			 */
			Int	 m = (top == i - 1 ? top : (top + bottom) / 2);

			/* Perform comparison.
			 */
			const Track	&compTrack  = tracks.Get(list_tracks->GetNthEntry(m)->GetHandle());
			const Info	&compInfo   = compTrack.GetInfo();
			const Format	&compFormat = compTrack.GetFormat();

			if ((sortByArtist &&  SortsAfter(compInfo.artist, thisInfo.artist)					 ) ||
			    (sortByTitle  &&  SortsAfter(compInfo.title, thisInfo.title)					 ) ||
			    (sortByTrack  &&  compInfo.track > thisInfo.track							 ) ||
			    (sortByTime	  && (compTrack.length > 0 ? compTrack.length : compTrack.approxLength) / compFormat.rate >
					     (thisTrack.length > 0 ? thisTrack.length : thisTrack.approxLength) / thisFormat.rate)) top    = m - 1;
			else													    bottom = m + 1;
		}

		/* Move element to target position.
		 */
		if (Math::Max(top, bottom) != i) list_tracks->MoveEntry(i, Math::Max(top, bottom));
	}

	/* Revert sort order if requested.
	 */
	for (Int i = 0; reverse && i < tracks.Length(); i++) list_tracks->MoveEntry(0, tracks.Length() - i - 1);

	surface->EndPaint();

	/* Redraw joblist.
	 */
	list_tracks->Paint(SP_UPDATE);

}

Void freac::DialogSplitter::OnSelectMode()
{
	Surface	*surface       = mainWnd->GetDrawSurface();
	Float	 scaleFactor   = surface->GetSurfaceDPI() / 96.0;

	surface->StartPaint(Rect(group_time->GetRealPosition() - Point(0, 8 * scaleFactor), group_time->GetRealSize() + Size(0, 8 * scaleFactor)));

	group_time->Hide();
	group_parts->Hide();
	group_metadata->Hide();

	if	(splitMode == SplitModeTime)	 group_time->Show();
	else if (splitMode == SplitModeParts)	 group_parts->Show();
	else if (splitMode == SplitModeMetadata) group_metadata->Show();

	surface->EndPaint();

	OnChangeConfiguration();
}

Void freac::DialogSplitter::OnToggleSplitAtTracks()
{
	if (splitAtTracks)
	{
		combo_time_exact->Hide();
		combo_time_mode->Show();
	}
	else
	{
		combo_time_mode->Hide();
		combo_time_exact->Show();
	}

	OnChangeConfiguration();
}

Void freac::DialogSplitter::OnToggleRepeatEnd(Bool repeat)
{
	repeatEnd = repeat;

	repeat_time->SetRepeat(repeatEnd);
	repeat_parts->SetRepeat(repeatEnd);
	repeat_metadata->SetRepeat(repeatEnd);

	OnChangeConfiguration();
}

Void freac::DialogSplitter::OnEditDuration()
{
	partDuration = Math::Max(1, Math::Min(edit_time_value->GetText().ToInt(), 999));

	edit_time_value->SetText(String::FromInt(partDuration));

	OnChangeConfiguration();
}

Void freac::DialogSplitter::OnChangeDuration()
{
	edit_time_value->SetText(String::FromInt(partDuration));

	OnChangeConfiguration();
}

Void freac::DialogSplitter::OnEditParts()
{
	numberOfParts = Math::Max(2, Math::Min(edit_parts->GetText().ToInt(), 100));

	edit_parts->SetText(String::FromInt(numberOfParts));

	OnChangeConfiguration();
}

Void freac::DialogSplitter::OnChangeParts()
{
	edit_parts->SetText(String::FromInt(numberOfParts));

	OnChangeConfiguration();
}

Void freac::DialogSplitter::OnChangeRepeatDuration(Int seconds)
{
	repeatSeconds = seconds;

	repeat_time->SetDuration(repeatSeconds);
	repeat_parts->SetDuration(repeatSeconds);
	repeat_metadata->SetDuration(repeatSeconds);

	OnChangeConfiguration();
}

Void freac::DialogSplitter::OnEditMetadataString()
{
	OnChangeConfiguration();
}

Void freac::DialogSplitter::OnChangeConfiguration()
{
	if (disableUpdates) return;

	/* Get configuration.
	 */
	BoCA::Config	*config = BoCA::Config::Get();

	String	 encoderID = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);
	Int	 splitMode = this->splitMode;

	/* Protect this code by a mutex.
	 */
	static Threads::Mutex	 mutex;

	Int	 suspendCount = GUI::Application::Lock::SuspendLock();

	Threads::Lock	 lock(mutex);

	GUI::Application::Lock::ResumeLock(suspendCount);

	/* Find target duration for output tracks.
	 */
	Int64	 targetDuration = -1;

	Int	 durationMode	= 1;
	Int	 durationRate	= 48000;

	if (joblistTracks.Length() > 0) durationRate = joblistTracks.GetFirst().GetFormat().rate;

	if (splitMode == SplitModeTime)
	{
		/* Split by duration.
		 */
		Int	 durationUnit = combo_time_unit->GetSelectedEntryNumber();

		if	(durationUnit == 0) targetDuration = durationRate * edit_time_value->GetText().ToInt()	     ;
		else if (durationUnit == 1) targetDuration = durationRate * edit_time_value->GetText().ToInt() *   60;
		else			    targetDuration = durationRate * edit_time_value->GetText().ToInt() * 3600;

		durationMode = combo_time_mode->GetSelectedEntryNumber();
	}
	else if (splitMode == SplitModeParts)
	{
		/* Split by number of parts.
		 */
		Float	 totalDuration = 0;

		foreach (const Track &track, joblistTracks)
		{
			if (!list_tracks->GetNthEntry(foreachindex)->IsMarked()) continue;

			totalDuration += GetTrackDuration(track, durationRate);
		}

		if (repeatEnd) totalDuration += repeatSeconds * durationRate * (numberOfParts - 1);

		targetDuration = Math::Ceil(totalDuration / numberOfParts);
	}

	/* Generate output tracks.
	 */
	Array<Track>	 currentTrackList;
	Int64		 currentDuration = 0;
	String		 currentMetadataString;

	resultTrackLists.RemoveAll();

	for (Int i = 0; i < list_tracks->Length(); i++)
	{
		ListEntry	*entry = list_tracks->GetNthEntry(i);

		if (!entry->IsMarked()) continue;

		const Track	&track = joblistTracks.Get(entry->GetHandle());

		if (splitMode == SplitModeTime || splitMode == SplitModeParts)
		{
			Int64	 trackDuration = GetTrackDuration(track, durationRate);

			if (currentTrackList.Length() > 0 && splitAtTracks)
			{
				/* For duration modes 0 (at most) and 1 (approximate), check
				 * the duration we would reach when including the next track.
				 */
				Int64	 newDuration = currentDuration + trackDuration;

				if ((durationMode == 0 &&  newDuration			 >  targetDuration		     ) ||
				    (durationMode == 1 && (newDuration - targetDuration) > (targetDuration - currentDuration)))
				{
					resultTrackLists.Add(currentTrackList);
					currentTrackList.RemoveAll();

					currentDuration = 0;

					if (repeatEnd && i < list_tracks->Length() - 1) AddPreviousPartRepeat(currentTrackList, currentDuration, durationRate);
				}
			}

			/* Add next track or segment.
			 */
			if (splitAtTracks)
			{
				currentTrackList.Add(track);

				currentDuration += trackDuration;

				/* Check if target duration is reached.
				 */
				if (currentDuration >= targetDuration)
				{
					resultTrackLists.Add(currentTrackList);
					currentTrackList.RemoveAll();

					currentDuration = 0;

					if (repeatEnd && i < list_tracks->Length() - 1) AddPreviousPartRepeat(currentTrackList, currentDuration, durationRate);
				}
			}
			else
			{
				Track	 trackToAdd	  = track;
				Int64	 trackToAddLength = 0;

				if	(trackToAdd.length	 > 0) trackToAddLength = trackToAdd.length;
				else if (trackToAdd.approxLength > 0) trackToAddLength = trackToAdd.approxLength;

				while (currentDuration + trackDuration > targetDuration)
				{
					/* Add next segment of track to current track list.
					 */
					Track	 cutTrack	  = trackToAdd;
					Int64	 cutTrackDuration = targetDuration - currentDuration;

					cutTrack.length = cutTrackDuration * cutTrack.GetFormat().rate / durationRate;

					if (trackToAddLength - cutTrack.length < cutTrack.GetFormat().rate / 8) cutTrack.length = trackToAddLength;

					currentTrackList.Add(cutTrack);
					resultTrackLists.Add(currentTrackList);
					currentTrackList.RemoveAll();

					currentDuration = 0;
					trackDuration  -= cutTrackDuration;

					trackToAdd.sampleOffset	+= cutTrack.length;

					if	(trackToAdd.length	 > 0) trackToAdd.length	      -= cutTrack.length;
					else if (trackToAdd.approxLength > 0) trackToAdd.approxLength -= cutTrack.length;

					if (repeatEnd) AddPreviousPartRepeat(currentTrackList, currentDuration, durationRate);
				}

				if (trackToAddLength > 0) currentTrackList.Add(trackToAdd);

				currentDuration += trackDuration;
			}
		}
		else if (splitMode == SplitModeMetadata)
		{
			String	 metadataString = Utilities::GetOutputFileName(track, edit_metadata->GetText(), NIL, False, encoderID, False, True, False, False);

			if (metadataString != currentMetadataString)
			{
				if (currentTrackList.Length() > 0)
				{
					resultTrackLists.Add(currentTrackList);
					currentTrackList.RemoveAll();

					if (repeatEnd) AddPreviousPartRepeat(currentTrackList, currentDuration, durationRate);
				}

				currentMetadataString = metadataString;
			}

			currentTrackList.Add(track);
		}
	}

	if (currentTrackList.Length() > 0) resultTrackLists.Add(currentTrackList);

	/* Add track lists to result list.
	 */
	Surface	*surface = mainWnd->GetDrawSurface();

	surface->StartPaint(Rect(list_result->GetRealPosition(), list_result->GetRealSize()));

	Int		 selectedEntry = list_result->GetSelectedEntryNumber();
	Scrollbar	*scrollbar     = NIL;
	Int		 scrollbarPos  = 0;

	for (Int i = 0; i < list_result->GetNOfObjects(); i++)
	{
		Object	*object = list_result->GetNthObject(i);

		if (object->GetObjectType() != Scrollbar::classID) continue;

		scrollbar    = (Scrollbar *) object;
		scrollbarPos = scrollbar->GetValue();

		break;
	}

	list_result->SetVisibleDirect(False);
	list_result->RemoveAllEntries();

	foreach (const Array<Track> &trackList, resultTrackLists)
	{
		/* Compute total length of track list.
		 */
		Track	 resultTrack = GetIdentifyingTrack(trackList, foreachindex == 0);

		resultTrack.length	 = 0;
		resultTrack.approxLength = 0;

		foreach (const Track &track, trackList)
		{
			if	(track.length	    > 0) resultTrack.length	  += track.length	* resultTrack.GetFormat().rate / track.GetFormat().rate;
			else if (track.approxLength > 0) resultTrack.approxLength += track.approxLength * resultTrack.GetFormat().rate / track.GetFormat().rate;
		}

		if (resultTrack.approxLength > 0)
		{
			resultTrack.approxLength += resultTrack.length;
			resultTrack.length	  = -1;
		}

		/* Add consolidated track to result list.
		 */
		list_result->AddEntry(GetResultEntryString(resultTrack));
	}

	if (selectedEntry >= 0) list_result->SelectNthEntry(selectedEntry);
	if (scrollbar != NIL) scrollbar->SetValue(scrollbarPos);

	list_result->Show();
	list_result->Process(SM_MOUSEMOVE, 0, 0);

	surface->EndPaint();
}

Void freac::DialogSplitter::AddPreviousPartRepeat(Array<Track> &currentTrackList, Int64 &currentDuration, Int durationRate) const
{
	/* Add requested number of seconds from previous part.
	 */
	Int64	 durationLeft = repeatSeconds * durationRate;

	foreachreverse (const Array<Track> &trackList, resultTrackLists)
	{
		foreachreverse (const Track &track, trackList)
		{
			Track	 trackToAdd    = track;
			Int64	 trackDuration = GetTrackDuration(track, durationRate);
			Int64	 durationToAdd = Math::Min(trackDuration, durationLeft);
			Int64	 segmentLength = durationToAdd * trackToAdd.GetFormat().rate / durationRate;

			if	(trackToAdd.length	 > 0) { trackToAdd.sampleOffset = trackToAdd.length	  - segmentLength; trackToAdd.length	   = segmentLength; }
			else if (trackToAdd.approxLength > 0) { trackToAdd.sampleOffset = trackToAdd.approxLength - segmentLength; trackToAdd.approxLength = segmentLength; }

			currentTrackList.Add(trackToAdd);

			durationLeft -= durationToAdd;

			if (durationLeft == 0) break;
		}

		if (durationLeft == 0) break;
	}

	currentDuration = repeatSeconds * durationRate - durationLeft;
}

Int64 freac::DialogSplitter::GetTrackDuration(const Track &track, Int durationRate) const
{
	if	(track.length	    > 0) return durationRate * track.length	  / track.GetFormat().rate;
	else if (track.approxLength > 0) return durationRate * track.approxLength / track.GetFormat().rate;

	return Int64(durationRate) * 240;
}

const Track &freac::DialogSplitter::GetIdentifyingTrack(const Array<Track> &trackList, Bool first) const
{
	if (!repeatEnd) return trackList.GetFirst();

	Int	 durationRate = trackList.GetFirst().GetFormat().rate;
	Int64	 durationLeft = repeatSeconds * durationRate;

	foreach (const Track &track, trackList)
	{
		Int64	 trackDuration = GetTrackDuration(track, durationRate);

		if (trackDuration > durationLeft) return track;

		durationLeft -= trackDuration;
	}

	return trackList.GetFirst();
}

Void freac::DialogSplitter::OnConvert()
{
	/* Save configuration.
	 */
	SaveSettings();

	/* Wait for track length updates to finish.
	 */
	updateTrackLengthsThread->Wait();

	/* Get and adjust configuration values.
	 */
	BoCA::Config	*config = BoCA::Config::Get();

	Bool	 warnLossyToLossless = config->GetIntValue(Config::CategorySettingsID, Config::SettingsWarnLossyToLosslessID, Config::SettingsWarnLossyToLosslessDefault);
	Bool	 encodeToSingleFile  = config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault);
	Bool	 writeToInputDir     = config->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault);
	String	 singleFileName	     = config->GetStringValue(Config::CategorySettingsID, Config::SettingsSingleFilenameID, Config::SettingsSingleFilenameDefault);
	String	 fileNamePattern     = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, Config::SettingsEncoderFilenamePatternDefault);

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, True);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, False);
	config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, edit_filename->GetText());

	/* Schedule conversion jobs.
	 */
	Bool	 aborted = False;
	String	 fileNames;

	foreach (const Array<Track> &trackList, resultTrackLists)
	{
		/* Set output filename.
		 */
		const Track	&track		 = GetIdentifyingTrack(trackList, foreachindex == 0);
		String		 defaultFileName = Utilities::GetOutputFileName(config, track);
		String		 baseFileName	 = String(defaultFileName).Head(defaultFileName.FindLast("."));
		String		 fileExtension	 = String(defaultFileName).Tail(defaultFileName.Length() - defaultFileName.FindLast("."));
		String		 outputFile	 = defaultFileName;
		Int		 count		 = 1;

		while (File(outputFile).Exists() || fileNames.Contains(outputFile)) outputFile = String(baseFileName).Append(" (").Append(String::FromInt(++count)).Append(")").Append(fileExtension);

		fileNames.Append(outputFile).Append("\n");

		config->SetStringValue(Config::CategorySettingsID, Config::SettingsSingleFilenameID, outputFile);

		/* Schedule job.
		 */
		Job	*job = new JobConvert(trackList);

		if (job->RunPrecheck() != Success())
		{
			DeleteObject(job);

			aborted = True;

			break;
		}

		config->SetIntValue(Config::CategorySettingsID, Config::SettingsWarnLossyToLosslessID, False);
	}

	/* Restore configuration values.
	 */
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWarnLossyToLosslessID, warnLossyToLossless);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, encodeToSingleFile);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, writeToInputDir);
	config->SetStringValue(Config::CategorySettingsID, Config::SettingsSingleFilenameID, singleFileName);
	config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, fileNamePattern);

	/* Close dialog window.
	 */
	if (!aborted) mainWnd->Close();
}

Void freac::DialogSplitter::OnSave()
{
	SaveSettings();

	OnCancel();
}

Void freac::DialogSplitter::OnCancel()
{
	Threads::Access::Set(cancelUpdateTrackLengths, True);

	Int	 suspendCount = GUI::Application::Lock::SuspendLock();

	updateTrackLengthsThread->Wait();

	GUI::Application::Lock::ResumeLock(suspendCount);

	mainWnd->Close();
}
