 /* fre:ac - free audio converter
  * Copyright (C) 2001-2022 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/config_tagformats.h>

#include <config.h>

using namespace BoCA::AS;

freac::ConfigureTagFormats::ConfigureTagFormats()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Tags");

	/* Get configuration.
	 */
	prependZero		= False;

	/* Create widgets.
	 */
	group_tags		= new GroupBox(i18n->TranslateString("Tag formats"), Point(7, 11), Size(552, 117));

	list_tag_formats	= new ListBox(Point(10, 13), Size(250, 94));
	list_tag_formats->SetFlags(LF_MULTICHECKBOX);

	Registry		&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_TAGGER) continue;

		const Array<TagSpec *>	&specs = boca.GetComponentTagSpecs(i);

		foreach (TagSpec *spec, specs)
		{
			list_tag_formats->AddEntry(spec->GetName(), config->GetIntValue(Config::CategoryTagsID, String("Enable").Append(spec->GetName().Replace(" ", NIL)), spec->IsDefault()));

			selected_encodings.Add(config->GetStringValue(Config::CategoryTagsID, spec->GetName().Replace(" ", NIL).Append("Encoding"), spec->GetDefaultEncoding()));
			tags_prependzero.Add(config->GetIntValue(Config::CategoryTagsID, String("TrackPrependZero").Append(spec->GetName().Replace(" ", NIL)), spec->IsPrependZeroDefault()));
		}
	}

	list_tag_formats->onSelectEntry.Connect(&ConfigureTagFormats::ToggleTags, this);
	list_tag_formats->onMarkEntry.Connect(&ConfigureTagFormats::ToggleTags, this);

	text_encoding		= new Text(i18n->AddColon(i18n->TranslateString("Encoding")), Point(268, 16));

	list_encodings		= new List();

	edit_encoding		= new EditBox(Point(text_encoding->GetUnscaledTextWidth() + 275, 13), Size(249 - text_encoding->GetUnscaledTextWidth(), 0));
	edit_encoding->onInput.Connect(&ConfigureTagFormats::OnEditEncoding, this);

	combo_encoding		= new ComboBox(Point(text_encoding->GetUnscaledTextWidth() + 275, 13), Size(249 - text_encoding->GetUnscaledTextWidth(), 0));
	combo_encoding->onSelectEntry.Connect(&ConfigureTagFormats::OnEditEncoding, this);
	combo_encoding->Hide();

	check_prependzero	= new CheckBox(i18n->TranslateString("Prepend zero to track numbers below 10"), Point(268, edit_encoding->GetY() + 28), Size(256, 0), &prependZero);
	check_prependzero->SetWidth(Math::Max(271, check_prependzero->GetUnscaledTextWidth() + 21));
	check_prependzero->onAction.Connect(&ConfigureTagFormats::TogglePrependZero, this);

	group_tags->SetWidth(281 + check_prependzero->GetWidth());

	group_tags->Add(list_tag_formats);
	group_tags->Add(text_encoding);
	group_tags->Add(edit_encoding);
	group_tags->Add(combo_encoding);
	group_tags->Add(check_prependzero);

	Add(group_tags);

	/* Adjust element widths.
	 */
	edit_encoding->SetWidth(group_tags->GetWidth() - edit_encoding->GetX() - 10);
	combo_encoding->SetWidth(group_tags->GetWidth() - combo_encoding->GetX() - 10);
	check_prependzero->SetWidth(group_tags->GetWidth() - check_prependzero->GetX() - 10);

	/* Finish initialization.
	 */
	ToggleTags();

	SetSize(group_tags->GetSize() + Size(14, 18));
}

freac::ConfigureTagFormats::~ConfigureTagFormats()
{
	DeleteObject(group_tags);
	DeleteObject(list_tag_formats);
	DeleteObject(text_encoding);
	DeleteObject(edit_encoding);
	DeleteObject(combo_encoding);
	DeleteObject(list_encodings);
	DeleteObject(check_prependzero);
}

