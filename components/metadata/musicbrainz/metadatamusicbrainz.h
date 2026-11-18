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

#include "discid.h"

BoCA_BEGIN_COMPONENT(MetadataMusicBrainz)

namespace BoCA
{
	class MetadataMusicBrainz : public CS::MetadataComponent
	{
		private:
			ConfigLayer		*configLayer;

			DiscMetadata		 known;

			MusicBrainzTOC		 toc;
			Bool			 haveTOC;

			MusicBrainzTOC		 dataFirstTOC;		// For the records in dataFirstRecords.
			Array<String>		 dataFirstRecords;

			XML::Document		*response;

			String			 GetServer() const;
		public:
			static const String	&GetComponentSpecs();

						 MetadataMusicBrainz();
						~MetadataMusicBrainz();

			ConfigLayer		*GetConfigurationLayer();

			Int			 GetQueryKeys() const;

			Int			 Query(const DiscMetadata &, Array<MetadataMatch> &);
			Bool			 Read(const String &, DiscMetadata &);

			String			 NormalizeRecordID(const String &);

			Int			 GetSubmitData() const;
			Int			 GetSubmitMethod() const;
			Bool			 Submit(const DiscMetadata &);
	};
};

BoCA_DEFINE_METADATA_COMPONENT(MetadataMusicBrainz)

BoCA_END_COMPONENT(MetadataMusicBrainz)
