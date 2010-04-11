

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0498 */
/* Compiler settings for drmexternals.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#ifdef _MSC_VER
#	pragma warning( disable: 4049 )  /* more than 64k source lines */
#endif

/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

/* verify that the <rpcsal.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCSAL_H_VERSION__
#define __REQUIRED_RPCSAL_H_VERSION__ 100
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __drmexternals_h__
#define __drmexternals_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

/* header files for imported files */
#include "wmdrmsdk.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_drmexternals_0000_0000 */
/* [local] */ 

/*
static const WCHAR *g_wszWMDRM_IsDRM                             = L"IsDRM";
static const WCHAR *g_wszWMDRM_IsDRMCached                       = L"IsDRMCached";
static const WCHAR *g_wszWMDRM_BaseLicenseAcqURL                 = L"BaseLAURL";
static const WCHAR *g_wszWMDRM_Rights                            = L"Rights";
static const WCHAR *g_wszWMDRM_LicenseID                         = L"LID";
static const WCHAR *g_wszWMDRM_DRMHeader                         = L"DRMHeader.";
static const WCHAR *g_wszWMDRM_DRMHeader_KeyID                   = L"DRMHeader.KID";
static const WCHAR *g_wszWMDRM_DRMHeader_LicenseAcqURL           = L"DRMHeader.LAINFO";
static const WCHAR *g_wszWMDRM_DRMHeader_ContentID               = L"DRMHeader.CID";
static const WCHAR *g_wszWMDRM_DRMHeader_IndividualizedVersion   = L"DRMHeader.SECURITYVERSION";
static const WCHAR *g_wszWMDRM_DRMHeader_ContentDistributor      = L"DRMHeader.ContentDistributor";
static const WCHAR *g_wszWMDRM_DRMHeader_SubscriptionContentID   = L"DRMHeader.SubscriptionContentID";
*/

#ifndef DRM_PLAYLIST_TYPES_DEFINED
#define DRM_PLAYLIST_TYPES_DEFINED 1
#define DRM_PLAYLIST_DATA_V2_VALID         ((DWORD) 0x00000001)
#define DRM_PLAYLIST_DATA_V1_VALID         ((DWORD) 0x00000002)
#define DRM_PLAYLIST_DATA_OTHER_VALID      ((DWORD) 0x00000004)
#define DRM_CANCEL_PLAYLIST_BURN          ((DWORD) 0x00000001)
#define DRM_COMMIT_PLAYLIST_BURN          ((DWORD) 0x00000002)
typedef struct __tagDRM_PLAYLIST_CONTENT_ID
    {
    LPCWSTR lpcwszV2Header;
    LPCSTR lpcszV1KID;
    BYTE *pbOtherData;
    DWORD cbOtherData;
    DWORD dwUniqueIDForSession;
    DWORD dwValidFields;
    } 	DRM_PLAYLIST_CONTENT_ID;

#endif // DRM_PLAYLIST_TYPES_DEFINED
typedef 
enum WMT_RIGHTS
    {	WMT_RIGHT_PLAYBACK	= 0x1,
	WMT_RIGHT_COPY_TO_NON_SDMI_DEVICE	= 0x2,
	WMT_RIGHT_COPY_TO_CD	= 0x8,
	WMT_RIGHT_COPY_TO_SDMI_DEVICE	= 0x10,
	WMT_RIGHT_ONE_TIME	= 0x20,
	WMT_RIGHT_SAVE_STREAM_PROTECTED	= 0x40,
	WMT_RIGHT_COPY	= 0x80,
	WMT_RIGHT_COLLABORATIVE_PLAY	= 0x100,
	WMT_RIGHT_SDMI_TRIGGER	= 0x10000,
	WMT_RIGHT_SDMI_NOMORECOPIES	= 0x20000
    } 	WMT_RIGHTS;

