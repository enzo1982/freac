 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/genconfig/genconfig_encoders.h>

#include <dllinterfaces.h>

#include <dialogs/bonkconfig.h>
#include <dialogs/bladeconfig.h>
#include <dialogs/lameconfig.h>
#include <dialogs/vorbisconfig.h>
#include <dialogs/faacconfig.h>
#include <dialogs/tvqconfig.h>

#include <3rdparty/bladedll/bladedll.h>
#include <3rdparty/lame/lame.h>
#include <3rdparty/vorbis/vorbisenc.h>
#include <3rdparty/faac/faac.h>

GeneralSettingsLayerEncoders::GeneralSettingsLayerEncoders() : Layer(bonkEnc::i18n->TranslateString("Encoders"))
{
	Point	 pos;
	Size	 size;

	currentConfig = bonkEnc::currentConfig;

	onTheFly	= currentConfig->enc_onTheFly;
	keepWaves	= currentConfig->enc_keepWaves;
	unicode_files	= currentConfig->useUnicodeNames;

	pos.x	= 7;
	pos.y	= 11;
	size.cx	= 344;
	size.cy	= 43;

	group_encoder	= new GroupBox(bonkEnc::i18n->TranslateString("Encoder"), pos, size);

	pos.x	= 17;
	pos.y	= 23;
	size.cx	= 186;
	size.cy	= 0;

	combo_encoder	= new ComboBox(pos, size);

	if (currentConfig->enable_blade)
	{
		String		 bladeVersion = "";
		BE_VERSION	 beVer;

		ex_beVersion(&beVer);

		bladeVersion.Append("v").Append(String::FromInt(beVer.byMajorVersion)).Append(".").Append(String::FromInt(beVer.byMinorVersion));

		combo_encoder->AddEntry(String("BladeEnc MP3 Encoder ").Append(bladeVersion));
	}

	if (currentConfig->enable_bonk)		combo_encoder->AddEntry(String("Bonk Audio Encoder v").Append(ex_bonk_get_version_string()));

	if (currentConfig->enable_faac)
	{
		String		 faacVersion = "";
		UnsignedInt	 samples;
		UnsignedInt	 buffer_size;
		faacEncHandle	 faac = ex_faacEncOpen(44100, 2, &samples, &buffer_size);

		faacVersion.Append("v").Append(ex_faacEncGetCurrentConfiguration(faac)->name);

		ex_faacEncClose(faac);

		combo_encoder->AddEntry(String("FAAC MP4/AAC Encoder ").Append(faacVersion));
	}

	if (currentConfig->enable_lame)		combo_encoder->AddEntry(String("LAME MP3 Encoder v").Append(ex_get_lame_short_version()));
	if (currentConfig->enable_vorbis)	combo_encoder->AddEntry(String("Ogg Vorbis Encoder v1.1.1"));

	if (currentConfig->enable_tvq)
	{
		char		 tvqVersionID[13];

		ex_TvqGetVersionID(V2, tvqVersionID);

		combo_encoder->AddEntry(String("TwinVQ VQF Encoder v").Append(tvqVersionID + 4));
	}

	combo_encoder->AddEntry(String("WAVE Out Filter v1.0"));

	combo_encoder->SelectNthEntry(currentConfig->encoder);

	pos.y	-= 1;
	pos.x	+= 194;
	size.cx	= 130;

	button_config	= new Button(bonkEnc::i18n->TranslateString("Configure encoder"), NIL, pos, size);
	button_config->onClick.Connect(&GeneralSettingsLayerEncoders::ConfigureEncoder, this);

	pos.x	= 7;
	pos.y	= 66;
	size.cx	= 344;
	size.cy	= 43;

	group_outdir	= new GroupBox(bonkEnc::i18n->TranslateString("Output directory"), pos, size);

	pos.x	= 17;
	pos.y	+= 12;
	size.cx	= 236;
	size.cy	= 0;

	edit_outdir	= new EditBox(currentConfig->enc_outdir, pos, size, 0);

	pos.x	+= 244;
	pos.y	-= 1;
	size.cx	= 0;

	button_outdir_browse= new Button(bonkEnc::i18n->TranslateString("Browse"), NIL, pos, size);
	button_outdir_browse->onClick.Connect(&GeneralSettingsLayerEncoders::SelectDir, this);

	pos.x	= 7;
	pos.y	= 121;
	size.cx	= 344;
	size.cy	= 43;

	group_filename	= new GroupBox(bonkEnc::i18n->TranslateString("Filename pattern"), pos, size);

	pos.x	= 17;
	pos.y	+= 12;
	size.cx	= 324;
	size.cy	= 0;

	edit_filename	= new EditBox(currentConfig->enc_filePattern, pos, size, 0);

	list_filename	= new ListBox(pos, size);
	list_filename->AddEntry("<artist> - <title>");
	list_filename->AddEntry("<artist>\\<artist> - <title>");
	list_filename->AddEntry("<artist> - <album> - <track> - <title>");
	list_filename->AddEntry("<artist> - <album>\\<track> - <title>");
	list_filename->AddEntry("<artist> - <album>\\<artist> - <album> - <track> - <title>");
	list_filename->AddEntry("<track> - <artist> - <title>");
	list_filename->AddEntry("<album>\\<track> - <artist> - <title>");
	list_filename->AddEntry("<genre>\\<artist> - <title>");
	list_filename->AddEntry("<filename>");

	edit_filename->SetDropDownList(list_filename);

	pos.x	= 359;
	pos.y	= 11;
	size.cx	= 178;
	size.cy	= 68;

	group_options	= new GroupBox(bonkEnc::i18n->TranslateString("Options"), pos, size);

	pos.x	+= 10;
	pos.y	+= 14;
	size.cx	= 157;
	size.cy	= 0;

	check_onTheFly	= new CheckBox(bonkEnc::i18n->TranslateString("Encode \'On-The-Fly\'"), pos, size, &onTheFly);
	check_onTheFly->onClick.Connect(&GeneralSettingsLayerEncoders::ToggleOnTheFly, this);

	pos.y += 26;

	check_keepWaves	= new CheckBox(bonkEnc::i18n->TranslateString("Keep ripped wave files"), pos, size, &keepWaves);

	ToggleOnTheFly();

	pos.x	= 359;
	pos.y	= 121;
	size.cx	= 178;
	size.cy	= 43;

	group_unicode	= new GroupBox(bonkEnc::i18n->TranslateString("Unicode"), pos, size);

	pos.x	+= 10;
	pos.y	+= 14;
	size.cx	= 157;
	size.cy	= 0;

	check_unicode_files= new CheckBox(bonkEnc::i18n->TranslateString("Use Unicode filenames"), pos, size, &unicode_files);

	if (!Setup::enableUnicode) check_unicode_files->Deactivate();

	RegisterObject(group_encoder);
	RegisterObject(combo_encoder);
	RegisterObject(button_config);
	RegisterObject(group_outdir);
	RegisterObject(edit_outdir);
	RegisterObject(button_outdir_browse);
	RegisterObject(group_filename);
	RegisterObject(edit_filename);
	RegisterObject(group_options);
	RegisterObject(check_onTheFly);
	RegisterObject(check_keepWaves);
	RegisterObject(group_unicode);
	RegisterObject(check_unicode_files);
}

