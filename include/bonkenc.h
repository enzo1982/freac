 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
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
#include "3rdparty/winamp/in2.h"

using namespace smooth;
using namespace smooth::GUI;
using namespace smooth::GUI::Dialogs;
using namespace smooth::Threads;

class InputFilter;

extern Int	 ENCODER_BONKENC;
extern Int	 ENCODER_BLADEENC;
extern Int	 ENCODER_LAMEENC;
extern Int	 ENCODER_VORBISENC;
extern Int	 ENCODER_FAAC;
extern Int	 ENCODER_TVQ;
extern Int	 ENCODER_WAVE;

typedef unsigned long  uint32;
typedef unsigned short uint16;
typedef unsigned char  uint8;

extern bonkEncDebug	*debug_out;

class bonkEnc : public Application
{
	protected:
		Window			*mainWnd;
		Statusbar		*mainWnd_statusbar;

		ListBox			*joblist;
		Text			*txt_joblist;
		Hyperlink		*hyperlink;
		DropArea		*droparea;

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

		Bool			 encoding;
		Thread			*encoder_thread;
		Bool			 stop_encoding;

		Bool			 playing;
		Bool			 paused;
		Thread			*play_thread;
		Bool			 stop_playback;

		Int			 player_activedrive;
		Int			 player_plugin;
		Int			 player_entry;

		Bool			 dontUpdateInfo;
		Int			 encoder_activedrive;

		Bool			 LoadBonkDLL();
		Void			 FreeBonkDLL();
		Bool			 LoadBladeDLL();
		Void			 FreeBladeDLL();
		Bool			 LoadLAMEDLL();
		Void			 FreeLAMEDLL();
		Bool			 LoadVorbisDLL();
		Void			 FreeVorbisDLL();
		Bool			 LoadFAACDLL();
		Void			 FreeFAACDLL();
		Bool			 LoadFAAD2DLL();
		Void			 FreeFAAD2DLL();
		Bool			 LoadTVQDLL();
		Void			 FreeTVQDLL();
		Bool			 LoadCDRipDLL();
		Void			 FreeCDRipDLL();
		Bool			 LoadID3DLL();
		Void			 FreeID3DLL();
		Bool			 LoadEUpdateDLL();
		Void			 FreeEUpdateDLL();
		Bool			 LoadMP4V2DLL();
		Void			 FreeMP4V2DLL();
		Bool			 LoadFLACDLL();
		Void			 FreeFLACDLL();

		Bool			 LoadWinampDLLs();
		Void			 FreeWinampDLLs();

		Void			 AddFileByName(String, String out = NIL);
		Int			 Encoder(Thread *);
		InputFilter		*CreateInputFilter(String, bonkEncTrack *);
		Void			 PlayItem(Int);
		Int			 PlayThread(Thread *);

		String			 LocalizeNumber(Int);
	public:
		static String		 version;
		static String		 cddbVersion;
		static String		 shortVersion;

		static bonkEncConfig	*currentConfig;
		static I18n::Translator	*i18n;

		HINSTANCE		 bonkdll;
		HINSTANCE		 bladedll;
		HINSTANCE		 lamedll;
		HINSTANCE		 vorbisdll;
		HINSTANCE		 faacdll;
		HINSTANCE		 faad2dll;
		HINSTANCE		 tvqdll;
		HINSTANCE		 cdripdll;
		HINSTANCE		 id3dll;
		HINSTANCE		 eupdatedll;
		HINSTANCE		 mp4v2dll;
		HINSTANCE		 flacdll;

		Array<HMODULE>		 winamp_in_plugins;
		Array<In_Module *>	 winamp_in_modules;
		Array<HMODULE>		 winamp_out_plugins;
		Array<Out_Module *>	 winamp_out_modules;

		Bool			 cddbRetry;

		Array<bonkEncTrack *>	 sa_formatinfo;

					 bonkEnc();
					~bonkEnc();

		Array<bonkEncTrack *>	*GetCDDBData();
	slots:
		Void			 ClearList();

		Void			 ReadCD();

		Void			 Encode();
		Void			 StopEncoding();

		Void			 PlaySelectedItem();
		Void			 PausePlayback();
		Void			 StopPlayback();
		Void			 PlayPrevious();
		Void			 PlayNext();

		Void			 SelectDir();
};

#endif
