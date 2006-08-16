 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
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
#include <dllinterfaces.h>

BonkEnc::CDDBRemote::CDDBRemote(Config *iConfig) : CDDB(iConfig)
{
	connected = False;
}

BonkEnc::CDDBRemote::~CDDBRemote()
{
}

String BonkEnc::CDDBRemote::GetCDDBQueryString()
{
	ex_CR_SetActiveCDROM(activeDriveID);
	ex_CR_ReadToc();

	Int	 numTocEntries = ex_CR_GetNumTocEntries();
	String	 str = String("cddb query ").Append(DiscIDToString(ComputeDiscID()));

	str.Append(" ").Append(String::FromInt(numTocEntries));

	for (int i = 0; i < numTocEntries; i++)
	{
		str.Append(" ").Append(String::FromInt(ex_CR_GetTocEntry(i).dwStartSector + 150));
	}

	str.Append(" ").Append(String::FromInt(ex_CR_GetTocEntry(numTocEntries).dwStartSector / 75 - ex_CR_GetTocEntry(0).dwStartSector / 75 + 2));

	return str;
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
				debug_out->OutputString("CDDB: > ");
				debug_out->OutputLine(command);

				out->OutputLine(command);
			}

			do
			{
				str = in->InputLine();

				debug_out->OutputString("CDDB: < ");
				debug_out->OutputLine(str);
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
			if (command == "")												break;

			char	*buffer = new char [256];

			gethostname(buffer, 256);

			str.Append("POST ");

			if (config->freedb_proxy_mode == 1) str.Append("http://").Append(config->freedb_server);

			str.Append(config->freedb_query_path).Append(" HTTP/1.0\n");
			str.Append("Host: ").Append(config->freedb_server).Append(":").Append(String::FromInt(config->freedb_http_port)).Append("\n");

			if (config->freedb_proxy_mode == 1 && config->freedb_proxy_user != NIL) str.Append("Proxy-authentication: Basic ").Append(String(String(config->freedb_proxy_user).Append(":").Append(config->freedb_proxy_password)).EncodeBase64()).Append("\n");

			str.Append("User-Email: ").Append(config->freedb_email).Append("\n");
			str.Append("Content-Length: ").Append(String::FromInt(String("cmd=").Append(command).Append("&hello=user+").Append(buffer).Append("+BonkEnc+").Append(BonkEnc::cddbVersion).Append("&proto=6\n").Length())).Append("\n");
			str.Append("Charset: UTF-8\n");
			str.Append("\n");

			for (int i = 0; i < command.Length(); i++) if (command[i] == ' ') command[i] = '+';

			str.Append("cmd=").Append(command).Append("&hello=user+").Append(buffer).Append("+BonkEnc+").Append(BonkEnc::cddbVersion).Append("&proto=6\n");

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

			debug_out->OutputString("CDDB: ");
			debug_out->OutputLine(str);

			out->OutputString(str);

			do
			{
				str = in->InputLine();

				debug_out->OutputString("CDDB: < ");
				debug_out->OutputLine(str);
			}
			while (str != "");

			do
			{
				str = in->InputLine();

				debug_out->OutputString("CDDB: < ");
				debug_out->OutputLine(str);
			}
			while (str[0] != '2' && str[0] != '3' && str[0] != '4' && str[0] != '5' && str != "");

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
	}

	SendCommand("");
	SendCommand("proto 6");

	hostNameBuffer.Resize(256);

	gethostname(hostNameBuffer, hostNameBuffer.Size());

	SendCommand(String("cddb hello user ").Append(hostNameBuffer).Append(" BonkEnc ").Append(BonkEnc::cddbVersion));

	return True;
}

String BonkEnc::CDDBRemote::Query(const String &discid)
{
	String	 str = SendCommand(GetCDDBQueryString());

	// no match found
	if (str[0] == '2' && str[1] == '0' && str[2] == '2') return "none";

	// exact match
	if (str[0] == '2' && str[1] == '0' && str[2] == '0')
	{
		String	 ret;

		for (Int s = 4; s < 256; s++)
		{
			if (str[s] == ' ')
			{
				for (Int i = 0; i < 8; i++) ret[s - 4 + i + 1] = str[s + i + 1];

				break;
			}
			else
			{
				ret[s - 4] = str[s];
			}
		}

		return ret;
	}

	// multiple exact matches
	if (str[0] == '2' && str[1] == '1' && (str[2] == '0' || str[2] == '1'))
	{
		ids.RemoveAll();
		titles.RemoveAll();
		categories.RemoveAll();

		String	 inputFormat = String::SetInputFormat("UTF-8");
		String	 outputFormat = String::SetOutputFormat("UTF-8");

		do
		{
			String	 val = in->InputLine();

			String	 id;
			String	 title;
			String	 category;

			debug_out->OutputString("CDDB: < ");
			debug_out->OutputLine(val);

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

			ids.AddEntry(id);
			titles.AddEntry(title);
			categories.AddEntry(category);
		}
		while (true);

		String::SetInputFormat(inputFormat);
		String::SetOutputFormat(outputFormat);

		if (str[2] == '0')	return "multiple";
		else			return "fuzzy";
	}

	return "error";
}

Bool BonkEnc::CDDBRemote::Read(const String &read, CDDBInfo *cddbInfo)
{
	String	 result = SendCommand(String("cddb read ").Append(read));

	if (!result.StartsWith("210")) return False;

	cddbInfo->discID = ComputeDiscID();

	for (Int i = 4; i < result.Length(); i++)
	{
		if (result[i] == ' ') break;

		cddbInfo->category[i - 4] = result[i];
	}

	result = "";

	String	 inputFormat = String::SetInputFormat("UTF-8");
	String	 outputFormat = String::SetOutputFormat("UTF-8");

	do
	{
		String	 val = in->InputLine();

		debug_out->OutputString("CDDB: < ");
		debug_out->OutputLine(val);

		if (val == ".") break;

		result.Append(val).Append("\n");
	}
	while (True);

	String::SetInputFormat(inputFormat);
	String::SetOutputFormat(outputFormat);

	return ParseCDDBRecord(result, cddbInfo);
}

