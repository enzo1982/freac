 /* fre:ac - free audio converter
  * Copyright (C) 2026 Shen-Ta Hsieh
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BOCA_MUSICBRAINZ_NETWORK
#define H_BOCA_MUSICBRAINZ_NETWORK

#include <boca.h>
#include <smooth/net/protocols/http.h>

namespace BoCA
{
	/* A flag that stops a request when it becomes True.
	 */
	class MusicBrainzCancelFlag
	{
		private:
			const Bool		*flag;
		public:
						 MusicBrainzCancelFlag(const Bool *iFlag) : flag(iFlag) { }
		slots:
			Bool			 IsSet()	{ return flag != NIL && *flag; }
	};

	/* HTTP requests to a MusicBrainz server.
	 */
	class MusicBrainzNetwork
	{
		private:
			static Threads::Mutex	 mutex;
			static UnsignedInt64	 lastRequestTime;

			static Void		 ApplyProxy(const Config *, Net::Protocols::HTTP &);
		public:
			static String		 NormalizeServerURL(const String &);

			static String		 GetHost(const String &);
			static String		 GetUserAgent(const String &, const String &);

			static Bool		 IsRateLimited(const String &);

			static Bool		 Pause(Int, const Bool *);

			static UnsignedInt64	 TicksToMilliseconds(UnsignedInt64, UnsignedInt, UnsignedInt);
			static Bool		 WaitForTurn(const String &, const Bool * = NIL);

			static Int		 Get(const Config *, const String &, Buffer<UnsignedByte> &, const Bool * = NIL);
	};
};

#endif
