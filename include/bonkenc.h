 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
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
#include "winamp/in2.h"

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

const int	 BYTE_INTEL	= 0;
const int	 BYTE_RAW	= 1;

typedef unsigned long  uint32;
typedef unsigned short uint16;
typedef unsigned char  uint8;

class bonkFormatInfo
{
	public:
		class bonkTrackInfo
		{
			public:
					 bonkTrackInfo()
					{
						isCDTrack = False;

						drive	= -1;
						discid	= 0;
						cdTrack	= -1;

						hasText	= False;
						track	= -1;
						year	= -1;
					}

				Bool	 isCDTrack;
				Int	 drive;
				Int	 discid;
				Int	 cdTrack;

				Bool	 hasText;
				String	 artist;
				String	 title;
				String	 album;
				Int	 track;
				String	 genre;
				Int	 year;
				String	 comment;
				String	 playorder;

				String	 length;
				String	 fileSize;

				String	 outfile;
				String	 origFilename;
		};

				 bonkFormatInfo()
				{
					trackInfo = new bonkTrackInfo;

					channels = 0;
					rate = 0;
					bits = 0;
					length = 0;
					fileSize = -1;
					order = BYTE_INTEL;
				}

				~bonkFormatInfo()
				{
					delete trackInfo;
				}

		bonkTrackInfo	*trackInfo;

		Int		 channels;
		Int		 rate;
		Int		 bits;
		Int		 length;
		Int		 fileSize;
		Int		 order;
};

class CDDBInfo;

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
		Bool			 LoadTVQDLL();
		Void			 FreeTVQDLL();
		Bool			 LoadCDRipDLL();
		Void			 FreeCDRipDLL();
		Bool			 LoadID3DLL();
		Void			 FreeID3DLL();
		Bool			 LoadEUpdateDLL();
		Void			 FreeEUpdateDLL();

		Bool			 LoadWinampDLLs();
		Void			 FreeWinampDLLs();

		Void			 AddFileByName(String, String out = NIL);
		Void			 ClearList();
		Void			 ReadCD();
		Void			 Encode();
		Int			 Encoder(Thread *);
		Void			 StopEncoding();
		InputFilter		*CreateInputFilter(String);
		Void			 PlayItem(Int);
		Void			 PlaySelectedItem();
		Int			 PlayThread(Thread *);
		Void			 PausePlayback();
		Void			 StopPlayback();
		Void			 PlayPrevious();
		Void			 PlayNext();
		Void			 SelectDir();
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
		HINSTANCE		 tvqdll;
		HINSTANCE		 cdripdll;
		HINSTANCE		 id3dll;
		HINSTANCE		 eupdatedll;

		Array<HMODULE>		 winamp_in_plugins;
		Array<In_Module *>	 winamp_in_modules;
		Array<HMODULE>		 winamp_out_plugins;
		Array<Out_Module *>	 winamp_out_modules;

		Bool			 cddbRetry;

		Array<String>		 cdText;
		CDDBInfo		*cddbInfo;
		Array<bonkFormatInfo *>	 sa_formatinfo;

					 bonkEnc();
					~bonkEnc();

		Int			 ReadCDText();
		Int			 FreeCDText();

		Array<bonkFormatInfo::bonkTrackInfo *>	*GetCDDBData();
};

#endif
