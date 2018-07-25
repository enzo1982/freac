 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CMDMAIN
#define H_FREAC_CMDMAIN

#include <smooth.h>
#include <boca.h>

#include "freac.h"

#include "engine/converter.h"

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

namespace freac
{
	class freacCommandline : public freac
	{
		private:
			/* Singleton class, therefore protected constructor/destructor
			 */
						 freacCommandline(const Array<String> &);
						~freacCommandline();

			const Array<String>	&args;

			String			 encoderID;

			Bool			 ScanForProgramOption(const String &, String * = NIL);
			Bool			 ScanForEncoderOption(const String &, String * = NIL);
			Void			 ScanForFiles(Array<String> *);

			Bool			 ParamHasArguments(const String &, Bool);
			Bool			 TracksToFiles(const String &, Array<String> *);
		public:
			Void			 ShowHelp(const String &);

			/* Returns a new or existing instance of freacCommandline
			 */
			static freacCommandline	*Get(const Array<String> &);

			/* Destroys an existing instance of freacCommandline
			 */
			static Void		 Free();
		slots:
			Void			 OnEncodeTrack(const BoCA::Track &, const String &, ConversionStep);
	};
};

#endif
