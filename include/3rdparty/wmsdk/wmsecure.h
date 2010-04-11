

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0498 */
/* Compiler settings for wmsecure.idl:
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

#ifndef __wmsecure_h__
#define __wmsecure_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWMAuthorizer_FWD_DEFINED__
#define __IWMAuthorizer_FWD_DEFINED__
typedef interface IWMAuthorizer IWMAuthorizer;
#endif 	/* __IWMAuthorizer_FWD_DEFINED__ */


#ifndef __IWMSecureChannel_FWD_DEFINED__
#define __IWMSecureChannel_FWD_DEFINED__
typedef interface IWMSecureChannel IWMSecureChannel;
#endif 	/* __IWMSecureChannel_FWD_DEFINED__ */


#ifndef __IWMGetSecureChannel_FWD_DEFINED__
#define __IWMGetSecureChannel_FWD_DEFINED__
typedef interface IWMGetSecureChannel IWMGetSecureChannel;
#endif 	/* __IWMGetSecureChannel_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_wmsecure_0000_0000 */
/* [local] */ 

//=========================================================================
//
//  THIS SOFTWARE HAS BEEN LICENSED FROM MICROSOFT CORPORATION PURSUANT 
//  TO THE TERMS OF AN END USER LICENSE AGREEMENT ("EULA").  
//  PLEASE REFER TO THE TEXT OF THE EULA TO DETERMINE THE RIGHTS TO USE THE SOFTWARE.  
//
// Copyright (C) Microsoft Corporation, 1999 - 1999  All Rights Reserved.
//
//=========================================================================


extern RPC_IF_HANDLE __MIDL_itf_wmsecure_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wmsecure_0000_0000_v0_0_s_ifspec;

#ifndef __IWMAuthorizer_INTERFACE_DEFINED__
#define __IWMAuthorizer_INTERFACE_DEFINED__

