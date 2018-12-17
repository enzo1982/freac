 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cddb/cddbbatch.h>
#include <cddb/cddblocal.h>
#include <cddb/cddbremote.h>
#include <config.h>
#include <utilities.h>

#include <dialogs/cddb/query.h>
#include <dialogs/error.h>

freac::CDDBBatch::CDDBBatch()
{
	ReadEntries();
}

freac::CDDBBatch::~CDDBBatch()
{
	SaveEntries();
}

Bool freac::CDDBBatch::ReadEntries()
{
	BoCA::Config	*config = BoCA::Config::Get();

	String::InputFormat	 inputFormat("UTF-8");
	String::OutputFormat	 outputFormat("UTF-8");

	/* Read saved queries from XML.
	 */
	XML::Document	*document = new XML::Document();

	if (document->LoadFile(String(config->cacheDir).Append("cddb").Append(Directory::GetDirectoryDelimiter()).Append("queries.xml")) == Success())
	{
		XML::Node	*root = document->GetRootNode();

		if (root != NIL)
		{
			for (Int i = 0; i < root->GetNOfNodes(); i++)
			{
				XML::Node	*node = root->GetNthNode(i);

				if (node->GetName() == "query") queries.Add(node->GetContent());
			}
		}
	}

	delete document;

	/* Read saved submits from XML and database cache.
	 */
	document = new XML::Document();

	if (document->LoadFile(String(config->cacheDir).Append("cddb").Append(Directory::GetDirectoryDelimiter()).Append("submits.xml")) == Success())
	{
		ReadEntriesXML(document);
	}

	delete document;

	return True;
}

