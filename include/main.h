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

typedef struct
{
	Int	 track;
	Int	 drive;

	Bool	 cdText;
	String	 artist;
	String	 title;
	String	 album;
	String	 comment;
	String	 genre;
	String	 year;

	String	 outfile;
	String	 origFilename;
}
bonkTrackInfo;

typedef struct
{
	bonkTrackInfo	*trackInfo;

	Int		 channels;
	Int		 rate;
	Int		 bits;
	Int		 length;
	Int		 order;
}
bonkFormatInfo;

// bonkEnc Application class definition

class bonkEnc : public Application
{
	private:
		Menu			*menu_file;
		Menu			*menu_options;
		Menu			*menu_addsubmenu;
		Menu			*menu_encode;

		Menubar			*mainWnd_menubar;
		Menubar			*mainWnd_iconbar;
		Window			*mainWnd;
		Titlebar		*mainWnd_titlebar;
		Statusbar		*mainWnd_statusbar;

		ListBox			*joblist;
		Text			*txt_joblist;
		Hyperlink		*hyperlink;
		DropArea		*droparea;

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

		Array<String>		 sa_joblist;
		Array<bonkTrackInfo *>	 sa_trackinfo;
		Bool			 encoding;
		Thread			*encoder_thread;

		Array<String>		 cdText;

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

		Int			 ReadCDText();
		Int			 FreeCDText();

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
		Array<bonkTrackInfo *>	*GetCDDBData();
		Void			 ConfigureEncoder();
		Void			 ConfigureGeneral();
		Void			 Encode();
		Int			 Encoder(Thread *);
		Void			 StopEncoding();
		Bool			 ExitProc();
		Void			 DrawProc();

		Bool			 SetLanguage(String);
	public:
		static bonkEncConfig	*currentConfig;
		static bonkTranslator	*i18n;

					 bonkEnc();
					~bonkEnc();
};

#endif
