 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CDDB_BATCH_QUERIES_
#define _H_CDDB_BATCH_QUERIES_

#include <cddb/cddb.h>

namespace BonkEnc
{
	class CDDBBatchQueries
	{
		private:
			Config			*config;

			Array<String>		 queries;

			Bool			 ReadEntries();
			Bool			 SaveEntries();
		public:
						 CDDBBatchQueries(Config *);
			virtual			~CDDBBatchQueries();

			const Array<String>	&GetQueries();

			Bool			 AddQuery(const String &);
			Bool			 DeleteQuery(Int);

			Bool			 Query(Int);
			Bool			 QueryAll();
	};
};

#endif
