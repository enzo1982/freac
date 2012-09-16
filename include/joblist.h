 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_JOBLIST
#define H_BONKENC_JOBLIST

#include <smooth.h>
#include <boca.h>

#include "config.h"

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA;

namespace BonkEnc
{
	class JobList : public ListBox
	{
		private:
			DropArea			*droparea;
			Text				*text;

			Button				*button_sel_all;
			Button				*button_sel_none;
			Button				*button_sel_toggle;

			Array<Track>			 tracks;

			Int				 headerTabsHash;

			Bool				 AddTrack(const Track &);

			ListEntry			*GetEntryByTrack(const Track &) const;

			Void				 AddHeaderTabs();
			Void				 UpdateTextLine();

			String				 GetEntryText(const Track &) const;
		public:
							 JobList(const Point &, const Size &);
			virtual				~JobList();

			Bool				 CanModifyJobList() const;

			Int				 GetNOfTracks() const;
			const Track			&GetNthTrack(Int) const;

			const Array<Track>		*GetTrackList();

			Bool				 RemoveNthTrack(Int);
			Bool				 RemoveAllTracks();

			const Track			&GetSelectedTrack() const;

			Int				 SetMetrics(const Point &, const Size &);

			Void				 AddTrackByDialog();
			Void				 AddTracksByPattern(const String &, const String &);

			Void				 UpdateTrackInfo(const Track &);

			Void				 RemoveSelectedTrack();
		slots:
			Void				 AddTracksByDragAndDrop(const Array<String> &);

			Void				 SelectAll();
			Void				 SelectNone();
			Void				 ToggleSelection();

			Void				 LoadList();
			Void				 SaveList();

			Void				 StartJobRemoveAllTracks();

			Void				 OnRegister(Widget *);
			Void				 OnUnregister(Widget *);

			Void				 OnSelectEntry();
			Void				 OnMarkEntry(ListEntry *);

			Void				 OnComponentSelectTrack(const Track &);

			Void				 OnComponentMarkTrack(const Track &);
			Void				 OnComponentUnmarkTrack(const Track &);

			Void				 OnChangeConfigurationSettings();
			Void				 OnChangeLanguageSettings();
	};
};

#endif
