 /* BonkEnc version 0.7
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  * Portions Copyright (C) 1999-2002 Albert L. Faber
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smoothx.h>
#include <main.h>
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

SMOOTHInt bonkEnc::ReadCDText()
{
	FreeCDText();

	const int	 nBufferSize	= 4 + 8 * sizeof(cdTextPackage) * 256;
	unsigned char	*pbtBuffer	= new unsigned char [nBufferSize];
	int		 nCDTextSize	= 0;
	char		*lpZero		= NIL;

	ex_CR_ReadCDText(pbtBuffer, nBufferSize, &nCDTextSize);

	if (nCDTextSize < 4) return SMOOTH::Error;

	int		 nNumPacks		= (nCDTextSize - 4) / sizeof(cdTextPackage);
	cdTextPackage	*pCDtextPacks		= NIL;
	char		 lpszBuffer[1024]	= {'\0',};
	int		 nInsertPos		= 0;

	for (int i = 0; i < nNumPacks; i++)
	{
		pCDtextPacks = (cdTextPackage *) &pbtBuffer[i * sizeof(cdTextPackage) + 4];

		if (0 == pCDtextPacks->block)
		{
			for (int j = 0; j < 12; j++)
			{
				lpszBuffer[nInsertPos++] = pCDtextPacks->data[j];
			}

			while (nInsertPos > 0 && NIL != (lpZero = (char *) memchr(lpszBuffer, '\0', nInsertPos)))
			{
				int nOut = (lpZero - lpszBuffer) + 1 ;

				if (pCDtextPacks->packType == 0x80) // Album/Track title
				{
					if (pCDtextPacks->trackNumber != 0) cdText.AddEntry(lpszBuffer, pCDtextPacks->trackNumber); //SetTrackName(CString(lpszBuffer), pCDtextPacks->trackNumber - 1);
				}

				if (pCDtextPacks->packType == 0x81) // Artist name
				{
					if (pCDtextPacks->trackNumber == 0) cdText.AddEntry(lpszBuffer, 0); //SetArtist(CString(lpszBuffer));
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

	return SMOOTH::Success;
}

SMOOTHInt bonkEnc::FreeCDText()
{
	cdText.DeleteAll();

	return SMOOTH::Success;
}
