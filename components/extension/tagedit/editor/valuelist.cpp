 /* fre:ac - free audio converter
  * Copyright (C) 2026 Shen-Ta Hsieh
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "valuelist.h"

/* A proposed value, for example a drop-down selection, is added to the
 * values unless they have it already. ShowDialog applies it.
 */
BoCA::ValueListDialog::ValueListDialog(const String &title, const Array<String> &nValues, const String &proposedValue)
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Tag Editor::Value list");

	values	 = nValues;
	proposed = False;

	if (proposedValue.Trim() != NIL)
	{
		proposed = True;

		foreach (const String &value, values) if (value == proposedValue.Trim()) proposed = False;

		if (proposed) values.Add(proposedValue.Trim());
	}

	Rect	 monitor = S::System::Screen::GetActiveScreenWorkArea();
	Size	 size	 = Size(420, 300);

	window		= new GUI::Window(title, Point(monitor.left, monitor.top) + Point((monitor.GetSize().cx - size.cx) / 2, (monitor.GetSize().cy - size.cy) / 2), size);
	titlebar	= new Titlebar(TB_CLOSEBUTTON);

	list_values	= new ListBox(Point(7, 7), Size(300, 200));
	list_values->onSelectEntry.Connect(&ValueListDialog::OnSelectValue, this);

	edit_value	= new EditBox(Point(7, 214), Size(300, 0));
	edit_value->onInput.Connect(&ValueListDialog::OnEditValue, this);

	button_add	= new Button(i18n->TranslateString("Add"),	Point(314, 7),	 Size(90, 0));
	button_remove	= new Button(i18n->TranslateString("Remove"),	Point(314, 36),	 Size(90, 0));
	button_up	= new Button(i18n->TranslateString("Move up"),	Point(314, 72),	 Size(90, 0));
	button_down	= new Button(i18n->TranslateString("Move down"), Point(314, 101), Size(90, 0));
	button_close	= new Button(i18n->TranslateString("Close"),	Point(314, 214), Size(90, 0));

	button_add->onAction.Connect(&ValueListDialog::OnAdd, this);
	button_remove->onAction.Connect(&ValueListDialog::OnRemove, this);
	button_up->onAction.Connect(&ValueListDialog::OnUp, this);
	button_down->onAction.Connect(&ValueListDialog::OnDown, this);
	button_close->onAction.Connect(&ValueListDialog::OnClose, this);

	Add(window);

	window->Add(titlebar);
	window->Add(list_values);
	window->Add(edit_value);
	window->Add(button_add);
	window->Add(button_remove);
	window->Add(button_up);
	window->Add(button_down);
	window->Add(button_close);

	window->SetFlags(window->GetFlags() | WF_NOTASKBUTTON | WF_MODAL);

	UpdateList(proposed ? values.Length() - 1 : 0);
}

BoCA::ValueListDialog::~ValueListDialog()
{
	DeleteObject(window);
	DeleteObject(titlebar);

	DeleteObject(list_values);
	DeleteObject(edit_value);

	DeleteObject(button_add);
	DeleteObject(button_remove);
	DeleteObject(button_up);
	DeleteObject(button_down);
	DeleteObject(button_close);
}

const Error &BoCA::ValueListDialog::ShowDialog()
{
	if (proposed) Changed();

	window->WaitUntilClosed();

	return error;
}

Void BoCA::ValueListDialog::UpdateList(Int selected)
{
	list_values->RemoveAllEntries();

	foreach (const String &value, values) list_values->AddEntry(value);

	if (values.Length() > 0) list_values->SelectNthEntry(Math::Max(0, Math::Min(selected, values.Length() - 1)));

	OnSelectValue();
}

/* Apply each change at once. Values are trimmed and empty values dropped.
 */
Void BoCA::ValueListDialog::Changed()
{
	Array<String>	 nonEmpty;

	foreach (const String &value, values) if (value.Trim() != NIL) nonEmpty.Add(value.Trim());

	onChangeValues.Emit(nonEmpty);
}

Void BoCA::ValueListDialog::OnSelectValue()
{
	Int	 n = list_values->GetSelectedEntryNumber();

	if (n >= 0) { edit_value->Activate();	edit_value->SetText(values.GetNth(n)); }
	else	    { edit_value->Deactivate(); edit_value->SetText(NIL);		 }
}

Void BoCA::ValueListDialog::OnEditValue(const String &text)
{
	Int	 n = list_values->GetSelectedEntryNumber();

	if (n < 0) return;

	values.SetNth(n, text);
	list_values->GetNthEntry(n)->SetText(text);

	Changed();
}

Void BoCA::ValueListDialog::OnAdd()
{
	values.Add(String());

	UpdateList(values.Length() - 1);

	edit_value->SetFocus();
}

Void BoCA::ValueListDialog::OnRemove()
{
	Int	 n = list_values->GetSelectedEntryNumber();

	if (n < 0) return;

	values.RemoveNth(n);

	UpdateList(n);
	Changed();
}

Void BoCA::ValueListDialog::OnUp()
{
	Int	 n = list_values->GetSelectedEntryNumber();

	if (n <= 0) return;

	String	 value = values.GetNth(n);

	values.SetNth(n, values.GetNth(n - 1));
	values.SetNth(n - 1, value);

	UpdateList(n - 1);
	Changed();
}

Void BoCA::ValueListDialog::OnDown()
{
	Int	 n = list_values->GetSelectedEntryNumber();

	if (n < 0 || n >= values.Length() - 1) return;

	String	 value = values.GetNth(n);

	values.SetNth(n, values.GetNth(n + 1));
	values.SetNth(n + 1, value);

	UpdateList(n + 1);
	Changed();
}

Void BoCA::ValueListDialog::OnClose()
{
	window->Close();
}
