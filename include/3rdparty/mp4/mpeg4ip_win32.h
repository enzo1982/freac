/*
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 * 
 * The Original Code is MPEG4IP.
 * 
 * The Initial Developer of the Original Code is Cisco Systems Inc.
 * Portions created by Cisco Systems Inc. are
 * Copyright (C) Cisco Systems Inc. 2005.  All Rights Reserved.
 * 
 * Contributor(s): 
 *		Bill May wmay@cisco.com
 */
/* windows defines */
#ifndef __MPEG4IP_WIN32_H__
#define __MPEG4IP_WIN32_H__
#define HAVE_IN_PORT_T
#define HAVE_SOCKLEN_T
#define NEED_SDL_VIDEO_IN_MAIN_THREAD
#define _WINSOCKAPI_
#define _INTEGRAL_MAX_BITS 64
#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

typedef unsigned __int64 uint64_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int64 u_int64_t;
typedef unsigned __int32 u_int32_t;
typedef unsigned __int16 u_int16_t;
typedef unsigned __int8 u_int8_t;
typedef __int64 int64_t;
typedef unsigned short in_port_t;
typedef int socklen_t;
typedef unsigned int uint;
#define snprintf _snprintf
#define strncasecmp _strnicmp
#define strcasecmp _stricmp

#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define write _write
#define lseek _lseek
#define close _close
#define open _open
#define access _access
#define vsnprintf _vsnprintf
#define stat _stati64
#define fstat _fstati64
#define F_OK 0
#define OPEN_RDWR (_O_RDWR | _O_BINARY)
#define OPEN_CREAT (_O_CREAT | _O_BINARY)
#define OPEN_RDONLY (_O_RDONLY | _O_BINARY)
#define srandom srand
#define random rand

#define IOSBINARY ios::binary

#ifdef __cplusplus
extern "C" {
#endif
int gettimeofday(struct timeval *t, void *);
#ifdef __cplusplus
}
#endif

#define MAX_UINT64 -1
#define D64F "I64d"
#define U64F  "I64u"
#define X64F "I64x"

#define TO_D64(a) (a##I64)
#define TO_U64(a) (a##UI64)

#define LOG_EMERG 0
#define LOG_ALERT 1
#define LOG_CRIT 2
#define LOG_ERR 3
#define LOG_WARNING 4
#define LOG_NOTICE 5
#define LOG_INFO 6
#define LOG_DEBUG 7

#define VAR_TO_FPOS(fpos, var) (fpos) = (var)
#define FPOS_TO_VAR(fpos, typed, var) (var) = (typed)(fpos)

#define __STRING(expr) #expr

#define FOPEN_READ_BINARY "rb"
#define FOPEN_WRITE_BINARY "wb"

#define UINT64_TO_DOUBLE(a) ((double)((int64_t)(a)))
#ifdef __cplusplus
extern "C" {
#endif
char *strcasestr(const char *haystack, const char *needle);
#ifdef __cplusplus
}
#endif


#define SIZEOF_BOOL 1

#endif
