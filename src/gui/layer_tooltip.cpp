 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <gui/layer_tooltip.h>

BonkEnc::LayerTooltip::LayerTooltip(const Track &track) : Layer()
{
	cover = NIL;

	if (track.pictures.Length() > 0)
	{
		cover = new Image(track.pictures.GetFirst().GetBitmap(), Point(4, 4), Size(40, 40));

		Add(cover);
	}

	text = new Text(GetTooltipText(track), Point(3 + (track.pictures.Length() > 0 ? 44 : 0), 3));

	Add(text);

	SetSize(Size(text->textSize.cx + 7 + (track.pictures.Length() > 0 ? 44 : 0), text->textSize.cy + 7));
}

BonkEnc::LayerTooltip::~LayerTooltip()
{
	if (cover != NIL) DeleteObject(cover);

	DeleteObject(text);
}

const String &BonkEnc::LayerTooltip::GetTooltipText(const Track &track)
{
	const Format	&format = track.GetFormat();

	static String	 tooltip;

	tooltip = String(BonkEnc::i18n->TranslateString("File")).Append(": ").Append(track.origFilename).Append("\n").
		  Append(BonkEnc::i18n->TranslateString("Size")).Append(": ").Append(track.fileSizeString).Append(" ").Append(BonkEnc::i18n->TranslateString("bytes")).Append("\n").
		  Append(BonkEnc::i18n->TranslateString("Artist")).Append(": ").Append(track.artist.Length() > 0 ? track.artist : BonkEnc::i18n->TranslateString("unknown artist")).Append("\n").
		  Append(BonkEnc::i18n->TranslateString("Title")).Append(": ").Append(track.title.Length() > 0 ? track.title : BonkEnc::i18n->TranslateString("unknown title")).Append("\n").
		  Append(track.length > 0 || track.approxLength > 0 ? String(BonkEnc::i18n->TranslateString("Length")).Append(": ").Append(track.lengthString).Append(" ").Append(BonkEnc::i18n->TranslateString("min")).Append("\n") : String(NIL)).
		  Append(track.length > 0 ? String(BonkEnc::i18n->TranslateString("Number of samples")).Append(": ").Append(S::I18n::Number::GetLocalizedNumberString(track.length)).Append("\n") : String(NIL)).
		  Append(BonkEnc::i18n->TranslateString("Sampling rate")).Append(": ").Append(S::I18n::Number::GetLocalizedNumberString(format.rate)).Append(" Hz\n").
		  Append(BonkEnc::i18n->TranslateString("Sample resolution")).Append(": ").Append(String::FromInt(format.bits)).Append(" ").Append(BonkEnc::i18n->TranslateString("bit")).Append("\n").
		  Append(BonkEnc::i18n->TranslateString("Channels")).Append(": ").Append((format.channels > 2 || format.channels < 1) ? String::FromInt(format.channels) : (format.channels == 1 ? BonkEnc::i18n->TranslateString("Mono") : BonkEnc::i18n->TranslateString("Stereo")));

	if (format.rate > 0 && format.channels > 0)
	{
		if	(track.length	    > 0) tooltip.Append(String(BonkEnc::i18n->TranslateString("\nBitrate")).Append(": ").Append(String::FromInt((Int) Math::Round(((Float) track.fileSize) / (track.length / (format.rate * format.channels)) * 8.0 / 1000.0))).Append(" kbps"));
		else if (track.approxLength > 0) tooltip.Append(String(BonkEnc::i18n->TranslateString("\nBitrate")).Append(": ~ ").Append(String::FromInt((Int) Math::Round(((Float) track.fileSize) / (track.approxLength / (format.rate * format.channels)) * 8.0 / 1000.0))).Append(" kbps"));

		wchar_t	 sign[2] = { 0x2248, 0 };

		if (Setup::enableUnicode) tooltip.Replace("~", sign);
	}

	return tooltip;
}
