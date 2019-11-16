 /* fre:ac - free audio converter
  * Copyright (C) 2001-2019 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cddb/cddblocal.h>
#include <freac.h>
#include <config.h>

using namespace smooth::IO;

freac::CDDBLocal::CDDBLocal()
{
}

freac::CDDBLocal::~CDDBLocal()
{
}

Bool freac::CDDBLocal::QueryUnixDB(const String &queryString)
{
	static String	 array[] = { "rock", "misc", "newage", "soundtrack", "blues", "jazz", "folk", "country", "reggae", "classical", "data", NIL };

	/* Get configuration.
	 */
	BoCA::Config	*config = BoCA::Config::Get();

	String	  cddbFolder = config->GetStringValue(Config::CategoryFreedbID, Config::FreedbDirectoryID, Config::FreedbDirectoryDefault);

	/* Parse query string.
	 */
	const Array<String>	&values = queryString.Explode(" ");

	Int		 numTocEntries = values.GetNth(3).ToInt();
	Int		 discID	       = StringToDiscID(values.GetNth(2));
	Int		 discLength    = values.GetNth(numTocEntries + 4).ToInt();
	Array<Int>	 discOffsets;

	for (Int i = 0; i < numTocEntries; i++) discOffsets.Add(values.GetNth(i + 4).ToInt());

	String::InputFormat	 inputFormat("UTF-8");
	String::OutputFormat	 outputFormat("UTF-8");

	for (Int i = 0; array[i] != NIL; i++)
	{
		String	 fileName = String(cddbFolder).Append(array[i]).Append(Directory::GetDirectoryDelimiter()).Append(DiscIDToString(discID));

		if (!File(fileName).Exists()) continue;

		/* Read CDDB record.
		 */
		InStream	 in(STREAM_FILE, fileName, IS_READ);
		String		 result = in.InputString(in.Size());

		/* Parse CDDB record.
		 */
		CDDBInfo	 cddbInfo;

		ParseCDDBRecord(result, cddbInfo);

		if (discLength == cddbInfo.discLength)
		{
			Bool	 match = True;

			for (Int j = 0; j < cddbInfo.trackOffsets.Length(); j++)
			{
				if (discOffsets.GetNth(j) != cddbInfo.trackOffsets.GetNth(j)) match = False;
			}

			if (match)
			{
				ids.Add(discID);
				categories.Add(array[i]);
				titles.Add(String(cddbInfo.dArtist).Append(" / ").Append(cddbInfo.dTitle));
				results.Add(result);
			}
		}
	}

	return (results.Length() != 0);
}

Bool freac::CDDBLocal::QueryWinDB(const String &queryString)
{
	static String	 array[] = { "rock", "misc", "newage", "soundtrack", "blues", "jazz", "folk", "country", "reggae", "classical", "data", NIL };

	/* Get configuration.
	 */
	BoCA::Config	*config = BoCA::Config::Get();

	String	  cddbFolder = config->GetStringValue(Config::CategoryFreedbID, Config::FreedbDirectoryID, Config::FreedbDirectoryDefault);

	/* Parse query string.
	 */
	const Array<String>	&values = queryString.Explode(" ");

	Int		 numTocEntries = values.GetNth(3).ToInt();
	Int		 discID	       = StringToDiscID(values.GetNth(2));
	Int		 discLength    = values.GetNth(numTocEntries + 4).ToInt();
	Array<Int>	 discOffsets;

	for (Int i = 0; i < numTocEntries; i++) discOffsets.Add(values.GetNth(i + 4).ToInt());

	String::InputFormat	 inputFormat("UTF-8");
	String::OutputFormat	 outputFormat("UTF-8");

	for (Int i = 0; array[i] != NIL; i++)
	{
		/* Find file containing CDDB record.
		 */
		Directory categoryDir = Directory(String(cddbFolder).Append(array[i]));
		String	  pattern     = String().CopyN(DiscIDToString(discID), 2).Append("to??");
		String	  fileName;

		do
		{
			const Array<File> &files = categoryDir.GetFilesByPattern(pattern);

			if (files.Length() == 1) fileName = files.GetFirst();

			if	(pattern[1] == 'a') pattern[1] = '9';
			else if (pattern[1] == '0') pattern[1] = 'f';
			else			    pattern[1] -= 1;

			if (pattern[1] == 'f')
			{
				if	(pattern[0] == 'a') pattern[0] = '9';
				else if (pattern[0] == '0') pattern[0] = 'f';
				else			    pattern[0] -= 1;
			}
		}
		while (fileName == NIL && !(pattern[0] == 'f' && pattern[1] == 'f'));

		if (fileName == NIL) continue;

		/* Read CDDB record.
		 */
		InStream	 in(STREAM_FILE, fileName, IS_READ);
		String		 idString = String("#FILENAME=").Append(DiscIDToString(discID));
		String		 result;

		while (in.GetPos() < in.Size())
		{
			if (in.InputLine() == idString)
			{
				Int	 start = in.GetPos();
				Int	 end   = in.GetPos();

				while (in.GetPos() < in.Size())
				{
					end = in.GetPos();

					if (in.InputLine().StartsWith("#FILENAME=")) break;
				}

				in.Seek(start);

				result = in.InputString(end - start);

				break;
			}
		}

		if (result == NIL) continue;

		/* Parse CDDB record.
		 */
		CDDBInfo	 cddbInfo;

		ParseCDDBRecord(result, cddbInfo);

		if (discLength == cddbInfo.discLength)
		{
			Bool	 match = True;

			for (Int j = 0; j < cddbInfo.trackOffsets.Length(); j++)
			{
				if (discOffsets.GetNth(j) != cddbInfo.trackOffsets.GetNth(j)) match = False;
			}

			if (match)
			{
				ids.Add(discID);
				categories.Add(array[i]);
				titles.Add(String(cddbInfo.dArtist).Append(" / ").Append(cddbInfo.dTitle));
				results.Add(result);
			}
		}
	}

	return (results.Length() != 0);
}