/* interface IWMAuthorizer */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMAuthorizer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D9B67D36-A9AD-4eb4-BAEF-DB284EF5504C")
    IWMAuthorizer : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetCertCount( 
            /* [out] */ DWORD *pcCerts) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCert( 
            /* [in] */ DWORD dwIndex,
            /* [out] */ BYTE **ppbCertData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSharedData( 
            /* [in] */ DWORD dwCertIndex,
            /* [in] */ const BYTE *pbSharedData,
            /* [in] */ BYTE *pbCert,
            /* [out] */ BYTE **ppbSharedData) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMAuthorizerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMAuthorizer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMAuthorizer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMAuthorizer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCertCount )( 
            IWMAuthorizer * This,
            /* [out] */ DWORD *pcCerts);
        
        HRESULT ( STDMETHODCALLTYPE *GetCert )( 
            IWMAuthorizer * This,
            /* [in] */ DWORD dwIndex,
            /* [out] */ BYTE **ppbCertData);
        
        HRESULT ( STDMETHODCALLTYPE *GetSharedData )( 
            IWMAuthorizer * This,
            /* [in] */ DWORD dwCertIndex,
            /* [in] */ const BYTE *pbSharedData,
            /* [in] */ BYTE *pbCert,
            /* [out] */ BYTE **ppbSharedData);
        
        END_INTERFACE
    } IWMAuthorizerVtbl;

    interface IWMAuthorizer
    {
        CONST_VTBL struct IWMAuthorizerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMAuthorizer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMAuthorizer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMAuthorizer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMAuthorizer_GetCertCount(This,pcCerts)	\
    ( (This)->lpVtbl -> GetCertCount(This,pcCerts) ) 

#define IWMAuthorizer_GetCert(This,dwIndex,ppbCertData)	\
    ( (This)->lpVtbl -> GetCert(This,dwIndex,ppbCertData) ) 

#define IWMAuthorizer_GetSharedData(This,dwCertIndex,pbSharedData,pbCert,ppbSharedData)	\
    ( (This)->lpVtbl -> GetSharedData(This,dwCertIndex,pbSharedData,pbCert,ppbSharedData) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMAuthorizer_INTERFACE_DEFINED__ */


#ifndef __IWMSecureChannel_INTERFACE_DEFINED__
#define __IWMSecureChannel_INTERFACE_DEFINED__

/* interface IWMSecureChannel */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMSecureChannel;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2720598A-D0F2-4189-BD10-91C46EF0936F")
    IWMSecureChannel : public IWMAuthorizer
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE WMSC_AddCertificate( 
            /* [in] */ IWMAuthorizer *pCert) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WMSC_AddSignature( 
            /* [in] */ BYTE *pbCertSig,
            /* [in] */ DWORD cbCertSig) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WMSC_Connect( 
            /* [in] */ IWMSecureChannel *pOtherSide) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WMSC_IsConnected( 
            /* [out] */ BOOL *pfIsConnected) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WMSC_Disconnect( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WMSC_GetValidCertificate( 
            /* [out] */ BYTE **ppbCertificate,
            /* [out] */ DWORD *pdwSignature) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WMSC_Encrypt( 
            /* [in] */ BYTE *pbData,
            /* [in] */ DWORD cbData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WMSC_Decrypt( 
            /* [in] */ BYTE *pbData,
            /* [in] */ DWORD cbData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WMSC_Lock( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WMSC_Unlock( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WMSC_SetSharedData( 
            /* [in] */ DWORD dwCertIndex,
            /* [in] */ const BYTE *pbSharedData) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMSecureChannelVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMSecureChannel * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMSecureChannel * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMSecureChannel * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCertCount )( 
            IWMSecureChannel * This,
            /* [out] */ DWORD *pcCerts);
        
        HRESULT ( STDMETHODCALLTYPE *GetCert )( 
            IWMSecureChannel * This,
            /* [in] */ DWORD dwIndex,
            /* [out] */ BYTE **ppbCertData);
        
        HRESULT ( STDMETHODCALLTYPE *GetSharedData )( 
            IWMSecureChannel * This,
            /* [in] */ DWORD dwCertIndex,
            /* [in] */ const BYTE *pbSharedData,
            /* [in] */ BYTE *pbCert,
            /* [out] */ BYTE **ppbSharedData);
        
        HRESULT ( STDMETHODCALLTYPE *WMSC_AddCertificate )( 
            IWMSecureChannel * This,
            /* [in] */ IWMAuthorizer *pCert);
        
        HRESULT ( STDMETHODCALLTYPE *WMSC_AddSignature )( 
            IWMSecureChannel * This,
            /* [in] */ BYTE *pbCertSig,
            /* [in] */ DWORD cbCertSig);
        
        HRESULT ( STDMETHODCALLTYPE *WMSC_Connect )( 
            IWMSecureChannel * This,
            /* [in] */ IWMSecureChannel *pOtherSide);
        
        HRESULT ( STDMETHODCALLTYPE *WMSC_IsConnected )( 
            IWMSecureChannel * This,
            /* [out] */ BOOL *pfIsConnected);
        
        HRESULT ( STDMETHODCALLTYPE *WMSC_Disconnect )( 
            IWMSecureChannel * This);
        
        HRESULT ( STDMETHODCALLTYPE *WMSC_GetValidCertificate )( 
            IWMSecureChannel * This,
            /* [out] */ BYTE **ppbCertificate,
            /* [out] */ DWORD *pdwSignature);
        
        HRESULT ( STDMETHODCALLTYPE *WMSC_Encrypt )( 
            IWMSecureChannel * This,
            /* [in] */ BYTE *pbData,
            /* [in] */ DWORD cbData);
        
        HRESULT ( STDMETHODCALLTYPE *WMSC_Decrypt )( 
            IWMSecureChannel * This,
            /* [in] */ BYTE *pbData,
            /* [in] */ DWORD cbData);
        
        HRESULT ( STDMETHODCALLTYPE *WMSC_Lock )( 
            IWMSecureChannel * This);
        
        HRESULT ( STDMETHODCALLTYPE *WMSC_Unlock )( 
            IWMSecureChannel * This);
        
        HRESULT ( STDMETHODCALLTYPE *WMSC_SetSharedData )( 
            IWMSecureChannel * This,
            /* [in] */ DWORD dwCertIndex,
            /* [in] */ const BYTE *pbSharedData);
        
        END_INTERFACE
    } IWMSecureChannelVtbl;

    interface IWMSecureChannel
    {
        CONST_VTBL struct IWMSecureChannelVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMSecureChannel_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMSecureChannel_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMSecureChannel_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMSecureChannel_GetCertCount(This,pcCerts)	\
    ( (This)->lpVtbl -> GetCertCount(This,pcCerts) ) 

#define IWMSecureChannel_GetCert(This,dwIndex,ppbCertData)	\
    ( (This)->lpVtbl -> GetCert(This,dwIndex,ppbCertData) ) 

#define IWMSecureChannel_GetSharedData(This,dwCertIndex,pbSharedData,pbCert,ppbSharedData)	\
    ( (This)->lpVtbl -> GetSharedData(This,dwCertIndex,pbSharedData,pbCert,ppbSharedData) ) 


#define IWMSecureChannel_WMSC_AddCertificate(This,pCert)	\
    ( (This)->lpVtbl -> WMSC_AddCertificate(This,pCert) ) 

#define IWMSecureChannel_WMSC_AddSignature(This,pbCertSig,cbCertSig)	\
    ( (This)->lpVtbl -> WMSC_AddSignature(This,pbCertSig,cbCertSig) ) 

#define IWMSecureChannel_WMSC_Connect(This,pOtherSide)	\
    ( (This)->lpVtbl -> WMSC_Connect(This,pOtherSide) ) 

#define IWMSecureChannel_WMSC_IsConnected(This,pfIsConnected)	\
    ( (This)->lpVtbl -> WMSC_IsConnected(This,pfIsConnected) ) 

#define IWMSecureChannel_WMSC_Disconnect(This)	\
    ( (This)->lpVtbl -> WMSC_Disconnect(This) ) 

#define IWMSecureChannel_WMSC_GetValidCertificate(This,ppbCertificate,pdwSignature)	\
    ( (This)->lpVtbl -> WMSC_GetValidCertificate(This,ppbCertificate,pdwSignature) ) 

#define IWMSecureChannel_WMSC_Encrypt(This,pbData,cbData)	\
    ( (This)->lpVtbl -> WMSC_Encrypt(This,pbData,cbData) ) 

#define IWMSecureChannel_WMSC_Decrypt(This,pbData,cbData)	\
    ( (This)->lpVtbl -> WMSC_Decrypt(This,pbData,cbData) ) 

#define IWMSecureChannel_WMSC_Lock(This)	\
    ( (This)->lpVtbl -> WMSC_Lock(This) ) 

#define IWMSecureChannel_WMSC_Unlock(This)	\
    ( (This)->lpVtbl -> WMSC_Unlock(This) ) 

#define IWMSecureChannel_WMSC_SetSharedData(This,dwCertIndex,pbSharedData)	\
    ( (This)->lpVtbl -> WMSC_SetSharedData(This,dwCertIndex,pbSharedData) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMSecureChannel_INTERFACE_DEFINED__ */


#ifndef __IWMGetSecureChannel_INTERFACE_DEFINED__
#define __IWMGetSecureChannel_INTERFACE_DEFINED__

/* interface IWMGetSecureChannel */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMGetSecureChannel;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("94bc0598-c3d2-11d3-bedf-00c04f612986")
    IWMGetSecureChannel : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetPeerSecureChannelInterface( 
            /* [out] */ IWMSecureChannel **ppPeer) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMGetSecureChannelVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMGetSecureChannel * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMGetSecureChannel * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMGetSecureChannel * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetPeerSecureChannelInterface )( 
            IWMGetSecureChannel * This,
            /* [out] */ IWMSecureChannel **ppPeer);
        
        END_INTERFACE
    } IWMGetSecureChannelVtbl;

    interface IWMGetSecureChannel
    {
        CONST_VTBL struct IWMGetSecureChannelVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMGetSecureChannel_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMGetSecureChannel_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMGetSecureChannel_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMGetSecureChannel_GetPeerSecureChannelInterface(This,ppPeer)	\
    ( (This)->lpVtbl -> GetPeerSecureChannelInterface(This,ppPeer) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMGetSecureChannel_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_wmsecure_0000_0003 */
/* [local] */ 

HRESULT STDMETHODCALLTYPE WMCreateSecureChannel( IWMSecureChannel** ppChannel );
HRESULT STDMETHODCALLTYPE WMCreateSecureChannel_Certified( IWMSecureChannel** ppChannel ); 
HRESULT STDMETHODCALLTYPE WMCreateSecureChannel_DES( IWMSecureChannel** ppChannel );
HRESULT STDMETHODCALLTYPE WMCreateSecureChannel_Certified_DES( IWMSecureChannel** ppChannel ); 


extern RPC_IF_HANDLE __MIDL_itf_wmsecure_0000_0003_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wmsecure_0000_0003_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


