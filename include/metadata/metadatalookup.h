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

#ifndef H_FREAC_METADATALOOKUP
#define H_FREAC_METADATALOOKUP

#include <smooth.h>
#include <boca.h>

using namespace smooth;

namespace freac
{
	/* Results of a metadata lookup.
	 */
	const Int	 LOOKUP_FOUND		= 0;
	const Int	 LOOKUP_NOT_FOUND	= 1;
	const Int	 LOOKUP_ERROR		= 2;
	const Int	 LOOKUP_CANCELED	= 3;
	const Int	 LOOKUP_NO_PROVIDER	= 4;

	/* CD frames per second, for TOC offsets.
	 */
	const Int	 FRAMES_PER_SECOND	= 75;

	/* Results of the selectMatch callback besides a match index.
	 */
	const Int	 MATCH_NONE		= -1;
	const Int	 MATCH_CANCELED		= -2;

	/* Looks up the metadata of one disc with the enabled metadata providers,
	 * in the order that the user set. This part has no user interface; call
	 * Lookup only from a worker thread.
	 */
	class MetadataLookup
	{
		private:
			Array<String>			 errors;
			Array<String>			 failedProviders;

			Bool				 autoSelect;

			Bool				 cancel;

			Threads::Mutex			 activeProviderMutex;
			BoCA::AS::MetadataComponent	*activeProvider;

			Void				 SetActiveProvider(BoCA::AS::MetadataComponent *);

			Bool				 IsFailedProvider(const String &) const;
			Void				 RecordProviderError(BoCA::AS::MetadataComponent *, const String &, BoCA::Protocol *);

			Int				 QueryProvider(BoCA::AS::MetadataComponent *, const String &, const BoCA::DiscMetadata &, Array<BoCA::MetadataMatch> &, Bool &, BoCA::Protocol *);
			Int				 ReadRecord(BoCA::AS::MetadataComponent *, const String &, const String &, BoCA::DiscMetadata &, BoCA::Protocol *);
		public:
			/* Creates a metadata provider; NIL if the component is missing
			 * or no metadata provider. Free it with DeleteComponent.
			 */
			static BoCA::AS::MetadataComponent *CreateProvider(const String &);

			static BoCA::DiscMetadata	 GetKnownData(const BoCA::Track &);
			static Array<String>		 GetProviders();
			static Int			 GetAvailableKeys(const BoCA::DiscMetadata &);

			static String			 GetDiscKey(const BoCA::Info &);
			static Bool			 IsSameDisc(const String &, const String &);
			static Bool			 IsSameQueuedDisc(const String &, const String &);
			static Int			 GetTOCIndex(const BoCA::Track &);

			/* Apply a record to a track or to the joblist tracks of its disc.
			 * With merge, the track keeps its provider fields and record ID;
			 * use this for records that the user edited from the track data.
			 */
			static Bool			 ApplyRecord(BoCA::Track &, const BoCA::DiscMetadata &, Bool merge = False);
			static Int			 ApplyToJoblist(const BoCA::DiscMetadata &, Bool, Bool merge = False);

			/* Records that were found or chosen in this session, so that
			 * adding a disc again does not repeat its lookup.
			 */
			static Bool			 GetCachedRecord(const BoCA::DiscMetadata &, BoCA::DiscMetadata &);
			static Void			 CacheRecord(const BoCA::DiscMetadata &);
			static Void			 RemoveCachedRecord(const BoCA::DiscMetadata &);

			/* True if a joblist track belongs to the disc, and the known
			 * data of the disc from that track.
			 */
			static Bool			 IsInJoblist(const BoCA::DiscMetadata &);
			static Bool			 GetJoblistKnownData(const BoCA::DiscMetadata &, BoCA::DiscMetadata &);

							 MetadataLookup();
							~MetadataLookup();

			/* Looks up known with providerIDs and, if it finds a match, fills
			 * record and returns LOOKUP_FOUND; record is valid only in that
			 * case. Call Lookup only once for each MetadataLookup object.
			 */
			Int				 Lookup(const BoCA::DiscMetadata &, const Array<String> &, BoCA::DiscMetadata &);

			/* Stops Lookup from another thread, also the running request of a
			 * provider. A Cancel before Lookup starts makes Lookup return
			 * LOOKUP_CANCELED at once.
			 */
			Void				 Cancel();

			/* Holds the errors of the latest Lookup call.
			 */
			const Array<String>		&GetErrors() const			{ return errors; }

			const Array<String>		&GetFailedProviders() const		{ return failedProviders; }
			Void				 SetFailedProviders(const Array<String> &);
		callbacks:
			/* Asks the user to choose a match for a query result. Returns the
			 * chosen index, MATCH_NONE if no match fits, which makes Lookup
			 * go on with the next provider, or MATCH_CANCELED, which makes
			 * Lookup return LOOKUP_CANCELED.
			 */
			Callback3<Int, const String &, const Array<BoCA::MetadataMatch> &, Bool>	 selectMatch;
		signals:
			Signal1<Void, const String &>							 onQueryProvider;
	};
};

#endif
