 /* fre:ac - free audio converter
  * Copyright (C) 2001-2020 Robert Kausch <robert.kausch@freac.org>
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

#include <boca.h>
#include <freac.h>

#include <playback.h>
#include <config.h>
#include <utilities.h>
#include <dllinterfaces.h>

#include <jobs/jobmanager.h>
#include <cddb/cddbcache.h>

#include <engine/component.h>

freac::freac	*freac::freac::instance = NIL;

/* General application information and fixed settings.
 */
String	 freac::freac::appName	      = "fre:ac";

#ifndef BUILD_VIDEO_DOWNLOADER
String	 freac::freac::appLongName    = "fre:ac - free audio converter";
#else
String	 freac::freac::appLongName    = "fre:ac - free video downloader";
#endif

#ifdef FREAC_VERSION
String	 freac::freac::version	      = FREAC_VERSION;
#else
#	ifdef FREAC_DATE
String	 freac::freac::version	      = "v1.1 RC 2";//(" FREAC_DATE ")";
#	else
String	 freac::freac::version	      = "v1.1 RC 2";
#	endif
#endif

#if defined __i386__ || defined _M_IX86
String	 freac::freac::architecture   = "x86";
#elif defined __x86_64__ || defined _M_AMD64
String	 freac::freac::architecture   = "x86-64";
#elif defined __arm__ || defined _M_ARM
String	 freac::freac::architecture   = "ARM";
#elif defined __aarch64__ || defined _M_ARM64
String	 freac::freac::architecture   = "ARM64";
#elif defined __mips__
String	 freac::freac::architecture   = "MIPS";
#elif defined __ppc__
String	 freac::freac::architecture   = "PPC";
#elif defined __ppc64__
String	 freac::freac::architecture   = "PPC64";
#else
String	 freac::freac::architecture   = "unknown";
#endif

String	 freac::freac::shortVersion   = "v1.1";
String	 freac::freac::cddbVersion    = "v1.1rc2"; // CDDB version may not contain spaces
String	 freac::freac::cddbMode	      = "submit";

String	 freac::freac::copyright      = "Copyright (C) 2001-2020 Robert Kausch";

String	 freac::freac::website	      = "https://www.freac.org/";

String	 freac::freac::issueTracker   = "https://github.com/enzo1982/freac/labels/bug";
String	 freac::freac::featureTracker = "https://github.com/enzo1982/freac/labels/enhancement";

#if defined __i386__
String	 freac::freac::updatePath     = "https://www.freac.org/eUpdate/eUpdate.xml";
#else
String	 freac::freac::updatePath     = String("https://www.freac.org/eUpdate/eUpdate-").Append(freac::freac::architecture).Append(".xml");
#endif

/* Use these settings for debugging.
 */
//String	 freac::freac::cddbMode	      = "test";
//String	 freac::freac::updatePath     = "file://eUpdate/eUpdate.xml";

freac::freac::freac() : Application(appName)
{
	/* Init BoCA library.
	 */
	BoCA::Init("fre:ac");

	/* Setup debug protocol.
	 */
	BoCA::Protocol	*debug = BoCA::Protocol::Get("Debug output");

	debug->Write("Starting fre:ac...");

	instance = this;

	/* Load configuration.
	 */
	debug->Write("  Loading config...");

	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	currentConfig = Config::Get();

	/* Set interface language.
	 */
	String	 language = config->GetStringValue(Config::CategorySettingsID, Config::SettingsLanguageID, Config::SettingsLanguageDefault);

	if (language != NIL) i18n->ActivateLanguage(language);
	else		     i18n->SelectUserDefaultLanguage();

	config->SetStringValue(Config::CategorySettingsID, Config::SettingsLanguageID, i18n->GetActiveLanguageID());

	/* Increment start count.
	 */
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsStartCountID, config->GetIntValue(Config::CategorySettingsID, Config::SettingsStartCountID, Config::SettingsStartCountDefault) + 1);

	/* Set process priority.
	 */
	Utilities::SetProcessPriority();

	/* Set default comment unless already set.
	 */
	config->SetStringValue(Config::CategoryTagsID, Config::TagsDefaultCommentID, config->GetStringValue(Config::CategoryTagsID, Config::TagsDefaultCommentID, String(appLongName).Append(" <").Append(website).Append(">")));

	/* Connect version information to BoCA callbacks.
	 */
	BoCA::Application::Get()->getScreenName.Connect(appLongName);

	BoCA::Application::Get()->getClientName.Connect(appName);
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

freac::freac::~freac()
{
	BoCA::Protocol	*debug = BoCA::Protocol::Get("Debug output");

	/* Quit the job manager.
	 */
	debug->Write("  Stopping job manager...");

	JobManager::Quit();

	debug->Write("  Cleaning up...");

	/* Free conversion locks.
	 */
	Component::FreeLockObjects();

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

	/* Free BoCA library.
	 */
	BoCA::Free();
}
