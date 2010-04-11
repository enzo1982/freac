

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0498 */
/* Compiler settings for wmdxva.idl:
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

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __wmdxva_h__
#define __wmdxva_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWMPlayerTimestampHook_FWD_DEFINED__
#define __IWMPlayerTimestampHook_FWD_DEFINED__
typedef interface IWMPlayerTimestampHook IWMPlayerTimestampHook;
#endif 	/* __IWMPlayerTimestampHook_FWD_DEFINED__ */


#ifndef __IWMCodecAMVideoAccelerator_FWD_DEFINED__
#define __IWMCodecAMVideoAccelerator_FWD_DEFINED__
typedef interface IWMCodecAMVideoAccelerator IWMCodecAMVideoAccelerator;
#endif 	/* __IWMCodecAMVideoAccelerator_FWD_DEFINED__ */


#ifndef __IWMCodecVideoAccelerator_FWD_DEFINED__
#define __IWMCodecVideoAccelerator_FWD_DEFINED__
typedef interface IWMCodecVideoAccelerator IWMCodecVideoAccelerator;
#endif 	/* __IWMCodecVideoAccelerator_FWD_DEFINED__ */


/* header files for imported files */
#include "mediaobj.h"
#include "videoacc.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_wmdxva_0000_0000 */
/* [local] */ 

//=========================================================================
//
// Microsoft Windows Media Technologies
// Copyright (C) Microsoft Corporation.  All Rights Reserved.
//
//=========================================================================
EXTERN_GUID( IID_IWMPlayerTimestampHook,   0x28580dda, 0xd98e, 0x48d0, 0xb7, 0xae, 0x69, 0xe4, 0x73, 0xa0, 0x28, 0x25);
EXTERN_GUID( IID_IWMCodecVideoAccelerator, 0x990641b0, 0x739f, 0x4e94, 0xa8, 0x08, 0x98, 0x88, 0xda, 0x8f, 0x75, 0xaf);
EXTERN_GUID( IID_IWMCodecAMVideoAccelerator, 0xd98ee251, 0x34e0, 0x4a2d, 0x93, 0x12, 0x9b, 0x4c, 0x78, 0x8d, 0x9f, 0xa1);


extern RPC_IF_HANDLE __MIDL_itf_wmdxva_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wmdxva_0000_0000_v0_0_s_ifspec;

#ifndef __IWMPlayerTimestampHook_INTERFACE_DEFINED__
#define __IWMPlayerTimestampHook_INTERFACE_DEFINED__

