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

#ifndef H_FREAC_JOBLIST
#define H_FREAC_JOBLIST

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

namespace freac
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

			UnsignedInt32			 headerTabsHash;

			Bool				 AddTrack(const BoCA::Track &);
			Bool				 RemoveTrack(const BoCA::Track &);

			Void				 FindTracksByPattern(Array<String> &, const String &, const String &, Bool) const;

			Void				 AddHeaderTabs();
			Void				 UpdateTextLine();

			static Bool			 SortsAfter(const String &, const String &);
			static String			 GetEntryText(const BoCA::Track &);
		public:
							 JobList(const Point &, const Size &);
			virtual				~JobList();

			Int				 GetNOfTracks() const;
			const BoCA::Track		&GetNthTrack(Int) const;

			const Array<BoCA::Track>	*GetTrackList();

			Bool				 RemoveNthTrack(Int);
			Bool				 RemoveAllTracks();

			const BoCA::Track		&GetSelectedTrack() const;

			Int				 SetMetrics(const Point &, const Size &);

			Void				 AddTrackByDialog();
			Void				 AddTracksByPattern(const String &, const String &, Bool);

			Void				 UpdateTrackInfo(const BoCA::Track &);

			Void				 RemoveSelectedTrack();

			ListEntry			*GetEntryByTrack(const BoCA::Track &) const;
		slots:
			Bool				 AddTracksByFileNames(const Array<String> &);
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

			Void				 OnClickTab(Int);

			Void				 OnComponentSelectTrack(const BoCA::Track &);

			Void				 OnComponentMarkTrack(const BoCA::Track &);
			Void				 OnComponentUnmarkTrack(const BoCA::Track &);

			Void				 OnChangeConfigurationSettings();
			Void				 OnChangeLanguageSettings();
			Void				 OnChangeHeaderColumns();
	};
};

#endif
