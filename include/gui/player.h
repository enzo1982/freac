 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_PLAYER
#define H_FREAC_PLAYER

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BonkEnc
{
	class JobList;

	class LayerPlayer : public Layer
	{
		private:
			JobList	*joblist;

			Button	*button_play;
			Button	*button_pause;
			Button	*button_stop;
			Button	*button_prev;
			Button	*button_next;
			Button	*button_open;

			Slider	*slider_play;
		public:
				 LayerPlayer(JobList *);
				~LayerPlayer();
		slots:
			Void	 OnChangeLanguageSettings();

			Void	 OnJoblistRemoveTrack(const BoCA::Track &);
			Void	 OnJoblistRemoveAllTracks();

			Void	 PlaySelectedItem();
			Void	 PauseResumePlayback();
			Void	 StopPlayback();
			Void	 OpenCDTray();

			Void	 OnChangePlayPosition();
	};
};

#endif
