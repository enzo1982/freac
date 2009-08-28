 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_CDDB_BATCH
#define H_BONKENC_CDDB_BATCH

#include <cddb/cddb.h>

namespace BonkEnc
{
	class CDDBBatch : public CDDB
	{
		private:
			Array<String>		 queries;
			Array<CDDBInfo>		 submits;

			Bool			 ReadEntries();
			Bool			 ReadEntriesXML(XML::Document *);

			Bool			 SaveEntries();

			Bool			 ConnectToServer()			{ return True; }
			Int			 Query(const String &)			{ return QUERY_RESULT_NONE; }
			Bool			 Read(const String &, Int, CDDBInfo &)	{ return False; }
			Bool			 CloseConnection()			{ return True; }
		public:
						 CDDBBatch();
			virtual			~CDDBBatch();

			const Array<String>	&GetQueries()				{ return queries; }
			const Array<CDDBInfo>	&GetSubmits()				{ return submits; }

			Bool			 AddQuery(const String &);
			Bool			 DeleteQuery(Int);

			Bool			 AddSubmit(const CDDBInfo &);
			Bool			 DeleteSubmit(const CDDBInfo &);

			Int			 Query(Int);
			Bool			 QueryAll();

			Bool			 Submit(const CDDBInfo &);
			Bool			 SubmitAll();
	};
};

#endif
