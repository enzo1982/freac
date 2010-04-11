

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0498 */
/* Compiler settings for mfobjects.idl:
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

#ifndef __mfobjects_h__
#define __mfobjects_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMFAttributes_FWD_DEFINED__
#define __IMFAttributes_FWD_DEFINED__
typedef interface IMFAttributes IMFAttributes;
#endif 	/* __IMFAttributes_FWD_DEFINED__ */


#ifndef __IMFMediaBuffer_FWD_DEFINED__
#define __IMFMediaBuffer_FWD_DEFINED__
typedef interface IMFMediaBuffer IMFMediaBuffer;
#endif 	/* __IMFMediaBuffer_FWD_DEFINED__ */


#ifndef __IMFSample_FWD_DEFINED__
#define __IMFSample_FWD_DEFINED__
typedef interface IMFSample IMFSample;
#endif 	/* __IMFSample_FWD_DEFINED__ */


#ifndef __IMF2DBuffer_FWD_DEFINED__
#define __IMF2DBuffer_FWD_DEFINED__
typedef interface IMF2DBuffer IMF2DBuffer;
#endif 	/* __IMF2DBuffer_FWD_DEFINED__ */


#ifndef __IMFMediaType_FWD_DEFINED__
#define __IMFMediaType_FWD_DEFINED__
typedef interface IMFMediaType IMFMediaType;
#endif 	/* __IMFMediaType_FWD_DEFINED__ */


#ifndef __IMFAudioMediaType_FWD_DEFINED__
#define __IMFAudioMediaType_FWD_DEFINED__
typedef interface IMFAudioMediaType IMFAudioMediaType;
#endif 	/* __IMFAudioMediaType_FWD_DEFINED__ */


#ifndef __IMFVideoMediaType_FWD_DEFINED__
#define __IMFVideoMediaType_FWD_DEFINED__
typedef interface IMFVideoMediaType IMFVideoMediaType;
#endif 	/* __IMFVideoMediaType_FWD_DEFINED__ */


#ifndef __IMFAsyncResult_FWD_DEFINED__
#define __IMFAsyncResult_FWD_DEFINED__
typedef interface IMFAsyncResult IMFAsyncResult;
#endif 	/* __IMFAsyncResult_FWD_DEFINED__ */


#ifndef __IMFAsyncCallback_FWD_DEFINED__
#define __IMFAsyncCallback_FWD_DEFINED__
typedef interface IMFAsyncCallback IMFAsyncCallback;
#endif 	/* __IMFAsyncCallback_FWD_DEFINED__ */


#ifndef __IMFMediaEvent_FWD_DEFINED__
#define __IMFMediaEvent_FWD_DEFINED__
typedef interface IMFMediaEvent IMFMediaEvent;
#endif 	/* __IMFMediaEvent_FWD_DEFINED__ */


#ifndef __IMFMediaEventGenerator_FWD_DEFINED__
#define __IMFMediaEventGenerator_FWD_DEFINED__
typedef interface IMFMediaEventGenerator IMFMediaEventGenerator;
#endif 	/* __IMFMediaEventGenerator_FWD_DEFINED__ */


#ifndef __IMFRemoteAsyncCallback_FWD_DEFINED__
#define __IMFRemoteAsyncCallback_FWD_DEFINED__
typedef interface IMFRemoteAsyncCallback IMFRemoteAsyncCallback;
#endif 	/* __IMFRemoteAsyncCallback_FWD_DEFINED__ */


#ifndef __IMFByteStream_FWD_DEFINED__
#define __IMFByteStream_FWD_DEFINED__
typedef interface IMFByteStream IMFByteStream;
#endif 	/* __IMFByteStream_FWD_DEFINED__ */


#ifndef __IMFCollection_FWD_DEFINED__
#define __IMFCollection_FWD_DEFINED__
typedef interface IMFCollection IMFCollection;
#endif 	/* __IMFCollection_FWD_DEFINED__ */


#ifndef __IMFMediaEventQueue_FWD_DEFINED__
#define __IMFMediaEventQueue_FWD_DEFINED__
typedef interface IMFMediaEventQueue IMFMediaEventQueue;
#endif 	/* __IMFMediaEventQueue_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"
#include "propsys.h"
#include "mediaobj.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_mfobjects_0000_0000 */
/* [local] */ 

typedef ULONGLONG QWORD;

#include <mmreg.h>
#if 0

#pragma pack(push, 1)
typedef struct tWAVEFORMATEX
    {
    WORD wFormatTag;
    WORD nChannels;
    DWORD nSamplesPerSec;
    DWORD nAvgBytesPerSec;
    WORD nBlockAlign;
    WORD wBitsPerSample;
    WORD cbSize;
    BYTE pExtraBytes[ 1 ];
    } 	WAVEFORMATEX;

typedef struct tWAVEFORMATEX *PWAVEFORMATEX;

typedef struct tWAVEFORMATEX *NPWAVEFORMATEX;

typedef struct tWAVEFORMATEX *LPWAVEFORMATEX;

typedef /* [public] */ struct __MIDL___MIDL_itf_mfobjects_0000_0000_0001
    {
    WORD wFormatTag;
    WORD nChannels;
    DWORD nSamplesPerSec;
    DWORD nAvgBytesPerSec;
    WORD nBlockAlign;
    WORD wBitsPerSample;
    WORD cbSize;
    WORD wValidBitsPerSample;
    DWORD dwChannelMask;
    GUID SubFormat;
    } 	WAVEFORMATEXTENSIBLE;

typedef struct __MIDL___MIDL_itf_mfobjects_0000_0000_0001 *PWAVEFORMATEXTENSIBLE;


#pragma pack(pop)
#endif /* 0 */
typedef 
enum _MF_ATTRIBUTE_TYPE
    {	MF_ATTRIBUTE_UINT32	= VT_UI4,
	MF_ATTRIBUTE_UINT64	= VT_UI8,
	MF_ATTRIBUTE_DOUBLE	= VT_R8,
	MF_ATTRIBUTE_GUID	= VT_CLSID,
	MF_ATTRIBUTE_STRING	= VT_LPWSTR,
	MF_ATTRIBUTE_BLOB	= ( VT_VECTOR | VT_UI1 ) ,
	MF_ATTRIBUTE_IUNKNOWN	= VT_UNKNOWN
    } 	MF_ATTRIBUTE_TYPE;

typedef 
enum _MF_ATTRIBUTES_MATCH_TYPE
    {	MF_ATTRIBUTES_MATCH_OUR_ITEMS	= 0,
	MF_ATTRIBUTES_MATCH_THEIR_ITEMS	= 1,
	MF_ATTRIBUTES_MATCH_ALL_ITEMS	= 2,
	MF_ATTRIBUTES_MATCH_INTERSECTION	= 3,
	MF_ATTRIBUTES_MATCH_SMALLER	= 4
    } 	MF_ATTRIBUTES_MATCH_TYPE;



extern RPC_IF_HANDLE __MIDL_itf_mfobjects_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfobjects_0000_0000_v0_0_s_ifspec;

#ifndef __IMFAttributes_INTERFACE_DEFINED__
#define __IMFAttributes_INTERFACE_DEFINED__

