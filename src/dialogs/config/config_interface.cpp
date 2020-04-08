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

#include <dialogs/config/config_interface.h>

#include <config.h>

freac::ConfigureInterface::ConfigureInterface()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Joblist");

	showJobs = config->GetIntValue(Config::CategorySettingsID, Config::SettingsShowJobsTabID, Config::SettingsShowJobsTabDefault);

	group_joblist	= new GroupBox(i18n->TranslateString("Joblist"), Point(7, 11), Size(552, 136));

	text_fields	= new Text(i18n->AddColon(i18n->TranslateString("Columns")), Point(9, 12));

	list_fields	= new ListBox(Point(16 + text_fields->GetUnscaledTextWidth(), 10), group_joblist->GetSize() - Size(114 + text_fields->GetUnscaledTextWidth(), 20));
	list_fields->SetFlags(LF_ALLOWREORDER | LF_MULTICHECKBOX);
	list_fields->AddTab(i18n->TranslateString("Name"), 100);
	list_fields->AddTab(i18n->TranslateString("Description"));
	list_fields->onSelectEntry.Connect(&ConfigureInterface::OnSelectJoblistField, this);

	/* Connect OnSelectJoblistField to check buttons state.
	 */
	list_fields->onChangeEntryOrder.Connect(&ConfigureInterface::OnSelectJoblistField, this);

	button_up	= new Button(i18n->TranslateString("Up"), Point(90, 10), Size());
	button_up->SetOrientation(OR_UPPERRIGHT);
	button_up->Deactivate();
	button_up->onAction.Connect(&ConfigureInterface::JoblistFieldMoveUp, this);

	button_down	= new Button(i18n->TranslateString("Down"), Point(90, 40), Size());
	button_down->SetOrientation(OR_UPPERRIGHT);
	button_down->Deactivate();
	button_down->onAction.Connect(&ConfigureInterface::JoblistFieldMoveDown, this);

	group_joblist->Add(text_fields);
	group_joblist->Add(list_fields);
	group_joblist->Add(button_up);
	group_joblist->Add(button_down);

	group_jobs	= new GroupBox(i18n->TranslateString("Jobs"), Point(7, 159), Size(552, 40));

	check_show_jobs	= new CheckBox(i18n->TranslateString("Show Jobs tab"), Point(10, 13), Size(group_jobs->GetWidth() - 20, 0), &showJobs);

	group_jobs->Add(check_show_jobs);

	FillJoblistFieldsList();

	Add(group_joblist);
	Add(group_jobs);

	SetSize(Size(566, 206));
}

freac::ConfigureInterface::~ConfigureInterface()
{
	DeleteObject(group_joblist);
	DeleteObject(text_fields);
	DeleteObject(list_fields);
	DeleteObject(button_up);
	DeleteObject(button_down);

	DeleteObject(group_jobs);
	DeleteObject(check_show_jobs);
}

