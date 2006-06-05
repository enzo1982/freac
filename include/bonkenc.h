 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_BONKENC_
#define _H_BONKENC_

#include <smooth.h>

#include "config.h"
#include "track.h"
#include "debug.h"
#include "cddbinfo.h"

using namespace smooth;
using namespace smooth::GUI;
using namespace smooth::GUI::Dialogs;
using namespace smooth::Threads;

namespace BonkEnc
{
	class JobList;

	class InputFilter;
};

extern BEEXPORT Int	 ENCODER_BONKENC;
extern BEEXPORT Int	 ENCODER_BLADEENC;
extern BEEXPORT Int	 ENCODER_LAMEENC;
extern BEEXPORT Int	 ENCODER_VORBISENC;
extern BEEXPORT Int	 ENCODER_FAAC;
extern BEEXPORT Int	 ENCODER_FLAC;
extern BEEXPORT Int	 ENCODER_TVQ;
extern BEEXPORT Int	 ENCODER_WAVE;

typedef unsigned long  uint32;
typedef unsigned short uint16;
typedef unsigned char  uint8;

namespace BonkEnc
{
	extern BEEXPORT Debug	*debug_out;

	class BEEXPORT BonkEnc : public Application
	{
		protected:
			Window			*mainWnd;
			Statusbar		*mainWnd_statusbar;

			Hyperlink		*hyperlink;

			CheckBox		*check_playlist;
			CheckBox		*check_cuesheet;

			Divider			*info_divider;
			Layer			*info_background;
			CheckBox		*info_checkbox;
			Divider			*info_bottom;
			Text			*info_text_artist;
			EditBox			*info_edit_artist;
			Text			*info_text_title;
			EditBox			*info_edit_title;
			Text			*info_text_album;
			EditBox			*info_edit_album;
			Text			*info_text_track;
			EditBox			*info_edit_track;
			Text			*info_text_year;
			EditBox			*info_edit_year;
			Text			*info_text_genre;
			EditBox			*info_edit_genre;
			ListBox			*info_list_genre;

			Text			*enc_filename;
			Text			*enc_time;
			Text			*enc_percent;
			Text			*enc_encoder;
			Text			*enc_progress;
			Text			*enc_outdir;

			EditBox			*edb_filename;
			EditBox			*edb_time;
			EditBox			*edb_percent;
			EditBox			*edb_encoder;
			EditBox			*edb_outdir;

			Button			*btn_outdir;

			Progressbar		*progress;
			Progressbar		*progress_total;

			Thread			*encoder_thread;
			Bool			 pause_encoding;
			Bool			 stop_encoding;

			Thread			*play_thread;
			Bool			 stop_playback;

			Int			 player_activedrive;
			Int			 player_plugin;

			Bool			 dontUpdateInfo;
			Int			 encoder_activedrive;

			Int			 totalSamples;
			Float			 totalSamplesDone;

			Int			 startTicks;
			Int			 lastTicks;

			Int			 lastPercent;

			Int			 Encoder(Thread *);

			String			 GetPlaylistFileName(Track *);
			String			 GetRelativeFileName(const String &, const String &);

			String			 GetOutputFileName(Track *);
			String			 GetSingleOutputFileName(Track *);

			Void			 ComputeTotalNumberOfSamples();
			Void			 FixTotalNumberOfSamples(Track *, Track *);

			Void			 InitProgressValues();
			Void			 UpdateProgressValues(Track *, Int);
			Void			 FinishProgressValues(Track *);
		public:
			static String		 version;
			static String		 cddbVersion;
			static String		 shortVersion;

			static Config		*currentConfig;
			static I18n::Translator	*i18n;

			JobList			*joblist;

			Bool			 encoding;
			Bool			 playing;
			Bool			 paused;

			Int			 player_entry;

						 BonkEnc();
						~BonkEnc();

			virtual CDDBInfo	*GetCDDBData();
		slots:
			Void			 ReadCD();

			Void			 Encode();
			Void			 PauseEncoding();
			Void			 StopEncoding();
	};
};

#endif
