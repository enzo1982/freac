 /* fre:ac - free audio converter
  * Copyright (C) 2001-2026 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BOCA_CDDB_REMOTE
#define H_BOCA_CDDB_REMOTE

#include "cddb.h"
#include <boca/common/protocol.h>

using namespace smooth::IO;

namespace BoCA
{
	class CDDBRemote : public CDDB
	{
		private:
			const Config		*config;
			const Bool		*cancel;

			Bool			 connected;

			Buffer<UnsignedByte>	 httpResultBuffer;

			Array<CDDBInfo>		 readEntries;

			Driver			*socket;
			InStream		*in;
			OutStream		*out;

			String			 SendCommand(const String &);
		public:
						 CDDBRemote(const Config *, const Bool * = NIL);
			virtual			~CDDBRemote();

			Bool			 ConnectToServer();
			Int			 Query(const String &);
			Bool			 Read(const String &, UnsignedInt32, CDDBInfo &);
			Bool			 Submit(const CDDBInfo &);
			Bool			 CloseConnection();
		slots:
			Bool			 IsCanceled()	{ return cancel != NIL && *cancel; }
	};
};

#endif
