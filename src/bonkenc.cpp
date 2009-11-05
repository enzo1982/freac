 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/dll.h>

Void smooth::AttachDLL(Void *instance)
{
}

Void smooth::DetachDLL()
{
}

#include <bonkenc.h>
#include <resources.h>
#include <stdlib.h>
#include <vector>
#include <time.h>

#include <utilities.h>
#include <dllinterfaces.h>
#include <joblist.h>
#include <jobs/jobmanager.h>

#include <cddb/cddblocal.h>
#include <cddb/cddbremote.h>
#include <dialogs/cddb/query.h>
#include <dialogs/cddb/submit.h>

#include <boca.h>

BonkEnc::BonkEnc	*BonkEnc::BonkEnc::instance 	= NIL;
BoCA::I18n		*BonkEnc::BonkEnc::i18n		= NIL;

/* General application information and fixed settings.
 */
String	 BonkEnc::BonkEnc::appName	= "BonkEnc";
String	 BonkEnc::BonkEnc::appLongName	= "BonkEnc Audio Encoder";
String	 BonkEnc::BonkEnc::version	= "CVS 2009xxyy";
String	 BonkEnc::BonkEnc::shortVersion	= "v1.1";
String	 BonkEnc::BonkEnc::cddbVersion	= "v1.1beta1pre";	// CDDB version may not contain spaces
String	 BonkEnc::BonkEnc::cddbMode	= "submit";
String	 BonkEnc::BonkEnc::website	= "http://www.bonkenc.org/";
String	 BonkEnc::BonkEnc::updatePath	= "http://www.bonkenc.org/eUpdate/eUpdate.xml";

/* Use these settings for debugging.
 */
//String	 BonkEnc::BonkEnc::cddbMode	= "test";
//String	 BonkEnc::BonkEnc::updatePath	= "file://eUpdate/eUpdate.xml";

BonkEnc::BonkEnc::BonkEnc()
{
#ifdef __WIN64__
	version.Append(" (x64)");
#endif
	BoCA::Protocol	*debug = BoCA::Protocol::Get("Debug output");

	debug->Write("Starting BonkEnc...");

	instance	= this;
	i18n		= BoCA::I18n::Get();

#ifdef __WIN32__
	CoInitialize(NIL);
#endif

	encoder		= new Encoder();
	currentConfig	= Config::Get();

	/* Set default comment if not set.
	 */
	BoCA::Config::Get()->SetStringValue(Config::CategoryTagsID, "DefaultComment", BoCA::Config::Get()->GetStringValue(Config::CategoryTagsID, "DefaultComment", String(appLongName).Append(" <").Append(website).Append(">")));

	/* Start job manager.
	 */
	JobManager::Start();

	if (DLLInterfaces::LoadCDRipDLL() == False)	currentConfig->enable_cdrip = False;
	else						currentConfig->enable_cdrip = True;

	if (DLLInterfaces::LoadEUpdateDLL() == False)	currentConfig->enable_eUpdate = False;
	else						currentConfig->enable_eUpdate = True;
}

BonkEnc::BonkEnc::~BonkEnc()
{
	JobManager::Quit();

#ifdef __WIN32__
	if (currentConfig->enable_cdrip) ex_CR_DeInit();
#endif

	if (currentConfig->enable_cdrip)	DLLInterfaces::FreeCDRipDLL();
	if (currentConfig->enable_eUpdate)	DLLInterfaces::FreeEUpdateDLL();

	/* Cleanup deletable objects before deleting encoder and translator.
	 */
	Object::ObjectCleanup();

	delete encoder;

	Config::Free();

	BoCA::Protocol	*debug = BoCA::Protocol::Get("Debug output");

	debug->Write("Leaving BonkEnc...");

	BoCA::Protocol::Free();

#ifdef __WIN32__
	CoUninitialize();
#endif
}

BonkEnc::BonkEnc *BonkEnc::BonkEnc::Get()
{
	return instance;
}
