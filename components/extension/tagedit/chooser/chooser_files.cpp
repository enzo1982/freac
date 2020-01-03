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

#include <boca.h>

#include "chooser_files.h"
#include "chooser_files_tree.h"
#include "chooser_files_utilities.h"

BoCA::ChooserFiles::ChooserFiles() : Chooser("Files")
{
	list_directories	= new ListBox(Point(7,7), Size(200, 150));

	div_split		= new Divider(210, OR_VERT | DIV_MOVABLE);
	div_split->onDrag.Connect(&ChooserFiles::OnDragDivider, this);

	edit_directory		= new EditBox(Point(215, 7), Size(100, 0));
	edit_directory->Deactivate();

	list_files		= new ListBox(Point(215, 34), Size(100, 150));
	list_files->Deactivate();
	list_files->onSelectEntry.Connect(&ChooserFiles::OnSelectFile, this);

	text_nofiles		= new Text(NIL, Point());
	text_nofiles->SetFont(GUI::Font(GUI::Font::Default, 12, GUI::Font::Bold, 0, Setup::InactiveTextColor));

	shortcut_previous	= new Shortcut(0, Input::Keyboard::KeyUp, list_files);
	shortcut_previous->onKeyDown.Connect(&ChooserFiles::OnShortcutPrevious, this);

	shortcut_next		= new Shortcut(0, Input::Keyboard::KeyDown, list_files);
	shortcut_next->onKeyDown.Connect(&ChooserFiles::OnShortcutNext, this);

	shortcut_first		= new Shortcut(0, Input::Keyboard::KeyHome, list_files);
	shortcut_first->onKeyDown.Connect(&ChooserFiles::OnShortcutFirst, this);

	shortcut_last		= new Shortcut(0, Input::Keyboard::KeyEnd, list_files);
	shortcut_last->onKeyDown.Connect(&ChooserFiles::OnShortcutLast, this);

	btn_save		= new Button(String(), Point(176, 30), Size());
	btn_save->SetOrientation(OR_LOWERRIGHT);
	btn_save->Deactivate();
	btn_save->onAction.Connect(&ChooserFiles::OnSave, this);

	btn_saveall		= new Button(String(), Point(88, 30), Size());
	btn_saveall->SetOrientation(OR_LOWERRIGHT);
	btn_saveall->Deactivate();
	btn_saveall->onAction.Connect(&ChooserFiles::OnSaveAll, this);

	ChooserFilesTree::onSelectDirectory.Connect(&ChooserFiles::OnSelectDirectory, this);

#ifdef __WIN32__
	/* Add local drives.
	 */
	for (Int i = 0; i < 26; i++)
	{
		char		 name[4] = { char('A' + i), ':', '\\', 0};
		Directory	 drive(name);

		if (drive.Exists())
		{
			Tree		*tree = new ChooserFilesTree(drive);

			trees.Add(tree);

			list_directories->Add(tree);
		}
	}
#else
	/* Add root directory.
	 */
	Tree	*root = new ChooserFilesTree(Directory("/"));

	trees.Add(root);

	list_directories->Add(root);
#endif

	Add(list_directories);

	Add(div_split);

	Add(edit_directory);
	Add(list_files);
	Add(text_nofiles);

	Add(shortcut_previous);
	Add(shortcut_next);
	Add(shortcut_first);
	Add(shortcut_last);

	Add(btn_save);
	Add(btn_saveall);

	onChangeSize.Connect(&ChooserFiles::OnChangeSize, this);

	Settings::Get()->onChangeLanguageSettings.Connect(&ChooserFiles::OnChangeLanguageSettings, this);
}

BoCA::ChooserFiles::~ChooserFiles()
{
	Settings::Get()->onChangeLanguageSettings.Disconnect(&ChooserFiles::OnChangeLanguageSettings, this);

	ChooserFilesTree::onSelectDirectory.Disconnect(&ChooserFiles::OnSelectDirectory, this);

	list_files->RemoveAllEntries();

	foreach (Tree *tree, trees) DeleteObject(tree);

	DeleteObject(list_directories);

	DeleteObject(div_split);

	DeleteObject(edit_directory);
	DeleteObject(list_files);
	DeleteObject(text_nofiles);

	DeleteObject(shortcut_previous);
	DeleteObject(shortcut_next);
	DeleteObject(shortcut_first);
	DeleteObject(shortcut_last);

	DeleteObject(btn_save);
	DeleteObject(btn_saveall);
}

/* Called when component canvas size changes.
 * ----
 */