GeneralSettingsLayerEncoders::~GeneralSettingsLayerEncoders()
{
	DeleteObject(group_encoder);
	DeleteObject(combo_encoder);
	DeleteObject(button_config);
	DeleteObject(group_outdir);
	DeleteObject(edit_outdir);
	DeleteObject(button_outdir_browse);
	DeleteObject(group_filename);
	DeleteObject(edit_filename);
	DeleteObject(list_filename);
	DeleteObject(group_options);
	DeleteObject(check_onTheFly);
	DeleteObject(check_keepWaves);
	DeleteObject(group_unicode);
	DeleteObject(check_unicode_files);
}

Void GeneralSettingsLayerEncoders::SelectDir()
{
	DirSelection	*dialog = new DirSelection();

	dialog->SetParentWindow(GetContainerWindow());
	dialog->SetCaption(String("\n").Append(bonkEnc::i18n->TranslateString("Select the folder in which the encoded files will be placed:")));
	dialog->SetDirName(edit_outdir->GetText());

	if (dialog->ShowDialog() == Success)
	{
		edit_outdir->SetText(dialog->GetDirName());
	}

	DeleteObject(dialog);
}

Void GeneralSettingsLayerEncoders::ConfigureEncoder()
{
	if (combo_encoder->GetSelectedEntryNumber() == ENCODER_WAVE)
	{
		QuickMessage(bonkEnc::i18n->TranslateString("No options can be configured for the WAVE Out filter!"), bonkEnc::i18n->TranslateString("WAVE Out filter"), MB_OK, IDI_INFORMATION);

		return;
	}

	Dialog	*dlg = NIL;

	if (combo_encoder->GetSelectedEntryNumber() == ENCODER_BONKENC)		dlg = new ConfigureBonkEnc();
	else if (combo_encoder->GetSelectedEntryNumber() == ENCODER_BLADEENC)	dlg = new ConfigureBladeEnc();
	else if (combo_encoder->GetSelectedEntryNumber() == ENCODER_LAMEENC)	dlg = new ConfigureLameEnc();
	else if (combo_encoder->GetSelectedEntryNumber() == ENCODER_VORBISENC)	dlg = new ConfigureVorbisEnc();
	else if (combo_encoder->GetSelectedEntryNumber() == ENCODER_FAAC)	dlg = new ConfigureFAAC();
	else if (combo_encoder->GetSelectedEntryNumber() == ENCODER_TVQ)	dlg = new ConfigureTVQ();

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Void GeneralSettingsLayerEncoders::ToggleOnTheFly()
{
	if (onTheFly)	check_keepWaves->Deactivate();
	else		check_keepWaves->Activate();
}

Int GeneralSettingsLayerEncoders::GetSelectedEncoder()
{
	return combo_encoder->GetSelectedEntryNumber();
}

Bool GeneralSettingsLayerEncoders::GetOnTheFly()
{
	return onTheFly;
}

Bool GeneralSettingsLayerEncoders::GetKeepWaveFiles()
{
	return keepWaves;
}

Bool GeneralSettingsLayerEncoders::GetUnicodeFilenames()
{
	return unicode_files;
}

String GeneralSettingsLayerEncoders::GetOutputDirectory()
{
	return edit_outdir->GetText();
}

String GeneralSettingsLayerEncoders::GetFilenamePattern()
{
	return edit_filename->GetText();
}
