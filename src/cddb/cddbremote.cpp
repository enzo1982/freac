 /* fre:ac - free audio converter
  * Copyright (C) 2001-2017 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
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

#ifdef __WIN32__
#	include <winsock2.h>
#else
#	include <unistd.h>
#endif

freac::CDDBRemote::CDDBRemote(Config *iConfig) : CDDB(iConfig)
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
	if (!connected && config->freedb_mode == FREEDB_MODE_CDDBP) return "error not connected";

	String	 str;
	String	 command = iCommand;

	switch (config->freedb_mode)
	{
		case FREEDB_MODE_CDDBP:
			if (command != NIL)
			{
				debug_out->OutputLine(String("CDDB: > ").Append(command));

				out->OutputLine(command);
				out->Flush();
			}

			do
			{
				str = in->InputLine();

				debug_out->OutputLine(String("CDDB: < ").Append(str));
			}
			while (str[0] != '2' && str[0] != '3' && str[0] != '4' && str[0] != '5');

			break;
		case FREEDB_MODE_HTTP:
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

			Net::Protocols::HTTP	 http(String("http://").Append(config->freedb_server).Append(":").Append(String::FromInt(config->freedb_http_port)).Append(config->freedb_query_path));

			http.SetParameter("cmd", String(command).Replace(" ", "+"));
			http.SetParameter("hello", String("user ").Append((char *) hostNameBuffer).Append(" ").Append(freac::appName).Append(" ").Append(freac::cddbVersion).Replace(" ", "+"));
			http.SetParameter("proto", "6");

			http.SetHeaderField("User-Email", config->freedb_email);
			http.SetHeaderField("Charset", "UTF-8");

			http.SetMode(Net::Protocols::HTTP_METHOD_GET);

			if (config->freedb_proxy_mode != 0)
			{
				http.SetProxy(config->freedb_proxy, config->freedb_proxy_port);
				http.SetProxyAuth(config->freedb_proxy_user, config->freedb_proxy_password);

				switch (config->freedb_proxy_mode)
				{
					case 1: http.SetProxyMode(Net::Protocols::HTTP_PROXY_HTTP);   break;
					case 2: http.SetProxyMode(Net::Protocols::HTTP_PROXY_HTTPS);  break;
					case 3: http.SetProxyMode(Net::Protocols::HTTP_PROXY_SOCKS4); break;
					case 4: http.SetProxyMode(Net::Protocols::HTTP_PROXY_SOCKS5); break;
				}
			}

			if (http.DownloadToBuffer(httpResultBuffer) == Error())
			{
				debug_out->OutputLine(String("CDDB: Error connecting to CDDB server at ").Append(config->freedb_server).Append(":").Append(String::FromInt(config->freedb_http_port)));

				str = "error";

				break;
			}

			in = new InStream(STREAM_BUFFER, httpResultBuffer, httpResultBuffer.Size());

			str = in->InputLine();
			debug_out->OutputLine(str);

			if (str.StartsWith("210") || str.StartsWith("211")) connected = true;
			else						    delete in;

			break;
	}

	return str;
}

Bool freac::CDDBRemote::ConnectToServer()
{
	if (config->freedb_mode == FREEDB_MODE_CDDBP)
	{
		switch (config->freedb_proxy_mode)
		{
			case 0:	socket = new DriverSocket(config->freedb_server, config->freedb_cddbp_port); break;
			case 1:	connected = False; return False;
			case 2:	socket = new DriverHTTPS(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_cddbp_port, config->freedb_proxy_user, config->freedb_proxy_password); break;
			case 3:	socket = new DriverSOCKS4(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_cddbp_port); break;
			case 4:	socket = new DriverSOCKS5(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_cddbp_port, config->freedb_proxy_user, config->freedb_proxy_password); break;
		}

		if (socket->GetLastError() != IO_ERROR_OK)
		{
			debug_out->OutputLine(String("CDDB: Error connecting to CDDB server at ").Append(config->freedb_server).Append(":").Append(String::FromInt(config->freedb_cddbp_port)));

			connected = False;

			delete socket;

			return False;
		}

		debug_out->OutputLine(String("CDDB: Connected to CDDB server at ").Append(config->freedb_server).Append(":").Append(config->freedb_cddbp_port));

		connected = True;

		in = new InStream(STREAM_DRIVER, socket);
		out = new OutStream(STREAM_STREAM, in);

		out->SetPackageSize(1024);
	}

	SendCommand(NIL);
	SendCommand("proto 6");

	hostNameBuffer.Resize(256);

	gethostname(hostNameBuffer, hostNameBuffer.Size());

	SendCommand(String("cddb hello user ").Append((char *) hostNameBuffer).Append(" ").Append(freac::appName).Append(" ").Append(freac::cddbVersion));

	return True;
}

Int freac::CDDBRemote::Query(Int discid)
{
	return Query(GetCDDBQueryString());
}

Int freac::CDDBRemote::Query(const String &queryString)
{
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
				for (Int j = 0; j < (str.Length() - s - 14); j++) title[j] = str[s + j + 10];

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
		String	 inputFormat = String::SetInputFormat("UTF-8");
		String	 outputFormat = String::SetOutputFormat("UTF-8");

		do
		{
			String	 val = in->InputLine();

			String	 id;
			String	 title;
			String	 category;

			debug_out->OutputLine(String("CDDB: < ").Append(val));

			if (val == ".") break;

			for (Int s = 0; s < 256; s++)
			{
				if (val[s] == ' ')
				{
					for (Int i = 0; i < 8; i++)			  id[i]	   = val[s + i +  1];
					for (Int j = 0; j < (val.Length() - s - 10); j++) title[j] = val[s + j + 10];

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

		String::SetInputFormat(inputFormat);
		String::SetOutputFormat(outputFormat);

		if (str[2] == '0') return QUERY_RESULT_MULTIPLE;
		else		   return QUERY_RESULT_FUZZY;
	}

	return QUERY_RESULT_ERROR;
}

Bool freac::CDDBRemote::Read(const String &category, Int discID, CDDBInfo &cddbInfo)
{
	/* Send read command and get result.
	 */
	String	 result = SendCommand(String("cddb read ").Append(category).Append(" ").Append(DiscIDToString(discID)));

	if (!result.StartsWith("210")) return False;

	cddbInfo.discID   = discID;
	cddbInfo.category = category;

	String	 inputFormat = String::SetInputFormat("UTF-8");
	String	 outputFormat = String::SetOutputFormat("UTF-8");

	result = NIL;

	do
	{
		String	 val = in->InputLine();

		debug_out->OutputLine(String("CDDB: < ").Append(val));

		if (val == ".") break;

		result.Append(val).Append("\n");
	}
	while (True);

	String::SetInputFormat(inputFormat);
	String::SetOutputFormat(outputFormat);

	/* Parse result and return.
	 */
	return ParseCDDBRecord(result, cddbInfo);
}

