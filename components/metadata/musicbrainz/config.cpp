 /* fre:ac - free audio converter
  * Copyright (C) 2026 Shen-Ta Hsieh
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "config.h"
#include "network.h"

const String	 BoCA::ConfigureMusicBrainz::ConfigID		= "MusicBrainz";

const String	 BoCA::ConfigureMusicBrainz::ServerID		= "Server";
const String	 BoCA::ConfigureMusicBrainz::ServerDefault	= "https://musicbrainz.org";

BoCA::ConfigureMusicBrainz::ConfigureMusicBrainz()
{
	const Config	*config = Config::Get();
	I18n		*i18n	= I18n::Get();

	i18n->SetContext("Configuration::MusicBrainz");

	group_server	= new GroupBox(i18n->TranslateString("Server"), Point(7, 11), Size(400, 70));

	text_server	= new Text(i18n->AddColon(i18n->TranslateString("Server URL")), Point(16, 26));

	edit_server	= new EditBox(config->GetStringValue(ConfigID, ServerID, ServerDefault), Point(text_server->GetUnscaledTextWidth() + 24, 23), Size(376 - text_server->GetUnscaledTextWidth(), 0), 0);

	list_server	= new List();
	list_server->AddEntry(ServerDefault);

	edit_server->SetDropDownList(list_server);

	text_note	= new Text(i18n->TranslateString("Enter the address of a mirror to use it instead of musicbrainz.org."), Point(16, 53));

	Add(group_server);
	Add(text_server);
	Add(edit_server);
	Add(text_note);

	SetSize(Size(414, 88));
}

BoCA::ConfigureMusicBrainz::~ConfigureMusicBrainz()
{
	DeleteObject(group_server);
	DeleteObject(text_server);
	DeleteObject(edit_server);
	DeleteObject(list_server);
	DeleteObject(text_note);
}

Int BoCA::ConfigureMusicBrainz::SaveSettings()
{
	Config	*config = Config::Get();
	I18n	*i18n	= I18n::Get();

	String	 server = MusicBrainzNetwork::NormalizeServerURL(edit_server->GetText());

	if (!server.StartsWith("http://") && !server.StartsWith("https://"))
	{
		i18n->SetContext("Configuration::MusicBrainz::Errors");

		Utilities::ErrorMessage(i18n->TranslateString("Please enter a server URL that starts with http:// or https://."));

		return Error();
	}

	config->SetStringValue(ConfigID, ServerID, server);

	return Success();
}
