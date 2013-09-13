 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CDDB_LOCAL
#define H_FREAC_CDDB_LOCAL

#include <cddb/cddb.h>

namespace BonkEnc
{
	class BEEXPORT CDDBLocal : public CDDB
	{
		private:
			Array<String>	 results;

			Bool		 QueryUnixDB(Int);
			Bool		 QueryWinDB(Int);
		public:
					 CDDBLocal(Config *);
			virtual		~CDDBLocal();

			Bool		 ConnectToServer();
			Int		 Query(Int);
			Int		 Query(const String &);
			Bool		 Read(const String &, Int, CDDBInfo &);
			Bool		 Submit(const CDDBInfo &);
			Bool		 CloseConnection();
	};
};

#endif
