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

#ifndef H_FREAC_METADATA_QUERY
#define H_FREAC_METADATA_QUERY

#include <smooth.h>
#include <boca.h>

#include <metadata/metadatalookup.h>

using namespace smooth;
using namespace smooth::GUI;

namespace freac
{
	/* Runs a metadata lookup on a worker thread behind a progress window.
	 */
	class MetadataQueryDlg : public Dialogs::Dialog
	{
		private:
			Window			*mainWnd;
			Titlebar		*mainWnd_titlebar;

			Text			*text_status;
			Progressbar		*prog_status;
			Button			*btn_cancel;

			MetadataLookup		 lookup;

			BoCA::DiscMetadata	 known;
			Array<String>		 providers;

			BoCA::DiscMetadata	 record;
			Int			 result;
			Bool			 canceled;
			Bool			 finished;
			Bool			 selecting;

			Threads::Mutex		 stateMutex;

			Threads::Thread		*queryThread;

			Int			 QueryThread();
		public:
			static Int		 QueryDisc(const BoCA::Track &, Bool, BoCA::DiscMetadata &);

			/* Show the errors of a lookup and offer to queue the discs. With more
			 * than one disc, the question is about the remaining queries.
			 */
			static Void		 OfferQueryLater(const Array<String> &, const BoCA::DiscMetadata &);
			static Void		 OfferQueryLater(const Array<String> &, const Array<BoCA::DiscMetadata> &);

			static Void		 ShowNoProviderError();
			static Void		 ShowQueueError();

						 MetadataQueryDlg(const BoCA::DiscMetadata &, const Array<String> &);
						~MetadataQueryDlg();

			const Error		&ShowDialog();
		accessors:
			Int			 GetResult() const	{ return result; }
			const BoCA::DiscMetadata &GetRecord() const	{ return record; }

			MetadataLookup		&GetLookup()		{ return lookup; }
		slots:
			Int			 SelectMatch(const String &, const Array<BoCA::MetadataMatch> &, Bool);
			Void			 OnQueryProvider(const String &);

			Bool			 CloseWindow();

			Void			 Cancel();
		private:
			Bool			 StopLookup();
	};
};

#endif
