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

#include <metadata/metadataqueue.h>
#include <metadata/metadatalookup.h>
#include <metadata/freedbdiscid.h>

using namespace BoCA;

/* Each change reads the file, changes it and saves it. The lock keeps
 * two threads from doing this at the same time, so that no change is
 * lost.
 */
static Threads::Mutex	 queueFileMutex;

static String BufferToHex(const Buffer<UnsignedByte> &buffer)
{
	String	 hex;

	for (Int i = 0; i < buffer.Size(); i++) hex.Append(Number((Int64) buffer[i]).ToHexString(2));

	return hex;
}

static Void HexToBuffer(const String &hex, Buffer<UnsignedByte> &buffer)
{
	buffer.Resize(hex.Length() / 2);

	for (Int i = 0; i < buffer.Size(); i++) buffer[i] = (Int64) Number::FromHexString(hex.SubString(2 * i, 2));
}

freac::MetadataQueue::MetadataQueue()
{
	Threads::Lock	 lock(queueFileMutex);

	Load();
}

freac::MetadataQueue::~MetadataQueue()
{
}

/* The queue file in a folder of the cache directory.
 */
String freac::MetadataQueue::GetFileName(const String &folder)
{
	return String(BoCA::Config::Get()->cacheDir).Append(folder).Append(Directory::GetDirectoryDelimiter()).Append("queries.xml");
}

/* Put back the queue file if a save stopped after it moved the old
 * file aside, and remove an old file that a save left next to it.
 */
static Void RestoreOldFile(const String &fileName)
{
	File	 file(fileName);
	File	 oldFile(String(fileName).Append(".old"));

	if (!oldFile.Exists()) return;

	if (!file.Exists()) oldFile.Move(file);
	else		    oldFile.Delete();
}

Bool freac::MetadataQueue::Load()
{
	String::InputFormat	 inputFormat("UTF-8");
	String::OutputFormat	 outputFormat("UTF-8");

	XML::Document	 document;

	queries.RemoveAll();

	RestoreOldFile(GetFileName("metadata"));

	if (document.LoadFile(GetFileName("metadata")) != Success()) return False;

	XML::Node	*root = document.GetRootNode();

	if (root == NIL) return False;

	for (Int i = 0; i < root->GetNOfNodes(); i++)
	{
		XML::Node	*node = root->GetNthNode(i);

		if (node->GetName() != "query") continue;

		DiscMetadata	 known;

		for (Int j = 0; j < node->GetNOfNodes(); j++)
		{
			XML::Node	*field = node->GetNthNode(j);

			if (field->GetName() == "mcdi")
			{
				Buffer<UnsignedByte>	 data;

				HexToBuffer(field->GetContent(), data);

				known.disc.mcdi.SetData(data);
			}
			else if (field->GetName() == "offsets") known.disc.offsets = field->GetContent();
			else if (field->GetName() == "artist")	known.disc.artist  = field->GetContent();
			else if (field->GetName() == "album")	known.disc.album   = field->GetContent();
		}

		if (MetadataLookup::GetDiscKey(known.disc) != NIL) queries.Add(known);
	}

	return True;
}

Bool freac::MetadataQueue::Save()
{
	String	 fileName = GetFileName("metadata");

	/* Delete the file if the queue is empty.
	 */
	if (queries.Length() == 0)
	{
		File	 file(fileName);

		RestoreOldFile(fileName);

		return !file.Exists() || file.Delete() == Success();
	}

	Directory(File(fileName).GetFilePath()).Create();

	XML::Document	*document = new XML::Document();
	XML::Node	*root	  = new XML::Node("metadataQueries");

	document->SetRootNode(root);

	foreach (const DiscMetadata &known, queries)
	{
		XML::Node	*node = root->AddNode("query");

		if (known.disc.mcdi.IsValid())	node->AddNode("mcdi", BufferToHex(known.disc.mcdi.GetData()));
		if (known.disc.offsets != NIL)	node->AddNode("offsets", known.disc.offsets);
		if (known.disc.artist  != NIL)	node->AddNode("artist", known.disc.artist);
		if (known.disc.album   != NIL)	node->AddNode("album", known.disc.album);
	}

	/* XML::Document::SaveFile reports success also when it cannot
	 * write the file. So write a new file and read it back, and put it
	 * in place of the old file only if it holds all queries.
	 */
	String	 newFileName = String(fileName).Append(".new");
	File	 newFile(newFileName);

	if (newFile.Exists()) newFile.Delete();

	document->SaveFile(newFileName);

	delete document;
	delete root;

	XML::Document	 check;
	Bool		 saved = (check.LoadFile(newFileName) == Success() && check.GetRootNode() != NIL && check.GetRootNode()->GetNOfNodes() == queries.Length());

	/* Moving a file onto an existing file fails on Windows. So move
	 * the old file aside first, and put it back if the new file cannot
	 * be moved in place. Load puts it back after a crash.
	 */
	if (saved)
	{
		File	 file(fileName);
		File	 oldFile(String(fileName).Append(".old"));

		RestoreOldFile(fileName);

		if (file.Exists() && file.Move(oldFile) != Success())
		{
			saved = False;
		}
		else if (newFile.Move(file) != Success())
		{
			if (oldFile.Exists()) oldFile.Move(file);

			saved = False;
		}
		else
		{
			if (oldFile.Exists()) oldFile.Delete();
		}
	}

	if (!saved && newFile.Exists()) newFile.Delete();

	return saved;
}

/* Add the discs that are not in the queue yet. The file is read and
 * saved once. Returns the number of discs that were added, or -1 if the
 * queue file cannot be read or saved.
 */
