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

#include <smooth.h>
#include <smooth/dll.h>

#include "protocols.h"
#include "config.h"

const String &BoCA::Protocols::GetComponentSpecs()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Components::Extensions");

	static String	 componentSpecs = String("					\
											\
	  <?xml version=\"1.0\" encoding=\"UTF-8\"?>					\
	  <component>									\
	    <name>").Append(i18n->TranslateString("Protocol Viewer")).Append("</name>	\
	    <version>1.0</version>							\
	    <id>protocols-ext</id>							\
	    <type>extension</type>							\
	    <succeed>tagedit-ext</succeed>						\
	    <succeed>youtube-ext</succeed>						\
	  </component>									\
											\
	");

	return componentSpecs;
}

Void smooth::AttachDLL(Void *instance)
{
}

Void smooth::DetachDLL()
{
}

BoCA::Protocols::Protocols()
{
	configLayer  = NIL;
	mainTabLayer = NIL;

	getMainTabLayer.Connect(&Protocols::GetMainTabLayer, this);
}

BoCA::Protocols::~Protocols()
{
	if (configLayer	 != NIL) Object::DeleteObject(configLayer);
	if (mainTabLayer != NIL) Object::DeleteObject(mainTabLayer);
}

ConfigLayer *BoCA::Protocols::GetConfigurationLayer()
{
	if (configLayer == NIL) configLayer = new ConfigureProtocols();

	return configLayer;
}

Layer *BoCA::Protocols::GetMainTabLayer()
{
	const Config	*config = GetConfiguration();

	if (!config->GetIntValue(ConfigureProtocols::ConfigID, "ShowProtocolsTab", False)) return NIL;

	if (mainTabLayer == NIL) mainTabLayer = new LayerProtocols();

	return mainTabLayer;
}
