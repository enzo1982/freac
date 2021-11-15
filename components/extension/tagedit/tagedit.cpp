 /* fre:ac - free audio converter
  * Copyright (C) 2001-2021 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "tagedit.h"
#include "config.h"

const String &BoCA::TagEdit::GetComponentSpecs()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Components::Extensions");

	static String	 componentSpecs = String("					\
											\
	  <?xml version=\"1.0\" encoding=\"UTF-8\"?>					\
	  <component>									\
	    <name>").Append(i18n->TranslateString("Tag Editor")).Append("</name>	\
	    <version>1.0</version>							\
	    <id>tagedit-ext</id>							\
	    <type>extension</type>							\
	  </component>									\
											\
	");

	return componentSpecs;
}

BoCA::TagEdit::TagEdit()
{
	configLayer  = NIL;
	mainTabLayer = NIL;

	getMainTabLayer.Connect(&TagEdit::GetMainTabLayer, this);
}

BoCA::TagEdit::~TagEdit()
{
	if (configLayer	 != NIL) Object::DeleteObject(configLayer);
	if (mainTabLayer != NIL) Object::DeleteObject(mainTabLayer);
}

ConfigLayer *BoCA::TagEdit::GetConfigurationLayer()
{
	if (configLayer == NIL) configLayer = new ConfigureTagEdit();

	return configLayer;
}

Layer *BoCA::TagEdit::GetMainTabLayer()
{
	if (mainTabLayer == NIL) mainTabLayer = new LayerTags();

	return mainTabLayer;
}
