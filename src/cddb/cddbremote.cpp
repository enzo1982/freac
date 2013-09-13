 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
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

#ifdef __WIN32__
#	include <winsock2.h>
#else
#	include <unistd.h>
#endif

#include <dllinterfaces.h>

BonkEnc::CDDBRemote::CDDBRemote(Config *iConfig) : CDDB(iConfig)
{
	connected = False;
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
				delete out;
				delete in;
				delete socket;

				connected = false;
			}

			if (command[0] == 'p' && command[1] == 'r' && command[2] == 'o' && command[3] == 't' && command[4] == 'o')	break;
			if (command[5] == 'h' && command[6] == 'e' && command[7] == 'l' && command[8] == 'l' && command[9] == 'o')	break;
			if (command[0] == 'q' && command[1] == 'u' && command[2] == 'i' && command[3] == 't')				break;
			if (command == NIL)												break;

			char	*buffer = new char [256];

			gethostname(buffer, 256);

			str.Append("POST ");

			if (config->freedb_proxy_mode == 1) str.Append("http://").Append(config->freedb_server);

			str.Append(config->freedb_query_path).Append(" HTTP/1.0\n");
			str.Append("Host: ").Append(config->freedb_server).Append(":").Append(String::FromInt(config->freedb_http_port)).Append("\n");

			if (config->freedb_proxy_mode == 1 && config->freedb_proxy_user != NIL) str.Append("Proxy-Authorization: Basic ").Append(String(String(config->freedb_proxy_user).Append(":").Append(config->freedb_proxy_password)).EncodeBase64()).Append("\n");

			str.Append("User-Email: ").Append(config->freedb_email).Append("\n");
			str.Append("Content-Length: ").Append(String::FromInt(String("cmd=").Append(command).Append("&hello=user+").Append(buffer).Append("+").Append(BonkEnc::appName).Append("+").Append(BonkEnc::cddbVersion).Append("&proto=6\n").Length())).Append("\n");
			str.Append("Charset: UTF-8\n");
			str.Append("\n");

			for (int i = 0; i < command.Length(); i++) if (command[i] == ' ') command[i] = '+';

			str.Append("cmd=").Append(command).Append("&hello=user+").Append(buffer).Append("+").Append(BonkEnc::appName).Append("+").Append(BonkEnc::cddbVersion).Append("&proto=6\n");

			delete [] buffer;

			if (config->freedb_proxy_mode == 0)		socket = new DriverSocket(config->freedb_server, config->freedb_http_port);
			else if (config->freedb_proxy_mode == 1)	socket = new DriverSocket(config->freedb_proxy, config->freedb_proxy_port);
			else if (config->freedb_proxy_mode == 2)	socket = new DriverHTTPS(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_http_port, config->freedb_proxy_user, config->freedb_proxy_password);
			else if (config->freedb_proxy_mode == 3)	socket = new DriverSOCKS4(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_http_port);
			else if (config->freedb_proxy_mode == 4)	socket = new DriverSOCKS5(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_http_port, config->freedb_proxy_user, config->freedb_proxy_password);

			if (socket->GetLastError() != IO_ERROR_OK)
			{
				debug_out->OutputLine(String("CDDB: Error connecting to CDDB server at ").Append(config->freedb_server).Append(":").Append(String::FromInt(config->freedb_http_port)));

				str = "error";

				delete socket;

				break;
			}

			in = new InStream(STREAM_DRIVER, socket);
			out = new OutStream(STREAM_STREAM, in);

			out->SetPackageSize(str.Length());

			debug_out->OutputLine(String("CDDB: ").Append(str));

			out->OutputString(str);
			out->Flush();

			do
			{
				str = in->InputLine();

				debug_out->OutputLine(String("CDDB: < ").Append(str));
			}
			while (str != NIL);

			do
			{
				str = in->InputLine();

				debug_out->OutputLine(String("CDDB: < ").Append(str));
			}
			while (str[0] != '2' && str[0] != '3' && str[0] != '4' && str[0] != '5' && str != NIL);

			if (str[1] == '1')
			{
				connected = true;
			}
			else
			{
				delete out;
				delete in;
				delete socket;
			}

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

	SendCommand(String("cddb hello user ").Append(hostNameBuffer).Append(" ").Append(BonkEnc::appName).Append(" ").Append(BonkEnc::cddbVersion));

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
	if (str[0] == '2' && str[1] == '0' && str[2] == '2') return QUERY_RESULT_NONE;

	// exact match
	if (str[0] == '2' && str[1] == '0' && str[2] == '0')
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
	if (str[0] == '2' && str[1] == '1' && (str[2] == '0' || str[2] == '1'))
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

	return ParseCDDBRecord(result, cddbInfo);
}

