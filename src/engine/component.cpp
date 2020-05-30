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

#include <engine/component.h>

using namespace BoCA;
using namespace BoCA::AS;

Array<Threads::Mutex *, Void *>	 freac::Component::mutexes;
Threads::Mutex			 freac::Component::managementMutex;

freac::Component::Component(const BoCA::Config *iConfiguration)
{
	configuration = iConfiguration;

	errorState    = False;
}

freac::Component::~Component()
{
}

Void freac::Component::SetErrorInfo(Bool error, const String &message, const String &replace1, const String &replace2, const String &replace3)
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	errorState  = error;
	errorString = i18n->TranslateString(message, "Errors").Replace("%1", replace1)
							      .Replace("%2", replace2)
							      .Replace("%3", replace3);
}

Bool freac::Component::LockComponent(StreamComponent *component)
{
	if (component->IsThreadSafe()) return True;

	/* Lock component if it's not thread safe.
	 */
	managementMutex.Lock();

	if (mutexes.Get(component->GetID().ComputeCRC32()) == NIL) mutexes.Add(new Threads::Mutex(), component->GetID().ComputeCRC32());

	managementMutex.Release();

	mutexes.Get(component->GetID().ComputeCRC32())->Lock();

	return True;
}

Bool freac::Component::UnlockComponent(StreamComponent *component)
{
	if (component->IsThreadSafe()) return True;

	/* Unlock component if it's not thread safe.
	 */
	mutexes.Get(component->GetID().ComputeCRC32())->Release();

	return True;
}

Void freac::Component::FreeLockObjects()
{
	foreach (Threads::Mutex *mutex, mutexes) delete mutex;

	mutexes.RemoveAll();
}
