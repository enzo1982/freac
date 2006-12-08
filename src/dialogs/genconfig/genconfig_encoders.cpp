 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
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
#include <dialogs/flacconfig.h>
#include <dialogs/tvqconfig.h>

#include <3rdparty/bladedll/bladedll.h>
#include <3rdparty/lame/lame.h>
#include <3rdparty/vorbis/vorbisenc.h>
#include <3rdparty/faac/faac.h>

BonkEnc::GeneralSettingsLayerEncoders::GeneralSettingsLayerEncoders() : Layer(BonkEnc::i18n->TranslateString("Encoders"))
{
	Point	 pos;
	Size	 size;

	currentConfig = BonkEnc::currentConfig;

	onTheFly	= currentConfig->enc_onTheFly;
	keepWaves	= currentConfig->enc_keepWaves;
	useInputDir	= currentConfig->writeToInputDir;
	allowOverwrite	= currentConfig->allowOverwrite;
	singleFile	= currentConfig->encodeToSingleFile;
	unicode_files	= currentConfig->useUnicodeNames;

	pos.x	= 7;
	pos.y	= 11;
	size.cx	= 344;
	size.cy	= 43;

	group_encoder	= new GroupBox(BonkEnc::i18n->TranslateString("Encoder"), pos, size);

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

	if (currentConfig->enable_flac)		combo_encoder->AddEntry(String("FLAC Audio Encoder v1.1.3"));
	if (currentConfig->enable_lame)		combo_encoder->AddEntry(String("LAME MP3 Encoder v").Append(ex_get_lame_short_version()));
	if (currentConfig->enable_vorbis)	combo_encoder->AddEntry(String("Ogg Vorbis Encoder v1.1.2"));

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

	button_config	= new Button(BonkEnc::i18n->TranslateString("Configure encoder"), NIL, pos, size);
	button_config->onAction.Connect(&GeneralSettingsLayerEncoders::ConfigureEncoder, this);

	pos.x	= 7;
	pos.y	= 66;
	size.cx	= 344;
	size.cy	= 93;

	group_outdir	= new GroupBox(BonkEnc::i18n->TranslateString("Output directory"), pos, size);

	pos.x	+= 10;
	pos.y	+= 14;
	size.cx	= 236;
	size.cy	= 0;

	check_useInputDir	= new CheckBox(BonkEnc::i18n->TranslateString("Use input file directory if possible"), pos, size, &useInputDir);
	check_useInputDir->onAction.Connect(&GeneralSettingsLayerEncoders::ToggleUseInputDir, this);

	pos.x	+= 17;
	pos.y	+= 23;
	size.cx	= 219;
	size.cy	= 0;

	check_allowOverwrite	= new CheckBox(BonkEnc::i18n->TranslateString("Allow overwriting input file"), pos, size, &allowOverwrite);

	ToggleUseInputDir();

	pos.x	= 17;
	pos.y	+= 25;
	size.cx	= 236;
	size.cy	= 0;

	edit_outdir	= new EditBox(currentConfig->enc_outdir, pos, size, 0);

	pos.x	+= 244;
	pos.y	-= 1;
	size.cx	= 0;

	button_outdir_browse= new Button(BonkEnc::i18n->TranslateString("Browse"), NIL, pos, size);
	button_outdir_browse->onAction.Connect(&GeneralSettingsLayerEncoders::SelectDir, this);

	pos.x	= 7;
	pos.y	= 171;
	size.cx	= 344;
	size.cy	= 43;

	group_filename	= new GroupBox(BonkEnc::i18n->TranslateString("Filename pattern"), pos, size);

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
	size.cy	= 94;

	group_options		= new GroupBox(BonkEnc::i18n->TranslateString("Options"), pos, size);

	pos.x	+= 10;
	pos.y	+= 14;
	size.cx	= 157;
	size.cy	= 0;

	check_onTheFly		= new CheckBox(BonkEnc::i18n->TranslateString("Encode \'On-The-Fly\'"), pos, size, &onTheFly);
	check_onTheFly->onAction.Connect(&GeneralSettingsLayerEncoders::ToggleOnTheFly, this);

	pos.y += 26;

	check_keepWaves		= new CheckBox(BonkEnc::i18n->TranslateString("Keep ripped wave files"), pos, size, &keepWaves);

	ToggleOnTheFly();

	pos.y += 26;

	check_singleFile	= new CheckBox(BonkEnc::i18n->TranslateString("Encode to single file"), pos, size, &singleFile);
	check_singleFile->onAction.Connect(&GeneralSettingsLayerEncoders::ToggleEncodeToSingleFile, this);

	ToggleEncodeToSingleFile();

	pos.x	= 359;
	pos.y	= 171;
	size.cx	= 178;
	size.cy	= 43;

	group_unicode		= new GroupBox(BonkEnc::i18n->TranslateString("Unicode"), pos, size);

	pos.x	+= 10;
	pos.y	+= 14;
	size.cx	= 157;
	size.cy	= 0;

	check_unicode_files	= new CheckBox(BonkEnc::i18n->TranslateString("Use Unicode filenames"), pos, size, &unicode_files);

	if (!Setup::enableUnicode) check_unicode_files->Deactivate();

	RegisterObject(group_encoder);
	RegisterObject(combo_encoder);
	RegisterObject(button_config);
	RegisterObject(group_outdir);
	RegisterObject(check_useInputDir);
	RegisterObject(check_allowOverwrite);
	RegisterObject(edit_outdir);
	RegisterObject(button_outdir_browse);
	RegisterObject(group_filename);
	RegisterObject(edit_filename);
	RegisterObject(group_options);
	RegisterObject(check_onTheFly);
	RegisterObject(check_keepWaves);
	RegisterObject(check_singleFile);
	RegisterObject(group_unicode);
	RegisterObject(check_unicode_files);
}