Bool BonkEnc::CDDBRemote::Submit(const CDDBInfo &oCddbInfo)
{
	CDDBInfo cddbInfo = oCddbInfo;

	if (!UpdateEntry(cddbInfo)) return False;

	String	 content = FormatCDDBRecord(cddbInfo);
	String	 str = "POST ";

	if (config->freedb_proxy_mode == 1) str.Append("http://").Append(config->freedb_server);

	str.Append(config->freedb_submit_path).Append(" HTTP/1.0\n");
	str.Append("Host: ").Append(config->freedb_server).Append(":").Append(String::FromInt(config->freedb_http_port)).Append("\n");

	if (config->freedb_proxy_mode == 1 && config->freedb_proxy_user != NIL) str.Append("Proxy-Authorization: Basic ").Append(String(String(config->freedb_proxy_user).Append(":").Append(config->freedb_proxy_password)).EncodeBase64()).Append("\n");

	str.Append("Category: ").Append(cddbInfo.category).Append("\n");
	str.Append("Discid: ").Append(cddbInfo.DiscIDToString()).Append("\n");
	str.Append("User-Email: ").Append(config->freedb_email).Append("\n");
	str.Append("Submit-Mode: ").Append(BonkEnc::cddbMode).Append("\n");
	str.Append("Content-Length: ").Append(String::FromInt(strlen(content.ConvertTo("UTF-8")))).Append("\n");
	str.Append("Charset: UTF-8\n");
	str.Append("\n");

	str.Append(content);

	String	 outputFormat = String::SetOutputFormat("UTF-8");

	debug_out->OutputLine(String("CDDB: ").Append(str));

	if	(config->freedb_proxy_mode == 0) socket = new DriverSocket(config->freedb_server, config->freedb_http_port);
	else if (config->freedb_proxy_mode == 1) socket = new DriverSocket(config->freedb_proxy, config->freedb_proxy_port);
	else if (config->freedb_proxy_mode == 2) socket = new DriverHTTPS(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_http_port, config->freedb_proxy_user, config->freedb_proxy_password);
	else if (config->freedb_proxy_mode == 3) socket = new DriverSOCKS4(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_http_port);
	else if (config->freedb_proxy_mode == 4) socket = new DriverSOCKS5(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_http_port, config->freedb_proxy_user, config->freedb_proxy_password);

	if (socket->GetLastError() != IO_ERROR_OK)
	{
		debug_out->OutputLine(String("CDDB: Error connecting to CDDB server at ").Append(config->freedb_server).Append(":").Append(String::FromInt(config->freedb_http_port)));

		delete socket;

		return False;
	}

	in = new InStream(STREAM_DRIVER, socket);
	out = new OutStream(STREAM_STREAM, in);

	out->SetPackageSize(str.Length());

	out->OutputLine(str);
	out->Flush();

	String::SetOutputFormat(outputFormat);

	do
	{
		str = in->InputLine();

		debug_out->OutputLine(String("CDDB: < ").Append(str));
	}
	while (str != NIL);

	do
	{
		str = in->InputLine();

		debug_out->OutputLine(String("CDDB: < ").Append(str));
	}
	while (str[0] != '2' && str[0] != '3' && str[0] != '4' && str[0] != '5');

	delete out;
	delete in;
	delete socket;

	if (str.StartsWith("200"))	return True;
	else				return False;
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
