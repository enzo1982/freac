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

#include <smooth.h>

#include "langnotify.h"
#include "dialog.h"

const String &BoCA::LanguageNotifier::GetComponentSpecs()
{
	static String	 componentSpecs = "		\
							\
	  <?xml version=\"1.0\" encoding=\"UTF-8\"?>	\
	  <component>					\
	    <name>LanguageNotifier</name>		\
	    <version>1.0</version>			\
	    <id>langnotify-ext</id>			\
	    <type>extension</type>			\
	  </component>					\
							\
	";

	return componentSpecs;
}

BoCA::LanguageNotifier::LanguageNotifier()
{
	static Bool	 checkedLanguage = False;

	dialog = NIL;

	if (checkedLanguage) return;

	checkedLanguage = True;

	/* Check if user default language is supported.
	 */
	I18n	*i18n	    = I18n::Get();
	String	 code	    = i18n->GetUserDefaultLanguageCode();

	if (code == "en" || code.StartsWith("en_") || code.StartsWith("en@")) return;

	Bool	 supported  = False;
	Bool	 incomplete = False;

	for (Int i = 0; i < i18n->GetNOfLanguages(); i++)
	{
		if ((						 i18n->GetNthLanguageID(i) == String("freac_").Append(code									 ).Append(".xml")) ||
		    (			   code.Contains("@") && i18n->GetNthLanguageID(i) == String("freac_").Append(					code.Head(		  code.Find("@"))).Append(".xml")) ||
		    (code.Contains("_") && code.Contains("@") && i18n->GetNthLanguageID(i) == String("freac_").Append(code.Head(code.Find("_"))).Append(code.Tail(code.Length() - code.Find("@"))).Append(".xml")) ||
		    (code.Contains("_") &&			 i18n->GetNthLanguageID(i) == String("freac_").Append(code.Head(code.Find("_"))							 ).Append(".xml")))
		{
			supported = True;

			if (i18n->IsNthLanguageIncomplete(i)) incomplete = True;

			break;
		}
	}

	if (supported && !incomplete) return;

	/* Display notification dialog at every fourth start.
	 */
	Config	*config = Config::Get();

	Int	 startCount = config->GetIntValue("LanguageNotifier", "StartCount", 0) + 1;

	if ((startCount - 5) % 10 == 0 && config->GetIntValue("LanguageNotifier", "ShowAgain", True))
	{
		dialog = new LanguageNotifierDialog(incomplete);

		dialog->ShowDialog();
	}

	config->SetIntValue("LanguageNotifier", "StartCount", startCount);
}

BoCA::LanguageNotifier::~LanguageNotifier()
{
	if (dialog != NIL) Object::DeleteObject(dialog);
}
