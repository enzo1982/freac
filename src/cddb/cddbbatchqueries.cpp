 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cddb/cddbbatchqueries.h>

BonkEnc::CDDBBatchQueries::CDDBBatchQueries(Config *iConfig)
{
	config = iConfig;

	ReadEntries();
}

BonkEnc::CDDBBatchQueries::~CDDBBatchQueries()
{
	SaveEntries();
}

Bool BonkEnc::CDDBBatchQueries::ReadEntries()
{
	XML::Document	*document = new XML::Document();

	if (document->LoadFile("cddb\\queries.xml") == Success())
	{
		XML::Node	*root = document->GetRootNode();

		if (root != NIL)
		{
			for (Int i = 0; i < root->GetNOfNodes(); i++)
			{
				XML::Node	*node = root->GetNthNode(i);

				if (node->GetName() == "query") queries.AddEntry(node->GetContent());
			}
		}
	}

	delete document;

	return True;
}

Bool BonkEnc::CDDBBatchQueries::SaveEntries()
{
	if (queries.GetNOfEntries() == 0)
	{
		File("cddb\\queries.xml").Delete();

		return True;
	}

	Directory("cddb").Create();

	XML::Document	*document = new XML::Document();
	XML::Node	*root = new XML::Node("cddbQueries");

	document->SetRootNode(root);

	for (Int i = 0; i < queries.GetNOfEntries(); i++)
	{
		root->AddNode("query", queries.GetNthEntry(i));
	}

	document->SaveFile("cddb\\queries.xml");

	delete document;
	delete root;

	return True;
}

Bool BonkEnc::CDDBBatchQueries::AddQuery(const String &query)
{
	for (Int i = 0; i < queries.GetNOfEntries(); i++)
	{
		if (queries.GetNthEntry(i) == query) return False;
	}

	queries.AddEntry(query);

	return True;
}

Bool BonkEnc::CDDBBatchQueries::DeleteQuery(Int n)
{
	queries.RemoveEntry(queries.GetNthEntryIndex(n));

	return True;
}

Bool BonkEnc::CDDBBatchQueries::Query(Int n)
{
	// Query entry and delete entry if successful

	// TODO: Actual query goes here

	DeleteQuery(n);

	return True;
}

Bool BonkEnc::CDDBBatchQueries::QueryAll()
{
	while (queries.GetNOfEntries() > 0)
	{
		if (!Query(0)) return False;
	}

	return True;
}

const Array<String> &BonkEnc::CDDBBatchQueries::GetQueries()
{
	return queries;
}
