 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_JOBMANAGER_
#define _H_JOBMANAGER_

#include <smooth.h>
#include "job.h"

using namespace smooth;
using namespace smooth::Threads;

namespace BonkEnc
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
			Thread			*managerThread;
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
