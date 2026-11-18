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

#ifndef H_BOCA_CDDB_LOCAL
#define H_BOCA_CDDB_LOCAL

#include "cddb.h"
#include <boca/common/protocol.h>

namespace BoCA
{
	class CDDBLocal : public CDDB
	{
		private:
			String		 directory;

			Array<String>	 results;

			Bool		 QueryUnixDB(const String &);
			Bool		 QueryWinDB(const String &);
		public:
					 CDDBLocal(const String &);
			virtual		~CDDBLocal();

			Bool		 ConnectToServer();
			Int		 Query(const String &);
			Bool		 Read(const String &, UnsignedInt32, CDDBInfo &);
			Bool		 Submit(const CDDBInfo &);
			Bool		 Submit(CDDBInfo &);

			Bool		 Write(const CDDBInfo &);
			Bool		 CloseConnection();
	};
};

#endif
