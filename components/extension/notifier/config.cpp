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

#include "config.h"

using namespace BoCA::AS;

using namespace smooth::IO;
using namespace smooth::GUI::Dialogs;

const String	 BoCA::ConfigureNotifier::ConfigID = "Notifier";

const String	 BoCA::ConfigureNotifier::ResourcesFolder = "freac.extension.notifier";
const String	 BoCA::ConfigureNotifier::DefaultMessage  = "Conversion process finished!";

BoCA::ConfigureNotifier::ConfigureNotifier()
{
	/* Get configuration.
	 */
	const Config	*config = Config::Get();

	enableNotifications	= config->GetIntValue(ConfigID, "EnableNotifications", False);

	playSound		= config->GetIntValue(ConfigID, "PlaySound", True);
	displayMessage		= config->GetIntValue(ConfigID, "DisplayMessage", False);
	minimumDuration		= config->GetIntValue(ConfigID, "MinimumDuration", False);

	String	 soundfile	= config->GetStringValue(ConfigID, "SoundFile", String("<sounds>/finished.flac").Replace("/", Directory::GetDirectoryDelimiter()));
	String	 message	= config->GetStringValue(ConfigID, "Message", DefaultMessage);

	Int	 seconds	= config->GetIntValue(ConfigID, "Seconds", 30);

	/* Create UI elements.
	 */
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Notifier::Configuration");

	group_notify	= new GroupBox(i18n->TranslateString("Notifications"), Point(7, 11), Size(450, 141));

	check_notify	= new CheckBox(i18n->TranslateString("Notify on finished conversion"), Point(10, 13), Size(group_notify->GetWidth() - 20, 0), &enableNotifications);
	check_notify->onAction.Connect(&ConfigureNotifier::OnToggleNotifications, this);

	/* Play a sound.
	 */
	check_sound	= new CheckBox(i18n->AddColon(i18n->TranslateString("Play sound")), Point(27, 40), Size(100, 0), &playSound);
	check_sound->onAction.Connect(&ConfigureNotifier::OnToggleNotifications, this);

	check_message	= new CheckBox(i18n->AddColon(i18n->TranslateString("Display message")), Point(27, 86), Size(100, 0), &displayMessage);
	check_message->onAction.Connect(&ConfigureNotifier::OnToggleNotifications, this);

	Int	 maxTextSize = Math::Max(check_sound->GetUnscaledTextWidth(), check_message->GetUnscaledTextWidth());

	check_sound->SetWidth(maxTextSize + 21);
	check_message->SetWidth(maxTextSize + 21);

	list_soundfile	= new List();
	list_soundfile->AddEntry(String("<sounds>/finished.flac").Replace("/", Directory::GetDirectoryDelimiter()));

	edit_soundfile	= new EditBox(soundfile, Point(check_sound->GetWidth() + 35, 39), Size(group_notify->GetWidth() - check_sound->GetWidth() - 133, 0));
	edit_soundfile->SetDropDownList(list_soundfile);

	btn_browse	= new Button(i18n->TranslateString("Select"), Point(90, 38), Size());
	btn_browse->SetOrientation(OR_UPPERRIGHT);
	btn_browse->onAction.Connect(&ConfigureNotifier::OnBrowse, this);

	Directory	 dir(Utilities::GetBoCADirectory().Append(ResourcesFolder));

	if (!dir.Exists()) dir = Utilities::GetBoCADirectory().Append("../freac/").Append(ResourcesFolder);

	btn_preview	= new Button(ImageLoader::Load(File(String(dir).Append("/icons/play.png"))), Point(edit_soundfile->GetX() + edit_soundfile->GetWidth() - 43 + (i18n->IsActiveLanguageRightToLeft() ? 22 : 0), 59), Size(25, 25));
	btn_preview->onAction.Connect(&ConfigureNotifier::OnPreview, this);
	btn_preview->SetFlags(BF_NOFRAME);

	btn_stop	= new Button(ImageLoader::Load(File(String(dir).Append("/icons/stop.png"))), Point(edit_soundfile->GetX() + edit_soundfile->GetWidth() - 21 - (i18n->IsActiveLanguageRightToLeft() ? 22 : 0), 59), Size(25, 25));
	btn_stop->onAction.Connect(&ConfigureNotifier::OnStop, this);
	btn_stop->SetFlags(BF_NOFRAME);

	Registry	&boca = Registry::Get();

	if (boca.GetNumberOfComponentsOfType(COMPONENT_TYPE_OUTPUT) == 0)
	{
		check_sound->Deactivate();
		check_message->Deactivate();
		list_soundfile->Deactivate();
		edit_soundfile->Deactivate();
		btn_browse->Deactivate();
		btn_preview->Deactivate();
		btn_stop->Deactivate();
	}

	/* Display a message.
	 */
	edit_message	= new EditBox(i18n->TranslateString(message), Point(check_message->GetWidth() + 35, 85), Size(group_notify->GetWidth() - check_message->GetWidth() - 45, 0));

	/* Long running conversions only.
	 */
	Int	 textWidth = Math::Max(Font().GetUnscaledTextSizeX(i18n->TranslateString("second(s)")), Font().GetUnscaledTextSizeX(i18n->TranslateString("minute(s)")));

	combo_units	= new ComboBox(Point(textWidth + 35, 112), Size(textWidth + 25, 0));
	combo_units->SetOrientation(OR_UPPERRIGHT);
	combo_units->AddEntry(i18n->TranslateString("second(s)"));
	combo_units->AddEntry(i18n->TranslateString("minute(s)"));

	if (seconds % 60 != 0) combo_units->SelectNthEntry(0);
	else		       combo_units->SelectNthEntry(1);

	list_amount	= new List();
	list_amount->AddEntry("1");
	list_amount->AddEntry("2");
	list_amount->AddEntry("3");
	list_amount->AddEntry("4");
	list_amount->AddEntry("5");
	list_amount->AddEntry("10");
	list_amount->AddEntry("15");
	list_amount->AddEntry("30");
	list_amount->AddEntry("45");
	list_amount->AddEntry("60");

	Int	 numberWidth = Font().GetUnscaledTextSizeX("00");

	edit_amount	= new EditBox(String::FromInt(seconds / (combo_units->GetSelectedEntryNumber() == 0 ? 1 : 60)), Point(combo_units->GetX() + numberWidth + 33, 112), Size(numberWidth + 25, 0));
	edit_amount->SetOrientation(OR_UPPERRIGHT);
	edit_amount->SetFlags(EDB_NUMERIC);
 	edit_amount->SetDropDownList(list_amount);

	check_time	= new CheckBox(i18n->AddColon(i18n->TranslateString("Only for conversions running longer than")), Point(27, 113), Size(group_notify->GetWidth() - edit_amount->GetX() - 35, 0), &minimumDuration);
	check_time->onAction.Connect(&ConfigureNotifier::OnToggleNotifications, this);

	/* Add elements.
	 */
	group_notify->Add(check_notify);

	group_notify->Add(check_sound);
	group_notify->Add(edit_soundfile);

	group_notify->Add(btn_browse);

	group_notify->Add(btn_preview);
	group_notify->Add(btn_stop);

	group_notify->Add(check_message);
	group_notify->Add(edit_message);

	group_notify->Add(check_time);
	group_notify->Add(edit_amount);
	group_notify->Add(combo_units);

	Add(group_notify);

	OnToggleNotifications();

	SetSize(group_notify->GetSize() + Size(14, 16));
}