Bool BonkEnc::CDDBRemote::Submit(CDDBInfo *cddbInfo)
{
	UpdateEntry(cddbInfo);

	String	 str;
	String	 content;

	content.Append("# xmcd").Append("\n");
	content.Append("# ").Append("\n");
	content.Append("# Track frame offsets:").Append("\n");

	for (Int i = 0; i < cddbInfo->trackOffsets.GetNOfEntries(); i++)
	{
		content.Append("#     ").Append(String::FromInt(cddbInfo->trackOffsets.GetNthEntry(i))).Append("\n");
	}

	content.Append("# ").Append("\n");
	content.Append("# Disc length: ").Append(String::FromInt(cddbInfo->discLength)).Append("\n");
	content.Append("# ").Append("\n");
	content.Append("# Revision: ").Append(String::FromInt(cddbInfo->revision)).Append("\n");
	content.Append("# Submitted via: ").Append("BonkEnc ").Append(BonkEnc::cddbVersion).Append("\n");
	content.Append("# ").Append("\n");

	content.Append(FormatCDDBEntry("DISCID", cddbInfo->DiscIDToString()));
	content.Append(FormatCDDBEntry("DTITLE", String(cddbInfo->dArtist).Append(" / ").Append(cddbInfo->dTitle)));
	content.Append(FormatCDDBEntry("DYEAR", String::FromInt(cddbInfo->dYear)));
	content.Append(FormatCDDBEntry("DGENRE", cddbInfo->dGenre));

	for (Int j = 0; j < cddbInfo->trackTitles.GetNOfEntries(); j++)
	{
		if (cddbInfo->dArtist == "Various")	content.Append(FormatCDDBEntry(String("TTITLE").Append(String::FromInt(j)), String(cddbInfo->trackArtists.GetNthEntry(j)).Append(" / ").Append(cddbInfo->trackTitles.GetNthEntry(j))));
		else					content.Append(FormatCDDBEntry(String("TTITLE").Append(String::FromInt(j)), cddbInfo->trackTitles.GetNthEntry(j)));
	}

	content.Append(FormatCDDBEntry("EXTD", cddbInfo->comment));

	for (Int k = 0; k < cddbInfo->trackComments.GetNOfEntries(); k++)
	{
		content.Append(FormatCDDBEntry(String("EXTT").Append(String::FromInt(k)), cddbInfo->trackComments.GetNthEntry(k)));
	}

	content.Append(FormatCDDBEntry("PLAYORDER", cddbInfo->playOrder));

	str.Append("POST ");

	if (config->freedb_proxy_mode == 1) str.Append("http://").Append(config->freedb_server);

	str.Append(config->freedb_submit_path).Append(" HTTP/1.0\n");
	str.Append("Host: ").Append(config->freedb_server).Append(":").Append(String::FromInt(config->freedb_http_port)).Append("\n");

	if (config->freedb_proxy_mode == 1 && config->freedb_proxy_user != NIL) str.Append("Proxy-authentication: Basic ").Append(String(String(config->freedb_proxy_user).Append(":").Append(config->freedb_proxy_password)).EncodeBase64()).Append("\n");

	str.Append("Category: ").Append(cddbInfo->category).Append("\n");
	str.Append("Discid: ").Append(cddbInfo->DiscIDToString()).Append("\n");
	str.Append("User-Email: ").Append(config->freedb_email).Append("\n");
	str.Append("Submit-Mode: ").Append("test").Append("\n");
	str.Append("Content-Length: ").Append(String::FromInt(strlen(content.ConvertTo("UTF-8")))).Append("\n");
	str.Append("Charset: UTF-8\n");
	str.Append("\n");

	str.Append(content);

	String	 outputFormat = String::SetOutputFormat("UTF-8");

	debug_out->OutputString("CDDB: ");
	debug_out->OutputLine(str);

	if (config->freedb_proxy_mode == 0)		socket = new DriverSocket(config->freedb_server, config->freedb_http_port);
	else if (config->freedb_proxy_mode == 1)	socket = new DriverSocket(config->freedb_proxy, config->freedb_proxy_port);
	else if (config->freedb_proxy_mode == 2)	socket = new DriverHTTPS(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_http_port, config->freedb_proxy_user, config->freedb_proxy_password);
	else if (config->freedb_proxy_mode == 3)	socket = new DriverSOCKS4(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_http_port);
	else if (config->freedb_proxy_mode == 4)	socket = new DriverSOCKS5(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_http_port, config->freedb_proxy_user, config->freedb_proxy_password);

	if (socket->GetLastError() != IO_ERROR_OK)
	{
		debug_out->OutputLine(String("CDDB: Error connecting to CDDB server at ").Append(config->freedb_server).Append(":").Append(String::FromInt(config->freedb_http_port)));

		delete socket;

		return False;
	}

	in = new InStream(STREAM_DRIVER, socket);
	out = new OutStream(STREAM_STREAM, in);

	out->OutputString(str);

	String::SetOutputFormat(outputFormat);

	do
	{
		str = in->InputLine();

		debug_out->OutputString("CDDB: < ");
		debug_out->OutputLine(str);
	}
	while (str != "");

	do
	{
		str = in->InputLine();

		debug_out->OutputString("CDDB: < ");
		debug_out->OutputLine(str);
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