Bool freac::CDDBRemote::Submit(const CDDBInfo &oCddbInfo)
{
	CDDBInfo cddbInfo = oCddbInfo;

	if (!UpdateEntry(cddbInfo)) return False;

	Net::Protocols::HTTP	 http(String("http://").Append(config->freedb_server).Append(":").Append(String::FromInt(config->freedb_http_port)).Append(config->freedb_submit_path));

	http.SetHeaderField("Category", cddbInfo.category);
	http.SetHeaderField("Discid", DiscIDToString(cddbInfo.discID));
	http.SetHeaderField("User-Email", config->freedb_email);
	http.SetHeaderField("Submit-Mode", freac::cddbMode);
	http.SetHeaderField("Charset", "UTF-8");

	if (config->freedb_proxy_mode != 0)
	{
		http.SetProxy(config->freedb_proxy, config->freedb_proxy_port);
		http.SetProxyAuth(config->freedb_proxy_user, config->freedb_proxy_password);

		switch (config->freedb_proxy_mode)
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
		debug_out->OutputLine(String("CDDB: Error connecting to CDDB server at ").Append(config->freedb_server).Append(":").Append(String::FromInt(config->freedb_http_port)));

		return False;
	}

	if (String((char *) (UnsignedByte *) httpResultBuffer).StartsWith("200")) return True;
	else									  return False;
}

Bool freac::CDDBRemote::CloseConnection()
{
	if (!connected && config->freedb_mode == FREEDB_MODE_CDDBP) return False;

	SendCommand("quit");

	if (config->freedb_mode == FREEDB_MODE_CDDBP)
	{
		delete out;
		delete in;
		delete socket;
	}

	return True;
}
