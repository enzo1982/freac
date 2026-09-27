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

#include "editor.h"
#include "valuelist.h"
#include "../utilities.h"

BoCA::Editor::Editor(const String &title) : Layer(title)
{
	ignoreSelect   = False;
	activeValueBox = NIL;
	activeTrackID  = -1;

	allowTrackChangeByArrowKey.Connect(True);
	allowTrackRemoveByDeleteKey.Connect(True);
}

BoCA::Editor::~Editor()
{
}

Void BoCA::Editor::OnSelectTrack(const Track &track)
{
}

Void BoCA::Editor::OnSelectAlbum(const Track &track)
{
}

Void BoCA::Editor::OnSelectNone()
{
}

/* Called when a field with several values requests its value list.
 * ----
 * Opens the value list dialog and applies each change at once
 * to the track that was selected when the dialog opened.
 */
Void BoCA::Editor::OnRequestValueList(MultiValueEditBox *box)
{
	ValueListDialog	 dialog(box->GetLabelText(), box->GetValues(), box->GetProposedValue());

	dialog.onChangeValues.Connect(&Editor::OnChangeValues, this);

	activeValueBox = box;
	activeTrackID  = track.GetTrackID();

	dialog.ShowDialog();

	activeValueBox = NIL;
	activeTrackID  = -1;
}

Void BoCA::Editor::OnChangeValues(const Array<String> &values)
{
	/* Apply changes only to the track whose values the dialog shows.
	 * The selection can change while the dialog is open, for example
	 * when a conversion removes the track from the joblist.
	 */
	if (activeValueBox == NIL || track.GetTrackID() != activeTrackID) return;

	Info	 info = track.GetInfo();

	TagUtilities::ReplaceOtherInfo(info, activeValueBox->GetKey(), values);

	activeValueBox->SetValues(Info::GetOtherInfoList(&info, activeValueBox->GetKey()));

	track.SetInfo(info);

	/* Prevent re-selecting entry and emit onModifyTrack signal.
	 */
	ignoreSelect = True;

	onModifyTrack.Emit(track);

	ignoreSelect = False;
}
