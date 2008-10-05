 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_JOBLIST_
#define _H_JOBLIST_

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

			Array<Track *>			 tracks;

			Bool				 AddTrack(const Track &);

			const String			&GetEntryText(const Track &);
			const String			&GetTooltipText(const Track &);
		public:
							 JobList(const Point &, const Size &);
			virtual				~JobList();

			Bool				 CanModifyJobList();

			Int				 GetNOfTracks();
			const Track			&GetNthTrack(Int);

			Bool				 RemoveNthTrack(Int);
			Bool				 RemoveAllTracks();

			const Track			&GetSelectedTrack();

			Int				 SetMetrics(const Point &, const Size &);

			Void				 AddTrackByDialog();
			Void				 AddTracksByPattern(const String &, const String &);

			Void				 AddTrackByFileName(const String &, const String & = NIL, Bool = True);

			Void				 UpdateTrackInfo(const Track &);

			Void				 RemoveSelectedTrack();
		slots:
			Void				 AddTrackByDragAndDrop(const String &);

			Void				 SelectAll();
			Void				 SelectNone();
			Void				 ToggleSelection();

			Void				 LoadList();
			Void				 SaveList();

			Void				 OnRegister(Widget *);
			Void				 OnUnregister(Widget *);

			Void				 OnSelectEntry();

			Void				 OnComponentSelectTrack(const Track &);

			Void				 OnChangeLanguageSettings();
	};
};

#endif
