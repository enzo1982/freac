 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CDDB_REMOTE
#define H_FREAC_CDDB_REMOTE

#include <cddb/cddb.h>
#include <boca/common/protocol.h>

using namespace smooth::IO;

namespace freac
{
	class CDDBRemote : public CDDB
	{
		private:
			Bool			 connected;

			Buffer<char>		 hostNameBuffer;
			Buffer<UnsignedByte>	 httpResultBuffer;

			Array<CDDBInfo>		 readEntries;

			Driver			*socket;
			InStream		*in;
			OutStream		*out;

			BoCA::Protocol		*protocol;

			String			 SendCommand(const String &);
		public:
						 CDDBRemote();
			virtual			~CDDBRemote();

			Bool			 ConnectToServer();
			Int			 Query(const String &);
			Bool			 Read(const String &, Int, CDDBInfo &);
			Bool			 Submit(const CDDBInfo &);
			Bool			 CloseConnection();
	};
};

#endif