BonkEnc::GeneralSettingsLayerEncoders::~GeneralSettingsLayerEncoders()
{
	DeleteObject(group_encoder);
	DeleteObject(combo_encoder);
	DeleteObject(button_config);
	DeleteObject(group_outdir);
	DeleteObject(check_useInputDir);
	DeleteObject(check_allowOverwrite);
	DeleteObject(edit_outdir);
	DeleteObject(button_outdir_browse);
	DeleteObject(group_filename);
	DeleteObject(edit_filename);
	DeleteObject(list_filename);
	DeleteObject(group_options);
	DeleteObject(check_onTheFly);
	DeleteObject(check_keepWaves);
	DeleteObject(check_singleFile);
	DeleteObject(group_unicode);
	DeleteObject(check_unicode_files);
}

Void BonkEnc::GeneralSettingsLayerEncoders::SelectDir()
{
	DirSelection	*dialog = new DirSelection();

	dialog->SetParentWindow(GetContainerWindow());
	dialog->SetCaption(String("\n").Append(BonkEnc::i18n->TranslateString("Select the folder in which the encoded files will be placed:")));
	dialog->SetDirName(edit_outdir->GetText());

	if (dialog->ShowDialog() == Success())
	{
		edit_outdir->SetText(dialog->GetDirName());
	}

	DeleteObject(dialog);
}

Void BonkEnc::GeneralSettingsLayerEncoders::ConfigureEncoder()
{
	if (combo_encoder->GetSelectedEntryNumber() == ENCODER_WAVE)
	{
		QuickMessage(BonkEnc::i18n->TranslateString("No options can be configured for the WAVE Out filter!"), BonkEnc::i18n->TranslateString("WAVE Out filter"), MB_OK, IDI_INFORMATION);

		return;
	}

	Dialog	*dlg = NIL;

	if (combo_encoder->GetSelectedEntryNumber() == ENCODER_BONKENC)		dlg = new ConfigureBonkEnc();
	else if (combo_encoder->GetSelectedEntryNumber() == ENCODER_BLADEENC)	dlg = new ConfigureBladeEnc();
	else if (combo_encoder->GetSelectedEntryNumber() == ENCODER_LAMEENC)	dlg = new ConfigureLameEnc();
	else if (combo_encoder->GetSelectedEntryNumber() == ENCODER_VORBISENC)	dlg = new ConfigureVorbisEnc();
	else if (combo_encoder->GetSelectedEntryNumber() == ENCODER_FAAC)	dlg = new ConfigureFAAC();
	else if (combo_encoder->GetSelectedEntryNumber() == ENCODER_FLAC)	dlg = new ConfigureFLAC();
	else if (combo_encoder->GetSelectedEntryNumber() == ENCODER_TVQ)	dlg = new ConfigureTVQ();

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Void BonkEnc::GeneralSettingsLayerEncoders::ToggleOnTheFly()
{
	if (onTheFly)	check_keepWaves->Deactivate();
	else		check_keepWaves->Activate();
}

Void BonkEnc::GeneralSettingsLayerEncoders::ToggleUseInputDir()
{
	if (useInputDir) check_allowOverwrite->Activate();
	else		 check_allowOverwrite->Deactivate();
}

Void BonkEnc::GeneralSettingsLayerEncoders::ToggleEncodeToSingleFile()
{
	if (singleFile)	{ onTheFly = True; check_onTheFly->Deactivate(); }
	else		{ check_onTheFly->Activate(); }

	ToggleOnTheFly();
}

Int BonkEnc::GeneralSettingsLayerEncoders::GetSelectedEncoder()
{
	return combo_encoder->GetSelectedEntryNumber();
}

Bool BonkEnc::GeneralSettingsLayerEncoders::GetUseInputDirectory()
{
	return useInputDir;
}

Bool BonkEnc::GeneralSettingsLayerEncoders::GetAllowOverwrite()
{
	return allowOverwrite;
}

Bool BonkEnc::GeneralSettingsLayerEncoders::GetEncodeToSingleFile()
{
	return singleFile;
}

Bool BonkEnc::GeneralSettingsLayerEncoders::GetOnTheFly()
{
	return onTheFly;
}

Bool BonkEnc::GeneralSettingsLayerEncoders::GetKeepWaveFiles()
{
	return keepWaves;
}

Bool BonkEnc::GeneralSettingsLayerEncoders::GetUnicodeFilenames()
{
	return unicode_files;
}

String BonkEnc::GeneralSettingsLayerEncoders::GetOutputDirectory()
{
	return edit_outdir->GetText();
}

String BonkEnc::GeneralSettingsLayerEncoders::GetFilenamePattern()
{
	return edit_filename->GetText();
}
