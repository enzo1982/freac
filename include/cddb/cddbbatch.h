 /* fre:ac - free audio converter
  * Copyright (C) 2001-2020 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CDDB_BATCH
#define H_FREAC_CDDB_BATCH

#include <cddb/cddb.h>

namespace freac
{
	class CDDBBatch : public CDDB
	{
		private:
			Array<String>		 queries;
			Array<CDDBInfo>		 submits;

			Array<String>		 queryErrors;

			Bool			 ReadEntries();
			Bool			 ReadEntriesXML(XML::Document *);

			Bool			 SaveEntries();

			Bool			 ConnectToServer()					{ return True; }
			Int			 Query(const String &)					{ return QUERY_RESULT_NONE; }
			Bool			 Read(const String &, UnsignedInt32, CDDBInfo &)	{ return False; }
			Bool			 CloseConnection()					{ return True; }
		public:
						 CDDBBatch();
			virtual			~CDDBBatch();

			const Array<String>	&GetQueries() const					{ return queries; }
			const Array<CDDBInfo>	&GetSubmits() const					{ return submits; }

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
