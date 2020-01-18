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
#endif

#ifndef _MSC_VER
#   define UINT8 unsigned char

#   define EXTERN_GUID(itf,l1,s1,s2,c1,c2,c3,c4,c5,c6,c7,c8)  \
      EXTERN_C const IID DECLSPEC_SELECTANY itf = {l1,s1,s2,{c1,c2,c3,c4,c5,c6,c7,c8}}
#endif

#define REFPROPVARIANT const PROPVARIANT &

#include "rpcsal.h"
#include "wmsdkidl.h"
