 /* fre:ac - free audio converter
  * Copyright (C) 2001-2023 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_DIALOG_SPLITTER
#define H_FREAC_DIALOG_SPLITTER

#include <smooth.h>
#include <boca.h>

#include "../jobs/job.h"

using namespace smooth;
using namespace smooth::GUI;

namespace freac
{
	enum SplitMode
	{
		SplitModeTime,
		SplitModeParts,
		SplitModeMetadata
	};

	class RepeatPreviousPartComponent;

	class DialogSplitter : public Dialogs::Dialog
	{
		private:
			Divider				*divbar;

			Window				*mainWnd;
			Titlebar			*mainWnd_titlebar;

			Text				*text_tracks;

			Button				*button_select_all;
			Button				*button_select_none;
			Button				*button_select_toggle;

			ListBox				*list_tracks;
			ListBox				*list_result;

			Progressbar			*progress_length;

			GroupBox			*group_mode;
			OptionBox			*option_mode_time;
			OptionBox			*option_mode_parts;
			OptionBox			*option_mode_silence;
			OptionBox			*option_mode_metadata;

			GroupBox			*group_time;
			Text				*text_time;
			ComboBox			*combo_time_mode;
			ComboBox			*combo_time_exact;
			EditBox				*edit_time_value;
			Arrows				*arrows_time_value;
			ComboBox			*combo_time_unit;
			CheckBox			*check_time_tracks;
			RepeatPreviousPartComponent	*repeat_time;

			GroupBox			*group_parts;
			Text				*text_parts;
			EditBox				*edit_parts;
			Arrows				*arrows_parts;
			CheckBox			*check_parts_tracks;
			RepeatPreviousPartComponent	*repeat_parts;

			GroupBox			*group_metadata;
			Text				*text_metadata;
			EditBox				*edit_metadata;
			List				*list_metadata_defaults;
			ListBox				*list_metadata_placeholders;
			RepeatPreviousPartComponent	*repeat_metadata;

			GroupBox			*group_filename;
			Text				*text_filename;
			EditBox				*edit_filename;
			List				*list_filename;
			CheckBox			*check_append_number;

			Button				*button_cancel;
			Button				*button_save;
			Button				*button_convert;

			Bool				 disableUpdates;

			Int				 splitMode;
			Bool				 splitAtTracks;

			Int				 partDuration;
			Int				 numberOfParts;

			Bool				 repeatEnd;
			Int				 repeatSeconds;

			Bool				 appendNumber;

			Array<BoCA::Track>		 joblistTracks;
			Array<Array<BoCA::Track> >	 resultTrackLists;

			Threads::Thread			*updateTrackLengthsThread;
			Bool				 updateTrackLengthsRunning;
			Bool				 cancelUpdateTrackLengths;

			Void				 SaveSettings();

			String				 GetJoblistEntryString(const BoCA::Track &) const;
			String				 GetResultEntryString(const BoCA::Track &) const;

			Int64				 GetTrackDuration(const BoCA::Track &, Int) const;

			Bool				 SortsAfter(const String &, const String &) const;

			Void				 UpdateTrackLengths();

			Void				 AddPreviousPartRepeat(Array<BoCA::Track> &, Int64 &, Int) const;

			const BoCA::Track		&GetIdentifyingTrack(const Array<BoCA::Track> &, Bool) const;
		public:
							 DialogSplitter(const Array<BoCA::Track> &);
							~DialogSplitter();

			const Error			&ShowDialog();
		slots:
			Void				 OnChangeSize(const Size &);

			Void				 SelectAll();
			Void				 SelectNone();
			Void				 ToggleSelection();

			Void				 OnMarkEntry(ListEntry *);
			Void				 OnClickTab(Int);

			Void				 OnSelectMode();
			Void				 OnToggleSplitAtTracks();

			Void				 OnEditDuration();
			Void				 OnChangeDuration();

			Void				 OnEditParts();
			Void				 OnChangeParts();

			Void				 OnToggleRepeatEnd(Bool);
			Void				 OnChangeRepeatDuration(Int);

			Void				 OnChangeSeconds();

			Void				 OnEditMetadataString();
			Void				 OnToggleMetadataEntry();

			Void				 OnEditFilenamePattern();
			Void				 OnToggleAppendNumber();

			Void				 OnChangeConfiguration();

			Void				 OnConvert();
			Void				 OnSave();
			Void				 OnCancel();
	};
};

#endif