Bool freac::CDDBLocal::ConnectToServer()
{
	return True;
}

Int freac::CDDBLocal::Query(const String &queryString)
{
	/* Try to find Unix style record first; if no match is found, try Windows style.
	 */
	if (!QueryUnixDB(queryString)) QueryWinDB(queryString);

	/* No match found.
	 */
	if (categories.Length() == 0) return QUERY_RESULT_NONE;

	/* Exact match.
	 */
	if (categories.Length() == 1) return QUERY_RESULT_SINGLE;

	/* Multiple exact matches.
	 */
	if (categories.Length() >  1) return QUERY_RESULT_MULTIPLE;

	return QUERY_RESULT_ERROR;
}

Bool freac::CDDBLocal::Read(const String &category, Int discID, CDDBInfo &cddbInfo)
{
	cddbInfo.discID   = discID;
	cddbInfo.category = category;

	String	 result;

	for (Int i = 0; i < categories.Length(); i++) if (categories.GetNth(i) == cddbInfo.category) result = results.GetNth(i);

	if (result == NIL) return False;
	else		   return ParseCDDBRecord(result, cddbInfo);
}

Bool freac::CDDBLocal::Submit(const CDDBInfo &oCddbInfo)
{
	BoCA::Config	*config	  = BoCA::Config::Get();
	BoCA::Protocol	*protocol = BoCA::Protocol::Get("CDDB communication");

	protocol->Write("Entering method: CDDBLocal::Submit(const CDDBInfo &)");

	CDDBInfo  cddbInfo = oCddbInfo;

	UpdateEntry(cddbInfo);

	String	  content = FormatCDDBRecord(cddbInfo);

	/* See if we have a Windows or Unix style DB.
	 */
	String	  cddbFolder  = config->GetStringValue(Config::CategoryFreedbID, Config::FreedbDirectoryID, Config::FreedbDirectoryDefault);
	Directory categoryDir = Directory(String(cddbFolder).Append(cddbInfo.category));
	String	  pattern     = String("??to??");

	/* Create directory if it doesn't exist.
	 */
	categoryDir.Create();

	if (categoryDir.GetFilesByPattern(pattern).Length() >= 1)
	{
		protocol->Write("Found Windows style DB.");

		pattern = String().CopyN(DiscIDToString(cddbInfo.discID), 2).Append("to??");

		String	  fileName;

		while (fileName == NIL && !(pattern[0] == '0' && pattern[1] == '0'))
		{
			const Array<File> &files = categoryDir.GetFilesByPattern(pattern);

			if (files.Length() == 1) fileName = files.GetFirst();

			if (pattern[1] == 'a')	    pattern[1] = '9';
			else if (pattern[1] == '0') pattern[1] = 'f';
			else			    pattern[1] -= 1;

			if (pattern[1] == 'f')
			{
				if (pattern[0] == 'a') pattern[0] = '9';
				else		       pattern[0] -= 1;
			}
		}

		protocol->Write(String("Writing to ").Append(fileName));

		InStream	 in(STREAM_FILE, fileName, IS_READ);
		OutStream	 out(STREAM_FILE, String(fileName).Append(".new"), OS_REPLACE);
		String		 idString = String("#FILENAME=").Append(DiscIDToString(cddbInfo.discID));
		Bool		 written  = False;

		String::InputFormat	 inputFormat("ISO-8859-1");
		String::OutputFormat	 outputFormat("ISO-8859-1");

		while (in.GetPos() < in.Size())
		{
			String	 line = in.InputLine();

			out.OutputString(String(line).Append("\n"));

			if (line == idString)
			{
				out.OutputString(content.ConvertTo("UTF-8"));

				while (in.GetPos() < in.Size())
				{
					line = in.InputLine();

					if (line.StartsWith("#FILENAME=")) { out.OutputString(String(line).Append("\n")); break; }
				}

				written = True;
			}
		}

		if (!written)
		{
			out.OutputString(String(idString).Append("\n"));
			out.OutputString(content.ConvertTo("UTF-8"));
		}

		in.Close();
		out.Close();

		File(fileName).Delete();
		File(String(fileName).Append(".new")).Move(fileName);
	}
	else
	{
		protocol->Write("Found Unix style DB.");

		String		 fileName = String(categoryDir).Append(Directory::GetDirectoryDelimiter()).Append(DiscIDToString(cddbInfo.discID));

		protocol->Write(String("Writing to ").Append(fileName));

		OutStream	 out(STREAM_FILE, fileName, OS_REPLACE);

		String::OutputFormat	 outputFormat("UTF-8");

		out.OutputString(content);
	}

	protocol->Write("Leaving method.");

	return True;
}

Bool freac::CDDBLocal::CloseConnection()
{
	return True;
}
