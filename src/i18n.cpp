 /* BonkEnc version 0.8
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smoothx.h>
#include <i18n.h>

bonkTranslator::bonkTranslator()
{
	GetSupportedLanguages();

	ActivateLanguage("english-internal");
}

bonkTranslator::~bonkTranslator()
{
	for (int i = 0; i < languages.GetNOfEntries(); i++)
	{
		delete languages.GetNthEntry(i);
	}

	languages.DeleteAll();
}

SMOOTHInt bonkTranslator::GetSupportedLanguages()
{
	SMOOTHString		 dir = SMOOTH::StartDirectory;
	_finddata_t		 fileData;
	int			 handle;
	SXMLDocument		*doc = NIL;
	bonkEncLanguageInfo	*language = NIL;

	{
		language = new bonkEncLanguageInfo();

		language->language = "English";
		language->encoding = "UTF-8";
		language->magic = "english-internal";
		language->author = "Robert Kausch <robert.kausch@gmx.net>";
		language->url = "http://www.bonkenc.org";
		language->isOutOfDate = false;

		languages.AddEntry(language);
	}

	dir.Append("lang\\");
	chdir(dir);

	if ((handle = _findfirst("lang_*.xml", &fileData)) != -1)
	{
		do
		{
			doc = new SXMLDocument();
			language = new bonkEncLanguageInfo();

			doc->LoadFile(fileData.name);

			SXMLNode	*info = doc->GetRootNode()->GetNodeByName("info");
			SMOOTHString	 program;
			SMOOTHString	 version;

			language->magic = fileData.name;

			for (int i = 0; i < info->GetNOfNodes(); i++)
			{
				SMOOTHString	 property = info->GetNthNode(i)->GetAttributeByName("name")->GetContent();

				if (property == "program")	 program = info->GetNthNode(i)->GetContent();
				if (property == "version")	 version = info->GetNthNode(i)->GetContent();
				if (property == "language")	 language->language = info->GetNthNode(i)->GetContent();
				if (property == "encoding")	 language->encoding = info->GetNthNode(i)->GetContent();
				if (property == "author")	 language->author = info->GetNthNode(i)->GetContent();
				if (property == "url")		 language->url = info->GetNthNode(i)->GetContent();
			}

			if (program != "BonkEnc")
			{
				delete language;
			}
			else
			{
				if (version[0] == '0' && version[2] < '8' && version[3] == 0)	language->isOutOfDate = true;
				else								language->isOutOfDate = false;

				languages.AddEntry(language);
			}

			ReadStrings(doc, language);

			delete doc;
		}
		while (_findnext(handle, &fileData) == 0);
	}

	_findclose(handle);

	chdir(SMOOTH::StartDirectory);

	return SMOOTH::Success;
}

SMOOTHInt bonkTranslator::GetStringChecksum(SMOOTHString string)
{
	SMOOTHInt	 checksum = (string.Length() & 127) << 24;
	SMOOTHInt	 value = 0;

	for (SMOOTHInt i = 0; i < string.Length(); i++) value += string[i];

	checksum += ((value & 65535) << 8);
	checksum += string[0];

	return checksum;
}

SMOOTHInt bonkTranslator::GetNOfLanguages()
{
	return languages.GetNOfEntries();
}

SMOOTHString bonkTranslator::GetNthLanguageName(SMOOTHInt index)
{
	return languages.GetNthEntry(index)->language;
}

SMOOTHString bonkTranslator::GetNthLanguageID(SMOOTHInt index)
{
	return languages.GetNthEntry(index)->magic;
}

SMOOTHString bonkTranslator::GetNthLanguageAuthor(SMOOTHInt index)
{
	return languages.GetNthEntry(index)->author;
}

SMOOTHString bonkTranslator::GetNthLanguageEncoding(SMOOTHInt index)
{
	return languages.GetNthEntry(index)->encoding;
}

SMOOTHString bonkTranslator::GetNthLanguageURL(SMOOTHInt index)
{
	return languages.GetNthEntry(index)->url;
}

SMOOTHInt bonkTranslator::ActivateLanguage(SMOOTHString magic)
{
	for (int i = 0; i < languages.GetNOfEntries(); i++)
	{
		if (languages.GetNthEntry(i)->magic == magic)
		{
			activeLanguage = languages.GetNthEntry(i);

			return SMOOTH::Success;
		}
	}

	return SMOOTH::Error;
}

SMOOTHString bonkTranslator::TranslateString(SMOOTHString string)
{
	SMOOTHString	 translation = activeLanguage->strings.GetEntry(GetStringChecksum(string));

	if (translation == NIL)	return string;
	else			return translation;
}

SMOOTHInt bonkTranslator::ReadStrings(SMOOTHXMLDocument *language, bonkEncLanguageInfo *info)
{
	SXMLNode	*entry = language->GetRootNode()->GetNodeByName("entry");

	do
	{
		if (entry->GetName() == "entry")
		{
			SMOOTHXMLNode	*property = entry->GetNodeByName("property");
			SMOOTHString	 translation;
			SMOOTHInt	 checksum = 0;

			do
			{
				if (property->GetName() == "property")
				{
					if (property->GetAttributeByName("name")->GetContent() == "string")
					{
						checksum = GetStringChecksum(property->GetContent());
					}
					else if (property->GetAttributeByName("name")->GetContent() == "translation")
					{
						translation = property->GetContent();
					}
				}

				property = property->GetNextNode();
			}
			while (property != NIL);

			info->strings.AddEntry(translation, checksum);
		}

		entry = entry->GetNextNode();
	}
	while (entry != NIL);

	return SMOOTH::Success;
}
