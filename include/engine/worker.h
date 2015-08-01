 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CONVERT_WORKER
#define H_FREAC_CONVERT_WORKER

#include "converter.h"

#include "decoder.h"
#include "encoder.h"
#include "verifier.h"

namespace BonkEnc
{
	class ConvertWorker : public Threads::Thread
	{
		protected:
			const BoCA::Config			*configuration;

			BoCA::Track				 trackToConvert;
			UnsignedInt64				 trackStartTicks;
			Int64					 trackPosition;

			String					 decoderName;
			ConversionStep				 conversionStep;

			Bool					 idle;
			Bool					 waiting;

			Bool					 pause;
			Bool					 cancel;
			Bool					 quit;

			Int					 Perform();
			Int64					 Loop(Decoder *, Verifier *, Encoder *);

			virtual Int				 Convert();
		public:
								 ConvertWorker(const BoCA::Config *);
			virtual					~ConvertWorker();

			Int					 Pause(Bool);
			Int					 Cancel();
			Int					 Quit();
		accessors:
			Bool					 IsIdle() const				      { return idle; }
			Bool					 IsWaiting() const			      { return waiting; }

			const BoCA::Track			&GetTrackToConvert() const		      { return trackToConvert; }
			Void					 SetTrackToConvert(const BoCA::Track &);

			const String				&GetDecoderName() const			      { return decoderName; }
			ConversionStep				 GetConversionStep() const		      { return conversionStep; }

			UnsignedInt64				 GetTrackStartTicks() const		      { return trackStartTicks; }
			Int64					 GetTrackPosition() const		      { return trackPosition; }
		signals:
			Signal1<Void, const BoCA::Track &>	 onFinishTrack;
			Signal2<Void, const BoCA::Track &,
				      const BoCA::Track &>	 onFixTotalSamples;

			Signal1<Void, String>			 onReportError;
			Signal1<Void, String>			 onReportWarning;
	};
};

#endif
