 /* fre:ac - free audio converter
  * Copyright (C) 2001-2026 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "cddblocal.h"
#include "cddbconfig.h"

using namespace smooth::IO;

BoCA::CDDBLocal::CDDBLocal(const String &iDirectory)
{
	directory = iDirectory;
}

BoCA::CDDBLocal::~CDDBLocal()
{
}

namespace BoCA
{
	/* The name suffix of the old file during a replace. It is not a
	 * common suffix, so that files of the user are never changed.
	 */
	static const char	*OldFileSuffix = ".freac-old";

	/* Write a string and count the bytes that it should add to the file.
	 * The count is compared with the file size after the stream is closed,
	 * because a short write is not reported by the stream.
	 */
	static Bool OutputCounted(OutStream &out, const String &string, Int64 &bytes)
	{
		bytes += strlen(string);

		return out.OutputString(string);
	}

	/* Close the stream and make sure that the file has all bytes written.
	 */
	static Bool CloseCounted(OutStream &out, const String &fileName, Int64 bytes, Bool written)
	{
		Bool	 closed = out.Close();

		return written && closed && File(fileName).GetFileSize() == bytes;
	}

	/* Put a new file in place of the old one. The old file is kept
	 * until the new file is in place, and is restored on failure.
	 */
	static Bool ReplaceFile(const String &fileName, const String &newFileName)
	{
		File	 file(fileName);
		File	 newFile(newFileName);

		if (!file.Exists()) return newFile.Move(file) == Success();

		String	 oldFileName = String(fileName).Append(OldFileSuffix);
		File	 oldFile(oldFileName);

		if (oldFile.Exists()) oldFile.Delete();

		if (file.Move(oldFile) != Success()) return False;

		if (newFile.Move(file) != Success())
		{
			oldFile.Move(file);

			return False;
		}

		oldFile.Delete();

		return True;
	}

	/* Put back a file that an interrupted replace left only as its old
	 * copy. An old copy next to its file is left over and is removed.
	 */
	static Void RestoreOldFile(const String &fileName)
	{
		File	 file(fileName);
		File	 oldFile(String(fileName).Append(OldFileSuffix));

		if (!oldFile.Exists()) return;

		if (!file.Exists()) oldFile.Move(file);
		else		    oldFile.Delete();
	}

	/* Do the same for the range files of a Windows style category folder.
	 */
	static Void RestoreOldRangeFiles(const Directory &directory)
	{
		const Array<File> &files = directory.GetFilesByPattern(String("??to??").Append(OldFileSuffix));

		foreach (const File &oldFile, files)
		{
			String	 oldFileName = oldFile;

			if (oldFileName.EndsWith(OldFileSuffix)) RestoreOldFile(oldFileName.Head(oldFileName.Length() - String(OldFileSuffix).Length()));
		}
	}
}

