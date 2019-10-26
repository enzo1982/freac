 /* fre:ac - free audio converter
  * Copyright (C) 2001-2019 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/format.h>

#include <config.h>
#include <resources.h>

#ifdef __WIN32__
#	include <smooth/init.win32.h>
#endif

freac::DialogSelectFormat::DialogSelectFormat(const Array<BoCA::Format> &formats)
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Dialogs::Format");

	Rect	 workArea = S::System::Screen::GetActiveScreenWorkArea();

	mainWnd			= new Window(i18n->TranslateString("Select sample format"), workArea.GetPosition() + Point((workArea.GetSize().cx - 565) / 2, (workArea.GetSize().cy - 375) / 2), Size(450, 250));
	mainWnd->SetMinimumSize(Size(400, 220));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());
	mainWnd->GetMainLayer()->onChangeSize.Connect(&DialogSelectFormat::OnChangeSize, this);

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	button_confirm	= new Button(i18n->TranslateString("Select"), Point(87, 29), Size());
	button_confirm->onAction.Connect(&DialogSelectFormat::OnConfirm, this);
	button_confirm->SetOrientation(OR_LOWERRIGHT);
	button_confirm->SetWidth(Math::Max(80, button_confirm->GetUnscaledTextWidth() + 14));
	button_confirm->SetX(7 + button_confirm->GetWidth());

	button_cancel		= new Button(i18n->TranslateString("Cancel"), Point(175, 29), Size());
	button_cancel->onAction.Connect(&DialogSelectFormat::OnCancel, this);
	button_cancel->SetOrientation(OR_LOWERRIGHT);
	button_cancel->SetX(95 + button_confirm->GetWidth());

	Config	*freacConfig	= Config::Get();

	text_formats		= new Text(i18n->TranslateString("The selected tracks have different sample formats. Please select\nthe desired sample format of the output file:"), Point(7, 5));
	list_formats		= new ListBox(Point(7, 12 + text_formats->GetUnscaledTextHeight()), Size(378, 90));

	/* Add formats and find minimum number of channels in list.
	 */
	Int	 minChannels = 32768;

	foreach (const BoCA::Format &format, formats)
	{
		String	 formatString = String(i18n->TranslateString("%1 Hz", "Technical").Replace("%1", S::I18n::Number::GetLocalizedNumberString(format.rate))).Append(", ").
					Append(i18n->TranslateString("%1 bit", "Technical").Replace("%1", String::FromInt(format.bits))).Append(format.fp ? String(" ").Append(i18n->TranslateString("float")) : String()).Append(", ").
					Append(format.channels > 2 ? (format.channels != 4 && format.channels != 5 && format.channels <= 8 ? String::FromInt(format.channels - 1).Append(".1") : String::FromInt(format.channels)) : (format.channels == 1 ? i18n->TranslateString("Mono") : i18n->TranslateString("Stereo")));

		if (format.channels < minChannels) minChannels = format.channels;

		list_formats->AddEntry(formatString);
	}

	/* Deactivate formats that would require upmixing and select first entry.
	 */
	Bool	 first = True;

	foreach (const BoCA::Format &format, formats)
	{
		if (format.channels > 2 && format.channels > minChannels)
		{
			list_formats->GetNthEntry(foreachindex)->Deactivate();

			continue;
		}

		if (first) list_formats->SelectNthEntry(foreachindex);

		first = False;
	}

	Add(mainWnd);

	mainWnd->Add(button_confirm);
	mainWnd->Add(button_cancel);

	mainWnd->Add(text_formats);
	mainWnd->Add(list_formats);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);

	mainWnd->SetFlags(WF_MODAL);
	mainWnd->SetIcon(ImageLoader::Load(String(freacConfig->resourcesPath).Append("icons/freac.png")));

#ifdef __WIN32__
	mainWnd->SetIconDirect(LoadImageA(hInstance, MAKEINTRESOURCEA(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
#endif
}

freac::DialogSelectFormat::~DialogSelectFormat()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);

	DeleteObject(text_formats);
	DeleteObject(list_formats);

	DeleteObject(button_confirm);
	DeleteObject(button_cancel);
}

const Error &freac::DialogSelectFormat::ShowDialog()
{
	error = Error();

	mainWnd->WaitUntilClosed();

	return error;
}

Int freac::DialogSelectFormat::GetSelectedEntryNumber() const
{
	return list_formats->GetSelectedEntryNumber();
}

Void freac::DialogSelectFormat::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(mainWnd->GetMainLayer()->GetPosition(), mainWnd->GetMainLayer()->GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	list_formats->SetSize(clientSize - Size(14, 59 + text_formats->GetUnscaledTextHeight()));
}

Void freac::DialogSelectFormat::OnConfirm()
{
	error = Success();

	mainWnd->Close();
}

Void freac::DialogSelectFormat::OnCancel()
{
	mainWnd->Close();
}
