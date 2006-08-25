 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CDDB_BATCH_
#define _H_CDDB_BATCH_

#include <cddb/cddb.h>

namespace BonkEnc
{
	class BEEXPORT CDDBBatch
	{
		private:
			Config	*config;
		public:
				 CDDBBatch(Config *);
			virtual	~CDDBBatch();

			Bool	 AddEntry(CDDBInfo *);

			Bool	 SubmitAll();
	};
};

#endif
