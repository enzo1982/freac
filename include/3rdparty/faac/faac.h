/*
 * FAAC - Freeware Advanced Audio Coder
 * Copyright (C) 2001 Menno Bakker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Id: faac.h,v 1.38 2012/03/01 18:34:17 knik Exp $
 */

#ifndef _FAAC_H_
#define _FAAC_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#if !defined(FAACAPI) && defined(__GNUC__) && (__GNUC__ >= 4)
# if defined(_WIN32)
#  define FAACAPI __stdcall __declspec(dllexport)
# else
#  define FAACAPI __attribute__((visibility("default")))
# endif
#endif
#ifndef FAACAPI
#  define FAACAPI
#endif

#pragma pack(push, 1)

typedef struct {
  void *ptr;
  char *name;
}
psymodellist_t;

#include "faaccfg.h"


typedef void *faacEncHandle;

#ifndef HAVE_INT32_T
typedef signed int int32_t;
#endif

/*
	Allows an application to get FAAC version info. This is intended
	purely for informative purposes.

	Returns FAAC_CFG_VERSION.
*/
int FAACAPI faacEncGetVersion(char **faac_id_string,
			      char **faac_copyright_string);


faacEncConfigurationPtr FAACAPI
  faacEncGetCurrentConfiguration(faacEncHandle hEncoder);


int FAACAPI faacEncSetConfiguration(faacEncHandle hEncoder,
				    faacEncConfigurationPtr config);


faacEncHandle FAACAPI faacEncOpen(unsigned long sampleRate,
				  unsigned int numChannels,
				  unsigned long *inputSamples,
                                  unsigned long *maxOutputBytes
                                 );

int FAACAPI faacEncGetDecoderSpecificInfo(faacEncHandle hEncoder, unsigned char **ppBuffer,
					  unsigned long *pSizeOfDecoderSpecificInfo);


int FAACAPI faacEncEncode(faacEncHandle hEncoder, int32_t * inputBuffer, unsigned int samplesInput,
			 unsigned char *outputBuffer,
			 unsigned int bufferSize);


int FAACAPI faacEncClose(faacEncHandle hEncoder);



#pragma pack(pop)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FAAC_H_ */
