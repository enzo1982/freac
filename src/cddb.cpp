 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <iolib/drivers/driver_socket.h>
#include <iolib/drivers/driver_socks4.h>
#include <iolib/drivers/driver_socks5.h>
#include <cddb.h>
#include <dllinterfaces.h>

Array<Array<Track *> *>	 bonkEncCDDB::infoCache;
Array<Bool>		 bonkEncCDDB::requestedDiscs;

int cddb_sum(int n)
{
	int	 ret = 0;

	while (n > 0)
	{
		ret = ret + (n % 10);
		n = n / 10;
	}

	return ret;
}

bonkEncCDDB::bonkEncCDDB(Config *iConfig)
{
	activeDriveID = 0;
	connected = False;

	category = "misc";

	config = iConfig;
}

bonkEncCDDB::~bonkEncCDDB()
{
	ids.RemoveAll();
	titles.RemoveAll();
	categories.RemoveAll();
}

Int bonkEncCDDB::SetActiveDrive(Int driveID)
{
	if (driveID >= ex_CR_GetNumCDROM())
	{
		return Failure;
	}
	else
	{
		activeDriveID = driveID;

		return Success;
	}
}

Int bonkEncCDDB::ComputeDiscID()
{
	ex_CR_SetActiveCDROM(activeDriveID);

	ex_CR_ReadToc();

	int	 numTocEntries = ex_CR_GetNumTocEntries();
	int	 n = 0;

	for (int i = 0; i < numTocEntries; i++)	n += cddb_sum((ex_CR_GetTocEntry(i).dwStartSector + 150) / 75);

	int	 t = (ex_CR_GetTocEntry(numTocEntries).dwStartSector - ex_CR_GetTocEntry(0).dwStartSector) / 75;

	return ((n % 0xff) << 24 | t << 8 | numTocEntries);
}

String bonkEncCDDB::GetDiscIDString()
{
	int	 id = ComputeDiscID();
	String	 str;

	for (int i = 28; i >= 0; i -= 4)
	{
		if (((id >> i) & 15) <= 9)	str[(28 - i) / 4] = '0' + ((id >> i) & 15);
		else				str[(28 - i) / 4] = 'a' + ((id >> i) & 15) - 10;
	}

	return str;
}

String bonkEncCDDB::GetCategory()
{
	return category;
}

String bonkEncCDDB::GetCDDBQueryString()
{
	String	 str = String("cddb query ").Append(GetDiscIDString());

	ex_CR_SetActiveCDROM(activeDriveID);

	ex_CR_ReadToc();

	Int	 numTocEntries = ex_CR_GetNumTocEntries();
	TOCENTRY entry;

	str.Append(" ").Append(String::FromInt(numTocEntries));

	for (int i = 0; i < numTocEntries; i++)
	{
		entry = ex_CR_GetTocEntry(i);

		str.Append(" ").Append(String::FromInt(entry.dwStartSector + 2 * 75));
	}

	entry = ex_CR_GetTocEntry(numTocEntries);

	str.Append(" ").Append(String::FromInt(entry.dwStartSector / 75 + 2));

	return str;
}

