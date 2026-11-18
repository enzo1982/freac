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

#include "cddbconfig.h"

const String	 BoCA::CDDBConfig::ConfigID			= "freedb";

const String	 BoCA::CDDBConfig::DirectoryID			= "Directory";
const String	 BoCA::CDDBConfig::DirectoryDefault		= String("freedb").Append(Directory::GetDirectoryDelimiter());

const String	 BoCA::CDDBConfig::ServerID			= "Server";
const String	 BoCA::CDDBConfig::ServerDefault		= "gnudb.gnudb.org";

const String	 BoCA::CDDBConfig::ModeID			= "Mode";
const Int	 BoCA::CDDBConfig::ModeDefault			= 0;

const String	 BoCA::CDDBConfig::HTTPPortID			= "HTTPPort";
const Int	 BoCA::CDDBConfig::HTTPPortDefault		= 80;

const String	 BoCA::CDDBConfig::CDDBPPortID			= "CDDBPPort";
const Int	 BoCA::CDDBConfig::CDDBPPortDefault		= 8880;

const String	 BoCA::CDDBConfig::QueryPathID			= "QueryPath";
const String	 BoCA::CDDBConfig::QueryPathDefault		= "/~cddb/cddb.cgi";

const String	 BoCA::CDDBConfig::SubmitPathID			= "SubmitPath";
const String	 BoCA::CDDBConfig::SubmitPathDefault		= "/~cddb/submit.cgi";

const String	 BoCA::CDDBConfig::EmailID			= "eMail";
const String	 BoCA::CDDBConfig::EmailDefault			= "cddb@freac.org";

const String	 BoCA::CDDBConfig::ProxyID			= ProxyConfig::ServerID;
const String	 BoCA::CDDBConfig::ProxyDefault			= ProxyConfig::ServerDefault;

const String	 BoCA::CDDBConfig::ProxyUserID			= ProxyConfig::UserID;
const String	 BoCA::CDDBConfig::ProxyUserDefault		= NIL;

const String	 BoCA::CDDBConfig::ProxyPasswordID		= ProxyConfig::PasswordID;
const String	 BoCA::CDDBConfig::ProxyPasswordDefault		= NIL;

const String	 BoCA::CDDBConfig::ProxyModeID			= ProxyConfig::ModeID;
const Int	 BoCA::CDDBConfig::ProxyModeDefault		= ProxyConfig::ModeDefault;

const String	 BoCA::CDDBConfig::ProxyPortID			= ProxyConfig::PortID;
const Int	 BoCA::CDDBConfig::ProxyPortDefault		= ProxyConfig::PortDefault;

const String	 BoCA::CDDBConfig::EnableLocalID		= "EnableLocalCDDB";
const Bool	 BoCA::CDDBConfig::EnableLocalDefault		= False;

const String	 BoCA::CDDBConfig::EnableRemoteID		= "EnableRemoteCDDB";
const Bool	 BoCA::CDDBConfig::EnableRemoteDefault		= True;

const String	 BoCA::CDDBConfig::EnableCacheID		= "EnableCDDBCache";
const Bool	 BoCA::CDDBConfig::EnableCacheDefault		= True;

const String	 BoCA::CDDBConfig::QueueSubmitsID		= "QueueSubmits";
const Bool	 BoCA::CDDBConfig::QueueSubmitsDefault		= False;

const String	 BoCA::CDDBConfig::UpdateJoblistID		= "UpdateJoblistOnSubmit";
const Bool	 BoCA::CDDBConfig::UpdateJoblistDefault		= True;

const String	 BoCA::CDDBConfig::SettingsID			= "Settings";

const String	 BoCA::CDDBConfig::WindowPosXID			= "WindowPosX";
const Int	 BoCA::CDDBConfig::WindowPosXDefault		= 100;

const String	 BoCA::CDDBConfig::WindowPosYID			= "WindowPosY";
const Int	 BoCA::CDDBConfig::WindowPosYDefault		= 100;

const String	 BoCA::CDDBConfig::DialogsID			= "Dialogs";

const String	 BoCA::CDDBConfig::ManageCacheSizeXID		= "CDDBManageCacheSizeX";
const Int	 BoCA::CDDBConfig::ManageCacheSizeXDefault	= 560;

const String	 BoCA::CDDBConfig::ManageCacheSizeYID		= "CDDBManageCacheSizeY";
const Int	 BoCA::CDDBConfig::ManageCacheSizeYDefault	= 360;

const String	 BoCA::CDDBConfig::ManageSubmitsSizeXID		= "CDDBManageSubmitsSizeX";
const Int	 BoCA::CDDBConfig::ManageSubmitsSizeXDefault	= 560;

const String	 BoCA::CDDBConfig::ManageSubmitsSizeYID		= "CDDBManageSubmitsSizeY";
const Int	 BoCA::CDDBConfig::ManageSubmitsSizeYDefault	= 360;

/* The folder of the local freedb database, with a trailing delimiter.
 */
String BoCA::CDDBConfig::GetLocalDirectory(const Config *config)
{
	String	 directory = config->GetStringValue(ConfigID, DirectoryID, DirectoryDefault);

	if (!directory.EndsWith(Directory::GetDirectoryDelimiter())) directory.Append(Directory::GetDirectoryDelimiter());

	return directory;
}

/* The folder of the CDDB cache, with a trailing delimiter.
 */
String BoCA::CDDBConfig::GetCacheDirectory(const Config *config)
{
	return String(config->cacheDir).Append("cddb").Append(Directory::GetDirectoryDelimiter());
}
