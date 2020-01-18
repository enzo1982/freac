

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0498 */
/* Compiler settings for wmdrmsdk.idl:
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
#define __REQUIRED_RPCNDR_H_VERSION__ 500
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

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __wmdrmsdk_h__
#define __wmdrmsdk_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDRMStatusCallback_FWD_DEFINED__
#define __IDRMStatusCallback_FWD_DEFINED__
typedef interface IDRMStatusCallback IDRMStatusCallback;
#endif 	/* __IDRMStatusCallback_FWD_DEFINED__ */


#ifndef __IWMDRMEventGenerator_FWD_DEFINED__
#define __IWMDRMEventGenerator_FWD_DEFINED__
typedef interface IWMDRMEventGenerator IWMDRMEventGenerator;
#endif 	/* __IWMDRMEventGenerator_FWD_DEFINED__ */


#ifndef __IWMDRMProvider_FWD_DEFINED__
#define __IWMDRMProvider_FWD_DEFINED__
typedef interface IWMDRMProvider IWMDRMProvider;
#endif 	/* __IWMDRMProvider_FWD_DEFINED__ */


#ifndef __IWMDRMSecurity_FWD_DEFINED__
#define __IWMDRMSecurity_FWD_DEFINED__
typedef interface IWMDRMSecurity IWMDRMSecurity;
#endif 	/* __IWMDRMSecurity_FWD_DEFINED__ */


#ifndef __IWMDRMEncrypt_FWD_DEFINED__
#define __IWMDRMEncrypt_FWD_DEFINED__
typedef interface IWMDRMEncrypt IWMDRMEncrypt;
#endif 	/* __IWMDRMEncrypt_FWD_DEFINED__ */


#ifndef __IWMDRMEncryptScatter_FWD_DEFINED__
#define __IWMDRMEncryptScatter_FWD_DEFINED__
typedef interface IWMDRMEncryptScatter IWMDRMEncryptScatter;
#endif 	/* __IWMDRMEncryptScatter_FWD_DEFINED__ */


#ifndef __IWMDRMDecrypt_FWD_DEFINED__
#define __IWMDRMDecrypt_FWD_DEFINED__
typedef interface IWMDRMDecrypt IWMDRMDecrypt;
#endif 	/* __IWMDRMDecrypt_FWD_DEFINED__ */


#ifndef __IWMDRMLicense_FWD_DEFINED__
#define __IWMDRMLicense_FWD_DEFINED__
typedef interface IWMDRMLicense IWMDRMLicense;
#endif 	/* __IWMDRMLicense_FWD_DEFINED__ */


#ifndef __IWMDRMLicenseQuery_FWD_DEFINED__
#define __IWMDRMLicenseQuery_FWD_DEFINED__
typedef interface IWMDRMLicenseQuery IWMDRMLicenseQuery;
#endif 	/* __IWMDRMLicenseQuery_FWD_DEFINED__ */


#ifndef __IWMDRMLicenseManagement_FWD_DEFINED__
#define __IWMDRMLicenseManagement_FWD_DEFINED__
typedef interface IWMDRMLicenseManagement IWMDRMLicenseManagement;
#endif 	/* __IWMDRMLicenseManagement_FWD_DEFINED__ */


#ifndef __IWMDRMNetReceiver_FWD_DEFINED__
#define __IWMDRMNetReceiver_FWD_DEFINED__
typedef interface IWMDRMNetReceiver IWMDRMNetReceiver;
#endif 	/* __IWMDRMNetReceiver_FWD_DEFINED__ */


#ifndef __IWMDRMNetTransmitter_FWD_DEFINED__
#define __IWMDRMNetTransmitter_FWD_DEFINED__
typedef interface IWMDRMNetTransmitter IWMDRMNetTransmitter;
#endif 	/* __IWMDRMNetTransmitter_FWD_DEFINED__ */


#ifndef __IWMDRMIndividualizationStatus_FWD_DEFINED__
#define __IWMDRMIndividualizationStatus_FWD_DEFINED__
typedef interface IWMDRMIndividualizationStatus IWMDRMIndividualizationStatus;
#endif 	/* __IWMDRMIndividualizationStatus_FWD_DEFINED__ */


#ifndef __IWMDRMLicenseBackupRestoreStatus_FWD_DEFINED__
#define __IWMDRMLicenseBackupRestoreStatus_FWD_DEFINED__
typedef interface IWMDRMLicenseBackupRestoreStatus IWMDRMLicenseBackupRestoreStatus;
#endif 	/* __IWMDRMLicenseBackupRestoreStatus_FWD_DEFINED__ */


#ifndef __IWMDRMNonSilentLicenseAquisition_FWD_DEFINED__
#define __IWMDRMNonSilentLicenseAquisition_FWD_DEFINED__
typedef interface IWMDRMNonSilentLicenseAquisition IWMDRMNonSilentLicenseAquisition;
#endif 	/* __IWMDRMNonSilentLicenseAquisition_FWD_DEFINED__ */


#ifndef __WMDRMContentEnablerActivate_FWD_DEFINED__
#define __WMDRMContentEnablerActivate_FWD_DEFINED__

#ifdef __cplusplus
typedef class WMDRMContentEnablerActivate WMDRMContentEnablerActivate;
#else
typedef struct WMDRMContentEnablerActivate WMDRMContentEnablerActivate;
#endif /* __cplusplus */

#endif 	/* __WMDRMContentEnablerActivate_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "mfobjects.h"
#include "mfidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_wmdrmsdk_0000_0000 */
/* [local] */ 

/*
static const WCHAR *g_wszWMDRM_ACTIONLIST_TAG                   = L"ACTIONLIST";
static const WCHAR *g_wszWMDRM_ACTION_TAG                       = L"ACTION";
static const WCHAR *g_wszWMDRM_RIGHT_PLAYBACK                   = L"Play";
static const WCHAR *g_wszWMDRM_RIGHT_COPY                       = L"Copy";
static const WCHAR *g_wszWMDRM_RIGHT_PLAYLIST_BURN              = L"PlaylistBurn";
static const WCHAR *g_wszWMDRM_RIGHT_CREATE_THUMBNAIL_IMAGE     = L"CreateThumbnailImage";
static const WCHAR *g_wszWMDRM_RIGHT_COPY_TO_CD                 = L"Print.redbook";
static const WCHAR *g_wszWMDRM_RIGHT_COPY_TO_SDMI_DEVICE        = L"Transfer.SDMI";
static const WCHAR *g_wszWMDRM_RIGHT_COPY_TO_NON_SDMI_DEVICE    = L"Transfer.NONSDMI";
static const WCHAR *g_wszWMDRM_RIGHT_BACKUP                     = L"Backup";
static const WCHAR *g_wszWMDRM_RIGHT_COLLABORATIVE_PLAY         = L"CollaborativePlay";
static const WCHAR *g_wszWMDRM_ActionAllowed                        = L"ActionAllowed.";
static const WCHAR *g_wszWMDRM_ActionAllowed_Playback               = L"ActionAllowed.Play";
static const WCHAR *g_wszWMDRM_ActionAllowed_Copy                   = L"ActionAllowed.Copy";
static const WCHAR *g_wszWMDRM_ActionAllowed_PlaylistBurn           = L"ActionAllowed.PlaylistBurn";
static const WCHAR *g_wszWMDRM_ActionAllowed_CreateThumbnailImage   = L"ActionAllowed.CreateThumbnailImage";
static const WCHAR *g_wszWMDRM_ActionAllowed_CopyToCD               = L"ActionAllowed.Print.redbook";
static const WCHAR *g_wszWMDRM_ActionAllowed_CopyToSDMIDevice       = L"ActionAllowed.Transfer.SDMI";
static const WCHAR *g_wszWMDRM_ActionAllowed_CopyToNonSDMIDevice    = L"ActionAllowed.Transfer.NONSDMI";
static const WCHAR *g_wszWMDRM_ActionAllowed_Backup                 = L"ActionAllowed.Backup";
static const WCHAR *g_wszWMDRM_ActionAllowed_CollaborativePlay      = L"ActionAllowed.CollaborativePlay";
static const WCHAR *g_wszWMDRM_LicenseState                      = L"LicenseStateData.";
static const WCHAR *g_wszWMDRM_LicenseState_Playback             = L"LicenseStateData.Play";
static const WCHAR *g_wszWMDRM_LicenseState_Copy                 = L"LicenseStateData.Copy";
static const WCHAR *g_wszWMDRM_LicenseState_PlaylistBurn         = L"LicenseStateData.PlaylistBurn";
static const WCHAR *g_wszWMDRM_LicenseState_CreateThumbnailImage = L"LicenseStateData.CreateThumbnailImage";
static const WCHAR *g_wszWMDRM_LicenseState_CopyToCD             = L"LicenseStateData.Print.redbook";
static const WCHAR *g_wszWMDRM_LicenseState_CopyToSDMIDevice     = L"LicenseStateData.Transfer.SDMI";
static const WCHAR *g_wszWMDRM_LicenseState_CopyToNonSDMIDevice  = L"LicenseStateData.Transfer.NONSDMI";
static const WCHAR *g_wszWMDRM_LicenseState_Backup               = L"LicenseStateData.Backup";
static const WCHAR *g_wszWMDRM_LicenseState_CollaborativePlay    = L"LicenseStateData.CollaborativePlay";
static const WCHAR *g_wszWMDRMNET_Revocation                     = L"WMDRMNET_REVOCATION";
static const WCHAR *g_wszWMDRM_SAPLEVEL                          = L"SAPLEVEL";
static const WCHAR *g_wszWMDRM_SAPRequired                       = L"SAPRequired";
static const WCHAR *g_wszWMDRM_SOURCEID                          = L"SOURCEID";
static const WCHAR *g_wszWMDRM_PRIORITY                          = L"PRIORITY";
static const WCHAR *g_wszWMDRM_ISSUEDATE                         = L"ISSUEDATE";
static const WCHAR *g_wszWMDRM_UplinkID                          = L"UplinkID";
static const WCHAR *g_wszWMDRM_REVINFOVERSION                    = L"REVINFOVERSION";
*/

