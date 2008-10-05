 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cdinfo/cdtext.h>
#include <bonkenc.h>
#include <dllinterfaces.h>

typedef struct
{
	unsigned char	 packType;
	unsigned char	 trackNumber;
	unsigned char	 sequenceNumber;

	unsigned char	 characterPosition	:4;
	unsigned char	 block			:3;
	unsigned char	 bDBC			:1;

	unsigned char	 data[12];
	unsigned char	 crc0;
	unsigned char	 crc1;
}
cdTextPackage;

BonkEnc::CDText::CDText()
{
}

BonkEnc::CDText::~CDText()
{
}

Int BonkEnc::CDText::ReadCDText()
{
	if (!BoCA::Config::Get()->cdrip_read_cdtext) return Success();

	cdText.RemoveAll();

	const int	 nBufferSize	= 4 + 8 * sizeof(cdTextPackage) * 256;
	unsigned char	*pbtBuffer	= new unsigned char [nBufferSize];
	int		 nCDTextSize	= 0;
	char		*lpZero		= NIL;

	ex_CR_ReadCDText(pbtBuffer, nBufferSize, &nCDTextSize);

	if (nCDTextSize < 4) { delete [] pbtBuffer; return Error(); }

	int		 nNumPacks		= (nCDTextSize - 4) / sizeof(cdTextPackage);
	cdTextPackage	*pCDtextPacks		= NIL;
	char		 lpszBuffer[1024]	= {'\0',};
	int		 nInsertPos		= 0;

	for (Int i = 0; i < nNumPacks; i++)
	{
		pCDtextPacks = (cdTextPackage *) &pbtBuffer[i * sizeof(cdTextPackage) + 4];

		if (pCDtextPacks->block == 0)
		{
			for (Int j = 0; j < 12; j++) lpszBuffer[nInsertPos++] = pCDtextPacks->data[j];

			while (nInsertPos > 0 && (lpZero = (char *) memchr(lpszBuffer, '\0', nInsertPos)) != NIL)
			{
				Int	 nOut = (lpZero - lpszBuffer) + 1;

				if (pCDtextPacks->packType == 0x80) // Album/Track title
				{
					if (pCDtextPacks->trackNumber == 0) cdText.Add(lpszBuffer, 100);
					if (pCDtextPacks->trackNumber != 0) cdText.Add(lpszBuffer, pCDtextPacks->trackNumber);
				}
				else if (pCDtextPacks->packType == 0x81) // Artist name
				{
					if (pCDtextPacks->trackNumber == 0) cdText.Add(lpszBuffer, 0);
				}

				nInsertPos -= nOut;

				memmove(lpszBuffer, lpZero + 1, 1024 - nOut -1);

				pCDtextPacks->trackNumber++;

				while (nInsertPos > 0 && lpszBuffer[ 0 ] == '\0')
				{
					memmove(lpszBuffer, lpszBuffer + 1, 1024 -1);
					nInsertPos--;
				}
			}
		}
	}

	delete [] pbtBuffer;

	return Success();
}

Int BonkEnc::CDText::ClearCDText()
{
	cdText.RemoveAll();

	return Success();
}

const Array<String> &BonkEnc::CDText::GetCDText()
{
	return cdText;
}