BoCA::ConfigureNotifier::~ConfigureNotifier()
{
	DeleteObject(group_notify);
	DeleteObject(check_notify);

	DeleteObject(check_sound);
	DeleteObject(edit_soundfile);
	DeleteObject(list_soundfile);

	DeleteObject(btn_browse);

	DeleteObject(btn_preview);
	DeleteObject(btn_stop);

	DeleteObject(check_message);
	DeleteObject(edit_message);

	DeleteObject(check_time);
	DeleteObject(edit_amount);
	DeleteObject(list_amount);
	DeleteObject(combo_units);

}

Void BoCA::ConfigureNotifier::OnToggleNotifications()
{
	if (enableNotifications)
	{
		Registry	&boca = Registry::Get();

		if (boca.GetNumberOfComponentsOfType(COMPONENT_TYPE_OUTPUT) > 0) check_sound->Activate();

		check_message->Activate();
		check_time->Activate();

		/* Play a sound.
		 */
		if (playSound && check_sound->IsActive())
		{
			edit_soundfile->Activate();

			btn_browse->Activate();

			btn_preview->Activate();
			btn_stop->Activate();
		}
		else
		{
			edit_soundfile->Deactivate();

			btn_browse->Deactivate();

			btn_preview->Deactivate();
			btn_stop->Deactivate();
		}

		/* Display a message.
		 */
		if (displayMessage)
		{
			edit_message->Activate();
		}
		else
		{
			edit_message->Deactivate();
		}

		/* Long running conversions only.
		 */
		if (minimumDuration)
		{
			edit_amount->Activate();
			combo_units->Activate();
		}
		else
		{
			edit_amount->Deactivate();
			combo_units->Deactivate();
		}
	}
	else
	{
		check_sound->Deactivate();
		edit_soundfile->Deactivate();

		btn_browse->Deactivate();

		btn_preview->Deactivate();
		btn_stop->Deactivate();

		check_message->Deactivate();
		edit_message->Deactivate();

		check_time->Deactivate();
		edit_amount->Deactivate();
		combo_units->Deactivate();
	}
}

