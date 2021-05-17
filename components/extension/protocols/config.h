 /* fre:ac - free audio converter
  * Copyright (C) 2001-2021 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_PROTOCOLS_CONFIG
#define H_PROTOCOLS_CONFIG

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA;

namespace BoCA
{
	class ConfigureProtocols : public ConfigLayer
	{
		private:
			GroupBox		*group_ui;
			CheckBox		*check_ui_showTab;

			GroupBox		*group_logs;
			CheckBox		*check_logs_archive;
			Text			*text_logs_folder;
			EditBox			*edit_logs_folder;
			Button			*btn_logs_select;
			CheckBox		*check_logs_delete;
			EditBox			*edit_logs_days;
			Text			*text_logs_days;

			GroupBox		*group_conversion;
			CheckBox		*check_complete_paths;
			CheckBox		*check_conversion_logs;
			CheckBox		*check_conversion_ripping;
			Text			*text_conversion_filename;
			EditBox			*edit_conversion_filename;
			List			*list_conversion_filename;

			Bool			 showLogsTab;

			Bool			 archiveLogs;
			Bool			 deleteLogs;
			Bool			 logCompletePaths;
			Bool			 saveConversionLogsWithFiles;
			Bool			 saveRippingLogsOnly;
		public:
			static const String	 ConfigID;

						 ConfigureProtocols();
						~ConfigureProtocols();

			Int			 SaveSettings();
		slots:
			Void			 ToggleArchiveLogs();
			Void			 ToggleDeleteLogs();
			Void			 ToggleSaveConversionLogsWithFiles();

			Void			 SelectFolder();
	};
};

#endif
