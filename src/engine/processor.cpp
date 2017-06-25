 /* fre:ac - free audio converter
  * Copyright (C) 2001-2017 Robert Kausch <robert.kausch@freac.org>
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

freac::Processor::Processor(const BoCA::Config *iConfiguration)
{
	configuration = iConfiguration;
}

freac::Processor::~Processor()
{
	Destroy();
}

Bool freac::Processor::Create(const Track &nTrack)
{
	Registry	&boca  = Registry::Get();
	Track		 track = nTrack;

	format = track.GetFormat();

	/* Create DSP components.
	 */
	const Array<String>	&components = configuration->GetStringValue(Config::CategoryProcessingID, Config::ProcessingComponentsID, Config::ProcessingComponentsDefault).Explode(",");

	foreach (const String &component, components)
	{
		AS::DSPComponent	*dsp = (AS::DSPComponent *) boca.CreateComponentByID(component);

		if (dsp == NIL)
		{
			Destroy();

			return False;
		}

		/* Activate DSP component.
		 */
		dsp->SetConfiguration(configuration);
		dsp->SetAudioTrackInfo(track);
		dsp->Activate();

		format = dsp->GetFormatInfo();

		track.SetFormat(format);

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

		if (dsp->GetErrorState()) BoCA::Utilities::ErrorMessage("Error: %1", dsp->GetErrorString());

		boca.DeleteComponent(dsp);
	}

	dsps.RemoveAll();

	return True;
}

const Format &freac::Processor::GetFormatInfo() const
{
	return format;
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
