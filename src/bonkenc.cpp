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

#include <boca.h>
#include <bonkenc.h>

Void smooth::AttachDLL(Void *instance)
{
	/* Init BoCA library.
	 */
	BoCA::Init(BonkEnc::BonkEnc::appName);
}

Void smooth::DetachDLL()
{
	/* Free BoCA library.
	 */
	BoCA::Free();
}

#include <playback.h>
#include <config.h>
#include <utilities.h>
#include <dllinterfaces.h>

#include <jobs/jobmanager.h>
#include <cddb/cddbcache.h>

#include <engine/decoder.h>
#include <engine/encoder.h>

BonkEnc::BonkEnc	*BonkEnc::BonkEnc::instance = NIL;

/* General application information and fixed settings.
 */
String	 BonkEnc::BonkEnc::appName	= "fre:ac";

#ifdef BUILD_VIDEO_DOWNLOADER
String	 BonkEnc::BonkEnc::appLongName	= "fre:ac - free video downloader";
#else
String	 BonkEnc::BonkEnc::appLongName	= "fre:ac - free audio converter";
#endif

String	 BonkEnc::BonkEnc::version	= "CVS 2016xxyy";

#if defined __i386__
String	 BonkEnc::BonkEnc::architecture = "x86";
#elif defined __x86_64__
String	 BonkEnc::BonkEnc::architecture = "x86-64";
#elif defined __arm__
String	 BonkEnc::BonkEnc::architecture = "ARM";
#elif defined __aarch64__
String	 BonkEnc::BonkEnc::architecture = "ARM64";
#elif defined __mips__
String	 BonkEnc::BonkEnc::architecture = "MIPS";
#elif defined __ppc__
String	 BonkEnc::BonkEnc::architecture = "PPC";
#elif defined __ppc64__
String	 BonkEnc::BonkEnc::architecture = "PPC64";
#else
String	 BonkEnc::BonkEnc::architecture = "unknown";
#endif

String	 BonkEnc::BonkEnc::shortVersion	= "v1.1";
String	 BonkEnc::BonkEnc::cddbVersion	= "v1.1beta1pre5";	// CDDB version may not contain spaces
String	 BonkEnc::BonkEnc::cddbMode	= "submit";

String	 BonkEnc::BonkEnc::copyright	= "Copyright (C) 2001-2016 Robert Kausch";

String	 BonkEnc::BonkEnc::website	= "http://www.freac.org/";
String	 BonkEnc::BonkEnc::updatePath	= "http://www.freac.org/eUpdate/eUpdate.xml";

/* Use these settings for debugging.
 */
//String	 BonkEnc::BonkEnc::cddbMode	= "test";
//String	 BonkEnc::BonkEnc::updatePath	= "file://eUpdate/eUpdate.xml";

BonkEnc::BonkEnc::BonkEnc()
{
	BoCA::Protocol	*debug = BoCA::Protocol::Get("Debug output");

	debug->Write("Starting fre:ac...");

	instance = this;

	/* Load configuration.
	 */
	debug->Write("  Loading config...");

	BoCA::Config	*config	= BoCA::Config::Get();

	currentConfig = Config::Get();

	/* Increment start count.
	 */
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsStartCountID, BoCA::Config::Get()->GetIntValue(Config::CategorySettingsID, Config::SettingsStartCountID, Config::SettingsStartCountDefault) + 1);

	/* Set process priority.
	 */
	Utilities::SetProcessPriority();

	/* Set default comment unless already set.
	 */
	config->SetStringValue(Config::CategoryTagsID, Config::TagsDefaultCommentID, BoCA::Config::Get()->GetStringValue(Config::CategoryTagsID, Config::TagsDefaultCommentID, String(appLongName).Append(" <").Append(website).Append(">")));

	/* Connect version information to BoCA callbacks.
	 */
	BoCA::Application::Get()->getClientName.Connect(appLongName);
	BoCA::Application::Get()->getClientVersion.Connect(version);

	/* Start job manager.
	 */
	debug->Write("  Starting job manager...");

	JobManager::Start();

	debug->Write("  Loading online updater...");

	if (DLLInterfaces::LoadEUpdateDLL() == False) currentConfig->enable_eUpdate = False;
	else					      currentConfig->enable_eUpdate = True;

	if (!currentConfig->enable_eUpdate) debug->Write("    Online updater not found.");
}

BonkEnc::BonkEnc::~BonkEnc()
{
	BoCA::Protocol	*debug = BoCA::Protocol::Get("Debug output");

	/* Quit the job manager.
	 */
	debug->Write("  Stopping job manager...");

	JobManager::Quit();

	debug->Write("  Cleaning up...");

	/* Free conversion locks.
	 */
	Decoder::FreeLockObjects();
	Encoder::FreeLockObjects();

	/* Free the audio player.
	 */
	Playback::Free();

	/* Free the CDDB cache.
	 */
	CDDBCache::Free();

	/* Free easyUpdate library.
	 */
	if (currentConfig->enable_eUpdate) DLLInterfaces::FreeEUpdateDLL();

	/* Cleanup deletable objects before deleting encoder and translator.
	 */
	Object::ObjectCleanup();

	Config::Free();

	debug->Write("Leaving fre:ac...");

	BoCA::Protocol::Free();
}
