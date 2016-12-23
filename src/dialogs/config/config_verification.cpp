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

#include <dialogs/config/config_verification.h>

#include <config.h>

using namespace BoCA::AS;

using namespace smooth::GUI::Dialogs;

freac::ConfigureVerification::ConfigureVerification()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Verification");

	verifyInput  = config->GetIntValue(Config::CategoryVerificationID, Config::VerificationVerifyInputID, Config::VerificationVerifyInputDefault);
	verifyOutput = config->GetIntValue(Config::CategoryVerificationID, Config::VerificationVerifyOutputID, Config::VerificationVerifyOutputDefault);

	group_verification	= new GroupBox(i18n->TranslateString("Verification"), Point(7, 11), Size(350, 128));

	check_input		= new CheckBox(i18n->TranslateString("Enable input verification"), Point(10, 14), Size(330, 0), &verifyInput);
	text_input		= new Text(i18n->TranslateString("Verify that lossless inputs decode to exactly the data\nthey were originally created from."), Point(27, 34));

	check_output		= new CheckBox(i18n->TranslateString("Enable output verification"), Point(10, 70), Size(330, 0), &verifyOutput);
	text_output		= new Text(i18n->TranslateString("Verify that lossless outputs decode to exactly the data\nused when encoding them."), Point(27, 90));

	group_verification->Add(check_input);
	group_verification->Add(text_input);
	group_verification->Add(check_output);
	group_verification->Add(text_output);

	Add(group_verification);

	SetSize(Size(364, 144));
}

freac::ConfigureVerification::~ConfigureVerification()
{
	DeleteObject(group_verification);
	DeleteObject(check_input);
	DeleteObject(text_input);
	DeleteObject(check_output);
	DeleteObject(text_output);
}

Int freac::ConfigureVerification::SaveSettings()
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue(Config::CategoryVerificationID, Config::VerificationVerifyInputID, verifyInput);
	config->SetIntValue(Config::CategoryVerificationID, Config::VerificationVerifyOutputID, verifyOutput);

	return Success();
}