#define DRM_LICENSE_STATE_DATA_VAGUE       1
#define DRM_LICENSE_STATE_DATA_OPL_PRESENT 2
#define DRM_LICENSE_STATE_DATA_SAP_PRESENT 4
#define WMDRM_BACKUP_OVERWRITE       ((DWORD) 0x00000001)
#define WMDRM_RESTORE_INDIVIDUALIZE  ((DWORD) 0x00000002)
#define WMDRM_SECURITY_PERFORM_INDIV               0x00000001L
#define WMDRM_SECURITY_PERFORM_REVOCATION_REFRESH  0x00000002L
#define WMDRM_SECURITY_PERFORM_FORCE_INDIV         0x00000004L
#define WMDRM_LICENSE_TYPE_XML       0x00000001L
#define WMDRM_LICENSE_TYPE_XMR       0x00000002L
#define WMDRM_CERTIFICATE_TYPE_V1       0x00000001L
#define WMDRM_CERTIFICATE_TYPE_V2       0x00000002L
#define WMDRM_ACQUIRE_LICENSE_SILENT              0x00000001L
#define WMDRM_ACQUIRE_LICENSE_NONSILENT           0x00000002L
#define WMDRM_ACQUIRE_LICENSE_LEGACY_NONSILENT    0x00000004L
#define WMDRM_DELETE_LICENSE_IMMEDIATELY    0x00000001L
#define WMDRM_DELETE_LICENSE_MARK_FOR_PURGE 0x00000002L
#define WMDRM_CLEAN_LICENSE_STORE_SYNC  0x00000001L
#define WMDRM_CLEAN_LICENSE_STORE_ASYNC 0x00000002L
#define WMDRM_CERTIFICATE_TYPE_XML 1
#define WMDRM_PROTECTION_TYPE_RC4        1
EXTERN_GUID( WMDRM_REVOCATIONTYPE_APP,          0x90A37313, 0x0ECF, 0x4CAA, 0xA9, 0x06, 0xB1, 0x88, 0xF6, 0x12, 0x93, 0x00 );
EXTERN_GUID( WMDRM_REVOCATIONTYPE_DEVICE,       0x3129E375, 0xCEB0, 0x47D5, 0x9C, 0xCA, 0x9D, 0xB7, 0x4C, 0xFD, 0x43, 0x32 );
EXTERN_GUID( WMDRM_REVOCATIONTYPE_CARDEA,       0xCD75E604, 0x543D, 0x4A9C, 0x9F, 0x09, 0xFE, 0x6D, 0x24, 0xE8, 0xBF, 0x90 );
EXTERN_GUID( WMDRM_REVOCATIONTYPE_REVINFO,      0xCCDE5A55, 0xA688, 0x4405, 0xA8, 0x8B, 0xD1, 0x3F, 0x90, 0xD5, 0xBA, 0x3E );
EXTERN_GUID( WMDRM_REVOCATIONTYPE_GRL,          0xa2190240, 0xb2ca, 0x40b3, 0xb4, 0x8d, 0x9b, 0xc4, 0xc2, 0xdc, 0x42, 0x8d );
EXTERN_GUID( WMDRM_REVOCATIONTYPE_HDCP,         0xe627553a, 0x820f, 0x40fd, 0xb4, 0x65, 0xa4, 0x28, 0x30, 0xa5, 0x74, 0xc9 );
typedef 
enum DRM_CRYPTO_TYPE
    {	CRYPTO_TYPE_MCE	= 0
    } 	DRM_CRYPTO_TYPE;

#if !DRM_LICENSE_STATE_TYPES_DEFINED
#define DRM_LICENSE_STATE_TYPES_DEFINED 1
typedef 
enum DRM_LICENSE_STATE_CATEGORY
    {	WM_DRM_LICENSE_STATE_NORIGHT	= 0,
	WM_DRM_LICENSE_STATE_UNLIM	= ( WM_DRM_LICENSE_STATE_NORIGHT + 1 ) ,
	WM_DRM_LICENSE_STATE_COUNT	= ( WM_DRM_LICENSE_STATE_UNLIM + 1 ) ,
	WM_DRM_LICENSE_STATE_FROM	= ( WM_DRM_LICENSE_STATE_COUNT + 1 ) ,
	WM_DRM_LICENSE_STATE_UNTIL	= ( WM_DRM_LICENSE_STATE_FROM + 1 ) ,
	WM_DRM_LICENSE_STATE_FROM_UNTIL	= ( WM_DRM_LICENSE_STATE_UNTIL + 1 ) ,
	WM_DRM_LICENSE_STATE_COUNT_FROM	= ( WM_DRM_LICENSE_STATE_FROM_UNTIL + 1 ) ,
	WM_DRM_LICENSE_STATE_COUNT_UNTIL	= ( WM_DRM_LICENSE_STATE_COUNT_FROM + 1 ) ,
	WM_DRM_LICENSE_STATE_COUNT_FROM_UNTIL	= ( WM_DRM_LICENSE_STATE_COUNT_UNTIL + 1 ) ,
	WM_DRM_LICENSE_STATE_EXPIRATION_AFTER_FIRSTUSE	= ( WM_DRM_LICENSE_STATE_COUNT_FROM_UNTIL + 1 ) 
    } 	DRM_LICENSE_STATE_CATEGORY;

typedef struct _DRM_LICENSE_STATE_DATA
    {
    DWORD dwStreamId;
    DRM_LICENSE_STATE_CATEGORY dwCategory;
    DWORD dwNumCounts;
    DWORD dwCount[ 4 ];
    DWORD dwNumDates;
    FILETIME datetime[ 4 ];
    DWORD dwVague;
    } 	DRM_LICENSE_STATE_DATA;

#endif
typedef 
enum MSDRM_STATUS
    {	DRM_ERROR	= 0,
	DRM_INFORMATION	= 1,
	DRM_BACKUPRESTORE_BEGIN	= 2,
	DRM_BACKUPRESTORE_END	= 3,
	DRM_BACKUPRESTORE_CONNECTING	= 4,
	DRM_BACKUPRESTORE_DISCONNECTING	= 5,
	DRM_ERROR_WITHURL	= 6,
	DRM_RESTRICTED_LICENSE	= 7,
	DRM_NEEDS_INDIVIDUALIZATION	= 8,
	DRM_PLAY_OPL_NOTIFICATION	= 9,
	DRM_COPY_OPL_NOTIFICATION	= 10,
	DRM_REFRESHCRL_COMPLETE	= 11
    } 	MSDRM_STATUS;

typedef 
enum DRM_ATTR_DATATYPE
    {	DRM_TYPE_DWORD	= 0,
	DRM_TYPE_STRING	= 1,
	DRM_TYPE_BINARY	= 2,
	DRM_TYPE_BOOL	= 3,
	DRM_TYPE_QWORD	= 4,
	DRM_TYPE_WORD	= 5,
	DRM_TYPE_GUID	= 6
    } 	DRM_ATTR_DATATYPE;

typedef 
enum DRM_HTTP_STATUS
    {	HTTP_NOTINITIATED	= 0,
	HTTP_CONNECTING	= ( HTTP_NOTINITIATED + 1 ) ,
	HTTP_REQUESTING	= ( HTTP_CONNECTING + 1 ) ,
	HTTP_RECEIVING	= ( HTTP_REQUESTING + 1 ) ,
	HTTP_COMPLETED	= ( HTTP_RECEIVING + 1 ) 
    } 	DRM_HTTP_STATUS;

typedef 
enum DRM_INDIVIDUALIZATION_STATUS
    {	INDI_UNDEFINED	= 0,
	INDI_BEGIN	= 0x1,
	INDI_SUCCEED	= 0x2,
	INDI_FAIL	= 0x4,
	INDI_CANCEL	= 0x8,
	INDI_DOWNLOAD	= 0x10,
	INDI_INSTALL	= 0x20
    } 	DRM_INDIVIDUALIZATION_STATUS;

typedef 
enum _DRM_ACTION_ALLOWED_QUERY_RESULTS
    {	DRM_ACTION_ALLOWED_QUERY_NOT_ENABLED	= 0x1,
	DRM_ACTION_ALLOWED_QUERY_NOT_ENABLED_NO_LICENSE	= 0x2,
	DRM_ACTION_ALLOWED_QUERY_NOT_ENABLED_NO_RIGHT	= 0x4,
	DRM_ACTION_ALLOWED_QUERY_NOT_ENABLED_EXHAUSTED	= 0x8,
	DRM_ACTION_ALLOWED_QUERY_NOT_ENABLED_EXPIRED	= 0x10,
	DRM_ACTION_ALLOWED_QUERY_NOT_ENABLED_NOT_STARTED	= 0x20,
	DRM_ACTION_ALLOWED_QUERY_NOT_ENABLED_APPSEC_TOO_LOW	= 0x40,
	DRM_ACTION_ALLOWED_QUERY_NOT_ENABLED_REQ_INDIV	= 0x80,
	DRM_ACTION_ALLOWED_QUERY_NOT_ENABLED_COPY_OPL_TOO_LOW	= 0x100,
	DRM_ACTION_ALLOWED_QUERY_NOT_ENABLED_COPY_OPL_EXCLUDED	= 0x200,
	DRM_ACTION_ALLOWED_QUERY_NOT_ENABLED_NO_CLOCK_SUPPORT	= 0x400,
	DRM_ACTION_ALLOWED_QUERY_NOT_ENABLED_NO_METERING_SUPPORT	= 0x800,
	DRM_ACTION_ALLOWED_QUERY_NOT_ENABLED_CHAIN_DEPTH_TOO_HIGH	= 0x1000
    } 	DRM_ACTION_ALLOWED_QUERY_RESULTS;

typedef struct _WMDRMCryptoData
    {
    DRM_CRYPTO_TYPE cryptoType;
    unsigned __int64 qwCounterID;
    unsigned __int64 qwOffset;
    } 	WMDRMCryptoData;

typedef struct _WMIndividualizeStatus
    {
    HRESULT hr;
    DRM_INDIVIDUALIZATION_STATUS enIndiStatus;
    LPSTR pszIndiRespUrl;
    DWORD dwHTTPRequest;
    DRM_HTTP_STATUS enHTTPStatus;
    DWORD dwHTTPReadProgress;
    DWORD dwHTTPReadTotal;
    } 	WM_INDIVIDUALIZE_STATUS;

typedef struct _WMBackupRestoreData
    {
    MSDRM_STATUS eStatus;
    BSTR bstrError;
    } 	WM_BACKUP_RESTORE_STATUS;

#ifndef DRM_OPL_TYPES
#define DRM_OPL_TYPES 1
#define WMDRM_OPL_PLAY              ((DWORD) 0x00000001)
#define WMDRM_OPL_COPY              ((DWORD) 0x00000002)
typedef struct __tagDRM_MINIMUM_OUTPUT_PROTECTION_LEVELS
    {
    WORD wCompressedDigitalVideo;
    WORD wUncompressedDigitalVideo;
    WORD wAnalogVideo;
    WORD wCompressedDigitalAudio;
    WORD wUncompressedDigitalAudio;
    } 	DRM_MINIMUM_OUTPUT_PROTECTION_LEVELS;

typedef struct __tagDRM_OPL_OUTPUT_IDS
    {
    WORD cIds;
    GUID *rgIds;
    } 	DRM_OPL_OUTPUT_IDS;

#define VER_DRM_PLAY_OPL_V1                 1
#define VER_DRM_PLAY_OPL                    2
#define VER_DRM_VIDEO_OUTPUT_PROTECTION     2
typedef struct __tagDRM_OUTPUT_PROTECTION
    {
    GUID guidId;
    BYTE bConfigData;
    } 	DRM_OUTPUT_PROTECTION;

typedef struct __tagDRM_OUTPUT_PROTECTION_EX
    {
    DWORD dwVersion;
    GUID guidId;
    DWORD dwConfigData;
    } 	DRM_OUTPUT_PROTECTION_EX;

typedef DRM_OUTPUT_PROTECTION DRM_AUDIO_OUTPUT_PROTECTION;

typedef DRM_OUTPUT_PROTECTION DRM_VIDEO_OUTPUT_PROTECTION;

typedef DRM_OUTPUT_PROTECTION_EX DRM_AUDIO_OUTPUT_PROTECTION_EX;

typedef DRM_OUTPUT_PROTECTION_EX DRM_VIDEO_OUTPUT_PROTECTION_EX;

