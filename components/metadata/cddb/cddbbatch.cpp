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

#include "cddbbatch.h"
#include "cddblocal.h"
#include "cddbremote.h"
#include "cddbconfig.h"

Threads::Mutex	 BoCA::CDDBBatch::submitsFileMutex;

BoCA::CDDBBatch::CDDBBatch(const Config *iConfig)
{
	config = iConfig;

	ReadEntries();
}

BoCA::CDDBBatch::~CDDBBatch()
{
	SaveEntries();
}

Bool BoCA::CDDBBatch::ReadEntries()
{
	Threads::Lock		 lock(submitsFileMutex);

	String::InputFormat	 inputFormat("UTF-8");
	String::OutputFormat	 outputFormat("UTF-8");

	/* Read saved submits from XML and database cache.
	 */
	XML::Document	*document = new XML::Document();

	if (document->LoadFile(CDDBConfig::GetCacheDirectory(config).Append("submits.xml")) == Success())
	{
		ReadEntriesXML(document);
	}

	delete document;

	return True;
}

Bool BoCA::CDDBBatch::ReadEntriesXML(XML::Document *document)
{
	XML::Node	*root = document->GetRootNode();

	if (root == NIL) return False;

	for (Int i = 0; i < root->GetNOfNodes(); i++)
	{
		XML::Node	*node = root->GetNthNode(i);

		if (node->GetName() == "submit")
		{
			InStream	*in = new InStream(STREAM_FILE, CDDBConfig::GetCacheDirectory(config).Append(node->GetAttributeByName("category")->GetContent()).Append(Directory::GetDirectoryDelimiter()).Append(node->GetContent()), IS_READ);

			if (in->Size() > 0)
			{
				String	 result = in->InputString(in->Size());
				CDDBInfo cddbInfo;

				ParseCDDBRecord(result, cddbInfo);

				cddbInfo.category = node->GetAttributeByName("category")->GetContent();

				for (Int j = 0; j < submits.Length(); j++)
				{
					if (submits.GetNth(j) == cddbInfo)
					{
						submits.Remove(submits.GetNthIndex(j));

						break;
					}
				}

				submits.Add(cddbInfo);
			}

			delete in;
		}
	}

	return True;
}

Bool BoCA::CDDBBatch::SaveEntries()
{
	Threads::Lock	 lock(submitsFileMutex);

	/* Save queued submits.
	 */
	if (submits.Length() == 0)
	{
		/* Delete submits file if no more saved submits exist.
		 */
		File(CDDBConfig::GetCacheDirectory(config).Append("submits.xml")).Delete();
	}
	else
	{
		Directory(CDDBConfig::GetCacheDirectory(config)).Create();

		XML::Document	*document = new XML::Document();
		XML::Node	*root = new XML::Node("cddbSubmits");

		document->SetRootNode(root);

		for (Int i = 0; i < submits.Length(); i++)
		{
			XML::Node	*node = root->AddNode("submit", CDDB::DiscIDToString(submits.GetNth(i).discID));

			node->SetAttribute("category", submits.GetNth(i).category);
		}

		document->SaveFile(CDDBConfig::GetCacheDirectory(config).Append("submits.xml"));

		delete document;
		delete root;
	}

	return True;
}

Bool BoCA::CDDBBatch::AddSubmit(const CDDBInfo &oCddbInfo)
{
	CDDBInfo  cddbInfo = oCddbInfo;

	UpdateEntry(cddbInfo);

	/* Create directory for entry.
	 */
	Directory	 cddbDir(CDDBConfig::GetCacheDirectory(config));

	if (!cddbDir.Exists()) cddbDir.Create();

	Directory	 categoryDir(CDDBConfig::GetCacheDirectory(config).Append(cddbInfo.category));

	if (!categoryDir.Exists()) categoryDir.Create();

	/* Save entry to batch queue.
	 */
	if (!CDDBLocal(CDDBConfig::GetCacheDirectory(config)).Submit(cddbInfo)) return False;

	for (Int i = 0; i < submits.Length(); i++)
	{
		if (submits.GetNth(i) == cddbInfo)
		{
			submits.Remove(submits.GetNthIndex(i));

			break;
		}
	}

	submits.Add(cddbInfo);

	return True;
}

Bool BoCA::CDDBBatch::DeleteSubmit(const CDDBInfo &cddbInfo)
{
	for (Int i = 0; i < submits.Length(); i++)
	{
		if (submits.GetNth(i) == cddbInfo)
		{
			submits.Remove(submits.GetNthIndex(i));

			break;
		}
	}

	return True;
}

Bool BoCA::CDDBBatch::Submit(const CDDBInfo &cddbInfo)
{
	/* Submit and delete entry if successful.
	 */
	if (!CDDBRemote(config).Submit(cddbInfo)) return False;

	DeleteSubmit(cddbInfo);

	return True;
}

Bool BoCA::CDDBBatch::SubmitAll()
{
	while (submits.Length() > 0)
	{
		if (!Submit(submits.GetNth(0))) return False;
	}

	return True;
}
