 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_GENCONFIG_
#define _H_GENCONFIG_

#include <smooth.h>
#include <main.h>

using namespace smooth;
using namespace smooth::GUI;

class configureGeneralSettings : public Dialog
{
	private:
		TabWidget	*reg_register;

		Layer		*register_layer_encoders;
		Layer		*register_layer_language;
		Layer		*register_layer_cdrip;
		Layer		*register_layer_cddb;
		Layer		*register_layer_plugins;
		Layer		*register_layer_tags;

		GroupBox	*encoders_group_encoder;
		ComboBox	*encoders_combo_encoder;
		Button		*encoders_button_config;
		GroupBox	*encoders_group_outdir;
		EditBox		*encoders_edit_outdir;
		Button		*encoders_button_outdir_browse;
		GroupBox	*encoders_group_filename;
		EditBox		*encoders_edit_filename;
		ListBox		*encoders_list_filename;

		GroupBox	*language_group_language;
		Text		*language_text_language;
		ComboBox	*language_combo_language;
		GroupBox	*language_group_info;
		Text		*language_text_info;
		Hyperlink	*language_link_url;

		GroupBox	*cdrip_group_drive;
		ComboBox	*cdrip_combo_drive;
		GroupBox	*cdrip_group_ripping;
		CheckBox	*cdrip_check_paranoia;
		ComboBox	*cdrip_combo_paranoia_mode;
		CheckBox	*cdrip_check_jitter;
		CheckBox	*cdrip_check_swapchannels;
		CheckBox	*cdrip_check_locktray;
		CheckBox	*cdrip_check_ntscsi;

		GroupBox	*cddb_group_cddb;
		CheckBox	*cddb_check_auto;
		CheckBox	*cddb_check_cache;
		Text		*cddb_text_mode;
		ComboBox	*cddb_combo_mode;
		Text		*cddb_text_server;
		EditBox		*cddb_edit_server;
		Text		*cddb_text_port;
		EditBox		*cddb_edit_port;
		Text		*cddb_text_email;
		EditBox		*cddb_edit_email;
		Button		*cddb_button_http;
		Button		*cddb_button_proxy;

		TabWidget	*plugins_tabs_plugins;
		Layer		*plugins_layer_input;
		ListBox		*plugins_list_input;
		Button		*plugins_button_input;
		Button		*plugins_button_input_about;
		Layer		*plugins_layer_output;
		ListBox		*plugins_list_output;
		Button		*plugins_button_output;
		Button		*plugins_button_output_about;

		GroupBox	*tags_group_tags;
		CheckBox	*tags_check_enable;
		Text		*tags_text_defcomment;
		EditBox		*tags_edit_defcomment;

		Divider		*divbar;

		Window		*mainWnd;
		Titlebar	*mainWnd_titlebar;

		Button		*btn_cancel;
		Button		*btn_ok;

		Bool		 cdparanoia;
		Bool		 jitter;
		Bool		 swapchannels;
		Bool		 locktray;
		Bool		 ntscsi;
		Bool		 enable_tags;
		Bool		 cddb_auto;
		Bool		 cddb_cache;
 
		bonkEncConfig	*currentConfig;

		Void		 OK();
		Void		 Cancel();
		Void		 SelectDir();
		Void		 SelectLanguage();
		Void		 ConfigureEncoder();
		Void		 SetParanoia();
		Void		 SetCDDBMode();
		Void		 HTTPSettings();
		Void		 ProxySettings();
		Void		 ToggleTags();
		Void		 SelectInputPlugin();
		Void		 SelectOutputPlugin();
		Void		 ConfigureInputPlugin();
		Void		 ConfigureOutputPlugin();
		Void		 AboutInputPlugin();
		Void		 AboutOutputPlugin();
	public:
				 configureGeneralSettings();
				~configureGeneralSettings();
		Int		 ShowDialog();
};

#endif
