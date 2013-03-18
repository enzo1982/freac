 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2012 Robert Kausch <robert.kausch@bonkenc.org>
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

using namespace smooth;
using namespace smooth::GUI;

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

			Array<BoCA::Track>		 tracks;

			Int				 headerTabsHash;

			Bool				 AddTrack(const BoCA::Track &);

			ListEntry			*GetEntryByTrack(const BoCA::Track &) const;

			Void				 AddHeaderTabs();
			Void				 UpdateTextLine();

			String				 GetEntryText(const BoCA::Track &) const;
		public:
							 JobList(const Point &, const Size &);
			virtual				~JobList();

			Bool				 CanModifyJobList() const;

			Int				 GetNOfTracks() const;
			const BoCA::Track		&GetNthTrack(Int) const;

			const Array<BoCA::Track>	*GetTrackList();

			Bool				 RemoveNthTrack(Int);
			Bool				 RemoveAllTracks();

			const BoCA::Track		&GetSelectedTrack() const;

			Int				 SetMetrics(const Point &, const Size &);

			Void				 AddTrackByDialog();
			Void				 AddTracksByPattern(const String &, const String &);

			Void				 UpdateTrackInfo(const BoCA::Track &);

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

			Void				 OnComponentSelectTrack(const BoCA::Track &);

			Void				 OnComponentMarkTrack(const BoCA::Track &);
			Void				 OnComponentUnmarkTrack(const BoCA::Track &);

			Void				 OnChangeConfigurationSettings();
			Void				 OnChangeLanguageSettings();
	};
};

#endif
