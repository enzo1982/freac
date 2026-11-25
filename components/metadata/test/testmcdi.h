 /* fre:ac - free audio converter
  * Copyright (C) 2026 Shen-Ta Hsieh
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_TEST_MCDI
#define H_FREAC_TEST_MCDI

#include <boca.h>

/* Build MCDI data in the format of a READ TOC response: a two byte length,
 * the first and last track, then eight bytes for each entry and the lead-out.
 * Entries from firstDataEntry on are data tracks (an Enhanced CD); pass the
 * number of entries for a disc without data tracks there. Entries before
 * firstAudioEntry are also data tracks (a mixed-mode disc with a data track
 * at the start); pass 0 (the default) for a disc without one.
 */
static BoCA::MCDI BuildMCDI(const Array<Int> &lbaOffsets, Int leadOutLBA, Int firstDataEntry, Int firstAudioEntry = 0)
{
	Int			 entries = lbaOffsets.Length();
	Int			 length	 = 2 + 8 * (entries + 1);
	Buffer<UnsignedByte>	 data(4 + 8 * (entries + 1));

	data.Zero();

	data[0] = length >> 8;
	data[1] = length & 255;
	data[2] = 1;
	data[3] = entries;

	for (Int i = 0; i <= entries; i++)
	{
		Int	 base = 4 + 8 * i;
		Int	 lba  = (i < entries) ? lbaOffsets.GetNth(i) : leadOutLBA;

		data[base + 1] = (i < entries && (i < firstAudioEntry || i >= firstDataEntry)) ? 0x14 : 0x10;
		data[base + 2] = (i < entries) ? i + 1 : 0xAA;

		data[base + 4] = (lba >> 24) & 255;
		data[base + 5] = (lba >> 16) & 255;
		data[base + 6] = (lba >>  8) & 255;
		data[base + 7] =  lba	     & 255;
	}

	return BoCA::MCDI(data);
}

#endif
