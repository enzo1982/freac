 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2007 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CDDB_LOCAL_
#define _H_CDDB_LOCAL_

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
