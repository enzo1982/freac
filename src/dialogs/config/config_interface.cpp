 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/config_interface.h>

#include <boca.h>

BonkEnc::ConfigureInterface::ConfigureInterface()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Interface");

	group_joblist	= new GroupBox(i18n->TranslateString("Joblist"), Point(7, 11), Size(530, 120));

	text_fields	= new Text(i18n->TranslateString("Columns:"), Point(9, 13));

	list_fields	= new ListBox(Point(16 + text_fields->textSize.cx, 10), group_joblist->GetSize() - Size(114 + text_fields->textSize.cx, 20));
	list_fields->SetFlags(LF_ALLOWREORDER | LF_MULTICHECKBOX);
	list_fields->onSelectEntry.Connect(&ConfigureInterface::OnSelectJoblistField, this);

	/* Connect OnSelectJoblistField to check buttons state.
	 */
	list_fields->onChangeEntryOrder.Connect(&ConfigureInterface::OnSelectJoblistField, this);

	button_up	= new Button(i18n->TranslateString("Up"), NIL, Point(90, 10), Size());
	button_up->SetOrientation(OR_UPPERRIGHT);
	button_up->Deactivate();
	button_up->onAction.Connect(&ConfigureInterface::JoblistFieldMoveUp, this);

	button_down	= new Button(i18n->TranslateString("Down"), NIL, Point(90, 40), Size());
	button_down->SetOrientation(OR_UPPERRIGHT);
	button_down->Deactivate();
	button_down->onAction.Connect(&ConfigureInterface::JoblistFieldMoveDown, this);

	group_joblist->Add(text_fields);
	group_joblist->Add(list_fields);
	group_joblist->Add(button_up);
	group_joblist->Add(button_down);

	FillJoblistFieldsList();

	Add(group_joblist);

	SetSize(Size(544, 138));
}

BonkEnc::ConfigureInterface::~ConfigureInterface()
{
	DeleteObject(group_joblist);
	DeleteObject(text_fields);
	DeleteObject(list_fields);
	DeleteObject(button_up);
	DeleteObject(button_down);
}

Void BonkEnc::ConfigureInterface::FillJoblistFieldsList()
{
	const Array<String>	&fields = BoCA::Config::Get()->GetStringValue(Config::CategoryJoblistID, Config::JoblistFieldsID, Config::JoblistFieldsDefault).Explode(",");

	Bool	 haveArtist   = False;
	Bool	 haveAlbum    = False;
	Bool	 haveTitle    = False;
	Bool	 haveGenre    = False;
	Bool	 haveDisc     = False;
	Bool	 haveTrack    = False;
	Bool	 haveTime     = False;
	Bool	 haveBytes    = False;
	Bool	 haveFile     = False;
	Bool	 haveFileType = False;

	foreach (const String &field, fields)
	{
		if	(field == "<artist>")	{ list_fields->AddEntry(field)->SetMark(True); haveArtist   = True; }
		else if (field == "<album>")	{ list_fields->AddEntry(field)->SetMark(True); haveAlbum    = True; }
		else if (field == "<title>")	{ list_fields->AddEntry(field)->SetMark(True); haveTitle    = True; }
		else if (field == "<genre>")	{ list_fields->AddEntry(field)->SetMark(True); haveGenre    = True; }
		else if (field == "<disc>")	{ list_fields->AddEntry(field)->SetMark(True); haveDisc	    = True; }
		else if (field == "<track>")	{ list_fields->AddEntry(field)->SetMark(True); haveTrack    = True; }
		else if (field == "<time>")	{ list_fields->AddEntry(field)->SetMark(True); haveTime     = True; }
		else if (field == "<bytes>")	{ list_fields->AddEntry(field)->SetMark(True); haveBytes    = True; }
		else if (field == "<file>")	{ list_fields->AddEntry(field)->SetMark(True); haveFile	    = True; }
		else if (field == "<filetype>")	{ list_fields->AddEntry(field)->SetMark(True); haveFileType = True; }
	}

	String::ExplodeFinish();

	if (!haveArtist)   list_fields->AddEntry("<artist>")->SetMark(False);
	if (!haveAlbum)    list_fields->AddEntry("<album>")->SetMark(False);
	if (!haveTitle)    list_fields->AddEntry("<title>")->SetMark(False);
	if (!haveGenre)    list_fields->AddEntry("<genre>")->SetMark(False);
	if (!haveDisc)	   list_fields->AddEntry("<disc>")->SetMark(False);
	if (!haveTrack)    list_fields->AddEntry("<track>")->SetMark(False);
	if (!haveTime)     list_fields->AddEntry("<time>")->SetMark(False);
	if (!haveBytes)    list_fields->AddEntry("<bytes>")->SetMark(False);
	if (!haveFile)     list_fields->AddEntry("<file>")->SetMark(False);
	if (!haveFileType) list_fields->AddEntry("<filetype>")->SetMark(False);
}

Void BonkEnc::ConfigureInterface::OnSelectJoblistField()
{
	if (list_fields->GetSelectedEntryNumber() == 0)				button_up->Deactivate();
	else									button_up->Activate();

	if (list_fields->GetSelectedEntryNumber() == list_fields->Length() - 1) button_down->Deactivate();
	else									button_down->Activate();
}

Void BonkEnc::ConfigureInterface::JoblistFieldMoveUp()
{
	list_fields->SwitchEntries(list_fields->GetSelectedEntryNumber(), list_fields->GetSelectedEntryNumber() - 1);

	list_fields->Paint(SP_UPDATE);
}

Void BonkEnc::ConfigureInterface::JoblistFieldMoveDown()
{
	list_fields->SwitchEntries(list_fields->GetSelectedEntryNumber(), list_fields->GetSelectedEntryNumber() + 1);

	list_fields->Paint(SP_UPDATE);
}

Int BonkEnc::ConfigureInterface::SaveSettings()
{
	String	 fields;

	for (Int i = 0; i < list_fields->Length(); i++)
	{
		ListEntry	*entry = list_fields->GetNthEntry(i);

		if (entry->IsMarked()) fields = fields.Append(fields != NIL ? "," : "").Append(entry->GetText());
	}

	BoCA::Config::Get()->SetStringValue(Config::CategoryJoblistID, Config::JoblistFieldsID, fields);

	return Success();
}
