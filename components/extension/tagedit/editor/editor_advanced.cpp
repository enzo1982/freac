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

#include "editor_advanced.h"
#include "editor_advanced_text.h"
#include "editor_advanced_number.h"

BoCA::LayerTagAdvanced::LayerTagAdvanced() : Editor("Advanced")
{
	LoadTagTypes();

	text_type	= new Text("Show fields for:", Point(7, 10));
	combo_type	= new ComboBox(Point(text_type->GetUnscaledTextWidth() + 14, 7), Size(220, 0));
	combo_type->onSelectEntry.Connect(&LayerTagAdvanced::OnSelectTagType, this);

	for (Int i = 0; i < tagTypes.Length(); i++)
	{
		combo_type->AddEntry(tagTypes.GetNth(i)->name);
	}

	list_fields	= new ListBox(Point(7, 34), Size(text_type->GetUnscaledTextWidth() + 227, 0));
	list_fields->onSelectEntry.Connect(&LayerTagAdvanced::OnSelectField, this);
	list_fields->AddTab("Name", 120);
	list_fields->AddTab("ID", 50);
	list_fields->AddTab("Value");

	/* This will later be assigned the editing
	 * layer for the selected tag field type.
	 */
	layer_field	= NIL;

	combo_type->SelectEntry("ID3v2");

	Add(text_type);
	Add(combo_type);

	Add(list_fields);

	/* Initially deactivate all input fields.
	 */
	OnSelectNone();

	onChangeSize.Connect(&LayerTagAdvanced::OnChangeSize, this);
}

BoCA::LayerTagAdvanced::~LayerTagAdvanced()
{
	if (layer_field != NIL)
	{
		DeleteObject(layer_field);

		layer_field = NIL;
	}

	DeleteObject(text_type);
	DeleteObject(combo_type);

	DeleteObject(list_fields);

	FreeTagTypes();
}

Void BoCA::LayerTagAdvanced::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	list_fields->SetHeight(clientSize.cy - 42);
}

/* Called when a track is selected from the list.
 * ----
 */
Void BoCA::LayerTagAdvanced::OnSelectTrack(const Track &track)
{
	list_fields->Activate();
}

/* Called when the last track is removed from the list.
 * ----
 */
Void BoCA::LayerTagAdvanced::OnSelectNone()
{
	list_fields->Deactivate();
}

Void BoCA::LayerTagAdvanced::OnSelectTagType()
{
	list_fields->RemoveAllEntries();

	if (layer_field != NIL)
	{
		Remove(layer_field);
		DeleteObject(layer_field);

		layer_field = NIL;
	}

	Int	 n = combo_type->GetSelectedEntryNumber();
	TagSpec	*tag = tagTypes.GetNth(n);

	foreach (const TagField &field, tag->fields)
	{
		list_fields->AddEntry(String(field.name).Append(ListEntry::tabDelimiter).Append(field.techId).Append(ListEntry::tabDelimiter).Append(""));
	}
}

Void BoCA::LayerTagAdvanced::OnSelectField()
{
	Int		 n     = list_fields->GetSelectedEntryNumber();
	TagSpec		*tag   = tagTypes.GetNth(combo_type->GetSelectedEntryNumber());
	const TagField	&field = tag->fields.GetNth(n);

	if (layer_field != NIL)
	{
		Remove(layer_field);
		DeleteObject(layer_field);

		layer_field = NIL;
	}

	switch (field.type)
	{
		case FIELD_TYPE_UNKNOWN:
			break;
		case FIELD_TYPE_TEXT:
			layer_field = new LayerEditText();

			break;
		case FIELD_TYPE_INTEGER:
			layer_field = new LayerEditNumber();

			break;
		case FIELD_TYPE_DATA:
			break;
		case FIELD_TYPE_GENRE:
			break;
		case FIELD_TYPE_PICTURE:
			break;
	}

	if (layer_field != NIL)
	{
		layer_field->SetPosition(Point(307, 27));

		Add(layer_field);
	}
}

Int BoCA::LayerTagAdvanced::LoadTagTypes()
{
	Directory		 dir(Utilities::GetBoCADirectory().Append("freac.extension.tagedit"));

	if (!dir.Exists()) dir = Utilities::GetBoCADirectory().Append("../freac/freac.extension.tagedit");

	const Array<File>	&files = dir.GetFilesByPattern("tag_*.xml");

	for (Int i = 0; i < files.Length(); i++)
	{
		TagSpec	*type = new TagSpec();

		type->LoadFromXML(files.GetNth(i));

		tagTypes.Add(type);
	}

	return Success();
}

Int BoCA::LayerTagAdvanced::FreeTagTypes()
{
	foreach (TagSpec *tagType, tagTypes) delete tagType;

	tagTypes.RemoveAll();

	return Success();
}
