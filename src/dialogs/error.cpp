 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/error.h>

#include <config.h>
#include <resources.h>

#include <boca.h>

#ifdef __WIN32__
#	include <smooth/init.win32.h>
#endif

freac::ErrorDialog::ErrorDialog(const Array<String> &errors)
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Dialogs::Errors");

	Rect	 workArea = S::System::Screen::GetActiveScreenWorkArea();

	mainWnd			= new Window(i18n->TranslateString("Errors"), workArea.GetPosition() + Point((workArea.GetSize().cx - 450) / 2, (workArea.GetSize().cy - 400) / 2), Size(450, 400));
	mainWnd->SetMinimumSize(Size(330, 220));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());
	mainWnd->GetMainLayer()->onChangeSize.Connect(&ErrorDialog::OnChangeSize, this);

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	btn_ok			= new Button(i18n->TranslateString("OK"), NIL, Point(87, 29), Size());
	btn_ok->onAction.Connect(&ErrorDialog::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	text_errors	= new Text(i18n->AddColon(i18n->TranslateString("%1 errors while processing job").Replace("%1", String::FromInt(errors.Length()))), Point(7, 5));

	list_errors	= new ListBox(Point(7, 24), Size(378, 180));
	list_errors->onSelectEntry.Connect(&ErrorDialog::OnSelectError, this);

	foreach (const String &error, errors) list_errors->AddEntry(error);

	edit_details	= new MultiEdit(NIL, Point(7, 104), Size(378, 57));
	edit_details->SetOrientation(OR_LOWERLEFT);
	edit_details->Deactivate();

	text_details	= new Text(i18n->TranslateString("select an entry for details"), Point());
	text_details->SetFont(GUI::Font(GUI::Font::Default, 12, GUI::Font::Bold, 0, Setup::InactiveTextColor));
	text_details->SetOrientation(OR_LOWERLEFT);

	Add(mainWnd);

	mainWnd->Add(btn_ok);

	mainWnd->Add(text_errors);
	mainWnd->Add(list_errors);

	mainWnd->Add(edit_details);
	mainWnd->Add(text_details);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);

	mainWnd->SetFlags(WF_MODAL);
	mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));

#ifdef __WIN32__
	mainWnd->SetIconDirect(LoadImageA(hInstance, MAKEINTRESOURCEA(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
#endif
}

freac::ErrorDialog::~ErrorDialog()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);

	DeleteObject(text_errors);
	DeleteObject(list_errors);

	DeleteObject(edit_details);
	DeleteObject(text_details);

	DeleteObject(btn_ok);
}

const Error &freac::ErrorDialog::ShowDialog()
{
	mainWnd->WaitUntilClosed();

	return error;
}

Void freac::ErrorDialog::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(mainWnd->GetMainLayer()->GetPosition(), mainWnd->GetMainLayer()->GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	list_errors->SetSize(clientSize - Size(14, 136));

	edit_details->SetWidth(clientSize.cx - 14);
	text_details->SetPosition(edit_details->GetPosition() + Point((edit_details->GetWidth() - text_details->GetUnscaledTextWidth()) / 2, (-(edit_details->GetHeight() - text_details->GetUnscaledTextHeight())) / 2));
}

Void freac::ErrorDialog::OK()
{
	mainWnd->Close();
}

Void freac::ErrorDialog::OnSelectError(ListEntry *error)
{
	text_details->Hide();
	edit_details->SetText(error->GetText());
}
