 /* BonkEnc Audio Encoder
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

	languages.RemoveAll();
}

Int bonkTranslator::GetSupportedLanguages()
{
	String			 dir = Application::GetApplicationDirectory().Append("lang\\");
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
		language->rightToLeft = False;
		language->isOutOfDate = False;

		languages.AddEntry(language);
	}

	chdir(dir);

	if ((handle = _findfirst("bonkenc_*.xml", &fileData)) != -1)
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
				if (property == "righttoleft")	 language->rightToLeft = (info->GetNthNode(i)->GetContent() == "true" ? True : False);
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
				if (version[0] < '1')	language->isOutOfDate = true;
				else			language->isOutOfDate = false;

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

	chdir(Application::GetApplicationDirectory());

	return Success;
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

			Setup::rightToLeft = activeLanguage->rightToLeft;

			return Success;
		}
	}

	return Error;
}

String bonkTranslator::TranslateString(String string)
{
	String	 translation = activeLanguage->strings.GetEntry(string.ComputeCRC32());

	if (translation == NIL)	return string;
	else			return translation;
}

Int bonkTranslator::ReadStrings(Document *language, bonkEncLanguageInfo *info)
{
	Node	*entry = language->GetRootNode()->GetNodeByName("data")->GetNodeByName("entry");

	while (entry != NIL)
	{
		if (entry->GetName() == "entry")
		{
			info->strings.AddEntry(entry->GetContent(), entry->GetAttributeByName("string")->GetContent().ComputeCRC32());
		}

		entry = entry->GetNextNode();
	}

	return Success;
}
