 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <i18n.h>
#include <direct.h>

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

Int bonkTranslator::GetSupportedLanguages()
{
	String			 dir = SMOOTH::StartDirectory;
	_finddata_t		 fileData;
	int			 handle;
	Document		*doc = NIL;
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
			doc = new Document();
			language = new bonkEncLanguageInfo();

			doc->LoadFile(fileData.name);

			Node	*info = doc->GetRootNode()->GetNodeByName("info");
			String	 program;
			String	 version;

			language->magic = fileData.name;

			for (int i = 0; i < info->GetNOfNodes(); i++)
			{
				String	 property = info->GetNthNode(i)->GetAttributeByName("name")->GetContent();

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

				bool	 done = false;

				for (int i = 0; i < languages.GetNOfEntries(); i++)
				{
					bonkEncLanguageInfo	*lang = languages.GetNthEntry(i);

					for (int j = 0; j < max(language->language.Length(), lang->language.Length()); j++)
					{
						if (language->language[j] < lang->language[j])
						{
							languages.InsertEntryAtPos(i, language);

							done = true;

							break;
						}
						else if (language->language[j] > lang->language[j])
						{
							break;
						}
					}

					if (i == languages.GetNOfEntries() - 1)
					{
						languages.AddEntry(language);

						done = true;
					}

					if (done) break;
				}
			}

			ReadStrings(doc, language);

			delete doc;
		}
		while (_findnext(handle, &fileData) == 0);
	}

	_findclose(handle);

	chdir(SMOOTH::StartDirectory);

	return Success;
}

Int bonkTranslator::GetStringChecksum(String string)
{
	Int	 checksum = (string.Length() & 127) << 24;
	Int	 value = 0;

	for (Int i = 0; i < string.Length(); i++) value += ((wchar_t *) string)[i];

	checksum += ((value & 65535) << 8);
	checksum += string[0];

	return checksum;
}

Int bonkTranslator::GetNOfLanguages()
{
	return languages.GetNOfEntries();
}

String bonkTranslator::GetNthLanguageName(Int index)
{
	return languages.GetNthEntry(index)->language;
}

String bonkTranslator::GetNthLanguageID(Int index)
{
	return languages.GetNthEntry(index)->magic;
}

String bonkTranslator::GetNthLanguageAuthor(Int index)
{
	return languages.GetNthEntry(index)->author;
}

String bonkTranslator::GetNthLanguageEncoding(Int index)
{
	return languages.GetNthEntry(index)->encoding;
}

String bonkTranslator::GetNthLanguageURL(Int index)
{
	return languages.GetNthEntry(index)->url;
}

Int bonkTranslator::ActivateLanguage(String magic)
{
	for (int i = 0; i < languages.GetNOfEntries(); i++)
	{
		if (languages.GetNthEntry(i)->magic == magic)
		{
			activeLanguage = languages.GetNthEntry(i);

			return Success;
		}
	}

	return Error;
}

String bonkTranslator::TranslateString(String string)
{
	String	 translation = activeLanguage->strings.GetEntry(GetStringChecksum(string));

	if (translation == NIL)	return string;
	else			return translation;
}

Int bonkTranslator::ReadStrings(Document *language, bonkEncLanguageInfo *info)
{
	Node	*entry = language->GetRootNode()->GetNodeByName("entry");

	while (entry != NIL)
	{
		if (entry->GetName() == "entry")
		{
			Node	*property = entry->GetNodeByName("property");
			String	 translation;
			Int	 checksum = 0;

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

	return Success;
}
