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
#include <smooth/dll.h>

#include "external.h"
#include "config.h"

const String &BoCA::ExternalConfig::GetComponentSpecs()
{
	static String	 componentSpecs = "		\
							\
	  <?xml version=\"1.0\" encoding=\"UTF-8\"?>	\
	  <component>					\
	    <name>External Codec Configurator</name>	\
	    <version>1.0</version>			\
	    <id>external-config</id>			\
	    <type>other</type>				\
	  </component>					\
							\
	";

	return componentSpecs;
}

Void smooth::AttachDLL(Void *instance)
{
}

Void smooth::DetachDLL()
{
}

BoCA::ExternalConfig::ExternalConfig()
{
	configLayer = NIL;
}

BoCA::ExternalConfig::~ExternalConfig()
{
	if (configLayer != NIL) Object::DeleteObject(configLayer);
}

ConfigLayer *BoCA::ExternalConfig::GetConfigurationLayer()
{
	if (configLayer == NIL) configLayer = new ConfigureExternal();

	return configLayer;
}
