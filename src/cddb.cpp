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
#include <cddb.h>
#include <dllinterfaces.h>

Array<BonkEnc::CDDBInfo *>	 BonkEnc::CDDB::infoCache;

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

BonkEnc::CDDB::CDDB(Config *iConfig)
{
	activeDriveID = iConfig->cdrip_activedrive;
	connected = False;

	category = "misc";

	config = iConfig;
}

BonkEnc::CDDB::~CDDB()
{
	ids.RemoveAll();
	titles.RemoveAll();
	categories.RemoveAll();
}

Int BonkEnc::CDDB::SetActiveDrive(Int driveID)
{
	if (driveID >= ex_CR_GetNumCDROM()) return Error();

	activeDriveID = driveID;

	return Success();
}

Int BonkEnc::CDDB::ComputeDiscID()
{
	ex_CR_SetActiveCDROM(activeDriveID);

	ex_CR_ReadToc();

	Int	 numTocEntries = ex_CR_GetNumTocEntries();
	Int	 n = 0;

	for (Int i = 0; i < numTocEntries; i++)	n += cddb_sum((ex_CR_GetTocEntry(i).dwStartSector + 150) / 75);

	Int	 t = ex_CR_GetTocEntry(numTocEntries).dwStartSector / 75 - ex_CR_GetTocEntry(0).dwStartSector / 75;

	return ((n % 0xff) << 24 | t << 8 | numTocEntries);
}

String BonkEnc::CDDB::DiscIDToString(Int discID)
{
	String	 result;

	for (Int i = 28; i >= 0; i -= 4)
	{
		if (((discID >> i) & 15) <= 9)	result[(28 - i) / 4] = '0' + ((discID >> i) & 15);
		else				result[(28 - i) / 4] = 'a' + ((discID >> i) & 15) - 10;
	}

	return result;
}

Int BonkEnc::CDDB::StringToDiscID(const String &string)
{
	Int	 result = 0;

	for (Int i = 0; i < 8; i++)
	{
		if (string[i] >= '0' && string[i] <= '9')	result += ((string[i] - '0') << ((7 - i) * 4));
		else if (string[i] >= 'a' && string[i] <= 'f')	result += ((string[i] - 'a' + 10) << ((7 - i) * 4));
	}

	return result;
}

String BonkEnc::CDDB::GetCategory()
{
	return category;
}

