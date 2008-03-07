 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FILTER_IN_VORBIS_
#define _H_FILTER_IN_VORBIS_

#include "inputfilter.h"
#include <3rdparty/vorbis/vorbisenc.h>

namespace BonkEnc
{
	class BEEXPORT FilterInVORBIS : public InputFilter
	{
		private:
			ogg_sync_state		 oy;
			ogg_stream_state	 os;
			ogg_page		 og;
			ogg_packet		 op;

			vorbis_info		 vi;
			vorbis_comment		 vc;
			vorbis_dsp_state	 vd;
			vorbis_block		 vb;

			char			*buffer;

			Buffer<unsigned char>	 backBuffer;
		public:
						 FilterInVORBIS(Config *, Track *);
						~FilterInVORBIS();

			Bool			 Activate();
			Bool			 Deactivate();

			Int			 ReadData(Buffer<UnsignedByte> &, Int);

			Track			*GetFileInfo(const String &);
	};
};

#endif
