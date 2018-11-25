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

#include "notifier.h"
#include "config.h"

using namespace smooth::GUI::Dialogs;

const String &BoCA::Notifier::GetComponentSpecs()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Components::Extensions");

	static String	 componentSpecs = String("						\
												\
	  <?xml version=\"1.0\" encoding=\"UTF-8\"?>						\
	  <component>										\
	    <name>").Append(i18n->TranslateString("Conversion Notifier")).Append("</name>	\
	    <version>1.0</version>								\
	    <id>notifier-ext</id>								\
	    <type>extension</type>								\
	  </component>										\
												\
	");

	return componentSpecs;
}

BoCA::Notifier::Notifier()
{
	configLayer = NIL;

	/* Connect slots.
	 */
	Engine	*engine = Engine::Get();

	engine->onStartConversion.Connect(&Notifier::OnStartConversion, this);
	engine->onFinishConversion.Connect(&Notifier::OnFinishConversion, this);
	engine->onCancelConversion.Connect(&Notifier::OnCancelConversion, this);
}

BoCA::Notifier::~Notifier()
{
	if (configLayer != NIL) Object::DeleteObject(configLayer);

	/* Disconnect slots.
	 */
	Engine	*engine = Engine::Get();

	engine->onStartConversion.Disconnect(&Notifier::OnStartConversion, this);
	engine->onFinishConversion.Disconnect(&Notifier::OnFinishConversion, this);
	engine->onCancelConversion.Disconnect(&Notifier::OnCancelConversion, this);
}

Void BoCA::Notifier::OnStartConversion(Int id, const Array<Track> &tracks)
{
	/* Add conversion start time to list.
	 */
	conversions.Add(S::System::System::Clock(), id);
}

Void BoCA::Notifier::OnFinishConversion(Int id)
{
	/* Get configuration.
	 */
	Config	*config = Config::Get();

	Bool	 enableNotifications = config->GetIntValue(ConfigureNotifier::ConfigID, "EnableNotifications", False);

	Bool	 playSound	     = config->GetIntValue(ConfigureNotifier::ConfigID, "PlaySound", True);
	Bool	 displayMessage	     = config->GetIntValue(ConfigureNotifier::ConfigID, "DisplayMessage", False);
	Bool	 minimumDuration     = config->GetIntValue(ConfigureNotifier::ConfigID, "MinimumDuration", False);

	String	 soundfile	     = config->GetStringValue(ConfigureNotifier::ConfigID, "SoundFile", String("<sounds>/finished.flac").Replace("/", Directory::GetDirectoryDelimiter()));
	String	 message	     = config->GetStringValue(ConfigureNotifier::ConfigID, "Message", ConfigureNotifier::DefaultMessage);

	Int	 seconds	     = config->GetIntValue(ConfigureNotifier::ConfigID, "Seconds", 30);

	/* Check conversion duration.
	 */
	UnsignedInt64	 duration = S::System::System::Clock() - conversions.Get(id);

	conversions.Remove(id);

	if (!enableNotifications || (minimumDuration && duration / 1000.0 < seconds)) return;

	/* Play sound if requested.
	 */
	if (playSound)
	{
		Directory	 dir(Utilities::GetBoCADirectory().Append(ConfigureNotifier::ResourcesFolder).Append("/sounds"));

		if (!dir.Exists()) dir = Utilities::GetBoCADirectory().Append("../freac/").Append(ConfigureNotifier::ResourcesFolder).Append("/sounds");

		player.Play(String(soundfile).Replace("<sounds>", dir));
	}

	/* DisplayMessage if requested.
	 */
	if (displayMessage)
	{
		I18n	*i18n = I18n::Get();

		i18n->SetContext("Extensions::Notifier");

		QuickMessage(i18n->TranslateString(message), i18n->TranslateString("Info"), Message::Buttons::Ok, Message::Icon::Information);
	}
}

Void BoCA::Notifier::OnCancelConversion(Int id)
{
	/* Remove conversion from list.
	 */
	conversions.Remove(id);
}

ConfigLayer *BoCA::Notifier::GetConfigurationLayer()
{
	if (configLayer == NIL) configLayer = new ConfigureNotifier();

	return configLayer;
}
