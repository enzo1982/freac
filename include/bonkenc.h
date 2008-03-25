 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
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
#include <boca.h>

#include "config.h"
#include "encoder.h"
#include "debug.h"
#include "cddb/cddb.h"
#include "cddb/cddbinfo.h"

using namespace smooth;
using namespace smooth::GUI;
using namespace smooth::GUI::Dialogs;

namespace BonkEnc
{
	class JobList;
	class Encoder;

	class InputFilter;
};

typedef unsigned long  uint32;
typedef unsigned short uint16;
typedef unsigned char  uint8;

namespace BonkEnc
{
	extern Debug	*debug_out;

	abstract class BonkEnc : public Application
	{
		protected:
			/* Singleton class, therefore protected constructor/destructor
			 */
			static BonkEnc			*instance;

							 BonkEnc();
			virtual				~BonkEnc();

			Window				*mainWnd;
			Statusbar			*mainWnd_statusbar;

			Hyperlink			*hyperlink;

			Config				*currentConfig;

			Bool				 dontUpdateInfo;
			Bool				 overwriteAll;

			Bool				 InitCDRip();
		public:
			static String			 version;
			static String			 shortVersion;
			static String			 cddbVersion;
			static String			 cddbMode;
			static String			 updatePath;

			static BoCA::I18n		*i18n;

			JobList				*joblist;

			Encoder				*encoder;

			virtual CDDBInfo		 GetCDDBData();
			CDDBInfo			 QueryCDDB(CDDB &);

			/* Returns an existing instance of BonkEnc
			 */
			static BonkEnc			*Get();
		slots:
			Void				 ReadCD();
	};
};

#endif
