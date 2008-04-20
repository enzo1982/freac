 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/job.h>

Array<BonkEnc::Job *>		 BonkEnc::Job::planned;
Array<BonkEnc::Job *>		 BonkEnc::Job::running;

Array<BonkEnc::Job *>		 BonkEnc::Job::all;

Signal0<Void>			 BonkEnc::Job::onChange;

Signal1<Void, BonkEnc::Job *>	 BonkEnc::Job::onPlanJob;
Signal1<Void, BonkEnc::Job *>	 BonkEnc::Job::onRunJob;
Signal1<Void, BonkEnc::Job *>	 BonkEnc::Job::onFinishJob;

BonkEnc::Job::Job() : ListEntry("Job")
{
	progress	= new Progressbar(Point(10, 20), Size(200, 0), OR_HORZ, PB_NOTEXT, 0, 1000, 0);

	Add(progress);

	SetHeight(50);

	planned.Add(this, GetHandle());
	all.Add(this, GetHandle());

	onChange.Emit();
	onPlanJob.Emit(this);
}

BonkEnc::Job::~Job()
{
	planned.Remove(GetHandle());
	running.Remove(GetHandle());

	all.Remove(GetHandle());

	onChange.Emit();
	onFinishJob.Emit(this);

	DeleteObject(progress);
}

Int BonkEnc::Job::Run()
{
	planned.Remove(GetHandle());
	running.Add(this, GetHandle());

	onChange.Emit();
	onRunJob.Emit(this);

	Perform();

	return Success();
}

Int BonkEnc::Job::SetProgress(Int nValue)
{
	progress->SetValue(nValue);

	return Success();
}
