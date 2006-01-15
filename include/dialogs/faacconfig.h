 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FAACCONFIG_
#define _H_FAACCONFIG_

#include <smooth.h>
#include <main.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BonkEnc
{
	class ConfigureFAAC : public Dialog
	{
		private:
			TabWidget	*tabwidget;

			Layer		*layer_format;

			GroupBox	*group_version;
			OptionBox	*option_version_mpeg2;
			OptionBox	*option_version_mpeg4;

			GroupBox	*group_aactype;
			OptionBox	*option_aactype_main;
			OptionBox	*option_aactype_low;
			OptionBox	*option_aactype_ltp;

			GroupBox	*group_id3v2;
			CheckBox	*check_id3v2;
			Text		*text_note;
			Text		*text_id3v2;

			GroupBox	*group_mp4;
			OptionBox	*option_mp4;
			OptionBox	*option_aac;

			Layer		*layer_quality;

			GroupBox	*group_bitrate;
			Slider		*slider_bitrate;
			OptionBox	*option_bitrate;
			EditBox		*edit_bitrate;
			Text		*text_bitrate_kbps;
			Slider		*slider_quality;
			OptionBox	*option_quality;
			EditBox		*edit_quality;
			Text		*text_quality_percent;

			GroupBox	*group_js;
			CheckBox	*check_js;

			GroupBox	*group_tns;
			CheckBox	*check_tns;

			GroupBox	*group_bandwidth;
			Text		*text_bandwidth;
			EditBox		*edit_bandwidth;

			Divider		*divbar;

			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			Button		*btn_cancel;
			Button		*btn_ok;

			Int		 mpegVersion;
			Int		 aacType;
			Int		 bitrate;
			Bool		 allowjs;
			Bool		 usetns;
			Int		 setQuality;
			Int		 aacQuality;
			Bool		 allowID3;
			Int		 fileFormat;

			Config		*currentConfig;

			Void		 OK();
			Void		 Cancel();
			Void		 SetMPEGVersion();
			Void		 SetBitrate();
			Void		 SetBitrateByEditBox();
			Void		 SetQuality();
			Void		 SetQualityByEditBox();
			Void		 SetFileFormat();
			Void		 ToggleBitrateQuality();
		public:
					 ConfigureFAAC();
					~ConfigureFAAC();

			const Error	&ShowDialog();
	};
};

#endif
