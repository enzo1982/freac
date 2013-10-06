 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CMDMAIN
#define H_FREAC_CMDMAIN

#include <smooth.h>
#include <boca.h>
#include "bonkenc.h"

#ifdef __WIN32__
#	define DLLEXPORT __declspec (dllexport)
#else
#	define DLLEXPORT __attribute__ ((visibility ("default")))
#endif

using namespace smooth;
using namespace smooth::GUI;

extern "C"
{
	Int DLLEXPORT StartConsole(const Array<String> &);
}

namespace BonkEnc
{
	class BonkEncCommandline : public BonkEnc
	{
		private:
			/* Singleton class, therefore protected constructor/destructor
			 */
							 BonkEncCommandline(const Array<String> &);
							~BonkEncCommandline();

			const Array<String>		&args;

			Bool				 ScanForParameter(const String &, String *);
			Void				 ScanForFiles(Array<String> *);
			Bool				 TracksToFiles(const String &, Array<String> *);
		public:
			Void				 ShowHelp(const String &);

			/* Returns a new or existing instance of BonkEncCommandline
			 */
			static BonkEncCommandline	*Get(const Array<String> &);

			/* Destroys an existing instance of BonkEncCommandline
			 */
			static Void			 Free();
		slots:
			Void				 OnEncodeTrack(const BoCA::Track &, const String &, Int);
	};
};

#endif