Bool freac::CDDBBatch::ReadEntriesXML(XML::Document *document)
{
	XML::Node	*root = document->GetRootNode();

	if (root == NIL) return False;

	BoCA::Config	*config = BoCA::Config::Get();

	for (Int i = 0; i < root->GetNOfNodes(); i++)
	{
		XML::Node	*node = root->GetNthNode(i);

		if (node->GetName() == "submit")
		{
			InStream	*in = new InStream(STREAM_FILE, String(config->cacheDir).Append("cddb").Append(Directory::GetDirectoryDelimiter()).Append(node->GetAttributeByName("category")->GetContent()).Append(Directory::GetDirectoryDelimiter()).Append(node->GetContent()), IS_READ);

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

Bool freac::CDDBBatch::SaveEntries()
{
	BoCA::Config	*config = BoCA::Config::Get();

	/* Save queued queries.
	 */
	if (queries.Length() == 0)
	{
		/* Delete queries file if no more saved queries exist.
		 */
		File(String(config->cacheDir).Append("cddb").Append(Directory::GetDirectoryDelimiter()).Append("queries.xml")).Delete();
	}
	else
	{
		Directory(String(config->cacheDir).Append("cddb")).Create();

		XML::Document	*document = new XML::Document();
		XML::Node	*root = new XML::Node("cddbQueries");

		document->SetRootNode(root);

		for (Int i = 0; i < queries.Length(); i++)
		{
			root->AddNode("query", queries.GetNth(i));
		}

		document->SaveFile(String(config->cacheDir).Append("cddb").Append(Directory::GetDirectoryDelimiter()).Append("queries.xml"));

		delete document;
		delete root;
	}

	/* Save queued submits.
	 */
	if (submits.Length() == 0)
	{
		/* Delete submits file if no more saved submits exist.
		 */
		File(String(config->cacheDir).Append("cddb").Append(Directory::GetDirectoryDelimiter()).Append("submits.xml")).Delete();
	}
	else
	{
		Directory(String(config->cacheDir).Append("cddb")).Create();

		XML::Document	*document = new XML::Document();
		XML::Node	*root = new XML::Node("cddbSubmits");

		document->SetRootNode(root);

		for (Int i = 0; i < submits.Length(); i++)
		{
			XML::Node	*node = root->AddNode("submit", DiscIDToString(submits.GetNth(i).discID));

			node->SetAttribute("category", submits.GetNth(i).category);
		}

		document->SaveFile(String(config->cacheDir).Append("cddb").Append(Directory::GetDirectoryDelimiter()).Append("submits.xml"));

		delete document;
		delete root;
	}

	return True;
}

Bool freac::CDDBBatch::AddQuery(const String &query)
{
	for (Int i = 0; i < queries.Length(); i++)
	{
		if (queries.GetNth(i) == query) return False;
	}

	queries.Add(query);

	return True;
}

Bool freac::CDDBBatch::DeleteQuery(Int n)
{
	queries.Remove(queries.GetNthIndex(n));

	return True;
}

Bool freac::CDDBBatch::AddSubmit(const CDDBInfo &oCddbInfo)
{
	BoCA::Config	*config = BoCA::Config::Get();

	CDDBInfo  cddbInfo = oCddbInfo;

	UpdateEntry(cddbInfo);

	/* Create directory for entry.
	 */
	Directory	 cddbDir(String(config->cacheDir).Append("cddb"));

	if (!cddbDir.Exists()) cddbDir.Create();

	Directory	 categoryDir(String(config->cacheDir).Append("cddb").Append(Directory::GetDirectoryDelimiter()).Append(cddbInfo.category));

	if (!categoryDir.Exists()) categoryDir.Create();

	/* Save current freedb path.
	 */
	String	 configFreedbDir = config->GetStringValue(Config::CategoryFreedbID, Config::FreedbDirectoryID, Config::FreedbDirectoryDefault);

	config->SetStringValue(Config::CategoryFreedbID, Config::FreedbDirectoryID, String(config->cacheDir).Append("cddb").Append(Directory::GetDirectoryDelimiter()));

	/* Save entry to batch queue.
	 */
	CDDBLocal().Submit(cddbInfo);

	for (Int i = 0; i < submits.Length(); i++)
	{
		if (submits.GetNth(i) == cddbInfo)
		{
			submits.Remove(submits.GetNthIndex(i));

			break;
		}
	}

	submits.Add(cddbInfo);

	/* Restore real freedb path.
	 */
	config->SetStringValue(Config::CategoryFreedbID, Config::FreedbDirectoryID, configFreedbDir);

	return True;
}

Bool freac::CDDBBatch::DeleteSubmit(const CDDBInfo &cddbInfo)
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

Int freac::CDDBBatch::Query(Int n)
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	/* Query entry and delete entry if successful.
	 */
	if (config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableLocalID, Config::FreedbEnableLocalDefault) ||
	    config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, Config::FreedbEnableRemoteDefault))
	{
		cddbQueryDlg	*dlg	     = new cddbQueryDlg(queries.GetNth(n));
		Int		 error	     = dlg->ShowDialog();
		String		 errorString = dlg->GetErrorString();
		const CDDBInfo	&cddbInfo    = dlg->GetCDDBInfo();

		Object::DeleteObject(dlg);

		if (error == Error())
		{
			/* Display error message and return.
			 */
			BoCA::Utilities::ErrorMessage(i18n->TranslateString("Some error occurred trying to connect to the freedb server.", "CDDB::Query::Errors"));

			return QUERY_RESULT_ERROR;
		}
		else if (errorString != NIL)
		{
			/* If there's an info message, add it to the list of errors.
			 */
			queryErrors.Add(errorString);
		}

		if (cddbInfo == NIL) return QUERY_RESULT_NONE;

		/* Save current freedb path.
		 */
		String	 configFreedbDir = config->GetStringValue(Config::CategoryFreedbID, Config::FreedbDirectoryID, Config::FreedbDirectoryDefault);

		config->SetStringValue(Config::CategoryFreedbID, Config::FreedbDirectoryID, String(config->cacheDir).Append("cddb").Append(Directory::GetDirectoryDelimiter()));

		/* Save entry to local cache.
		 */
		CDDBLocal().Submit(cddbInfo);

		/* Restore real freedb path.
		 */
		config->SetStringValue(Config::CategoryFreedbID, Config::FreedbDirectoryID, configFreedbDir);

		DeleteQuery(n);

		return QUERY_RESULT_SINGLE;
	}

	return QUERY_RESULT_ERROR;
}

Bool freac::CDDBBatch::QueryAll()
{
	/* Run all scheduled queries.
	 */
	Int	 skipped = 0;

	while (queries.Length() > skipped)
	{
		Int	 result = Query(skipped);

		if	(result == QUERY_RESULT_ERROR) return False;
		else if (result == QUERY_RESULT_NONE)  skipped++;
	}

	/* Display errors.
	 */
	if (queryErrors.Length() > 0)
	{
		ErrorDialog	 dialog(queryErrors);

		dialog.ShowDialog();
	}

	return True;
}

Bool freac::CDDBBatch::Submit(const CDDBInfo &cddbInfo)
{
	/* Submit and delete entry if successful.
	 */
	if (!CDDBRemote().Submit(cddbInfo))
	{
		BoCA::I18n	*i18n	= BoCA::I18n::Get();

		i18n->SetContext("CDDB::Submit::Errors");

		BoCA::Utilities::ErrorMessage(i18n->TranslateString("Some error occurred trying to connect to the freedb server."));

		return False;
	}

	DeleteSubmit(cddbInfo);

	return True;
}

Bool freac::CDDBBatch::SubmitAll()
{
	while (submits.Length() > 0)
	{
		if (!Submit(submits.GetNth(0))) return False;
	}

	return True;
}
