

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0416 */
/* Compiler settings for wmsbuffer.idl:
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

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __wmsbuffer_h__
#define __wmsbuffer_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __INSSBuffer_FWD_DEFINED__
#define __INSSBuffer_FWD_DEFINED__
typedef interface INSSBuffer INSSBuffer;
#endif 	/* __INSSBuffer_FWD_DEFINED__ */


#ifndef __INSSBuffer2_FWD_DEFINED__
#define __INSSBuffer2_FWD_DEFINED__
typedef interface INSSBuffer2 INSSBuffer2;
#endif 	/* __INSSBuffer2_FWD_DEFINED__ */


#ifndef __INSSBuffer3_FWD_DEFINED__
#define __INSSBuffer3_FWD_DEFINED__
typedef interface INSSBuffer3 INSSBuffer3;
#endif 	/* __INSSBuffer3_FWD_DEFINED__ */


#ifndef __INSSBuffer4_FWD_DEFINED__
#define __INSSBuffer4_FWD_DEFINED__
typedef interface INSSBuffer4 INSSBuffer4;
#endif 	/* __INSSBuffer4_FWD_DEFINED__ */


#ifndef __IWMSBufferAllocator_FWD_DEFINED__
#define __IWMSBufferAllocator_FWD_DEFINED__
typedef interface IWMSBufferAllocator IWMSBufferAllocator;
#endif 	/* __IWMSBufferAllocator_FWD_DEFINED__ */


/* header files for imported files */
#include "objidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_wmsbuffer_0000 */
/* [local] */ 

//=========================================================================
//
// Microsoft Windows Media Technologies
// Copyright (C) Microsoft Corporation.  All Rights Reserved.
//
//=========================================================================
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
EXTERN_GUID( IID_INSSBuffer, 0xE1CD3524,0x03D7,0x11d2,0x9E,0xED,0x00,0x60,0x97,0xD2,0xD7,0xCF );
EXTERN_GUID( IID_IWMSBuffer, 0xE1CD3524,0x03D7,0x11d2,0x9E,0xED,0x00,0x60,0x97,0xD2,0xD7,0xCF );
EXTERN_GUID( IID_INSSBuffer2,0x4f528693,0x1035,0x43fe,0xb4,0x28,0x75,0x75,0x61,0xad,0x3a,0x68 );
EXTERN_GUID( IID_INSSBuffer3,0xc87ceaaf,0x75be,0x4bc4,0x84,0xeb,0xac,0x27,0x98,0x50,0x76,0x72 );
EXTERN_GUID( IID_INSSBuffer4,0xb6b8fd5a,0x32e2,0x49d4,0xa9,0x10,0xc2,0x6c,0xc8,0x54,0x65,0xed );
EXTERN_GUID( IID_IWMSBufferAllocator, 0x61103CA4,0x2033,0x11d2,0x9E,0xF1,0x00,0x60,0x97,0xD2,0xD7,0xCF );
#define IWMSBuffer INSSBuffer



extern RPC_IF_HANDLE __MIDL_itf_wmsbuffer_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wmsbuffer_0000_v0_0_s_ifspec;

#ifndef __INSSBuffer_INTERFACE_DEFINED__
#define __INSSBuffer_INTERFACE_DEFINED__

/* interface INSSBuffer */
/* [version][uuid][unique][object][local] */ 


