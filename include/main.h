 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_MAIN_
#define _H_MAIN_

#include <smooth.h>
#include "i18n.h"
#include "config.h"

using namespace smooth;
using namespace smooth::GUI;

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

// bonkEnc Application class definition

class bonkEnc : public Application
{
	private:
		Menu			*menu_file;
		Menu			*menu_options;
		Menu			*menu_addsubmenu;
		Menu			*menu_encode;
		Menu			*menu_drives;
		Menu			*menu_seldrive;
		Menu			*menu_database;

		Menubar			*mainWnd_menubar;
		Menubar			*mainWnd_iconbar;
		Window			*mainWnd;
		Titlebar		*mainWnd_titlebar;
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
		ComboBox		*info_combo_genre;

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

		HINSTANCE		 bonkdll;

		Bool			 LoadBonkDLL();
		Void			 FreeBonkDLL();

		HINSTANCE		 bladedll;

		Bool			 LoadBladeDLL();
		Void			 FreeBladeDLL();

		HINSTANCE		 lamedll;

		Bool			 LoadLAMEDLL();
		Void			 FreeLAMEDLL();

		HINSTANCE		 vorbisdll;

		Bool			 LoadVorbisDLL();
		Void			 FreeVorbisDLL();

		HINSTANCE		 faacdll;

		Bool			 LoadFAACDLL();
		Void			 FreeFAACDLL();

		HINSTANCE		 tvqdll;

		Bool			 LoadTVQDLL();
		Void			 FreeTVQDLL();

		HINSTANCE		 cdripdll;

		Bool			 LoadCDRipDLL();
		Void			 FreeCDRipDLL();

		HINSTANCE		 id3dll;

		Bool			 LoadID3DLL();
		Void			 FreeID3DLL();

		Void			 ConsoleMode();
		Bool			 ScanForParameter(String, String *);
		Void			 ScanForFiles(Array<String> *);

		Void			 Close();
		Void			 About();
		Void			 AddFile();
		Void			 AddFileByName(String, String out = NIL);
		Void			 AddDragDropFile(String *);
		Void			 RemoveFile();
		Void			 ClearList();
		Void			 ReadCD();
		Void			 ReadSpecificCD();
		Void			 ConfigureEncoder();
		Void			 ConfigureGeneral();
		Void			 Encode();
		Int			 Encoder(Thread *);
		Void			 StopEncoding();
		Bool			 ExitProc();
		Void			 DrawProc();
		Void			 ShowHideTitleInfo();
		Void			 UpdateTitleInfo();
		Void			 SelectJoblistEntry();
		Void			 SubmitCDDBData();
		Void			 ToggleCDDB();

		Bool			 SetLanguage(String);
	public:
		static bonkEncConfig	*currentConfig;
		static bonkTranslator	*i18n;

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
