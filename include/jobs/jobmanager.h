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

#ifndef H_FREAC_JOBMANAGER
#define H_FREAC_JOBMANAGER

#include "job.h"

using namespace smooth;

namespace freac
{
	class JobManager
	{
		private:
			/* Singleton class, therefore private constructor/destructor
			 */
			static JobManager	*instance;

						 JobManager();
			virtual			~JobManager();

			Bool			 exitThread;
			Threads::Thread		*managerThread;
		public:
			/* Methods to start and quit the JobManager
			 */
			static Error		 Start();
			static Error		 Quit();
		slots:
			Void			 ManagerThread();

			Void			 OnFinishJob(Job *);
	};
};

#endif
