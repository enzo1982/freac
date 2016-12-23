 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "dialog.h"

/* Custom text widget to draw without erasing the background.
 */
class TransparentText : public Text
{
	public:
		TransparentText(const String &iText, const Point &iPos) : Text(iText, iPos)
		{
		}

		Int Paint(Int message)
		{
			if (!IsRegistered()) return Error();
			if (!IsVisible())    return Success();

			if (text != NIL && message == SP_PAINT)
			{
				Rect	 rect	 = Rect(GetRealPosition(), GetRealSize());
				Surface	*surface = GetDrawSurface();

				surface->SetText(text, Rect::OverlapRect(Rect(rect.GetPosition(), Size(scaledTextSize.cx, Math::Round(scaledTextSize.cy * 1.2))), GetVisibleArea()), font);
			}

			return Success();
		}
};

BoCA::LanguageNotifierDialog::LanguageNotifierDialog(Bool incomplete)
{
	Config		*config = Config::Get();

	mainWnd			= new GUI::Window("Help translate this project", Point(100, 100), Size(380, 350));
	mainWnd_titlebar	= new Titlebar(TB_NONE);
	divider			= new Divider(39, OR_HORZ | OR_BOTTOM);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divider);

	button_close		= new Button("Close", NIL, Point(87, 29), Size());
	button_close->onAction.Connect(&LanguageNotifierDialog::Close, this);
	button_close->SetOrientation(OR_LOWERRIGHT);

	mainWnd->Add(button_close);

	Bitmap	 bitmap = ImageLoader::Load(File(Utilities::GetBoCADirectory().Append("../freac/freac.extension.langnotify/langnotify.png")));

	bitmap.SetBackgroundColor(Setup::BackgroundColor);

	image_background	= new Image(bitmap, Point(0, 0), Size(192, 192));

	mainWnd->Add(image_background);

#ifdef __APPLE__
	String	 location = "freac.app/Contents/Resources";
#else
	String	 location = "the fre:ac directory";
#endif

#ifndef __WIN32__
	if (Directory(S::System::System::GetResourcesDirectory().Append("freac")).Exists()) location = Directory(S::System::System::GetResourcesDirectory().Append("freac"));
#endif

	if (!incomplete) text_info = new TransparentText(String("Your native language does not seem to be supported by fre:ac yet.\n\nDid you know you can easily create your own translation and share\nit with other users?\n\nPlease have a look at the Readme.lang file in the lang subfolder\nof ").Append(location).Append(" for instructions on how to create a\ntranslation and submit it to the fre:ac project.\n\nIf you have any questions, please contact translate@freac.org."), Point(7, 45));
	else		 text_info = new TransparentText(String("The translation for your native language seems to be incomplete.\n\nDid you know you can easily complete it and share the result with\nother users?\n\nPlease have a look at the Readme.lang file in the lang subfolder\nof ").Append(location).Append(" for instructions on how to edit a\ntranslation and submit it to the fre:ac project.\n\nIf you have any questions, please contact translate@freac.org."), Point(7, 45));

	text_intro		= new TransparentText("Please help translate this project!", Point(7, 12));
	text_intro->SetFont(Font(Font::Default, 12, Font::Bold));
	text_intro->SetX((Math::Max(text_intro->GetUnscaledTextWidth(), text_info->GetUnscaledTextWidth()) + 9 - text_intro->GetUnscaledTextWidth()) / 2);

	text_thanks		= new TransparentText("Thank you very much!", Point(7, text_info->GetUnscaledTextHeight() + 58));
	text_thanks->SetFont(Font(Font::Default, 12, Font::Bold));
	text_thanks->SetX((Math::Max(text_intro->GetUnscaledTextWidth(), text_info->GetUnscaledTextWidth()) + 9 - text_thanks->GetUnscaledTextWidth()) / 2);

	image_background->SetPosition(Point((Math::Max(text_intro->GetUnscaledTextWidth(), text_info->GetUnscaledTextWidth()) - 178) / 2, (text_info->GetUnscaledTextHeight() - 94) / 2));

	mainWnd->Add(text_intro);
	mainWnd->Add(text_info);
	mainWnd->Add(text_thanks);

	remind = config->GetIntValue("LanguageNotifier", "ShowAgain", True);

	check_remind		= new CheckBox("Remind me later", Point(7, 27), Size(), &remind);
	check_remind->SetOrientation(OR_LOWERLEFT);
	check_remind->SetWidth(check_remind->GetUnscaledTextWidth() + 23);

	mainWnd->Add(check_remind);

	Add(mainWnd);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_TOPMOST | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("icons/freac.png"));

#ifndef __WIN32__
	if (Directory(S::System::System::GetResourcesDirectory().Append("freac")).Exists()) mainWnd->SetIcon(ImageLoader::Load(S::System::System::GetResourcesDirectory().Append("freac/icons/freac.png")));
#endif

	Rect	 workArea = Screen::GetActiveScreenWorkArea();

	Size	 wndSize  = Size(Math::Max(text_intro->GetUnscaledTextWidth(), text_info->GetUnscaledTextWidth()) + 21, text_info->GetUnscaledTextHeight() + 165);
	Point	 wndPos	  = workArea.GetPosition() + Point((workArea.GetSize().cx - wndSize.cx) / 2, (workArea.GetSize().cy - wndSize.cy) / 2);

	mainWnd->SetMetrics(wndPos, wndSize);
}

BoCA::LanguageNotifierDialog::~LanguageNotifierDialog()
{
	DeleteObject(mainWnd);
	DeleteObject(mainWnd_titlebar);

	DeleteObject(divider);

	DeleteObject(button_close);

	DeleteObject(text_intro);
	DeleteObject(text_info);
	DeleteObject(text_thanks);

	DeleteObject(image_background);

	DeleteObject(check_remind);
}

const Error &BoCA::LanguageNotifierDialog::ShowDialog()
{
	mainWnd->Show();

	return error;
}

Void BoCA::LanguageNotifierDialog::Close()
{
	Config	*config = Config::Get();

	mainWnd->Close();

	config->SetIntValue("LanguageNotifier", "ShowAgain", remind);
}
