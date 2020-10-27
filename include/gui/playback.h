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

#ifndef H_FREAC_PLAYBACK
#define H_FREAC_PLAYBACK

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

namespace freac
{
	class Player;
	class JobList;

	class PlaybackWidget : public Layer
	{
		private:
			Player		*player;
			JobList		*joblist;

			Button		*button_play;
			Button		*button_pause;
			Button		*button_stop;
			Button		*button_prev;
			Button		*button_next;
			Button		*button_open;

			Slider		*slider_play;

			BoCA::Track	 playingTrack;
		public:
					 PlaybackWidget(Player *, JobList *);
					~PlaybackWidget();
		slots:
			Void		 OnChangeLanguageSettings();

			Void		 OnJoblistRemoveTrack(const BoCA::Track &);
			Void		 OnJoblistRemoveAllTracks();

			Void		 PlaySelectedItem();
			Void		 PlayPreviousItem();
			Void		 PlayNextItem();

			Void		 Play(const BoCA::Track &);

			Void		 PauseResumePlayback();
			Void		 StopPlayback();

			Void		 OnPlay(const BoCA::Track &);
			Void		 OnFinish(const BoCA::Track &);

			Void		 OpenCDTray();

			Void		 OnChangePlayPosition();
	};
};

#endif
