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
 * Copyright (C) Cisco Systems Inc. 2000, 2001.  All Rights Reserved.
 * 
 * Contributor(s): 
 *		Dave Mackie		dmackie@cisco.com
 */

#ifndef __MPEG4IP_INCLUDED__
#define __MPEG4IP_INCLUDED__

/* project wide applicable stuff here */


#ifdef WIN32
#define HAVE_IN_PORT_T
#define HAVE_SOCKLEN_T
#define NEED_SDL_VIDEO_IN_MAIN_THREAD
#else
#undef PACKAGE
#undef VERSION
#include <mpeg4ip_config.h>
#undef PACKAGE
#undef VERSION
// so these don't propogate
#endif

// the mpeg4ip_package and mpeg4ip_version are always in this
// file 
#include "mpeg4ip_version.h"

#ifdef WIN32

#define _WINSOCKAPI_
#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#ifndef __GNUC__
typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int64 int64_t;

typedef unsigned char	 uint8_t;
typedef unsigned short	 uint16_t;
typedef unsigned long	 uint32_t;
typedef unsigned __int64 uint64_t;
#endif

typedef unsigned __int64 u_int64_t;
typedef unsigned long	 u_int32_t;
typedef unsigned short	 u_int16_t;
typedef unsigned char	 u_int8_t;

typedef unsigned short in_port_t;
typedef int socklen_t;

#ifndef __GNUC__
typedef int ssize_t;
#endif

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

#ifndef __GNUC__
#define PATH_MAX MAX_PATH
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

#ifdef HAVE_FPOS_T___POS
#define FPOS_TO_VAR(fpos, typed, var) (var) = (typed)((fpos).__pos)
#define VAR_TO_FPOS(fpos, var) (fpos).__pos = (var)
#else
#define FPOS_TO_VAR(fpos, typed, var) (var) = (typed)(fpos)
#define VAR_TO_FPOS(fpos, var) (fpos) = (var)
#endif

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

#else /* UNIX */
/*****************************************************************************
 *   UNIX LIKE DEFINES BELOW THIS POINT
 *****************************************************************************/
#ifdef sun
#include <sys/feature_tests.h>
#endif

#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#else
#ifndef sun
#if _FILE_OFFSET_BITS < 64
 #error File offset bits is already set to non-64 value
#endif
#endif
#endif

#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE
#endif
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#if !defined(HAVE_INTTYPES_H) && !defined(HAVE_STDINT_H)
#error "Don't have stdint.h or inttypes.h - no way to get uint8_t"
#endif

#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <string.h>
#include <ctype.h>
#include <netdb.h>
#include <sys/stat.h>
#ifdef TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif
#include <sys/param.h>

#ifdef __cplusplus
extern "C" {
#endif
char *strcasestr(const char *haystack, const char *needle);
#ifdef __cplusplus
}
#endif

#define OPEN_RDWR O_RDWR
#define OPEN_CREAT O_CREAT 
#define OPEN_RDONLY O_RDONLY

#define closesocket close
#define IOSBINARY ios::bin

#if SIZEOF_LONG == 8
#define MAX_UINT64 -1LU
#define D64F "ld"
#define U64F  "lu"
#define X64F "lx"

#define TO_D64(a) (a##L)
#define TO_U64(a) (a##LU)
#else
#define MAX_UINT64 -1LLU
#define D64F "lld"
#define U64F  "llu"
#define X64F "llx"

#define TO_D64(a) (a##LL)
#define TO_U64(a) (a##LLU)
#endif

#ifdef HAVE_FPOS_T___POS
#define FPOS_TO_VAR(fpos, typed, var) (var) = (typed)((fpos).__pos)
#define VAR_TO_FPOS(fpos, var) (fpos).__pos = (var)
#else
#define FPOS_TO_VAR(fpos, typed, var) (var) = (typed)(fpos)
#define VAR_TO_FPOS(fpos, var) (fpos) = (var)
#endif

#define FOPEN_READ_BINARY "r"
#define FOPEN_WRITE_BINARY "w"
#define UINT64_TO_DOUBLE(a) ((double)(a))
#endif /* define unix */

/*****************************************************************************
 *             Generic type includes used in the whole package               *
 *****************************************************************************/
#define D64  "%"D64F
#define U64  "%"U64F
#define X64 "%"X64F

#define M_LLU TO_U64(1000)
#define M_64 TO_U64(1000)
#define LLU  U64

#include <stdarg.h>
typedef void (*error_msg_func_t)(int loglevel,
				 const char *lib,
				 const char *fmt,
				 va_list ap);
typedef void (*lib_message_func_t)(int loglevel,
				   const char *lib,
				   const char *fmt,
				   ...);
#ifndef HAVE_IN_PORT_T
typedef uint16_t in_port_t;
#endif

#ifndef HAVE_SOCKLEN_T
typedef unsigned int socklen_t;
#endif

#ifdef sun
#include <limits.h>
#define u_int8_t uint8_t
#define u_int16_t uint16_t
#define u_int32_t uint32_t
#define u_int64_t uint64_t
#define __STRING(expr) #expr
#endif

#ifndef HAVE_STRSEP
#ifdef __cplusplus
extern "C" {
#endif
char *strsep(char **strp, const char *delim); 
#ifdef __cplusplus
}
#endif
#endif

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef INADDR_NONE
#define INADDR_NONE (-1)
#endif

#define MALLOC_STRUCTURE(a) ((a *)malloc(sizeof(a)))

#define CHECK_AND_FREE(a) if ((a) != NULL) { free((void *)(a)); (a) = NULL;}

#define NUM_ELEMENTS_IN_ARRAY(name) ((sizeof((name))) / (sizeof(*(name))))

#define ADV_SPACE(a) {while (isspace(*(a)) && (*(a) != '\0'))(a)++;}

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef __cplusplus

#ifndef bool
 #if SIZEOF_BOOL == 8
  typedef uint64_t bool;
 #else
   #if SIZEOF_BOOL == 4
    typedef uint32_t bool;
   #else
     #if SIZEOF_BOOL == 2
      typedef uint16_t bool;
     #else
      typedef unsigned char bool;
     #endif
   #endif
 #endif
 #ifndef false
 #define false FALSE
 #endif
 #ifndef true
 #define true TRUE
 #endif
#endif

#endif

#ifndef ROUND
# ifdef HAVE_RINT
# define ROUND(f) rint(f)
# else
# define ROUND(f) (int)(floor((f) + 0.5))
# endif
#endif

#ifndef INT16_MAX
# define INT16_MAX (32767)
#endif
#ifndef INT16_MIN 
# define INT16_MIN (-32767-1)
#endif 

#ifndef UINT32_MAX
# define UINT32_MAX             (4294967295U)
#endif

#ifndef UINT64_MAX
# define UINT64_MAX TO_U64(0xffffffffffffffff)
#endif

typedef enum audio_format_t {
  AUDIO_FMT_U8 = 0,
  AUDIO_FMT_S8,
  AUDIO_FMT_U16LSB,
  AUDIO_FMT_S16LSB,
  AUDIO_FMT_U16MSB,
  AUDIO_FMT_S16MSB,
  AUDIO_FMT_U16,
  AUDIO_FMT_S16,
  AUDIO_FMT_FLOAT,
  AUDIO_FMT_HW_AC3,
} audio_format_t;

#endif /* __MPEG4IP_INCLUDED__ */

