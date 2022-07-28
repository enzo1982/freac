 /* fre:ac - free audio converter
  * Copyright (C) 2001-2022 Robert Kausch <robert.kausch@freac.org>
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
			Bool			 firstFile;
			Bool			 stopped;
			Int			 errorCode;

			BoCA::Picture		 coverFront;
			BoCA::Picture		 coverBack;

			Bool			 ScanForProgramOption(const String &, String * = NIL);
			Bool			 ScanForEncoderOption(const String &, String * = NIL);
			Void			 ScanForFiles(Array<String> *);

			Bool			 ParamHasArguments(const String &, Bool);
			Bool			 TracksToFiles(const String &, Array<String> *);

			String			 GetAbsolutePathName(const String &path) const;

			BoCA::Picture		 LoadCoverArt(const String &, Int);

			Bool			 AddToJoblist(const Array<String> &, Bool);

			Bool			 SetConfigDefaults(BoCA::Config *, Bool);
			Bool			 SetEncoderDefaults(BoCA::Config *, Bool);
		public:
			Void			 ShowHelp(const String & = NIL);

			Void			 Stop();

			/* Returns a new or existing instance of freacCommandline
			 */
			static freacCommandline	*Get(const Array<String> & = Array<String>());

			/* Destroys an existing instance of freacCommandline
			 */
			static Void		 Free();
		accessors:
			Int			 GetErrorCode() { return errorCode; }
		slots:
			Void			 OnEncodeTrack(const BoCA::Track &, const String &, const String &, ConversionStep);
			Void			 OnFinishEncoding(Bool);
	};
};

#endif
