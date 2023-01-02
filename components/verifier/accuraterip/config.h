 /* fre:ac - free audio converter
  * Copyright (C) 2001-2023 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_ACCURATERIPCONFIG
#define H_ACCURATERIPCONFIG

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA;

namespace BoCA
{
	class ConfigureAccurateRip : public ConfigLayer
	{
		private:
			GroupBox		*group_settings;
			CheckBox		*check_enabled;

			Text			*text_path;
			EditBox			*edit_path;
			List			*list_path;

			GroupBox		*group_drives;
			Text			*text_drive;
			ComboBox		*combo_drive;
			Text			*text_status;
			Text			*text_status_value;
			Text			*text_drivedb;
			Text			*text_drivedb_offset;
			Text			*text_detected;
			Text			*text_detected_offset;
			Text			*text_configured;
			Text			*text_configured_offset;
			Button			*button_configure;

			GroupBox		*group_cache;
			CheckBox		*check_cache;
			Text			*text_expire;
			Slider			*slider_expire;
			Text			*text_expire_value;

			GroupBox		*group_options;
			CheckBox		*check_notify_missing;
			CheckBox		*check_notify_success;

			Bool			 enableAccurateRip;

			Bool			 enableCache;
			Int			 expireCache;

			String			 selectedDrivePath;

			Bool			 notifyMissing;
			Bool			 notifySuccess;

			Void			 UpdateDriveList();

			Int			 CacheDaysToIndex(Int);
			Int			 CacheIndexToDays(Int);
		slots:
			Void			 ToggleEnabled();

			Void			 ToggleCache();
			Void			 SetExpireTime();

			Void			 SelectDrive();
			Void			 DetectOffset();
		public:
			static const String	 ConfigID;
			static const String	 ConfigCategoryRipperID;

			static const String	 ConfigEnabledID;
			static const Bool	 ConfigEnabledDefault;

			static const String	 ConfigPathID;
			static const String	 ConfigPathDefault;

			static const String	 ConfigEnableCacheID;
			static const Bool	 ConfigEnableCacheDefault;

			static const String	 ConfigExpireCacheID;
			static const Int	 ConfigExpireCacheDefault;

			static const String	 ConfigNotifyMissingEntryID;
			static const Bool	 ConfigNotifyMissingEntryDefault;

			static const String	 ConfigNotifySuccessID;
			static const Bool	 ConfigNotifySuccessDefault;

			static const String	 ResourcesFolder;

						 ConfigureAccurateRip();
						~ConfigureAccurateRip();

			Int			 SaveSettings();
	};
};

#endif
