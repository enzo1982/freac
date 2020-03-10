 /* fre:ac - free audio converter
  * Copyright (C) 2001-2019 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_COMPONENT
#define H_FREAC_COMPONENT

#include <smooth.h>
#include <boca.h>

using namespace smooth;

namespace freac
{
	class Component
	{
		private:
			static Array<Threads::Mutex *, Void *>	 mutexes;
			static Threads::Mutex			 managementMutex;

			Bool					 errorState;
			String					 errorString;
		protected:
			const BoCA::Config			*configuration;

			Void					 SetError(const String &, const String & = NIL, const String & = NIL, const String & = NIL);
		public:
			static Void				 FreeLockObjects();

								 Component(const BoCA::Config *);
			virtual					~Component();

			static Bool				 LockComponent(BoCA::AS::StreamComponent *);
			static Bool				 UnlockComponent(BoCA::AS::StreamComponent *);
		accessors:
			Bool					 GetErrorState() const	{ return errorState; }
			const String				&GetErrorString() const	{ return errorString; }
	};
};

#endif
