 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/dll.h>

#include <boca.h>

Void smooth::AttachDLL(Void *instance)
{
	/* Init BoCA library.
	 */
	BoCA::Init();
}

Void smooth::DetachDLL()
{
	/* Free BoCA library.
	 */
	BoCA::Free();
}

#include <bonkenc.h>
#include <playback.h>
#include <config.h>
#include <dllinterfaces.h>

#include <jobs/jobmanager.h>
#include <cddb/cddbcache.h>
#include <engine/converter.h>

BonkEnc::BonkEnc	*BonkEnc::BonkEnc::instance = NIL;

/* General application information and fixed settings.
 */
String	 BonkEnc::BonkEnc::appName	= "fre:ac";

#ifdef BUILD_VIDEO_DOWNLOADER
String	 BonkEnc::BonkEnc::appLongName	= "fre:ac - free video downloader";
#else
String	 BonkEnc::BonkEnc::appLongName	= "fre:ac - free audio converter";
#endif

String	 BonkEnc::BonkEnc::version	= "CVS 2013xxyy";

#if defined __i386__
String	 BonkEnc::BonkEnc::architecture = "x86";
#elif defined __x86_64__
String	 BonkEnc::BonkEnc::architecture = "x86-64";
#else
String	 BonkEnc::BonkEnc::architecture = "unknown";
#endif

String	 BonkEnc::BonkEnc::shortVersion	= "v1.1";
String	 BonkEnc::BonkEnc::cddbVersion	= "v1.1beta1pre3";	// CDDB version may not contain spaces
String	 BonkEnc::BonkEnc::cddbMode	= "submit";
String	 BonkEnc::BonkEnc::website	= "http://www.freac.org/";
String	 BonkEnc::BonkEnc::updatePath	= "http://www.freac.org/eUpdate/eUpdate.xml";

/* Use these settings for debugging.
 */
//String	 BonkEnc::BonkEnc::cddbMode	= "test";
//String	 BonkEnc::BonkEnc::updatePath	= "file://eUpdate/eUpdate.xml";

BonkEnc::BonkEnc::BonkEnc()
{
#ifdef __x86_64__
	version.Append(" (x64)");
#endif

	BoCA::Protocol	*debug = BoCA::Protocol::Get("Debug output");

	debug->Write("Starting fre:ac...");

	instance = this;

	encoder		= new Converter();
	currentConfig	= Config::Get();

	/* Set default comment unless already set.
	 */
	BoCA::Config::Get()->SetStringValue(Config::CategoryTagsID, Config::TagsDefaultCommentID, BoCA::Config::Get()->GetStringValue(Config::CategoryTagsID, Config::TagsDefaultCommentID, String(appLongName).Append(" <").Append(website).Append(">")));

	/* Connect version information to BoCA callbacks.
	 */
	BoCA::Application::Get()->getClientName.Connect(appLongName);
	BoCA::Application::Get()->getClientVersion.Connect(version);

	/* Start job manager.
	 */
	JobManager::Start();

	if (DLLInterfaces::LoadEUpdateDLL() == False)	currentConfig->enable_eUpdate = False;
	else						currentConfig->enable_eUpdate = True;
}

BonkEnc::BonkEnc::~BonkEnc()
{
	/* Quit the job manager.
	 */
	JobManager::Quit();

	/* Free the audio player.
	 */
	Playback::Free();

	/* Free the CDDB cache.
	 */
	CDDBCache::Free();

	/* Set first start flag to false.
	 */
	BoCA::Config::Get()->SetIntValue(Config::CategorySettingsID, Config::SettingsFirstStartID, False);

	/* Free easyUpdate library.
	 */
	if (currentConfig->enable_eUpdate) DLLInterfaces::FreeEUpdateDLL();

	/* Cleanup deletable objects before deleting encoder and translator.
	 */
	Object::ObjectCleanup();

	delete encoder;

	Config::Free();

	BoCA::Protocol	*debug = BoCA::Protocol::Get("Debug output");

	debug->Write("Leaving fre:ac...");

	BoCA::Protocol::Free();
}

BonkEnc::BonkEnc *BonkEnc::BonkEnc::Get()
{
	return instance;
}
