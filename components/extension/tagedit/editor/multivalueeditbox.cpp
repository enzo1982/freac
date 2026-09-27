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

#include "multivalueeditbox.h"

BoCA::MultiValueEditBox::MultiValueEditBox(const String &iKey, Text *iLabel, const Point &iPos, const Size &iSize) : EditBox(iPos, iSize)
{
	key   = iKey;
	label = iLabel;

	cursor->getContextMenu.Connect(&MultiValueEditBox::GetMenu, this);

	onLeftButtonDoubleClick.Connect(&MultiValueEditBox::OnDoubleClick, this);
	onInput.Connect(&MultiValueEditBox::OnInputText, this);
	onSelectEntry.Connect(&MultiValueEditBox::OnSelectDropDownEntry, this);
}

BoCA::MultiValueEditBox::~MultiValueEditBox()
{
}

/* Show values and switch between editable and read-only.
 */
Void BoCA::MultiValueEditBox::SetValues(const Array<String> &nValues)
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Tag Editor::Value list");

	values = nValues;

	if (IsReadOnly())
	{
		SetText(GetDisplayText());
		SetTooltipText(i18n->TranslateString("Double-click to edit values"));

		cursor->Deactivate();
	}
	else
	{
		/* Activate the cursor first, so that the text is the first
		 * entry of its undo history.
		 */
		cursor->Activate();

		SetText(values.Length() == 1 ? values.GetFirst() : String());
		SetTooltipText(NIL);
	}
}

/* Keep the value list in step with typing in an editable box.
 */
Void BoCA::MultiValueEditBox::OnInputText(const String &text)
{
	if (IsReadOnly()) return;

	values.RemoveAll();

	if (text != NIL) values.Add(text);
}

/* A typing attempt in an active read-only box requests the value list.
 */
Int BoCA::MultiValueEditBox::Process(Int message, Int wParam, Int lParam)
{
	if (IsActive() && IsReadOnly() && IsFocussed())
	{
		Bool	 printable = (message == SM_CHAR && wParam >= 0x20 && wParam != 0x7F);
		Bool	 deleting  = (message == SM_KEYDOWN && (wParam == Input::Keyboard::KeyBack || wParam == Input::Keyboard::KeyDelete));

#ifdef __APPLE__
		/* The Cocoa backend sends SM_CHAR only to active cursors, so
		 * use the key down message of letter and digit keys instead.
		 */
		Bool	 shortcut  = (Input::Keyboard::GetKeyState(Input::Keyboard::KeyCommand) || Input::Keyboard::GetKeyState(Input::Keyboard::KeyControl));

		printable = printable || (message == SM_KEYDOWN && !shortcut && ((wParam >= Input::Keyboard::Key0 && wParam <= Input::Keyboard::Key9) ||
										 (wParam >= Input::Keyboard::KeyA && wParam <= Input::Keyboard::KeyZ)));
#endif

		if (printable || deleting)
		{
			onRequestValueList.Emit(this);

			return MessageProcessed;
		}
	}

	return EditBox::Process(message, wParam, lParam);
}

Void BoCA::MultiValueEditBox::OnDoubleClick(const Point &)
{
	if (IsActive() && IsReadOnly()) onRequestValueList.Emit(this);
}

/* A drop-down selection sets the text directly, bypassing our read-only
 * display; restore the joined value list so several values are not lost,
 * and open the value list with the selection as a proposed new value.
 */
Void BoCA::MultiValueEditBox::OnSelectDropDownEntry(ListEntry *entry)
{
	if (!IsReadOnly()) return;

	proposedValue = (entry != NIL ? entry->GetText() : String());

	SetText(GetDisplayText());

	if (IsActive()) onRequestValueList.Emit(this);

	proposedValue = NIL;
}

/* The text of the label without its colon, as a title for the value list.
 */
String BoCA::MultiValueEditBox::GetLabelText() const
{
	if (label == NIL) return NIL;

	String	 name = label->GetText();

	if (name.EndsWith(":")) name = name.Head(name.Length() - 1);

	return name.Trim();
}

Void BoCA::MultiValueEditBox::OnEditValues()
{
	if (IsActive()) onRequestValueList.Emit(this);
}

/* Standard edit menu plus "Edit values...". An inactive box
 * cannot be edited, so it does not offer the value list.
 */
PopupMenu *BoCA::MultiValueEditBox::GetMenu()
{
	I18n		*i18n = I18n::Get();
	PopupMenu	*menu = cursor->GetContextMenu();

	/* The cursor has no menu if it is inactive and empty.
	 */
	if (menu == NIL || !IsActive()) return menu;

	i18n->SetContext("Extensions::Tag Editor::Value list");

	menu->AddEntry();
	menu->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Edit values")))->onAction.Connect(&MultiValueEditBox::OnEditValues, this);

	return menu;
}
