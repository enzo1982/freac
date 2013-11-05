 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <tools/encoding.h>

#include <string.h>

String BonkEnc::Encoding::GuessEncoding(const char *string)
{
	/* Check if it is UTF-8.
	 */
	if (IsUTF8(string)) return "UTF-8";

//	Int	 length = strlen(string);
//	char	 chars[256];

//	InitArray(chars, string);

	/* Check if it is a KOI8 variant.
	 */
/*	if (Sum(chars, 0xC0, 0xDF) + chars[0xA3] + Sum(chars, 0xE0, 0xFF) + chars[0xB3] >= length * 0.3)
	{
		if (chars[0xA4] + chars[0xA6] + chars[0xA7] + chars[0xAD] +
		    chars[0xB4] + chars[0xB6] + chars[0xB7] + chars[0xBD] > 0) return "KOI8-U";
		else							       return "KOI8-R";
	}
*/
	/* Check if it is Windows CP-1251.
	 */
/*	if (Sum(chars, 0xC0, 0xDF) + Sum(chars, 0xE0, 0xFF) >= length * 0.3)
	{
		return "CP1251";
	}
*/
	/* No idea. Treat it like ISO Latin 1.
	 */
	return "ISO-8859-1";
}

Bool BonkEnc::Encoding::IsUTF8(const char *string)
{
	Int	 length = strlen(string);

	for (Int i = 0; i < length; i++)
	{
		if (						 ((unsigned char *) string)[i    ] <= 0x7F) {         continue; }

		if (i < length - 1									   &&
		    ((unsigned char *) string)[i    ] >= 0xC0 && ((unsigned char *) string)[i    ] <= 0xDF &&
		    ((unsigned char *) string)[i + 1] >= 0x80 && ((unsigned char *) string)[i + 1] <= 0xBF) { i += 1; continue; }

		if (i < length - 2									   &&
		    ((unsigned char *) string)[i    ] >= 0xE0 && ((unsigned char *) string)[i    ] <= 0xEF &&
		    ((unsigned char *) string)[i + 1] >= 0x80 && ((unsigned char *) string)[i + 1] <= 0xBF &&
		    ((unsigned char *) string)[i + 2] >= 0x80 && ((unsigned char *) string)[i + 2] <= 0xBF) { i += 2; continue; }

		if (i < length - 3									   &&
		    ((unsigned char *) string)[i    ] >= 0xF0 && ((unsigned char *) string)[i    ] <= 0xF7 &&
		    ((unsigned char *) string)[i + 1] >= 0x80 && ((unsigned char *) string)[i + 1] <= 0xBF &&
		    ((unsigned char *) string)[i + 2] >= 0x80 && ((unsigned char *) string)[i + 2] <= 0xBF &&
		    ((unsigned char *) string)[i + 3] >= 0x80 && ((unsigned char *) string)[i + 3] <= 0xBF) { i += 3; continue; }

		return False;
	}

	return True;
}

Void BonkEnc::Encoding::InitArray(char chars[256], const char *string)
{
	Int	 length = strlen(string);

	for (Int i = 0; i <    256; i++) chars[i] = 0;
	for (Int i = 0; i < length; i++) chars[(unsigned char) string[i]]++;
}

Int BonkEnc::Encoding::Sum(char chars[256], Int min, Int max)
{
	Int	 sum = 0;

	for (Int i = min; i <= max; i++) sum += chars[i];

	return sum;
}
