 /* BonkEnc version 0.7
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_I18N_
#define _H_I18N_

#include <smoothx.h>

typedef struct
{
	SMOOTHArray<SMOOTHString>	 strings;

	SMOOTHString			 language;
	SMOOTHString			 encoding;
	SMOOTHString			 magic;
	SMOOTHString			 author;
	SMOOTHString			 url;
	SMOOTHBool			 isOutOfDate;
}
bonkEncLanguageInfo;

class bonkTranslator
{
	private:
		SMOOTHArray<bonkEncLanguageInfo *>	 languages;
		bonkEncLanguageInfo			*activeLanguage;

		SMOOTHInt				 GetSupportedLanguages();
		SMOOTHInt				 ReadStrings(SMOOTHXMLDocument *, bonkEncLanguageInfo *);

		SMOOTHInt				 GetStringChecksum(SMOOTHString);
	public:
							 bonkTranslator();
							~bonkTranslator();

		SMOOTHInt				 GetNOfLanguages();

		SMOOTHString				 GetNthLanguageName(SMOOTHInt);
		SMOOTHString				 GetNthLanguageID(SMOOTHInt);
		SMOOTHString				 GetNthLanguageAuthor(SMOOTHInt);
		SMOOTHString				 GetNthLanguageEncoding(SMOOTHInt);
		SMOOTHString				 GetNthLanguageURL(SMOOTHInt);

		SMOOTHInt				 ActivateLanguage(SMOOTHString);
		SMOOTHString				 TranslateString(SMOOTHString);
};

#endif
