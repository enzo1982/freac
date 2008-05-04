 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <gui/main_tags.h>

BonkEnc::LayerTags::LayerTags() : Layer("Tags")
{
	LoadTagTypes();

	Point	 pos;
	Size	 size;

	pos.x	= 7;
	pos.y	= 220;

	text_type	= new Text("Show fields for:", pos);

	pos.x	+= text_type->textSize.cx + 7;
	pos.y	-= 3;
	size.cx	= 220;

	combo_type	= new ComboBox(pos, size);
	combo_type->onSelectEntry.Connect(&LayerTags::OnSelectTagType, this);

	for (Int i = 0; i < tagTypes.Length(); i++)
	{
		combo_type->AddEntry(tagTypes.GetNth(i)->name);
	}

	pos.x	= 7;
	pos.y	+= 27;
	size.cx	= text_type->textSize.cx + 227;

	list_fields	= new ListBox(pos, size);
	list_fields->AddTab("Name", 120);
	list_fields->AddTab("ID", 50);
	list_fields->AddTab("Value");

	combo_type->SelectEntry("ID3v2");

	Add(text_type);
	Add(combo_type);

	Add(list_fields);

	onChangeSize.Connect(&LayerTags::OnChangeSize, this);
}

BonkEnc::LayerTags::~LayerTags()
{
	DeleteObject(text_type);
	DeleteObject(combo_type);

	DeleteObject(list_fields);

	FreeTagTypes();
}

Void BonkEnc::LayerTags::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	list_fields->SetHeight(clientSize.cy - 252);
}

Void BonkEnc::LayerTags::OnSelectTagType()
{
	list_fields->RemoveAllEntries();

	Int	 n = combo_type->GetSelectedEntryNumber();
	TagSpec	*tag = tagTypes.GetNth(n);

	foreach (TagField field, tag->fields)
	{
		list_fields->AddEntry(String(field.name).Append("\t").Append(field.techId).Append("\t").Append(""));
	}
}

Int BonkEnc::LayerTags::LoadTagTypes()
{
	Directory		 dir(GUI::Application::GetApplicationDirectory().Append("tagging"));

	const Array<File>	&files = dir.GetFilesByPattern("tag_*.xml");

	for (Int i = 0; i < files.Length(); i++)
	{
		TagSpec	*type = new TagSpec();

		type->LoadFromXML(files.GetNth(i));

		tagTypes.Add(type);
	}

	return Success();
}

Int BonkEnc::LayerTags::FreeTagTypes()
{
	for (Int i = 0; i < tagTypes.Length(); i++)
	{
		delete tagTypes.GetNth(i);
	}

	tagTypes.RemoveAll();

	return Success();
}

Int BonkEnc::TagSpec::LoadFromXML(const String &fileName)
{
	XML::Document	*document = new XML::Document();

	document->LoadFile(fileName);

	XML::Node	*root = document->GetRootNode();

	for (Int i = 0; i < root->GetNOfNodes(); i++)
	{
		XML::Node	*node = root->GetNthNode(i);

		if	(node->GetName() == "name")	name	= node->GetContent();
		else if (node->GetName() == "fields")
		{
			for (Int j = 0; j < node->GetNOfNodes(); j++)
			{
				XML::Node	*node2 = node->GetNthNode(j);

				if (node2->GetName() != "field") continue;

				String		 typeString = node2->GetAttributeByName("type")->GetContent();
				Int		 type = FIELD_TYPE_UNKNOWN;

				if	(typeString == "text")	  type = FIELD_TYPE_TEXT;
				else if	(typeString == "integer") type = FIELD_TYPE_INTEGER;
				else if	(typeString == "data")	  type = FIELD_TYPE_DATA;
				else if	(typeString == "genre")	  type = FIELD_TYPE_GENRE;
				else if	(typeString == "picture") type = FIELD_TYPE_PICTURE;

				fields.Add(TagField(node2->GetAttributeByName("name")->GetContent(), type, node2->GetContent()));
			}
		}
	}

	delete document;

	return Success();
}
