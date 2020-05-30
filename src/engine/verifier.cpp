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

#include <engine/verifier.h>

using namespace BoCA;
using namespace BoCA::AS;

freac::Verifier::Verifier(const BoCA::Config *iConfiguration) : Component(iConfiguration)
{
	verifier = NIL;
}

freac::Verifier::~Verifier()
{
	Destroy();
}

Bool freac::Verifier::Create(const Track &track)
{
	Registry	&boca = Registry::Get();

	/* Create verifier component.
	 */
	verifier = boca.CreateVerifierForTrack(track, configuration);

	if (verifier == NIL) return False;

	/* Activate verifier.
	 */
	verifier->SetConfiguration(configuration);
	verifier->SetAudioTrackInfo(track);
	verifier->Activate();

	return True;
}

Bool freac::Verifier::Destroy()
{
	if (verifier == NIL) return False;

	Registry	&boca = Registry::Get();

	verifier->Deactivate();

	SetErrorInfo(verifier->GetErrorState(), verifier->GetErrorString());

	boca.DeleteComponent(verifier);

	verifier = NIL;

	return True;
}

Int freac::Verifier::Process(Buffer<UnsignedByte> &buffer)
{
	if (verifier == NIL) return 0;

	return verifier->ProcessData(buffer);
}

Bool freac::Verifier::Verify()
{
	if (verifier == NIL) return True;

	Bool	 result = verifier->Verify();

	SetErrorInfo(verifier->GetErrorState(), verifier->GetErrorString());

	return result;
}