EXTERN_C const IID IID_INSSBuffer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E1CD3524-03D7-11d2-9EED-006097D2D7CF")
    INSSBuffer : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetLength( 
            /* [out] */ DWORD *pdwLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetLength( 
            /* [in] */ DWORD dwLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMaxLength( 
            /* [out] */ DWORD *pdwLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBuffer( 
            /* [out] */ BYTE **ppdwBuffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBufferAndLength( 
            /* [out] */ BYTE **ppdwBuffer,
            /* [out] */ DWORD *pdwLength) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct INSSBufferVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INSSBuffer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INSSBuffer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INSSBuffer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetLength )( 
            INSSBuffer * This,
            /* [out] */ DWORD *pdwLength);
        
        HRESULT ( STDMETHODCALLTYPE *SetLength )( 
            INSSBuffer * This,
            /* [in] */ DWORD dwLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetMaxLength )( 
            INSSBuffer * This,
            /* [out] */ DWORD *pdwLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetBuffer )( 
            INSSBuffer * This,
            /* [out] */ BYTE **ppdwBuffer);
        
        HRESULT ( STDMETHODCALLTYPE *GetBufferAndLength )( 
            INSSBuffer * This,
            /* [out] */ BYTE **ppdwBuffer,
            /* [out] */ DWORD *pdwLength);
        
        END_INTERFACE
    } INSSBufferVtbl;

    interface INSSBuffer
    {
        CONST_VTBL struct INSSBufferVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INSSBuffer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define INSSBuffer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define INSSBuffer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define INSSBuffer_GetLength(This,pdwLength)	\
    ( (This)->lpVtbl -> GetLength(This,pdwLength) ) 

#define INSSBuffer_SetLength(This,dwLength)	\
    ( (This)->lpVtbl -> SetLength(This,dwLength) ) 

#define INSSBuffer_GetMaxLength(This,pdwLength)	\
    ( (This)->lpVtbl -> GetMaxLength(This,pdwLength) ) 

#define INSSBuffer_GetBuffer(This,ppdwBuffer)	\
    ( (This)->lpVtbl -> GetBuffer(This,ppdwBuffer) ) 

#define INSSBuffer_GetBufferAndLength(This,ppdwBuffer,pdwLength)	\
    ( (This)->lpVtbl -> GetBufferAndLength(This,ppdwBuffer,pdwLength) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE INSSBuffer_GetLength_Proxy( 
    INSSBuffer * This,
    /* [out] */ DWORD *pdwLength);


void __RPC_STUB INSSBuffer_GetLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE INSSBuffer_SetLength_Proxy( 
    INSSBuffer * This,
    /* [in] */ DWORD dwLength);


void __RPC_STUB INSSBuffer_SetLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE INSSBuffer_GetMaxLength_Proxy( 
    INSSBuffer * This,
    /* [out] */ DWORD *pdwLength);


void __RPC_STUB INSSBuffer_GetMaxLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE INSSBuffer_GetBuffer_Proxy( 
    INSSBuffer * This,
    /* [out] */ BYTE **ppdwBuffer);


void __RPC_STUB INSSBuffer_GetBuffer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE INSSBuffer_GetBufferAndLength_Proxy( 
    INSSBuffer * This,
    /* [out] */ BYTE **ppdwBuffer,
    /* [out] */ DWORD *pdwLength);


void __RPC_STUB INSSBuffer_GetBufferAndLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __INSSBuffer_INTERFACE_DEFINED__ */


#ifndef __INSSBuffer2_INTERFACE_DEFINED__
#define __INSSBuffer2_INTERFACE_DEFINED__

/* interface INSSBuffer2 */
/* [version][uuid][unique][object][local] */ 


EXTERN_C const IID IID_INSSBuffer2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4F528693-1035-43fe-B428-757561AD3A68")
    INSSBuffer2 : public INSSBuffer
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetSampleProperties( 
            /* [in] */ DWORD cbProperties,
            /* [out] */ BYTE *pbProperties) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSampleProperties( 
            /* [in] */ DWORD cbProperties,
            /* [in] */ BYTE *pbProperties) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct INSSBuffer2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INSSBuffer2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INSSBuffer2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INSSBuffer2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetLength )( 
            INSSBuffer2 * This,
            /* [out] */ DWORD *pdwLength);
        
        HRESULT ( STDMETHODCALLTYPE *SetLength )( 
            INSSBuffer2 * This,
            /* [in] */ DWORD dwLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetMaxLength )( 
            INSSBuffer2 * This,
            /* [out] */ DWORD *pdwLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetBuffer )( 
            INSSBuffer2 * This,
            /* [out] */ BYTE **ppdwBuffer);
        
        HRESULT ( STDMETHODCALLTYPE *GetBufferAndLength )( 
            INSSBuffer2 * This,
            /* [out] */ BYTE **ppdwBuffer,
            /* [out] */ DWORD *pdwLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetSampleProperties )( 
            INSSBuffer2 * This,
            /* [in] */ DWORD cbProperties,
            /* [out] */ BYTE *pbProperties);
        
        HRESULT ( STDMETHODCALLTYPE *SetSampleProperties )( 
            INSSBuffer2 * This,
            /* [in] */ DWORD cbProperties,
            /* [in] */ BYTE *pbProperties);
        
        END_INTERFACE
    } INSSBuffer2Vtbl;

    interface INSSBuffer2
    {
        CONST_VTBL struct INSSBuffer2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INSSBuffer2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define INSSBuffer2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define INSSBuffer2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define INSSBuffer2_GetLength(This,pdwLength)	\
    ( (This)->lpVtbl -> GetLength(This,pdwLength) ) 

#define INSSBuffer2_SetLength(This,dwLength)	\
    ( (This)->lpVtbl -> SetLength(This,dwLength) ) 

#define INSSBuffer2_GetMaxLength(This,pdwLength)	\
    ( (This)->lpVtbl -> GetMaxLength(This,pdwLength) ) 

#define INSSBuffer2_GetBuffer(This,ppdwBuffer)	\
    ( (This)->lpVtbl -> GetBuffer(This,ppdwBuffer) ) 

#define INSSBuffer2_GetBufferAndLength(This,ppdwBuffer,pdwLength)	\
    ( (This)->lpVtbl -> GetBufferAndLength(This,ppdwBuffer,pdwLength) ) 


#define INSSBuffer2_GetSampleProperties(This,cbProperties,pbProperties)	\
    ( (This)->lpVtbl -> GetSampleProperties(This,cbProperties,pbProperties) ) 

#define INSSBuffer2_SetSampleProperties(This,cbProperties,pbProperties)	\
    ( (This)->lpVtbl -> SetSampleProperties(This,cbProperties,pbProperties) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE INSSBuffer2_GetSampleProperties_Proxy( 
    INSSBuffer2 * This,
    /* [in] */ DWORD cbProperties,
    /* [out] */ BYTE *pbProperties);


void __RPC_STUB INSSBuffer2_GetSampleProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE INSSBuffer2_SetSampleProperties_Proxy( 
    INSSBuffer2 * This,
    /* [in] */ DWORD cbProperties,
    /* [in] */ BYTE *pbProperties);


void __RPC_STUB INSSBuffer2_SetSampleProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __INSSBuffer2_INTERFACE_DEFINED__ */


#ifndef __INSSBuffer3_INTERFACE_DEFINED__
#define __INSSBuffer3_INTERFACE_DEFINED__

/* interface INSSBuffer3 */
/* [version][uuid][unique][object][local] */ 


EXTERN_C const IID IID_INSSBuffer3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C87CEAAF-75BE-4bc4-84EB-AC2798507672")
    INSSBuffer3 : public INSSBuffer2
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetProperty( 
            /* [in] */ GUID guidBufferProperty,
            /* [in] */ void *pvBufferProperty,
            /* [in] */ DWORD dwBufferPropertySize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProperty( 
            /* [in] */ GUID guidBufferProperty,
            /* [out] */ void *pvBufferProperty,
            /* [out][in] */ DWORD *pdwBufferPropertySize) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct INSSBuffer3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INSSBuffer3 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INSSBuffer3 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INSSBuffer3 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetLength )( 
            INSSBuffer3 * This,
            /* [out] */ DWORD *pdwLength);
        
        HRESULT ( STDMETHODCALLTYPE *SetLength )( 
            INSSBuffer3 * This,
            /* [in] */ DWORD dwLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetMaxLength )( 
            INSSBuffer3 * This,
            /* [out] */ DWORD *pdwLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetBuffer )( 
            INSSBuffer3 * This,
            /* [out] */ BYTE **ppdwBuffer);
        
        HRESULT ( STDMETHODCALLTYPE *GetBufferAndLength )( 
            INSSBuffer3 * This,
            /* [out] */ BYTE **ppdwBuffer,
            /* [out] */ DWORD *pdwLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetSampleProperties )( 
            INSSBuffer3 * This,
            /* [in] */ DWORD cbProperties,
            /* [out] */ BYTE *pbProperties);
        
        HRESULT ( STDMETHODCALLTYPE *SetSampleProperties )( 
            INSSBuffer3 * This,
            /* [in] */ DWORD cbProperties,
            /* [in] */ BYTE *pbProperties);
        
        HRESULT ( STDMETHODCALLTYPE *SetProperty )( 
            INSSBuffer3 * This,
            /* [in] */ GUID guidBufferProperty,
            /* [in] */ void *pvBufferProperty,
            /* [in] */ DWORD dwBufferPropertySize);
        
        HRESULT ( STDMETHODCALLTYPE *GetProperty )( 
            INSSBuffer3 * This,
            /* [in] */ GUID guidBufferProperty,
            /* [out] */ void *pvBufferProperty,
            /* [out][in] */ DWORD *pdwBufferPropertySize);
        
        END_INTERFACE
    } INSSBuffer3Vtbl;

    interface INSSBuffer3
    {
        CONST_VTBL struct INSSBuffer3Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INSSBuffer3_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define INSSBuffer3_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define INSSBuffer3_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define INSSBuffer3_GetLength(This,pdwLength)	\
    ( (This)->lpVtbl -> GetLength(This,pdwLength) ) 

#define INSSBuffer3_SetLength(This,dwLength)	\
    ( (This)->lpVtbl -> SetLength(This,dwLength) ) 

#define INSSBuffer3_GetMaxLength(This,pdwLength)	\
    ( (This)->lpVtbl -> GetMaxLength(This,pdwLength) ) 

#define INSSBuffer3_GetBuffer(This,ppdwBuffer)	\
    ( (This)->lpVtbl -> GetBuffer(This,ppdwBuffer) ) 

#define INSSBuffer3_GetBufferAndLength(This,ppdwBuffer,pdwLength)	\
    ( (This)->lpVtbl -> GetBufferAndLength(This,ppdwBuffer,pdwLength) ) 


#define INSSBuffer3_GetSampleProperties(This,cbProperties,pbProperties)	\
    ( (This)->lpVtbl -> GetSampleProperties(This,cbProperties,pbProperties) ) 

#define INSSBuffer3_SetSampleProperties(This,cbProperties,pbProperties)	\
    ( (This)->lpVtbl -> SetSampleProperties(This,cbProperties,pbProperties) ) 


#define INSSBuffer3_SetProperty(This,guidBufferProperty,pvBufferProperty,dwBufferPropertySize)	\
    ( (This)->lpVtbl -> SetProperty(This,guidBufferProperty,pvBufferProperty,dwBufferPropertySize) ) 

#define INSSBuffer3_GetProperty(This,guidBufferProperty,pvBufferProperty,pdwBufferPropertySize)	\
    ( (This)->lpVtbl -> GetProperty(This,guidBufferProperty,pvBufferProperty,pdwBufferPropertySize) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE INSSBuffer3_SetProperty_Proxy( 
    INSSBuffer3 * This,
    /* [in] */ GUID guidBufferProperty,
    /* [in] */ void *pvBufferProperty,
    /* [in] */ DWORD dwBufferPropertySize);


void __RPC_STUB INSSBuffer3_SetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE INSSBuffer3_GetProperty_Proxy( 
    INSSBuffer3 * This,
    /* [in] */ GUID guidBufferProperty,
    /* [out] */ void *pvBufferProperty,
    /* [out][in] */ DWORD *pdwBufferPropertySize);


void __RPC_STUB INSSBuffer3_GetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __INSSBuffer3_INTERFACE_DEFINED__ */


#ifndef __INSSBuffer4_INTERFACE_DEFINED__
#define __INSSBuffer4_INTERFACE_DEFINED__

/* interface INSSBuffer4 */
/* [version][uuid][unique][object][local] */ 


EXTERN_C const IID IID_INSSBuffer4;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B6B8FD5A-32E2-49d4-A910-C26CC85465ED")
    INSSBuffer4 : public INSSBuffer3
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetPropertyCount( 
            /* [out] */ DWORD *pcBufferProperties) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPropertyByIndex( 
            /* [in] */ DWORD dwBufferPropertyIndex,
            /* [out] */ GUID *pguidBufferProperty,
            /* [out] */ void *pvBufferProperty,
            /* [out][in] */ DWORD *pdwBufferPropertySize) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct INSSBuffer4Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INSSBuffer4 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INSSBuffer4 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INSSBuffer4 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetLength )( 
            INSSBuffer4 * This,
            /* [out] */ DWORD *pdwLength);
        
        HRESULT ( STDMETHODCALLTYPE *SetLength )( 
            INSSBuffer4 * This,
            /* [in] */ DWORD dwLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetMaxLength )( 
            INSSBuffer4 * This,
            /* [out] */ DWORD *pdwLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetBuffer )( 
            INSSBuffer4 * This,
            /* [out] */ BYTE **ppdwBuffer);
        
        HRESULT ( STDMETHODCALLTYPE *GetBufferAndLength )( 
            INSSBuffer4 * This,
            /* [out] */ BYTE **ppdwBuffer,
            /* [out] */ DWORD *pdwLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetSampleProperties )( 
            INSSBuffer4 * This,
            /* [in] */ DWORD cbProperties,
            /* [out] */ BYTE *pbProperties);
        
        HRESULT ( STDMETHODCALLTYPE *SetSampleProperties )( 
            INSSBuffer4 * This,
            /* [in] */ DWORD cbProperties,
            /* [in] */ BYTE *pbProperties);
        
        HRESULT ( STDMETHODCALLTYPE *SetProperty )( 
            INSSBuffer4 * This,
            /* [in] */ GUID guidBufferProperty,
            /* [in] */ void *pvBufferProperty,
            /* [in] */ DWORD dwBufferPropertySize);
        
        HRESULT ( STDMETHODCALLTYPE *GetProperty )( 
            INSSBuffer4 * This,
            /* [in] */ GUID guidBufferProperty,
            /* [out] */ void *pvBufferProperty,
            /* [out][in] */ DWORD *pdwBufferPropertySize);
        
        HRESULT ( STDMETHODCALLTYPE *GetPropertyCount )( 
            INSSBuffer4 * This,
            /* [out] */ DWORD *pcBufferProperties);
        
        HRESULT ( STDMETHODCALLTYPE *GetPropertyByIndex )( 
            INSSBuffer4 * This,
            /* [in] */ DWORD dwBufferPropertyIndex,
            /* [out] */ GUID *pguidBufferProperty,
            /* [out] */ void *pvBufferProperty,
            /* [out][in] */ DWORD *pdwBufferPropertySize);
        
        END_INTERFACE
    } INSSBuffer4Vtbl;

    interface INSSBuffer4
    {
        CONST_VTBL struct INSSBuffer4Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INSSBuffer4_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define INSSBuffer4_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define INSSBuffer4_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define INSSBuffer4_GetLength(This,pdwLength)	\
    ( (This)->lpVtbl -> GetLength(This,pdwLength) ) 

#define INSSBuffer4_SetLength(This,dwLength)	\
    ( (This)->lpVtbl -> SetLength(This,dwLength) ) 

#define INSSBuffer4_GetMaxLength(This,pdwLength)	\
    ( (This)->lpVtbl -> GetMaxLength(This,pdwLength) ) 

#define INSSBuffer4_GetBuffer(This,ppdwBuffer)	\
    ( (This)->lpVtbl -> GetBuffer(This,ppdwBuffer) ) 

#define INSSBuffer4_GetBufferAndLength(This,ppdwBuffer,pdwLength)	\
    ( (This)->lpVtbl -> GetBufferAndLength(This,ppdwBuffer,pdwLength) ) 


#define INSSBuffer4_GetSampleProperties(This,cbProperties,pbProperties)	\
    ( (This)->lpVtbl -> GetSampleProperties(This,cbProperties,pbProperties) ) 

#define INSSBuffer4_SetSampleProperties(This,cbProperties,pbProperties)	\
    ( (This)->lpVtbl -> SetSampleProperties(This,cbProperties,pbProperties) ) 


#define INSSBuffer4_SetProperty(This,guidBufferProperty,pvBufferProperty,dwBufferPropertySize)	\
    ( (This)->lpVtbl -> SetProperty(This,guidBufferProperty,pvBufferProperty,dwBufferPropertySize) ) 

#define INSSBuffer4_GetProperty(This,guidBufferProperty,pvBufferProperty,pdwBufferPropertySize)	\
    ( (This)->lpVtbl -> GetProperty(This,guidBufferProperty,pvBufferProperty,pdwBufferPropertySize) ) 


#define INSSBuffer4_GetPropertyCount(This,pcBufferProperties)	\
    ( (This)->lpVtbl -> GetPropertyCount(This,pcBufferProperties) ) 

#define INSSBuffer4_GetPropertyByIndex(This,dwBufferPropertyIndex,pguidBufferProperty,pvBufferProperty,pdwBufferPropertySize)	\
    ( (This)->lpVtbl -> GetPropertyByIndex(This,dwBufferPropertyIndex,pguidBufferProperty,pvBufferProperty,pdwBufferPropertySize) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE INSSBuffer4_GetPropertyCount_Proxy( 
    INSSBuffer4 * This,
    /* [out] */ DWORD *pcBufferProperties);


void __RPC_STUB INSSBuffer4_GetPropertyCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE INSSBuffer4_GetPropertyByIndex_Proxy( 
    INSSBuffer4 * This,
    /* [in] */ DWORD dwBufferPropertyIndex,
    /* [out] */ GUID *pguidBufferProperty,
    /* [out] */ void *pvBufferProperty,
    /* [out][in] */ DWORD *pdwBufferPropertySize);


void __RPC_STUB INSSBuffer4_GetPropertyByIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __INSSBuffer4_INTERFACE_DEFINED__ */


#ifndef __IWMSBufferAllocator_INTERFACE_DEFINED__
#define __IWMSBufferAllocator_INTERFACE_DEFINED__

/* interface IWMSBufferAllocator */
/* [version][uuid][unique][object][local] */ 


EXTERN_C const IID IID_IWMSBufferAllocator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("61103CA4-2033-11d2-9EF1-006097D2D7CF")
    IWMSBufferAllocator : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AllocateBuffer( 
            /* [in] */ DWORD dwMaxBufferSize,
            /* [out] */ INSSBuffer **ppBuffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AllocatePageSizeBuffer( 
            /* [in] */ DWORD dwMaxBufferSize,
            /* [out] */ INSSBuffer **ppBuffer) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMSBufferAllocatorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMSBufferAllocator * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMSBufferAllocator * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMSBufferAllocator * This);
        
        HRESULT ( STDMETHODCALLTYPE *AllocateBuffer )( 
            IWMSBufferAllocator * This,
            /* [in] */ DWORD dwMaxBufferSize,
            /* [out] */ INSSBuffer **ppBuffer);
        
        HRESULT ( STDMETHODCALLTYPE *AllocatePageSizeBuffer )( 
            IWMSBufferAllocator * This,
            /* [in] */ DWORD dwMaxBufferSize,
            /* [out] */ INSSBuffer **ppBuffer);
        
        END_INTERFACE
    } IWMSBufferAllocatorVtbl;

    interface IWMSBufferAllocator
    {
        CONST_VTBL struct IWMSBufferAllocatorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMSBufferAllocator_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMSBufferAllocator_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMSBufferAllocator_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMSBufferAllocator_AllocateBuffer(This,dwMaxBufferSize,ppBuffer)	\
    ( (This)->lpVtbl -> AllocateBuffer(This,dwMaxBufferSize,ppBuffer) ) 

#define IWMSBufferAllocator_AllocatePageSizeBuffer(This,dwMaxBufferSize,ppBuffer)	\
    ( (This)->lpVtbl -> AllocatePageSizeBuffer(This,dwMaxBufferSize,ppBuffer) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IWMSBufferAllocator_AllocateBuffer_Proxy( 
    IWMSBufferAllocator * This,
    /* [in] */ DWORD dwMaxBufferSize,
    /* [out] */ INSSBuffer **ppBuffer);


void __RPC_STUB IWMSBufferAllocator_AllocateBuffer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMSBufferAllocator_AllocatePageSizeBuffer_Proxy( 
    IWMSBufferAllocator * This,
    /* [in] */ DWORD dwMaxBufferSize,
    /* [out] */ INSSBuffer **ppBuffer);


void __RPC_STUB IWMSBufferAllocator_AllocatePageSizeBuffer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMSBufferAllocator_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


