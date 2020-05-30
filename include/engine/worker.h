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

#ifndef H_FREAC_CONVERT_WORKER
#define H_FREAC_CONVERT_WORKER

#include <smooth.h>
#include <boca.h>

#include "converter.h"

namespace freac
{
	class Decoder;
	class Verifier;
	class Processor;
	class Encoder;

	class ConvertWorker : public Threads::Thread
	{
		protected:
			const BoCA::Config				*configuration;

			Int						 conversionID;

			String						 logName;

			Int						 numThreads;

			BoCA::Track					 trackToConvert;
			UnsignedInt64					 trackStartTicks;
			Int64						 trackPosition;

			String						 decoderName;
			String						 encoderName;
			ConversionStep					 conversionStep;

			Bool						 idle;
			Bool						 waiting;
			Bool						 error;

			Bool						 pause;
			Bool						 cancel;
			Bool						 quit;

			Int						 Perform();
			Int64						 Loop(Decoder *, Verifier *, BoCA::FormatConverter *, Processor *, Encoder *);

			Void						 VerifyInput(const String &, Verifier *);

			Void						 LogConversionStart(Decoder *, const String &, const String & = NIL) const;
			Void						 LogConversionEnd(const String &, Int64, UnsignedInt32, const String &, const String &) const;

			virtual Int					 Convert();
		public:
									 ConvertWorker(const BoCA::Config *, Int);
			virtual						~ConvertWorker();

			Int						 Pause(Bool);
			Int						 Cancel();
			Int						 Quit();
		accessors:
			Bool						 IsIdle() const					{ return idle; }
			Bool						 IsWaiting() const				{ return waiting; }
			Bool						 IsError() const				{ return error; }

			Void						 SetLogName(const String &nLogName)		{ logName = nLogName; }

			Void						 SetNumberOfThreads(Int nNumThreads)		{ numThreads = nNumThreads; }

			const BoCA::Track				&GetTrackToConvert() const			{ return trackToConvert; }
			Void						 SetTrackToConvert(const BoCA::Track &);

			const String					&GetDecoderName() const				{ return decoderName; }
			const String					&GetEncoderName() const				{ return encoderName; }
			ConversionStep					 GetConversionStep() const			{ return conversionStep; }

			UnsignedInt64					 GetTrackStartTicks() const			{ return trackStartTicks; }
			Int64						 GetTrackPosition() const			{ return trackPosition; }
		signals:
			Signal2<Void, const BoCA::Track &, Bool>	 onFinishTrack;
			Signal2<Void, const BoCA::Track &,
				      const BoCA::Track &>		 onFixTotalSamples;

			Signal1<Void, const String &>			 onReportError;
			Signal1<Void, const String &>			 onReportWarning;
	};
};

#endif