Int freac::MetadataQueue::AddQueries(const Array<DiscMetadata> &discs)
{
	Threads::Lock	 lock(queueFileMutex);
	Int		 added = 0;

	/* Do not replace a queue file that cannot be read. Its queries
	 * would be lost.
	 */
	if (!Load() && File(GetFileName("metadata")).Exists()) return -1;

	foreach (const DiscMetadata &known, discs)
	{
		String	 key   = MetadataLookup::GetDiscKey(known.disc);
		Bool	 found = (key == NIL);

		for (Int i = 0; i < queries.Length() && !found; i++)
		{
			if (MetadataLookup::IsSameQueuedDisc(MetadataLookup::GetDiscKey(queries.GetNth(i).disc), key)) found = True;
		}

		if (found) continue;

		queries.Add(known);

		added++;
	}

	if (added > 0 && !Save()) return -1;

	return added;
}

Bool freac::MetadataQueue::RemoveNthQuery(Int n)
{
	if (n < 0 || n >= queries.Length()) return False;

	Threads::Lock	 lock(queueFileMutex);
	String		 key = MetadataLookup::GetDiscKey(queries.GetNth(n).disc);

	/* Keep the list as it was if the file cannot be read or saved,
	 * so that the list shows what the file holds.
	 */
	Array<DiscMetadata>	 previous = queries;

	if (!Load() && File(GetFileName("metadata")).Exists()) { queries = previous; return False; }

	previous = queries;

	for (Int i = 0; i < queries.Length(); i++)
	{
		if (MetadataLookup::GetDiscKey(queries.GetNth(i).disc) == key) { queries.RemoveNth(i); break; }
	}

	if (!Save()) { queries = previous; return False; }

	return True;
}

/* Remove a disc that was found, so that a later lookup of the queue
 * does not replace the record that was chosen for it. The file is only
 * saved if it holds the disc.
 */
Bool freac::MetadataQueue::RemoveQuery(const DiscMetadata &disc)
{
	Threads::Lock	 lock(queueFileMutex);
	String		 key = MetadataLookup::GetDiscKey(disc.disc);

	if (key == NIL) return True;

	Array<DiscMetadata>	 previous = queries;

	if (!Load()) { queries = previous; return !File(GetFileName("metadata")).Exists(); }

	previous = queries;

	Bool	 removed = False;

	for (Int i = queries.Length() - 1; i >= 0; i--)
	{
		if (!MetadataLookup::IsSameQueuedDisc(MetadataLookup::GetDiscKey(queries.GetNth(i).disc), key)) continue;

		queries.RemoveNth(i);

		removed = True;
	}

	if (removed && !Save()) { queries = previous; return False; }

	return True;
}

/* A line for the queue list: artist and album if known,
 * and the number of tracks and the freedb disc ID.
 */
String freac::MetadataQueue::GetDescription(const DiscMetadata &known)
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	const Info	&disc	   = known.disc;
	Int		 numTracks = 0;
	UnsignedInt32	 discID	   = 0;

	if (disc.mcdi.IsValid())
	{
		numTracks = disc.mcdi.GetNumberOfAudioTracks();
		discID	  = FreedbDiscID::FromMCDI(disc.mcdi);
	}
	else if (disc.offsets != NIL)
	{
		const Array<String>	&values = disc.offsets.Explode("+");

		numTracks = (Int64) Number::FromHexString(values.GetFirst());
		discID	  = FreedbDiscID::FromOffsets(disc.offsets);
	}

	String	 text = i18n->TranslateString("%1 tracks, disc ID %2", "Metadata::Queue").Replace("%1", String::FromInt(numTracks)).Replace("%2", FreedbDiscID::ToString(discID));

	if (disc.album != NIL) text = String(disc.artist).Append(" - ").Append(disc.album).Append(" (").Append(text).Append(")");

	return text;
}

/* Convert the entries of <cacheDir>/cddb/queries.xml. Each old entry
 * is a CDDB query string: "cddb query <id> <n> <offset 1> ... <offset n>
 * <seconds>". The old file stays.
 */
Bool freac::MetadataQueue::MigrateCDDBQueries()
{
	if (File(GetFileName("metadata")).Exists()) return False;

	String::InputFormat	 inputFormat("UTF-8");
	String::OutputFormat	 outputFormat("UTF-8");

	XML::Document	 document;

	if (document.LoadFile(GetFileName("cddb")) != Success()) return False;

	XML::Node	*root = document.GetRootNode();

	if (root == NIL) return False;

	Array<DiscMetadata>	 discs;

	for (Int i = 0; i < root->GetNOfNodes(); i++)
	{
		XML::Node	*node = root->GetNthNode(i);

		if (node->GetName() != "query") continue;

		/* Positions of <n> and <offset 1> in the query string.
		 */
		const Int		 numEntriesToken  = 3;
		const Int		 firstOffsetToken = 4;

		const Array<String>	&values = node->GetContent().Explode(" ");

		if (values.Length() < firstOffsetToken + 1) continue;

		Int	 numEntries = values.GetNth(numEntriesToken).ToInt();

		if (numEntries <= 0 || values.Length() < firstOffsetToken + numEntries + 1) continue;

		String	 offsets = Number((Int64) numEntries).ToHexString();

		for (Int j = 0; j < numEntries; j++) offsets.Append("+").Append(Number((Int64) values.GetNth(firstOffsetToken + j).ToInt()).ToHexString());

		offsets.Append("+").Append(Number((Int64) values.GetNth(firstOffsetToken + numEntries).ToInt() * FRAMES_PER_SECOND).ToHexString());

		DiscMetadata	 known;

		known.disc.offsets = offsets.ToUpper();

		discs.Add(known);
	}

	MetadataQueue().AddQueries(discs);

	return True;
}
