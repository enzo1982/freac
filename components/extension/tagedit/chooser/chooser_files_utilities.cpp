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

#include "chooser_files_utilities.h"

BoCA::AS::DecoderComponent *BoCA::ChooserFilesUtilities::CreateDecoderComponent(const String &iFile)
{
	String			 file = iFile.ToLower();
	DecoderComponent	*component = NIL;
	Registry		&boca = Registry::Get();

	/* First check those decoders that claim
	 * to support the file extension.
	 */
	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_DECODER) continue;

		const Array<FileFormat *>	&formats = boca.GetComponentFormats(i);

		foreach (FileFormat *format, formats)
		{
			const Array<String>	&extensions = format->GetExtensions();

			foreach (const String &extension, extensions)
			{
				if (!file.EndsWith(String(".").Append(extension.ToLower()))) continue;

				component = (DecoderComponent *) Registry::Get().CreateComponentByID(boca.GetComponentID(i));

				if (component != NIL)
				{
					if (component->CanOpenStream(file)) return component;

					boca.DeleteComponent(component);
				}
			}
		}
	}

	/* No suitable decoder found; try all decoders now.
	 */
	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_DECODER) continue;

		component = (DecoderComponent *) Registry::Get().CreateComponentByID(boca.GetComponentID(i));

		if (component != NIL)
		{
			if (component->CanOpenStream(file)) return component;

			boca.DeleteComponent(component);
		}
	}

	return NIL;
}
