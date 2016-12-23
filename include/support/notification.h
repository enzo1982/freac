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

#ifndef H_FREAC_NOTIFICATION
#define H_FREAC_NOTIFICATION

#include <smooth.h>

using namespace smooth;

namespace freac
{
	class Notification
	{
		private:
			/* Singleton class, therefore private constructor/destructor
			 */
			static Notification	*instance;

			Void			*privateData;

						 Notification();
						~Notification();
		public:
			Void			 ProcessSystemMessage(Int, Int, Int);

			/* Returns a new or existing instance of Notification
			 */
			static Notification	*Get();

			/* Destroys an existing instance of Notification
			 */
			static Void		 Free();
		signals:
			Signal0<Void>		 onDriveChange;

			Signal1<Void, Int>	 onDiscInsert;
			Signal1<Void, Int>	 onDiscRemove;
	};
};

#endif
