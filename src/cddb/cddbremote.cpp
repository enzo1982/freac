 /* fre:ac - free audio converter
  * Copyright (C) 2001-2020 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/io/drivers/driver_socket.h>
#include <smooth/io/drivers/driver_https.h>
#include <smooth/io/drivers/driver_socks4.h>
#include <smooth/io/drivers/driver_socks5.h>

#include <cddb/cddbremote.h>
#include <freac.h>
#include <config.h>

#ifdef __WIN32__
#	include <winsock2.h>
#else
#	include <unistd.h>
#endif

using namespace BoCA;

freac::CDDBRemote::CDDBRemote()
{
	connected = False;

	socket	  = NIL;

	in	  = NIL;
	out	  = NIL;
}

freac::CDDBRemote::~CDDBRemote()
{
}

String freac::CDDBRemote::SendCommand(const String &iCommand)
{
	BoCA::Config	*config   = BoCA::Config::Get();
	BoCA::Protocol	*protocol = BoCA::Protocol::Get("CDDB communication");

	Int	 freedbMode = config->GetIntValue(Config::CategoryFreedbID, Config::FreedbModeID, Config::FreedbModeDefault);

	if (!connected && freedbMode == FREEDB_MODE_CDDBP) return "error not connected";

	String	 str;
	String	 command = iCommand;

	switch (freedbMode)
	{
		case FREEDB_MODE_CDDBP:
			if (command != NIL)
			{
				protocol->Write(String("CDDB: > ").Append(command));

				out->OutputLine(command);
			}

			do
			{
				str = in->InputLine();

				protocol->Write(String("CDDB: < ").Append(str));
			}
			while (str[0] != '2' && str[0] != '3' && str[0] != '4' && str[0] != '5');

			break;
		case FREEDB_MODE_HTTP_GET:
		case FREEDB_MODE_HTTP_POST:
			if (connected)
			{
				delete in;

				connected = false;
			}

			if (command.StartsWith("proto"))	break;
			if (command.StartsWith("cddb hello"))	break;
			if (command.StartsWith("quit"))		break;
			if (command == NIL)			break;

			hostNameBuffer.Resize(256);

			gethostname(hostNameBuffer, hostNameBuffer.Size());

			String	 freedb_server	  = config->GetStringValue(Config::CategoryFreedbID, Config::FreedbServerID, Config::FreedbServerDefault);

			Int	 freedb_http_port = config->GetIntValue(Config::CategoryFreedbID, Config::FreedbHTTPPortID, Config::FreedbHTTPPortDefault);

			Net::Protocols::HTTP	 http(String("http://").Append(freedb_server).Append(":").Append(String::FromInt(freedb_http_port)).Append(config->GetStringValue(Config::CategoryFreedbID, Config::FreedbQueryPathID, Config::FreedbQueryPathDefault)));

			http.SetParameter("cmd", String(command).Replace(" ", "+"));
			http.SetParameter("hello", String("user ").Append((char *) hostNameBuffer).Append(" ").Append(freac::appName).Append(" ").Append(freac::cddbVersion).Replace(" ", "+"));
			http.SetParameter("proto", "6");

			http.SetHeaderField("User-Email", config->GetStringValue(Config::CategoryFreedbID, Config::FreedbEmailID, Config::FreedbEmailDefault));
			http.SetHeaderField("Charset", "UTF-8");

			if (freedbMode == FREEDB_MODE_HTTP_GET) http.SetMode(Net::Protocols::HTTP_METHOD_GET);
			else					http.SetMode(Net::Protocols::HTTP_METHOD_POST);

			if (config->GetIntValue(Config::CategoryFreedbID, Config::FreedbProxyModeID, Config::FreedbProxyModeDefault) != 0)
			{
				http.SetProxy(config->GetStringValue(Config::CategoryFreedbID, Config::FreedbProxyID, Config::FreedbProxyDefault), config->GetIntValue(Config::CategoryFreedbID, Config::FreedbProxyPortID, Config::FreedbProxyPortDefault));
				http.SetProxyAuth(config->GetStringValue(Config::CategoryFreedbID, Config::FreedbProxyUserID, Config::FreedbProxyUserDefault), config->GetStringValue(Config::CategoryFreedbID, Config::FreedbProxyPasswordID, Config::FreedbProxyPasswordDefault));

				switch (config->GetIntValue(Config::CategoryFreedbID, Config::FreedbProxyModeID, Config::FreedbProxyModeDefault))
				{
					case 1: http.SetProxyMode(Net::Protocols::HTTP_PROXY_HTTP);   break;
					case 2: http.SetProxyMode(Net::Protocols::HTTP_PROXY_HTTPS);  break;
					case 3: http.SetProxyMode(Net::Protocols::HTTP_PROXY_SOCKS4); break;
					case 4: http.SetProxyMode(Net::Protocols::HTTP_PROXY_SOCKS5); break;
				}
			}

			if (http.DownloadToBuffer(httpResultBuffer) == Error())
			{
				protocol->Write(String("CDDB: Error connecting to CDDB server at ").Append(freedb_server).Append(":").Append(String::FromInt(freedb_http_port)), MessageTypeError);

				str = "error";

				break;
			}

			in = new InStream(STREAM_BUFFER, httpResultBuffer, httpResultBuffer.Size());

			str = in->InputLine();
			protocol->Write(str);

			if (str.StartsWith("210") || str.StartsWith("211")) connected = true;
			else						    delete in;

			break;
	}

	return str;
}

Bool freac::CDDBRemote::ConnectToServer()
{
	BoCA::Config	*config   = BoCA::Config::Get();
	BoCA::Protocol	*protocol = BoCA::Protocol::Get("CDDB communication");

	Int	 freedbMode = config->GetIntValue(Config::CategoryFreedbID, Config::FreedbModeID, Config::FreedbModeDefault);

	if (freedbMode == FREEDB_MODE_CDDBP)
	{
		String	 freedb_server		= config->GetStringValue(Config::CategoryFreedbID, Config::FreedbServerID, Config::FreedbServerDefault);

		Int	 freedb_cddbp_port	= config->GetIntValue(Config::CategoryFreedbID, Config::FreedbCDDBPPortID, Config::FreedbCDDBPPortDefault);

		String	 freedb_proxy		= config->GetStringValue(Config::CategoryFreedbID, Config::FreedbProxyID, Config::FreedbProxyDefault);
		Int	 freedb_proxy_port	= config->GetIntValue(Config::CategoryFreedbID, Config::FreedbProxyPortID, Config::FreedbProxyPortDefault);

		String	 freedb_proxy_user	= config->GetStringValue(Config::CategoryFreedbID, Config::FreedbProxyUserID, Config::FreedbProxyUserDefault);
		String	 freedb_proxy_password	= config->GetStringValue(Config::CategoryFreedbID, Config::FreedbProxyPasswordID, Config::FreedbProxyPasswordDefault);

		switch (config->GetIntValue(Config::CategoryFreedbID, Config::FreedbProxyModeID, Config::FreedbProxyModeDefault))
		{
			case 0:	socket = new DriverSocket(freedb_server, freedb_cddbp_port); break;
			case 1:	connected = False; return False;
			case 2:	socket = new DriverHTTPS(freedb_proxy, freedb_proxy_port, freedb_server, freedb_cddbp_port, freedb_proxy_user, freedb_proxy_password); break;
			case 3:	socket = new DriverSOCKS4(freedb_proxy, freedb_proxy_port, freedb_server, freedb_cddbp_port); break;
			case 4:	socket = new DriverSOCKS5(freedb_proxy, freedb_proxy_port, freedb_server, freedb_cddbp_port, freedb_proxy_user, freedb_proxy_password); break;
		}

		if (socket->GetLastError() != IO_ERROR_OK)
		{
			protocol->Write(String("CDDB: Error connecting to CDDB server at ").Append(freedb_server).Append(":").Append(String::FromInt(freedb_cddbp_port)), MessageTypeError);

			connected = False;

			delete socket;

			return False;
		}

		protocol->Write(String("CDDB: Connected to CDDB server at ").Append(freedb_server).Append(":").Append(freedb_cddbp_port));

		connected = True;

		in = new InStream(STREAM_DRIVER, socket);
		out = new OutStream(STREAM_STREAM, in);
	}

	SendCommand(NIL);
	SendCommand("proto 6");

	hostNameBuffer.Resize(256);

	gethostname(hostNameBuffer, hostNameBuffer.Size());

	SendCommand(String("cddb hello user ").Append((char *) hostNameBuffer).Append(" ").Append(freac::appName).Append(" ").Append(freac::cddbVersion));

	return True;
}

Int freac::CDDBRemote::Query(const String &queryString)
{
	BoCA::Protocol	*protocol = BoCA::Protocol::Get("CDDB communication");

	String	 str = SendCommand(queryString);

	ids.RemoveAll();
	titles.RemoveAll();
	categories.RemoveAll();

	/* No match found.
	 */
	if (str.StartsWith("202")) return QUERY_RESULT_NONE;

	/* Exact match.
	 */
	if (str.StartsWith("200"))
	{
		String	 id;
		String	 title;
		String	 category;

		for (Int s = 4; s < 256; s++)
		{
			if (str[s] == ' ')
			{
				for (Int i = 0; i < 8; i++)			  id[i]	   = str[s + i +  1];
				for (Int i = 0; i < (str.Length() - s - 14); i++) title[i] = str[s + i + 10];

				break;
			}
			else
			{
				category[s - 4] = str[s];
			}
		}

		ids.Add(StringToDiscID(id));
		titles.Add(title);
		categories.Add(category);

		return QUERY_RESULT_SINGLE;
	}

	/* Multiple exact matches.
	 */
	if (str.StartsWith("210") || str.StartsWith("211"))
	{
		String::InputFormat	 inputFormat("UTF-8");
		String::OutputFormat	 outputFormat("UTF-8");

		do
		{
			String	 val = in->InputLine();

			String	 id;
			String	 title;
			String	 category;

			protocol->Write(String("CDDB: < ").Append(val));

			if (val == ".") break;

			for (Int s = 0; s < 256; s++)
			{
				if (val[s] == ' ')
				{
					for (Int i = 0; i < 8; i++)			  id[i]	   = val[s + i +  1];
					for (Int i = 0; i < (val.Length() - s - 10); i++) title[i] = val[s + i + 10];

					break;
				}
				else
				{
					category[s] = val[s];
				}
			}

			ids.Add(StringToDiscID(id));
			titles.Add(title);
			categories.Add(category);
		}
		while (true);

		if (str[2] == '0') return QUERY_RESULT_MULTIPLE;
		else		   return QUERY_RESULT_FUZZY;
	}

	return QUERY_RESULT_ERROR;
}