/* interface IWMPlayerTimestampHook */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMPlayerTimestampHook;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("28580dda-d98e-48d0-b7ae-69e473a02825")
    IWMPlayerTimestampHook : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE MapTimestamp( 
            /* [in] */ REFERENCE_TIME rtIn,
            /* [out] */ REFERENCE_TIME *prtOut) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPlayerTimestampHookVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPlayerTimestampHook * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPlayerTimestampHook * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPlayerTimestampHook * This);
        
        HRESULT ( STDMETHODCALLTYPE *MapTimestamp )( 
            IWMPlayerTimestampHook * This,
            /* [in] */ REFERENCE_TIME rtIn,
            /* [out] */ REFERENCE_TIME *prtOut);
        
        END_INTERFACE
    } IWMPlayerTimestampHookVtbl;

    interface IWMPlayerTimestampHook
    {
        CONST_VTBL struct IWMPlayerTimestampHookVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPlayerTimestampHook_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMPlayerTimestampHook_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMPlayerTimestampHook_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMPlayerTimestampHook_MapTimestamp(This,rtIn,prtOut)	\
    ( (This)->lpVtbl -> MapTimestamp(This,rtIn,prtOut) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMPlayerTimestampHook_INTERFACE_DEFINED__ */


#ifndef __IWMCodecAMVideoAccelerator_INTERFACE_DEFINED__
#define __IWMCodecAMVideoAccelerator_INTERFACE_DEFINED__

/* interface IWMCodecAMVideoAccelerator */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMCodecAMVideoAccelerator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("d98ee251-34e0-4a2d-9312-9b4c788d9fa1")
    IWMCodecAMVideoAccelerator : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetAcceleratorInterface( 
            /* [in] */ IAMVideoAccelerator *pIAMVA) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE NegotiateConnection( 
            /* [in] */ DMO_MEDIA_TYPE *pMediaType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetPlayerNotify( 
            /* [in] */ IWMPlayerTimestampHook *pHook) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMCodecAMVideoAcceleratorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMCodecAMVideoAccelerator * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMCodecAMVideoAccelerator * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMCodecAMVideoAccelerator * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetAcceleratorInterface )( 
            IWMCodecAMVideoAccelerator * This,
            /* [in] */ IAMVideoAccelerator *pIAMVA);
        
        HRESULT ( STDMETHODCALLTYPE *NegotiateConnection )( 
            IWMCodecAMVideoAccelerator * This,
            /* [in] */ DMO_MEDIA_TYPE *pMediaType);
        
        HRESULT ( STDMETHODCALLTYPE *SetPlayerNotify )( 
            IWMCodecAMVideoAccelerator * This,
            /* [in] */ IWMPlayerTimestampHook *pHook);
        
        END_INTERFACE
    } IWMCodecAMVideoAcceleratorVtbl;

    interface IWMCodecAMVideoAccelerator
    {
        CONST_VTBL struct IWMCodecAMVideoAcceleratorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMCodecAMVideoAccelerator_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMCodecAMVideoAccelerator_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMCodecAMVideoAccelerator_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMCodecAMVideoAccelerator_SetAcceleratorInterface(This,pIAMVA)	\
    ( (This)->lpVtbl -> SetAcceleratorInterface(This,pIAMVA) ) 

#define IWMCodecAMVideoAccelerator_NegotiateConnection(This,pMediaType)	\
    ( (This)->lpVtbl -> NegotiateConnection(This,pMediaType) ) 

#define IWMCodecAMVideoAccelerator_SetPlayerNotify(This,pHook)	\
    ( (This)->lpVtbl -> SetPlayerNotify(This,pHook) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMCodecAMVideoAccelerator_INTERFACE_DEFINED__ */


#ifndef __IWMCodecVideoAccelerator_INTERFACE_DEFINED__
#define __IWMCodecVideoAccelerator_INTERFACE_DEFINED__

/* interface IWMCodecVideoAccelerator */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMCodecVideoAccelerator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("990641b0-739f-4e94-a808-9888da8f75af")
    IWMCodecVideoAccelerator : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE NegotiateConnection( 
            /* [in] */ IAMVideoAccelerator *pIAMVA,
            /* [in] */ DMO_MEDIA_TYPE *pMediaType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetPlayerNotify( 
            /* [in] */ IWMPlayerTimestampHook *pHook) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMCodecVideoAcceleratorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMCodecVideoAccelerator * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMCodecVideoAccelerator * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMCodecVideoAccelerator * This);
        
        HRESULT ( STDMETHODCALLTYPE *NegotiateConnection )( 
            IWMCodecVideoAccelerator * This,
            /* [in] */ IAMVideoAccelerator *pIAMVA,
            /* [in] */ DMO_MEDIA_TYPE *pMediaType);
        
        HRESULT ( STDMETHODCALLTYPE *SetPlayerNotify )( 
            IWMCodecVideoAccelerator * This,
            /* [in] */ IWMPlayerTimestampHook *pHook);
        
        END_INTERFACE
    } IWMCodecVideoAcceleratorVtbl;

    interface IWMCodecVideoAccelerator
    {
        CONST_VTBL struct IWMCodecVideoAcceleratorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMCodecVideoAccelerator_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMCodecVideoAccelerator_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMCodecVideoAccelerator_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMCodecVideoAccelerator_NegotiateConnection(This,pIAMVA,pMediaType)	\
    ( (This)->lpVtbl -> NegotiateConnection(This,pIAMVA,pMediaType) ) 

#define IWMCodecVideoAccelerator_SetPlayerNotify(This,pHook)	\
    ( (This)->lpVtbl -> SetPlayerNotify(This,pHook) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMCodecVideoAccelerator_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