Bool BoCA::CDDBLocal::QueryUnixDB(const String &queryString)
{
	static String	 array[] = { "rock", "misc", "newage", "soundtrack", "blues", "jazz", "folk", "country", "reggae", "classical", "data", NIL };

	String	  cddbFolder = directory;

	/* Parse query string.
	 */
	const Array<String>	&values = queryString.Explode(" ");

	Int		 numTocEntries = values.GetNth(3).ToInt();
	UnsignedInt32	 discID	       = StringToDiscID(values.GetNth(2));
	Int		 discLength    = values.GetNth(numTocEntries + 4).ToInt();
	Array<Int>	 discOffsets;

	for (Int i = 0; i < numTocEntries; i++) discOffsets.Add(values.GetNth(i + 4).ToInt());

	String::InputFormat	 inputFormat("UTF-8");
	String::OutputFormat	 outputFormat("UTF-8");

	for (Int i = 0; array[i] != NIL; i++)
	{
		String	 fileName = String(cddbFolder).Append(array[i]).Append(Directory::GetDirectoryDelimiter()).Append(CDDB::DiscIDToString(discID));

		RestoreOldFile(fileName);

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

Bool BoCA::CDDBLocal::QueryWinDB(const String &queryString)
{
	static String	 array[] = { "rock", "misc", "newage", "soundtrack", "blues", "jazz", "folk", "country", "reggae", "classical", "data", NIL };

	String	  cddbFolder = directory;

	/* Parse query string.
	 */
	const Array<String>	&values = queryString.Explode(" ");

	Int		 numTocEntries = values.GetNth(3).ToInt();
	UnsignedInt32	 discID	       = StringToDiscID(values.GetNth(2));
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

		RestoreOldRangeFiles(categoryDir);

		String	  pattern     = String().CopyN(CDDB::DiscIDToString(discID), 2).Append("to??");
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
		String		 idString = String("#FILENAME=").Append(CDDB::DiscIDToString(discID));
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

Bool BoCA::CDDBLocal::ConnectToServer()
{
	return True;
}

Int BoCA::CDDBLocal::Query(const String &queryString)
{
	/* Forget the matches of an earlier query.
	 */
	ids.RemoveAll();
	titles.RemoveAll();
	categories.RemoveAll();
	results.RemoveAll();

	/* Try to find Unix style record first; if no match is found, try Windows style.
	 */
	if (!QueryUnixDB(queryString)) QueryWinDB(queryString);

	/* No match found.
	 */
	if (categories.Length() == 0) return METADATA_QUERY_RESULT_NONE;

	/* Exact match.
	 */
	if (categories.Length() == 1) return METADATA_QUERY_RESULT_SINGLE;

	/* Multiple exact matches.
	 */
	if (categories.Length() >  1) return METADATA_QUERY_RESULT_MULTIPLE;

	return METADATA_QUERY_RESULT_ERROR;
}

Bool BoCA::CDDBLocal::Read(const String &category, UnsignedInt32 discID, CDDBInfo &cddbInfo)
{
	cddbInfo.discID   = discID;
	cddbInfo.category = category;

	String	 result;

	for (Int i = 0; i < categories.Length(); i++) if (categories.GetNth(i) == cddbInfo.category) result = results.GetNth(i);

	if (result == NIL) return False;
	else		   return ParseCDDBRecord(result, cddbInfo);
}

Bool BoCA::CDDBLocal::Submit(const CDDBInfo &oCddbInfo)
{
	CDDBInfo  cddbInfo = oCddbInfo;

	UpdateEntry(cddbInfo);

	return Write(cddbInfo);
}

/* Write a record as it is, without a new revision or category. The
 * cache uses this to replace its own copy of a record.
 */
Bool BoCA::CDDBLocal::Write(const CDDBInfo &cddbInfo)
{
	BoCA::Protocol	*protocol = BoCA::Protocol::Get("CDDB communication");

	String	  cddbFolder  = directory;

	protocol->Write(String("Writing info to local CDDB database at: ").Append(cddbFolder));

	String	  content = FormatCDDBRecord(cddbInfo);

	/* See if we have a Windows or Unix style DB.
	 */
	Directory categoryDir = Directory(String(cddbFolder).Append(cddbInfo.category));
	String	  pattern     = String("??to??");

	/* Create directory if it doesn't exist.
	 */
	categoryDir.Create();

	RestoreOldRangeFiles(categoryDir);

	if (categoryDir.GetFilesByPattern(pattern).Length() >= 1)
	{
		protocol->Write("    Found Windows style DB.");

		/* Find the file for this disc ID. Search down from the
		 * first two digits of the disc ID and include 00to??.
		 */
		pattern = String().CopyN(CDDB::DiscIDToString(cddbInfo.discID), 2).Append("to??");

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

		if (fileName == NIL)
		{
			protocol->Write("    No file for this disc ID.");

			return False;
		}

		protocol->Write(String("    Writing to: ").Append(String(fileName).Tail(fileName.Length() - cddbFolder.Length())));

		String		 newFileName = String(fileName).Append(".new");
		InStream	 in(STREAM_FILE, fileName, IS_READ);

		if (in.GetLastError() != IO_ERROR_OK)
		{
			protocol->Write("    Unable to open the file.");

			return False;
		}

		OutStream	 out(STREAM_FILE, newFileName, OS_REPLACE);

		if (out.GetLastError() != IO_ERROR_OK)
		{
			File(newFileName).Delete();

			protocol->Write("    Unable to create the new file.");

			return False;
		}

		String		 idString = String("#FILENAME=").Append(CDDB::DiscIDToString(cddbInfo.discID));
		Bool		 written  = False;
		Bool		 success  = True;
		Int64		 bytes	  = 0;

		String::InputFormat	 inputFormat("ISO-8859-1");
		String::OutputFormat	 outputFormat("ISO-8859-1");

		while (success && in.GetPos() < in.Size())
		{
			String	 line = in.InputLine();

			success = OutputCounted(out, String(line).Append("\n"), bytes);

			if (success && line == idString)
			{
				success = OutputCounted(out, content.ConvertTo("UTF-8"), bytes);

				while (success && in.GetPos() < in.Size())
				{
					line = in.InputLine();

					if (line.StartsWith("#FILENAME=")) { success = OutputCounted(out, String(line).Append("\n"), bytes); break; }
				}

				written = True;
			}
		}

		if (success && !written)
		{
			success = OutputCounted(out, String(idString).Append("\n"), bytes) &&
				  OutputCounted(out, content.ConvertTo("UTF-8"), bytes);
		}

		in.Close();

		if (!CloseCounted(out, newFileName, bytes, success))
		{
			File(newFileName).Delete();

			protocol->Write("    Unable to write the file.");

			return False;
		}

		if (!ReplaceFile(fileName, newFileName))
		{
			File(newFileName).Delete();

			protocol->Write("    Unable to replace the file.");

			return False;
		}
	}
	else
	{
		protocol->Write("    Found Unix style DB.");

		String		 fileName    = String(categoryDir).Append(Directory::GetDirectoryDelimiter()).Append(CDDB::DiscIDToString(cddbInfo.discID));
		String		 newFileName = String(fileName).Append(".new");

		RestoreOldFile(fileName);

		protocol->Write(String("    Writing to: ").Append(String(fileName).Tail(fileName.Length() - cddbFolder.Length())));

		OutStream	 out(STREAM_FILE, newFileName, OS_REPLACE);

		if (out.GetLastError() != IO_ERROR_OK)
		{
			File(newFileName).Delete();

			protocol->Write("    Unable to create the new file.");

			return False;
		}

		String::OutputFormat	 outputFormat("UTF-8");

		Int64		 bytes	 = 0;
		Bool		 success = OutputCounted(out, content, bytes);

		if (!CloseCounted(out, newFileName, bytes, success))
		{
			File(newFileName).Delete();

			protocol->Write("    Unable to write the file.");

			return False;
		}

		if (!ReplaceFile(fileName, newFileName))
		{
			File(newFileName).Delete();

			protocol->Write("    Unable to replace the file.");

			return False;
		}
	}

	protocol->Write("Finished CDDB transaction.");

	return True;
}

Bool BoCA::CDDBLocal::CloseConnection()
{
	return True;
}
