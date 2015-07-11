 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <engine/verifier.h>

using namespace BoCA;
using namespace BoCA::AS;

BonkEnc::Verifier::Verifier(const BoCA::Config *iConfiguration)
{
	configuration = iConfiguration;

	verifier      = NIL;
}

BonkEnc::Verifier::~Verifier()
{
	Destroy();
}

Bool BonkEnc::Verifier::Create(const Track &track)
{
	Registry	&boca = Registry::Get();

	/* Create verifier component.
	 */
	verifier = boca.CreateVerifierForTrack(track);

	if (verifier == NIL) return False;

	/* Activate verifier.
	 */
	verifier->SetConfiguration(configuration);
	verifier->SetAudioTrackInfo(track);
	verifier->Activate();

	return True;
}

Bool BonkEnc::Verifier::Destroy()
{
	if (verifier == NIL) return False;

	Registry	&boca = Registry::Get();

	verifier->Deactivate();

	if (verifier->GetErrorState()) BoCA::Utilities::ErrorMessage("Error: %1", verifier->GetErrorString());

	boca.DeleteComponent(verifier);

	verifier = NIL;

	return True;
}

Int BonkEnc::Verifier::Process(Buffer<UnsignedByte> &buffer)
{
	if (verifier == NIL) return 0;

	return verifier->ProcessData(buffer);
}

Bool BonkEnc::Verifier::Verify()
{
	if (verifier == NIL) return True;

	return verifier->Verify();
}