String BonkEnc::CDDB::GetCDDBQueryString()
{
	String	 str = String("cddb query ").Append(DiscIDToString(ComputeDiscID()));

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

String BonkEnc::CDDB::SendCommand(const String &iCommand)
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

Bool BonkEnc::CDDB::ConnectToServer()
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

String BonkEnc::CDDB::Query(const String &discid)
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

Bool BonkEnc::CDDB::Read(const String &read, CDDBInfo *cddbInfo)
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

	Int	 index = 0;

	while (index < result.Length())
	{
		String	 line = ParseCDDBEntry(result, index);

		if (line.StartsWith("DTITLE"))
		{
			Int	 k;

			for (k = 7; k < line.Length(); k++)
			{
				if (line[k] == ' ' && line[k + 1] == '/' && line[k + 2] == ' ') break;

				cddbInfo->dArtist[k - 7] = line[k];
			}

			for (Int l = k + 3; l < line.Length(); l++) cddbInfo->dTitle[l - k - 3] = line[l];

			if (cddbInfo->dTitle == "") cddbInfo->dTitle = cddbInfo->dArtist;

			cddbInfo->oDArtist = cddbInfo->dArtist;
			cddbInfo->oDTitle = cddbInfo->dTitle;
		}
		else if (line.StartsWith("DGENRE"))
		{
			for (Int l = 7; l < line.Length(); l++) cddbInfo->dGenre[l - 7] = line[l];

			cddbInfo->oDGenre = cddbInfo->dGenre;
		}
		else if (line.StartsWith("DYEAR"))
		{
			String	 year;

			for (Int l = 6; l < line.Length(); l++) year[l - 6] = line[l];

			cddbInfo->dYear = year.ToInt();
		}
		else if (line.StartsWith("TTITLE"))
		{
			String	 track;
			Int	 k;

			for (k = 6; k >= 0; k++)
			{
				if (line[k] == '=')	break;
				else			track[k - 6] = line[k];
			}

			String	 artist;
			String	 title;

			if (cddbInfo->dArtist == "Various")
			{
				Int	 l;

				for (l = k + 1; l < line.Length(); l++)
				{
					if (line[l] == ' ' && line[l + 1] == '/' && line[l + 2] == ' ') break;

					artist[l - k - 1] = line[l];
				}

				for (Int m = l + 3; m < line.Length(); m++) title[m - l - 3] = line[m];

				if (title == "") { title = artist; artist = ""; }
			}
			else
			{
				for (Int l = k + 1; l < line.Length(); l++) title[l - k - 1] = line[l];
			}

			cddbInfo->trackArtists.AddEntry(artist, track.ToInt());
			cddbInfo->trackTitles.AddEntry(title, track.ToInt());

			cddbInfo->oTrackArtists.AddEntry(artist, track.ToInt());
			cddbInfo->oTrackTitles.AddEntry(title, track.ToInt());
		}
		else if (line.StartsWith("EXTD"))
		{
			for (Int k = 5; k < line.Length(); k++) cddbInfo->comment[k - 5] = line[k];

			cddbInfo->oComment = cddbInfo->comment;
		}
		else if (line.StartsWith("EXTT"))
		{
			String	 track;
			Int	 k;

			for (k = 4; k >= 0; k++)
			{
				if (line[k] == '=')	break;
				else			track[k - 4] = line[k];
			}

			String	 comment;

			for (Int l = k + 1; l < line.Length(); l++) comment[l - k - 1] = line[l];

			cddbInfo->trackComments.AddEntry(comment, track.ToInt());

			cddbInfo->oTrackComments.AddEntry(comment, track.ToInt());
		}
		else if (line.StartsWith("PLAYORDER"))
		{
			for (Int k = 10; k < line.Length(); k++) cddbInfo->playOrder[k - 10] = line[k];
		}
		else if (line.StartsWith("# Revision: "))
		{
			String	 revision;

			for (Int l = 12; l < line.Length(); l++) revision[l - 12] = line[l];

			cddbInfo->revision = revision.ToInt();
		}
		else if (line.StartsWith("# Track frame offsets:"))
		{
			Int	 track = 0;

			do
			{
				Int	 oldIndex = index;

				line = ParseCDDBEntry(result, index);

				if (line[0] == '#' && line.Length() <= 2) break;

				Int	 firstDigit = 0;
				String	 offset;

				for (Int n = 2; n < line.Length(); n++)
				{
					if (line[n] != ' ' && line[n] != '\t')
					{
						firstDigit = n;

						break;
					}
				}

				for (Int l = firstDigit; l < line.Length(); l++) offset[l - firstDigit] = line[l];

				if (offset.ToInt() == 0)
				{
					index = oldIndex;

					break;
				}

				cddbInfo->trackOffsets.AddEntry(offset.ToInt(), track++);
			}
			while (True);
		}
		else if (line.StartsWith("# Disc length: "))
		{
			String	 discLength;

			for (Int l = 15; l < line.Length(); l++) discLength[l - 15] = line[l];

			cddbInfo->discLength = discLength.ToInt();
		}
		else if (line.StartsWith("210 "))
		{
			String	 category;

			for (Int l = 4; l < line.Length(); l++)
			{
				if (line[l] == ' ') break;

				category[l - 4] = line[l];
			}

			cddbInfo->category = category;
		}
	}

	return True;
}