String bonkEncCDDB::SendCommand(String command)
{
	if (!connected && config->freedb_mode == FREEDB_MODE_CDDBP) return "error not connected";

	String	 str;

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
				char	*buffer = in->InputLine();

				str = buffer;

				delete [] buffer;

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
			str.Append("User-Email: ").Append(config->freedb_email).Append("\n");
			str.Append("Content-Length: ").Append(String::FromInt(String("cmd=").Append(command).Append("&hello=user+").Append(buffer).Append("+BonkEnc+").Append(bonkEnc::cddbVersion).Append("&proto=6\n").Length())).Append("\n");
			str.Append("Charset: UTF-8\n");
			str.Append("\n");

			for (int i = 0; i < command.Length(); i++) if (command[i] == ' ') command[i] = '+';

			str.Append("cmd=").Append(command).Append("&hello=user+").Append(buffer).Append("+BonkEnc+").Append(bonkEnc::cddbVersion).Append("&proto=6\n");

			delete [] buffer;

			if (config->freedb_proxy_mode == 0)		socket = new IOLibDriverSocket(config->freedb_server, config->freedb_http_port);
			else if (config->freedb_proxy_mode == 1)	socket = new IOLibDriverSocket(config->freedb_proxy, config->freedb_proxy_port);
			else if (config->freedb_proxy_mode == 2)	socket = new IOLibDriverSOCKS4(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_http_port);
			else if (config->freedb_proxy_mode == 3)	socket = new IOLibDriverSOCKS5(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_http_port);

			if (socket->GetLastError() != IOLIB_ERROR_OK)
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
				char	*buffer = in->InputLine();

				str = buffer;

				delete [] buffer;

				debug_out->OutputString("CDDB: < ");
				debug_out->OutputLine(str);
			}
			while (str != "");

			do
			{
				char	*buffer = in->InputLine();

				str = buffer;

				delete [] buffer;

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

Bool bonkEncCDDB::ConnectToServer()
{
	if (config->freedb_mode == FREEDB_MODE_CDDBP)
	{
		if (config->freedb_proxy_mode == 0)		socket = new IOLibDriverSocket(config->freedb_server, config->freedb_cddbp_port);
		else if (config->freedb_proxy_mode == 1)	socket = new IOLibDriverSOCKS4(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_cddbp_port);
		else if (config->freedb_proxy_mode == 2)	socket = new IOLibDriverSOCKS5(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_cddbp_port);

		if (socket->GetLastError() != IOLIB_ERROR_OK)
		{
			debug_out->OutputLine(String("CDDB: Error connecting to CDDB server at ").Append(config->freedb_server).Append(":").Append(String::FromInt(config->freedb_cddbp_port)));

			connected = false;

			delete socket;

			return false;
		}

		debug_out->OutputLine(String("CDDB: Connected to CDDB server at ").Append(config->freedb_server).Append(":").Append(config->freedb_cddbp_port));

		connected = true;

		in = new InStream(STREAM_DRIVER, socket);
		out = new OutStream(STREAM_STREAM, in);
	}

	SendCommand("");
	SendCommand("proto 6");

	char	*buffer = new char [256];

	gethostname(buffer, 256);

	SendCommand(String("cddb hello user ").Append(buffer).Append(" BonkEnc ").Append(bonkEnc::cddbVersion));

	delete [] buffer;

	return true;
}

String bonkEncCDDB::Query(String discid)
{
	String	 str = SendCommand(GetCDDBQueryString());

	// no match found
	if (str[0] == '2' && str[1] == '0' && str[2] == '2') return "none";

	// exact match
	if (str[0] == '2' && str[1] == '0' && str[2] == '0')
	{
		String	 ret;

		for (int s = 4; s < 256; s++)
		{
			if (str[s] == ' ')
			{
				for (int i = 0; i < 8; i++) ret[s - 4 + i + 1] = str[s + i + 1];

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

		do
		{
			String	 val;
			char	*buffer = in->InputLine();

			val.ImportFrom("UTF-8", buffer);

			delete [] buffer;

			String	 id;
			String	 title;
			String	 category;

			debug_out->OutputString("CDDB: < ");
			debug_out->OutputLine(val.ConvertTo("UTF-8"));

			if (val == ".") break;

			for (int s = 0; s < 256; s++)
			{
				if (val[s] == ' ')
				{
					for (int i = 0; i < 8; i++)				id[i] = val[s + i + 1];
					for (int j = 0; j < (val.Length() - s - 10); j++)	title[j] = val[s + j + 10];

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

		if (str[2] == '0')	return "multiple";
		else			return "fuzzy";
	}

	return "error";
}

String bonkEncCDDB::Read(String query)
{
	String	 str = SendCommand(String("cddb read ").Append(query));

	if (str[0] == '2' && str[1] == '1' && str[2] == '0')
	{
		category = "";

		for (int i = 4; i < str.Length(); i++)
		{
			if (str[i] == ' ') break;

			category[i - 4] = str[i];
		}

		str = "";

		do
		{
			String	 val;

			char	*buffer = in->InputLine();

			val.ImportFrom("UTF-8", buffer);

			delete [] buffer;

			debug_out->OutputString("CDDB: < ");
			debug_out->OutputLine(val.ConvertTo("UTF-8"));

			if (val == ".") break;

			str.Append(val).Append("\n");
		}
		while (true);

		return str;
	}
	else
	{
		return "error";
	}
}

String bonkEncCDDB::Submit(Array<Track *> *cddbInfo)
{
	String	 str;
	String	 content;

	content.Append("# xmcd").Append("\n");
	content.Append("# ").Append("\n");
	content.Append("# Track frame offsets:").Append("\n");

	for (int i = 1; i < cddbInfo->GetNOfEntries(); i++)
	{
		content.Append("#     ").Append(String::FromInt(cddbInfo->GetNthEntry(i)->offset)).Append("\n");
	}

	content.Append("# ").Append("\n");
	content.Append("# Disc length: ").Append(String::FromInt(cddbInfo->GetNthEntry(0)->disclength)).Append("\n");
	content.Append("# ").Append("\n");
	content.Append("# Revision: ").Append(String::FromInt(cddbInfo->GetNthEntry(0)->revision)).Append("\n");
	content.Append("# Submitted via: ").Append("BonkEnc ").Append(bonkEnc::cddbVersion).Append("\n");
	content.Append("# ").Append("\n");

	content.Append("DISCID=").Append(cddbInfo->GetNthEntry(0)->discid).Append("\n");
	content.Append("DTITLE=").Append(cddbInfo->GetNthEntry(0)->artist).Append(" / ").Append(cddbInfo->GetNthEntry(0)->album).Append("\n");
	content.Append("DYEAR=").Append(cddbInfo->GetNthEntry(0)->year).Append("\n");
	content.Append("DGENRE=").Append(cddbInfo->GetNthEntry(0)->genre).Append("\n");

	for (int j = 1; j < cddbInfo->GetNOfEntries(); j++)
	{
		content.Append("TTITLE").Append(String::FromInt(j - 1)).Append("=").Append(cddbInfo->GetNthEntry(j)->title).Append("\n");
	}

	content.Append("EXTD=").Append(cddbInfo->GetNthEntry(0)->comment).Append("\n");

	for (int k = 1; k < cddbInfo->GetNOfEntries(); k++)
	{
		content.Append("EXTT").Append(String::FromInt(k - 1)).Append("=").Append(cddbInfo->GetNthEntry(k)->comment).Append("\n");
	}

	content.Append("PLAYORDER=").Append(cddbInfo->GetNthEntry(0)->playorder).Append("\n");

	str.Append("POST ").Append(config->freedb_submit_path).Append(" HTTP/1.0\n");
	str.Append("Category: ").Append(cddbInfo->GetNthEntry(0)->category).Append("\n");
	str.Append("Discid: ").Append(cddbInfo->GetNthEntry(0)->discid).Append("\n");
	str.Append("User-Email: ").Append(config->freedb_email).Append("\n");
	str.Append("Submit-Mode: ").Append("submit").Append("\n");
	str.Append("Content-Length: ").Append(String::FromInt(strlen(content.ConvertTo("UTF-8")))).Append("\n");
	str.Append("Charset: UTF-8\n");
	str.Append("\n");

	str.Append(content);

	debug_out->OutputString("CDDB: ");
	debug_out->OutputLine(str.ConvertTo("UTF-8"));

	if (config->freedb_proxy_mode == 0)		socket = new IOLibDriverSocket(config->freedb_server, config->freedb_http_port);
	else if (config->freedb_proxy_mode == 1)	socket = new IOLibDriverSOCKS4(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_http_port);
	else if (config->freedb_proxy_mode == 2)	socket = new IOLibDriverSOCKS5(config->freedb_proxy, config->freedb_proxy_port, config->freedb_server, config->freedb_http_port);

	if (socket->GetLastError() != IOLIB_ERROR_OK)
	{
		debug_out->OutputLine(String("CDDB: Error connecting to CDDB server at ").Append(config->freedb_server).Append(":").Append(String::FromInt(config->freedb_http_port)));

		str = "error";

		delete socket;

		return str;
	}

	in = new InStream(STREAM_DRIVER, socket);
	out = new OutStream(STREAM_STREAM, in);

	out->OutputString(str.ConvertTo("UTF-8"));

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

	return str;
}

Bool bonkEncCDDB::CloseConnection()
{
	if (!connected && config->freedb_mode == FREEDB_MODE_CDDBP) return false;

	SendCommand("quit");

	if (config->freedb_mode == FREEDB_MODE_CDDBP)
	{
		delete out;
		delete in;
		delete socket;
	}

	return true;
}

Int bonkEncCDDB::GetNOfMatches()
{
	return ids.GetNOfEntries();
}

String bonkEncCDDB::GetNthID(Int n)
{
	return ids.GetNthEntry(n);
}

String bonkEncCDDB::GetNthTitle(Int n)
{
	return titles.GetNthEntry(n);
}

String bonkEncCDDB::GetNthCategory(Int n)
{
	return categories.GetNthEntry(n);
}
