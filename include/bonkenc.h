 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
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
#include "cddb/cddb.h"
#include "cddb/cddbinfo.h"

using namespace smooth;
using namespace smooth::GUI;
using namespace smooth::GUI::Dialogs;

namespace BonkEnc
{
	class JobList;
	class Encoder;
};

typedef unsigned long  uint32;
typedef unsigned short uint16;
typedef unsigned char  uint8;

namespace BonkEnc
{
	abstract class BonkEnc : public Application
	{
		protected:
			/* Singleton class, therefore protected constructor/destructor
			 */
			static BonkEnc			*instance;

							 BonkEnc();
			virtual				~BonkEnc();

			GUI::Window			*mainWnd;
			Statusbar			*mainWnd_statusbar;

			Hyperlink			*hyperlink;

			Config				*currentConfig;

			Bool				 dontUpdateInfo;
			Bool				 overwriteAll;
		public:
			static String			 appName;
			static String			 appLongName;
			static String			 version;
			static String			 shortVersion;
			static String			 cddbVersion;
			static String			 cddbMode;
			static String			 website;
			static String			 updatePath;

			static BoCA::I18n		*i18n;

			JobList				*joblist;

			Encoder				*encoder;

			/* Returns an existing instance of BonkEnc
			 */
			static BonkEnc			*Get();
	};
};

#endif
