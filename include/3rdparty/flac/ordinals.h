/* libFLAC - Free Lossless Audio Codec library
 * Copyright (C) 2000,2001,2002,2003  Josh Coalson
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA  02111-1307, USA.
 */

#ifndef FLAC__ORDINALS_H
#define FLAC__ORDINALS_H

#ifdef true
#undef true
#endif
#ifdef false
#undef false
#endif
#ifndef __cplusplus
#define true 1
#define false 0
#endif

typedef int FLAC__bool;

typedef signed char FLAC__int8;
typedef unsigned char FLAC__uint8;

#if defined _MSC_VER
typedef __int16 FLAC__int16;
typedef __int32 FLAC__int32;
typedef __int64 FLAC__int64;
typedef unsigned __int16 FLAC__uint16;
typedef unsigned __int32 FLAC__uint32;
typedef unsigned __int64 FLAC__uint64;
#else
typedef short FLAC__int16;
typedef int FLAC__int32;
typedef long long FLAC__int64;
typedef unsigned short FLAC__uint16;
typedef unsigned int FLAC__uint32;
typedef unsigned long long FLAC__uint64;
#endif

typedef FLAC__uint8 FLAC__byte;
typedef float FLAC__real;

#endif
