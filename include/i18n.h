 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_I18N_
#define _H_I18N_

#include <smooth.h>

using namespace smooth;
using namespace smooth::XML;

typedef struct
{
	Array<String>	 strings;

	String		 language;
	String		 encoding;
	String		 magic;
	String		 author;
	String		 url;
	Bool		 rightToLeft;
	Bool		 isOutOfDate;
}
bonkEncLanguageInfo;

class bonkTranslator
{
	private:
		Array<bonkEncLanguageInfo *>	 languages;
		bonkEncLanguageInfo		*activeLanguage;

		Int				 GetSupportedLanguages();
		Int				 ReadStrings(Document *, bonkEncLanguageInfo *);

		Int				 GetStringChecksum(String);
	public:
						 bonkTranslator();
						~bonkTranslator();

		Int				 GetNOfLanguages();

		String				 GetNthLanguageName(Int);
		String				 GetNthLanguageID(Int);
		String				 GetNthLanguageAuthor(Int);
		String				 GetNthLanguageEncoding(Int);
		String				 GetNthLanguageURL(Int);

		Int				 ActivateLanguage(String);
		String				 TranslateString(String);
};

#endif