Void BoCA::ChooserFiles::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	list_directories->SetHeight(clientSize.cy - 15);

	edit_directory->SetWidth(clientSize.cx - edit_directory->GetX() - 8);
	list_files->SetSize(Size(clientSize.cx - list_files->GetX() - 8, clientSize.cy - 72));
	text_nofiles->SetPosition(list_files->GetPosition() + Point((list_files->GetWidth() - text_nofiles->GetUnscaledTextWidth()) / 2, (list_files->GetHeight() - 13 - text_nofiles->GetUnscaledTextHeight()) / 2 + 13));
}

/* Called when application language is changed.
 * ----
 */
Void BoCA::ChooserFiles::OnChangeLanguageSettings()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Tag Editor");

	SetText(i18n->TranslateString("Files"));

	/* Hide all affected widgets prior to changing
	 * labels to avoid flickering.
	 */
	Bool	 prevVisible = IsVisible();

	if (prevVisible) Hide();

	list_files->RemoveAllTabs();

	list_files->AddTab(i18n->TranslateString("File"));

	text_nofiles->SetText(i18n->TranslateString("no audio files found"));

	btn_save->SetText(i18n->TranslateString("Save"));
	btn_save->SetWidth(Math::Max(80, btn_save->GetUnscaledTextWidth() + 14));

	btn_saveall->SetText(i18n->TranslateString("Save all"));
	btn_saveall->SetWidth(Math::Max(80, btn_saveall->GetUnscaledTextWidth() + 14));

	btn_save->SetX(btn_save->GetWidth() + btn_saveall->GetWidth() + 16);
	btn_saveall->SetX(btn_saveall->GetWidth() + 8);

	/* OnChangeSize will correct sizes of any other widgets.
	 */
	OnChangeSize(GetSize());

	/* Show all widgets again.
	 */
	if (prevVisible) Show();
}

/* Called when the splitting divider is dragged with the mouse.
 * ----
 */
