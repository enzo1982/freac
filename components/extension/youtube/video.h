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

#ifndef H_YOUTUBE_VIDEO
#define H_YOUTUBE_VIDEO

#include <smooth.h>
#include <boca.h>

using namespace smooth;

using namespace BoCA;

#include "videosite.h"

namespace BoCA
{
	class Video
	{
		private:
			static Int			 activeDownloads;

			Threads::Mutex			 downloadMutex;

			VideoSite			*videoSite;

			String				 videoURL;
			String				 videoPageHTML;

			String				 videoFile;
			String				 videoDecoder;

			String				 videoTitle;
			String				 videoDescription;

			String				 videoUploader;
			String				 videoDate;

			String				 videoThumbnailURL;
			Picture				 videoThumbnail;

			Bool				 pageDownloaded;
			Bool				 metadataQueried;

			Bool				 videoDownloadStarted;
			Bool				 videoDownloadFinished;

			Bool				 doCancelDownload;

			Int				 DownloaderThread(String);

			Bool				 DownloadPage();
		public:
							 Video(VideoSite *);
							~Video();

			Bool				 QueryMetadata();
			const String			&GetDecoderID();

			Bool				 Download();
			Bool				 CancelDownload();
		accessors:
			Bool				 IsDownloadStarted() const		{ return videoDownloadStarted; }
			Bool				 IsDownloadFinished() const		{ return videoDownloadFinished; }

			Bool				 IsDownloadCancelled() const		{ return doCancelDownload; }

			Bool				 SetVideoURL(const String &nVideoURL)	{ videoURL = nVideoURL; return True; }
			const String			&GetVideoURL() const			{ return videoURL; }

			Bool				 SetVideoFile(const String &nVideoFile)	{ videoFile = nVideoFile; return True; }
			const String			&GetVideoFile() const			{ return videoFile; }

			const String			&GetVideoSiteName() const		{ return videoSite->GetName(); }

			const String			&GetVideoTitle() const			{ return videoTitle; }
			const String			&GetVideoDescription() const		{ return videoDescription; }

			const String			&GetVideoUploader() const		{ return videoUploader; }
			const String			&GetVideoDate() const			{ return videoDate; }

			const String			&GetVideoThumbnailURL() const		{ return videoThumbnailURL; }
			const Picture			&GetVideoThumbnail() const		{ return videoThumbnail; }
		signals:
			Signal1<Void, Int>		 downloadProgress;
			Signal1<Void, const String &>	 downloadSpeed;

			Signal1<Void, Video *>		 startDownload;
			Signal1<Void, Video *>		 finishDownload;
		slots:
			Bool				 DoCancelDownload()			{ return doCancelDownload; }
	};
};

#endif