Void freac::ConfigureInterface::FillJoblistFieldsList()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Joblist::Columns");

	Array<String>	 entryTexts;
	Array<Bool>	 entryPresent;

	entryTexts.Add(String("<artist>").Append(ListEntry::tabDelimiter).Append(i18n->TranslateString("Track artist")));	   entryPresent.Add(False);
	entryTexts.Add(String("<title>").Append(ListEntry::tabDelimiter).Append(i18n->TranslateString("Track title")));		   entryPresent.Add(False);
	entryTexts.Add(String("<genre>").Append(ListEntry::tabDelimiter).Append(i18n->TranslateString("Track genre")));		   entryPresent.Add(False);
	entryTexts.Add(String("<album>").Append(ListEntry::tabDelimiter).Append(i18n->TranslateString("Album title")));		   entryPresent.Add(False);
	entryTexts.Add(String("<albumartist>").Append(ListEntry::tabDelimiter).Append(i18n->TranslateString("Album artist")));	   entryPresent.Add(False);
	entryTexts.Add(String("<disc>").Append(ListEntry::tabDelimiter).Append(i18n->TranslateString("Disc number")));		   entryPresent.Add(False);
	entryTexts.Add(String("<track>").Append(ListEntry::tabDelimiter).Append(i18n->TranslateString("Track number")));	   entryPresent.Add(False);
	entryTexts.Add(String("<rating>").Append(ListEntry::tabDelimiter).Append(i18n->TranslateString("Track rating")));	   entryPresent.Add(False);
	entryTexts.Add(String("<time>").Append(ListEntry::tabDelimiter).Append(i18n->TranslateString("Track length")));		   entryPresent.Add(False);
	entryTexts.Add(String("<bytes>").Append(ListEntry::tabDelimiter).Append(i18n->TranslateString("Track size")));		   entryPresent.Add(False);
	entryTexts.Add(String("<bitrate>").Append(ListEntry::tabDelimiter).Append(i18n->TranslateString("Bitrate")));		   entryPresent.Add(False);
	entryTexts.Add(String("<samplerate>").Append(ListEntry::tabDelimiter).Append(i18n->TranslateString("Sampling rate")));     entryPresent.Add(False);
	entryTexts.Add(String("<channels>").Append(ListEntry::tabDelimiter).Append(i18n->TranslateString("Channels")));		   entryPresent.Add(False);
	entryTexts.Add(String("<resolution>").Append(ListEntry::tabDelimiter).Append(i18n->TranslateString("Sample resolution"))); entryPresent.Add(False);
	entryTexts.Add(String("<drive>").Append(ListEntry::tabDelimiter).Append(i18n->TranslateString("CD drive")));		   entryPresent.Add(False);
	entryTexts.Add(String("<discid>").Append(ListEntry::tabDelimiter).Append(i18n->TranslateString("Disc ID")));		   entryPresent.Add(False);
	entryTexts.Add(String("<file>").Append(ListEntry::tabDelimiter).Append(i18n->TranslateString("File name")));		   entryPresent.Add(False);
	entryTexts.Add(String("<filetype>").Append(ListEntry::tabDelimiter).Append(i18n->TranslateString("File type")));	   entryPresent.Add(False);
	entryTexts.Add(String("<outputfile>").Append(ListEntry::tabDelimiter).Append(i18n->TranslateString("Output file name")));  entryPresent.Add(False);

	const Array<String>	&fields = BoCA::Config::Get()->GetStringValue(Config::CategoryJoblistID, Config::JoblistFieldsID, Config::JoblistFieldsDefault).Explode(",");

	foreach (const String &field, fields)
	{
		for (Int i = 0; i < entryTexts.Length(); i++)
		{
			const String	&entryText = entryTexts.GetNth(i);

			if (entryText.StartsWith(field)) { list_fields->AddEntry(entryText, True); entryPresent.SetNth(i, True); }
		}
	}

	for (Int i = 0; i < entryTexts.Length(); i++)
	{
		const String	&entryText = entryTexts.GetNth(i);

		if (!entryPresent.GetNth(i)) list_fields->AddEntry(entryText, False);
	}
}

Void freac::ConfigureInterface::OnSelectJoblistField()
{
	if (list_fields->GetSelectedEntryNumber() == 0)				button_up->Deactivate();
	else									button_up->Activate();

	if (list_fields->GetSelectedEntryNumber() == list_fields->Length() - 1) button_down->Deactivate();
	else									button_down->Activate();
}

Void freac::ConfigureInterface::JoblistFieldMoveUp()
{
	list_fields->MoveEntry(list_fields->GetSelectedEntryNumber(), list_fields->GetSelectedEntryNumber() - 1);

	list_fields->Paint(SP_UPDATE);
}

Void freac::ConfigureInterface::JoblistFieldMoveDown()
{
	list_fields->MoveEntry(list_fields->GetSelectedEntryNumber(), list_fields->GetSelectedEntryNumber() + 1);

	list_fields->Paint(SP_UPDATE);
}

Int freac::ConfigureInterface::SaveSettings()
{
	String	 fields;

	for (Int i = 0; i < list_fields->Length(); i++)
	{
		ListEntry	*entry	   = list_fields->GetNthEntry(i);
		const String	&entryText = entry->GetText();

		if (entry->IsMarked()) fields = fields.Append(fields != NIL ? "," : NIL).Append(entryText.Head(entryText.Find(ListEntry::tabDelimiter)));
	}

	BoCA::Config	*config = BoCA::Config::Get();

	config->SetStringValue(Config::CategoryJoblistID, Config::JoblistFieldsID, fields);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsShowJobsTabID, showJobs);

	return Success();
}
