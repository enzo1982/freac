 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <gui/layer_tooltip.h>

using namespace BoCA;

BonkEnc::LayerTooltip::LayerTooltip(const Track &track) : Layer()
{
	cover = NIL;

	if (track.pictures.Length() > 0)
	{
		Bitmap	 bitmap = track.pictures.GetFirst().GetBitmap();

		/* Scale bitmap to max 40x40 pixels.
		 */
		Size	 bmpSize = bitmap.GetSize();

		if	(float(bmpSize.cx) / 40 >= float(bmpSize.cy) / 40 && bmpSize.cx > 40) bitmap.Scale(bmpSize * (float(40) / bmpSize.cx));
		else if (float(bmpSize.cy) / 40 >= float(bmpSize.cx) / 40 && bmpSize.cy > 40) bitmap.Scale(bmpSize * (float(40) / bmpSize.cy));

		/* Create and add image widget.
		 */
		cover = new Image(bitmap, Point(4, 4), Size(40, 40));

		Add(cover);
	}

	text = new Text(GetTooltipText(track), Point(3 + (track.pictures.Length() > 0 ? 44 : 0), 3));

	Add(text);

	SetSize(Size(text->GetUnscaledTextWidth() + 7 + (track.pictures.Length() > 0 ? 44 : 0), text->GetUnscaledTextHeight() + 7));
}

BonkEnc::LayerTooltip::~LayerTooltip()
{
	if (cover != NIL) DeleteObject(cover);

	DeleteObject(text);
}

const String &BonkEnc::LayerTooltip::GetTooltipText(const Track &track)
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Joblist");

	const Format	&format = track.GetFormat();
	const Info	&info = track.GetInfo();

	static String	 tooltip;

	tooltip = String(i18n->TranslateString("File")).Append(": ").Append(track.origFilename).Append("\n").
		  Append(i18n->TranslateString("Size")).Append(": ").Append(track.GetFileSizeString()).Append(" ").Append(i18n->TranslateString("bytes")).Append("\n").
		  Append(i18n->TranslateString("Artist")).Append(": ").Append(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist")).Append("\n").
		  Append(i18n->TranslateString("Title")).Append(": ").Append(info.title.Length() > 0 ? info.title : i18n->TranslateString("unknown title")).Append("\n").
		  Append(track.length > 0 || track.approxLength > 0 ? i18n->TranslateString("Length").Append(": ").Append(track.GetLengthString()).Append(" ").Append(i18n->TranslateString("min")).Append("\n") : String()).
		  Append(track.length > 0 ? i18n->TranslateString("Number of samples").Append(": ").Append(S::I18n::Number::GetLocalizedNumberString(track.length)).Append("\n") : String()).
		  Append(i18n->TranslateString("Sampling rate")).Append(": ").Append(S::I18n::Number::GetLocalizedNumberString(format.rate)).Append(" Hz\n").
		  Append(i18n->TranslateString("Sample resolution")).Append(": ").Append(String::FromInt(format.bits)).Append(" ").Append(i18n->TranslateString("bit")).Append("\n").
		  Append(i18n->TranslateString("Channels")).Append(": ").Append(format.channels > 2 ? (format.channels != 4 && format.channels != 5 && format.channels <= 8 ? String::FromInt(format.channels - 1).Append(".1") : String::FromInt(format.channels)) : (format.channels == 1 ? i18n->TranslateString("Mono") : i18n->TranslateString("Stereo")));

	if (format.rate > 0)
	{
		if	(track.length	    > 0) tooltip.Append("\n").Append(i18n->TranslateString("Bitrate").Append(": ").Append(String::FromInt((Int) Math::Round(((Float) track.fileSize) / (track.length / format.rate) * 8.0 / 1000.0))).Append(" kbps"));
		else if (track.approxLength > 0) tooltip.Append("\n").Append(i18n->TranslateString("Bitrate").Append(": ~ ").Append(String::FromInt((Int) Math::Round(((Float) track.fileSize) / (track.approxLength / format.rate) * 8.0 / 1000.0))).Append(" kbps"));

		if (Setup::enableUnicode)
		{
			static wchar_t	 sign[2] = { 0x2248, 0 };

			tooltip.Replace("~", sign);
		}
	}

	return tooltip;
}