typedef struct __tagDRM_VIDEO_OUTPUT_PROTECTION_IDS
    {
    WORD cEntries;
    DRM_VIDEO_OUTPUT_PROTECTION *rgVop;
    } 	DRM_VIDEO_OUTPUT_PROTECTION_IDS;

typedef struct __tagDRM_VIDEO_OUTPUT_PROTECTION_IDS_EX
    {
    DWORD dwVersion;
    WORD cEntries;
    DRM_VIDEO_OUTPUT_PROTECTION_EX *rgVop;
    } 	DRM_VIDEO_OUTPUT_PROTECTION_IDS_EX;

typedef struct __tagDRM_AUDIO_OUTPUT_PROTECTION_IDS
    {
    WORD cEntries;
    DRM_AUDIO_OUTPUT_PROTECTION *rgAop;
    } 	DRM_AUDIO_OUTPUT_PROTECTION_IDS;

typedef struct __tagDRM_AUDIO_OUTPUT_PROTECTION_IDS_EX
    {
    DWORD dwVersion;
    WORD cEntries;
    DRM_AUDIO_OUTPUT_PROTECTION_EX *rgAop;
    } 	DRM_AUDIO_OUTPUT_PROTECTION_IDS_EX;

typedef struct __tagDRM_PLAY_OPL
    {
    DRM_MINIMUM_OUTPUT_PROTECTION_LEVELS minOPL;
    DRM_OPL_OUTPUT_IDS oplIdReserved;
    DRM_VIDEO_OUTPUT_PROTECTION_IDS vopi;
    } 	DRM_PLAY_OPL;

typedef struct __tagDRM_PLAY_OPL_EX
    {
    DWORD dwVersion;
    DRM_MINIMUM_OUTPUT_PROTECTION_LEVELS minOPL;
    DRM_OPL_OUTPUT_IDS oplIdReserved;
    DRM_VIDEO_OUTPUT_PROTECTION_IDS_EX vopi;
    } 	DRM_PLAY_OPL_EX;

typedef struct __tagDRM_COPY_OPL
    {
    WORD wMinimumCopyLevel;
    DRM_OPL_OUTPUT_IDS oplIdIncludes;
    DRM_OPL_OUTPUT_IDS oplIdExcludes;
    } 	DRM_COPY_OPL;

#endif // DRM_OPL_TYPES
typedef struct _WMDRMOutputProtectionLevels
    {
    WORD wCompressedDigitalVideo;
    WORD wUncompressedDigitalVideo;
    WORD wAnalogVideo;
    WORD wCompressedDigitalAudio;
    WORD wUncompressedDigitalAudio;
    WORD wMinimumCopyProtectionLevel;
    } 	WMDRM_OUTPUT_PROTECTION_LEVELS;

typedef struct _WMDRMAnalogVideoRestrictions
    {
    GUID guidRestrictionID;
    DWORD dwRestrictionData;
    } 	WMDRM_ANALOG_VIDEO_RESTRICTIONS;

typedef struct _WMDRMAnalogVideoRestrictionsEx
    {
    DWORD dwVersion;
    GUID guidRestrictionID;
    DWORD cbRestrictionData;
    BYTE *pbRestrictionData;
    } 	WMDRM_ANALOG_VIDEO_RESTRICTIONS_EX;

typedef 
enum _WMDRMNET_POLICY_TYPE
    {	WMDRMNET_POLICY_TYPE_UNDEFINED	= 0,
	WMDRMNET_POLICY_TYPE_TRANSCRYPTPLAY	= 0x1
    } 	WMDRMNET_POLICY_TYPE;

typedef struct _tagWMDRMNET_POLICY
    {
    WMDRMNET_POLICY_TYPE ePolicyType;
    BYTE *pbPolicy;
    } 	WMDRMNET_POLICY;

typedef struct _WMDRMNET_POLICY_TRANSCRYPTPLAY
    {
    DWORD cbData;
    BYTE *pbData;
    } 	WMDRMNET_POLICY_TRANSCRYPTPLAY;

typedef struct _WMDRM_ENCRYPT_SCATTER_INFO
    {
    DWORD dwStreamID;
    DWORD dwSampleProtectionVersion;
    DWORD cbProtectionInfo;
    BYTE *pbProtectionInfo;
    } 	WMDRM_ENCRYPT_SCATTER_INFO;

typedef struct WMDRM_ENCRYPT_SCATTER_BLOCK
    {
    DWORD dwStreamID;
    DWORD cbBlock;
    BYTE *pbBlock;
    } 	WMDRM_ENCRYPT_SCATTER_BLOCK;

EXTERN_C HRESULT STDMETHODCALLTYPE WMDRMStartup( );
EXTERN_C HRESULT STDMETHODCALLTYPE WMDRMShutdown( );
EXTERN_C HRESULT STDMETHODCALLTYPE WMDRMCreateProvider( OUT IWMDRMProvider **ppDRMProvider );
//
// WMDRMCreateProtectedProvider is only available via a Microsoft issued WMDRMSDK stub library
//
EXTERN_C HRESULT STDMETHODCALLTYPE WMDRMCreateProtectedProvider( OUT IWMDRMProvider **ppDRMProvider );


extern RPC_IF_HANDLE __MIDL_itf_wmdrmsdk_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wmdrmsdk_0000_0000_v0_0_s_ifspec;

#ifndef __IDRMStatusCallback_INTERFACE_DEFINED__
#define __IDRMStatusCallback_INTERFACE_DEFINED__

