//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
//
#pragma once

#include <windows.h>

#ifdef __WIN64__
#   define _COM_interface struct
#   define interface _COM_interface
#endif

#ifndef _MSC_VER
#   define UINT8 unsigned char

#   define EXTERN_GUID(itf,l1,s1,s2,c1,c2,c3,c4,c5,c6,c7,c8)  \
      EXTERN_C const IID DECLSPEC_SELECTANY itf = {l1,s1,s2,{c1,c2,c3,c4,c5,c6,c7,c8}}

#   ifndef __WIN64__
	typedef struct _BYTE_BLOB {
	    unsigned long clSize;
	    byte abData[ 1 ];
	} BYTE_BLOB;
#   endif
#endif

typedef struct {
    GUID fmtid;
    DWORD pid;
} PROPERTYKEY;

#define REFPROPVARIANT const PROPVARIANT &

#include "rpcsal.h"
#include "wmsdkidl.h"
#include "asferr.h"
#include "nserror.h"