Void BoCA::ChooserFiles::OnDragDivider(Int pos)
{
	Rect	 clientRect = Rect(GetRealPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	if (pos > clientSize.cx - 300) pos = clientSize.cx - 300;
	if (pos <		  150) pos =		     150;

	if (div_split->GetPos() == pos) return;

	Surface	*surface = GetDrawSurface();

	surface->StartPaint(GetVisibleArea());

	div_split->Hide();
	list_directories->Hide();
	edit_directory->Hide();
	list_files->Hide();

	list_directories->SetWidth(pos - 10);

	edit_directory->SetWidth(clientSize.cx - pos - 13);
	edit_directory->SetX(pos + 5);

	list_files->SetWidth(clientSize.cx - pos - 13);
	list_files->SetX(pos + 5);

	div_split->SetPos(pos);

	text_nofiles->SetPosition(list_files->GetPosition() + Point((list_files->GetWidth() - text_nofiles->GetUnscaledTextWidth()) / 2, (list_files->GetHeight() - 13 - text_nofiles->GetUnscaledTextHeight()) / 2 + 13));

	div_split->Show();
	list_directories->Show();
	edit_directory->Show();
	list_files->Show();

	if (text_nofiles->IsVisible()) text_nofiles->Paint(SP_PAINT);

	surface->EndPaint();
}

/* Called when a directory entry is selected.
 * ----
 * Reads files in the selected directory and adds them to the files list.
 */
Void BoCA::ChooserFiles::OnSelectDirectory(const Directory &directory)
{
	if (String(directory) == edit_directory->GetText()) return;

	GetSupportedFileExtensions();

	if (modified.Length() > 0)
	{
		if (Dialogs::Message::Button::Yes == Dialogs::QuickMessage("There are unsaved files in this directory. Would you like to save them now?", "Save changes", Dialogs::Message::Buttons::YesNo, Dialogs::Message::Icon::Question)) OnSaveAll();
	}

	edit_directory->SetText(String(directory).Append(Directory::GetDirectoryDelimiter()));

	if (IsRegistered()) container->GetDrawSurface()->StartPaint(Rect(list_files->GetRealPosition(), list_files->GetRealSize()));

	list_files->RemoveAllEntries();

	const Array<File>	&files = directory.GetFiles();

	foreach (const File &file, files)
	{
		String	 filename = file.GetFileName();
		String	 extension = filename.Tail(filename.Length() - filename.FindLast(".") - 1).ToLower();

		if (extensions.Get(extension.ComputeCRC32()) != NIL) list_files->AddEntry(filename);
	}

	if (list_files->Length() == 0)
	{
		list_files->Deactivate();

		if (text_nofiles->IsVisible())	text_nofiles->Paint(SP_PAINT);
		else				text_nofiles->Show();
	}
	else
	{
		text_nofiles->Hide();
		list_files->Activate();
	}

	if (IsRegistered()) container->GetDrawSurface()->EndPaint();

	btn_save->Deactivate();
	btn_saveall->Deactivate();

	onSelectNone.Emit();

	tracks.RemoveAll();
	modified.RemoveAll();
}

/* Called when a file entry is selected.
 * ----
 * Tries to open the file and generate a track object.
 */
Void BoCA::ChooserFiles::OnSelectFile(ListEntry *entry)
{
	if (tracks.Get(list_files->GetSelectedEntryNumber()) != NIL)
	{
		if (modified.Get(list_files->GetSelectedEntryNumber()) != NIL)	btn_save->Activate();
		else								btn_save->Deactivate();

		onSelectTrack.Emit(tracks.Get(list_files->GetSelectedEntryNumber()));

		return;
	}

	File			 file(entry->GetText(), edit_directory->GetText());
	DecoderComponent	*decoder = ChooserFilesUtilities::CreateDecoderComponent(file);

	if (decoder == NIL)
	{
		I18n	*i18n = I18n::Get();

		i18n->SetContext("Extensions::Tag Editor::Errors");

		Utilities::ErrorMessage(i18n->TranslateString("Unable to open file: %1\n\nError: %2").Replace("%1", file.GetFileName()).Replace("%2", i18n->TranslateString("Unknown file type", "Messages")));

		return;
	}

	Track	 track;
	Error	 error = decoder->GetStreamInfo(file, track);
	String	 errorString = decoder->GetErrorString();

	Registry::Get().DeleteComponent(decoder);

	if (error == Error())
	{
		I18n	*i18n = I18n::Get();

		i18n->SetContext("Extensions::Tag Editor::Errors");

		Utilities::ErrorMessage(i18n->TranslateString("Unable to open file: %1\n\nError: %2").Replace("%1", file.GetFileName()).Replace("%2", i18n->TranslateString(errorString, "Messages")));

		return;
	}

	track.fileName = file;

	btn_save->Deactivate();

	onSelectTrack.Emit(track);

	tracks.Add(track, list_files->GetSelectedEntryNumber());
}

/* Called when the up arrow key is pressed.
 * ----
 * Selects the previous file.
 */
Void BoCA::ChooserFiles::OnShortcutPrevious()
{
	if (!IsVisible() || !allowTrackChangeByArrowKey.Call()) return;

	if (list_files->GetSelectedEntryNumber() == -1) list_files->SelectNthEntry(list_files->Length()			- 1);
	else						list_files->SelectNthEntry(list_files->GetSelectedEntryNumber() - 1);
}

/* Called when the down arrow key is pressed.
 * ----
 * Selects the next file.
 */
Void BoCA::ChooserFiles::OnShortcutNext()
{
	if (!IsVisible() || !allowTrackChangeByArrowKey.Call()) return;

	list_files->SelectNthEntry(list_files->GetSelectedEntryNumber() + 1);
}

/* Called when the home key is pressed.
 * ----
 * Selects the first file.
 */
Void BoCA::ChooserFiles::OnShortcutFirst()
{
	if (!IsVisible() || !allowTrackRemoveByDeleteKey.Call()) return;

	list_files->SelectNthEntry(0);
}

/* Called when the end key is pressed.
 * ----
 * Selects the last file.
 */
Void BoCA::ChooserFiles::OnShortcutLast()
{
	if (!IsVisible() || !allowTrackRemoveByDeleteKey.Call()) return;

	list_files->SelectNthEntry(list_files->Length() - 1);
}

/* Called when a track is modified.
 * ----
 * Finds the track and marks it as modified.
 */
Void BoCA::ChooserFiles::OnModifyTrack(const Track &track)
{
	for (Int i = 0; i < tracks.Length(); i++)
	{
		if (tracks.GetNth(i).GetTrackID() == track.GetTrackID())
		{
			GUI::Font	 font = list_files->GetNthEntry(tracks.GetNthIndex(i))->GetFont();

			font.SetColor(Color(0, 0, 255));

			list_files->GetNthEntry(tracks.GetNthIndex(i))->SetFont(font);

			/* Update track.
			 */
			tracks.GetNthReference(i) = track;

			/* Add it to the list of modified tracks.
			 */
			modified.Add(track.GetTrackID(), tracks.GetNthIndex(i));

			if (tracks.GetNthIndex(i) == list_files->GetSelectedEntryNumber()) btn_save->Activate();

			btn_saveall->Activate();

			return;
		}
	}
}

/* Called when the "Save" button is clicked.
 * ----
 * Invokes SaveFileTag() for the selected file.
 */
Void BoCA::ChooserFiles::OnSave()
{
	Int	 i = list_files->GetSelectedEntryNumber();

	if (SaveFileTag(tracks.Get(i)) != Success()) return;

	GUI::Font	 font = list_files->GetNthEntry(i)->GetFont();

	font.SetColor(Color(0, 0, 0));

	list_files->GetNthEntry(i)->SetFont(font);

	modified.Remove(i);

	btn_save->Deactivate();

	if (modified.Length() == 0) btn_saveall->Deactivate();
}

/* Called when the "Save all" button is clicked.
 * ----
 * Invokes SaveFileTag() for all modified files.
 */
Void BoCA::ChooserFiles::OnSaveAll()
{
	Int	 failed = 0;

	while (modified.Length() - failed > 0)
	{
		Int	 i = modified.GetNthIndex(0 + failed);

		if (SaveFileTag(tracks.Get(i)) != Success()) { failed++; continue; }

		GUI::Font	 font = list_files->GetNthEntry(i)->GetFont();

		font.SetColor(Color(0, 0, 0));

		list_files->GetNthEntry(i)->SetFont(font);

		modified.Remove(i);
	}

	if (modified.Get(list_files->GetSelectedEntryNumber()) == NIL) btn_save->Deactivate();

	if (modified.Length() == 0) btn_saveall->Deactivate();
}

/* Called when the currently selected entry needs to be selected again.
 * ----
 */
Void BoCA::ChooserFiles::ReselectEntry()
{
	if (list_files->GetSelectedEntry() == NIL) return;

	OnSelectFile(list_files->GetSelectedEntry());
}

Int BoCA::ChooserFiles::SaveFileTag(const Track &track)
{
	Config	*config = Config::Get();

	Int	 error	     = Error();
	String	 errorString = "Unknown error";

	/* Create decoder component.
	 */
	DecoderComponent	*decoder = ChooserFilesUtilities::CreateDecoderComponent(track.fileName);

	/* Loop over supported formats.
	 */
	String	 lcURI = track.fileName.ToLower();

	foreach (FileFormat *format, decoder->GetFormats())
	{
		foreach (const String &extension, format->GetExtensions())
		{
			if (!lcURI.EndsWith(String(".").Append(extension))) continue;

			/* Update tags.
			 */
			const Array<TagFormat>	&tagFormats = format->GetTagFormats();

			foreach (const TagFormat &tagFormat, tagFormats)
			{
				Registry	&boca	= AS::Registry::Get();
				TaggerComponent	*tagger = (AS::TaggerComponent *) boca.CreateComponentByID(tagFormat.GetTagger());

				if (tagger != NIL)
				{
					foreach (TagSpec *spec, tagger->GetTagSpecs())
					{
						if (spec->GetName() != tagFormat.GetName()) continue;

						/* Set to Success() by default, so we won't report
						 * an error if the tag type is simply deactivated.
						 */
						error = Success();

						if (config->GetIntValue("Tags", String("Enable").Append(tagFormat.GetName().Replace(" ", NIL)), spec->IsDefault()))
						{
							error	    = tagger->UpdateStreamInfo(track.fileName, track);
							errorString = tagger->GetErrorString();
						}

						break;
					}

					boca.DeleteComponent(tagger);
				}
				else
				{
					error	    = Error();
					errorString = "Not supported";
				}
			}

			break;
		}
	}

	Registry::Get().DeleteComponent(decoder);

	/* Check for error.
	 */
	if (error == Error())
	{
		I18n	*i18n = I18n::Get();

		i18n->SetContext("Extensions::Tag Editor::Errors");

		Utilities::ErrorMessage(i18n->TranslateString("Unable to update tag: %1\n\nError: %2").Replace("%1", track.fileName).Replace("%2", i18n->TranslateString(errorString, "Messages")));

		return Error();
	}

	return Success();
}

Void BoCA::ChooserFiles::GetSupportedFileExtensions()
{
	if (extensions.Length() > 0) return;

	Registry		&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_DECODER) continue;

		const Array<FileFormat *>	&formats = boca.GetComponentFormats(i);

		foreach (FileFormat *format, formats)
		{
			const Array<String>	&extensions = format->GetExtensions();

			foreach (const String &extension, extensions) this->extensions.Add(extension.ToLower(), extension.ToLower().ComputeCRC32());
		}
	}
}
