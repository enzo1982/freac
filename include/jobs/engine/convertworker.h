 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_JOB_CONVERT_WORKER
#define H_FREAC_JOB_CONVERT_WORKER

#include "../../engine/encoder.h"

namespace BonkEnc
{
	const Int	 CONVERTER_STEP_ON_THE_FLY = 0;
	const Int	 CONVERTER_STEP_DECODE	   = 1;
	const Int	 CONVERTER_STEP_ENCODE	   = 2;

	class JobConvertWorker : public Threads::Thread
	{
		private:
			BoCA::Track				 trackToConvert;
			UnsignedInt64				 trackStartTicks;
			Int64					 trackPosition;

			Encoder					*singleFileEncoder;
			Int64					 encodedSamples;

			String					 decoderName;
			Int					 conversionStep;

			Bool					 idle;
			Bool					 waiting;

			Bool					 pause;
			Bool					 cancel;
			Bool					 quit;

			Int					 Perform();

			Int					 Convert(const BoCA::Track &);
		public:
								 JobConvertWorker();
								~JobConvertWorker();

			Int					 Pause(Bool);
			Int					 Cancel();
			Int					 Quit();
		accessors:
			Bool					 IsIdle() const				      { return idle; }
			Bool					 IsWaiting() const			      { return waiting; }

			Void					 SetSingleFileEncoder(Encoder *nEncoder)      { singleFileEncoder = nEncoder; }

			const BoCA::Track			&GetTrackToConvert() const		      { return trackToConvert; }
			Void					 SetTrackToConvert(const BoCA::Track &nTrack) { trackToConvert = nTrack; idle = False; waiting = True; }

			const String				&GetDecoderName() const			      { return decoderName; }
			Int					 GetConversionStep() const		      { return conversionStep; }

			UnsignedInt64				 GetTrackStartTicks() const		      { return trackStartTicks; }
			Int64					 GetTrackPosition() const		      { return trackPosition; }
		signals:
			Signal2<Void, const BoCA::Track &,
				      const BoCA::Track &>	 onFixTotalSamples;
			Signal1<Void, const BoCA::Track &>	 onFinishTrack;
	};
};

#endif