Void BoCA::ConfigureNotifier::OnBrowse()
{
	/* Create dialog.
	 */
	FileSelection	 dialog;
	Directory	 dir(Utilities::GetBoCADirectory().Append(ResourcesFolder).Append("/sounds"));

	if (!dir.Exists()) dir = Utilities::GetBoCADirectory().Append("../freac/").Append(ResourcesFolder).Append("/sounds");

	dialog.SetParentWindow(container->GetContainerWindow());
	dialog.SetInitialPath(File(edit_soundfile->GetText().Replace("<sounds>", dir)).GetFilePath());

	/* Get available file types.
	 */
	Array<String>	 types;
	Array<String>	 extensions;

	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_DECODER) continue;

		const Array<FileFormat *>	&formats = boca.GetComponentFormats(i);

		foreach (FileFormat *format, formats)
		{
			const Array<String>	&formatExtensions = format->GetExtensions();
			String			 extension;

			foreach (const String &formatExtension, formatExtensions)
			{
				extension.Append("*.").Append(formatExtension);

				if (foreachindex < formatExtensions.Length() - 1) extension.Append("; ");
			}

			types.Add(format->GetName().Append(" (").Append(extension).Append(")"));
			extensions.Add(extension);
		}
	}

	/* Add file types to dialog.
	 */
	String	 fileTypes;

	foreach (const String &extension, extensions)
	{
		if (!fileTypes.Contains(extension)) fileTypes.Append(foreachindex > 0 ? ";" : NIL).Append(extension);
	}

	I18n	*i18n = I18n::Get();

	i18n->SetContext("Joblist");

	dialog.AddFilter(i18n->TranslateString("Audio Files"), fileTypes);

	foreach (const String &type, types) dialog.AddFilter(type, extensions.GetNth(foreachindex));

	dialog.AddFilter(i18n->TranslateString("All Files"), "*.*");

	/* Show dialog.
	 */
	if (dialog.ShowDialog() == Success()) edit_soundfile->SetText(dialog.GetFileName());
}

Void BoCA::ConfigureNotifier::OnPreview()
{
	Directory	 dir(Utilities::GetBoCADirectory().Append(ResourcesFolder).Append("/sounds"));

	if (!dir.Exists()) dir = Utilities::GetBoCADirectory().Append("../freac/").Append(ResourcesFolder).Append("/sounds");

	player.Play(edit_soundfile->GetText().Replace("<sounds>", dir));
}

Void BoCA::ConfigureNotifier::OnStop()
{
	player.Stop();
}

Int BoCA::ConfigureNotifier::SaveSettings()
{
	Config	*config = Config::Get();
	I18n	*i18n	= I18n::Get();

	i18n->SetContext("Extensions::Notifier");

	config->SetIntValue(ConfigID, "EnableNotifications", enableNotifications);

	config->SetIntValue(ConfigID, "PlaySound", playSound);
	config->SetIntValue(ConfigID, "DisplayMessage", displayMessage);
	config->SetIntValue(ConfigID, "MinimumDuration", minimumDuration);

	config->SetStringValue(ConfigID, "SoundFile", edit_soundfile->GetText());

	if (edit_message->GetText() == i18n->TranslateString(DefaultMessage)) config->SetStringValue(ConfigID, "Message", DefaultMessage);
	else								      config->SetStringValue(ConfigID, "Message", edit_message->GetText());

	config->SetIntValue(ConfigID, "Seconds", edit_amount->GetText().ToInt() * (combo_units->GetSelectedEntryNumber() == 0 ? 1 : 60));

	return Success();
}
