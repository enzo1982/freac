 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
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
#include "i18n.h"
#include "config.h"
#include "winamp/in2.h"

using namespace smooth;
using namespace smooth::GUI;

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

						track = -1;
						drive = -1;
						discid = 0;

						hasText = False;
						year = -1;
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
					trackInfo = NIL;

					channels = 0;
					rate = 0;
					bits = 0;
					length = 0;
					fileSize = -1;
					order = BYTE_INTEL;
				}

				~bonkFormatInfo()
				{
					if (trackInfo != NIL) delete trackInfo;
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

		Progressbar		*progress;

		Bool			 encoding;
		Thread			*encoder_thread;

		Bool			 dontUpdateInfo;
		Bool			 cddbRetry;
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

		Bool			 LoadWinampDLLs();
		Void			 FreeWinampDLLs();

		Void			 AddFileByName(String, String out = NIL);
		Void			 ClearList();
		Void			 ReadCD();
		Void			 Encode();
		Int			 Encoder(Thread *);
		Void			 StopEncoding();
		InputFilter		*CreateInputFilter(String);
	public:
		static String		 version;
		static String		 cddbVersion;
		static String		 shortVersion;

		static bonkEncConfig	*currentConfig;
		static bonkTranslator	*i18n;

		HINSTANCE		 bonkdll;
		HINSTANCE		 bladedll;
		HINSTANCE		 lamedll;
		HINSTANCE		 vorbisdll;
		HINSTANCE		 faacdll;
		HINSTANCE		 tvqdll;
		HINSTANCE		 cdripdll;
		HINSTANCE		 id3dll;

		Array<HMODULE>		 winamp_in_plugins;
		Array<In_Module *>	 winamp_in_modules;
		Array<HMODULE>		 winamp_out_plugins;
		Array<Out_Module *>	 winamp_out_modules;

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
