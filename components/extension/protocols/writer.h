 /* fre:ac - free audio converter
  * Copyright (C) 2001-2021 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_PROTOCOLS_WRITER
#define H_PROTOCOLS_WRITER

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::IO;

namespace BoCA
{
	class ProtocolData
	{
		public:
							 ProtocolData();
							~ProtocolData();

			Int				 conversionID;

			String				 fileHeader;

			String				 fileName;
			OutStream			*fileStream;

			String				 copyName;
			OutStream			*copyStream;

			const Array<Track>		*trackList;
			String				 singleFileName;
	};

	class ProtocolWriter
	{
		private:
			static String			 FormatHeader(const Protocol *);
			static Void			 SaveProtocol(const Protocol *, OutStream *);

			static Array<ProtocolData>	 protocolData;
			static Int			 instanceCount;

			Void				 OutputMessage(OutStream *, const String &);
		public:
			static Void			 SaveProtocol(const Protocol *, const String &);

							 ProtocolWriter();
							~ProtocolWriter();

			String				 GetProtocolFileName(const Protocol *);
			String				 GetAdditionalCopyName(const Protocol *);
		slots:
			Void				 OnUpdateProtocolList();
			Void				 OnUpdateProtocol(const String &);

			Void				 OnStartConversion(Int, const Array<Track> &);
			Void				 OnSingleFileConversion(Int, const String &);
			Void				 OnFinishConversion(Int);
	};
};

#endif
