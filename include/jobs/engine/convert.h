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

#ifndef H_FREAC_JOB_CONVERT
#define H_FREAC_JOB_CONVERT

#include <boca.h>

#include "../job.h"
#include "../../engine/converter.h"

namespace freac
{
	class JobConvert : public Job
	{
		private:
			static Int					 conversionCount;

			static Array<JobConvert *>			 conversionJobs;
			static Bool					 conversionPaused;

			Int						 conversionID;
			String						 logName;

			Array<BoCA::Track>				 tracks;
			Array<Int>					 trackActions;

			Bool						 autoRip;

			Bool						 skipTrack;
			Bool						 stopConversion;

			String						 singleOutFile;
			BoCA::Format					 singleTrackSampleFormat;

			Void						 UpdateTrackProgress(Int, Int);
			Void						 UpdateTotalProgress(Int, Int);

			Int						 GetNumberOfWorkers(Int) const;

			BoCA::Format					 GetSingleTrackSampleFormat() const;
			BoCA::Track					 ConsolidateTrackInfo();

			Void						 LogSettings(const String &, Int) const;
			Void						 LogCDInfo() const;
		public:
			static Bool					 IsConverting()	{ return conversionJobs.Length(); }
			static Bool					 IsPaused()	{ return conversionPaused;  }

			static Void					 Pause()	{ if (IsConverting()) conversionPaused = True;  }
			static Void					 Resume()	{ if (IsConverting()) conversionPaused = False; }

			static Void					 Skip();
			static Void					 Stop();

									 JobConvert(const Array<BoCA::Track> &, Bool = False);
			virtual						~JobConvert();

			virtual Error					 Precheck();
		signals:
			static Signal0<Void>				 onStartEncoding;
			static Signal1<Void, Bool>			 onFinishEncoding;

			static Signal4<Void, const BoCA::Track &,
					     const String &,
					     const String &,
					     ConversionStep>		 onEncodeTrack;

			static Signal2<Void, Int, Int>			 onTrackProgress;
			static Signal2<Void, Int, Int>			 onTotalProgress;
		slots:
			Void						 OnWorkerReportError(const String &);
			Void						 OnWorkerReportWarning(const String &);

			virtual Error					 Perform();
	};
};

#endif