/* interface IMFAttributes */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFAttributes;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2cd2d921-c447-44a7-a13c-4adabfc247e3")
    IMFAttributes : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetItem( 
            __RPC__in REFGUID guidKey,
            /* [full][out][in] */ __RPC__inout_opt PROPVARIANT *pValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetItemType( 
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out MF_ATTRIBUTE_TYPE *pType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CompareItem( 
            __RPC__in REFGUID guidKey,
            __RPC__in REFPROPVARIANT Value,
            /* [out] */ __RPC__out BOOL *pbResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Compare( 
            __RPC__in_opt IMFAttributes *pTheirs,
            MF_ATTRIBUTES_MATCH_TYPE MatchType,
            /* [out] */ __RPC__out BOOL *pbResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetUINT32( 
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *punValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetUINT64( 
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT64 *punValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDouble( 
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out double *pfValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetGUID( 
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out GUID *pguidValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStringLength( 
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *pcchLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetString( 
            __RPC__in REFGUID guidKey,
            /* [size_is][out] */ __RPC__out_ecount_full(cchBufSize) LPWSTR pwszValue,
            UINT32 cchBufSize,
            /* [full][out][in] */ __RPC__inout_opt UINT32 *pcchLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAllocatedString( 
            __RPC__in REFGUID guidKey,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(( *pcchLength + 1 ) ) LPWSTR *ppwszValue,
            /* [out] */ __RPC__out UINT32 *pcchLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBlobSize( 
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *pcbBlobSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBlob( 
            __RPC__in REFGUID guidKey,
            /* [size_is][out] */ __RPC__out_ecount_full(cbBufSize) UINT8 *pBuf,
            UINT32 cbBufSize,
            /* [full][out][in] */ __RPC__inout_opt UINT32 *pcbBlobSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAllocatedBlob( 
            __RPC__in REFGUID guidKey,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbSize) UINT8 **ppBuf,
            /* [out] */ __RPC__out UINT32 *pcbSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetUnknown( 
            __RPC__in REFGUID guidKey,
            __RPC__in REFIID riid,
            /* [iid_is][out] */ __RPC__deref_out_opt LPVOID *ppv) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetItem( 
            __RPC__in REFGUID guidKey,
            __RPC__in REFPROPVARIANT Value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteItem( 
            __RPC__in REFGUID guidKey) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteAllItems( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetUINT32( 
            __RPC__in REFGUID guidKey,
            UINT32 unValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetUINT64( 
            __RPC__in REFGUID guidKey,
            UINT64 unValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDouble( 
            __RPC__in REFGUID guidKey,
            double fValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetGUID( 
            __RPC__in REFGUID guidKey,
            __RPC__in REFGUID guidValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetString( 
            __RPC__in REFGUID guidKey,
            /* [string][in] */ __RPC__in LPCWSTR wszValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetBlob( 
            __RPC__in REFGUID guidKey,
            /* [size_is][in] */ __RPC__in_ecount_full(cbBufSize) const UINT8 *pBuf,
            UINT32 cbBufSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetUnknown( 
            __RPC__in REFGUID guidKey,
            /* [in] */ __RPC__in_opt IUnknown *pUnknown) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LockStore( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnlockStore( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCount( 
            /* [out] */ __RPC__out UINT32 *pcItems) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetItemByIndex( 
            UINT32 unIndex,
            /* [out] */ __RPC__out GUID *pguidKey,
            /* [full][out][in] */ __RPC__inout_opt PROPVARIANT *pValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CopyAllItems( 
            /* [in] */ __RPC__in_opt IMFAttributes *pDest) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFAttributesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFAttributes * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFAttributes * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFAttributes * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetItem )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            /* [full][out][in] */ __RPC__inout_opt PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemType )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out MF_ATTRIBUTE_TYPE *pType);
        
        HRESULT ( STDMETHODCALLTYPE *CompareItem )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFPROPVARIANT Value,
            /* [out] */ __RPC__out BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *Compare )( 
            IMFAttributes * This,
            __RPC__in_opt IMFAttributes *pTheirs,
            MF_ATTRIBUTES_MATCH_TYPE MatchType,
            /* [out] */ __RPC__out BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT32 )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT64 )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT64 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetDouble )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out double *pfValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetGUID )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out GUID *pguidValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetStringLength )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetString )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][out] */ __RPC__out_ecount_full(cchBufSize) LPWSTR pwszValue,
            UINT32 cchBufSize,
            /* [full][out][in] */ __RPC__inout_opt UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedString )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(( *pcchLength + 1 ) ) LPWSTR *ppwszValue,
            /* [out] */ __RPC__out UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlobSize )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlob )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][out] */ __RPC__out_ecount_full(cbBufSize) UINT8 *pBuf,
            UINT32 cbBufSize,
            /* [full][out][in] */ __RPC__inout_opt UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedBlob )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbSize) UINT8 **ppBuf,
            /* [out] */ __RPC__out UINT32 *pcbSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetUnknown )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFIID riid,
            /* [iid_is][out] */ __RPC__deref_out_opt LPVOID *ppv);
        
        HRESULT ( STDMETHODCALLTYPE *SetItem )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFPROPVARIANT Value);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteItem )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteAllItems )( 
            IMFAttributes * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT32 )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            UINT32 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT64 )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            UINT64 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetDouble )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            double fValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetGUID )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFGUID guidValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetString )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            /* [string][in] */ __RPC__in LPCWSTR wszValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetBlob )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][in] */ __RPC__in_ecount_full(cbBufSize) const UINT8 *pBuf,
            UINT32 cbBufSize);
        
        HRESULT ( STDMETHODCALLTYPE *SetUnknown )( 
            IMFAttributes * This,
            __RPC__in REFGUID guidKey,
            /* [in] */ __RPC__in_opt IUnknown *pUnknown);
        
        HRESULT ( STDMETHODCALLTYPE *LockStore )( 
            IMFAttributes * This);
        
        HRESULT ( STDMETHODCALLTYPE *UnlockStore )( 
            IMFAttributes * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IMFAttributes * This,
            /* [out] */ __RPC__out UINT32 *pcItems);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemByIndex )( 
            IMFAttributes * This,
            UINT32 unIndex,
            /* [out] */ __RPC__out GUID *pguidKey,
            /* [full][out][in] */ __RPC__inout_opt PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *CopyAllItems )( 
            IMFAttributes * This,
            /* [in] */ __RPC__in_opt IMFAttributes *pDest);
        
        END_INTERFACE
    } IMFAttributesVtbl;

    interface IMFAttributes
    {
        CONST_VTBL struct IMFAttributesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFAttributes_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFAttributes_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFAttributes_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFAttributes_GetItem(This,guidKey,pValue)	\
    ( (This)->lpVtbl -> GetItem(This,guidKey,pValue) ) 

#define IMFAttributes_GetItemType(This,guidKey,pType)	\
    ( (This)->lpVtbl -> GetItemType(This,guidKey,pType) ) 

#define IMFAttributes_CompareItem(This,guidKey,Value,pbResult)	\
    ( (This)->lpVtbl -> CompareItem(This,guidKey,Value,pbResult) ) 

#define IMFAttributes_Compare(This,pTheirs,MatchType,pbResult)	\
    ( (This)->lpVtbl -> Compare(This,pTheirs,MatchType,pbResult) ) 

#define IMFAttributes_GetUINT32(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT32(This,guidKey,punValue) ) 

#define IMFAttributes_GetUINT64(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT64(This,guidKey,punValue) ) 

#define IMFAttributes_GetDouble(This,guidKey,pfValue)	\
    ( (This)->lpVtbl -> GetDouble(This,guidKey,pfValue) ) 

#define IMFAttributes_GetGUID(This,guidKey,pguidValue)	\
    ( (This)->lpVtbl -> GetGUID(This,guidKey,pguidValue) ) 

#define IMFAttributes_GetStringLength(This,guidKey,pcchLength)	\
    ( (This)->lpVtbl -> GetStringLength(This,guidKey,pcchLength) ) 

#define IMFAttributes_GetString(This,guidKey,pwszValue,cchBufSize,pcchLength)	\
    ( (This)->lpVtbl -> GetString(This,guidKey,pwszValue,cchBufSize,pcchLength) ) 

#define IMFAttributes_GetAllocatedString(This,guidKey,ppwszValue,pcchLength)	\
    ( (This)->lpVtbl -> GetAllocatedString(This,guidKey,ppwszValue,pcchLength) ) 

#define IMFAttributes_GetBlobSize(This,guidKey,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlobSize(This,guidKey,pcbBlobSize) ) 

#define IMFAttributes_GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize) ) 

#define IMFAttributes_GetAllocatedBlob(This,guidKey,ppBuf,pcbSize)	\
    ( (This)->lpVtbl -> GetAllocatedBlob(This,guidKey,ppBuf,pcbSize) ) 

#define IMFAttributes_GetUnknown(This,guidKey,riid,ppv)	\
    ( (This)->lpVtbl -> GetUnknown(This,guidKey,riid,ppv) ) 

#define IMFAttributes_SetItem(This,guidKey,Value)	\
    ( (This)->lpVtbl -> SetItem(This,guidKey,Value) ) 

#define IMFAttributes_DeleteItem(This,guidKey)	\
    ( (This)->lpVtbl -> DeleteItem(This,guidKey) ) 

#define IMFAttributes_DeleteAllItems(This)	\
    ( (This)->lpVtbl -> DeleteAllItems(This) ) 

#define IMFAttributes_SetUINT32(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT32(This,guidKey,unValue) ) 

#define IMFAttributes_SetUINT64(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT64(This,guidKey,unValue) ) 

#define IMFAttributes_SetDouble(This,guidKey,fValue)	\
    ( (This)->lpVtbl -> SetDouble(This,guidKey,fValue) ) 

#define IMFAttributes_SetGUID(This,guidKey,guidValue)	\
    ( (This)->lpVtbl -> SetGUID(This,guidKey,guidValue) ) 

#define IMFAttributes_SetString(This,guidKey,wszValue)	\
    ( (This)->lpVtbl -> SetString(This,guidKey,wszValue) ) 

#define IMFAttributes_SetBlob(This,guidKey,pBuf,cbBufSize)	\
    ( (This)->lpVtbl -> SetBlob(This,guidKey,pBuf,cbBufSize) ) 

#define IMFAttributes_SetUnknown(This,guidKey,pUnknown)	\
    ( (This)->lpVtbl -> SetUnknown(This,guidKey,pUnknown) ) 

#define IMFAttributes_LockStore(This)	\
    ( (This)->lpVtbl -> LockStore(This) ) 

#define IMFAttributes_UnlockStore(This)	\
    ( (This)->lpVtbl -> UnlockStore(This) ) 

#define IMFAttributes_GetCount(This,pcItems)	\
    ( (This)->lpVtbl -> GetCount(This,pcItems) ) 

#define IMFAttributes_GetItemByIndex(This,unIndex,pguidKey,pValue)	\
    ( (This)->lpVtbl -> GetItemByIndex(This,unIndex,pguidKey,pValue) ) 

#define IMFAttributes_CopyAllItems(This,pDest)	\
    ( (This)->lpVtbl -> CopyAllItems(This,pDest) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFAttributes_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfobjects_0000_0001 */
/* [local] */ 


enum MF_ATTRIBUTE_SERIALIZE_OPTIONS
    {	MF_ATTRIBUTE_SERIALIZE_UNKNOWN_BYREF	= 0x1
    } ;
STDAPI MFSerializeAttributesToStream(
  IMFAttributes * pAttr, 
  DWORD dwOptions, 
  IStream * pStm);
STDAPI MFDeserializeAttributesFromStream(
  IMFAttributes * pAttr, 
  DWORD dwOptions, 
  IStream * pStm);


extern RPC_IF_HANDLE __MIDL_itf_mfobjects_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfobjects_0000_0001_v0_0_s_ifspec;

#ifndef __IMFMediaBuffer_INTERFACE_DEFINED__
#define __IMFMediaBuffer_INTERFACE_DEFINED__

/* interface IMFMediaBuffer */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFMediaBuffer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("045FA593-8799-42b8-BC8D-8968C6453507")
    IMFMediaBuffer : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Lock( 
            /* [out] */ 
            __deref_out_bcount_part(*pcbMaxLength, *pcbCurrentLength)  BYTE **ppbBuffer,
            /* [out] */ 
            __out_opt  DWORD *pcbMaxLength,
            /* [out] */ 
            __out_opt  DWORD *pcbCurrentLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Unlock( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentLength( 
            /* [out] */ 
            __out  DWORD *pcbCurrentLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCurrentLength( 
            /* [in] */ DWORD cbCurrentLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMaxLength( 
            /* [out] */ 
            __out  DWORD *pcbMaxLength) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFMediaBufferVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFMediaBuffer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFMediaBuffer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFMediaBuffer * This);
        
        HRESULT ( STDMETHODCALLTYPE *Lock )( 
            IMFMediaBuffer * This,
            /* [out] */ 
            __deref_out_bcount_part(*pcbMaxLength, *pcbCurrentLength)  BYTE **ppbBuffer,
            /* [out] */ 
            __out_opt  DWORD *pcbMaxLength,
            /* [out] */ 
            __out_opt  DWORD *pcbCurrentLength);
        
        HRESULT ( STDMETHODCALLTYPE *Unlock )( 
            IMFMediaBuffer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCurrentLength )( 
            IMFMediaBuffer * This,
            /* [out] */ 
            __out  DWORD *pcbCurrentLength);
        
        HRESULT ( STDMETHODCALLTYPE *SetCurrentLength )( 
            IMFMediaBuffer * This,
            /* [in] */ DWORD cbCurrentLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetMaxLength )( 
            IMFMediaBuffer * This,
            /* [out] */ 
            __out  DWORD *pcbMaxLength);
        
        END_INTERFACE
    } IMFMediaBufferVtbl;

    interface IMFMediaBuffer
    {
        CONST_VTBL struct IMFMediaBufferVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFMediaBuffer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFMediaBuffer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFMediaBuffer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFMediaBuffer_Lock(This,ppbBuffer,pcbMaxLength,pcbCurrentLength)	\
    ( (This)->lpVtbl -> Lock(This,ppbBuffer,pcbMaxLength,pcbCurrentLength) ) 

#define IMFMediaBuffer_Unlock(This)	\
    ( (This)->lpVtbl -> Unlock(This) ) 

#define IMFMediaBuffer_GetCurrentLength(This,pcbCurrentLength)	\
    ( (This)->lpVtbl -> GetCurrentLength(This,pcbCurrentLength) ) 

#define IMFMediaBuffer_SetCurrentLength(This,cbCurrentLength)	\
    ( (This)->lpVtbl -> SetCurrentLength(This,cbCurrentLength) ) 

#define IMFMediaBuffer_GetMaxLength(This,pcbMaxLength)	\
    ( (This)->lpVtbl -> GetMaxLength(This,pcbMaxLength) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFMediaBuffer_INTERFACE_DEFINED__ */


#ifndef __IMFSample_INTERFACE_DEFINED__
#define __IMFSample_INTERFACE_DEFINED__

/* interface IMFSample */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFSample;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("c40a00f2-b93a-4d80-ae8c-5a1c634f58e4")
    IMFSample : public IMFAttributes
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetSampleFlags( 
            /* [out] */ 
            __out  DWORD *pdwSampleFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSampleFlags( 
            /* [in] */ DWORD dwSampleFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSampleTime( 
            /* [out] */ 
            __out  LONGLONG *phnsSampleTime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSampleTime( 
            /* [in] */ LONGLONG hnsSampleTime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSampleDuration( 
            /* [out] */ 
            __out  LONGLONG *phnsSampleDuration) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSampleDuration( 
            /* [in] */ LONGLONG hnsSampleDuration) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBufferCount( 
            /* [out] */ 
            __out  DWORD *pdwBufferCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBufferByIndex( 
            /* [in] */ DWORD dwIndex,
            /* [out] */ 
            __out  IMFMediaBuffer **ppBuffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ConvertToContiguousBuffer( 
            /* [out] */ 
            __out  IMFMediaBuffer **ppBuffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddBuffer( 
            /* [in] */ IMFMediaBuffer *pBuffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveBufferByIndex( 
            /* [in] */ DWORD dwIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveAllBuffers( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTotalLength( 
            /* [out] */ 
            __out  DWORD *pcbTotalLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CopyToBuffer( 
            /* [in] */ IMFMediaBuffer *pBuffer) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFSampleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFSample * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFSample * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFSample * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetItem )( 
            IMFSample * This,
            REFGUID guidKey,
            /* [full][out][in] */ PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemType )( 
            IMFSample * This,
            REFGUID guidKey,
            /* [out] */ MF_ATTRIBUTE_TYPE *pType);
        
        HRESULT ( STDMETHODCALLTYPE *CompareItem )( 
            IMFSample * This,
            REFGUID guidKey,
            REFPROPVARIANT Value,
            /* [out] */ BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *Compare )( 
            IMFSample * This,
            IMFAttributes *pTheirs,
            MF_ATTRIBUTES_MATCH_TYPE MatchType,
            /* [out] */ BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT32 )( 
            IMFSample * This,
            REFGUID guidKey,
            /* [out] */ UINT32 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT64 )( 
            IMFSample * This,
            REFGUID guidKey,
            /* [out] */ UINT64 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetDouble )( 
            IMFSample * This,
            REFGUID guidKey,
            /* [out] */ double *pfValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetGUID )( 
            IMFSample * This,
            REFGUID guidKey,
            /* [out] */ GUID *pguidValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetStringLength )( 
            IMFSample * This,
            REFGUID guidKey,
            /* [out] */ UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetString )( 
            IMFSample * This,
            REFGUID guidKey,
            /* [size_is][out] */ LPWSTR pwszValue,
            UINT32 cchBufSize,
            /* [full][out][in] */ UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedString )( 
            IMFSample * This,
            REFGUID guidKey,
            /* [size_is][size_is][out] */ LPWSTR *ppwszValue,
            /* [out] */ UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlobSize )( 
            IMFSample * This,
            REFGUID guidKey,
            /* [out] */ UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlob )( 
            IMFSample * This,
            REFGUID guidKey,
            /* [size_is][out] */ UINT8 *pBuf,
            UINT32 cbBufSize,
            /* [full][out][in] */ UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedBlob )( 
            IMFSample * This,
            REFGUID guidKey,
            /* [size_is][size_is][out] */ UINT8 **ppBuf,
            /* [out] */ UINT32 *pcbSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetUnknown )( 
            IMFSample * This,
            REFGUID guidKey,
            REFIID riid,
            /* [iid_is][out] */ LPVOID *ppv);
        
        HRESULT ( STDMETHODCALLTYPE *SetItem )( 
            IMFSample * This,
            REFGUID guidKey,
            REFPROPVARIANT Value);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteItem )( 
            IMFSample * This,
            REFGUID guidKey);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteAllItems )( 
            IMFSample * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT32 )( 
            IMFSample * This,
            REFGUID guidKey,
            UINT32 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT64 )( 
            IMFSample * This,
            REFGUID guidKey,
            UINT64 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetDouble )( 
            IMFSample * This,
            REFGUID guidKey,
            double fValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetGUID )( 
            IMFSample * This,
            REFGUID guidKey,
            REFGUID guidValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetString )( 
            IMFSample * This,
            REFGUID guidKey,
            /* [string][in] */ LPCWSTR wszValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetBlob )( 
            IMFSample * This,
            REFGUID guidKey,
            /* [size_is][in] */ const UINT8 *pBuf,
            UINT32 cbBufSize);
        
        HRESULT ( STDMETHODCALLTYPE *SetUnknown )( 
            IMFSample * This,
            REFGUID guidKey,
            /* [in] */ IUnknown *pUnknown);
        
        HRESULT ( STDMETHODCALLTYPE *LockStore )( 
            IMFSample * This);
        
        HRESULT ( STDMETHODCALLTYPE *UnlockStore )( 
            IMFSample * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IMFSample * This,
            /* [out] */ UINT32 *pcItems);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemByIndex )( 
            IMFSample * This,
            UINT32 unIndex,
            /* [out] */ GUID *pguidKey,
            /* [full][out][in] */ PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *CopyAllItems )( 
            IMFSample * This,
            /* [in] */ IMFAttributes *pDest);
        
        HRESULT ( STDMETHODCALLTYPE *GetSampleFlags )( 
            IMFSample * This,
            /* [out] */ 
            __out  DWORD *pdwSampleFlags);
        
        HRESULT ( STDMETHODCALLTYPE *SetSampleFlags )( 
            IMFSample * This,
            /* [in] */ DWORD dwSampleFlags);
        
        HRESULT ( STDMETHODCALLTYPE *GetSampleTime )( 
            IMFSample * This,
            /* [out] */ 
            __out  LONGLONG *phnsSampleTime);
        
        HRESULT ( STDMETHODCALLTYPE *SetSampleTime )( 
            IMFSample * This,
            /* [in] */ LONGLONG hnsSampleTime);
        
        HRESULT ( STDMETHODCALLTYPE *GetSampleDuration )( 
            IMFSample * This,
            /* [out] */ 
            __out  LONGLONG *phnsSampleDuration);
        
        HRESULT ( STDMETHODCALLTYPE *SetSampleDuration )( 
            IMFSample * This,
            /* [in] */ LONGLONG hnsSampleDuration);
        
        HRESULT ( STDMETHODCALLTYPE *GetBufferCount )( 
            IMFSample * This,
            /* [out] */ 
            __out  DWORD *pdwBufferCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetBufferByIndex )( 
            IMFSample * This,
            /* [in] */ DWORD dwIndex,
            /* [out] */ 
            __out  IMFMediaBuffer **ppBuffer);
        
        HRESULT ( STDMETHODCALLTYPE *ConvertToContiguousBuffer )( 
            IMFSample * This,
            /* [out] */ 
            __out  IMFMediaBuffer **ppBuffer);
        
        HRESULT ( STDMETHODCALLTYPE *AddBuffer )( 
            IMFSample * This,
            /* [in] */ IMFMediaBuffer *pBuffer);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveBufferByIndex )( 
            IMFSample * This,
            /* [in] */ DWORD dwIndex);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveAllBuffers )( 
            IMFSample * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTotalLength )( 
            IMFSample * This,
            /* [out] */ 
            __out  DWORD *pcbTotalLength);
        
        HRESULT ( STDMETHODCALLTYPE *CopyToBuffer )( 
            IMFSample * This,
            /* [in] */ IMFMediaBuffer *pBuffer);
        
        END_INTERFACE
    } IMFSampleVtbl;

    interface IMFSample
    {
        CONST_VTBL struct IMFSampleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFSample_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFSample_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFSample_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFSample_GetItem(This,guidKey,pValue)	\
    ( (This)->lpVtbl -> GetItem(This,guidKey,pValue) ) 

#define IMFSample_GetItemType(This,guidKey,pType)	\
    ( (This)->lpVtbl -> GetItemType(This,guidKey,pType) ) 

#define IMFSample_CompareItem(This,guidKey,Value,pbResult)	\
    ( (This)->lpVtbl -> CompareItem(This,guidKey,Value,pbResult) ) 

#define IMFSample_Compare(This,pTheirs,MatchType,pbResult)	\
    ( (This)->lpVtbl -> Compare(This,pTheirs,MatchType,pbResult) ) 

#define IMFSample_GetUINT32(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT32(This,guidKey,punValue) ) 

#define IMFSample_GetUINT64(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT64(This,guidKey,punValue) ) 

#define IMFSample_GetDouble(This,guidKey,pfValue)	\
    ( (This)->lpVtbl -> GetDouble(This,guidKey,pfValue) ) 

#define IMFSample_GetGUID(This,guidKey,pguidValue)	\
    ( (This)->lpVtbl -> GetGUID(This,guidKey,pguidValue) ) 

#define IMFSample_GetStringLength(This,guidKey,pcchLength)	\
    ( (This)->lpVtbl -> GetStringLength(This,guidKey,pcchLength) ) 

#define IMFSample_GetString(This,guidKey,pwszValue,cchBufSize,pcchLength)	\
    ( (This)->lpVtbl -> GetString(This,guidKey,pwszValue,cchBufSize,pcchLength) ) 

#define IMFSample_GetAllocatedString(This,guidKey,ppwszValue,pcchLength)	\
    ( (This)->lpVtbl -> GetAllocatedString(This,guidKey,ppwszValue,pcchLength) ) 

#define IMFSample_GetBlobSize(This,guidKey,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlobSize(This,guidKey,pcbBlobSize) ) 

#define IMFSample_GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize) ) 

#define IMFSample_GetAllocatedBlob(This,guidKey,ppBuf,pcbSize)	\
    ( (This)->lpVtbl -> GetAllocatedBlob(This,guidKey,ppBuf,pcbSize) ) 

#define IMFSample_GetUnknown(This,guidKey,riid,ppv)	\
    ( (This)->lpVtbl -> GetUnknown(This,guidKey,riid,ppv) ) 

#define IMFSample_SetItem(This,guidKey,Value)	\
    ( (This)->lpVtbl -> SetItem(This,guidKey,Value) ) 

#define IMFSample_DeleteItem(This,guidKey)	\
    ( (This)->lpVtbl -> DeleteItem(This,guidKey) ) 

#define IMFSample_DeleteAllItems(This)	\
    ( (This)->lpVtbl -> DeleteAllItems(This) ) 

#define IMFSample_SetUINT32(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT32(This,guidKey,unValue) ) 

#define IMFSample_SetUINT64(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT64(This,guidKey,unValue) ) 

#define IMFSample_SetDouble(This,guidKey,fValue)	\
    ( (This)->lpVtbl -> SetDouble(This,guidKey,fValue) ) 

#define IMFSample_SetGUID(This,guidKey,guidValue)	\
    ( (This)->lpVtbl -> SetGUID(This,guidKey,guidValue) ) 

#define IMFSample_SetString(This,guidKey,wszValue)	\
    ( (This)->lpVtbl -> SetString(This,guidKey,wszValue) ) 

#define IMFSample_SetBlob(This,guidKey,pBuf,cbBufSize)	\
    ( (This)->lpVtbl -> SetBlob(This,guidKey,pBuf,cbBufSize) ) 

#define IMFSample_SetUnknown(This,guidKey,pUnknown)	\
    ( (This)->lpVtbl -> SetUnknown(This,guidKey,pUnknown) ) 

#define IMFSample_LockStore(This)	\
    ( (This)->lpVtbl -> LockStore(This) ) 

#define IMFSample_UnlockStore(This)	\
    ( (This)->lpVtbl -> UnlockStore(This) ) 

#define IMFSample_GetCount(This,pcItems)	\
    ( (This)->lpVtbl -> GetCount(This,pcItems) ) 

#define IMFSample_GetItemByIndex(This,unIndex,pguidKey,pValue)	\
    ( (This)->lpVtbl -> GetItemByIndex(This,unIndex,pguidKey,pValue) ) 

#define IMFSample_CopyAllItems(This,pDest)	\
    ( (This)->lpVtbl -> CopyAllItems(This,pDest) ) 


#define IMFSample_GetSampleFlags(This,pdwSampleFlags)	\
    ( (This)->lpVtbl -> GetSampleFlags(This,pdwSampleFlags) ) 

#define IMFSample_SetSampleFlags(This,dwSampleFlags)	\
    ( (This)->lpVtbl -> SetSampleFlags(This,dwSampleFlags) ) 

#define IMFSample_GetSampleTime(This,phnsSampleTime)	\
    ( (This)->lpVtbl -> GetSampleTime(This,phnsSampleTime) ) 

#define IMFSample_SetSampleTime(This,hnsSampleTime)	\
    ( (This)->lpVtbl -> SetSampleTime(This,hnsSampleTime) ) 

#define IMFSample_GetSampleDuration(This,phnsSampleDuration)	\
    ( (This)->lpVtbl -> GetSampleDuration(This,phnsSampleDuration) ) 

#define IMFSample_SetSampleDuration(This,hnsSampleDuration)	\
    ( (This)->lpVtbl -> SetSampleDuration(This,hnsSampleDuration) ) 

#define IMFSample_GetBufferCount(This,pdwBufferCount)	\
    ( (This)->lpVtbl -> GetBufferCount(This,pdwBufferCount) ) 

#define IMFSample_GetBufferByIndex(This,dwIndex,ppBuffer)	\
    ( (This)->lpVtbl -> GetBufferByIndex(This,dwIndex,ppBuffer) ) 

#define IMFSample_ConvertToContiguousBuffer(This,ppBuffer)	\
    ( (This)->lpVtbl -> ConvertToContiguousBuffer(This,ppBuffer) ) 

#define IMFSample_AddBuffer(This,pBuffer)	\
    ( (This)->lpVtbl -> AddBuffer(This,pBuffer) ) 

#define IMFSample_RemoveBufferByIndex(This,dwIndex)	\
    ( (This)->lpVtbl -> RemoveBufferByIndex(This,dwIndex) ) 

#define IMFSample_RemoveAllBuffers(This)	\
    ( (This)->lpVtbl -> RemoveAllBuffers(This) ) 

#define IMFSample_GetTotalLength(This,pcbTotalLength)	\
    ( (This)->lpVtbl -> GetTotalLength(This,pcbTotalLength) ) 

#define IMFSample_CopyToBuffer(This,pBuffer)	\
    ( (This)->lpVtbl -> CopyToBuffer(This,pBuffer) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFSample_INTERFACE_DEFINED__ */


#ifndef __IMF2DBuffer_INTERFACE_DEFINED__
#define __IMF2DBuffer_INTERFACE_DEFINED__

/* interface IMF2DBuffer */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMF2DBuffer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7DC9D5F9-9ED9-44ec-9BBF-0600BB589FBB")
    IMF2DBuffer : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Lock2D( 
            /* [out] */ 
            __out  BYTE **pbScanline0,
            /* [out] */ LONG *plPitch) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Unlock2D( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetScanline0AndPitch( 
            /* [out] */ 
            __out  BYTE **pbScanline0,
            /* [out] */ 
            __out  LONG *plPitch) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsContiguousFormat( 
            /* [out] */ 
            __out  BOOL *pfIsContiguous) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetContiguousLength( 
            /* [out] */ 
            __out  DWORD *pcbLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ContiguousCopyTo( 
            /* [size_is][out] */ 
            __out_bcount(cbDestBuffer)  BYTE *pbDestBuffer,
            /* [in] */ DWORD cbDestBuffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ContiguousCopyFrom( 
            /* [size_is][in] */ 
            __in_bcount(cbSrcBuffer)  const BYTE *pbSrcBuffer,
            /* [in] */ DWORD cbSrcBuffer) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMF2DBufferVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMF2DBuffer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMF2DBuffer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMF2DBuffer * This);
        
        HRESULT ( STDMETHODCALLTYPE *Lock2D )( 
            IMF2DBuffer * This,
            /* [out] */ 
            __out  BYTE **pbScanline0,
            /* [out] */ LONG *plPitch);
        
        HRESULT ( STDMETHODCALLTYPE *Unlock2D )( 
            IMF2DBuffer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetScanline0AndPitch )( 
            IMF2DBuffer * This,
            /* [out] */ 
            __out  BYTE **pbScanline0,
            /* [out] */ 
            __out  LONG *plPitch);
        
        HRESULT ( STDMETHODCALLTYPE *IsContiguousFormat )( 
            IMF2DBuffer * This,
            /* [out] */ 
            __out  BOOL *pfIsContiguous);
        
        HRESULT ( STDMETHODCALLTYPE *GetContiguousLength )( 
            IMF2DBuffer * This,
            /* [out] */ 
            __out  DWORD *pcbLength);
        
        HRESULT ( STDMETHODCALLTYPE *ContiguousCopyTo )( 
            IMF2DBuffer * This,
            /* [size_is][out] */ 
            __out_bcount(cbDestBuffer)  BYTE *pbDestBuffer,
            /* [in] */ DWORD cbDestBuffer);
        
        HRESULT ( STDMETHODCALLTYPE *ContiguousCopyFrom )( 
            IMF2DBuffer * This,
            /* [size_is][in] */ 
            __in_bcount(cbSrcBuffer)  const BYTE *pbSrcBuffer,
            /* [in] */ DWORD cbSrcBuffer);
        
        END_INTERFACE
    } IMF2DBufferVtbl;

    interface IMF2DBuffer
    {
        CONST_VTBL struct IMF2DBufferVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMF2DBuffer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMF2DBuffer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMF2DBuffer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMF2DBuffer_Lock2D(This,pbScanline0,plPitch)	\
    ( (This)->lpVtbl -> Lock2D(This,pbScanline0,plPitch) ) 

#define IMF2DBuffer_Unlock2D(This)	\
    ( (This)->lpVtbl -> Unlock2D(This) ) 

#define IMF2DBuffer_GetScanline0AndPitch(This,pbScanline0,plPitch)	\
    ( (This)->lpVtbl -> GetScanline0AndPitch(This,pbScanline0,plPitch) ) 

#define IMF2DBuffer_IsContiguousFormat(This,pfIsContiguous)	\
    ( (This)->lpVtbl -> IsContiguousFormat(This,pfIsContiguous) ) 

#define IMF2DBuffer_GetContiguousLength(This,pcbLength)	\
    ( (This)->lpVtbl -> GetContiguousLength(This,pcbLength) ) 

#define IMF2DBuffer_ContiguousCopyTo(This,pbDestBuffer,cbDestBuffer)	\
    ( (This)->lpVtbl -> ContiguousCopyTo(This,pbDestBuffer,cbDestBuffer) ) 

#define IMF2DBuffer_ContiguousCopyFrom(This,pbSrcBuffer,cbSrcBuffer)	\
    ( (This)->lpVtbl -> ContiguousCopyFrom(This,pbSrcBuffer,cbSrcBuffer) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMF2DBuffer_INTERFACE_DEFINED__ */


#ifndef __IMFMediaType_INTERFACE_DEFINED__
#define __IMFMediaType_INTERFACE_DEFINED__

/* interface IMFMediaType */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFMediaType;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("44ae0fa8-ea31-4109-8d2e-4cae4997c555")
    IMFMediaType : public IMFAttributes
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetMajorType( 
            /* [out] */ 
            __out  GUID *pguidMajorType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsCompressedFormat( 
            /* [out] */ 
            __out  BOOL *pfCompressed) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsEqual( 
            /* [in] */ IMFMediaType *pIMediaType,
            /* [out] */ 
            __out  DWORD *pdwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRepresentation( 
            /* [in] */ GUID guidRepresentation,
            /* [out] */ 
            __out  LPVOID *ppvRepresentation) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FreeRepresentation( 
            /* [in] */ GUID guidRepresentation,
            /* [in] */ LPVOID pvRepresentation) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFMediaTypeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFMediaType * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFMediaType * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFMediaType * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetItem )( 
            IMFMediaType * This,
            REFGUID guidKey,
            /* [full][out][in] */ PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemType )( 
            IMFMediaType * This,
            REFGUID guidKey,
            /* [out] */ MF_ATTRIBUTE_TYPE *pType);
        
        HRESULT ( STDMETHODCALLTYPE *CompareItem )( 
            IMFMediaType * This,
            REFGUID guidKey,
            REFPROPVARIANT Value,
            /* [out] */ BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *Compare )( 
            IMFMediaType * This,
            IMFAttributes *pTheirs,
            MF_ATTRIBUTES_MATCH_TYPE MatchType,
            /* [out] */ BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT32 )( 
            IMFMediaType * This,
            REFGUID guidKey,
            /* [out] */ UINT32 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT64 )( 
            IMFMediaType * This,
            REFGUID guidKey,
            /* [out] */ UINT64 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetDouble )( 
            IMFMediaType * This,
            REFGUID guidKey,
            /* [out] */ double *pfValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetGUID )( 
            IMFMediaType * This,
            REFGUID guidKey,
            /* [out] */ GUID *pguidValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetStringLength )( 
            IMFMediaType * This,
            REFGUID guidKey,
            /* [out] */ UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetString )( 
            IMFMediaType * This,
            REFGUID guidKey,
            /* [size_is][out] */ LPWSTR pwszValue,
            UINT32 cchBufSize,
            /* [full][out][in] */ UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedString )( 
            IMFMediaType * This,
            REFGUID guidKey,
            /* [size_is][size_is][out] */ LPWSTR *ppwszValue,
            /* [out] */ UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlobSize )( 
            IMFMediaType * This,
            REFGUID guidKey,
            /* [out] */ UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlob )( 
            IMFMediaType * This,
            REFGUID guidKey,
            /* [size_is][out] */ UINT8 *pBuf,
            UINT32 cbBufSize,
            /* [full][out][in] */ UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedBlob )( 
            IMFMediaType * This,
            REFGUID guidKey,
            /* [size_is][size_is][out] */ UINT8 **ppBuf,
            /* [out] */ UINT32 *pcbSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetUnknown )( 
            IMFMediaType * This,
            REFGUID guidKey,
            REFIID riid,
            /* [iid_is][out] */ LPVOID *ppv);
        
        HRESULT ( STDMETHODCALLTYPE *SetItem )( 
            IMFMediaType * This,
            REFGUID guidKey,
            REFPROPVARIANT Value);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteItem )( 
            IMFMediaType * This,
            REFGUID guidKey);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteAllItems )( 
            IMFMediaType * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT32 )( 
            IMFMediaType * This,
            REFGUID guidKey,
            UINT32 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT64 )( 
            IMFMediaType * This,
            REFGUID guidKey,
            UINT64 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetDouble )( 
            IMFMediaType * This,
            REFGUID guidKey,
            double fValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetGUID )( 
            IMFMediaType * This,
            REFGUID guidKey,
            REFGUID guidValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetString )( 
            IMFMediaType * This,
            REFGUID guidKey,
            /* [string][in] */ LPCWSTR wszValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetBlob )( 
            IMFMediaType * This,
            REFGUID guidKey,
            /* [size_is][in] */ const UINT8 *pBuf,
            UINT32 cbBufSize);
        
        HRESULT ( STDMETHODCALLTYPE *SetUnknown )( 
            IMFMediaType * This,
            REFGUID guidKey,
            /* [in] */ IUnknown *pUnknown);
        
        HRESULT ( STDMETHODCALLTYPE *LockStore )( 
            IMFMediaType * This);
        
        HRESULT ( STDMETHODCALLTYPE *UnlockStore )( 
            IMFMediaType * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IMFMediaType * This,
            /* [out] */ UINT32 *pcItems);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemByIndex )( 
            IMFMediaType * This,
            UINT32 unIndex,
            /* [out] */ GUID *pguidKey,
            /* [full][out][in] */ PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *CopyAllItems )( 
            IMFMediaType * This,
            /* [in] */ IMFAttributes *pDest);
        
        HRESULT ( STDMETHODCALLTYPE *GetMajorType )( 
            IMFMediaType * This,
            /* [out] */ 
            __out  GUID *pguidMajorType);
        
        HRESULT ( STDMETHODCALLTYPE *IsCompressedFormat )( 
            IMFMediaType * This,
            /* [out] */ 
            __out  BOOL *pfCompressed);
        
        HRESULT ( STDMETHODCALLTYPE *IsEqual )( 
            IMFMediaType * This,
            /* [in] */ IMFMediaType *pIMediaType,
            /* [out] */ 
            __out  DWORD *pdwFlags);
        
        HRESULT ( STDMETHODCALLTYPE *GetRepresentation )( 
            IMFMediaType * This,
            /* [in] */ GUID guidRepresentation,
            /* [out] */ 
            __out  LPVOID *ppvRepresentation);
        
        HRESULT ( STDMETHODCALLTYPE *FreeRepresentation )( 
            IMFMediaType * This,
            /* [in] */ GUID guidRepresentation,
            /* [in] */ LPVOID pvRepresentation);
        
        END_INTERFACE
    } IMFMediaTypeVtbl;

    interface IMFMediaType
    {
        CONST_VTBL struct IMFMediaTypeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFMediaType_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFMediaType_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFMediaType_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFMediaType_GetItem(This,guidKey,pValue)	\
    ( (This)->lpVtbl -> GetItem(This,guidKey,pValue) ) 

#define IMFMediaType_GetItemType(This,guidKey,pType)	\
    ( (This)->lpVtbl -> GetItemType(This,guidKey,pType) ) 

#define IMFMediaType_CompareItem(This,guidKey,Value,pbResult)	\
    ( (This)->lpVtbl -> CompareItem(This,guidKey,Value,pbResult) ) 

#define IMFMediaType_Compare(This,pTheirs,MatchType,pbResult)	\
    ( (This)->lpVtbl -> Compare(This,pTheirs,MatchType,pbResult) ) 

#define IMFMediaType_GetUINT32(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT32(This,guidKey,punValue) ) 

#define IMFMediaType_GetUINT64(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT64(This,guidKey,punValue) ) 

#define IMFMediaType_GetDouble(This,guidKey,pfValue)	\
    ( (This)->lpVtbl -> GetDouble(This,guidKey,pfValue) ) 

#define IMFMediaType_GetGUID(This,guidKey,pguidValue)	\
    ( (This)->lpVtbl -> GetGUID(This,guidKey,pguidValue) ) 

#define IMFMediaType_GetStringLength(This,guidKey,pcchLength)	\
    ( (This)->lpVtbl -> GetStringLength(This,guidKey,pcchLength) ) 

#define IMFMediaType_GetString(This,guidKey,pwszValue,cchBufSize,pcchLength)	\
    ( (This)->lpVtbl -> GetString(This,guidKey,pwszValue,cchBufSize,pcchLength) ) 

#define IMFMediaType_GetAllocatedString(This,guidKey,ppwszValue,pcchLength)	\
    ( (This)->lpVtbl -> GetAllocatedString(This,guidKey,ppwszValue,pcchLength) ) 

#define IMFMediaType_GetBlobSize(This,guidKey,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlobSize(This,guidKey,pcbBlobSize) ) 

#define IMFMediaType_GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize) ) 

#define IMFMediaType_GetAllocatedBlob(This,guidKey,ppBuf,pcbSize)	\
    ( (This)->lpVtbl -> GetAllocatedBlob(This,guidKey,ppBuf,pcbSize) ) 

#define IMFMediaType_GetUnknown(This,guidKey,riid,ppv)	\
    ( (This)->lpVtbl -> GetUnknown(This,guidKey,riid,ppv) ) 

#define IMFMediaType_SetItem(This,guidKey,Value)	\
    ( (This)->lpVtbl -> SetItem(This,guidKey,Value) ) 

#define IMFMediaType_DeleteItem(This,guidKey)	\
    ( (This)->lpVtbl -> DeleteItem(This,guidKey) ) 

#define IMFMediaType_DeleteAllItems(This)	\
    ( (This)->lpVtbl -> DeleteAllItems(This) ) 

#define IMFMediaType_SetUINT32(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT32(This,guidKey,unValue) ) 

#define IMFMediaType_SetUINT64(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT64(This,guidKey,unValue) ) 

#define IMFMediaType_SetDouble(This,guidKey,fValue)	\
    ( (This)->lpVtbl -> SetDouble(This,guidKey,fValue) ) 

#define IMFMediaType_SetGUID(This,guidKey,guidValue)	\
    ( (This)->lpVtbl -> SetGUID(This,guidKey,guidValue) ) 

#define IMFMediaType_SetString(This,guidKey,wszValue)	\
    ( (This)->lpVtbl -> SetString(This,guidKey,wszValue) ) 

#define IMFMediaType_SetBlob(This,guidKey,pBuf,cbBufSize)	\
    ( (This)->lpVtbl -> SetBlob(This,guidKey,pBuf,cbBufSize) ) 

#define IMFMediaType_SetUnknown(This,guidKey,pUnknown)	\
    ( (This)->lpVtbl -> SetUnknown(This,guidKey,pUnknown) ) 

#define IMFMediaType_LockStore(This)	\
    ( (This)->lpVtbl -> LockStore(This) ) 

#define IMFMediaType_UnlockStore(This)	\
    ( (This)->lpVtbl -> UnlockStore(This) ) 

#define IMFMediaType_GetCount(This,pcItems)	\
    ( (This)->lpVtbl -> GetCount(This,pcItems) ) 

#define IMFMediaType_GetItemByIndex(This,unIndex,pguidKey,pValue)	\
    ( (This)->lpVtbl -> GetItemByIndex(This,unIndex,pguidKey,pValue) ) 

#define IMFMediaType_CopyAllItems(This,pDest)	\
    ( (This)->lpVtbl -> CopyAllItems(This,pDest) ) 


#define IMFMediaType_GetMajorType(This,pguidMajorType)	\
    ( (This)->lpVtbl -> GetMajorType(This,pguidMajorType) ) 

#define IMFMediaType_IsCompressedFormat(This,pfCompressed)	\
    ( (This)->lpVtbl -> IsCompressedFormat(This,pfCompressed) ) 

#define IMFMediaType_IsEqual(This,pIMediaType,pdwFlags)	\
    ( (This)->lpVtbl -> IsEqual(This,pIMediaType,pdwFlags) ) 

#define IMFMediaType_GetRepresentation(This,guidRepresentation,ppvRepresentation)	\
    ( (This)->lpVtbl -> GetRepresentation(This,guidRepresentation,ppvRepresentation) ) 

#define IMFMediaType_FreeRepresentation(This,guidRepresentation,pvRepresentation)	\
    ( (This)->lpVtbl -> FreeRepresentation(This,guidRepresentation,pvRepresentation) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFMediaType_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfobjects_0000_0005 */
/* [local] */ 

#define MF_MEDIATYPE_EQUAL_MAJOR_TYPES  0x00000001
#define MF_MEDIATYPE_EQUAL_FORMAT_TYPES 0x00000002
#define MF_MEDIATYPE_EQUAL_FORMAT_DATA  0x00000004
#define MF_MEDIATYPE_EQUAL_FORMAT_USER_DATA  0x00000008



extern RPC_IF_HANDLE __MIDL_itf_mfobjects_0000_0005_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfobjects_0000_0005_v0_0_s_ifspec;

#ifndef __IMFAudioMediaType_INTERFACE_DEFINED__
#define __IMFAudioMediaType_INTERFACE_DEFINED__

/* interface IMFAudioMediaType */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFAudioMediaType;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("26a0adc3-ce26-4672-9304-69552edd3faf")
    IMFAudioMediaType : public IMFMediaType
    {
    public:
        virtual const WAVEFORMATEX *STDMETHODCALLTYPE GetAudioFormat( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFAudioMediaTypeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFAudioMediaType * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFAudioMediaType * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFAudioMediaType * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetItem )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            /* [full][out][in] */ PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemType )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            /* [out] */ MF_ATTRIBUTE_TYPE *pType);
        
        HRESULT ( STDMETHODCALLTYPE *CompareItem )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            REFPROPVARIANT Value,
            /* [out] */ BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *Compare )( 
            IMFAudioMediaType * This,
            IMFAttributes *pTheirs,
            MF_ATTRIBUTES_MATCH_TYPE MatchType,
            /* [out] */ BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT32 )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            /* [out] */ UINT32 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT64 )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            /* [out] */ UINT64 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetDouble )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            /* [out] */ double *pfValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetGUID )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            /* [out] */ GUID *pguidValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetStringLength )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            /* [out] */ UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetString )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            /* [size_is][out] */ LPWSTR pwszValue,
            UINT32 cchBufSize,
            /* [full][out][in] */ UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedString )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            /* [size_is][size_is][out] */ LPWSTR *ppwszValue,
            /* [out] */ UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlobSize )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            /* [out] */ UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlob )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            /* [size_is][out] */ UINT8 *pBuf,
            UINT32 cbBufSize,
            /* [full][out][in] */ UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedBlob )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            /* [size_is][size_is][out] */ UINT8 **ppBuf,
            /* [out] */ UINT32 *pcbSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetUnknown )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            REFIID riid,
            /* [iid_is][out] */ LPVOID *ppv);
        
        HRESULT ( STDMETHODCALLTYPE *SetItem )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            REFPROPVARIANT Value);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteItem )( 
            IMFAudioMediaType * This,
            REFGUID guidKey);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteAllItems )( 
            IMFAudioMediaType * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT32 )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            UINT32 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT64 )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            UINT64 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetDouble )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            double fValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetGUID )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            REFGUID guidValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetString )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            /* [string][in] */ LPCWSTR wszValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetBlob )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            /* [size_is][in] */ const UINT8 *pBuf,
            UINT32 cbBufSize);
        
        HRESULT ( STDMETHODCALLTYPE *SetUnknown )( 
            IMFAudioMediaType * This,
            REFGUID guidKey,
            /* [in] */ IUnknown *pUnknown);
        
        HRESULT ( STDMETHODCALLTYPE *LockStore )( 
            IMFAudioMediaType * This);
        
        HRESULT ( STDMETHODCALLTYPE *UnlockStore )( 
            IMFAudioMediaType * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IMFAudioMediaType * This,
            /* [out] */ UINT32 *pcItems);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemByIndex )( 
            IMFAudioMediaType * This,
            UINT32 unIndex,
            /* [out] */ GUID *pguidKey,
            /* [full][out][in] */ PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *CopyAllItems )( 
            IMFAudioMediaType * This,
            /* [in] */ IMFAttributes *pDest);
        
        HRESULT ( STDMETHODCALLTYPE *GetMajorType )( 
            IMFAudioMediaType * This,
            /* [out] */ 
            __out  GUID *pguidMajorType);
        
        HRESULT ( STDMETHODCALLTYPE *IsCompressedFormat )( 
            IMFAudioMediaType * This,
            /* [out] */ 
            __out  BOOL *pfCompressed);
        
        HRESULT ( STDMETHODCALLTYPE *IsEqual )( 
            IMFAudioMediaType * This,
            /* [in] */ IMFMediaType *pIMediaType,
            /* [out] */ 
            __out  DWORD *pdwFlags);
        
        HRESULT ( STDMETHODCALLTYPE *GetRepresentation )( 
            IMFAudioMediaType * This,
            /* [in] */ GUID guidRepresentation,
            /* [out] */ 
            __out  LPVOID *ppvRepresentation);
        
        HRESULT ( STDMETHODCALLTYPE *FreeRepresentation )( 
            IMFAudioMediaType * This,
            /* [in] */ GUID guidRepresentation,
            /* [in] */ LPVOID pvRepresentation);
        
        const WAVEFORMATEX *( STDMETHODCALLTYPE *GetAudioFormat )( 
            IMFAudioMediaType * This);
        
        END_INTERFACE
    } IMFAudioMediaTypeVtbl;

    interface IMFAudioMediaType
    {
        CONST_VTBL struct IMFAudioMediaTypeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFAudioMediaType_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFAudioMediaType_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFAudioMediaType_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFAudioMediaType_GetItem(This,guidKey,pValue)	\
    ( (This)->lpVtbl -> GetItem(This,guidKey,pValue) ) 

#define IMFAudioMediaType_GetItemType(This,guidKey,pType)	\
    ( (This)->lpVtbl -> GetItemType(This,guidKey,pType) ) 

#define IMFAudioMediaType_CompareItem(This,guidKey,Value,pbResult)	\
    ( (This)->lpVtbl -> CompareItem(This,guidKey,Value,pbResult) ) 

#define IMFAudioMediaType_Compare(This,pTheirs,MatchType,pbResult)	\
    ( (This)->lpVtbl -> Compare(This,pTheirs,MatchType,pbResult) ) 

#define IMFAudioMediaType_GetUINT32(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT32(This,guidKey,punValue) ) 

#define IMFAudioMediaType_GetUINT64(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT64(This,guidKey,punValue) ) 

#define IMFAudioMediaType_GetDouble(This,guidKey,pfValue)	\
    ( (This)->lpVtbl -> GetDouble(This,guidKey,pfValue) ) 

#define IMFAudioMediaType_GetGUID(This,guidKey,pguidValue)	\
    ( (This)->lpVtbl -> GetGUID(This,guidKey,pguidValue) ) 

#define IMFAudioMediaType_GetStringLength(This,guidKey,pcchLength)	\
    ( (This)->lpVtbl -> GetStringLength(This,guidKey,pcchLength) ) 

#define IMFAudioMediaType_GetString(This,guidKey,pwszValue,cchBufSize,pcchLength)	\
    ( (This)->lpVtbl -> GetString(This,guidKey,pwszValue,cchBufSize,pcchLength) ) 

#define IMFAudioMediaType_GetAllocatedString(This,guidKey,ppwszValue,pcchLength)	\
    ( (This)->lpVtbl -> GetAllocatedString(This,guidKey,ppwszValue,pcchLength) ) 

#define IMFAudioMediaType_GetBlobSize(This,guidKey,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlobSize(This,guidKey,pcbBlobSize) ) 

#define IMFAudioMediaType_GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize) ) 

#define IMFAudioMediaType_GetAllocatedBlob(This,guidKey,ppBuf,pcbSize)	\
    ( (This)->lpVtbl -> GetAllocatedBlob(This,guidKey,ppBuf,pcbSize) ) 

#define IMFAudioMediaType_GetUnknown(This,guidKey,riid,ppv)	\
    ( (This)->lpVtbl -> GetUnknown(This,guidKey,riid,ppv) ) 

#define IMFAudioMediaType_SetItem(This,guidKey,Value)	\
    ( (This)->lpVtbl -> SetItem(This,guidKey,Value) ) 

#define IMFAudioMediaType_DeleteItem(This,guidKey)	\
    ( (This)->lpVtbl -> DeleteItem(This,guidKey) ) 

#define IMFAudioMediaType_DeleteAllItems(This)	\
    ( (This)->lpVtbl -> DeleteAllItems(This) ) 

#define IMFAudioMediaType_SetUINT32(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT32(This,guidKey,unValue) ) 

#define IMFAudioMediaType_SetUINT64(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT64(This,guidKey,unValue) ) 

#define IMFAudioMediaType_SetDouble(This,guidKey,fValue)	\
    ( (This)->lpVtbl -> SetDouble(This,guidKey,fValue) ) 

#define IMFAudioMediaType_SetGUID(This,guidKey,guidValue)	\
    ( (This)->lpVtbl -> SetGUID(This,guidKey,guidValue) ) 

#define IMFAudioMediaType_SetString(This,guidKey,wszValue)	\
    ( (This)->lpVtbl -> SetString(This,guidKey,wszValue) ) 

#define IMFAudioMediaType_SetBlob(This,guidKey,pBuf,cbBufSize)	\
    ( (This)->lpVtbl -> SetBlob(This,guidKey,pBuf,cbBufSize) ) 

#define IMFAudioMediaType_SetUnknown(This,guidKey,pUnknown)	\
    ( (This)->lpVtbl -> SetUnknown(This,guidKey,pUnknown) ) 

#define IMFAudioMediaType_LockStore(This)	\
    ( (This)->lpVtbl -> LockStore(This) ) 

#define IMFAudioMediaType_UnlockStore(This)	\
    ( (This)->lpVtbl -> UnlockStore(This) ) 

#define IMFAudioMediaType_GetCount(This,pcItems)	\
    ( (This)->lpVtbl -> GetCount(This,pcItems) ) 

#define IMFAudioMediaType_GetItemByIndex(This,unIndex,pguidKey,pValue)	\
    ( (This)->lpVtbl -> GetItemByIndex(This,unIndex,pguidKey,pValue) ) 

#define IMFAudioMediaType_CopyAllItems(This,pDest)	\
    ( (This)->lpVtbl -> CopyAllItems(This,pDest) ) 


#define IMFAudioMediaType_GetMajorType(This,pguidMajorType)	\
    ( (This)->lpVtbl -> GetMajorType(This,pguidMajorType) ) 

#define IMFAudioMediaType_IsCompressedFormat(This,pfCompressed)	\
    ( (This)->lpVtbl -> IsCompressedFormat(This,pfCompressed) ) 

#define IMFAudioMediaType_IsEqual(This,pIMediaType,pdwFlags)	\
    ( (This)->lpVtbl -> IsEqual(This,pIMediaType,pdwFlags) ) 

#define IMFAudioMediaType_GetRepresentation(This,guidRepresentation,ppvRepresentation)	\
    ( (This)->lpVtbl -> GetRepresentation(This,guidRepresentation,ppvRepresentation) ) 

#define IMFAudioMediaType_FreeRepresentation(This,guidRepresentation,pvRepresentation)	\
    ( (This)->lpVtbl -> FreeRepresentation(This,guidRepresentation,pvRepresentation) ) 


#define IMFAudioMediaType_GetAudioFormat(This)	\
    ( (This)->lpVtbl -> GetAudioFormat(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFAudioMediaType_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfobjects_0000_0006 */
/* [local] */ 

#ifndef _WINGDI_
typedef DWORD RGBQUAD;

typedef /* [public][public] */ struct __MIDL___MIDL_itf_mfobjects_0000_0006_0001
    {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
    } 	BITMAPINFOHEADER;

typedef /* [public] */ struct __MIDL___MIDL_itf_mfobjects_0000_0006_0002
    {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[ 1 ];
    } 	BITMAPINFO;

#endif

#if !defined( _MFVIDEOFORMAT_ )
#define _MFVIDEOFORMAT_
typedef 
enum _MFVideoInterlaceMode
    {	MFVideoInterlace_Unknown	= 0,
	MFVideoInterlace_Progressive	= 2,
	MFVideoInterlace_FieldInterleavedUpperFirst	= 3,
	MFVideoInterlace_FieldInterleavedLowerFirst	= 4,
	MFVideoInterlace_FieldSingleUpper	= 5,
	MFVideoInterlace_FieldSingleLower	= 6,
	MFVideoInterlace_MixedInterlaceOrProgressive	= 7,
	MFVideoInterlace_Last	= ( MFVideoInterlace_MixedInterlaceOrProgressive + 1 ) ,
	MFVideoInterlace_ForceDWORD	= 0x7fffffff
    } 	MFVideoInterlaceMode;

#define MFVideoInterlace_FieldSingleUpperFirst MFVideoInterlace_FieldSingleUpper
#define MFVideoInterlace_FieldSingleLowerFirst MFVideoInterlace_FieldSingleLower
typedef 
enum _MFVideoTransferFunction
    {	MFVideoTransFunc_Unknown	= 0,
	MFVideoTransFunc_10	= 1,
	MFVideoTransFunc_18	= 2,
	MFVideoTransFunc_20	= 3,
	MFVideoTransFunc_22	= 4,
	MFVideoTransFunc_709	= 5,
	MFVideoTransFunc_240M	= 6,
	MFVideoTransFunc_sRGB	= 7,
	MFVideoTransFunc_28	= 8,
	MFVideoTransFunc_Last	= ( MFVideoTransFunc_28 + 1 ) ,
	MFVideoTransFunc_ForceDWORD	= 0x7fffffff
    } 	MFVideoTransferFunction;

typedef 
enum _MFVideoPrimaries
    {	MFVideoPrimaries_Unknown	= 0,
	MFVideoPrimaries_reserved	= 1,
	MFVideoPrimaries_BT709	= 2,
	MFVideoPrimaries_BT470_2_SysM	= 3,
	MFVideoPrimaries_BT470_2_SysBG	= 4,
	MFVideoPrimaries_SMPTE170M	= 5,
	MFVideoPrimaries_SMPTE240M	= 6,
	MFVideoPrimaries_EBU3213	= 7,
	MFVideoPrimaries_SMPTE_C	= 8,
	MFVideoPrimaries_Last	= ( MFVideoPrimaries_SMPTE_C + 1 ) ,
	MFVideoPrimaries_ForceDWORD	= 0x7fffffff
    } 	MFVideoPrimaries;

typedef 
enum _MFVideoLighting
    {	MFVideoLighting_Unknown	= 0,
	MFVideoLighting_bright	= 1,
	MFVideoLighting_office	= 2,
	MFVideoLighting_dim	= 3,
	MFVideoLighting_dark	= 4,
	MFVideoLighting_Last	= ( MFVideoLighting_dark + 1 ) ,
	MFVideoLighting_ForceDWORD	= 0x7fffffff
    } 	MFVideoLighting;

typedef 
enum _MFVideoTransferMatrix
    {	MFVideoTransferMatrix_Unknown	= 0,
	MFVideoTransferMatrix_BT709	= 1,
	MFVideoTransferMatrix_BT601	= 2,
	MFVideoTransferMatrix_SMPTE240M	= 3,
	MFVideoTransferMatrix_Last	= ( MFVideoTransferMatrix_SMPTE240M + 1 ) ,
	MFVideoTransferMatrix_ForceDWORD	= 0x7fffffff
    } 	MFVideoTransferMatrix;

typedef 
enum _MFVideoChromaSubsampling
    {	MFVideoChromaSubsampling_Unknown	= 0,
	MFVideoChromaSubsampling_ProgressiveChroma	= 0x8,
	MFVideoChromaSubsampling_Horizontally_Cosited	= 0x4,
	MFVideoChromaSubsampling_Vertically_Cosited	= 0x2,
	MFVideoChromaSubsampling_Vertically_AlignedChromaPlanes	= 0x1,
	MFVideoChromaSubsampling_MPEG2	= ( MFVideoChromaSubsampling_Horizontally_Cosited | MFVideoChromaSubsampling_Vertically_AlignedChromaPlanes ) ,
	MFVideoChromaSubsampling_MPEG1	= MFVideoChromaSubsampling_Vertically_AlignedChromaPlanes,
	MFVideoChromaSubsampling_DV_PAL	= ( MFVideoChromaSubsampling_Horizontally_Cosited | MFVideoChromaSubsampling_Vertically_Cosited ) ,
	MFVideoChromaSubsampling_Cosited	= ( ( MFVideoChromaSubsampling_Horizontally_Cosited | MFVideoChromaSubsampling_Vertically_Cosited )  | MFVideoChromaSubsampling_Vertically_AlignedChromaPlanes ) ,
	MFVideoChromaSubsampling_Last	= ( MFVideoChromaSubsampling_Cosited + 1 ) ,
	MFVideoChromaSubsampling_ForceDWORD	= 0x7fffffff
    } 	MFVideoChromaSubsampling;

typedef 
enum _MFNominalRange
    {	MFNominalRange_Unknown	= 0,
	MFNominalRange_Normal	= 1,
	MFNominalRange_Wide	= 2,
	MFNominalRange_0_255	= 1,
	MFNominalRange_16_235	= 2,
	MFNominalRange_48_208	= 3,
	MFNominalRange_Last	= ( MFNominalRange_48_208 + 1 ) ,
	MFNominalRange_ForceDWORD	= 0x7fffffff
    } 	MFNominalRange;

typedef 
enum _MFVideoFlags
    {	MFVideoFlag_PAD_TO_Mask	= ( 0x1 | 0x2 ) ,
	MFVideoFlag_PAD_TO_None	= ( 0 * 0x1 ) ,
	MFVideoFlag_PAD_TO_4x3	= ( 1 * 0x1 ) ,
	MFVideoFlag_PAD_TO_16x9	= ( 2 * 0x1 ) ,
	MFVideoFlag_SrcContentHintMask	= ( ( 0x4 | 0x8 )  | 0x10 ) ,
	MFVideoFlag_SrcContentHintNone	= ( 0 * 0x4 ) ,
	MFVideoFlag_SrcContentHint16x9	= ( 1 * 0x4 ) ,
	MFVideoFlag_SrcContentHint235_1	= ( 2 * 0x4 ) ,
	MFVideoFlag_AnalogProtected	= 0x20,
	MFVideoFlag_DigitallyProtected	= 0x40,
	MFVideoFlag_ProgressiveContent	= 0x80,
	MFVideoFlag_FieldRepeatCountMask	= ( ( 0x100 | 0x200 )  | 0x400 ) ,
	MFVideoFlag_FieldRepeatCountShift	= 8,
	MFVideoFlag_ProgressiveSeqReset	= 0x800,
	MFVideoFlag_PanScanEnabled	= 0x20000,
	MFVideoFlag_LowerFieldFirst	= 0x40000,
	MFVideoFlag_BottomUpLinearRep	= 0x80000,
	MFVideoFlags_DXVASurface	= 0x100000,
	MFVideoFlags_RenderTargetSurface	= 0x400000,
	MFVideoFlags_ForceQWORD	= 0x7fffffff
    } 	MFVideoFlags;

typedef struct _MFRatio
    {
    DWORD Numerator;
    DWORD Denominator;
    } 	MFRatio;

typedef struct _MFOffset
    {
    WORD fract;
    short value;
    } 	MFOffset;

typedef struct _MFVideoArea
    {
    MFOffset OffsetX;
    MFOffset OffsetY;
    SIZE Area;
    } 	MFVideoArea;

typedef struct _MFVideoInfo
    {
    DWORD dwWidth;
    DWORD dwHeight;
    MFRatio PixelAspectRatio;
    MFVideoChromaSubsampling SourceChromaSubsampling;
    MFVideoInterlaceMode InterlaceMode;
    MFVideoTransferFunction TransferFunction;
    MFVideoPrimaries ColorPrimaries;
    MFVideoTransferMatrix TransferMatrix;
    MFVideoLighting SourceLighting;
    MFRatio FramesPerSecond;
    MFNominalRange NominalRange;
    MFVideoArea GeometricAperture;
    MFVideoArea MinimumDisplayAperture;
    MFVideoArea PanScanAperture;
    unsigned __int64 VideoFlags;
    } 	MFVideoInfo;

typedef struct __MFAYUVSample
    {
    BYTE bCrValue;
    BYTE bCbValue;
    BYTE bYValue;
    BYTE bSampleAlpha8;
    } 	MFAYUVSample;

typedef struct _MFARGB
    {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbAlpha;
    } 	MFARGB;

typedef union _MFPaletteEntry
    {
    MFARGB ARGB;
    MFAYUVSample AYCbCr;
    } 	MFPaletteEntry;

typedef struct _MFVideoSurfaceInfo
    {
    DWORD Format;
    DWORD PaletteEntries;
    MFPaletteEntry Palette[ 1 ];
    } 	MFVideoSurfaceInfo;

typedef struct _MFVideoCompressedInfo
    {
    LONGLONG AvgBitrate;
    LONGLONG AvgBitErrorRate;
    DWORD MaxKeyFrameSpacing;
    } 	MFVideoCompressedInfo;

typedef struct _MFVIDEOFORMAT
    {
    DWORD dwSize;
    MFVideoInfo videoInfo;
    GUID guidFormat;
    MFVideoCompressedInfo compressedInfo;
    MFVideoSurfaceInfo surfaceInfo;
    } 	MFVIDEOFORMAT;

typedef 
enum _MFStandardVideoFormat
    {	MFStdVideoFormat_reserved	= 0,
	MFStdVideoFormat_NTSC	= ( MFStdVideoFormat_reserved + 1 ) ,
	MFStdVideoFormat_PAL	= ( MFStdVideoFormat_NTSC + 1 ) ,
	MFStdVideoFormat_DVD_NTSC	= ( MFStdVideoFormat_PAL + 1 ) ,
	MFStdVideoFormat_DVD_PAL	= ( MFStdVideoFormat_DVD_NTSC + 1 ) ,
	MFStdVideoFormat_DV_PAL	= ( MFStdVideoFormat_DVD_PAL + 1 ) ,
	MFStdVideoFormat_DV_NTSC	= ( MFStdVideoFormat_DV_PAL + 1 ) ,
	MFStdVideoFormat_ATSC_SD480i	= ( MFStdVideoFormat_DV_NTSC + 1 ) ,
	MFStdVideoFormat_ATSC_HD1080i	= ( MFStdVideoFormat_ATSC_SD480i + 1 ) ,
	MFStdVideoFormat_ATSC_HD720p	= ( MFStdVideoFormat_ATSC_HD1080i + 1 ) 
    } 	MFStandardVideoFormat;

#endif



extern RPC_IF_HANDLE __MIDL_itf_mfobjects_0000_0006_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfobjects_0000_0006_v0_0_s_ifspec;

#ifndef __IMFVideoMediaType_INTERFACE_DEFINED__
#define __IMFVideoMediaType_INTERFACE_DEFINED__

/* interface IMFVideoMediaType */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFVideoMediaType;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("b99f381f-a8f9-47a2-a5af-ca3a225a3890")
    IMFVideoMediaType : public IMFMediaType
    {
    public:
        virtual const MFVIDEOFORMAT *STDMETHODCALLTYPE GetVideoFormat( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetVideoRepresentation( 
            /* [in] */ GUID guidRepresentation,
            /* [out] */ 
            __out  LPVOID *ppvRepresentation,
            /* [in] */ LONG lStride) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFVideoMediaTypeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFVideoMediaType * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFVideoMediaType * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFVideoMediaType * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetItem )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            /* [full][out][in] */ PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemType )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            /* [out] */ MF_ATTRIBUTE_TYPE *pType);
        
        HRESULT ( STDMETHODCALLTYPE *CompareItem )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            REFPROPVARIANT Value,
            /* [out] */ BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *Compare )( 
            IMFVideoMediaType * This,
            IMFAttributes *pTheirs,
            MF_ATTRIBUTES_MATCH_TYPE MatchType,
            /* [out] */ BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT32 )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            /* [out] */ UINT32 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT64 )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            /* [out] */ UINT64 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetDouble )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            /* [out] */ double *pfValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetGUID )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            /* [out] */ GUID *pguidValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetStringLength )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            /* [out] */ UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetString )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            /* [size_is][out] */ LPWSTR pwszValue,
            UINT32 cchBufSize,
            /* [full][out][in] */ UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedString )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            /* [size_is][size_is][out] */ LPWSTR *ppwszValue,
            /* [out] */ UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlobSize )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            /* [out] */ UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlob )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            /* [size_is][out] */ UINT8 *pBuf,
            UINT32 cbBufSize,
            /* [full][out][in] */ UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedBlob )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            /* [size_is][size_is][out] */ UINT8 **ppBuf,
            /* [out] */ UINT32 *pcbSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetUnknown )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            REFIID riid,
            /* [iid_is][out] */ LPVOID *ppv);
        
        HRESULT ( STDMETHODCALLTYPE *SetItem )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            REFPROPVARIANT Value);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteItem )( 
            IMFVideoMediaType * This,
            REFGUID guidKey);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteAllItems )( 
            IMFVideoMediaType * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT32 )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            UINT32 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT64 )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            UINT64 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetDouble )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            double fValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetGUID )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            REFGUID guidValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetString )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            /* [string][in] */ LPCWSTR wszValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetBlob )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            /* [size_is][in] */ const UINT8 *pBuf,
            UINT32 cbBufSize);
        
        HRESULT ( STDMETHODCALLTYPE *SetUnknown )( 
            IMFVideoMediaType * This,
            REFGUID guidKey,
            /* [in] */ IUnknown *pUnknown);
        
        HRESULT ( STDMETHODCALLTYPE *LockStore )( 
            IMFVideoMediaType * This);
        
        HRESULT ( STDMETHODCALLTYPE *UnlockStore )( 
            IMFVideoMediaType * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IMFVideoMediaType * This,
            /* [out] */ UINT32 *pcItems);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemByIndex )( 
            IMFVideoMediaType * This,
            UINT32 unIndex,
            /* [out] */ GUID *pguidKey,
            /* [full][out][in] */ PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *CopyAllItems )( 
            IMFVideoMediaType * This,
            /* [in] */ IMFAttributes *pDest);
        
        HRESULT ( STDMETHODCALLTYPE *GetMajorType )( 
            IMFVideoMediaType * This,
            /* [out] */ 
            __out  GUID *pguidMajorType);
        
        HRESULT ( STDMETHODCALLTYPE *IsCompressedFormat )( 
            IMFVideoMediaType * This,
            /* [out] */ 
            __out  BOOL *pfCompressed);
        
        HRESULT ( STDMETHODCALLTYPE *IsEqual )( 
            IMFVideoMediaType * This,
            /* [in] */ IMFMediaType *pIMediaType,
            /* [out] */ 
            __out  DWORD *pdwFlags);
        
        HRESULT ( STDMETHODCALLTYPE *GetRepresentation )( 
            IMFVideoMediaType * This,
            /* [in] */ GUID guidRepresentation,
            /* [out] */ 
            __out  LPVOID *ppvRepresentation);
        
        HRESULT ( STDMETHODCALLTYPE *FreeRepresentation )( 
            IMFVideoMediaType * This,
            /* [in] */ GUID guidRepresentation,
            /* [in] */ LPVOID pvRepresentation);
        
        const MFVIDEOFORMAT *( STDMETHODCALLTYPE *GetVideoFormat )( 
            IMFVideoMediaType * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetVideoRepresentation )( 
            IMFVideoMediaType * This,
            /* [in] */ GUID guidRepresentation,
            /* [out] */ 
            __out  LPVOID *ppvRepresentation,
            /* [in] */ LONG lStride);
        
        END_INTERFACE
    } IMFVideoMediaTypeVtbl;

    interface IMFVideoMediaType
    {
        CONST_VTBL struct IMFVideoMediaTypeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFVideoMediaType_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFVideoMediaType_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFVideoMediaType_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFVideoMediaType_GetItem(This,guidKey,pValue)	\
    ( (This)->lpVtbl -> GetItem(This,guidKey,pValue) ) 

#define IMFVideoMediaType_GetItemType(This,guidKey,pType)	\
    ( (This)->lpVtbl -> GetItemType(This,guidKey,pType) ) 

#define IMFVideoMediaType_CompareItem(This,guidKey,Value,pbResult)	\
    ( (This)->lpVtbl -> CompareItem(This,guidKey,Value,pbResult) ) 

#define IMFVideoMediaType_Compare(This,pTheirs,MatchType,pbResult)	\
    ( (This)->lpVtbl -> Compare(This,pTheirs,MatchType,pbResult) ) 

#define IMFVideoMediaType_GetUINT32(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT32(This,guidKey,punValue) ) 

#define IMFVideoMediaType_GetUINT64(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT64(This,guidKey,punValue) ) 

#define IMFVideoMediaType_GetDouble(This,guidKey,pfValue)	\
    ( (This)->lpVtbl -> GetDouble(This,guidKey,pfValue) ) 

#define IMFVideoMediaType_GetGUID(This,guidKey,pguidValue)	\
    ( (This)->lpVtbl -> GetGUID(This,guidKey,pguidValue) ) 

#define IMFVideoMediaType_GetStringLength(This,guidKey,pcchLength)	\
    ( (This)->lpVtbl -> GetStringLength(This,guidKey,pcchLength) ) 

#define IMFVideoMediaType_GetString(This,guidKey,pwszValue,cchBufSize,pcchLength)	\
    ( (This)->lpVtbl -> GetString(This,guidKey,pwszValue,cchBufSize,pcchLength) ) 

#define IMFVideoMediaType_GetAllocatedString(This,guidKey,ppwszValue,pcchLength)	\
    ( (This)->lpVtbl -> GetAllocatedString(This,guidKey,ppwszValue,pcchLength) ) 

#define IMFVideoMediaType_GetBlobSize(This,guidKey,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlobSize(This,guidKey,pcbBlobSize) ) 

#define IMFVideoMediaType_GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize) ) 

#define IMFVideoMediaType_GetAllocatedBlob(This,guidKey,ppBuf,pcbSize)	\
    ( (This)->lpVtbl -> GetAllocatedBlob(This,guidKey,ppBuf,pcbSize) ) 

#define IMFVideoMediaType_GetUnknown(This,guidKey,riid,ppv)	\
    ( (This)->lpVtbl -> GetUnknown(This,guidKey,riid,ppv) ) 

#define IMFVideoMediaType_SetItem(This,guidKey,Value)	\
    ( (This)->lpVtbl -> SetItem(This,guidKey,Value) ) 

#define IMFVideoMediaType_DeleteItem(This,guidKey)	\
    ( (This)->lpVtbl -> DeleteItem(This,guidKey) ) 

#define IMFVideoMediaType_DeleteAllItems(This)	\
    ( (This)->lpVtbl -> DeleteAllItems(This) ) 

#define IMFVideoMediaType_SetUINT32(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT32(This,guidKey,unValue) ) 

#define IMFVideoMediaType_SetUINT64(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT64(This,guidKey,unValue) ) 

#define IMFVideoMediaType_SetDouble(This,guidKey,fValue)	\
    ( (This)->lpVtbl -> SetDouble(This,guidKey,fValue) ) 

#define IMFVideoMediaType_SetGUID(This,guidKey,guidValue)	\
    ( (This)->lpVtbl -> SetGUID(This,guidKey,guidValue) ) 

#define IMFVideoMediaType_SetString(This,guidKey,wszValue)	\
    ( (This)->lpVtbl -> SetString(This,guidKey,wszValue) ) 

#define IMFVideoMediaType_SetBlob(This,guidKey,pBuf,cbBufSize)	\
    ( (This)->lpVtbl -> SetBlob(This,guidKey,pBuf,cbBufSize) ) 

#define IMFVideoMediaType_SetUnknown(This,guidKey,pUnknown)	\
    ( (This)->lpVtbl -> SetUnknown(This,guidKey,pUnknown) ) 

#define IMFVideoMediaType_LockStore(This)	\
    ( (This)->lpVtbl -> LockStore(This) ) 

#define IMFVideoMediaType_UnlockStore(This)	\
    ( (This)->lpVtbl -> UnlockStore(This) ) 

#define IMFVideoMediaType_GetCount(This,pcItems)	\
    ( (This)->lpVtbl -> GetCount(This,pcItems) ) 

#define IMFVideoMediaType_GetItemByIndex(This,unIndex,pguidKey,pValue)	\
    ( (This)->lpVtbl -> GetItemByIndex(This,unIndex,pguidKey,pValue) ) 

#define IMFVideoMediaType_CopyAllItems(This,pDest)	\
    ( (This)->lpVtbl -> CopyAllItems(This,pDest) ) 


#define IMFVideoMediaType_GetMajorType(This,pguidMajorType)	\
    ( (This)->lpVtbl -> GetMajorType(This,pguidMajorType) ) 

#define IMFVideoMediaType_IsCompressedFormat(This,pfCompressed)	\
    ( (This)->lpVtbl -> IsCompressedFormat(This,pfCompressed) ) 

#define IMFVideoMediaType_IsEqual(This,pIMediaType,pdwFlags)	\
    ( (This)->lpVtbl -> IsEqual(This,pIMediaType,pdwFlags) ) 

#define IMFVideoMediaType_GetRepresentation(This,guidRepresentation,ppvRepresentation)	\
    ( (This)->lpVtbl -> GetRepresentation(This,guidRepresentation,ppvRepresentation) ) 

#define IMFVideoMediaType_FreeRepresentation(This,guidRepresentation,pvRepresentation)	\
    ( (This)->lpVtbl -> FreeRepresentation(This,guidRepresentation,pvRepresentation) ) 


#define IMFVideoMediaType_GetVideoFormat(This)	\
    ( (This)->lpVtbl -> GetVideoFormat(This) ) 

#define IMFVideoMediaType_GetVideoRepresentation(This,guidRepresentation,ppvRepresentation,lStride)	\
    ( (This)->lpVtbl -> GetVideoRepresentation(This,guidRepresentation,ppvRepresentation,lStride) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFVideoMediaType_INTERFACE_DEFINED__ */


#ifndef __IMFAsyncResult_INTERFACE_DEFINED__
#define __IMFAsyncResult_INTERFACE_DEFINED__

/* interface IMFAsyncResult */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFAsyncResult;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("ac6b7889-0740-4d51-8619-905994a55cc6")
    IMFAsyncResult : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetState( 
            /* [out] */ __RPC__deref_out_opt IUnknown **ppunkState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStatus( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetStatus( 
            /* [in] */ HRESULT hrStatus) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetObject( 
            /* [out] */ __RPC__deref_out_opt IUnknown **ppObject) = 0;
        
        virtual /* [local] */ IUnknown *STDMETHODCALLTYPE GetStateNoAddRef( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFAsyncResultVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFAsyncResult * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFAsyncResult * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFAsyncResult * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetState )( 
            IMFAsyncResult * This,
            /* [out] */ __RPC__deref_out_opt IUnknown **ppunkState);
        
        HRESULT ( STDMETHODCALLTYPE *GetStatus )( 
            IMFAsyncResult * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetStatus )( 
            IMFAsyncResult * This,
            /* [in] */ HRESULT hrStatus);
        
        HRESULT ( STDMETHODCALLTYPE *GetObject )( 
            IMFAsyncResult * This,
            /* [out] */ __RPC__deref_out_opt IUnknown **ppObject);
        
        /* [local] */ IUnknown *( STDMETHODCALLTYPE *GetStateNoAddRef )( 
            IMFAsyncResult * This);
        
        END_INTERFACE
    } IMFAsyncResultVtbl;

    interface IMFAsyncResult
    {
        CONST_VTBL struct IMFAsyncResultVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFAsyncResult_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFAsyncResult_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFAsyncResult_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFAsyncResult_GetState(This,ppunkState)	\
    ( (This)->lpVtbl -> GetState(This,ppunkState) ) 

#define IMFAsyncResult_GetStatus(This)	\
    ( (This)->lpVtbl -> GetStatus(This) ) 

#define IMFAsyncResult_SetStatus(This,hrStatus)	\
    ( (This)->lpVtbl -> SetStatus(This,hrStatus) ) 

#define IMFAsyncResult_GetObject(This,ppObject)	\
    ( (This)->lpVtbl -> GetObject(This,ppObject) ) 

#define IMFAsyncResult_GetStateNoAddRef(This)	\
    ( (This)->lpVtbl -> GetStateNoAddRef(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFAsyncResult_INTERFACE_DEFINED__ */


#ifndef __IMFAsyncCallback_INTERFACE_DEFINED__
#define __IMFAsyncCallback_INTERFACE_DEFINED__

/* interface IMFAsyncCallback */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFAsyncCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("a27003cf-2354-4f2a-8d6a-ab7cff15437e")
    IMFAsyncCallback : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetParameters( 
            /* [out] */ __RPC__out DWORD *pdwFlags,
            /* [out] */ __RPC__out DWORD *pdwQueue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ __RPC__in_opt IMFAsyncResult *pAsyncResult) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFAsyncCallbackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFAsyncCallback * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFAsyncCallback * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFAsyncCallback * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetParameters )( 
            IMFAsyncCallback * This,
            /* [out] */ __RPC__out DWORD *pdwFlags,
            /* [out] */ __RPC__out DWORD *pdwQueue);
        
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMFAsyncCallback * This,
            /* [in] */ __RPC__in_opt IMFAsyncResult *pAsyncResult);
        
        END_INTERFACE
    } IMFAsyncCallbackVtbl;

    interface IMFAsyncCallback
    {
        CONST_VTBL struct IMFAsyncCallbackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFAsyncCallback_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFAsyncCallback_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFAsyncCallback_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFAsyncCallback_GetParameters(This,pdwFlags,pdwQueue)	\
    ( (This)->lpVtbl -> GetParameters(This,pdwFlags,pdwQueue) ) 

#define IMFAsyncCallback_Invoke(This,pAsyncResult)	\
    ( (This)->lpVtbl -> Invoke(This,pAsyncResult) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFAsyncCallback_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfobjects_0000_0009 */
/* [local] */ 

#define MFASYNC_FAST_IO_PROCESSING_CALLBACK 0x00000001
#define MFASYNC_SIGNAL_CALLBACK 0x00000002
#define MFASYNC_CALLBACK_QUEUE_UNDEFINED              0x00000000
#define MFASYNC_CALLBACK_QUEUE_STANDARD               0x00000001
#define MFASYNC_CALLBACK_QUEUE_RT                     0x00000002
#define MFASYNC_CALLBACK_QUEUE_IO                     0x00000003
#define MFASYNC_CALLBACK_QUEUE_TIMER                  0x00000004
#define MFASYNC_CALLBACK_QUEUE_LONG_FUNCTION          0x00000007
#define MFASYNC_CALLBACK_QUEUE_PRIVATE_MASK           0xFFFF0000
#define MFASYNC_CALLBACK_QUEUE_ALL                    0xFFFFFFFF

enum __MIDL___MIDL_itf_mfobjects_0000_0009_0001
    {	MEUnknown	= 0,
	MEError	= ( MEUnknown + 1 ) ,
	MEExtendedType	= ( MEError + 1 ) ,
	MESessionUnknown	= 100,
	MESessionTopologySet	= ( MESessionUnknown + 1 ) ,
	MESessionTopologiesCleared	= ( MESessionTopologySet + 1 ) ,
	MESessionStarted	= ( MESessionTopologiesCleared + 1 ) ,
	MESessionPaused	= ( MESessionStarted + 1 ) ,
	MESessionStopped	= ( MESessionPaused + 1 ) ,
	MESessionClosed	= ( MESessionStopped + 1 ) ,
	MESessionEnded	= ( MESessionClosed + 1 ) ,
	MESessionRateChanged	= ( MESessionEnded + 1 ) ,
	MESessionScrubSampleComplete	= ( MESessionRateChanged + 1 ) ,
	MESessionCapabilitiesChanged	= ( MESessionScrubSampleComplete + 1 ) ,
	MESessionTopologyStatus	= ( MESessionCapabilitiesChanged + 1 ) ,
	MESessionNotifyPresentationTime	= ( MESessionTopologyStatus + 1 ) ,
	MENewPresentation	= ( MESessionNotifyPresentationTime + 1 ) ,
	MELicenseAcquisitionStart	= ( MENewPresentation + 1 ) ,
	MELicenseAcquisitionCompleted	= ( MELicenseAcquisitionStart + 1 ) ,
	MEIndividualizationStart	= ( MELicenseAcquisitionCompleted + 1 ) ,
	MEIndividualizationCompleted	= ( MEIndividualizationStart + 1 ) ,
	MEEnablerProgress	= ( MEIndividualizationCompleted + 1 ) ,
	MEEnablerCompleted	= ( MEEnablerProgress + 1 ) ,
	MEPolicyError	= ( MEEnablerCompleted + 1 ) ,
	MEPolicyReport	= ( MEPolicyError + 1 ) ,
	MEBufferingStarted	= ( MEPolicyReport + 1 ) ,
	MEBufferingStopped	= ( MEBufferingStarted + 1 ) ,
	MEConnectStart	= ( MEBufferingStopped + 1 ) ,
	MEConnectEnd	= ( MEConnectStart + 1 ) ,
	MEReconnectStart	= ( MEConnectEnd + 1 ) ,
	MEReconnectEnd	= ( MEReconnectStart + 1 ) ,
	MERendererEvent	= ( MEReconnectEnd + 1 ) ,
	MESourceUnknown	= 200,
	MESourceStarted	= ( MESourceUnknown + 1 ) ,
	MEStreamStarted	= ( MESourceStarted + 1 ) ,
	MESourceSeeked	= ( MEStreamStarted + 1 ) ,
	MEStreamSeeked	= ( MESourceSeeked + 1 ) ,
	MENewStream	= ( MEStreamSeeked + 1 ) ,
	MEUpdatedStream	= ( MENewStream + 1 ) ,
	MESourceStopped	= ( MEUpdatedStream + 1 ) ,
	MEStreamStopped	= ( MESourceStopped + 1 ) ,
	MESourcePaused	= ( MEStreamStopped + 1 ) ,
	MEStreamPaused	= ( MESourcePaused + 1 ) ,
	MEEndOfPresentation	= ( MEStreamPaused + 1 ) ,
	MEEndOfStream	= ( MEEndOfPresentation + 1 ) ,
	MEMediaSample	= ( MEEndOfStream + 1 ) ,
	MEStreamTick	= ( MEMediaSample + 1 ) ,
	MEStreamThinMode	= ( MEStreamTick + 1 ) ,
	MEStreamFormatChanged	= ( MEStreamThinMode + 1 ) ,
	MESourceRateChanged	= ( MEStreamFormatChanged + 1 ) ,
	MEEndOfPresentationSegment	= ( MESourceRateChanged + 1 ) ,
	MESourceCharacteristicsChanged	= ( MEEndOfPresentationSegment + 1 ) ,
	MESourceRateChangeRequested	= ( MESourceCharacteristicsChanged + 1 ) ,
	MESourceMetadataChanged	= ( MESourceRateChangeRequested + 1 ) ,
	MESinkUnknown	= 300,
	MEStreamSinkStarted	= ( MESinkUnknown + 1 ) ,
	MEStreamSinkStopped	= ( MEStreamSinkStarted + 1 ) ,
	MEStreamSinkPaused	= ( MEStreamSinkStopped + 1 ) ,
	MEStreamSinkRateChanged	= ( MEStreamSinkPaused + 1 ) ,
	MEStreamSinkRequestSample	= ( MEStreamSinkRateChanged + 1 ) ,
	MEStreamSinkMarker	= ( MEStreamSinkRequestSample + 1 ) ,
	MEStreamSinkPrerolled	= ( MEStreamSinkMarker + 1 ) ,
	MEStreamSinkScrubSampleComplete	= ( MEStreamSinkPrerolled + 1 ) ,
	MEStreamSinkFormatChanged	= ( MEStreamSinkScrubSampleComplete + 1 ) ,
	MEStreamSinkDeviceChanged	= ( MEStreamSinkFormatChanged + 1 ) ,
	MEQualityNotify	= ( MEStreamSinkDeviceChanged + 1 ) ,
	MESinkInvalidated	= ( MEQualityNotify + 1 ) ,
	MEAudioSessionNameChanged	= ( MESinkInvalidated + 1 ) ,
	MEAudioSessionVolumeChanged	= ( MEAudioSessionNameChanged + 1 ) ,
	MEAudioSessionDeviceRemoved	= ( MEAudioSessionVolumeChanged + 1 ) ,
	MEAudioSessionServerShutdown	= ( MEAudioSessionDeviceRemoved + 1 ) ,
	MEAudioSessionGroupingParamChanged	= ( MEAudioSessionServerShutdown + 1 ) ,
	MEAudioSessionIconChanged	= ( MEAudioSessionGroupingParamChanged + 1 ) ,
	MEAudioSessionFormatChanged	= ( MEAudioSessionIconChanged + 1 ) ,
	MEAudioSessionDisconnected	= ( MEAudioSessionFormatChanged + 1 ) ,
	MEAudioSessionExclusiveModeOverride	= ( MEAudioSessionDisconnected + 1 ) ,
	METrustUnknown	= 400,
	MEPolicyChanged	= ( METrustUnknown + 1 ) ,
	MEContentProtectionMessage	= ( MEPolicyChanged + 1 ) ,
	MEPolicySet	= ( MEContentProtectionMessage + 1 ) ,
	MEWMDRMLicenseBackupCompleted	= 500,
	MEWMDRMLicenseBackupProgress	= 501,
	MEWMDRMLicenseRestoreCompleted	= 502,
	MEWMDRMLicenseRestoreProgress	= 503,
	MEWMDRMLicenseAcquisitionCompleted	= 506,
	MEWMDRMIndividualizationCompleted	= 508,
	MEWMDRMIndividualizationProgress	= 513,
	MEWMDRMProximityCompleted	= 514,
	MEWMDRMLicenseStoreCleaned	= 515,
	MEWMDRMRevocationDownloadCompleted	= 516,
	MEReservedMax	= 10000
    } ;
typedef DWORD MediaEventType;



extern RPC_IF_HANDLE __MIDL_itf_mfobjects_0000_0009_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfobjects_0000_0009_v0_0_s_ifspec;

#ifndef __IMFMediaEvent_INTERFACE_DEFINED__
#define __IMFMediaEvent_INTERFACE_DEFINED__

/* interface IMFMediaEvent */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFMediaEvent;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("DF598932-F10C-4E39-BBA2-C308F101DAA3")
    IMFMediaEvent : public IMFAttributes
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetType( 
            /* [out] */ __RPC__out MediaEventType *pmet) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetExtendedType( 
            /* [out] */ __RPC__out GUID *pguidExtendedType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStatus( 
            /* [out] */ __RPC__out HRESULT *phrStatus) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetValue( 
            /* [out] */ __RPC__out PROPVARIANT *pvValue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFMediaEventVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFMediaEvent * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFMediaEvent * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFMediaEvent * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetItem )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            /* [full][out][in] */ __RPC__inout_opt PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemType )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out MF_ATTRIBUTE_TYPE *pType);
        
        HRESULT ( STDMETHODCALLTYPE *CompareItem )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFPROPVARIANT Value,
            /* [out] */ __RPC__out BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *Compare )( 
            IMFMediaEvent * This,
            __RPC__in_opt IMFAttributes *pTheirs,
            MF_ATTRIBUTES_MATCH_TYPE MatchType,
            /* [out] */ __RPC__out BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT32 )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT64 )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT64 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetDouble )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out double *pfValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetGUID )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out GUID *pguidValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetStringLength )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetString )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][out] */ __RPC__out_ecount_full(cchBufSize) LPWSTR pwszValue,
            UINT32 cchBufSize,
            /* [full][out][in] */ __RPC__inout_opt UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedString )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(( *pcchLength + 1 ) ) LPWSTR *ppwszValue,
            /* [out] */ __RPC__out UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlobSize )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlob )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][out] */ __RPC__out_ecount_full(cbBufSize) UINT8 *pBuf,
            UINT32 cbBufSize,
            /* [full][out][in] */ __RPC__inout_opt UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedBlob )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbSize) UINT8 **ppBuf,
            /* [out] */ __RPC__out UINT32 *pcbSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetUnknown )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFIID riid,
            /* [iid_is][out] */ __RPC__deref_out_opt LPVOID *ppv);
        
        HRESULT ( STDMETHODCALLTYPE *SetItem )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFPROPVARIANT Value);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteItem )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteAllItems )( 
            IMFMediaEvent * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT32 )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            UINT32 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT64 )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            UINT64 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetDouble )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            double fValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetGUID )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFGUID guidValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetString )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            /* [string][in] */ __RPC__in LPCWSTR wszValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetBlob )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][in] */ __RPC__in_ecount_full(cbBufSize) const UINT8 *pBuf,
            UINT32 cbBufSize);
        
        HRESULT ( STDMETHODCALLTYPE *SetUnknown )( 
            IMFMediaEvent * This,
            __RPC__in REFGUID guidKey,
            /* [in] */ __RPC__in_opt IUnknown *pUnknown);
        
        HRESULT ( STDMETHODCALLTYPE *LockStore )( 
            IMFMediaEvent * This);
        
        HRESULT ( STDMETHODCALLTYPE *UnlockStore )( 
            IMFMediaEvent * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IMFMediaEvent * This,
            /* [out] */ __RPC__out UINT32 *pcItems);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemByIndex )( 
            IMFMediaEvent * This,
            UINT32 unIndex,
            /* [out] */ __RPC__out GUID *pguidKey,
            /* [full][out][in] */ __RPC__inout_opt PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *CopyAllItems )( 
            IMFMediaEvent * This,
            /* [in] */ __RPC__in_opt IMFAttributes *pDest);
        
        HRESULT ( STDMETHODCALLTYPE *GetType )( 
            IMFMediaEvent * This,
            /* [out] */ __RPC__out MediaEventType *pmet);
        
        HRESULT ( STDMETHODCALLTYPE *GetExtendedType )( 
            IMFMediaEvent * This,
            /* [out] */ __RPC__out GUID *pguidExtendedType);
        
        HRESULT ( STDMETHODCALLTYPE *GetStatus )( 
            IMFMediaEvent * This,
            /* [out] */ __RPC__out HRESULT *phrStatus);
        
        HRESULT ( STDMETHODCALLTYPE *GetValue )( 
            IMFMediaEvent * This,
            /* [out] */ __RPC__out PROPVARIANT *pvValue);
        
        END_INTERFACE
    } IMFMediaEventVtbl;

    interface IMFMediaEvent
    {
        CONST_VTBL struct IMFMediaEventVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFMediaEvent_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFMediaEvent_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFMediaEvent_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFMediaEvent_GetItem(This,guidKey,pValue)	\
    ( (This)->lpVtbl -> GetItem(This,guidKey,pValue) ) 

#define IMFMediaEvent_GetItemType(This,guidKey,pType)	\
    ( (This)->lpVtbl -> GetItemType(This,guidKey,pType) ) 

#define IMFMediaEvent_CompareItem(This,guidKey,Value,pbResult)	\
    ( (This)->lpVtbl -> CompareItem(This,guidKey,Value,pbResult) ) 

#define IMFMediaEvent_Compare(This,pTheirs,MatchType,pbResult)	\
    ( (This)->lpVtbl -> Compare(This,pTheirs,MatchType,pbResult) ) 

#define IMFMediaEvent_GetUINT32(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT32(This,guidKey,punValue) ) 

#define IMFMediaEvent_GetUINT64(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT64(This,guidKey,punValue) ) 

#define IMFMediaEvent_GetDouble(This,guidKey,pfValue)	\
    ( (This)->lpVtbl -> GetDouble(This,guidKey,pfValue) ) 

#define IMFMediaEvent_GetGUID(This,guidKey,pguidValue)	\
    ( (This)->lpVtbl -> GetGUID(This,guidKey,pguidValue) ) 

#define IMFMediaEvent_GetStringLength(This,guidKey,pcchLength)	\
    ( (This)->lpVtbl -> GetStringLength(This,guidKey,pcchLength) ) 

#define IMFMediaEvent_GetString(This,guidKey,pwszValue,cchBufSize,pcchLength)	\
    ( (This)->lpVtbl -> GetString(This,guidKey,pwszValue,cchBufSize,pcchLength) ) 

#define IMFMediaEvent_GetAllocatedString(This,guidKey,ppwszValue,pcchLength)	\
    ( (This)->lpVtbl -> GetAllocatedString(This,guidKey,ppwszValue,pcchLength) ) 

#define IMFMediaEvent_GetBlobSize(This,guidKey,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlobSize(This,guidKey,pcbBlobSize) ) 

#define IMFMediaEvent_GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize) ) 

#define IMFMediaEvent_GetAllocatedBlob(This,guidKey,ppBuf,pcbSize)	\
    ( (This)->lpVtbl -> GetAllocatedBlob(This,guidKey,ppBuf,pcbSize) ) 

#define IMFMediaEvent_GetUnknown(This,guidKey,riid,ppv)	\
    ( (This)->lpVtbl -> GetUnknown(This,guidKey,riid,ppv) ) 

#define IMFMediaEvent_SetItem(This,guidKey,Value)	\
    ( (This)->lpVtbl -> SetItem(This,guidKey,Value) ) 

#define IMFMediaEvent_DeleteItem(This,guidKey)	\
    ( (This)->lpVtbl -> DeleteItem(This,guidKey) ) 

#define IMFMediaEvent_DeleteAllItems(This)	\
    ( (This)->lpVtbl -> DeleteAllItems(This) ) 

#define IMFMediaEvent_SetUINT32(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT32(This,guidKey,unValue) ) 

#define IMFMediaEvent_SetUINT64(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT64(This,guidKey,unValue) ) 

#define IMFMediaEvent_SetDouble(This,guidKey,fValue)	\
    ( (This)->lpVtbl -> SetDouble(This,guidKey,fValue) ) 

#define IMFMediaEvent_SetGUID(This,guidKey,guidValue)	\
    ( (This)->lpVtbl -> SetGUID(This,guidKey,guidValue) ) 

#define IMFMediaEvent_SetString(This,guidKey,wszValue)	\
    ( (This)->lpVtbl -> SetString(This,guidKey,wszValue) ) 

#define IMFMediaEvent_SetBlob(This,guidKey,pBuf,cbBufSize)	\
    ( (This)->lpVtbl -> SetBlob(This,guidKey,pBuf,cbBufSize) ) 

#define IMFMediaEvent_SetUnknown(This,guidKey,pUnknown)	\
    ( (This)->lpVtbl -> SetUnknown(This,guidKey,pUnknown) ) 

#define IMFMediaEvent_LockStore(This)	\
    ( (This)->lpVtbl -> LockStore(This) ) 

#define IMFMediaEvent_UnlockStore(This)	\
    ( (This)->lpVtbl -> UnlockStore(This) ) 

#define IMFMediaEvent_GetCount(This,pcItems)	\
    ( (This)->lpVtbl -> GetCount(This,pcItems) ) 

#define IMFMediaEvent_GetItemByIndex(This,unIndex,pguidKey,pValue)	\
    ( (This)->lpVtbl -> GetItemByIndex(This,unIndex,pguidKey,pValue) ) 

#define IMFMediaEvent_CopyAllItems(This,pDest)	\
    ( (This)->lpVtbl -> CopyAllItems(This,pDest) ) 


#define IMFMediaEvent_GetType(This,pmet)	\
    ( (This)->lpVtbl -> GetType(This,pmet) ) 

#define IMFMediaEvent_GetExtendedType(This,pguidExtendedType)	\
    ( (This)->lpVtbl -> GetExtendedType(This,pguidExtendedType) ) 

#define IMFMediaEvent_GetStatus(This,phrStatus)	\
    ( (This)->lpVtbl -> GetStatus(This,phrStatus) ) 

#define IMFMediaEvent_GetValue(This,pvValue)	\
    ( (This)->lpVtbl -> GetValue(This,pvValue) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFMediaEvent_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfobjects_0000_0010 */
/* [local] */ 

#define MF_EVENT_FLAG_NO_WAIT 0x00000001



extern RPC_IF_HANDLE __MIDL_itf_mfobjects_0000_0010_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfobjects_0000_0010_v0_0_s_ifspec;

#ifndef __IMFMediaEventGenerator_INTERFACE_DEFINED__
#define __IMFMediaEventGenerator_INTERFACE_DEFINED__

/* interface IMFMediaEventGenerator */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFMediaEventGenerator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2CD0BD52-BCD5-4B89-B62C-EADC0C031E7D")
    IMFMediaEventGenerator : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetEvent( 
            /* [in] */ DWORD dwFlags,
            /* [out] */ __RPC__deref_out_opt IMFMediaEvent **ppEvent) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE BeginGetEvent( 
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE EndGetEvent( 
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  IMFMediaEvent **ppEvent) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueueEvent( 
            /* [in] */ MediaEventType met,
            /* [in] */ __RPC__in REFGUID guidExtendedType,
            /* [in] */ HRESULT hrStatus,
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvValue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFMediaEventGeneratorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFMediaEventGenerator * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFMediaEventGenerator * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFMediaEventGenerator * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetEvent )( 
            IMFMediaEventGenerator * This,
            /* [in] */ DWORD dwFlags,
            /* [out] */ __RPC__deref_out_opt IMFMediaEvent **ppEvent);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *BeginGetEvent )( 
            IMFMediaEventGenerator * This,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *EndGetEvent )( 
            IMFMediaEventGenerator * This,
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  IMFMediaEvent **ppEvent);
        
        HRESULT ( STDMETHODCALLTYPE *QueueEvent )( 
            IMFMediaEventGenerator * This,
            /* [in] */ MediaEventType met,
            /* [in] */ __RPC__in REFGUID guidExtendedType,
            /* [in] */ HRESULT hrStatus,
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvValue);
        
        END_INTERFACE
    } IMFMediaEventGeneratorVtbl;

    interface IMFMediaEventGenerator
    {
        CONST_VTBL struct IMFMediaEventGeneratorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFMediaEventGenerator_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFMediaEventGenerator_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFMediaEventGenerator_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFMediaEventGenerator_GetEvent(This,dwFlags,ppEvent)	\
    ( (This)->lpVtbl -> GetEvent(This,dwFlags,ppEvent) ) 

#define IMFMediaEventGenerator_BeginGetEvent(This,pCallback,punkState)	\
    ( (This)->lpVtbl -> BeginGetEvent(This,pCallback,punkState) ) 

#define IMFMediaEventGenerator_EndGetEvent(This,pResult,ppEvent)	\
    ( (This)->lpVtbl -> EndGetEvent(This,pResult,ppEvent) ) 

#define IMFMediaEventGenerator_QueueEvent(This,met,guidExtendedType,hrStatus,pvValue)	\
    ( (This)->lpVtbl -> QueueEvent(This,met,guidExtendedType,hrStatus,pvValue) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFMediaEventGenerator_RemoteBeginGetEvent_Proxy( 
    IMFMediaEventGenerator * This,
    /* [in] */ __RPC__in_opt IMFRemoteAsyncCallback *pCallback);


void __RPC_STUB IMFMediaEventGenerator_RemoteBeginGetEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFMediaEventGenerator_RemoteEndGetEvent_Proxy( 
    IMFMediaEventGenerator * This,
    /* [in] */ __RPC__in_opt IUnknown *pResult,
    /* [out] */ __RPC__out DWORD *pcbEvent,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbEvent) BYTE **ppbEvent);


void __RPC_STUB IMFMediaEventGenerator_RemoteEndGetEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMFMediaEventGenerator_INTERFACE_DEFINED__ */


#ifndef __IMFRemoteAsyncCallback_INTERFACE_DEFINED__
#define __IMFRemoteAsyncCallback_INTERFACE_DEFINED__

/* interface IMFRemoteAsyncCallback */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFRemoteAsyncCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("a27003d0-2354-4f2a-8d6a-ab7cff15437e")
    IMFRemoteAsyncCallback : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ HRESULT hr,
            /* [in] */ __RPC__in_opt IUnknown *pRemoteResult) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFRemoteAsyncCallbackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFRemoteAsyncCallback * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFRemoteAsyncCallback * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFRemoteAsyncCallback * This);
        
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMFRemoteAsyncCallback * This,
            /* [in] */ HRESULT hr,
            /* [in] */ __RPC__in_opt IUnknown *pRemoteResult);
        
        END_INTERFACE
    } IMFRemoteAsyncCallbackVtbl;

    interface IMFRemoteAsyncCallback
    {
        CONST_VTBL struct IMFRemoteAsyncCallbackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFRemoteAsyncCallback_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFRemoteAsyncCallback_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFRemoteAsyncCallback_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFRemoteAsyncCallback_Invoke(This,hr,pRemoteResult)	\
    ( (This)->lpVtbl -> Invoke(This,hr,pRemoteResult) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFRemoteAsyncCallback_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfobjects_0000_0012 */
/* [local] */ 

typedef 
enum _MFBYTESTREAM_SEEK_ORIGIN
    {	msoBegin	= 0,
	msoCurrent	= ( msoBegin + 1 ) 
    } 	MFBYTESTREAM_SEEK_ORIGIN;



extern RPC_IF_HANDLE __MIDL_itf_mfobjects_0000_0012_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfobjects_0000_0012_v0_0_s_ifspec;

#ifndef __IMFByteStream_INTERFACE_DEFINED__
#define __IMFByteStream_INTERFACE_DEFINED__

/* interface IMFByteStream */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFByteStream;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("ad4c1b00-4bf7-422f-9175-756693d9130d")
    IMFByteStream : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetCapabilities( 
            /* [out] */ __RPC__out DWORD *pdwCapabilities) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLength( 
            /* [out] */ __RPC__out QWORD *pqwLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetLength( 
            /* [in] */ QWORD qwLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentPosition( 
            /* [out] */ __RPC__out QWORD *pqwPosition) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCurrentPosition( 
            /* [in] */ QWORD qwPosition) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsEndOfStream( 
            /* [out] */ __RPC__out BOOL *pfEndOfStream) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Read( 
            /* [in] */ 
            __out_bcount_part(cb, *pcbRead)  BYTE *pb,
            /* [in] */ ULONG cb,
            /* [out] */ 
            __out  ULONG *pcbRead) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE BeginRead( 
            /* [in] */ 
            __out_bcount(cb)  BYTE *pb,
            /* [in] */ ULONG cb,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE EndRead( 
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  ULONG *pcbRead) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Write( 
            /* [in] */ 
            __in_bcount(cb)  const BYTE *pb,
            /* [in] */ ULONG cb,
            /* [out] */ 
            __out __deref_out_range(<=, cb)  ULONG *pcbWritten) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE BeginWrite( 
            /* [in] */ 
            __in_bcount(cb)  const BYTE *pb,
            /* [in] */ ULONG cb,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE EndWrite( 
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  ULONG *pcbWritten) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Seek( 
            /* [in] */ MFBYTESTREAM_SEEK_ORIGIN SeekOrigin,
            /* [in] */ LONGLONG llSeekOffset,
            /* [in] */ DWORD dwSeekFlags,
            /* [out] */ 
            __out_opt  QWORD *pqwCurrentPosition) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Flush( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFByteStreamVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFByteStream * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFByteStream * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFByteStream * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCapabilities )( 
            IMFByteStream * This,
            /* [out] */ __RPC__out DWORD *pdwCapabilities);
        
        HRESULT ( STDMETHODCALLTYPE *GetLength )( 
            IMFByteStream * This,
            /* [out] */ __RPC__out QWORD *pqwLength);
        
        HRESULT ( STDMETHODCALLTYPE *SetLength )( 
            IMFByteStream * This,
            /* [in] */ QWORD qwLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetCurrentPosition )( 
            IMFByteStream * This,
            /* [out] */ __RPC__out QWORD *pqwPosition);
        
        HRESULT ( STDMETHODCALLTYPE *SetCurrentPosition )( 
            IMFByteStream * This,
            /* [in] */ QWORD qwPosition);
        
        HRESULT ( STDMETHODCALLTYPE *IsEndOfStream )( 
            IMFByteStream * This,
            /* [out] */ __RPC__out BOOL *pfEndOfStream);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Read )( 
            IMFByteStream * This,
            /* [in] */ 
            __out_bcount_part(cb, *pcbRead)  BYTE *pb,
            /* [in] */ ULONG cb,
            /* [out] */ 
            __out  ULONG *pcbRead);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *BeginRead )( 
            IMFByteStream * This,
            /* [in] */ 
            __out_bcount(cb)  BYTE *pb,
            /* [in] */ ULONG cb,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *EndRead )( 
            IMFByteStream * This,
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  ULONG *pcbRead);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Write )( 
            IMFByteStream * This,
            /* [in] */ 
            __in_bcount(cb)  const BYTE *pb,
            /* [in] */ ULONG cb,
            /* [out] */ 
            __out __deref_out_range(<=, cb)  ULONG *pcbWritten);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *BeginWrite )( 
            IMFByteStream * This,
            /* [in] */ 
            __in_bcount(cb)  const BYTE *pb,
            /* [in] */ ULONG cb,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *EndWrite )( 
            IMFByteStream * This,
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  ULONG *pcbWritten);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Seek )( 
            IMFByteStream * This,
            /* [in] */ MFBYTESTREAM_SEEK_ORIGIN SeekOrigin,
            /* [in] */ LONGLONG llSeekOffset,
            /* [in] */ DWORD dwSeekFlags,
            /* [out] */ 
            __out_opt  QWORD *pqwCurrentPosition);
        
        HRESULT ( STDMETHODCALLTYPE *Flush )( 
            IMFByteStream * This);
        
        HRESULT ( STDMETHODCALLTYPE *Close )( 
            IMFByteStream * This);
        
        END_INTERFACE
    } IMFByteStreamVtbl;

    interface IMFByteStream
    {
        CONST_VTBL struct IMFByteStreamVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFByteStream_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFByteStream_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFByteStream_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFByteStream_GetCapabilities(This,pdwCapabilities)	\
    ( (This)->lpVtbl -> GetCapabilities(This,pdwCapabilities) ) 

#define IMFByteStream_GetLength(This,pqwLength)	\
    ( (This)->lpVtbl -> GetLength(This,pqwLength) ) 

#define IMFByteStream_SetLength(This,qwLength)	\
    ( (This)->lpVtbl -> SetLength(This,qwLength) ) 

#define IMFByteStream_GetCurrentPosition(This,pqwPosition)	\
    ( (This)->lpVtbl -> GetCurrentPosition(This,pqwPosition) ) 

#define IMFByteStream_SetCurrentPosition(This,qwPosition)	\
    ( (This)->lpVtbl -> SetCurrentPosition(This,qwPosition) ) 

#define IMFByteStream_IsEndOfStream(This,pfEndOfStream)	\
    ( (This)->lpVtbl -> IsEndOfStream(This,pfEndOfStream) ) 

#define IMFByteStream_Read(This,pb,cb,pcbRead)	\
    ( (This)->lpVtbl -> Read(This,pb,cb,pcbRead) ) 

#define IMFByteStream_BeginRead(This,pb,cb,pCallback,punkState)	\
    ( (This)->lpVtbl -> BeginRead(This,pb,cb,pCallback,punkState) ) 

#define IMFByteStream_EndRead(This,pResult,pcbRead)	\
    ( (This)->lpVtbl -> EndRead(This,pResult,pcbRead) ) 

#define IMFByteStream_Write(This,pb,cb,pcbWritten)	\
    ( (This)->lpVtbl -> Write(This,pb,cb,pcbWritten) ) 

#define IMFByteStream_BeginWrite(This,pb,cb,pCallback,punkState)	\
    ( (This)->lpVtbl -> BeginWrite(This,pb,cb,pCallback,punkState) ) 

#define IMFByteStream_EndWrite(This,pResult,pcbWritten)	\
    ( (This)->lpVtbl -> EndWrite(This,pResult,pcbWritten) ) 

#define IMFByteStream_Seek(This,SeekOrigin,llSeekOffset,dwSeekFlags,pqwCurrentPosition)	\
    ( (This)->lpVtbl -> Seek(This,SeekOrigin,llSeekOffset,dwSeekFlags,pqwCurrentPosition) ) 

#define IMFByteStream_Flush(This)	\
    ( (This)->lpVtbl -> Flush(This) ) 

#define IMFByteStream_Close(This)	\
    ( (This)->lpVtbl -> Close(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFByteStream_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfobjects_0000_0013 */
/* [local] */ 

#define MFBYTESTREAM_IS_READABLE                0x00000001
#define MFBYTESTREAM_IS_WRITABLE                0x00000002
#define MFBYTESTREAM_IS_SEEKABLE                0x00000004
#define MFBYTESTREAM_IS_REMOTE                  0x00000008
#define MFBYTESTREAM_IS_DIRECTORY               0x00000080
#define MFBYTESTREAM_HAS_SLOW_SEEK              0x00000100
#define MFBYTESTREAM_IS_PARTIALLY_DOWNLOADED    0x00000200
#define MFBYTESTREAM_SEEK_FLAG_CANCEL_PENDING_IO 0x00000001
EXTERN_GUID( MF_BYTESTREAM_ORIGIN_NAME, 0xfc358288, 0x3cb6, 0x460c, 0xa4, 0x24, 0xb6, 0x68, 0x12, 0x60, 0x37, 0x5a);
EXTERN_GUID( MF_BYTESTREAM_CONTENT_TYPE, 0xfc358289, 0x3cb6, 0x460c, 0xa4, 0x24, 0xb6, 0x68, 0x12, 0x60, 0x37, 0x5a);
EXTERN_GUID( MF_BYTESTREAM_DURATION, 0xfc35828a, 0x3cb6, 0x460c, 0xa4, 0x24, 0xb6, 0x68, 0x12, 0x60, 0x37, 0x5a);
EXTERN_GUID( MF_BYTESTREAM_LAST_MODIFIED_TIME, 0xfc35828b, 0x3cb6, 0x460c, 0xa4, 0x24, 0xb6, 0x68, 0x12, 0x60, 0x37, 0x5a);
typedef /* [public] */ 
enum __MIDL___MIDL_itf_mfobjects_0000_0013_0001
    {	MF_ACCESSMODE_READ	= 1,
	MF_ACCESSMODE_WRITE	= 2,
	MF_ACCESSMODE_READWRITE	= 3
    } 	MF_FILE_ACCESSMODE;

typedef /* [public] */ 
enum __MIDL___MIDL_itf_mfobjects_0000_0013_0002
    {	MF_OPENMODE_FAIL_IF_NOT_EXIST	= 0,
	MF_OPENMODE_FAIL_IF_EXIST	= 1,
	MF_OPENMODE_RESET_IF_EXIST	= 2,
	MF_OPENMODE_APPEND_IF_EXIST	= 3,
	MF_OPENMODE_DELETE_IF_EXIST	= 4
    } 	MF_FILE_OPENMODE;

typedef /* [public] */ 
enum __MIDL___MIDL_itf_mfobjects_0000_0013_0003
    {	MF_FILEFLAGS_NONE	= 0,
	MF_FILEFLAGS_NOBUFFERING	= 0x1
    } 	MF_FILE_FLAGS;



extern RPC_IF_HANDLE __MIDL_itf_mfobjects_0000_0013_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfobjects_0000_0013_v0_0_s_ifspec;

#ifndef __IMFCollection_INTERFACE_DEFINED__
#define __IMFCollection_INTERFACE_DEFINED__

/* interface IMFCollection */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5BC8A76B-869A-46a3-9B03-FA218A66AEBE")
    IMFCollection : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetElementCount( 
            /* [out] */ __RPC__out DWORD *pcElements) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetElement( 
            /* [in] */ DWORD dwElementIndex,
            /* [out] */ __RPC__deref_out_opt IUnknown **ppUnkElement) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddElement( 
            /* [in] */ __RPC__in_opt IUnknown *pUnkElement) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveElement( 
            /* [in] */ DWORD dwElementIndex,
            /* [out] */ __RPC__deref_out_opt IUnknown **ppUnkElement) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InsertElementAt( 
            /* [in] */ DWORD dwIndex,
            /* [in] */ __RPC__in_opt IUnknown *pUnknown) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveAllElements( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFCollection * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetElementCount )( 
            IMFCollection * This,
            /* [out] */ __RPC__out DWORD *pcElements);
        
        HRESULT ( STDMETHODCALLTYPE *GetElement )( 
            IMFCollection * This,
            /* [in] */ DWORD dwElementIndex,
            /* [out] */ __RPC__deref_out_opt IUnknown **ppUnkElement);
        
        HRESULT ( STDMETHODCALLTYPE *AddElement )( 
            IMFCollection * This,
            /* [in] */ __RPC__in_opt IUnknown *pUnkElement);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveElement )( 
            IMFCollection * This,
            /* [in] */ DWORD dwElementIndex,
            /* [out] */ __RPC__deref_out_opt IUnknown **ppUnkElement);
        
        HRESULT ( STDMETHODCALLTYPE *InsertElementAt )( 
            IMFCollection * This,
            /* [in] */ DWORD dwIndex,
            /* [in] */ __RPC__in_opt IUnknown *pUnknown);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveAllElements )( 
            IMFCollection * This);
        
        END_INTERFACE
    } IMFCollectionVtbl;

    interface IMFCollection
    {
        CONST_VTBL struct IMFCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFCollection_GetElementCount(This,pcElements)	\
    ( (This)->lpVtbl -> GetElementCount(This,pcElements) ) 

#define IMFCollection_GetElement(This,dwElementIndex,ppUnkElement)	\
    ( (This)->lpVtbl -> GetElement(This,dwElementIndex,ppUnkElement) ) 

#define IMFCollection_AddElement(This,pUnkElement)	\
    ( (This)->lpVtbl -> AddElement(This,pUnkElement) ) 

#define IMFCollection_RemoveElement(This,dwElementIndex,ppUnkElement)	\
    ( (This)->lpVtbl -> RemoveElement(This,dwElementIndex,ppUnkElement) ) 

#define IMFCollection_InsertElementAt(This,dwIndex,pUnknown)	\
    ( (This)->lpVtbl -> InsertElementAt(This,dwIndex,pUnknown) ) 

#define IMFCollection_RemoveAllElements(This)	\
    ( (This)->lpVtbl -> RemoveAllElements(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFCollection_INTERFACE_DEFINED__ */


#ifndef __IMFMediaEventQueue_INTERFACE_DEFINED__
#define __IMFMediaEventQueue_INTERFACE_DEFINED__

/* interface IMFMediaEventQueue */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFMediaEventQueue;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("36f846fc-2256-48b6-b58e-e2b638316581")
    IMFMediaEventQueue : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetEvent( 
            /* [in] */ DWORD dwFlags,
            /* [out] */ 
            __out  IMFMediaEvent **ppEvent) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BeginGetEvent( 
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndGetEvent( 
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  IMFMediaEvent **ppEvent) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueueEvent( 
            /* [in] */ IMFMediaEvent *pEvent) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueueEventParamVar( 
            /* [in] */ MediaEventType met,
            /* [in] */ REFGUID guidExtendedType,
            /* [in] */ HRESULT hrStatus,
            /* [unique][in] */ const PROPVARIANT *pvValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueueEventParamUnk( 
            /* [in] */ MediaEventType met,
            /* [in] */ REFGUID guidExtendedType,
            /* [in] */ HRESULT hrStatus,
            /* [unique][in] */ IUnknown *pUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Shutdown( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFMediaEventQueueVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFMediaEventQueue * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFMediaEventQueue * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFMediaEventQueue * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetEvent )( 
            IMFMediaEventQueue * This,
            /* [in] */ DWORD dwFlags,
            /* [out] */ 
            __out  IMFMediaEvent **ppEvent);
        
        HRESULT ( STDMETHODCALLTYPE *BeginGetEvent )( 
            IMFMediaEventQueue * This,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState);
        
        HRESULT ( STDMETHODCALLTYPE *EndGetEvent )( 
            IMFMediaEventQueue * This,
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  IMFMediaEvent **ppEvent);
        
        HRESULT ( STDMETHODCALLTYPE *QueueEvent )( 
            IMFMediaEventQueue * This,
            /* [in] */ IMFMediaEvent *pEvent);
        
        HRESULT ( STDMETHODCALLTYPE *QueueEventParamVar )( 
            IMFMediaEventQueue * This,
            /* [in] */ MediaEventType met,
            /* [in] */ REFGUID guidExtendedType,
            /* [in] */ HRESULT hrStatus,
            /* [unique][in] */ const PROPVARIANT *pvValue);
        
        HRESULT ( STDMETHODCALLTYPE *QueueEventParamUnk )( 
            IMFMediaEventQueue * This,
            /* [in] */ MediaEventType met,
            /* [in] */ REFGUID guidExtendedType,
            /* [in] */ HRESULT hrStatus,
            /* [unique][in] */ IUnknown *pUnk);
        
        HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            IMFMediaEventQueue * This);
        
        END_INTERFACE
    } IMFMediaEventQueueVtbl;

    interface IMFMediaEventQueue
    {
        CONST_VTBL struct IMFMediaEventQueueVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFMediaEventQueue_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFMediaEventQueue_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFMediaEventQueue_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFMediaEventQueue_GetEvent(This,dwFlags,ppEvent)	\
    ( (This)->lpVtbl -> GetEvent(This,dwFlags,ppEvent) ) 

#define IMFMediaEventQueue_BeginGetEvent(This,pCallback,punkState)	\
    ( (This)->lpVtbl -> BeginGetEvent(This,pCallback,punkState) ) 

#define IMFMediaEventQueue_EndGetEvent(This,pResult,ppEvent)	\
    ( (This)->lpVtbl -> EndGetEvent(This,pResult,ppEvent) ) 

#define IMFMediaEventQueue_QueueEvent(This,pEvent)	\
    ( (This)->lpVtbl -> QueueEvent(This,pEvent) ) 

#define IMFMediaEventQueue_QueueEventParamVar(This,met,guidExtendedType,hrStatus,pvValue)	\
    ( (This)->lpVtbl -> QueueEventParamVar(This,met,guidExtendedType,hrStatus,pvValue) ) 

#define IMFMediaEventQueue_QueueEventParamUnk(This,met,guidExtendedType,hrStatus,pUnk)	\
    ( (This)->lpVtbl -> QueueEventParamUnk(This,met,guidExtendedType,hrStatus,pUnk) ) 

#define IMFMediaEventQueue_Shutdown(This)	\
    ( (This)->lpVtbl -> Shutdown(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFMediaEventQueue_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long *, unsigned long            , LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long *, unsigned char *, LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long *, unsigned char *, LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long *, LPSAFEARRAY * ); 

unsigned long             __RPC_USER  BSTR_UserSize64(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal64(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal64(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree64(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize64(     unsigned long *, unsigned long            , LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal64(  unsigned long *, unsigned char *, LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal64(unsigned long *, unsigned char *, LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree64(     unsigned long *, LPSAFEARRAY * ); 

/* [local] */ HRESULT STDMETHODCALLTYPE IMFMediaEventGenerator_BeginGetEvent_Proxy( 
    IMFMediaEventGenerator * This,
    /* [in] */ IMFAsyncCallback *pCallback,
    /* [in] */ IUnknown *punkState);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFMediaEventGenerator_BeginGetEvent_Stub( 
    IMFMediaEventGenerator * This,
    /* [in] */ __RPC__in_opt IMFRemoteAsyncCallback *pCallback);

/* [local] */ HRESULT STDMETHODCALLTYPE IMFMediaEventGenerator_EndGetEvent_Proxy( 
    IMFMediaEventGenerator * This,
    /* [in] */ IMFAsyncResult *pResult,
    /* [out] */ 
    __out  IMFMediaEvent **ppEvent);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFMediaEventGenerator_EndGetEvent_Stub( 
    IMFMediaEventGenerator * This,
    /* [in] */ __RPC__in_opt IUnknown *pResult,
    /* [out] */ __RPC__out DWORD *pcbEvent,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbEvent) BYTE **ppbEvent);



/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


