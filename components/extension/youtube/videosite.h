 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_YOUTUBE_VIDEOSITE
#define H_YOUTUBE_VIDEOSITE

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA;

#include <smooth-js/v8.h>

namespace BoCA
{
	class Metadata
	{
		public:
			String	 title;
			String	 description;

			String	 date;
			String	 thumbnail;

			String	 uploader;
	};

	class VideoSite
	{
		private:
			static String			 ReplaceInnerHTML(const String &);

			String				 name;
			String				 version;

			Array<String>			 decoders;

			String				 script;

			v8::Isolate			*isolate;
			v8::Persistent<v8::Context>	 context;

			Bool				 CreateScriptContext();
			Bool				 DestroyScriptContext();

			Int				 ParseXML(const String &);
		public:
							 VideoSite(const String &);
							~VideoSite();

			Bool				 IsSane();

			Bool				 CanHandleURL(const String &);

			String				 GetVideoURL(const String &);
			String				 GetDecoderID(const String &);

			Metadata			 QueryMetadata(const String &);
		accessors:
			const String			&GetName() const	{ return name; }
			const String			&GetVersion() const	{ return version; }

			const Array<String>		&GetDecoders() const	{ return decoders; }
	};
};

#endif
