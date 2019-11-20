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

#include <engine/processor.h>

#include <config.h>

using namespace BoCA;
using namespace BoCA::AS;

freac::Processor::Processor(const BoCA::Config *iConfiguration) : Component(iConfiguration)
{
}

freac::Processor::~Processor()
{
	Destroy();
}

Bool freac::Processor::Create(const Track &track)
{
	Registry	&boca = Registry::Get();

	format = track.GetFormat();

	/* Get config values.
	 */
	Bool	 enableProcessing = configuration->GetIntValue(Config::CategoryProcessingID, Config::ProcessingEnableProcessingID, Config::ProcessingEnableProcessingDefault);

	if (!enableProcessing) return True;

	/* Create DSP components.
	 */
	const Array<String>	&components = configuration->GetStringValue(Config::CategoryProcessingID, Config::ProcessingComponentsID, Config::ProcessingComponentsDefault).Explode(",");
	Track			 dspTrack   = track;

	foreach (const String &component, components)
	{
		AS::DSPComponent	*dsp = (AS::DSPComponent *) boca.CreateComponentByID(component);

		if (dsp == NIL)
		{
			Destroy();

			return False;
		}

		/* Lock processor if it's not thread safe.
		 */
		LockComponent(dsp);

		/* Activate DSP component.
		 */
		dsp->SetConfiguration(configuration);
		dsp->SetAudioTrackInfo(dspTrack);

		if (dsp->Activate() == False)
		{
			SetError("Could not set up DSP processor: %1\n\nError: %2", dsp->GetName(), dsp->GetErrorString());

			UnlockComponent(dsp);

			boca.DeleteComponent(dsp);

			Destroy();

			return False;
		}

		format = dsp->GetFormatInfo();

		dspTrack.SetFormat(format);

		dsps.Add(dsp);
	}

	return True;
}

Bool freac::Processor::Destroy()
{
	Registry	&boca = Registry::Get();

	foreach (AS::DSPComponent *dsp, dsps)
	{
		dsp->Deactivate();

		if (dsp->GetErrorState()) SetError("Error: %1", dsp->GetErrorString());

		UnlockComponent(dsp);

		boca.DeleteComponent(dsp);
	}

	dsps.RemoveAll();

	return True;
}

Int freac::Processor::Transform(Buffer<UnsignedByte> &buffer)
{
	/* Call transform for every DSP component.
	 */
	foreach (AS::DSPComponent *dsp, dsps) dsp->TransformData(buffer);

	return buffer.Size();
}

Int freac::Processor::Finish(Buffer<UnsignedByte> &buffer)
{
	if (buffer.Size() != 0) return -1;

	foreach (AS::DSPComponent *dsp, dsps)
	{
		/* Transform data already in return buffer.
		 */
		if (buffer.Size() != 0) dsp->TransformData(buffer);

		/* Flush DSP component.
		 */
		Buffer<UnsignedByte>	 flush;

		dsp->Flush(flush);

		/* Append remaining data to return buffer.
		 */
		buffer.Resize(buffer.Size() + flush.Size());

		memcpy(buffer + buffer.Size() - flush.Size(), flush, flush.Size());
	}

	return buffer.Size();
}