#define DRM_DEVICE_REGISTER_TYPE_STREAMING      ((DWORD) 0x00000001)
#define DRM_DEVICE_REGISTER_TYPE_STORAGE        ((DWORD) 0x00000002)
#ifndef DRM_PROTECTION_SCHEMES_DEFINED
#define DRM_PROTECTION_SCHEMES_DEFINED 1
EXTERN_GUID( WMDRM_PROTECTION_SCHEME_ACP,                      0xc3fd11c6, 0xf8b7, 0x4d20, 0xB0, 0x08, 0x1d, 0xb1, 0x7d, 0x61, 0xf2, 0xda );
EXTERN_GUID( WMDRM_PROTECTION_SCHEME_MAX_VGA_RESOLUTION,       0xd783a191, 0xe083, 0x4baf, 0xb2, 0xda, 0xe6, 0x9f, 0x91, 0x0b, 0x37, 0x72 );
EXTERN_GUID( WMDRM_PROTECTION_SCHEME_MAX_COMPONENT_RESOLUTION, 0x811c5110, 0x46c8, 0x4c6e, 0x81, 0x63, 0xc0, 0x48, 0x2a, 0x15, 0xd4, 0x7e );
EXTERN_GUID( WMDRM_PROTECTION_SCHEME_DOWNSAMPLE_REQUIRED,      0x8f81a453, 0x998c, 0x4596, 0xb1, 0x50, 0xf8, 0x48, 0x6d, 0xf6, 0xf2, 0xe6 );
EXTERN_GUID( WMDRM_PROTECTION_SCHEME_MAX_VGA_RESOLUTION_1,         0xd9619433, 0xec9f, 0x489e, 0x9e, 0x07, 0x77, 0x0d, 0xed, 0xd8, 0xbd, 0x04 );
EXTERN_GUID( WMDRM_PROTECTION_SCHEME_MAX_VGA_RESOLUTION_2,         0xa24f0a24, 0x5c92, 0x4737, 0x9b, 0x21, 0xa2, 0x3e, 0x4f, 0xed, 0x32, 0x2e );
EXTERN_GUID( WMDRM_PROTECTION_SCHEME_MAX_VGA_RESOLUTION_3,         0xc4db4310, 0xa1ae, 0x422e, 0xb7, 0xa7, 0x21, 0xbc, 0x34, 0x73, 0xda, 0x77 );
EXTERN_GUID( WMDRM_PROTECTION_SCHEME_MAX_VGA_RESOLUTION_4,         0xe84e4d21, 0x3b1c, 0x44bc, 0xa2, 0xec, 0xc4, 0x81, 0x08, 0x58, 0x7c, 0xad );
EXTERN_GUID( WMDRM_PROTECTION_SCHEME_MAX_COMPONENT_RESOLUTION_1,         0xec3a1c03, 0xdd4e, 0x47f7, 0xbf, 0xe5, 0xae, 0xe7, 0x53, 0x48, 0x66, 0x71 );
EXTERN_GUID( WMDRM_PROTECTION_SCHEME_MAX_COMPONENT_RESOLUTION_2,         0x327803ca, 0xd914, 0x4da3, 0x97, 0x28, 0xe1, 0x9e, 0x82, 0xbe, 0x8c, 0x25 );
EXTERN_GUID( WMDRM_PROTECTION_SCHEME_MAX_COMPONENT_RESOLUTION_3,         0x36d9c83f, 0xaf07, 0x4727, 0xab, 0x74, 0xca, 0x69, 0x31, 0xba, 0x69, 0xf3 );
EXTERN_GUID( WMDRM_PROTECTION_SCHEME_MAX_COMPONENT_RESOLUTION_4,         0x9443d876, 0x1d46, 0x40cb, 0xbd, 0xe5, 0x9c, 0xc1, 0x46, 0x4c, 0x9b, 0xaf );
#endif // DRM_PROTECTION_SCHEMES_DEFINED
#ifndef DRM_WM_GET_LIC_DATA_DEFINED
#define DRM_WM_GET_LIC_DATA_DEFINED 1
typedef struct _WMGetLicenseData
    {
    DWORD dwSize;
    HRESULT hr;
    BSTR wszURL;
    WCHAR *wszLocalFilename;
    BYTE *pbPostData;
    DWORD dwPostDataSize;
    } 	WM_GET_LICENSE_DATA;

#endif // DRM_WM_GET_LIC_DATA_DEFINED
#define WMDRM_KEYTYPE_RC4       0
#define WMDRM_KEYTYPE_COCKTAIL  1
#ifndef WMDRM_IMPORT_SESSION_KEY_DEFINED
#define WMDRM_IMPORT_SESSION_KEY_DEFINED 1
typedef struct __tagWMDRM_IMPORT_SESSION_KEY
    {
    DWORD dwKeyType;
    DWORD cbKey;
    BYTE rgbKey[ 1 ];
    } 	WMDRM_IMPORT_SESSION_KEY;

#endif // WMDRM_IMPORT_SESSION_KEY_DEFINED
#ifndef WMDRM_IMPORT_CONTENT_KEY_DEFINED
#define WMDRM_IMPORT_CONTENT_KEY_DEFINED 1
typedef struct __tagWMDRM_IMPORT_CONTENT_KEY
    {
    DWORD dwVersion;
    DWORD cbStructSize;
    DWORD dwIVKeyType;
    DWORD cbIVKey;
    DWORD dwContentKeyType;
    DWORD cbContentKey;
    BYTE rgbKeyData[ 1 ];
    } 	WMDRM_IMPORT_CONTENT_KEY;

#endif // WMDRM_IMPORT_CONTENT_KEY_DEFINED
#ifndef WMDRM_IMPORT_INIT_STRUCT_DEFINED
#define WMDRM_IMPORT_INIT_STRUCT_DEFINED 1
typedef struct WMDRM_IMPORT_INIT_STRUCT
    {
    DWORD dwVersion;
    DWORD cbEncryptedSessionKeyMessage;
    BYTE *pbEncryptedSessionKeyMessage;
    DWORD cbEncryptedKeyMessage;
    BYTE *pbEncryptedKeyMessage;
    } 	WMDRM_IMPORT_INIT_STRUCT;

#endif // WMDRM_IMPORT_INIT_STRUCT_DEFINED


extern RPC_IF_HANDLE __MIDL_itf_drmexternals_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_drmexternals_0000_0000_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


