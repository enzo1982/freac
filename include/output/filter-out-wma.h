 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_FILTER_OUT_WMA
#define H_FREAC_FILTER_OUT_WMA

#include "outputfilter.h"
#include <3rdparty/wmsdk/wmsdk.h>

namespace BonkEnc
{
	class FilterOutWMA : public OutputFilter
	{
		private:
			IWMWriter		*m_pWriter;
			IWMWriterAdvanced	*m_pWriterAdvanced;

			IWMWriterFileSink	*m_pWriterFileSink;

			IWMProfileManager	*m_pProfileManager;
			IWMProfile		*m_pProfile;

			IWMStreamConfig		*m_pStreamConfig;

			Int64			 samplesWritten;

			Buffer<signed short>	 samplesBuffer;

			Int			 GetDefaultCodec(IWMCodecInfo3 *);
			IWMStreamConfig		*GetBestCodecFormat(IWMCodecInfo3 *, DWORD, const Track *);

			Bool			 SetInputFormat(IWMWriter *, const Track *);
		public:
						 FilterOutWMA(Config *, Track *);
						~FilterOutWMA();

			Bool			 Activate();
			Bool			 Deactivate();

			Int			 WriteData(Buffer<UnsignedByte> &, Int);
	};
};

#endif