Bool BonkEnc::CDDB::Submit(CDDBInfo *cddbInfo)
{
	CDDBInfo	*revisionInfo = new CDDBInfo();

	ConnectToServer();

	if (cddbInfo->revision == 0)
	{
		// see if the entry already exists in another category

		String	 query = Query(cddbInfo->DiscIDToString());

		if (query != "none" && query != "error")
		{
			if (query == "multiple")
			{
				cddbInfo->category = GetNthCategory(0);
			}
			else if (query != "fuzzy")
			{
				for (Int i = 0; i < query.Length(); i++)
				{
					if (query[i] == ' ') break;

					cddbInfo->category[i] = query[i];
				}
			}
		}
	}

	if (Read(String(cddbInfo->category).Append(" ").Append(cddbInfo->DiscIDToString()), revisionInfo))
	{
		if (cddbInfo->revision == 0)
		{
			for (Int i = 0; i < cddbInfo->trackOffsets.GetNOfEntries(); i++)
			{
				if (cddbInfo->trackOffsets.GetNthEntry(i) != revisionInfo->trackOffsets.GetNthEntry(i))
				{
					// found disc ID collision

					if	(cddbInfo->category == "rock")		cddbInfo->category = "misc";
					else if (cddbInfo->category == "misc")		cddbInfo->category = "newage";
					else if (cddbInfo->category == "newage")	cddbInfo->category = "soundtrack";
					else if (cddbInfo->category == "soundtrack")	cddbInfo->category = "blues";
					else if (cddbInfo->category == "blues")		cddbInfo->category = "jazz";
					else if (cddbInfo->category == "jazz")		cddbInfo->category = "folk";
					else if (cddbInfo->category == "folk")		cddbInfo->category = "country";
					else if (cddbInfo->category == "country")	cddbInfo->category = "reggae";
					else if (cddbInfo->category == "reggae")	cddbInfo->category = "classical";
					else if (cddbInfo->category == "classical")	cddbInfo->category = "data";
					else if (cddbInfo->category == "data")		cddbInfo->category = "unknown";

					CloseConnection();

					delete revisionInfo;

					return Submit(cddbInfo);
				}
			}
		}

		// increment revision of old entry

		cddbInfo->revision = revisionInfo->revision + 1;
	}

	CloseConnection();

	delete revisionInfo;

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

	str.Append(config->freedb_query_path).Append(" HTTP/1.0\n");
	str.Append("Host: ").Append(config->freedb_server).Append(":").Append(String::FromInt(config->freedb_http_port)).Append("\n");

	if (config->freedb_proxy_mode == 1 && config->freedb_proxy_user != NIL) str.Append("Proxy-authentication: Basic ").Append(String(String(config->freedb_proxy_user).Append(":").Append(config->freedb_proxy_password)).EncodeBase64()).Append("\n");

	str.Append("Category: ").Append(cddbInfo->category).Append("\n");
	str.Append("Discid: ").Append(cddbInfo->DiscIDToString()).Append("\n");
	str.Append("User-Email: ").Append(config->freedb_email).Append("\n");
	str.Append("Submit-Mode: ").Append("submit").Append("\n");
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

Bool BonkEnc::CDDB::CloseConnection()
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

/* Format a CDDB entry according to the rules
   from the freedb how-to. Replace special
   characters and split lines at 256 chars. */

String BonkEnc::CDDB::FormatCDDBEntry(const String &entry, const String &value)
{
	if (value == "") return String(entry).Append("=\n");

	String	 result;

	for (Int i = 0; i < value.Length(); )
	{
		String	 line = String(entry).Append("=");

		for (Int c = 0; c < 254 - entry.Length() && i < value.Length(); c++, i++)
		{
			if (value[i] == '\n' || value[i] == '\t' || value[i] == '\\')
			{
				if (c >= 253 - entry.Length()) break;

				if (value[i] == '\n') line.Append("\\n");
				if (value[i] == '\t') line.Append("\\t");
				if (value[i] == '\\') line.Append("\\\\");

				c++;
			}
			else
			{
				line[line.Length()] = value[i];
			}
		}

		result.Append(line).Append("\n");
	}

	return result;
}

/* Parse the next CDDB entry according to the
   rules from the freedb how-to. Substitute
   special character sequences and concatenate
   multiline entries. */

String BonkEnc::CDDB::ParseCDDBEntry(const String &cddb, Int &index)
{
	String	 result;
	String	 entry;

	while (index < cddb.Length())
	{
		if (cddb[index] == '=')		break;
		if (cddb[index] == '\n')	{ index++; return entry; }

		entry[entry.Length()] = cddb[index++];
	}

	result.Append(entry).Append("=");

	index -= entry.Length();

	while (index < cddb.Length())
	{
		String	 line;
		Int	 oldIndex = index;

		while (index < cddb.Length())
		{
			if (cddb[index] == '\n') break;

			if (cddb[index] == '\\')
			{
				if (cddb[index + 1] == 'n') line[line.Length()] = '\n';
				if (cddb[index + 1] == 't') line[line.Length()] = '\t';
				if (cddb[index + 1] == '\\') line[line.Length()] = '\\';

				index += 2;
			}
			else
			{
				line[line.Length()] = cddb[index++];
			}
		}

		if (!line.StartsWith(String(entry).Append("="))) { index = oldIndex; break; }

		for (Int j = entry.Length() + 1; j < line.Length(); j++) result[result.Length()] = line[j];

		index++;
	}

	return result;
}

Int BonkEnc::CDDB::GetNumberOfMatches()
{
	return ids.GetNOfEntries();
}

String BonkEnc::CDDB::GetNthDiscID(Int n)
{
	return ids.GetNthEntry(n);
}

String BonkEnc::CDDB::GetNthTitle(Int n)
{
	return titles.GetNthEntry(n);
}

String BonkEnc::CDDB::GetNthCategory(Int n)
{
	return categories.GetNthEntry(n);
}
