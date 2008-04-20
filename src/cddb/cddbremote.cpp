 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
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
#include <bonkenc.h>
#include <dllinterfaces.h>

BonkEnc::CDDBRemote::CDDBRemote()
{
	connected = False;

	debug = Protocol::Get("Debug");
}

BonkEnc::CDDBRemote::~CDDBRemote()
{
}

String BonkEnc::CDDBRemote::SendCommand(const String &iCommand)
{
	if (!connected && config->freedb_mode == FREEDB_MODE_CDDBP) return "error not connected";

	String	 str;
	String	 command = iCommand;

	switch (config->freedb_mode)
	{
		case FREEDB_MODE_CDDBP:
			if (command != "")
			{
				debug->Write(String("CDDB: > ").Append(command));

				out->OutputLine(command);
			}

			do
			{
				str = in->InputLine();

				debug->Write(String("CDDB: < ").Append(str));
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
			if (command == "")			break;

			hostNameBuffer.Resize(256);

			gethostname(hostNameBuffer, hostNameBuffer.Size());

			Net::Protocols::HTTP	 http(String("http://").Append(config->freedb_server).Append(":").Append(String::FromInt(config->freedb_http_port)).Append(config->freedb_query_path));

			http.SetParameter("cmd", String(command).Replace(" ", "+"));
			http.SetParameter("hello", String("user ").Append(hostNameBuffer).Append(" BonkEnc ").Append(BonkEnc::cddbVersion).Replace(" ", "+"));
			http.SetParameter("proto", "6");

			http.SetHeaderField("User-Email", config->freedb_email);
			http.SetHeaderField("Charset", "UTF-8");

			http.SetMode(Net::Protocols::HTTP_METHOD_POST);

			if (config->freedb_proxy_mode != 0)
			{
				http.SetProxy(config->freedb_proxy, config->freedb_proxy_port);
				http.SetProxyAuth(config->freedb_proxy_user, config->freedb_proxy_password);

				if	(config->freedb_proxy_mode == 1) http.SetProxyMode(Net::Protocols::HTTP_PROXY_HTTP);
				else if (config->freedb_proxy_mode == 2) http.SetProxyMode(Net::Protocols::HTTP_PROXY_HTTPS);
				else if (config->freedb_proxy_mode == 3) http.SetProxyMode(Net::Protocols::HTTP_PROXY_SOCKS4);
				else if (config->freedb_proxy_mode == 4) http.SetProxyMode(Net::Protocols::HTTP_PROXY_SOCKS5);
			}

			if (http.DownloadToBuffer(httpResultBuffer) == Error())
			{
				debug->WriteError(String("CDDB: Error connecting to CDDB server at ").Append(config->freedb_server).Append(":").Append(String::FromInt(config->freedb_http_port)));

				str = "error";

				break;
			}

			in = new InStream(STREAM_BUFFER, httpResultBuffer, httpResultBuffer.Size());

			str = in->InputLine();
			debug->Write(str);

			if (str.StartsWith("210")) connected = true;
			else			   delete in;

			break;
	}

	return str;
}

Bool BonkEnc::CDDBRemote::ConnectToServer()
{
	if (config->freedb_mode == FREEDB_MODE_CDDBP)
	{
		if (config->freedb_proxy_mode == 0)		socket = new DriverSocket(config->freedb_server, config->freedb_cddbp_port);
		else if (config->freedb_proxy_mode == 1)	{ connected = False; return False; }
		else if (config->freedb_proxy_mode == 2)	socket = new DriverHTTPS(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_cddbp_port, config->freedb_proxy_user, config->freedb_proxy_password);
		else if (config->freedb_proxy_mode == 3)	socket = new DriverSOCKS4(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_cddbp_port);
		else if (config->freedb_proxy_mode == 4)	socket = new DriverSOCKS5(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_cddbp_port, config->freedb_proxy_user, config->freedb_proxy_password);

		if (socket->GetLastError() != IO_ERROR_OK)
		{
			debug->WriteError(String("CDDB: Error connecting to CDDB server at ").Append(config->freedb_server).Append(":").Append(String::FromInt(config->freedb_cddbp_port)));

			connected = False;

			delete socket;

			return False;
		}

		debug->Write(String("CDDB: Connected to CDDB server at ").Append(config->freedb_server).Append(":").Append(config->freedb_cddbp_port));

		connected = True;

		in = new InStream(STREAM_DRIVER, socket);
		out = new OutStream(STREAM_STREAM, in);
	}

	SendCommand("");
	SendCommand("proto 6");

	hostNameBuffer.Resize(256);

	gethostname(hostNameBuffer, hostNameBuffer.Size());

	SendCommand(String("cddb hello user ").Append(hostNameBuffer).Append(" BonkEnc ").Append(BonkEnc::cddbVersion));

	return True;
}

Int BonkEnc::CDDBRemote::Query(Int discid)
{
	return Query(GetCDDBQueryString());
}

Int BonkEnc::CDDBRemote::Query(const String &queryString)
{
	String	 str = SendCommand(queryString);

	ids.RemoveAll();
	titles.RemoveAll();
	categories.RemoveAll();

	// no match found
	if (str.StartsWith("202")) return QUERY_RESULT_NONE;

	// exact match
	if (str.StartsWith("200"))
	{
		String	 id;
		String	 title;
		String	 category;

		for (Int s = 4; s < 256; s++)
		{
			if (str[s] == ' ')
			{
				for (Int i = 0; i < 8; i++)				id[i] = str[s + i + 1];
				for (Int j = 0; j < (str.Length() - s - 14); j++)	title[j] = str[s + j + 10];

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

	// multiple exact matches
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

			debug->Write(String("CDDB: < ").Append(val));

			if (val == ".") break;

			for (Int s = 0; s < 256; s++)
			{
				if (val[s] == ' ')
				{
					for (Int i = 0; i < 8; i++)				id[i] = val[s + i + 1];
					for (Int j = 0; j < (val.Length() - s - 10); j++)	title[j] = val[s + j + 10];

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

		if (str[2] == '0')	return QUERY_RESULT_MULTIPLE;
		else			return QUERY_RESULT_FUZZY;
	}

	return QUERY_RESULT_ERROR;
}

Bool BonkEnc::CDDBRemote::Read(const String &category, Int discID, CDDBInfo &cddbInfo)
{
	String	 result = SendCommand(String("cddb read ").Append(category).Append(" ").Append(DiscIDToString(discID)));

	if (!result.StartsWith("210")) return False;

	cddbInfo.discID   = discID;
	cddbInfo.category = category;

	String	 inputFormat = String::SetInputFormat("UTF-8");
	String	 outputFormat = String::SetOutputFormat("UTF-8");

	result = "";

	do
	{
		String	 val = in->InputLine();

		debug->Write(String("CDDB: < ").Append(val));

		if (val == ".") break;

		result.Append(val).Append("\n");
	}
	while (True);

	String::SetInputFormat(inputFormat);
	String::SetOutputFormat(outputFormat);

	return ParseCDDBRecord(result, cddbInfo);
}

Bool BonkEnc::CDDBRemote::Submit(const CDDBInfo &oCddbInfo)
{
	CDDBInfo cddbInfo = oCddbInfo;

	if (!UpdateEntry(cddbInfo)) return False;

	Net::Protocols::HTTP	 http(String("http://").Append(config->freedb_server).Append(":").Append(String::FromInt(config->freedb_http_port)).Append(config->freedb_submit_path));

	http.SetHeaderField("Category", cddbInfo.category);
	http.SetHeaderField("Discid", cddbInfo.DiscIDToString());
	http.SetHeaderField("User-Email", config->freedb_email);
	http.SetHeaderField("Submit-Mode", BonkEnc::cddbMode);
	http.SetHeaderField("Charset", "UTF-8");

	if (config->freedb_proxy_mode != 0)
	{
		http.SetProxy(config->freedb_proxy, config->freedb_proxy_port);
		http.SetProxyAuth(config->freedb_proxy_user, config->freedb_proxy_password);

		if	(config->freedb_proxy_mode == 1) http.SetProxyMode(Net::Protocols::HTTP_PROXY_HTTP);
		else if (config->freedb_proxy_mode == 2) http.SetProxyMode(Net::Protocols::HTTP_PROXY_HTTPS);
		else if (config->freedb_proxy_mode == 3) http.SetProxyMode(Net::Protocols::HTTP_PROXY_SOCKS4);
		else if (config->freedb_proxy_mode == 4) http.SetProxyMode(Net::Protocols::HTTP_PROXY_SOCKS5);
	}

	http.SetContent(FormatCDDBRecord(cddbInfo));

	if (http.DownloadToBuffer(httpResultBuffer) == Error())
	{
		debug->WriteError(String("CDDB: Error connecting to CDDB server at ").Append(config->freedb_server).Append(":").Append(String::FromInt(config->freedb_http_port)));

		return False;
	}

	if (String((char *) (UnsignedByte *) httpResultBuffer).StartsWith("200")) return True;
	else									  return False;
}

Bool BonkEnc::CDDBRemote::CloseConnection()
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
