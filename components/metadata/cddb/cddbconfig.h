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

#ifndef H_BOCA_CDDB_CONFIG
#define H_BOCA_CDDB_CONFIG

#include <boca.h>

namespace BoCA
{
	/* Configuration IDs and defaults of the CDDB component. The IDs
	 * are those of the fre:ac "freedb" category, so that existing
	 * settings stay valid without a migration.
	 */
	class CDDBConfig
	{
		public:
			static const String	 ConfigID;

			static const String	 DirectoryID;
			static const String	 DirectoryDefault;

			static const String	 ServerID;
			static const String	 ServerDefault;

			static const String	 ModeID;
			static const Int	 ModeDefault;

			static const String	 HTTPPortID;
			static const Int	 HTTPPortDefault;

			static const String	 CDDBPPortID;
			static const Int	 CDDBPPortDefault;

			static const String	 QueryPathID;
			static const String	 QueryPathDefault;

			static const String	 SubmitPathID;
			static const String	 SubmitPathDefault;

			static const String	 EmailID;
			static const String	 EmailDefault;

			static const String	 ProxyID;
			static const String	 ProxyDefault;

			static const String	 ProxyUserID;
			static const String	 ProxyUserDefault;

			static const String	 ProxyPasswordID;
			static const String	 ProxyPasswordDefault;

			static const String	 ProxyModeID;
			static const Int	 ProxyModeDefault;

			static const String	 ProxyPortID;
			static const Int	 ProxyPortDefault;

			static const String	 EnableLocalID;
			static const Bool	 EnableLocalDefault;

			static const String	 EnableRemoteID;
			static const Bool	 EnableRemoteDefault;

			static const String	 EnableCacheID;
			static const Bool	 EnableCacheDefault;

			static const String	 QueueSubmitsID;
			static const Bool	 QueueSubmitsDefault;

			static const String	 UpdateJoblistID;
			static const Bool	 UpdateJoblistDefault;

			/* Window position and dialog sizes, in the fre:ac categories.
			 */
			static const String	 SettingsID;

			static const String	 WindowPosXID;
			static const Int	 WindowPosXDefault;

			static const String	 WindowPosYID;
			static const Int	 WindowPosYDefault;

			static const String	 DialogsID;

			static const String	 ManageCacheSizeXID;
			static const Int	 ManageCacheSizeXDefault;

			static const String	 ManageCacheSizeYID;
			static const Int	 ManageCacheSizeYDefault;

			static const String	 ManageSubmitsSizeXID;
			static const Int	 ManageSubmitsSizeXDefault;

			static const String	 ManageSubmitsSizeYID;
			static const Int	 ManageSubmitsSizeYDefault;

			static String		 GetLocalDirectory(const Config *);
			static String		 GetCacheDirectory(const Config *);
	};
};

#endif