Bool freac::CDDBRemote::Read(const String &category, UnsignedInt32 discID, CDDBInfo &cddbInfo)
{
	BoCA::Protocol	*protocol = BoCA::Protocol::Get("CDDB communication");

	/* Check cache of already read entries.
	 */
	foreach (const CDDBInfo &entry, readEntries)
	{
		if (entry.category != category || entry.discID != discID) continue;

		cddbInfo = entry;

		return True;
	}

	/* Send read command and get result.
	 */
	String	 result = SendCommand(String("cddb read ").Append(category).Append(" ").Append(DiscIDToString(discID)));

	if (!result.StartsWith("210")) return False;

	cddbInfo.discID   = discID;
	cddbInfo.category = category;

	String::InputFormat	 inputFormat("UTF-8");
	String::OutputFormat	 outputFormat("UTF-8");

	result = NIL;

	do
	{
		String	 val = in->InputLine();

		protocol->Write(String("CDDB: < ").Append(val));

		if (val == ".") break;

		result.Append(val).Append("\n");
	}
	while (True);

	/* Parse result and add entry to cache.
	 */
	if (!ParseCDDBRecord(result, cddbInfo)) return False;

	readEntries.Add(cddbInfo);

	return True;
}

Bool freac::CDDBRemote::Submit(const CDDBInfo &oCddbInfo)
{
	BoCA::Config	*config   = BoCA::Config::Get();
	BoCA::Protocol	*protocol = BoCA::Protocol::Get("CDDB communication");

	CDDBInfo cddbInfo = oCddbInfo;

	if (!UpdateEntry(cddbInfo)) return False;

	String	 freedb_server	  = config->GetStringValue(Config::CategoryFreedbID, Config::FreedbServerID, Config::FreedbServerDefault);
	Int	 freedb_http_port = config->GetIntValue(Config::CategoryFreedbID, Config::FreedbHTTPPortID, Config::FreedbHTTPPortDefault);

	Net::Protocols::HTTP	 http(String("http://").Append(freedb_server).Append(":").Append(String::FromInt(freedb_http_port)).Append(config->GetStringValue(Config::CategoryFreedbID, Config::FreedbSubmitPathID, Config::FreedbSubmitPathDefault)));

	http.SetHeaderField("Category", cddbInfo.category);
	http.SetHeaderField("Discid", DiscIDToString(cddbInfo.discID));
	http.SetHeaderField("User-Email", config->GetStringValue(Config::CategoryFreedbID, Config::FreedbEmailID, Config::FreedbEmailDefault));
	http.SetHeaderField("Submit-Mode", freac::cddbMode);
	http.SetHeaderField("Charset", "UTF-8");

	if (config->GetIntValue(Config::CategoryFreedbID, Config::FreedbProxyModeID, Config::FreedbProxyModeDefault) != 0)
	{
		http.SetProxy(config->GetStringValue(Config::CategoryFreedbID, Config::FreedbProxyID, Config::FreedbProxyDefault), config->GetIntValue(Config::CategoryFreedbID, Config::FreedbProxyPortID, Config::FreedbProxyPortDefault));
		http.SetProxyAuth(config->GetStringValue(Config::CategoryFreedbID, Config::FreedbProxyUserID, Config::FreedbProxyUserDefault), config->GetStringValue(Config::CategoryFreedbID, Config::FreedbProxyPasswordID, Config::FreedbProxyPasswordDefault));

		switch (config->GetIntValue(Config::CategoryFreedbID, Config::FreedbProxyModeID, Config::FreedbProxyModeDefault))
		{
			case 1: http.SetProxyMode(Net::Protocols::HTTP_PROXY_HTTP);   break;
			case 2: http.SetProxyMode(Net::Protocols::HTTP_PROXY_HTTPS);  break;
			case 3: http.SetProxyMode(Net::Protocols::HTTP_PROXY_SOCKS4); break;
			case 4: http.SetProxyMode(Net::Protocols::HTTP_PROXY_SOCKS5); break;
		}
	}

	http.SetContent(FormatCDDBRecord(cddbInfo));

	if (http.DownloadToBuffer(httpResultBuffer) == Error())
	{
		protocol->Write(String("CDDB: Error connecting to CDDB server at ").Append(freedb_server).Append(":").Append(String::FromInt(freedb_http_port)), MessageTypeError);

		return False;
	}

	if (String((char *) (UnsignedByte *) httpResultBuffer).StartsWith("200")) return True;
	else									  return False;
}

Bool freac::CDDBRemote::CloseConnection()
{
	BoCA::Config	*config = BoCA::Config::Get();

	Int	 freedbMode = config->GetIntValue(Config::CategoryFreedbID, Config::FreedbModeID, Config::FreedbModeDefault);

	if (!connected && freedbMode == FREEDB_MODE_CDDBP) return False;

	SendCommand("quit");

	if (freedbMode == FREEDB_MODE_CDDBP)
	{
		delete out;
		delete in;
		delete socket;
	}

	return True;
}