Void freac::ConfigureTagFormats::OnEditEncoding()
{
	if (edit_encoding->IsVisible())	selected_encodings.SetNth(list_tag_formats->GetSelectedEntryNumber(), edit_encoding->GetText());
	else				selected_encodings.SetNth(list_tag_formats->GetSelectedEntryNumber(), combo_encoding->GetSelectedEntry()->GetText());
}

Void freac::ConfigureTagFormats::ToggleTags()
{
	ListEntry	*entry = list_tag_formats->GetSelectedEntry();

	if (entry == NIL)
	{
		edit_encoding->SetText(NIL);

		text_encoding->Deactivate();
		edit_encoding->Deactivate();

		check_prependzero->Deactivate();

		return;
	}

	/* Hide all controls first.
	 */
	edit_encoding->Hide();
	combo_encoding->Hide();

	list_encodings->RemoveAllEntries();
	combo_encoding->RemoveAllEntries();

	/* Configure and show controls according to format capabilities.
	 */
	Registry	&boca = Registry::Get();
	Int		 n = 0;

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_TAGGER) continue;

		const Array<TagSpec *>	&specs = boca.GetComponentTagSpecs(i);

		foreach (TagSpec *spec, specs)
		{
			if (n++ != list_tag_formats->GetSelectedEntryNumber()) continue;

			const Array<String>	&encodings = spec->GetEncodings();

			foreach (const String &encoding, encodings)
			{
				list_encodings->AddEntry(encoding);
				combo_encoding->AddEntry(encoding);
			}

			if (spec->IsFreeEncodingSupported())
			{
				edit_encoding->SetText(selected_encodings.GetNth(n - 1));
				edit_encoding->Show();
			}
			else
			{
				combo_encoding->SelectEntry(selected_encodings.GetNth(n - 1));
				combo_encoding->Show();
			}

			check_prependzero->SetChecked(tags_prependzero.GetNth(n - 1));

			if (spec->IsPrependZeroAllowed()) check_prependzero->Activate();
			else				  check_prependzero->Deactivate();

			break;
		}

		if (n > list_tag_formats->GetSelectedEntryNumber()) break;
	}

	edit_encoding->SetDropDownList(list_encodings);

	/* Activate encoding controls if more than one encoding is available.
	 */
	if (list_encodings->Length() > 1)
	{
		text_encoding->Activate();
		edit_encoding->Activate();
		combo_encoding->Activate();
	}
	else
	{
		text_encoding->Deactivate();
		edit_encoding->Deactivate();
		combo_encoding->Deactivate();
	}
}

Void freac::ConfigureTagFormats::TogglePrependZero()
{
	tags_prependzero.SetNth(list_tag_formats->GetSelectedEntryNumber(), prependZero);
}

Int freac::ConfigureTagFormats::SaveSettings()
{
	BoCA::Config	*config = BoCA::Config::Get();
	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_TAGGER) continue;

		const Array<TagSpec *>	&specs = boca.GetComponentTagSpecs(i);

		foreach (TagSpec *spec, specs)
		{
			config->SetIntValue(Config::CategoryTagsID, String("Enable").Append(spec->GetName().Replace(" ", NIL)), list_tag_formats->GetEntry(spec->GetName())->IsMarked());
			config->SetStringValue(Config::CategoryTagsID, spec->GetName().Replace(" ", NIL).Append("Encoding"), selected_encodings.GetNth(list_tag_formats->GetEntryNumber(spec->GetName())));

			if (spec->IsPrependZeroAllowed()) config->SetIntValue(Config::CategoryTagsID, String("TrackPrependZero").Append(spec->GetName().Replace(" ", NIL)), tags_prependzero.GetNth(list_tag_formats->GetEntryNumber(spec->GetName())));
		}
	}

	return Success();
}
