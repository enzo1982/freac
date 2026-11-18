 /* fre:ac - free audio converter
  * Copyright (C) 2026 Shen-Ta Hsieh
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <boca.h>

#include "cddbinfo.h"
#include "cddblocal.h"
#include "cddbremote.h"

BoCA_BEGIN_COMPONENT(MetadataCDDB)

namespace BoCA
{
	class MetadataCDDB : public CS::MetadataComponent
	{
		private:
			enum Source { SourceNone, SourceCache, SourceLocal, SourceRemote };

			ConfigLayer		*configLayer;

			DiscMetadata		 known;
			String			 queryString;

			Source			 source;
			Bool			 fuzzy;

			CDDBInfo		 cached;

			CDDBLocal		*local;
			CDDBRemote		*remote;

			Int			 QueryDatabase(CDDB &, Array<MetadataMatch> &);
		public:
			static const String	&GetComponentSpecs();

						 MetadataCDDB();
						~MetadataCDDB();

			ConfigLayer		*GetConfigurationLayer();

			Int			 GetQueryKeys() const;

			Int			 Query(const DiscMetadata &, Array<MetadataMatch> &);
			Bool			 Read(const String &, DiscMetadata &);

			Int			 GetSubmitData() const;
			Int			 GetSubmitMethod() const;
			Bool			 Submit(const DiscMetadata &);
	};
};

BoCA_DEFINE_METADATA_COMPONENT(MetadataCDDB)

BoCA_END_COMPONENT(MetadataCDDB)
