 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "tagspec.h"

Int BoCA::TagSpec::LoadFromXML(const String &fileName)
{
	XML::Document	*document = new XML::Document();

	document->LoadFile(fileName);

	XML::Node	*root = document->GetRootNode();

	for (Int i = 0; i < root->GetNOfNodes(); i++)
	{
		XML::Node	*node = root->GetNthNode(i);

		if	(node->GetName() == "name")	name	= node->GetContent();
		else if (node->GetName() == "fields")
		{
			for (Int j = 0; j < node->GetNOfNodes(); j++)
			{
				XML::Node	*node2 = node->GetNthNode(j);

				if (node2->GetName() != "field") continue;

				String		 typeString = node2->GetAttributeByName("type")->GetContent();
				Int		 type = FIELD_TYPE_UNKNOWN;

				if	(typeString == "text")	  type = FIELD_TYPE_TEXT;
				else if	(typeString == "integer") type = FIELD_TYPE_INTEGER;
				else if	(typeString == "data")	  type = FIELD_TYPE_DATA;
				else if	(typeString == "genre")	  type = FIELD_TYPE_GENRE;
				else if	(typeString == "picture") type = FIELD_TYPE_PICTURE;

				fields.Add(TagField(node2->GetAttributeByName("name")->GetContent(), type, node2->GetContent()));
			}
		}
	}

	delete document;

	return Success();
}