/* interface IDRMStatusCallback */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDRMStatusCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("08548704-75B1-4982-9B26-FB385DEE741D")
    IDRMStatusCallback : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnStatus( 
            /* [in] */ MSDRM_STATUS Status,
            /* [in] */ HRESULT hr,
            /* [in] */ DRM_ATTR_DATATYPE dwType,
            /* [in] */ BYTE *pValue,
            /* [in] */ void *pvContext) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDRMStatusCallbackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDRMStatusCallback * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDRMStatusCallback * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDRMStatusCallback * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnStatus )( 
            IDRMStatusCallback * This,
            /* [in] */ MSDRM_STATUS Status,
            /* [in] */ HRESULT hr,
            /* [in] */ DRM_ATTR_DATATYPE dwType,
            /* [in] */ BYTE *pValue,
            /* [in] */ void *pvContext);
        
        END_INTERFACE
    } IDRMStatusCallbackVtbl;

    interface IDRMStatusCallback
    {
        CONST_VTBL struct IDRMStatusCallbackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDRMStatusCallback_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDRMStatusCallback_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDRMStatusCallback_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDRMStatusCallback_OnStatus(This,Status,hr,dwType,pValue,pvContext)	\
    ( (This)->lpVtbl -> OnStatus(This,Status,hr,dwType,pValue,pvContext) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDRMStatusCallback_INTERFACE_DEFINED__ */


#ifndef __IWMDRMEventGenerator_INTERFACE_DEFINED__
#define __IWMDRMEventGenerator_INTERFACE_DEFINED__

/* interface IWMDRMEventGenerator */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMDRMEventGenerator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CE11CC91-E615-4E0B-BE1F-09B5AEC033F2")
    IWMDRMEventGenerator : public IMFMediaEventGenerator
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CancelAsyncOperation( 
            /* [in] */ IUnknown *punkCancelationCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Shutdown( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMDRMEventGeneratorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMDRMEventGenerator * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMDRMEventGenerator * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMDRMEventGenerator * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetEvent )( 
            IWMDRMEventGenerator * This,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IMFMediaEvent **ppEvent);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *BeginGetEvent )( 
            IWMDRMEventGenerator * This,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *EndGetEvent )( 
            IWMDRMEventGenerator * This,
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  IMFMediaEvent **ppEvent);
        
        HRESULT ( STDMETHODCALLTYPE *QueueEvent )( 
            IWMDRMEventGenerator * This,
            /* [in] */ MediaEventType met,
            /* [in] */ REFGUID guidExtendedType,
            /* [in] */ HRESULT hrStatus,
            /* [unique][in] */ const PROPVARIANT *pvValue);
        
        HRESULT ( STDMETHODCALLTYPE *CancelAsyncOperation )( 
            IWMDRMEventGenerator * This,
            /* [in] */ IUnknown *punkCancelationCookie);
        
        HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            IWMDRMEventGenerator * This);
        
        END_INTERFACE
    } IWMDRMEventGeneratorVtbl;

    interface IWMDRMEventGenerator
    {
        CONST_VTBL struct IWMDRMEventGeneratorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMDRMEventGenerator_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMDRMEventGenerator_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMDRMEventGenerator_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMDRMEventGenerator_GetEvent(This,dwFlags,ppEvent)	\
    ( (This)->lpVtbl -> GetEvent(This,dwFlags,ppEvent) ) 

#define IWMDRMEventGenerator_BeginGetEvent(This,pCallback,punkState)	\
    ( (This)->lpVtbl -> BeginGetEvent(This,pCallback,punkState) ) 

#define IWMDRMEventGenerator_EndGetEvent(This,pResult,ppEvent)	\
    ( (This)->lpVtbl -> EndGetEvent(This,pResult,ppEvent) ) 

#define IWMDRMEventGenerator_QueueEvent(This,met,guidExtendedType,hrStatus,pvValue)	\
    ( (This)->lpVtbl -> QueueEvent(This,met,guidExtendedType,hrStatus,pvValue) ) 


#define IWMDRMEventGenerator_CancelAsyncOperation(This,punkCancelationCookie)	\
    ( (This)->lpVtbl -> CancelAsyncOperation(This,punkCancelationCookie) ) 

#define IWMDRMEventGenerator_Shutdown(This)	\
    ( (This)->lpVtbl -> Shutdown(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMDRMEventGenerator_INTERFACE_DEFINED__ */


#ifndef __IWMDRMProvider_INTERFACE_DEFINED__
#define __IWMDRMProvider_INTERFACE_DEFINED__

/* interface IWMDRMProvider */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMDRMProvider;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("98DC8917-8640-4b1d-9F1E-A590D40357EE")
    IWMDRMProvider : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateObject( 
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMDRMProviderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMDRMProvider * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMDRMProvider * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMDRMProvider * This);
        
        HRESULT ( STDMETHODCALLTYPE *CreateObject )( 
            IWMDRMProvider * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        END_INTERFACE
    } IWMDRMProviderVtbl;

    interface IWMDRMProvider
    {
        CONST_VTBL struct IWMDRMProviderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMDRMProvider_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMDRMProvider_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMDRMProvider_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMDRMProvider_CreateObject(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> CreateObject(This,riid,ppvObject) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMDRMProvider_INTERFACE_DEFINED__ */


#ifndef __IWMDRMSecurity_INTERFACE_DEFINED__
#define __IWMDRMSecurity_INTERFACE_DEFINED__

/* interface IWMDRMSecurity */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMDRMSecurity;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("70800F56-5239-4a0b-A8B8-D53C6BAE4171")
    IWMDRMSecurity : public IWMDRMEventGenerator
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetSecurityVersion( 
            /* [out] */ BSTR *pbstrVersion) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMachineCertificate( 
            /* [in] */ DWORD dwCertificateType,
            /* [out] */ BYTE rgbVersion[ 4 ],
            /* [size_is][size_is][out] */ BYTE **ppbCertificate,
            /* [out] */ DWORD *pcbCertificate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PerformSecurityUpdate( 
            /* [in] */ DWORD dwFlags,
            /* [out] */ IUnknown **ppunkCancelationCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetRevocationData( 
            /* [in] */ REFGUID guidRevocationType,
            /* [size_is][in] */ BYTE *pbCRL,
            /* [in] */ DWORD cbCRL) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRevocationData( 
            /* [in] */ REFGUID guidRevocationType,
            /* [size_is][out] */ BYTE *pbCRL,
            /* [out][in] */ DWORD *pcbCRL) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRevocationDataVersion( 
            /* [in] */ REFGUID guidRevocationType,
            /* [out] */ ULONGLONG *pdwCRLVersion) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CheckCertForRevocation( 
            /* [in] */ REFGUID rguidRevocationList,
            /* [size_is][in] */ BYTE *pbCert,
            /* [in] */ DWORD cbCert,
            /* [out] */ BOOL *pfRevoked) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetContentEnablersForRevocations( 
            /* [size_is][in] */ BYTE **rgpbCerts,
            /* [size_is][in] */ DWORD *rgpdwCertSizes,
            /* [size_is][in] */ GUID **rgpguidCerts,
            /* [in] */ DWORD cCerts,
            /* [in] */ HRESULT hResultHint,
            /* [size_is][out] */ IMFContentEnabler **prgContentEnablers,
            /* [out][in] */ DWORD *pcContentEnablers) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetContentEnablersFromHashes( 
            /* [size_is][in] */ BSTR *rgpbCertHashes,
            /* [in] */ DWORD cCerts,
            /* [in] */ HRESULT hResultHint,
            /* [size_is][out] */ IMFContentEnabler **prgContentEnablers,
            /* [out][in] */ DWORD *pcContentEnablers) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMDRMSecurityVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMDRMSecurity * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMDRMSecurity * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMDRMSecurity * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetEvent )( 
            IWMDRMSecurity * This,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IMFMediaEvent **ppEvent);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *BeginGetEvent )( 
            IWMDRMSecurity * This,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *EndGetEvent )( 
            IWMDRMSecurity * This,
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  IMFMediaEvent **ppEvent);
        
        HRESULT ( STDMETHODCALLTYPE *QueueEvent )( 
            IWMDRMSecurity * This,
            /* [in] */ MediaEventType met,
            /* [in] */ REFGUID guidExtendedType,
            /* [in] */ HRESULT hrStatus,
            /* [unique][in] */ const PROPVARIANT *pvValue);
        
        HRESULT ( STDMETHODCALLTYPE *CancelAsyncOperation )( 
            IWMDRMSecurity * This,
            /* [in] */ IUnknown *punkCancelationCookie);
        
        HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            IWMDRMSecurity * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetSecurityVersion )( 
            IWMDRMSecurity * This,
            /* [out] */ BSTR *pbstrVersion);
        
        HRESULT ( STDMETHODCALLTYPE *GetMachineCertificate )( 
            IWMDRMSecurity * This,
            /* [in] */ DWORD dwCertificateType,
            /* [out] */ BYTE rgbVersion[ 4 ],
            /* [size_is][size_is][out] */ BYTE **ppbCertificate,
            /* [out] */ DWORD *pcbCertificate);
        
        HRESULT ( STDMETHODCALLTYPE *PerformSecurityUpdate )( 
            IWMDRMSecurity * This,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IUnknown **ppunkCancelationCookie);
        
        HRESULT ( STDMETHODCALLTYPE *SetRevocationData )( 
            IWMDRMSecurity * This,
            /* [in] */ REFGUID guidRevocationType,
            /* [size_is][in] */ BYTE *pbCRL,
            /* [in] */ DWORD cbCRL);
        
        HRESULT ( STDMETHODCALLTYPE *GetRevocationData )( 
            IWMDRMSecurity * This,
            /* [in] */ REFGUID guidRevocationType,
            /* [size_is][out] */ BYTE *pbCRL,
            /* [out][in] */ DWORD *pcbCRL);
        
        HRESULT ( STDMETHODCALLTYPE *GetRevocationDataVersion )( 
            IWMDRMSecurity * This,
            /* [in] */ REFGUID guidRevocationType,
            /* [out] */ ULONGLONG *pdwCRLVersion);
        
        HRESULT ( STDMETHODCALLTYPE *CheckCertForRevocation )( 
            IWMDRMSecurity * This,
            /* [in] */ REFGUID rguidRevocationList,
            /* [size_is][in] */ BYTE *pbCert,
            /* [in] */ DWORD cbCert,
            /* [out] */ BOOL *pfRevoked);
        
        HRESULT ( STDMETHODCALLTYPE *GetContentEnablersForRevocations )( 
            IWMDRMSecurity * This,
            /* [size_is][in] */ BYTE **rgpbCerts,
            /* [size_is][in] */ DWORD *rgpdwCertSizes,
            /* [size_is][in] */ GUID **rgpguidCerts,
            /* [in] */ DWORD cCerts,
            /* [in] */ HRESULT hResultHint,
            /* [size_is][out] */ IMFContentEnabler **prgContentEnablers,
            /* [out][in] */ DWORD *pcContentEnablers);
        
        HRESULT ( STDMETHODCALLTYPE *GetContentEnablersFromHashes )( 
            IWMDRMSecurity * This,
            /* [size_is][in] */ BSTR *rgpbCertHashes,
            /* [in] */ DWORD cCerts,
            /* [in] */ HRESULT hResultHint,
            /* [size_is][out] */ IMFContentEnabler **prgContentEnablers,
            /* [out][in] */ DWORD *pcContentEnablers);
        
        END_INTERFACE
    } IWMDRMSecurityVtbl;

    interface IWMDRMSecurity
    {
        CONST_VTBL struct IWMDRMSecurityVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMDRMSecurity_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMDRMSecurity_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMDRMSecurity_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMDRMSecurity_GetEvent(This,dwFlags,ppEvent)	\
    ( (This)->lpVtbl -> GetEvent(This,dwFlags,ppEvent) ) 

#define IWMDRMSecurity_BeginGetEvent(This,pCallback,punkState)	\
    ( (This)->lpVtbl -> BeginGetEvent(This,pCallback,punkState) ) 

#define IWMDRMSecurity_EndGetEvent(This,pResult,ppEvent)	\
    ( (This)->lpVtbl -> EndGetEvent(This,pResult,ppEvent) ) 

#define IWMDRMSecurity_QueueEvent(This,met,guidExtendedType,hrStatus,pvValue)	\
    ( (This)->lpVtbl -> QueueEvent(This,met,guidExtendedType,hrStatus,pvValue) ) 


#define IWMDRMSecurity_CancelAsyncOperation(This,punkCancelationCookie)	\
    ( (This)->lpVtbl -> CancelAsyncOperation(This,punkCancelationCookie) ) 

#define IWMDRMSecurity_Shutdown(This)	\
    ( (This)->lpVtbl -> Shutdown(This) ) 


#define IWMDRMSecurity_GetSecurityVersion(This,pbstrVersion)	\
    ( (This)->lpVtbl -> GetSecurityVersion(This,pbstrVersion) ) 

#define IWMDRMSecurity_GetMachineCertificate(This,dwCertificateType,rgbVersion,ppbCertificate,pcbCertificate)	\
    ( (This)->lpVtbl -> GetMachineCertificate(This,dwCertificateType,rgbVersion,ppbCertificate,pcbCertificate) ) 

#define IWMDRMSecurity_PerformSecurityUpdate(This,dwFlags,ppunkCancelationCookie)	\
    ( (This)->lpVtbl -> PerformSecurityUpdate(This,dwFlags,ppunkCancelationCookie) ) 

#define IWMDRMSecurity_SetRevocationData(This,guidRevocationType,pbCRL,cbCRL)	\
    ( (This)->lpVtbl -> SetRevocationData(This,guidRevocationType,pbCRL,cbCRL) ) 

#define IWMDRMSecurity_GetRevocationData(This,guidRevocationType,pbCRL,pcbCRL)	\
    ( (This)->lpVtbl -> GetRevocationData(This,guidRevocationType,pbCRL,pcbCRL) ) 

#define IWMDRMSecurity_GetRevocationDataVersion(This,guidRevocationType,pdwCRLVersion)	\
    ( (This)->lpVtbl -> GetRevocationDataVersion(This,guidRevocationType,pdwCRLVersion) ) 

#define IWMDRMSecurity_CheckCertForRevocation(This,rguidRevocationList,pbCert,cbCert,pfRevoked)	\
    ( (This)->lpVtbl -> CheckCertForRevocation(This,rguidRevocationList,pbCert,cbCert,pfRevoked) ) 

#define IWMDRMSecurity_GetContentEnablersForRevocations(This,rgpbCerts,rgpdwCertSizes,rgpguidCerts,cCerts,hResultHint,prgContentEnablers,pcContentEnablers)	\
    ( (This)->lpVtbl -> GetContentEnablersForRevocations(This,rgpbCerts,rgpdwCertSizes,rgpguidCerts,cCerts,hResultHint,prgContentEnablers,pcContentEnablers) ) 

#define IWMDRMSecurity_GetContentEnablersFromHashes(This,rgpbCertHashes,cCerts,hResultHint,prgContentEnablers,pcContentEnablers)	\
    ( (This)->lpVtbl -> GetContentEnablersFromHashes(This,rgpbCertHashes,cCerts,hResultHint,prgContentEnablers,pcContentEnablers) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMDRMSecurity_INTERFACE_DEFINED__ */


#ifndef __IWMDRMEncrypt_INTERFACE_DEFINED__
#define __IWMDRMEncrypt_INTERFACE_DEFINED__

/* interface IWMDRMEncrypt */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMDRMEncrypt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C9E0A5F4-DD22-4dc4-B795-88FA91F0516B")
    IWMDRMEncrypt : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Encrypt( 
            /* [size_is][out][in] */ BYTE *pbData,
            /* [in] */ DWORD cbData,
            /* [in] */ WMDRMCryptoData *pWMCryptoData) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMDRMEncryptVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMDRMEncrypt * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMDRMEncrypt * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMDRMEncrypt * This);
        
        HRESULT ( STDMETHODCALLTYPE *Encrypt )( 
            IWMDRMEncrypt * This,
            /* [size_is][out][in] */ BYTE *pbData,
            /* [in] */ DWORD cbData,
            /* [in] */ WMDRMCryptoData *pWMCryptoData);
        
        END_INTERFACE
    } IWMDRMEncryptVtbl;

    interface IWMDRMEncrypt
    {
        CONST_VTBL struct IWMDRMEncryptVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMDRMEncrypt_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMDRMEncrypt_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMDRMEncrypt_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMDRMEncrypt_Encrypt(This,pbData,cbData,pWMCryptoData)	\
    ( (This)->lpVtbl -> Encrypt(This,pbData,cbData,pWMCryptoData) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMDRMEncrypt_INTERFACE_DEFINED__ */


#ifndef __IWMDRMEncryptScatter_INTERFACE_DEFINED__
#define __IWMDRMEncryptScatter_INTERFACE_DEFINED__

/* interface IWMDRMEncryptScatter */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMDRMEncryptScatter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2662fa39-4c62-481a-956c-7bfcb33c2888")
    IWMDRMEncryptScatter : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE InitEncryptScatter( 
            /* [in] */ DWORD cStreams,
            /* [size_is][in] */ WMDRM_ENCRYPT_SCATTER_INFO *rgInfos) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EncryptScatter( 
            /* [in] */ DWORD cBlocks,
            /* [size_is][in] */ WMDRM_ENCRYPT_SCATTER_BLOCK *rgBlocks,
            /* [in] */ WMDRMCryptoData *pWMCryptoData,
            /* [in] */ DWORD cbOutput,
            /* [size_is][out] */ BYTE *pbOutput) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMDRMEncryptScatterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMDRMEncryptScatter * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMDRMEncryptScatter * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMDRMEncryptScatter * This);
        
        HRESULT ( STDMETHODCALLTYPE *InitEncryptScatter )( 
            IWMDRMEncryptScatter * This,
            /* [in] */ DWORD cStreams,
            /* [size_is][in] */ WMDRM_ENCRYPT_SCATTER_INFO *rgInfos);
        
        HRESULT ( STDMETHODCALLTYPE *EncryptScatter )( 
            IWMDRMEncryptScatter * This,
            /* [in] */ DWORD cBlocks,
            /* [size_is][in] */ WMDRM_ENCRYPT_SCATTER_BLOCK *rgBlocks,
            /* [in] */ WMDRMCryptoData *pWMCryptoData,
            /* [in] */ DWORD cbOutput,
            /* [size_is][out] */ BYTE *pbOutput);
        
        END_INTERFACE
    } IWMDRMEncryptScatterVtbl;

    interface IWMDRMEncryptScatter
    {
        CONST_VTBL struct IWMDRMEncryptScatterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMDRMEncryptScatter_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMDRMEncryptScatter_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMDRMEncryptScatter_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMDRMEncryptScatter_InitEncryptScatter(This,cStreams,rgInfos)	\
    ( (This)->lpVtbl -> InitEncryptScatter(This,cStreams,rgInfos) ) 

#define IWMDRMEncryptScatter_EncryptScatter(This,cBlocks,rgBlocks,pWMCryptoData,cbOutput,pbOutput)	\
    ( (This)->lpVtbl -> EncryptScatter(This,cBlocks,rgBlocks,pWMCryptoData,cbOutput,pbOutput) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMDRMEncryptScatter_INTERFACE_DEFINED__ */


#ifndef __IWMDRMDecrypt_INTERFACE_DEFINED__
#define __IWMDRMDecrypt_INTERFACE_DEFINED__

/* interface IWMDRMDecrypt */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMDRMDecrypt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C9E0A5F5-DD22-4dc4-B795-88FA91F0516B")
    IWMDRMDecrypt : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Decrypt( 
            /* [size_is][out][in] */ BYTE *pbData,
            /* [in] */ DWORD cbData,
            /* [in] */ WMDRMCryptoData *pWMCryptoData) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMDRMDecryptVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMDRMDecrypt * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMDRMDecrypt * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMDRMDecrypt * This);
        
        HRESULT ( STDMETHODCALLTYPE *Decrypt )( 
            IWMDRMDecrypt * This,
            /* [size_is][out][in] */ BYTE *pbData,
            /* [in] */ DWORD cbData,
            /* [in] */ WMDRMCryptoData *pWMCryptoData);
        
        END_INTERFACE
    } IWMDRMDecryptVtbl;

    interface IWMDRMDecrypt
    {
        CONST_VTBL struct IWMDRMDecryptVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMDRMDecrypt_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMDRMDecrypt_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMDRMDecrypt_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMDRMDecrypt_Decrypt(This,pbData,cbData,pWMCryptoData)	\
    ( (This)->lpVtbl -> Decrypt(This,pbData,cbData,pWMCryptoData) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMDRMDecrypt_INTERFACE_DEFINED__ */


#ifndef __IWMDRMLicense_INTERFACE_DEFINED__
#define __IWMDRMLicense_INTERFACE_DEFINED__

/* interface IWMDRMLicense */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMDRMLicense;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C9E0A5F3-DD22-4dc4-B795-88FA91F0516B")
    IWMDRMLicense : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ResetEnumeration( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNext( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLicense( 
            /* [size_is][size_is][out] */ BYTE **ppbLicense,
            /* [out] */ DWORD *pcbLicense,
            /* [out] */ DWORD *pdwLicenseType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLicenseProperty( 
            /* [in] */ BSTR bstrName,
            /* [out] */ PROPVARIANT *ppropVariant) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateDecryptor( 
            /* [out] */ IWMDRMDecrypt **ppDecryptor) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateSecureDecryptor( 
            /* [size_is][in] */ BYTE *pbCertificate,
            /* [in] */ DWORD cbCertificate,
            /* [in] */ DWORD dwCertificateType,
            /* [in] */ DWORD dwFlags,
            /* [out] */ BYTE *pbInitializationVector,
            /* [out] */ DWORD *pcbInitializationVector,
            /* [out] */ IWMDRMDecrypt **ppDecryptor) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateEncryptor( 
            /* [out] */ IWMDRMEncrypt **ppEncryptor) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PersistLicense( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CanPersist( 
            /* [out] */ BOOL *pfCanPersist) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAnalogVideoRestrictionLevels( 
            /* [size_is][out] */ WMDRM_ANALOG_VIDEO_RESTRICTIONS rgAnalogVideoRestrictions[  ],
            /* [out][in] */ DWORD *pcRestrictions) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetOutputProtectionLevels( 
            /* [out] */ WMDRM_OUTPUT_PROTECTION_LEVELS *pOPLs) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetInclusionList( 
            /* [size_is][size_is][out] */ GUID **ppGuids,
            /* [out] */ DWORD *pcGuids) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPolicy( 
            /* [size_is][size_is][out] */ BYTE **ppbPolicy,
            /* [out] */ DWORD *pcbPolicy) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMDRMLicenseVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMDRMLicense * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMDRMLicense * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMDRMLicense * This);
        
        HRESULT ( STDMETHODCALLTYPE *ResetEnumeration )( 
            IWMDRMLicense * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetNext )( 
            IWMDRMLicense * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetLicense )( 
            IWMDRMLicense * This,
            /* [size_is][size_is][out] */ BYTE **ppbLicense,
            /* [out] */ DWORD *pcbLicense,
            /* [out] */ DWORD *pdwLicenseType);
        
        HRESULT ( STDMETHODCALLTYPE *GetLicenseProperty )( 
            IWMDRMLicense * This,
            /* [in] */ BSTR bstrName,
            /* [out] */ PROPVARIANT *ppropVariant);
        
        HRESULT ( STDMETHODCALLTYPE *CreateDecryptor )( 
            IWMDRMLicense * This,
            /* [out] */ IWMDRMDecrypt **ppDecryptor);
        
        HRESULT ( STDMETHODCALLTYPE *CreateSecureDecryptor )( 
            IWMDRMLicense * This,
            /* [size_is][in] */ BYTE *pbCertificate,
            /* [in] */ DWORD cbCertificate,
            /* [in] */ DWORD dwCertificateType,
            /* [in] */ DWORD dwFlags,
            /* [out] */ BYTE *pbInitializationVector,
            /* [out] */ DWORD *pcbInitializationVector,
            /* [out] */ IWMDRMDecrypt **ppDecryptor);
        
        HRESULT ( STDMETHODCALLTYPE *CreateEncryptor )( 
            IWMDRMLicense * This,
            /* [out] */ IWMDRMEncrypt **ppEncryptor);
        
        HRESULT ( STDMETHODCALLTYPE *PersistLicense )( 
            IWMDRMLicense * This);
        
        HRESULT ( STDMETHODCALLTYPE *CanPersist )( 
            IWMDRMLicense * This,
            /* [out] */ BOOL *pfCanPersist);
        
        HRESULT ( STDMETHODCALLTYPE *GetAnalogVideoRestrictionLevels )( 
            IWMDRMLicense * This,
            /* [size_is][out] */ WMDRM_ANALOG_VIDEO_RESTRICTIONS rgAnalogVideoRestrictions[  ],
            /* [out][in] */ DWORD *pcRestrictions);
        
        HRESULT ( STDMETHODCALLTYPE *GetOutputProtectionLevels )( 
            IWMDRMLicense * This,
            /* [out] */ WMDRM_OUTPUT_PROTECTION_LEVELS *pOPLs);
        
        HRESULT ( STDMETHODCALLTYPE *GetInclusionList )( 
            IWMDRMLicense * This,
            /* [size_is][size_is][out] */ GUID **ppGuids,
            /* [out] */ DWORD *pcGuids);
        
        HRESULT ( STDMETHODCALLTYPE *GetPolicy )( 
            IWMDRMLicense * This,
            /* [size_is][size_is][out] */ BYTE **ppbPolicy,
            /* [out] */ DWORD *pcbPolicy);
        
        END_INTERFACE
    } IWMDRMLicenseVtbl;

    interface IWMDRMLicense
    {
        CONST_VTBL struct IWMDRMLicenseVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMDRMLicense_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMDRMLicense_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMDRMLicense_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMDRMLicense_ResetEnumeration(This)	\
    ( (This)->lpVtbl -> ResetEnumeration(This) ) 

#define IWMDRMLicense_GetNext(This)	\
    ( (This)->lpVtbl -> GetNext(This) ) 

#define IWMDRMLicense_GetLicense(This,ppbLicense,pcbLicense,pdwLicenseType)	\
    ( (This)->lpVtbl -> GetLicense(This,ppbLicense,pcbLicense,pdwLicenseType) ) 

#define IWMDRMLicense_GetLicenseProperty(This,bstrName,ppropVariant)	\
    ( (This)->lpVtbl -> GetLicenseProperty(This,bstrName,ppropVariant) ) 

#define IWMDRMLicense_CreateDecryptor(This,ppDecryptor)	\
    ( (This)->lpVtbl -> CreateDecryptor(This,ppDecryptor) ) 

#define IWMDRMLicense_CreateSecureDecryptor(This,pbCertificate,cbCertificate,dwCertificateType,dwFlags,pbInitializationVector,pcbInitializationVector,ppDecryptor)	\
    ( (This)->lpVtbl -> CreateSecureDecryptor(This,pbCertificate,cbCertificate,dwCertificateType,dwFlags,pbInitializationVector,pcbInitializationVector,ppDecryptor) ) 

#define IWMDRMLicense_CreateEncryptor(This,ppEncryptor)	\
    ( (This)->lpVtbl -> CreateEncryptor(This,ppEncryptor) ) 

#define IWMDRMLicense_PersistLicense(This)	\
    ( (This)->lpVtbl -> PersistLicense(This) ) 

#define IWMDRMLicense_CanPersist(This,pfCanPersist)	\
    ( (This)->lpVtbl -> CanPersist(This,pfCanPersist) ) 

#define IWMDRMLicense_GetAnalogVideoRestrictionLevels(This,rgAnalogVideoRestrictions,pcRestrictions)	\
    ( (This)->lpVtbl -> GetAnalogVideoRestrictionLevels(This,rgAnalogVideoRestrictions,pcRestrictions) ) 

#define IWMDRMLicense_GetOutputProtectionLevels(This,pOPLs)	\
    ( (This)->lpVtbl -> GetOutputProtectionLevels(This,pOPLs) ) 

#define IWMDRMLicense_GetInclusionList(This,ppGuids,pcGuids)	\
    ( (This)->lpVtbl -> GetInclusionList(This,ppGuids,pcGuids) ) 

#define IWMDRMLicense_GetPolicy(This,ppbPolicy,pcbPolicy)	\
    ( (This)->lpVtbl -> GetPolicy(This,ppbPolicy,pcbPolicy) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMDRMLicense_INTERFACE_DEFINED__ */


#ifndef __IWMDRMLicenseQuery_INTERFACE_DEFINED__
#define __IWMDRMLicenseQuery_INTERFACE_DEFINED__

/* interface IWMDRMLicenseQuery */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMDRMLicenseQuery;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("20B970DF-0A58-4d3c-817C-4E3CCBB253AC")
    IWMDRMLicenseQuery : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE QueryLicenseState( 
            /* [in] */ __RPC__in BSTR bstrKID,
            /* [in] */ DWORD cActionsToQuery,
            /* [size_is][in] */ __RPC__in_ecount_full(cActionsToQuery) BSTR rgbstrActionsToQuery[  ],
            /* [size_is][out] */ __RPC__out_ecount_full(cActionsToQuery) DRM_LICENSE_STATE_DATA rgResultStateData[  ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetActionAllowedQueryParams( 
            /* [in] */ BOOL fIsMF,
            /* [in] */ DWORD dwAppSecLevel,
            /* [in] */ BOOL fHasSerialNumber,
            /* [in] */ __RPC__in BSTR bstrDeviceCert) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryActionAllowed( 
            /* [in] */ __RPC__in BSTR bstrKID,
            /* [in] */ __RPC__in BSTR bstrMinReqIndivVersion,
            /* [in] */ DWORD cActionsToQuery,
            /* [size_is][in] */ __RPC__in_ecount_full(cActionsToQuery) BSTR rgbstrActionsToQuery[  ],
            /* [size_is][out] */ __RPC__out_ecount_full(cActionsToQuery) DWORD rgdwQueryResult[  ]) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMDRMLicenseQueryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMDRMLicenseQuery * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMDRMLicenseQuery * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMDRMLicenseQuery * This);
        
        HRESULT ( STDMETHODCALLTYPE *QueryLicenseState )( 
            IWMDRMLicenseQuery * This,
            /* [in] */ __RPC__in BSTR bstrKID,
            /* [in] */ DWORD cActionsToQuery,
            /* [size_is][in] */ __RPC__in_ecount_full(cActionsToQuery) BSTR rgbstrActionsToQuery[  ],
            /* [size_is][out] */ __RPC__out_ecount_full(cActionsToQuery) DRM_LICENSE_STATE_DATA rgResultStateData[  ]);
        
        HRESULT ( STDMETHODCALLTYPE *SetActionAllowedQueryParams )( 
            IWMDRMLicenseQuery * This,
            /* [in] */ BOOL fIsMF,
            /* [in] */ DWORD dwAppSecLevel,
            /* [in] */ BOOL fHasSerialNumber,
            /* [in] */ __RPC__in BSTR bstrDeviceCert);
        
        HRESULT ( STDMETHODCALLTYPE *QueryActionAllowed )( 
            IWMDRMLicenseQuery * This,
            /* [in] */ __RPC__in BSTR bstrKID,
            /* [in] */ __RPC__in BSTR bstrMinReqIndivVersion,
            /* [in] */ DWORD cActionsToQuery,
            /* [size_is][in] */ __RPC__in_ecount_full(cActionsToQuery) BSTR rgbstrActionsToQuery[  ],
            /* [size_is][out] */ __RPC__out_ecount_full(cActionsToQuery) DWORD rgdwQueryResult[  ]);
        
        END_INTERFACE
    } IWMDRMLicenseQueryVtbl;

    interface IWMDRMLicenseQuery
    {
        CONST_VTBL struct IWMDRMLicenseQueryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMDRMLicenseQuery_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMDRMLicenseQuery_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMDRMLicenseQuery_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMDRMLicenseQuery_QueryLicenseState(This,bstrKID,cActionsToQuery,rgbstrActionsToQuery,rgResultStateData)	\
    ( (This)->lpVtbl -> QueryLicenseState(This,bstrKID,cActionsToQuery,rgbstrActionsToQuery,rgResultStateData) ) 

#define IWMDRMLicenseQuery_SetActionAllowedQueryParams(This,fIsMF,dwAppSecLevel,fHasSerialNumber,bstrDeviceCert)	\
    ( (This)->lpVtbl -> SetActionAllowedQueryParams(This,fIsMF,dwAppSecLevel,fHasSerialNumber,bstrDeviceCert) ) 

#define IWMDRMLicenseQuery_QueryActionAllowed(This,bstrKID,bstrMinReqIndivVersion,cActionsToQuery,rgbstrActionsToQuery,rgdwQueryResult)	\
    ( (This)->lpVtbl -> QueryActionAllowed(This,bstrKID,bstrMinReqIndivVersion,cActionsToQuery,rgbstrActionsToQuery,rgdwQueryResult) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMDRMLicenseQuery_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_wmdrmsdk_0000_0009 */
/* [local] */ 

typedef struct WMDRM_LICENSE_FILTER
    {
    DWORD dwVersion;
    BSTR bstrKID;
    BSTR bstrRights;
    BSTR bstrAllowedSourceIDs;
    } 	WMDRM_LICENSE_FILTER;



extern RPC_IF_HANDLE __MIDL_itf_wmdrmsdk_0000_0009_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wmdrmsdk_0000_0009_v0_0_s_ifspec;

#ifndef __IWMDRMLicenseManagement_INTERFACE_DEFINED__
#define __IWMDRMLicenseManagement_INTERFACE_DEFINED__

/* interface IWMDRMLicenseManagement */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMDRMLicenseManagement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F4828DC7-8945-4D05-AB05-667AB99D29EE")
    IWMDRMLicenseManagement : public IWMDRMEventGenerator
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateLicenseEnumeration( 
            /* [in] */ __RPC__in WMDRM_LICENSE_FILTER *pLicenseFilter,
            /* [out] */ __RPC__deref_out_opt IWMDRMLicense **pEnumerator) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteLicense( 
            /* [in] */ __RPC__in BSTR bstrKID,
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CleanLicenseStore( 
            /* [in] */ DWORD dwFlags,
            /* [out] */ __RPC__deref_out_opt IUnknown **ppunkCancelationCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE StoreLicense( 
            /* [in] */ __RPC__in BSTR bstrLicenseResponse) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AcquireLicense( 
            /* [in] */ __RPC__in BSTR bstrURL,
            /* [in] */ __RPC__in BSTR bstrHeaderData,
            /* [in] */ __RPC__in BSTR bstrActions,
            /* [in] */ DWORD dwFlags,
            /* [out] */ __RPC__deref_out_opt IUnknown **ppunkCancelationCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MonitorLicenseAcquisition( 
            /* [in] */ __RPC__in BSTR bstrKID,
            /* [in] */ __RPC__in BSTR bstrHeader,
            /* [in] */ __RPC__in BSTR bstrActions,
            /* [out] */ __RPC__deref_out_opt IUnknown **ppunkCancelationCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BackupLicenses( 
            /* [in] */ __RPC__in BSTR bstrBackupDirectory,
            /* [in] */ DWORD dwFlags,
            /* [out] */ __RPC__deref_out_opt IUnknown **ppunkCancelationCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RestoreLicenses( 
            /* [in] */ __RPC__in BSTR bstrBackupDirectory,
            /* [in] */ DWORD dwFlags,
            /* [out] */ __RPC__deref_out_opt IUnknown **ppunkCancelationCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateLicenseRevocationChallenge( 
            /* [size_is][in] */ __RPC__in_ecount_full(cbMachineID) BYTE *pbMachineID,
            /* [in] */ DWORD cbMachineID,
            /* [size_is][in] */ __RPC__in_ecount_full(cbChallenge) BYTE *pbChallenge,
            /* [in] */ DWORD cbChallenge,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbChallengeOutput) BYTE **ppbChallengeOutput,
            /* [out] */ __RPC__out DWORD *pcbChallengeOutput) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ProcessLicenseRevocationResponse( 
            /* [size_is][in] */ __RPC__in_ecount_full(cbSignedLRB) BYTE *pbSignedLRB,
            /* [in] */ DWORD cbSignedLRB,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbSignedACK) BYTE **ppbSignedACK,
            /* [out] */ __RPC__out DWORD *pcbSignedACK) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ProcessLicenseDeletionMessage( 
            /* [in] */ __RPC__in BSTR bstrDeletionMessage) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMDRMLicenseManagementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMDRMLicenseManagement * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMDRMLicenseManagement * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMDRMLicenseManagement * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetEvent )( 
            IWMDRMLicenseManagement * This,
            /* [in] */ DWORD dwFlags,
            /* [out] */ __RPC__deref_out_opt IMFMediaEvent **ppEvent);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *BeginGetEvent )( 
            IWMDRMLicenseManagement * This,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *EndGetEvent )( 
            IWMDRMLicenseManagement * This,
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  IMFMediaEvent **ppEvent);
        
        HRESULT ( STDMETHODCALLTYPE *QueueEvent )( 
            IWMDRMLicenseManagement * This,
            /* [in] */ MediaEventType met,
            /* [in] */ __RPC__in REFGUID guidExtendedType,
            /* [in] */ HRESULT hrStatus,
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvValue);
        
        HRESULT ( STDMETHODCALLTYPE *CancelAsyncOperation )( 
            IWMDRMLicenseManagement * This,
            /* [in] */ __RPC__in_opt IUnknown *punkCancelationCookie);
        
        HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            IWMDRMLicenseManagement * This);
        
        HRESULT ( STDMETHODCALLTYPE *CreateLicenseEnumeration )( 
            IWMDRMLicenseManagement * This,
            /* [in] */ __RPC__in WMDRM_LICENSE_FILTER *pLicenseFilter,
            /* [out] */ __RPC__deref_out_opt IWMDRMLicense **pEnumerator);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteLicense )( 
            IWMDRMLicenseManagement * This,
            /* [in] */ __RPC__in BSTR bstrKID,
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE *CleanLicenseStore )( 
            IWMDRMLicenseManagement * This,
            /* [in] */ DWORD dwFlags,
            /* [out] */ __RPC__deref_out_opt IUnknown **ppunkCancelationCookie);
        
        HRESULT ( STDMETHODCALLTYPE *StoreLicense )( 
            IWMDRMLicenseManagement * This,
            /* [in] */ __RPC__in BSTR bstrLicenseResponse);
        
        HRESULT ( STDMETHODCALLTYPE *AcquireLicense )( 
            IWMDRMLicenseManagement * This,
            /* [in] */ __RPC__in BSTR bstrURL,
            /* [in] */ __RPC__in BSTR bstrHeaderData,
            /* [in] */ __RPC__in BSTR bstrActions,
            /* [in] */ DWORD dwFlags,
            /* [out] */ __RPC__deref_out_opt IUnknown **ppunkCancelationCookie);
        
        HRESULT ( STDMETHODCALLTYPE *MonitorLicenseAcquisition )( 
            IWMDRMLicenseManagement * This,
            /* [in] */ __RPC__in BSTR bstrKID,
            /* [in] */ __RPC__in BSTR bstrHeader,
            /* [in] */ __RPC__in BSTR bstrActions,
            /* [out] */ __RPC__deref_out_opt IUnknown **ppunkCancelationCookie);
        
        HRESULT ( STDMETHODCALLTYPE *BackupLicenses )( 
            IWMDRMLicenseManagement * This,
            /* [in] */ __RPC__in BSTR bstrBackupDirectory,
            /* [in] */ DWORD dwFlags,
            /* [out] */ __RPC__deref_out_opt IUnknown **ppunkCancelationCookie);
        
        HRESULT ( STDMETHODCALLTYPE *RestoreLicenses )( 
            IWMDRMLicenseManagement * This,
            /* [in] */ __RPC__in BSTR bstrBackupDirectory,
            /* [in] */ DWORD dwFlags,
            /* [out] */ __RPC__deref_out_opt IUnknown **ppunkCancelationCookie);
        
        HRESULT ( STDMETHODCALLTYPE *CreateLicenseRevocationChallenge )( 
            IWMDRMLicenseManagement * This,
            /* [size_is][in] */ __RPC__in_ecount_full(cbMachineID) BYTE *pbMachineID,
            /* [in] */ DWORD cbMachineID,
            /* [size_is][in] */ __RPC__in_ecount_full(cbChallenge) BYTE *pbChallenge,
            /* [in] */ DWORD cbChallenge,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbChallengeOutput) BYTE **ppbChallengeOutput,
            /* [out] */ __RPC__out DWORD *pcbChallengeOutput);
        
        HRESULT ( STDMETHODCALLTYPE *ProcessLicenseRevocationResponse )( 
            IWMDRMLicenseManagement * This,
            /* [size_is][in] */ __RPC__in_ecount_full(cbSignedLRB) BYTE *pbSignedLRB,
            /* [in] */ DWORD cbSignedLRB,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbSignedACK) BYTE **ppbSignedACK,
            /* [out] */ __RPC__out DWORD *pcbSignedACK);
        
        HRESULT ( STDMETHODCALLTYPE *ProcessLicenseDeletionMessage )( 
            IWMDRMLicenseManagement * This,
            /* [in] */ __RPC__in BSTR bstrDeletionMessage);
        
        END_INTERFACE
    } IWMDRMLicenseManagementVtbl;

    interface IWMDRMLicenseManagement
    {
        CONST_VTBL struct IWMDRMLicenseManagementVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMDRMLicenseManagement_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMDRMLicenseManagement_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMDRMLicenseManagement_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMDRMLicenseManagement_GetEvent(This,dwFlags,ppEvent)	\
    ( (This)->lpVtbl -> GetEvent(This,dwFlags,ppEvent) ) 

#define IWMDRMLicenseManagement_BeginGetEvent(This,pCallback,punkState)	\
    ( (This)->lpVtbl -> BeginGetEvent(This,pCallback,punkState) ) 

#define IWMDRMLicenseManagement_EndGetEvent(This,pResult,ppEvent)	\
    ( (This)->lpVtbl -> EndGetEvent(This,pResult,ppEvent) ) 

#define IWMDRMLicenseManagement_QueueEvent(This,met,guidExtendedType,hrStatus,pvValue)	\
    ( (This)->lpVtbl -> QueueEvent(This,met,guidExtendedType,hrStatus,pvValue) ) 


#define IWMDRMLicenseManagement_CancelAsyncOperation(This,punkCancelationCookie)	\
    ( (This)->lpVtbl -> CancelAsyncOperation(This,punkCancelationCookie) ) 

#define IWMDRMLicenseManagement_Shutdown(This)	\
    ( (This)->lpVtbl -> Shutdown(This) ) 


#define IWMDRMLicenseManagement_CreateLicenseEnumeration(This,pLicenseFilter,pEnumerator)	\
    ( (This)->lpVtbl -> CreateLicenseEnumeration(This,pLicenseFilter,pEnumerator) ) 

#define IWMDRMLicenseManagement_DeleteLicense(This,bstrKID,dwFlags)	\
    ( (This)->lpVtbl -> DeleteLicense(This,bstrKID,dwFlags) ) 

#define IWMDRMLicenseManagement_CleanLicenseStore(This,dwFlags,ppunkCancelationCookie)	\
    ( (This)->lpVtbl -> CleanLicenseStore(This,dwFlags,ppunkCancelationCookie) ) 

#define IWMDRMLicenseManagement_StoreLicense(This,bstrLicenseResponse)	\
    ( (This)->lpVtbl -> StoreLicense(This,bstrLicenseResponse) ) 

#define IWMDRMLicenseManagement_AcquireLicense(This,bstrURL,bstrHeaderData,bstrActions,dwFlags,ppunkCancelationCookie)	\
    ( (This)->lpVtbl -> AcquireLicense(This,bstrURL,bstrHeaderData,bstrActions,dwFlags,ppunkCancelationCookie) ) 

#define IWMDRMLicenseManagement_MonitorLicenseAcquisition(This,bstrKID,bstrHeader,bstrActions,ppunkCancelationCookie)	\
    ( (This)->lpVtbl -> MonitorLicenseAcquisition(This,bstrKID,bstrHeader,bstrActions,ppunkCancelationCookie) ) 

#define IWMDRMLicenseManagement_BackupLicenses(This,bstrBackupDirectory,dwFlags,ppunkCancelationCookie)	\
    ( (This)->lpVtbl -> BackupLicenses(This,bstrBackupDirectory,dwFlags,ppunkCancelationCookie) ) 

#define IWMDRMLicenseManagement_RestoreLicenses(This,bstrBackupDirectory,dwFlags,ppunkCancelationCookie)	\
    ( (This)->lpVtbl -> RestoreLicenses(This,bstrBackupDirectory,dwFlags,ppunkCancelationCookie) ) 

#define IWMDRMLicenseManagement_CreateLicenseRevocationChallenge(This,pbMachineID,cbMachineID,pbChallenge,cbChallenge,ppbChallengeOutput,pcbChallengeOutput)	\
    ( (This)->lpVtbl -> CreateLicenseRevocationChallenge(This,pbMachineID,cbMachineID,pbChallenge,cbChallenge,ppbChallengeOutput,pcbChallengeOutput) ) 

#define IWMDRMLicenseManagement_ProcessLicenseRevocationResponse(This,pbSignedLRB,cbSignedLRB,ppbSignedACK,pcbSignedACK)	\
    ( (This)->lpVtbl -> ProcessLicenseRevocationResponse(This,pbSignedLRB,cbSignedLRB,ppbSignedACK,pcbSignedACK) ) 

#define IWMDRMLicenseManagement_ProcessLicenseDeletionMessage(This,bstrDeletionMessage)	\
    ( (This)->lpVtbl -> ProcessLicenseDeletionMessage(This,bstrDeletionMessage) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMDRMLicenseManagement_INTERFACE_DEFINED__ */


#ifndef __IWMDRMNetReceiver_INTERFACE_DEFINED__
#define __IWMDRMNetReceiver_INTERFACE_DEFINED__

/* interface IWMDRMNetReceiver */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMDRMNetReceiver;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F9C074A4-15D6-44C0-8A6D-2446ACC109AE")
    IWMDRMNetReceiver : public IWMDRMEventGenerator
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetRegistrationChallenge( 
            /* [size_is][size_is][out] */ BYTE **ppbRegistrationChallenge,
            /* [out] */ DWORD *pcbRegistrationChallenge) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ProcessRegistrationResponse( 
            /* [size_is][in] */ BYTE *pbRegistrationResponse,
            /* [in] */ DWORD cbRegistrationResponse,
            /* [out] */ IUnknown **ppunkCancellationCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLicenseChallenge( 
            /* [in] */ BSTR bstrAction,
            /* [size_is][size_is][out] */ BYTE **ppbLicenseChallenge,
            /* [out] */ DWORD *pcbLicenseChallenge) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ProcessLicenseResponse( 
            /* [size_is][in] */ BYTE *pbLicenseResponse,
            /* [in] */ DWORD cbLicenseResponse,
            /* [size_is][size_is][out] */ BYTE **ppbWMDRMNetLicenseRepresentation,
            /* [out] */ DWORD *pcbWMDRMNetLicenseRepresentation) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMDRMNetReceiverVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMDRMNetReceiver * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMDRMNetReceiver * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMDRMNetReceiver * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetEvent )( 
            IWMDRMNetReceiver * This,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IMFMediaEvent **ppEvent);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *BeginGetEvent )( 
            IWMDRMNetReceiver * This,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *EndGetEvent )( 
            IWMDRMNetReceiver * This,
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  IMFMediaEvent **ppEvent);
        
        HRESULT ( STDMETHODCALLTYPE *QueueEvent )( 
            IWMDRMNetReceiver * This,
            /* [in] */ MediaEventType met,
            /* [in] */ REFGUID guidExtendedType,
            /* [in] */ HRESULT hrStatus,
            /* [unique][in] */ const PROPVARIANT *pvValue);
        
        HRESULT ( STDMETHODCALLTYPE *CancelAsyncOperation )( 
            IWMDRMNetReceiver * This,
            /* [in] */ IUnknown *punkCancelationCookie);
        
        HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            IWMDRMNetReceiver * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetRegistrationChallenge )( 
            IWMDRMNetReceiver * This,
            /* [size_is][size_is][out] */ BYTE **ppbRegistrationChallenge,
            /* [out] */ DWORD *pcbRegistrationChallenge);
        
        HRESULT ( STDMETHODCALLTYPE *ProcessRegistrationResponse )( 
            IWMDRMNetReceiver * This,
            /* [size_is][in] */ BYTE *pbRegistrationResponse,
            /* [in] */ DWORD cbRegistrationResponse,
            /* [out] */ IUnknown **ppunkCancellationCookie);
        
        HRESULT ( STDMETHODCALLTYPE *GetLicenseChallenge )( 
            IWMDRMNetReceiver * This,
            /* [in] */ BSTR bstrAction,
            /* [size_is][size_is][out] */ BYTE **ppbLicenseChallenge,
            /* [out] */ DWORD *pcbLicenseChallenge);
        
        HRESULT ( STDMETHODCALLTYPE *ProcessLicenseResponse )( 
            IWMDRMNetReceiver * This,
            /* [size_is][in] */ BYTE *pbLicenseResponse,
            /* [in] */ DWORD cbLicenseResponse,
            /* [size_is][size_is][out] */ BYTE **ppbWMDRMNetLicenseRepresentation,
            /* [out] */ DWORD *pcbWMDRMNetLicenseRepresentation);
        
        END_INTERFACE
    } IWMDRMNetReceiverVtbl;

    interface IWMDRMNetReceiver
    {
        CONST_VTBL struct IWMDRMNetReceiverVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMDRMNetReceiver_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMDRMNetReceiver_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMDRMNetReceiver_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMDRMNetReceiver_GetEvent(This,dwFlags,ppEvent)	\
    ( (This)->lpVtbl -> GetEvent(This,dwFlags,ppEvent) ) 

#define IWMDRMNetReceiver_BeginGetEvent(This,pCallback,punkState)	\
    ( (This)->lpVtbl -> BeginGetEvent(This,pCallback,punkState) ) 

#define IWMDRMNetReceiver_EndGetEvent(This,pResult,ppEvent)	\
    ( (This)->lpVtbl -> EndGetEvent(This,pResult,ppEvent) ) 

#define IWMDRMNetReceiver_QueueEvent(This,met,guidExtendedType,hrStatus,pvValue)	\
    ( (This)->lpVtbl -> QueueEvent(This,met,guidExtendedType,hrStatus,pvValue) ) 


#define IWMDRMNetReceiver_CancelAsyncOperation(This,punkCancelationCookie)	\
    ( (This)->lpVtbl -> CancelAsyncOperation(This,punkCancelationCookie) ) 

#define IWMDRMNetReceiver_Shutdown(This)	\
    ( (This)->lpVtbl -> Shutdown(This) ) 


#define IWMDRMNetReceiver_GetRegistrationChallenge(This,ppbRegistrationChallenge,pcbRegistrationChallenge)	\
    ( (This)->lpVtbl -> GetRegistrationChallenge(This,ppbRegistrationChallenge,pcbRegistrationChallenge) ) 

#define IWMDRMNetReceiver_ProcessRegistrationResponse(This,pbRegistrationResponse,cbRegistrationResponse,ppunkCancellationCookie)	\
    ( (This)->lpVtbl -> ProcessRegistrationResponse(This,pbRegistrationResponse,cbRegistrationResponse,ppunkCancellationCookie) ) 

#define IWMDRMNetReceiver_GetLicenseChallenge(This,bstrAction,ppbLicenseChallenge,pcbLicenseChallenge)	\
    ( (This)->lpVtbl -> GetLicenseChallenge(This,bstrAction,ppbLicenseChallenge,pcbLicenseChallenge) ) 

#define IWMDRMNetReceiver_ProcessLicenseResponse(This,pbLicenseResponse,cbLicenseResponse,ppbWMDRMNetLicenseRepresentation,pcbWMDRMNetLicenseRepresentation)	\
    ( (This)->lpVtbl -> ProcessLicenseResponse(This,pbLicenseResponse,cbLicenseResponse,ppbWMDRMNetLicenseRepresentation,pcbWMDRMNetLicenseRepresentation) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMDRMNetReceiver_INTERFACE_DEFINED__ */


#ifndef __IWMDRMNetTransmitter_INTERFACE_DEFINED__
#define __IWMDRMNetTransmitter_INTERFACE_DEFINED__

/* interface IWMDRMNetTransmitter */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMDRMNetTransmitter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F040C12B-4415-4F20-93F0-453EB519A756")
    IWMDRMNetTransmitter : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetLicenseChallenge( 
            /* [size_is][in] */ BYTE *pbLicenseChallenge,
            /* [in] */ DWORD cbLicenseChallenge) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRootLicenseResponse( 
            /* [in] */ BSTR bstrKID,
            /* [size_is][size_is][out] */ BYTE **ppbLicenseResponse,
            /* [out] */ DWORD *pcbLicenseResponse) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLeafLicenseResponse( 
            /* [in] */ BSTR bstrKID,
            /* [in] */ WMDRMNET_POLICY *pPolicy,
            /* [out] */ IWMDRMEncrypt **ppIWMDRMEncrypt,
            /* [size_is][size_is][out] */ BYTE **ppbLicenseResponse,
            /* [out] */ DWORD *pcbLicenseResponse) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMDRMNetTransmitterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMDRMNetTransmitter * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMDRMNetTransmitter * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMDRMNetTransmitter * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetLicenseChallenge )( 
            IWMDRMNetTransmitter * This,
            /* [size_is][in] */ BYTE *pbLicenseChallenge,
            /* [in] */ DWORD cbLicenseChallenge);
        
        HRESULT ( STDMETHODCALLTYPE *GetRootLicenseResponse )( 
            IWMDRMNetTransmitter * This,
            /* [in] */ BSTR bstrKID,
            /* [size_is][size_is][out] */ BYTE **ppbLicenseResponse,
            /* [out] */ DWORD *pcbLicenseResponse);
        
        HRESULT ( STDMETHODCALLTYPE *GetLeafLicenseResponse )( 
            IWMDRMNetTransmitter * This,
            /* [in] */ BSTR bstrKID,
            /* [in] */ WMDRMNET_POLICY *pPolicy,
            /* [out] */ IWMDRMEncrypt **ppIWMDRMEncrypt,
            /* [size_is][size_is][out] */ BYTE **ppbLicenseResponse,
            /* [out] */ DWORD *pcbLicenseResponse);
        
        END_INTERFACE
    } IWMDRMNetTransmitterVtbl;

    interface IWMDRMNetTransmitter
    {
        CONST_VTBL struct IWMDRMNetTransmitterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMDRMNetTransmitter_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMDRMNetTransmitter_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMDRMNetTransmitter_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMDRMNetTransmitter_SetLicenseChallenge(This,pbLicenseChallenge,cbLicenseChallenge)	\
    ( (This)->lpVtbl -> SetLicenseChallenge(This,pbLicenseChallenge,cbLicenseChallenge) ) 

#define IWMDRMNetTransmitter_GetRootLicenseResponse(This,bstrKID,ppbLicenseResponse,pcbLicenseResponse)	\
    ( (This)->lpVtbl -> GetRootLicenseResponse(This,bstrKID,ppbLicenseResponse,pcbLicenseResponse) ) 

#define IWMDRMNetTransmitter_GetLeafLicenseResponse(This,bstrKID,pPolicy,ppIWMDRMEncrypt,ppbLicenseResponse,pcbLicenseResponse)	\
    ( (This)->lpVtbl -> GetLeafLicenseResponse(This,bstrKID,pPolicy,ppIWMDRMEncrypt,ppbLicenseResponse,pcbLicenseResponse) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMDRMNetTransmitter_INTERFACE_DEFINED__ */


#ifndef __IWMDRMIndividualizationStatus_INTERFACE_DEFINED__
#define __IWMDRMIndividualizationStatus_INTERFACE_DEFINED__

/* interface IWMDRMIndividualizationStatus */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMDRMIndividualizationStatus;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7DB091BD-DB6A-4FA4-BCBF-0783E256C78C")
    IWMDRMIndividualizationStatus : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetStatus( 
            /* [out] */ WM_INDIVIDUALIZE_STATUS *pStatus) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMDRMIndividualizationStatusVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMDRMIndividualizationStatus * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMDRMIndividualizationStatus * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMDRMIndividualizationStatus * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetStatus )( 
            IWMDRMIndividualizationStatus * This,
            /* [out] */ WM_INDIVIDUALIZE_STATUS *pStatus);
        
        END_INTERFACE
    } IWMDRMIndividualizationStatusVtbl;

    interface IWMDRMIndividualizationStatus
    {
        CONST_VTBL struct IWMDRMIndividualizationStatusVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMDRMIndividualizationStatus_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMDRMIndividualizationStatus_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMDRMIndividualizationStatus_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMDRMIndividualizationStatus_GetStatus(This,pStatus)	\
    ( (This)->lpVtbl -> GetStatus(This,pStatus) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMDRMIndividualizationStatus_INTERFACE_DEFINED__ */


#ifndef __IWMDRMLicenseBackupRestoreStatus_INTERFACE_DEFINED__
#define __IWMDRMLicenseBackupRestoreStatus_INTERFACE_DEFINED__

/* interface IWMDRMLicenseBackupRestoreStatus */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMDRMLicenseBackupRestoreStatus;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F653C5A2-0EB3-4503-9D88-C200552D0D55")
    IWMDRMLicenseBackupRestoreStatus : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetStatus( 
            /* [out] */ WM_BACKUP_RESTORE_STATUS *pStatus) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMDRMLicenseBackupRestoreStatusVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMDRMLicenseBackupRestoreStatus * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMDRMLicenseBackupRestoreStatus * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMDRMLicenseBackupRestoreStatus * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetStatus )( 
            IWMDRMLicenseBackupRestoreStatus * This,
            /* [out] */ WM_BACKUP_RESTORE_STATUS *pStatus);
        
        END_INTERFACE
    } IWMDRMLicenseBackupRestoreStatusVtbl;

    interface IWMDRMLicenseBackupRestoreStatus
    {
        CONST_VTBL struct IWMDRMLicenseBackupRestoreStatusVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMDRMLicenseBackupRestoreStatus_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMDRMLicenseBackupRestoreStatus_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMDRMLicenseBackupRestoreStatus_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMDRMLicenseBackupRestoreStatus_GetStatus(This,pStatus)	\
    ( (This)->lpVtbl -> GetStatus(This,pStatus) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMDRMLicenseBackupRestoreStatus_INTERFACE_DEFINED__ */


#ifndef __IWMDRMNonSilentLicenseAquisition_INTERFACE_DEFINED__
#define __IWMDRMNonSilentLicenseAquisition_INTERFACE_DEFINED__

/* interface IWMDRMNonSilentLicenseAquisition */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMDRMNonSilentLicenseAquisition;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8AE059DD-BE6B-481A-863A-59056B50B748")
    IWMDRMNonSilentLicenseAquisition : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetURL( 
            /* [out] */ BSTR *pbstrURL) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetChallenge( 
            /* [out] */ BSTR *pbstrChallenge) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMDRMNonSilentLicenseAquisitionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMDRMNonSilentLicenseAquisition * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMDRMNonSilentLicenseAquisition * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMDRMNonSilentLicenseAquisition * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetURL )( 
            IWMDRMNonSilentLicenseAquisition * This,
            /* [out] */ BSTR *pbstrURL);
        
        HRESULT ( STDMETHODCALLTYPE *GetChallenge )( 
            IWMDRMNonSilentLicenseAquisition * This,
            /* [out] */ BSTR *pbstrChallenge);
        
        END_INTERFACE
    } IWMDRMNonSilentLicenseAquisitionVtbl;

    interface IWMDRMNonSilentLicenseAquisition
    {
        CONST_VTBL struct IWMDRMNonSilentLicenseAquisitionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMDRMNonSilentLicenseAquisition_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMDRMNonSilentLicenseAquisition_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMDRMNonSilentLicenseAquisition_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMDRMNonSilentLicenseAquisition_GetURL(This,pbstrURL)	\
    ( (This)->lpVtbl -> GetURL(This,pbstrURL) ) 

#define IWMDRMNonSilentLicenseAquisition_GetChallenge(This,pbstrChallenge)	\
    ( (This)->lpVtbl -> GetChallenge(This,pbstrChallenge) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMDRMNonSilentLicenseAquisition_INTERFACE_DEFINED__ */



#ifndef __WMDRMContentEnablerLib_LIBRARY_DEFINED__
#define __WMDRMContentEnablerLib_LIBRARY_DEFINED__

/* library WMDRMContentEnablerLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_WMDRMContentEnablerLib;

EXTERN_C const CLSID CLSID_WMDRMContentEnablerActivate;

#ifdef __cplusplus

class DECLSPEC_UUID("82435bdf-f7c1-4df9-8103-eeabebf3d6e1")
WMDRMContentEnablerActivate;
#endif
#endif /* __WMDRMContentEnablerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  BSTR_UserSize64(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal64(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal64(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree64(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


