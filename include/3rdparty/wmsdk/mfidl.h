

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0498 */
/* Compiler settings for mfidl.idl:
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

#ifndef __mfidl_h__
#define __mfidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMFMediaSession_FWD_DEFINED__
#define __IMFMediaSession_FWD_DEFINED__
typedef interface IMFMediaSession IMFMediaSession;
#endif 	/* __IMFMediaSession_FWD_DEFINED__ */


#ifndef __IMFSourceResolver_FWD_DEFINED__
#define __IMFSourceResolver_FWD_DEFINED__
typedef interface IMFSourceResolver IMFSourceResolver;
#endif 	/* __IMFSourceResolver_FWD_DEFINED__ */


#ifndef __IMFMediaSource_FWD_DEFINED__
#define __IMFMediaSource_FWD_DEFINED__
typedef interface IMFMediaSource IMFMediaSource;
#endif 	/* __IMFMediaSource_FWD_DEFINED__ */


#ifndef __IMFMediaStream_FWD_DEFINED__
#define __IMFMediaStream_FWD_DEFINED__
typedef interface IMFMediaStream IMFMediaStream;
#endif 	/* __IMFMediaStream_FWD_DEFINED__ */


#ifndef __IMFMediaSink_FWD_DEFINED__
#define __IMFMediaSink_FWD_DEFINED__
typedef interface IMFMediaSink IMFMediaSink;
#endif 	/* __IMFMediaSink_FWD_DEFINED__ */


#ifndef __IMFStreamSink_FWD_DEFINED__
#define __IMFStreamSink_FWD_DEFINED__
typedef interface IMFStreamSink IMFStreamSink;
#endif 	/* __IMFStreamSink_FWD_DEFINED__ */


#ifndef __IMFVideoSampleAllocator_FWD_DEFINED__
#define __IMFVideoSampleAllocator_FWD_DEFINED__
typedef interface IMFVideoSampleAllocator IMFVideoSampleAllocator;
#endif 	/* __IMFVideoSampleAllocator_FWD_DEFINED__ */


#ifndef __IMFTopology_FWD_DEFINED__
#define __IMFTopology_FWD_DEFINED__
typedef interface IMFTopology IMFTopology;
#endif 	/* __IMFTopology_FWD_DEFINED__ */


#ifndef __IMFTopologyNode_FWD_DEFINED__
#define __IMFTopologyNode_FWD_DEFINED__
typedef interface IMFTopologyNode IMFTopologyNode;
#endif 	/* __IMFTopologyNode_FWD_DEFINED__ */


#ifndef __IMFGetService_FWD_DEFINED__
#define __IMFGetService_FWD_DEFINED__
typedef interface IMFGetService IMFGetService;
#endif 	/* __IMFGetService_FWD_DEFINED__ */


#ifndef __IMFClock_FWD_DEFINED__
#define __IMFClock_FWD_DEFINED__
typedef interface IMFClock IMFClock;
#endif 	/* __IMFClock_FWD_DEFINED__ */


#ifndef __IMFPresentationClock_FWD_DEFINED__
#define __IMFPresentationClock_FWD_DEFINED__
typedef interface IMFPresentationClock IMFPresentationClock;
#endif 	/* __IMFPresentationClock_FWD_DEFINED__ */


#ifndef __IMFPresentationTimeSource_FWD_DEFINED__
#define __IMFPresentationTimeSource_FWD_DEFINED__
typedef interface IMFPresentationTimeSource IMFPresentationTimeSource;
#endif 	/* __IMFPresentationTimeSource_FWD_DEFINED__ */


#ifndef __IMFClockStateSink_FWD_DEFINED__
#define __IMFClockStateSink_FWD_DEFINED__
typedef interface IMFClockStateSink IMFClockStateSink;
#endif 	/* __IMFClockStateSink_FWD_DEFINED__ */


#ifndef __IMFPresentationDescriptor_FWD_DEFINED__
#define __IMFPresentationDescriptor_FWD_DEFINED__
typedef interface IMFPresentationDescriptor IMFPresentationDescriptor;
#endif 	/* __IMFPresentationDescriptor_FWD_DEFINED__ */


#ifndef __IMFStreamDescriptor_FWD_DEFINED__
#define __IMFStreamDescriptor_FWD_DEFINED__
typedef interface IMFStreamDescriptor IMFStreamDescriptor;
#endif 	/* __IMFStreamDescriptor_FWD_DEFINED__ */


#ifndef __IMFMediaTypeHandler_FWD_DEFINED__
#define __IMFMediaTypeHandler_FWD_DEFINED__
typedef interface IMFMediaTypeHandler IMFMediaTypeHandler;
#endif 	/* __IMFMediaTypeHandler_FWD_DEFINED__ */


#ifndef __IMFTimer_FWD_DEFINED__
#define __IMFTimer_FWD_DEFINED__
typedef interface IMFTimer IMFTimer;
#endif 	/* __IMFTimer_FWD_DEFINED__ */


#ifndef __IMFActivate_FWD_DEFINED__
#define __IMFActivate_FWD_DEFINED__
typedef interface IMFActivate IMFActivate;
#endif 	/* __IMFActivate_FWD_DEFINED__ */


#ifndef __IMFShutdown_FWD_DEFINED__
#define __IMFShutdown_FWD_DEFINED__
typedef interface IMFShutdown IMFShutdown;
#endif 	/* __IMFShutdown_FWD_DEFINED__ */


#ifndef __IMFTopoLoader_FWD_DEFINED__
#define __IMFTopoLoader_FWD_DEFINED__
typedef interface IMFTopoLoader IMFTopoLoader;
#endif 	/* __IMFTopoLoader_FWD_DEFINED__ */


#ifndef __IMFContentProtectionManager_FWD_DEFINED__
#define __IMFContentProtectionManager_FWD_DEFINED__
typedef interface IMFContentProtectionManager IMFContentProtectionManager;
#endif 	/* __IMFContentProtectionManager_FWD_DEFINED__ */


#ifndef __IMFContentEnabler_FWD_DEFINED__
#define __IMFContentEnabler_FWD_DEFINED__
typedef interface IMFContentEnabler IMFContentEnabler;
#endif 	/* __IMFContentEnabler_FWD_DEFINED__ */


#ifndef __IMFMetadata_FWD_DEFINED__
#define __IMFMetadata_FWD_DEFINED__
typedef interface IMFMetadata IMFMetadata;
#endif 	/* __IMFMetadata_FWD_DEFINED__ */


#ifndef __IMFMetadataProvider_FWD_DEFINED__
#define __IMFMetadataProvider_FWD_DEFINED__
typedef interface IMFMetadataProvider IMFMetadataProvider;
#endif 	/* __IMFMetadataProvider_FWD_DEFINED__ */


#ifndef __IMFRateSupport_FWD_DEFINED__
#define __IMFRateSupport_FWD_DEFINED__
typedef interface IMFRateSupport IMFRateSupport;
#endif 	/* __IMFRateSupport_FWD_DEFINED__ */


#ifndef __IMFRateControl_FWD_DEFINED__
#define __IMFRateControl_FWD_DEFINED__
typedef interface IMFRateControl IMFRateControl;
#endif 	/* __IMFRateControl_FWD_DEFINED__ */


#ifndef __IMFSimpleAudioVolume_FWD_DEFINED__
#define __IMFSimpleAudioVolume_FWD_DEFINED__
typedef interface IMFSimpleAudioVolume IMFSimpleAudioVolume;
#endif 	/* __IMFSimpleAudioVolume_FWD_DEFINED__ */


#ifndef __IMFAudioStreamVolume_FWD_DEFINED__
#define __IMFAudioStreamVolume_FWD_DEFINED__
typedef interface IMFAudioStreamVolume IMFAudioStreamVolume;
#endif 	/* __IMFAudioStreamVolume_FWD_DEFINED__ */


#ifndef __IMFAudioPolicy_FWD_DEFINED__
#define __IMFAudioPolicy_FWD_DEFINED__
typedef interface IMFAudioPolicy IMFAudioPolicy;
#endif 	/* __IMFAudioPolicy_FWD_DEFINED__ */


#ifndef __IMFSampleGrabberSinkCallback_FWD_DEFINED__
#define __IMFSampleGrabberSinkCallback_FWD_DEFINED__
typedef interface IMFSampleGrabberSinkCallback IMFSampleGrabberSinkCallback;
#endif 	/* __IMFSampleGrabberSinkCallback_FWD_DEFINED__ */


#ifndef __IMFWorkQueueServices_FWD_DEFINED__
#define __IMFWorkQueueServices_FWD_DEFINED__
typedef interface IMFWorkQueueServices IMFWorkQueueServices;
#endif 	/* __IMFWorkQueueServices_FWD_DEFINED__ */


#ifndef __IMFQualityManager_FWD_DEFINED__
#define __IMFQualityManager_FWD_DEFINED__
typedef interface IMFQualityManager IMFQualityManager;
#endif 	/* __IMFQualityManager_FWD_DEFINED__ */


#ifndef __IMFQualityAdvise_FWD_DEFINED__
#define __IMFQualityAdvise_FWD_DEFINED__
typedef interface IMFQualityAdvise IMFQualityAdvise;
#endif 	/* __IMFQualityAdvise_FWD_DEFINED__ */


#ifndef __IMFRealTimeClient_FWD_DEFINED__
#define __IMFRealTimeClient_FWD_DEFINED__
typedef interface IMFRealTimeClient IMFRealTimeClient;
#endif 	/* __IMFRealTimeClient_FWD_DEFINED__ */


#ifndef __IMFSequencerSource_FWD_DEFINED__
#define __IMFSequencerSource_FWD_DEFINED__
typedef interface IMFSequencerSource IMFSequencerSource;
#endif 	/* __IMFSequencerSource_FWD_DEFINED__ */


#ifndef __IMFMediaSourceTopologyProvider_FWD_DEFINED__
#define __IMFMediaSourceTopologyProvider_FWD_DEFINED__
typedef interface IMFMediaSourceTopologyProvider IMFMediaSourceTopologyProvider;
#endif 	/* __IMFMediaSourceTopologyProvider_FWD_DEFINED__ */


#ifndef __IMFMediaSourcePresentationProvider_FWD_DEFINED__
#define __IMFMediaSourcePresentationProvider_FWD_DEFINED__
typedef interface IMFMediaSourcePresentationProvider IMFMediaSourcePresentationProvider;
#endif 	/* __IMFMediaSourcePresentationProvider_FWD_DEFINED__ */


#ifndef __IMFTopologyNodeAttributeEditor_FWD_DEFINED__
#define __IMFTopologyNodeAttributeEditor_FWD_DEFINED__
typedef interface IMFTopologyNodeAttributeEditor IMFTopologyNodeAttributeEditor;
#endif 	/* __IMFTopologyNodeAttributeEditor_FWD_DEFINED__ */


#ifndef __IMFByteStreamBuffering_FWD_DEFINED__
#define __IMFByteStreamBuffering_FWD_DEFINED__
typedef interface IMFByteStreamBuffering IMFByteStreamBuffering;
#endif 	/* __IMFByteStreamBuffering_FWD_DEFINED__ */


#ifndef __IMFNetCredential_FWD_DEFINED__
#define __IMFNetCredential_FWD_DEFINED__
typedef interface IMFNetCredential IMFNetCredential;
#endif 	/* __IMFNetCredential_FWD_DEFINED__ */


#ifndef __IMFNetCredentialManager_FWD_DEFINED__
#define __IMFNetCredentialManager_FWD_DEFINED__
typedef interface IMFNetCredentialManager IMFNetCredentialManager;
#endif 	/* __IMFNetCredentialManager_FWD_DEFINED__ */


#ifndef __IMFNetCredentialCache_FWD_DEFINED__
#define __IMFNetCredentialCache_FWD_DEFINED__
typedef interface IMFNetCredentialCache IMFNetCredentialCache;
#endif 	/* __IMFNetCredentialCache_FWD_DEFINED__ */


#ifndef __IMFSourceOpenMonitor_FWD_DEFINED__
#define __IMFSourceOpenMonitor_FWD_DEFINED__
typedef interface IMFSourceOpenMonitor IMFSourceOpenMonitor;
#endif 	/* __IMFSourceOpenMonitor_FWD_DEFINED__ */


#ifndef __IMFNetProxyLocator_FWD_DEFINED__
#define __IMFNetProxyLocator_FWD_DEFINED__
typedef interface IMFNetProxyLocator IMFNetProxyLocator;
#endif 	/* __IMFNetProxyLocator_FWD_DEFINED__ */


#ifndef __IMFNetProxyLocatorFactory_FWD_DEFINED__
#define __IMFNetProxyLocatorFactory_FWD_DEFINED__
typedef interface IMFNetProxyLocatorFactory IMFNetProxyLocatorFactory;
#endif 	/* __IMFNetProxyLocatorFactory_FWD_DEFINED__ */


#ifndef __IMFSaveJob_FWD_DEFINED__
#define __IMFSaveJob_FWD_DEFINED__
typedef interface IMFSaveJob IMFSaveJob;
#endif 	/* __IMFSaveJob_FWD_DEFINED__ */


#ifndef __IMFNetSchemeHandlerConfig_FWD_DEFINED__
#define __IMFNetSchemeHandlerConfig_FWD_DEFINED__
typedef interface IMFNetSchemeHandlerConfig IMFNetSchemeHandlerConfig;
#endif 	/* __IMFNetSchemeHandlerConfig_FWD_DEFINED__ */


#ifndef __IMFSchemeHandler_FWD_DEFINED__
#define __IMFSchemeHandler_FWD_DEFINED__
typedef interface IMFSchemeHandler IMFSchemeHandler;
#endif 	/* __IMFSchemeHandler_FWD_DEFINED__ */


#ifndef __IMFByteStreamHandler_FWD_DEFINED__
#define __IMFByteStreamHandler_FWD_DEFINED__
typedef interface IMFByteStreamHandler IMFByteStreamHandler;
#endif 	/* __IMFByteStreamHandler_FWD_DEFINED__ */


#ifndef __IMFTrustedInput_FWD_DEFINED__
#define __IMFTrustedInput_FWD_DEFINED__
typedef interface IMFTrustedInput IMFTrustedInput;
#endif 	/* __IMFTrustedInput_FWD_DEFINED__ */


#ifndef __IMFInputTrustAuthority_FWD_DEFINED__
#define __IMFInputTrustAuthority_FWD_DEFINED__
typedef interface IMFInputTrustAuthority IMFInputTrustAuthority;
#endif 	/* __IMFInputTrustAuthority_FWD_DEFINED__ */


#ifndef __IMFTrustedOutput_FWD_DEFINED__
#define __IMFTrustedOutput_FWD_DEFINED__
typedef interface IMFTrustedOutput IMFTrustedOutput;
#endif 	/* __IMFTrustedOutput_FWD_DEFINED__ */


#ifndef __IMFOutputTrustAuthority_FWD_DEFINED__
#define __IMFOutputTrustAuthority_FWD_DEFINED__
typedef interface IMFOutputTrustAuthority IMFOutputTrustAuthority;
#endif 	/* __IMFOutputTrustAuthority_FWD_DEFINED__ */


#ifndef __IMFOutputPolicy_FWD_DEFINED__
#define __IMFOutputPolicy_FWD_DEFINED__
typedef interface IMFOutputPolicy IMFOutputPolicy;
#endif 	/* __IMFOutputPolicy_FWD_DEFINED__ */


#ifndef __IMFOutputSchema_FWD_DEFINED__
#define __IMFOutputSchema_FWD_DEFINED__
typedef interface IMFOutputSchema IMFOutputSchema;
#endif 	/* __IMFOutputSchema_FWD_DEFINED__ */


#ifndef __IMFSecureChannel_FWD_DEFINED__
#define __IMFSecureChannel_FWD_DEFINED__
typedef interface IMFSecureChannel IMFSecureChannel;
#endif 	/* __IMFSecureChannel_FWD_DEFINED__ */


#ifndef __IMFSampleProtection_FWD_DEFINED__
#define __IMFSampleProtection_FWD_DEFINED__
typedef interface IMFSampleProtection IMFSampleProtection;
#endif 	/* __IMFSampleProtection_FWD_DEFINED__ */


#ifndef __IMFMediaSinkPreroll_FWD_DEFINED__
#define __IMFMediaSinkPreroll_FWD_DEFINED__
typedef interface IMFMediaSinkPreroll IMFMediaSinkPreroll;
#endif 	/* __IMFMediaSinkPreroll_FWD_DEFINED__ */


#ifndef __IMFFinalizableMediaSink_FWD_DEFINED__
#define __IMFFinalizableMediaSink_FWD_DEFINED__
typedef interface IMFFinalizableMediaSink IMFFinalizableMediaSink;
#endif 	/* __IMFFinalizableMediaSink_FWD_DEFINED__ */


#ifndef __IMFRemoteProxy_FWD_DEFINED__
#define __IMFRemoteProxy_FWD_DEFINED__
typedef interface IMFRemoteProxy IMFRemoteProxy;
#endif 	/* __IMFRemoteProxy_FWD_DEFINED__ */


#ifndef __IMFObjectReferenceStream_FWD_DEFINED__
#define __IMFObjectReferenceStream_FWD_DEFINED__
typedef interface IMFObjectReferenceStream IMFObjectReferenceStream;
#endif 	/* __IMFObjectReferenceStream_FWD_DEFINED__ */


#ifndef __IMFPMPHost_FWD_DEFINED__
#define __IMFPMPHost_FWD_DEFINED__
typedef interface IMFPMPHost IMFPMPHost;
#endif 	/* __IMFPMPHost_FWD_DEFINED__ */


#ifndef __IMFPMPClient_FWD_DEFINED__
#define __IMFPMPClient_FWD_DEFINED__
typedef interface IMFPMPClient IMFPMPClient;
#endif 	/* __IMFPMPClient_FWD_DEFINED__ */


#ifndef __IMFPMPServer_FWD_DEFINED__
#define __IMFPMPServer_FWD_DEFINED__
typedef interface IMFPMPServer IMFPMPServer;
#endif 	/* __IMFPMPServer_FWD_DEFINED__ */


#ifndef __IMFRemoteDesktopPlugin_FWD_DEFINED__
#define __IMFRemoteDesktopPlugin_FWD_DEFINED__
typedef interface IMFRemoteDesktopPlugin IMFRemoteDesktopPlugin;
#endif 	/* __IMFRemoteDesktopPlugin_FWD_DEFINED__ */


#ifndef __IMFSAMIStyle_FWD_DEFINED__
#define __IMFSAMIStyle_FWD_DEFINED__
typedef interface IMFSAMIStyle IMFSAMIStyle;
#endif 	/* __IMFSAMIStyle_FWD_DEFINED__ */


/* header files for imported files */
#include "mfobjects.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_mfidl_0000_0000 */
/* [local] */ 

#include <windef.h>
typedef 
enum MFSESSION_SETTOPOLOGY_FLAGS
    {	MFSESSION_SETTOPOLOGY_IMMEDIATE	= 0x1,
	MFSESSION_SETTOPOLOGY_NORESOLUTION	= 0x2
    } 	MFSESSION_SETTOPOLOGY_FLAGS;

typedef 
enum MFSESSION_GETFULLTOPOLOGY_FLAGS
    {	MFSESSION_GETFULLTOPOLOGY_CURRENT	= 0x1
    } 	MFSESSION_GETFULLTOPOLOGY_FLAGS;

typedef 
enum MFPMPSESSION_CREATION_FLAGS
    {	MFPMPSESSION_UNPROTECTED_PROCESS	= 0x1
    } 	MFPMPSESSION_CREATION_FLAGS;

typedef unsigned __int64 TOPOID;






extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0000_v0_0_s_ifspec;

#ifndef __IMFMediaSession_INTERFACE_DEFINED__
#define __IMFMediaSession_INTERFACE_DEFINED__

/* interface IMFMediaSession */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFMediaSession;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("90377834-21D0-4dee-8214-BA2E3E6C1127")
    IMFMediaSession : public IMFMediaEventGenerator
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetTopology( 
            /* [in] */ DWORD dwSetTopologyFlags,
            /* [in] */ __RPC__in_opt IMFTopology *pTopology) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ClearTopologies( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Start( 
            /* [unique][in] */ __RPC__in_opt const GUID *pguidTimeFormat,
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvarStartPosition) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Pause( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Shutdown( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetClock( 
            /* [out] */ __RPC__deref_out_opt IMFClock **ppClock) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSessionCapabilities( 
            /* [out] */ __RPC__out DWORD *pdwCaps) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFullTopology( 
            /* [in] */ DWORD dwGetFullTopologyFlags,
            /* [in] */ TOPOID TopoId,
            /* [out] */ __RPC__deref_out_opt IMFTopology **ppFullTopology) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFMediaSessionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFMediaSession * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFMediaSession * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFMediaSession * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetEvent )( 
            IMFMediaSession * This,
            /* [in] */ DWORD dwFlags,
            /* [out] */ __RPC__deref_out_opt IMFMediaEvent **ppEvent);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *BeginGetEvent )( 
            IMFMediaSession * This,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *EndGetEvent )( 
            IMFMediaSession * This,
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  IMFMediaEvent **ppEvent);
        
        HRESULT ( STDMETHODCALLTYPE *QueueEvent )( 
            IMFMediaSession * This,
            /* [in] */ MediaEventType met,
            /* [in] */ __RPC__in REFGUID guidExtendedType,
            /* [in] */ HRESULT hrStatus,
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetTopology )( 
            IMFMediaSession * This,
            /* [in] */ DWORD dwSetTopologyFlags,
            /* [in] */ __RPC__in_opt IMFTopology *pTopology);
        
        HRESULT ( STDMETHODCALLTYPE *ClearTopologies )( 
            IMFMediaSession * This);
        
        HRESULT ( STDMETHODCALLTYPE *Start )( 
            IMFMediaSession * This,
            /* [unique][in] */ __RPC__in_opt const GUID *pguidTimeFormat,
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvarStartPosition);
        
        HRESULT ( STDMETHODCALLTYPE *Pause )( 
            IMFMediaSession * This);
        
        HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IMFMediaSession * This);
        
        HRESULT ( STDMETHODCALLTYPE *Close )( 
            IMFMediaSession * This);
        
        HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            IMFMediaSession * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetClock )( 
            IMFMediaSession * This,
            /* [out] */ __RPC__deref_out_opt IMFClock **ppClock);
        
        HRESULT ( STDMETHODCALLTYPE *GetSessionCapabilities )( 
            IMFMediaSession * This,
            /* [out] */ __RPC__out DWORD *pdwCaps);
        
        HRESULT ( STDMETHODCALLTYPE *GetFullTopology )( 
            IMFMediaSession * This,
            /* [in] */ DWORD dwGetFullTopologyFlags,
            /* [in] */ TOPOID TopoId,
            /* [out] */ __RPC__deref_out_opt IMFTopology **ppFullTopology);
        
        END_INTERFACE
    } IMFMediaSessionVtbl;

    interface IMFMediaSession
    {
        CONST_VTBL struct IMFMediaSessionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFMediaSession_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFMediaSession_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFMediaSession_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFMediaSession_GetEvent(This,dwFlags,ppEvent)	\
    ( (This)->lpVtbl -> GetEvent(This,dwFlags,ppEvent) ) 

#define IMFMediaSession_BeginGetEvent(This,pCallback,punkState)	\
    ( (This)->lpVtbl -> BeginGetEvent(This,pCallback,punkState) ) 

#define IMFMediaSession_EndGetEvent(This,pResult,ppEvent)	\
    ( (This)->lpVtbl -> EndGetEvent(This,pResult,ppEvent) ) 

#define IMFMediaSession_QueueEvent(This,met,guidExtendedType,hrStatus,pvValue)	\
    ( (This)->lpVtbl -> QueueEvent(This,met,guidExtendedType,hrStatus,pvValue) ) 


#define IMFMediaSession_SetTopology(This,dwSetTopologyFlags,pTopology)	\
    ( (This)->lpVtbl -> SetTopology(This,dwSetTopologyFlags,pTopology) ) 

#define IMFMediaSession_ClearTopologies(This)	\
    ( (This)->lpVtbl -> ClearTopologies(This) ) 

#define IMFMediaSession_Start(This,pguidTimeFormat,pvarStartPosition)	\
    ( (This)->lpVtbl -> Start(This,pguidTimeFormat,pvarStartPosition) ) 

#define IMFMediaSession_Pause(This)	\
    ( (This)->lpVtbl -> Pause(This) ) 

#define IMFMediaSession_Stop(This)	\
    ( (This)->lpVtbl -> Stop(This) ) 

#define IMFMediaSession_Close(This)	\
    ( (This)->lpVtbl -> Close(This) ) 

#define IMFMediaSession_Shutdown(This)	\
    ( (This)->lpVtbl -> Shutdown(This) ) 

#define IMFMediaSession_GetClock(This,ppClock)	\
    ( (This)->lpVtbl -> GetClock(This,ppClock) ) 

#define IMFMediaSession_GetSessionCapabilities(This,pdwCaps)	\
    ( (This)->lpVtbl -> GetSessionCapabilities(This,pdwCaps) ) 

#define IMFMediaSession_GetFullTopology(This,dwGetFullTopologyFlags,TopoId,ppFullTopology)	\
    ( (This)->lpVtbl -> GetFullTopology(This,dwGetFullTopologyFlags,TopoId,ppFullTopology) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFMediaSession_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0001 */
/* [local] */ 

EXTERN_GUID( MF_SESSION_TOPOLOADER, 0x1e83d482, 0x1f1c, 0x4571, 0x84, 0x5, 0x88, 0xf4, 0xb2, 0x18, 0x1f, 0x71);
EXTERN_GUID( MF_SESSION_GLOBAL_TIME, 0x1e83d482, 0x1f1c, 0x4571, 0x84, 0x5, 0x88, 0xf4, 0xb2, 0x18, 0x1f, 0x72);
EXTERN_GUID( MF_SESSION_QUALITY_MANAGER, 0x1e83d482, 0x1f1c, 0x4571, 0x84, 0x5, 0x88, 0xf4, 0xb2, 0x18, 0x1f, 0x73);
EXTERN_GUID( MF_SESSION_CONTENT_PROTECTION_MANAGER, 0x1e83d482, 0x1f1c, 0x4571, 0x84, 0x5, 0x88, 0xf4, 0xb2, 0x18, 0x1f, 0x74);
EXTERN_GUID( MF_SESSION_SERVER_CONTEXT, 0xafe5b291, 0x50fa, 0x46e8, 0xb9, 0xbe, 0xc, 0xc, 0x3c, 0xe4, 0xb3, 0xa5);
EXTERN_GUID( MF_SESSION_REMOTE_SOURCE_MODE, 0xf4033ef4, 0x9bb3, 0x4378, 0x94, 0x1f, 0x85, 0xa0, 0x85, 0x6b, 0xc2, 0x44);
EXTERN_GUID( MF_SESSION_APPROX_EVENT_OCCURRENCE_TIME, 0x190e852f, 0x6238, 0x42d1, 0xb5, 0xaf, 0x69, 0xea, 0x33, 0x8e, 0xf8, 0x50);
EXTERN_GUID( MF_PMP_SERVER_CONTEXT, 0x2f00c910, 0xd2cf, 0x4278, 0x8b, 0x6a, 0xd0, 0x77, 0xfa, 0xc3, 0xa2, 0x5f);
STDAPI MFCreateMediaSession(
    IMFAttributes* pConfiguration,
    __out IMFMediaSession** ppMediaSession
    );
STDAPI MFCreatePMPMediaSession(
    DWORD dwCreationFlags,
    IMFAttributes *pConfiguration,
    __out IMFMediaSession** ppMediaSession,
    __out IMFActivate **ppEnablerActivate
    );
typedef 
enum MF_OBJECT_TYPE
    {	MF_OBJECT_MEDIASOURCE	= 0,
	MF_OBJECT_BYTESTREAM	= ( MF_OBJECT_MEDIASOURCE + 1 ) ,
	MF_OBJECT_INVALID	= ( MF_OBJECT_BYTESTREAM + 1 ) 
    } 	MF_OBJECT_TYPE;


enum __MIDL___MIDL_itf_mfidl_0000_0001_0001
    {	MF_RESOLUTION_MEDIASOURCE	= 0x1,
	MF_RESOLUTION_BYTESTREAM	= 0x2,
	MF_RESOLUTION_CONTENT_DOES_NOT_HAVE_TO_MATCH_EXTENSION_OR_MIME_TYPE	= 0x10,
	MF_RESOLUTION_KEEP_BYTE_STREAM_ALIVE_ON_FAIL	= 0x20,
	MF_RESOLUTION_READ	= 0x10000,
	MF_RESOLUTION_WRITE	= 0x20000
    } ;
typedef 
enum _MF_CONNECT_METHOD
    {	MF_CONNECT_DIRECT	= 0,
	MF_CONNECT_ALLOW_CONVERTER	= 0x1,
	MF_CONNECT_ALLOW_DECODER	= 0x3,
	MF_CONNECT_AS_OPTIONAL	= 0x10000,
	MF_CONNECT_AS_OPTIONAL_BRANCH	= 0x20000
    } 	MF_CONNECT_METHOD;

typedef 
enum _MF_TOPOLOGY_RESOLUTION_STATUS_FLAGS
    {	MF_TOPOLOGY_RESOLUTION_SUCCEEDED	= 0,
	MF_OPTIONAL_NODE_REJECTED_MEDIA_TYPE	= 0x1,
	MF_OPTIONAL_NODE_REJECTED_PROTECTED_PROCESS	= 0x2
    } 	MF_TOPOLOGY_RESOLUTION_STATUS_FLAGS;




extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0001_v0_0_s_ifspec;

#ifndef __IMFSourceResolver_INTERFACE_DEFINED__
#define __IMFSourceResolver_INTERFACE_DEFINED__

/* interface IMFSourceResolver */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFSourceResolver;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FBE5A32D-A497-4b61-BB85-97B1A848A6E3")
    IMFSourceResolver : public IUnknown
    {
    public:
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE CreateObjectFromURL( 
            /* [in] */ LPCWSTR pwszURL,
            /* [in] */ DWORD dwFlags,
            /* [in] */ IPropertyStore *pProps,
            /* [out] */ 
            __out  MF_OBJECT_TYPE *pObjectType,
            /* [out] */ 
            __out  IUnknown **ppObject) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE CreateObjectFromByteStream( 
            /* [in] */ IMFByteStream *pByteStream,
            /* [in] */ LPCWSTR pwszURL,
            /* [in] */ DWORD dwFlags,
            /* [in] */ IPropertyStore *pProps,
            /* [out] */ 
            __out  MF_OBJECT_TYPE *pObjectType,
            /* [out] */ 
            __out  IUnknown **ppObject) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE BeginCreateObjectFromURL( 
            /* [in] */ LPCWSTR pwszURL,
            /* [in] */ DWORD dwFlags,
            /* [in] */ IPropertyStore *pProps,
            /* [out] */ 
            __out_opt  IUnknown **ppIUnknownCancelCookie,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE EndCreateObjectFromURL( 
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  MF_OBJECT_TYPE *pObjectType,
            /* [out] */ 
            __out  IUnknown **ppObject) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE BeginCreateObjectFromByteStream( 
            /* [in] */ IMFByteStream *pByteStream,
            /* [in] */ LPCWSTR pwszURL,
            /* [in] */ DWORD dwFlags,
            /* [in] */ IPropertyStore *pProps,
            /* [out] */ 
            __out_opt  IUnknown **ppIUnknownCancelCookie,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE EndCreateObjectFromByteStream( 
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  MF_OBJECT_TYPE *pObjectType,
            /* [out] */ 
            __out  IUnknown **ppObject) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE CancelObjectCreation( 
            /* [in] */ IUnknown *pIUnknownCancelCookie) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFSourceResolverVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFSourceResolver * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFSourceResolver * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFSourceResolver * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *CreateObjectFromURL )( 
            IMFSourceResolver * This,
            /* [in] */ LPCWSTR pwszURL,
            /* [in] */ DWORD dwFlags,
            /* [in] */ IPropertyStore *pProps,
            /* [out] */ 
            __out  MF_OBJECT_TYPE *pObjectType,
            /* [out] */ 
            __out  IUnknown **ppObject);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *CreateObjectFromByteStream )( 
            IMFSourceResolver * This,
            /* [in] */ IMFByteStream *pByteStream,
            /* [in] */ LPCWSTR pwszURL,
            /* [in] */ DWORD dwFlags,
            /* [in] */ IPropertyStore *pProps,
            /* [out] */ 
            __out  MF_OBJECT_TYPE *pObjectType,
            /* [out] */ 
            __out  IUnknown **ppObject);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *BeginCreateObjectFromURL )( 
            IMFSourceResolver * This,
            /* [in] */ LPCWSTR pwszURL,
            /* [in] */ DWORD dwFlags,
            /* [in] */ IPropertyStore *pProps,
            /* [out] */ 
            __out_opt  IUnknown **ppIUnknownCancelCookie,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *EndCreateObjectFromURL )( 
            IMFSourceResolver * This,
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  MF_OBJECT_TYPE *pObjectType,
            /* [out] */ 
            __out  IUnknown **ppObject);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *BeginCreateObjectFromByteStream )( 
            IMFSourceResolver * This,
            /* [in] */ IMFByteStream *pByteStream,
            /* [in] */ LPCWSTR pwszURL,
            /* [in] */ DWORD dwFlags,
            /* [in] */ IPropertyStore *pProps,
            /* [out] */ 
            __out_opt  IUnknown **ppIUnknownCancelCookie,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *EndCreateObjectFromByteStream )( 
            IMFSourceResolver * This,
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  MF_OBJECT_TYPE *pObjectType,
            /* [out] */ 
            __out  IUnknown **ppObject);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *CancelObjectCreation )( 
            IMFSourceResolver * This,
            /* [in] */ IUnknown *pIUnknownCancelCookie);
        
        END_INTERFACE
    } IMFSourceResolverVtbl;

    interface IMFSourceResolver
    {
        CONST_VTBL struct IMFSourceResolverVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFSourceResolver_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFSourceResolver_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFSourceResolver_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFSourceResolver_CreateObjectFromURL(This,pwszURL,dwFlags,pProps,pObjectType,ppObject)	\
    ( (This)->lpVtbl -> CreateObjectFromURL(This,pwszURL,dwFlags,pProps,pObjectType,ppObject) ) 

#define IMFSourceResolver_CreateObjectFromByteStream(This,pByteStream,pwszURL,dwFlags,pProps,pObjectType,ppObject)	\
    ( (This)->lpVtbl -> CreateObjectFromByteStream(This,pByteStream,pwszURL,dwFlags,pProps,pObjectType,ppObject) ) 

#define IMFSourceResolver_BeginCreateObjectFromURL(This,pwszURL,dwFlags,pProps,ppIUnknownCancelCookie,pCallback,punkState)	\
    ( (This)->lpVtbl -> BeginCreateObjectFromURL(This,pwszURL,dwFlags,pProps,ppIUnknownCancelCookie,pCallback,punkState) ) 

#define IMFSourceResolver_EndCreateObjectFromURL(This,pResult,pObjectType,ppObject)	\
    ( (This)->lpVtbl -> EndCreateObjectFromURL(This,pResult,pObjectType,ppObject) ) 

#define IMFSourceResolver_BeginCreateObjectFromByteStream(This,pByteStream,pwszURL,dwFlags,pProps,ppIUnknownCancelCookie,pCallback,punkState)	\
    ( (This)->lpVtbl -> BeginCreateObjectFromByteStream(This,pByteStream,pwszURL,dwFlags,pProps,ppIUnknownCancelCookie,pCallback,punkState) ) 

#define IMFSourceResolver_EndCreateObjectFromByteStream(This,pResult,pObjectType,ppObject)	\
    ( (This)->lpVtbl -> EndCreateObjectFromByteStream(This,pResult,pObjectType,ppObject) ) 

#define IMFSourceResolver_CancelObjectCreation(This,pIUnknownCancelCookie)	\
    ( (This)->lpVtbl -> CancelObjectCreation(This,pIUnknownCancelCookie) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFSourceResolver_RemoteBeginCreateObjectFromURL_Proxy( 
    IMFSourceResolver * This,
    /* [string][in] */ __RPC__in LPCWSTR pwszURL,
    /* [in] */ DWORD dwFlags,
    /* [in] */ __RPC__in_opt IMFRemoteAsyncCallback *pCallback);


void __RPC_STUB IMFSourceResolver_RemoteBeginCreateObjectFromURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFSourceResolver_RemoteEndCreateObjectFromURL_Proxy( 
    IMFSourceResolver * This,
    /* [in] */ __RPC__in_opt IUnknown *pResult,
    /* [out] */ __RPC__out MF_OBJECT_TYPE *pObjectType,
    /* [out] */ __RPC__deref_out_opt IUnknown **ppObject);


void __RPC_STUB IMFSourceResolver_RemoteEndCreateObjectFromURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFSourceResolver_RemoteBeginCreateObjectFromByteStream_Proxy( 
    IMFSourceResolver * This,
    /* [in] */ __RPC__in_opt IMFByteStream *pByteStream,
    /* [in] */ __RPC__in LPCWSTR pwszURL,
    /* [in] */ DWORD dwFlags,
    /* [in] */ __RPC__in_opt IMFRemoteAsyncCallback *pCallback);


void __RPC_STUB IMFSourceResolver_RemoteBeginCreateObjectFromByteStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFSourceResolver_RemoteEndCreateObjectFromByteStream_Proxy( 
    IMFSourceResolver * This,
    /* [in] */ __RPC__in_opt IUnknown *pResult,
    /* [out] */ __RPC__out MF_OBJECT_TYPE *pObjectType,
    /* [out] */ __RPC__deref_out_opt IUnknown **ppObject);


void __RPC_STUB IMFSourceResolver_RemoteEndCreateObjectFromByteStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMFSourceResolver_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0002 */
/* [local] */ 

STDAPI MFCreateSourceResolver( 
        /* out */     __out IMFSourceResolver     **ppISourceResolver);
STDAPI CreatePropertyStore( 
        /* out */     __out IPropertyStore        **ppStore);
STDAPI MFGetSupportedSchemes(                      
       __out PROPVARIANT* pPropVarSchemeArray  );  
STDAPI MFGetSupportedMimeTypes(                      
       __out PROPVARIANT* pPropVarMimeTypeArray  );  
static const PROPERTYKEY MFPKEY_SourceOpenMonitor = { { 0x074d4637, 0xb5ae, 0x465d, { 0xaf, 0x17, 0x1a, 0x53, 0x8d, 0x28, 0x59, 0xdd } }, 0x02 } ; 
static const PROPERTYKEY MFPKEY_ASFMediaSource_ApproxSeek = { { 0xb4cd270f, 0x244d, 0x4969, { 0xbb, 0x92, 0x3f, 0x0f, 0xb8, 0x31, 0x6f, 0x10 } }, 0x01 } ; 
typedef 
enum _MFMEDIASOURCE_CHARACTERISTICS
    {	MFMEDIASOURCE_IS_LIVE	= 0x1,
	MFMEDIASOURCE_CAN_SEEK	= 0x2,
	MFMEDIASOURCE_CAN_PAUSE	= 0x4,
	MFMEDIASOURCE_HAS_SLOW_SEEK	= 0x8
    } 	MFMEDIASOURCE_CHARACTERISTICS;





extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0002_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0002_v0_0_s_ifspec;

#ifndef __IMFMediaSource_INTERFACE_DEFINED__
#define __IMFMediaSource_INTERFACE_DEFINED__

/* interface IMFMediaSource */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFMediaSource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("279a808d-aec7-40c8-9c6b-a6b492c78a66")
    IMFMediaSource : public IMFMediaEventGenerator
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetCharacteristics( 
            /* [out] */ __RPC__out DWORD *pdwCharacteristics) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE CreatePresentationDescriptor( 
            /* [out] */ 
            __out  IMFPresentationDescriptor **ppPresentationDescriptor) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Start( 
            /* [in] */ __RPC__in_opt IMFPresentationDescriptor *pPresentationDescriptor,
            /* [unique][in] */ __RPC__in_opt const GUID *pguidTimeFormat,
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvarStartPosition) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Pause( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Shutdown( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFMediaSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFMediaSource * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFMediaSource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFMediaSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetEvent )( 
            IMFMediaSource * This,
            /* [in] */ DWORD dwFlags,
            /* [out] */ __RPC__deref_out_opt IMFMediaEvent **ppEvent);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *BeginGetEvent )( 
            IMFMediaSource * This,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *EndGetEvent )( 
            IMFMediaSource * This,
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  IMFMediaEvent **ppEvent);
        
        HRESULT ( STDMETHODCALLTYPE *QueueEvent )( 
            IMFMediaSource * This,
            /* [in] */ MediaEventType met,
            /* [in] */ __RPC__in REFGUID guidExtendedType,
            /* [in] */ HRESULT hrStatus,
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetCharacteristics )( 
            IMFMediaSource * This,
            /* [out] */ __RPC__out DWORD *pdwCharacteristics);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *CreatePresentationDescriptor )( 
            IMFMediaSource * This,
            /* [out] */ 
            __out  IMFPresentationDescriptor **ppPresentationDescriptor);
        
        HRESULT ( STDMETHODCALLTYPE *Start )( 
            IMFMediaSource * This,
            /* [in] */ __RPC__in_opt IMFPresentationDescriptor *pPresentationDescriptor,
            /* [unique][in] */ __RPC__in_opt const GUID *pguidTimeFormat,
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvarStartPosition);
        
        HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IMFMediaSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *Pause )( 
            IMFMediaSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            IMFMediaSource * This);
        
        END_INTERFACE
    } IMFMediaSourceVtbl;

    interface IMFMediaSource
    {
        CONST_VTBL struct IMFMediaSourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFMediaSource_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFMediaSource_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFMediaSource_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFMediaSource_GetEvent(This,dwFlags,ppEvent)	\
    ( (This)->lpVtbl -> GetEvent(This,dwFlags,ppEvent) ) 

#define IMFMediaSource_BeginGetEvent(This,pCallback,punkState)	\
    ( (This)->lpVtbl -> BeginGetEvent(This,pCallback,punkState) ) 

#define IMFMediaSource_EndGetEvent(This,pResult,ppEvent)	\
    ( (This)->lpVtbl -> EndGetEvent(This,pResult,ppEvent) ) 

#define IMFMediaSource_QueueEvent(This,met,guidExtendedType,hrStatus,pvValue)	\
    ( (This)->lpVtbl -> QueueEvent(This,met,guidExtendedType,hrStatus,pvValue) ) 


#define IMFMediaSource_GetCharacteristics(This,pdwCharacteristics)	\
    ( (This)->lpVtbl -> GetCharacteristics(This,pdwCharacteristics) ) 

#define IMFMediaSource_CreatePresentationDescriptor(This,ppPresentationDescriptor)	\
    ( (This)->lpVtbl -> CreatePresentationDescriptor(This,ppPresentationDescriptor) ) 

#define IMFMediaSource_Start(This,pPresentationDescriptor,pguidTimeFormat,pvarStartPosition)	\
    ( (This)->lpVtbl -> Start(This,pPresentationDescriptor,pguidTimeFormat,pvarStartPosition) ) 

#define IMFMediaSource_Stop(This)	\
    ( (This)->lpVtbl -> Stop(This) ) 

#define IMFMediaSource_Pause(This)	\
    ( (This)->lpVtbl -> Pause(This) ) 

#define IMFMediaSource_Shutdown(This)	\
    ( (This)->lpVtbl -> Shutdown(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFMediaSource_RemoteCreatePresentationDescriptor_Proxy( 
    IMFMediaSource * This,
    /* [out] */ __RPC__out DWORD *pcbPD,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbPD) BYTE **pbPD,
    /* [out] */ __RPC__deref_out_opt IMFPresentationDescriptor **ppRemotePD);


void __RPC_STUB IMFMediaSource_RemoteCreatePresentationDescriptor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMFMediaSource_INTERFACE_DEFINED__ */


#ifndef __IMFMediaStream_INTERFACE_DEFINED__
#define __IMFMediaStream_INTERFACE_DEFINED__

/* interface IMFMediaStream */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFMediaStream;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D182108F-4EC6-443f-AA42-A71106EC825F")
    IMFMediaStream : public IMFMediaEventGenerator
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetMediaSource( 
            /* [out] */ __RPC__deref_out_opt IMFMediaSource **ppMediaSource) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStreamDescriptor( 
            /* [out] */ __RPC__deref_out_opt IMFStreamDescriptor **ppStreamDescriptor) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE RequestSample( 
            /* [in] */ IUnknown *pToken) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFMediaStreamVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFMediaStream * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFMediaStream * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFMediaStream * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetEvent )( 
            IMFMediaStream * This,
            /* [in] */ DWORD dwFlags,
            /* [out] */ __RPC__deref_out_opt IMFMediaEvent **ppEvent);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *BeginGetEvent )( 
            IMFMediaStream * This,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *EndGetEvent )( 
            IMFMediaStream * This,
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  IMFMediaEvent **ppEvent);
        
        HRESULT ( STDMETHODCALLTYPE *QueueEvent )( 
            IMFMediaStream * This,
            /* [in] */ MediaEventType met,
            /* [in] */ __RPC__in REFGUID guidExtendedType,
            /* [in] */ HRESULT hrStatus,
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetMediaSource )( 
            IMFMediaStream * This,
            /* [out] */ __RPC__deref_out_opt IMFMediaSource **ppMediaSource);
        
        HRESULT ( STDMETHODCALLTYPE *GetStreamDescriptor )( 
            IMFMediaStream * This,
            /* [out] */ __RPC__deref_out_opt IMFStreamDescriptor **ppStreamDescriptor);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *RequestSample )( 
            IMFMediaStream * This,
            /* [in] */ IUnknown *pToken);
        
        END_INTERFACE
    } IMFMediaStreamVtbl;

    interface IMFMediaStream
    {
        CONST_VTBL struct IMFMediaStreamVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFMediaStream_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFMediaStream_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFMediaStream_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFMediaStream_GetEvent(This,dwFlags,ppEvent)	\
    ( (This)->lpVtbl -> GetEvent(This,dwFlags,ppEvent) ) 

#define IMFMediaStream_BeginGetEvent(This,pCallback,punkState)	\
    ( (This)->lpVtbl -> BeginGetEvent(This,pCallback,punkState) ) 

#define IMFMediaStream_EndGetEvent(This,pResult,ppEvent)	\
    ( (This)->lpVtbl -> EndGetEvent(This,pResult,ppEvent) ) 

#define IMFMediaStream_QueueEvent(This,met,guidExtendedType,hrStatus,pvValue)	\
    ( (This)->lpVtbl -> QueueEvent(This,met,guidExtendedType,hrStatus,pvValue) ) 


#define IMFMediaStream_GetMediaSource(This,ppMediaSource)	\
    ( (This)->lpVtbl -> GetMediaSource(This,ppMediaSource) ) 

#define IMFMediaStream_GetStreamDescriptor(This,ppStreamDescriptor)	\
    ( (This)->lpVtbl -> GetStreamDescriptor(This,ppStreamDescriptor) ) 

#define IMFMediaStream_RequestSample(This,pToken)	\
    ( (This)->lpVtbl -> RequestSample(This,pToken) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFMediaStream_RemoteRequestSample_Proxy( 
    IMFMediaStream * This);


void __RPC_STUB IMFMediaStream_RemoteRequestSample_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMFMediaStream_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0004 */
/* [local] */ 

#define MEDIASINK_FIXED_STREAMS         0x00000001
#define MEDIASINK_CANNOT_MATCH_CLOCK    0x00000002
#define MEDIASINK_RATELESS              0x00000004
#define MEDIASINK_CLOCK_REQUIRED        0x00000008
#define MEDIASINK_CAN_PREROLL           0x00000010
#define MEDIASINK_REQUIRE_REFERENCE_MEDIATYPE 0x00000020




extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0004_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0004_v0_0_s_ifspec;

#ifndef __IMFMediaSink_INTERFACE_DEFINED__
#define __IMFMediaSink_INTERFACE_DEFINED__

/* interface IMFMediaSink */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFMediaSink;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6ef2a660-47c0-4666-b13d-cbb717f2fa2c")
    IMFMediaSink : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetCharacteristics( 
            /* [out] */ __RPC__out DWORD *pdwCharacteristics) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddStreamSink( 
            /* [in] */ DWORD dwStreamSinkIdentifier,
            /* [in] */ __RPC__in_opt IMFMediaType *pMediaType,
            /* [out] */ __RPC__deref_out_opt IMFStreamSink **ppStreamSink) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveStreamSink( 
            /* [in] */ DWORD dwStreamSinkIdentifier) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStreamSinkCount( 
            /* [out] */ __RPC__out DWORD *pcStreamSinkCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStreamSinkByIndex( 
            /* [in] */ DWORD dwIndex,
            /* [out] */ __RPC__deref_out_opt IMFStreamSink **ppStreamSink) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStreamSinkById( 
            /* [in] */ DWORD dwStreamSinkIdentifier,
            /* [out] */ __RPC__deref_out_opt IMFStreamSink **ppStreamSink) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetPresentationClock( 
            /* [in] */ __RPC__in_opt IMFPresentationClock *pPresentationClock) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPresentationClock( 
            /* [out] */ __RPC__deref_out_opt IMFPresentationClock **ppPresentationClock) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Shutdown( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFMediaSinkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFMediaSink * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFMediaSink * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFMediaSink * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCharacteristics )( 
            IMFMediaSink * This,
            /* [out] */ __RPC__out DWORD *pdwCharacteristics);
        
        HRESULT ( STDMETHODCALLTYPE *AddStreamSink )( 
            IMFMediaSink * This,
            /* [in] */ DWORD dwStreamSinkIdentifier,
            /* [in] */ __RPC__in_opt IMFMediaType *pMediaType,
            /* [out] */ __RPC__deref_out_opt IMFStreamSink **ppStreamSink);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveStreamSink )( 
            IMFMediaSink * This,
            /* [in] */ DWORD dwStreamSinkIdentifier);
        
        HRESULT ( STDMETHODCALLTYPE *GetStreamSinkCount )( 
            IMFMediaSink * This,
            /* [out] */ __RPC__out DWORD *pcStreamSinkCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetStreamSinkByIndex )( 
            IMFMediaSink * This,
            /* [in] */ DWORD dwIndex,
            /* [out] */ __RPC__deref_out_opt IMFStreamSink **ppStreamSink);
        
        HRESULT ( STDMETHODCALLTYPE *GetStreamSinkById )( 
            IMFMediaSink * This,
            /* [in] */ DWORD dwStreamSinkIdentifier,
            /* [out] */ __RPC__deref_out_opt IMFStreamSink **ppStreamSink);
        
        HRESULT ( STDMETHODCALLTYPE *SetPresentationClock )( 
            IMFMediaSink * This,
            /* [in] */ __RPC__in_opt IMFPresentationClock *pPresentationClock);
        
        HRESULT ( STDMETHODCALLTYPE *GetPresentationClock )( 
            IMFMediaSink * This,
            /* [out] */ __RPC__deref_out_opt IMFPresentationClock **ppPresentationClock);
        
        HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            IMFMediaSink * This);
        
        END_INTERFACE
    } IMFMediaSinkVtbl;

    interface IMFMediaSink
    {
        CONST_VTBL struct IMFMediaSinkVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFMediaSink_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFMediaSink_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFMediaSink_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFMediaSink_GetCharacteristics(This,pdwCharacteristics)	\
    ( (This)->lpVtbl -> GetCharacteristics(This,pdwCharacteristics) ) 

#define IMFMediaSink_AddStreamSink(This,dwStreamSinkIdentifier,pMediaType,ppStreamSink)	\
    ( (This)->lpVtbl -> AddStreamSink(This,dwStreamSinkIdentifier,pMediaType,ppStreamSink) ) 

#define IMFMediaSink_RemoveStreamSink(This,dwStreamSinkIdentifier)	\
    ( (This)->lpVtbl -> RemoveStreamSink(This,dwStreamSinkIdentifier) ) 

#define IMFMediaSink_GetStreamSinkCount(This,pcStreamSinkCount)	\
    ( (This)->lpVtbl -> GetStreamSinkCount(This,pcStreamSinkCount) ) 

#define IMFMediaSink_GetStreamSinkByIndex(This,dwIndex,ppStreamSink)	\
    ( (This)->lpVtbl -> GetStreamSinkByIndex(This,dwIndex,ppStreamSink) ) 

#define IMFMediaSink_GetStreamSinkById(This,dwStreamSinkIdentifier,ppStreamSink)	\
    ( (This)->lpVtbl -> GetStreamSinkById(This,dwStreamSinkIdentifier,ppStreamSink) ) 

#define IMFMediaSink_SetPresentationClock(This,pPresentationClock)	\
    ( (This)->lpVtbl -> SetPresentationClock(This,pPresentationClock) ) 

#define IMFMediaSink_GetPresentationClock(This,ppPresentationClock)	\
    ( (This)->lpVtbl -> GetPresentationClock(This,ppPresentationClock) ) 

#define IMFMediaSink_Shutdown(This)	\
    ( (This)->lpVtbl -> Shutdown(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFMediaSink_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0005 */
/* [local] */ 

typedef 
enum _MFSTREAMSINK_MARKER_TYPE
    {	MFSTREAMSINK_MARKER_DEFAULT	= 0,
	MFSTREAMSINK_MARKER_ENDOFSEGMENT	= ( MFSTREAMSINK_MARKER_DEFAULT + 1 ) ,
	MFSTREAMSINK_MARKER_TICK	= ( MFSTREAMSINK_MARKER_ENDOFSEGMENT + 1 ) ,
	MFSTREAMSINK_MARKER_EVENT	= ( MFSTREAMSINK_MARKER_TICK + 1 ) 
    } 	MFSTREAMSINK_MARKER_TYPE;




extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0005_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0005_v0_0_s_ifspec;

#ifndef __IMFStreamSink_INTERFACE_DEFINED__
#define __IMFStreamSink_INTERFACE_DEFINED__

/* interface IMFStreamSink */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFStreamSink;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0A97B3CF-8E7C-4a3d-8F8C-0C843DC247FB")
    IMFStreamSink : public IMFMediaEventGenerator
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetMediaSink( 
            /* [out] */ __RPC__deref_out_opt IMFMediaSink **ppMediaSink) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIdentifier( 
            /* [out] */ __RPC__out DWORD *pdwIdentifier) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMediaTypeHandler( 
            /* [out] */ __RPC__deref_out_opt IMFMediaTypeHandler **ppHandler) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ProcessSample( 
            /* [in] */ __RPC__in_opt IMFSample *pSample) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PlaceMarker( 
            /* [in] */ MFSTREAMSINK_MARKER_TYPE eMarkerType,
            /* [in] */ __RPC__in const PROPVARIANT *pvarMarkerValue,
            /* [in] */ __RPC__in const PROPVARIANT *pvarContextValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Flush( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFStreamSinkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFStreamSink * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFStreamSink * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFStreamSink * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetEvent )( 
            IMFStreamSink * This,
            /* [in] */ DWORD dwFlags,
            /* [out] */ __RPC__deref_out_opt IMFMediaEvent **ppEvent);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *BeginGetEvent )( 
            IMFStreamSink * This,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *EndGetEvent )( 
            IMFStreamSink * This,
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  IMFMediaEvent **ppEvent);
        
        HRESULT ( STDMETHODCALLTYPE *QueueEvent )( 
            IMFStreamSink * This,
            /* [in] */ MediaEventType met,
            /* [in] */ __RPC__in REFGUID guidExtendedType,
            /* [in] */ HRESULT hrStatus,
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetMediaSink )( 
            IMFStreamSink * This,
            /* [out] */ __RPC__deref_out_opt IMFMediaSink **ppMediaSink);
        
        HRESULT ( STDMETHODCALLTYPE *GetIdentifier )( 
            IMFStreamSink * This,
            /* [out] */ __RPC__out DWORD *pdwIdentifier);
        
        HRESULT ( STDMETHODCALLTYPE *GetMediaTypeHandler )( 
            IMFStreamSink * This,
            /* [out] */ __RPC__deref_out_opt IMFMediaTypeHandler **ppHandler);
        
        HRESULT ( STDMETHODCALLTYPE *ProcessSample )( 
            IMFStreamSink * This,
            /* [in] */ __RPC__in_opt IMFSample *pSample);
        
        HRESULT ( STDMETHODCALLTYPE *PlaceMarker )( 
            IMFStreamSink * This,
            /* [in] */ MFSTREAMSINK_MARKER_TYPE eMarkerType,
            /* [in] */ __RPC__in const PROPVARIANT *pvarMarkerValue,
            /* [in] */ __RPC__in const PROPVARIANT *pvarContextValue);
        
        HRESULT ( STDMETHODCALLTYPE *Flush )( 
            IMFStreamSink * This);
        
        END_INTERFACE
    } IMFStreamSinkVtbl;

    interface IMFStreamSink
    {
        CONST_VTBL struct IMFStreamSinkVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFStreamSink_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFStreamSink_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFStreamSink_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFStreamSink_GetEvent(This,dwFlags,ppEvent)	\
    ( (This)->lpVtbl -> GetEvent(This,dwFlags,ppEvent) ) 

#define IMFStreamSink_BeginGetEvent(This,pCallback,punkState)	\
    ( (This)->lpVtbl -> BeginGetEvent(This,pCallback,punkState) ) 

#define IMFStreamSink_EndGetEvent(This,pResult,ppEvent)	\
    ( (This)->lpVtbl -> EndGetEvent(This,pResult,ppEvent) ) 

#define IMFStreamSink_QueueEvent(This,met,guidExtendedType,hrStatus,pvValue)	\
    ( (This)->lpVtbl -> QueueEvent(This,met,guidExtendedType,hrStatus,pvValue) ) 


#define IMFStreamSink_GetMediaSink(This,ppMediaSink)	\
    ( (This)->lpVtbl -> GetMediaSink(This,ppMediaSink) ) 

#define IMFStreamSink_GetIdentifier(This,pdwIdentifier)	\
    ( (This)->lpVtbl -> GetIdentifier(This,pdwIdentifier) ) 

#define IMFStreamSink_GetMediaTypeHandler(This,ppHandler)	\
    ( (This)->lpVtbl -> GetMediaTypeHandler(This,ppHandler) ) 

#define IMFStreamSink_ProcessSample(This,pSample)	\
    ( (This)->lpVtbl -> ProcessSample(This,pSample) ) 

#define IMFStreamSink_PlaceMarker(This,eMarkerType,pvarMarkerValue,pvarContextValue)	\
    ( (This)->lpVtbl -> PlaceMarker(This,eMarkerType,pvarMarkerValue,pvarContextValue) ) 

#define IMFStreamSink_Flush(This)	\
    ( (This)->lpVtbl -> Flush(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFStreamSink_INTERFACE_DEFINED__ */


#ifndef __IMFVideoSampleAllocator_INTERFACE_DEFINED__
#define __IMFVideoSampleAllocator_INTERFACE_DEFINED__

/* interface IMFVideoSampleAllocator */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFVideoSampleAllocator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("86cbc910-e533-4751-8e3b-f19b5b806a03")
    IMFVideoSampleAllocator : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetDirectXManager( 
            /* [unique][in] */ IUnknown *pManager) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UninitializeSampleAllocator( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InitializeSampleAllocator( 
            /* [in] */ DWORD cRequestedFrames,
            /* [in] */ IMFMediaType *pMediaType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AllocateSample( 
            /* [out] */ IMFSample **ppSample) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFVideoSampleAllocatorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFVideoSampleAllocator * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFVideoSampleAllocator * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFVideoSampleAllocator * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetDirectXManager )( 
            IMFVideoSampleAllocator * This,
            /* [unique][in] */ IUnknown *pManager);
        
        HRESULT ( STDMETHODCALLTYPE *UninitializeSampleAllocator )( 
            IMFVideoSampleAllocator * This);
        
        HRESULT ( STDMETHODCALLTYPE *InitializeSampleAllocator )( 
            IMFVideoSampleAllocator * This,
            /* [in] */ DWORD cRequestedFrames,
            /* [in] */ IMFMediaType *pMediaType);
        
        HRESULT ( STDMETHODCALLTYPE *AllocateSample )( 
            IMFVideoSampleAllocator * This,
            /* [out] */ IMFSample **ppSample);
        
        END_INTERFACE
    } IMFVideoSampleAllocatorVtbl;

    interface IMFVideoSampleAllocator
    {
        CONST_VTBL struct IMFVideoSampleAllocatorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFVideoSampleAllocator_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFVideoSampleAllocator_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFVideoSampleAllocator_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFVideoSampleAllocator_SetDirectXManager(This,pManager)	\
    ( (This)->lpVtbl -> SetDirectXManager(This,pManager) ) 

#define IMFVideoSampleAllocator_UninitializeSampleAllocator(This)	\
    ( (This)->lpVtbl -> UninitializeSampleAllocator(This) ) 

#define IMFVideoSampleAllocator_InitializeSampleAllocator(This,cRequestedFrames,pMediaType)	\
    ( (This)->lpVtbl -> InitializeSampleAllocator(This,cRequestedFrames,pMediaType) ) 

#define IMFVideoSampleAllocator_AllocateSample(This,ppSample)	\
    ( (This)->lpVtbl -> AllocateSample(This,ppSample) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFVideoSampleAllocator_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0007 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0007_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0007_v0_0_s_ifspec;

#ifndef __IMFTopology_INTERFACE_DEFINED__
#define __IMFTopology_INTERFACE_DEFINED__

/* interface IMFTopology */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFTopology;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("83CF873A-F6DA-4bc8-823F-BACFD55DC433")
    IMFTopology : public IMFAttributes
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetTopologyID( 
            /* [out] */ __RPC__out TOPOID *pID) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE AddNode( 
            /* [in] */ IMFTopologyNode *pNode) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE RemoveNode( 
            /* [in] */ IMFTopologyNode *pNode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNodeCount( 
            /* [out] */ __RPC__out WORD *pwNodes) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNode( 
            /* [in] */ WORD wIndex,
            /* [out] */ __RPC__deref_out_opt IMFTopologyNode **ppNode) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Clear( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CloneFrom( 
            /* [in] */ __RPC__in_opt IMFTopology *pTopology) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNodeByID( 
            /* [in] */ TOPOID qwTopoNodeID,
            /* [out] */ __RPC__deref_out_opt IMFTopologyNode **ppNode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSourceNodeCollection( 
            /* [out] */ __RPC__deref_out_opt IMFCollection **ppCollection) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetOutputNodeCollection( 
            /* [out] */ __RPC__deref_out_opt IMFCollection **ppCollection) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFTopologyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFTopology * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFTopology * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFTopology * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetItem )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            /* [full][out][in] */ __RPC__inout_opt PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemType )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out MF_ATTRIBUTE_TYPE *pType);
        
        HRESULT ( STDMETHODCALLTYPE *CompareItem )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFPROPVARIANT Value,
            /* [out] */ __RPC__out BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *Compare )( 
            IMFTopology * This,
            __RPC__in_opt IMFAttributes *pTheirs,
            MF_ATTRIBUTES_MATCH_TYPE MatchType,
            /* [out] */ __RPC__out BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT32 )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT64 )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT64 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetDouble )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out double *pfValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetGUID )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out GUID *pguidValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetStringLength )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetString )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][out] */ __RPC__out_ecount_full(cchBufSize) LPWSTR pwszValue,
            UINT32 cchBufSize,
            /* [full][out][in] */ __RPC__inout_opt UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedString )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(( *pcchLength + 1 ) ) LPWSTR *ppwszValue,
            /* [out] */ __RPC__out UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlobSize )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlob )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][out] */ __RPC__out_ecount_full(cbBufSize) UINT8 *pBuf,
            UINT32 cbBufSize,
            /* [full][out][in] */ __RPC__inout_opt UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedBlob )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbSize) UINT8 **ppBuf,
            /* [out] */ __RPC__out UINT32 *pcbSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetUnknown )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFIID riid,
            /* [iid_is][out] */ __RPC__deref_out_opt LPVOID *ppv);
        
        HRESULT ( STDMETHODCALLTYPE *SetItem )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFPROPVARIANT Value);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteItem )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteAllItems )( 
            IMFTopology * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT32 )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            UINT32 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT64 )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            UINT64 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetDouble )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            double fValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetGUID )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFGUID guidValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetString )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            /* [string][in] */ __RPC__in LPCWSTR wszValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetBlob )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][in] */ __RPC__in_ecount_full(cbBufSize) const UINT8 *pBuf,
            UINT32 cbBufSize);
        
        HRESULT ( STDMETHODCALLTYPE *SetUnknown )( 
            IMFTopology * This,
            __RPC__in REFGUID guidKey,
            /* [in] */ __RPC__in_opt IUnknown *pUnknown);
        
        HRESULT ( STDMETHODCALLTYPE *LockStore )( 
            IMFTopology * This);
        
        HRESULT ( STDMETHODCALLTYPE *UnlockStore )( 
            IMFTopology * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IMFTopology * This,
            /* [out] */ __RPC__out UINT32 *pcItems);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemByIndex )( 
            IMFTopology * This,
            UINT32 unIndex,
            /* [out] */ __RPC__out GUID *pguidKey,
            /* [full][out][in] */ __RPC__inout_opt PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *CopyAllItems )( 
            IMFTopology * This,
            /* [in] */ __RPC__in_opt IMFAttributes *pDest);
        
        HRESULT ( STDMETHODCALLTYPE *GetTopologyID )( 
            IMFTopology * This,
            /* [out] */ __RPC__out TOPOID *pID);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *AddNode )( 
            IMFTopology * This,
            /* [in] */ IMFTopologyNode *pNode);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *RemoveNode )( 
            IMFTopology * This,
            /* [in] */ IMFTopologyNode *pNode);
        
        HRESULT ( STDMETHODCALLTYPE *GetNodeCount )( 
            IMFTopology * This,
            /* [out] */ __RPC__out WORD *pwNodes);
        
        HRESULT ( STDMETHODCALLTYPE *GetNode )( 
            IMFTopology * This,
            /* [in] */ WORD wIndex,
            /* [out] */ __RPC__deref_out_opt IMFTopologyNode **ppNode);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Clear )( 
            IMFTopology * This);
        
        HRESULT ( STDMETHODCALLTYPE *CloneFrom )( 
            IMFTopology * This,
            /* [in] */ __RPC__in_opt IMFTopology *pTopology);
        
        HRESULT ( STDMETHODCALLTYPE *GetNodeByID )( 
            IMFTopology * This,
            /* [in] */ TOPOID qwTopoNodeID,
            /* [out] */ __RPC__deref_out_opt IMFTopologyNode **ppNode);
        
        HRESULT ( STDMETHODCALLTYPE *GetSourceNodeCollection )( 
            IMFTopology * This,
            /* [out] */ __RPC__deref_out_opt IMFCollection **ppCollection);
        
        HRESULT ( STDMETHODCALLTYPE *GetOutputNodeCollection )( 
            IMFTopology * This,
            /* [out] */ __RPC__deref_out_opt IMFCollection **ppCollection);
        
        END_INTERFACE
    } IMFTopologyVtbl;

    interface IMFTopology
    {
        CONST_VTBL struct IMFTopologyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFTopology_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFTopology_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFTopology_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFTopology_GetItem(This,guidKey,pValue)	\
    ( (This)->lpVtbl -> GetItem(This,guidKey,pValue) ) 

#define IMFTopology_GetItemType(This,guidKey,pType)	\
    ( (This)->lpVtbl -> GetItemType(This,guidKey,pType) ) 

#define IMFTopology_CompareItem(This,guidKey,Value,pbResult)	\
    ( (This)->lpVtbl -> CompareItem(This,guidKey,Value,pbResult) ) 

#define IMFTopology_Compare(This,pTheirs,MatchType,pbResult)	\
    ( (This)->lpVtbl -> Compare(This,pTheirs,MatchType,pbResult) ) 

#define IMFTopology_GetUINT32(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT32(This,guidKey,punValue) ) 

#define IMFTopology_GetUINT64(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT64(This,guidKey,punValue) ) 

#define IMFTopology_GetDouble(This,guidKey,pfValue)	\
    ( (This)->lpVtbl -> GetDouble(This,guidKey,pfValue) ) 

#define IMFTopology_GetGUID(This,guidKey,pguidValue)	\
    ( (This)->lpVtbl -> GetGUID(This,guidKey,pguidValue) ) 

#define IMFTopology_GetStringLength(This,guidKey,pcchLength)	\
    ( (This)->lpVtbl -> GetStringLength(This,guidKey,pcchLength) ) 

#define IMFTopology_GetString(This,guidKey,pwszValue,cchBufSize,pcchLength)	\
    ( (This)->lpVtbl -> GetString(This,guidKey,pwszValue,cchBufSize,pcchLength) ) 

#define IMFTopology_GetAllocatedString(This,guidKey,ppwszValue,pcchLength)	\
    ( (This)->lpVtbl -> GetAllocatedString(This,guidKey,ppwszValue,pcchLength) ) 

#define IMFTopology_GetBlobSize(This,guidKey,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlobSize(This,guidKey,pcbBlobSize) ) 

#define IMFTopology_GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize) ) 

#define IMFTopology_GetAllocatedBlob(This,guidKey,ppBuf,pcbSize)	\
    ( (This)->lpVtbl -> GetAllocatedBlob(This,guidKey,ppBuf,pcbSize) ) 

#define IMFTopology_GetUnknown(This,guidKey,riid,ppv)	\
    ( (This)->lpVtbl -> GetUnknown(This,guidKey,riid,ppv) ) 

#define IMFTopology_SetItem(This,guidKey,Value)	\
    ( (This)->lpVtbl -> SetItem(This,guidKey,Value) ) 

#define IMFTopology_DeleteItem(This,guidKey)	\
    ( (This)->lpVtbl -> DeleteItem(This,guidKey) ) 

#define IMFTopology_DeleteAllItems(This)	\
    ( (This)->lpVtbl -> DeleteAllItems(This) ) 

#define IMFTopology_SetUINT32(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT32(This,guidKey,unValue) ) 

#define IMFTopology_SetUINT64(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT64(This,guidKey,unValue) ) 

#define IMFTopology_SetDouble(This,guidKey,fValue)	\
    ( (This)->lpVtbl -> SetDouble(This,guidKey,fValue) ) 

#define IMFTopology_SetGUID(This,guidKey,guidValue)	\
    ( (This)->lpVtbl -> SetGUID(This,guidKey,guidValue) ) 

#define IMFTopology_SetString(This,guidKey,wszValue)	\
    ( (This)->lpVtbl -> SetString(This,guidKey,wszValue) ) 

#define IMFTopology_SetBlob(This,guidKey,pBuf,cbBufSize)	\
    ( (This)->lpVtbl -> SetBlob(This,guidKey,pBuf,cbBufSize) ) 

#define IMFTopology_SetUnknown(This,guidKey,pUnknown)	\
    ( (This)->lpVtbl -> SetUnknown(This,guidKey,pUnknown) ) 

#define IMFTopology_LockStore(This)	\
    ( (This)->lpVtbl -> LockStore(This) ) 

#define IMFTopology_UnlockStore(This)	\
    ( (This)->lpVtbl -> UnlockStore(This) ) 

#define IMFTopology_GetCount(This,pcItems)	\
    ( (This)->lpVtbl -> GetCount(This,pcItems) ) 

#define IMFTopology_GetItemByIndex(This,unIndex,pguidKey,pValue)	\
    ( (This)->lpVtbl -> GetItemByIndex(This,unIndex,pguidKey,pValue) ) 

#define IMFTopology_CopyAllItems(This,pDest)	\
    ( (This)->lpVtbl -> CopyAllItems(This,pDest) ) 


#define IMFTopology_GetTopologyID(This,pID)	\
    ( (This)->lpVtbl -> GetTopologyID(This,pID) ) 

#define IMFTopology_AddNode(This,pNode)	\
    ( (This)->lpVtbl -> AddNode(This,pNode) ) 

#define IMFTopology_RemoveNode(This,pNode)	\
    ( (This)->lpVtbl -> RemoveNode(This,pNode) ) 

#define IMFTopology_GetNodeCount(This,pwNodes)	\
    ( (This)->lpVtbl -> GetNodeCount(This,pwNodes) ) 

#define IMFTopology_GetNode(This,wIndex,ppNode)	\
    ( (This)->lpVtbl -> GetNode(This,wIndex,ppNode) ) 

#define IMFTopology_Clear(This)	\
    ( (This)->lpVtbl -> Clear(This) ) 

#define IMFTopology_CloneFrom(This,pTopology)	\
    ( (This)->lpVtbl -> CloneFrom(This,pTopology) ) 

#define IMFTopology_GetNodeByID(This,qwTopoNodeID,ppNode)	\
    ( (This)->lpVtbl -> GetNodeByID(This,qwTopoNodeID,ppNode) ) 

#define IMFTopology_GetSourceNodeCollection(This,ppCollection)	\
    ( (This)->lpVtbl -> GetSourceNodeCollection(This,ppCollection) ) 

#define IMFTopology_GetOutputNodeCollection(This,ppCollection)	\
    ( (This)->lpVtbl -> GetOutputNodeCollection(This,ppCollection) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFTopology_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0008 */
/* [local] */ 

EXTERN_GUID( MF_TOPOLOGY_PROJECTSTART, 0x7ed3f802, 0x86bb, 0x4b3f, 0xb7, 0xe4, 0x7c, 0xb4, 0x3a, 0xfd, 0x4b, 0x80);
EXTERN_GUID( MF_TOPOLOGY_PROJECTSTOP, 0x7ed3f803, 0x86bb, 0x4b3f, 0xb7, 0xe4, 0x7c, 0xb4, 0x3a, 0xfd, 0x4b, 0x80);
EXTERN_GUID( MF_TOPOLOGY_NO_MARKIN_MARKOUT, 0x7ed3f804, 0x86bb, 0x4b3f, 0xb7, 0xe4, 0x7c, 0xb4, 0x3a, 0xfd, 0x4b, 0x80);
STDAPI MFCreateTopology(
    __out IMFTopology ** ppTopo );
typedef 
enum MF_TOPOLOGY_TYPE
    {	MF_TOPOLOGY_OUTPUT_NODE	= 0,
	MF_TOPOLOGY_SOURCESTREAM_NODE	= ( MF_TOPOLOGY_OUTPUT_NODE + 1 ) ,
	MF_TOPOLOGY_TRANSFORM_NODE	= ( MF_TOPOLOGY_SOURCESTREAM_NODE + 1 ) ,
	MF_TOPOLOGY_TEE_NODE	= ( MF_TOPOLOGY_TRANSFORM_NODE + 1 ) ,
	MF_TOPOLOGY_MAX	= 0xffffffff
    } 	MF_TOPOLOGY_TYPE;



extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0008_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0008_v0_0_s_ifspec;

#ifndef __IMFTopologyNode_INTERFACE_DEFINED__
#define __IMFTopologyNode_INTERFACE_DEFINED__

/* interface IMFTopologyNode */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFTopologyNode;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("83CF873A-F6DA-4bc8-823F-BACFD55DC430")
    IMFTopologyNode : public IMFAttributes
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetObject( 
            /* [in] */ __RPC__in_opt IUnknown *pObject) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetObject( 
            /* [out] */ __RPC__deref_out_opt IUnknown **ppObject) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNodeType( 
            /* [out] */ __RPC__out MF_TOPOLOGY_TYPE *pType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTopoNodeID( 
            /* [out] */ __RPC__out TOPOID *pID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetTopoNodeID( 
            /* [in] */ TOPOID ullTopoID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetInputCount( 
            /* [out] */ __RPC__out DWORD *pcInputs) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetOutputCount( 
            /* [out] */ __RPC__out DWORD *pcOutputs) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE ConnectOutput( 
            /* [in] */ DWORD dwOutputIndex,
            /* [in] */ IMFTopologyNode *pDownstreamNode,
            /* [in] */ DWORD dwInputIndexOnDownstreamNode) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE DisconnectOutput( 
            /* [in] */ DWORD dwOutputIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetInput( 
            /* [in] */ DWORD dwInputIndex,
            /* [out] */ __RPC__deref_out_opt IMFTopologyNode **ppUpstreamNode,
            /* [out] */ __RPC__out DWORD *pdwOutputIndexOnUpstreamNode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetOutput( 
            /* [in] */ DWORD dwOutputIndex,
            /* [out] */ __RPC__deref_out_opt IMFTopologyNode **ppDownstreamNode,
            /* [out] */ __RPC__out DWORD *pdwInputIndexOnDownstreamNode) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE SetOutputPrefType( 
            /* [in] */ DWORD dwOutputIndex,
            /* [in] */ IMFMediaType *pType) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetOutputPrefType( 
            /* [in] */ DWORD dwOutputIndex,
            /* [out] */ 
            __out  IMFMediaType **ppType) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE SetInputPrefType( 
            /* [in] */ DWORD dwInputIndex,
            /* [in] */ IMFMediaType *pType) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetInputPrefType( 
            /* [in] */ DWORD dwInputIndex,
            /* [out] */ 
            __out  IMFMediaType **ppType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CloneFrom( 
            /* [in] */ __RPC__in_opt IMFTopologyNode *pNode) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFTopologyNodeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFTopologyNode * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFTopologyNode * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFTopologyNode * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetItem )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            /* [full][out][in] */ __RPC__inout_opt PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemType )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out MF_ATTRIBUTE_TYPE *pType);
        
        HRESULT ( STDMETHODCALLTYPE *CompareItem )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFPROPVARIANT Value,
            /* [out] */ __RPC__out BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *Compare )( 
            IMFTopologyNode * This,
            __RPC__in_opt IMFAttributes *pTheirs,
            MF_ATTRIBUTES_MATCH_TYPE MatchType,
            /* [out] */ __RPC__out BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT32 )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT64 )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT64 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetDouble )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out double *pfValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetGUID )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out GUID *pguidValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetStringLength )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetString )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][out] */ __RPC__out_ecount_full(cchBufSize) LPWSTR pwszValue,
            UINT32 cchBufSize,
            /* [full][out][in] */ __RPC__inout_opt UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedString )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(( *pcchLength + 1 ) ) LPWSTR *ppwszValue,
            /* [out] */ __RPC__out UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlobSize )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlob )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][out] */ __RPC__out_ecount_full(cbBufSize) UINT8 *pBuf,
            UINT32 cbBufSize,
            /* [full][out][in] */ __RPC__inout_opt UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedBlob )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbSize) UINT8 **ppBuf,
            /* [out] */ __RPC__out UINT32 *pcbSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetUnknown )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFIID riid,
            /* [iid_is][out] */ __RPC__deref_out_opt LPVOID *ppv);
        
        HRESULT ( STDMETHODCALLTYPE *SetItem )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFPROPVARIANT Value);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteItem )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteAllItems )( 
            IMFTopologyNode * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT32 )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            UINT32 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT64 )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            UINT64 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetDouble )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            double fValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetGUID )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFGUID guidValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetString )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            /* [string][in] */ __RPC__in LPCWSTR wszValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetBlob )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][in] */ __RPC__in_ecount_full(cbBufSize) const UINT8 *pBuf,
            UINT32 cbBufSize);
        
        HRESULT ( STDMETHODCALLTYPE *SetUnknown )( 
            IMFTopologyNode * This,
            __RPC__in REFGUID guidKey,
            /* [in] */ __RPC__in_opt IUnknown *pUnknown);
        
        HRESULT ( STDMETHODCALLTYPE *LockStore )( 
            IMFTopologyNode * This);
        
        HRESULT ( STDMETHODCALLTYPE *UnlockStore )( 
            IMFTopologyNode * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IMFTopologyNode * This,
            /* [out] */ __RPC__out UINT32 *pcItems);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemByIndex )( 
            IMFTopologyNode * This,
            UINT32 unIndex,
            /* [out] */ __RPC__out GUID *pguidKey,
            /* [full][out][in] */ __RPC__inout_opt PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *CopyAllItems )( 
            IMFTopologyNode * This,
            /* [in] */ __RPC__in_opt IMFAttributes *pDest);
        
        HRESULT ( STDMETHODCALLTYPE *SetObject )( 
            IMFTopologyNode * This,
            /* [in] */ __RPC__in_opt IUnknown *pObject);
        
        HRESULT ( STDMETHODCALLTYPE *GetObject )( 
            IMFTopologyNode * This,
            /* [out] */ __RPC__deref_out_opt IUnknown **ppObject);
        
        HRESULT ( STDMETHODCALLTYPE *GetNodeType )( 
            IMFTopologyNode * This,
            /* [out] */ __RPC__out MF_TOPOLOGY_TYPE *pType);
        
        HRESULT ( STDMETHODCALLTYPE *GetTopoNodeID )( 
            IMFTopologyNode * This,
            /* [out] */ __RPC__out TOPOID *pID);
        
        HRESULT ( STDMETHODCALLTYPE *SetTopoNodeID )( 
            IMFTopologyNode * This,
            /* [in] */ TOPOID ullTopoID);
        
        HRESULT ( STDMETHODCALLTYPE *GetInputCount )( 
            IMFTopologyNode * This,
            /* [out] */ __RPC__out DWORD *pcInputs);
        
        HRESULT ( STDMETHODCALLTYPE *GetOutputCount )( 
            IMFTopologyNode * This,
            /* [out] */ __RPC__out DWORD *pcOutputs);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *ConnectOutput )( 
            IMFTopologyNode * This,
            /* [in] */ DWORD dwOutputIndex,
            /* [in] */ IMFTopologyNode *pDownstreamNode,
            /* [in] */ DWORD dwInputIndexOnDownstreamNode);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *DisconnectOutput )( 
            IMFTopologyNode * This,
            /* [in] */ DWORD dwOutputIndex);
        
        HRESULT ( STDMETHODCALLTYPE *GetInput )( 
            IMFTopologyNode * This,
            /* [in] */ DWORD dwInputIndex,
            /* [out] */ __RPC__deref_out_opt IMFTopologyNode **ppUpstreamNode,
            /* [out] */ __RPC__out DWORD *pdwOutputIndexOnUpstreamNode);
        
        HRESULT ( STDMETHODCALLTYPE *GetOutput )( 
            IMFTopologyNode * This,
            /* [in] */ DWORD dwOutputIndex,
            /* [out] */ __RPC__deref_out_opt IMFTopologyNode **ppDownstreamNode,
            /* [out] */ __RPC__out DWORD *pdwInputIndexOnDownstreamNode);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *SetOutputPrefType )( 
            IMFTopologyNode * This,
            /* [in] */ DWORD dwOutputIndex,
            /* [in] */ IMFMediaType *pType);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *GetOutputPrefType )( 
            IMFTopologyNode * This,
            /* [in] */ DWORD dwOutputIndex,
            /* [out] */ 
            __out  IMFMediaType **ppType);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *SetInputPrefType )( 
            IMFTopologyNode * This,
            /* [in] */ DWORD dwInputIndex,
            /* [in] */ IMFMediaType *pType);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *GetInputPrefType )( 
            IMFTopologyNode * This,
            /* [in] */ DWORD dwInputIndex,
            /* [out] */ 
            __out  IMFMediaType **ppType);
        
        HRESULT ( STDMETHODCALLTYPE *CloneFrom )( 
            IMFTopologyNode * This,
            /* [in] */ __RPC__in_opt IMFTopologyNode *pNode);
        
        END_INTERFACE
    } IMFTopologyNodeVtbl;

    interface IMFTopologyNode
    {
        CONST_VTBL struct IMFTopologyNodeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFTopologyNode_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFTopologyNode_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFTopologyNode_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFTopologyNode_GetItem(This,guidKey,pValue)	\
    ( (This)->lpVtbl -> GetItem(This,guidKey,pValue) ) 

#define IMFTopologyNode_GetItemType(This,guidKey,pType)	\
    ( (This)->lpVtbl -> GetItemType(This,guidKey,pType) ) 

#define IMFTopologyNode_CompareItem(This,guidKey,Value,pbResult)	\
    ( (This)->lpVtbl -> CompareItem(This,guidKey,Value,pbResult) ) 

#define IMFTopologyNode_Compare(This,pTheirs,MatchType,pbResult)	\
    ( (This)->lpVtbl -> Compare(This,pTheirs,MatchType,pbResult) ) 

#define IMFTopologyNode_GetUINT32(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT32(This,guidKey,punValue) ) 

#define IMFTopologyNode_GetUINT64(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT64(This,guidKey,punValue) ) 

#define IMFTopologyNode_GetDouble(This,guidKey,pfValue)	\
    ( (This)->lpVtbl -> GetDouble(This,guidKey,pfValue) ) 

#define IMFTopologyNode_GetGUID(This,guidKey,pguidValue)	\
    ( (This)->lpVtbl -> GetGUID(This,guidKey,pguidValue) ) 

#define IMFTopologyNode_GetStringLength(This,guidKey,pcchLength)	\
    ( (This)->lpVtbl -> GetStringLength(This,guidKey,pcchLength) ) 

#define IMFTopologyNode_GetString(This,guidKey,pwszValue,cchBufSize,pcchLength)	\
    ( (This)->lpVtbl -> GetString(This,guidKey,pwszValue,cchBufSize,pcchLength) ) 

#define IMFTopologyNode_GetAllocatedString(This,guidKey,ppwszValue,pcchLength)	\
    ( (This)->lpVtbl -> GetAllocatedString(This,guidKey,ppwszValue,pcchLength) ) 

#define IMFTopologyNode_GetBlobSize(This,guidKey,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlobSize(This,guidKey,pcbBlobSize) ) 

#define IMFTopologyNode_GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize) ) 

#define IMFTopologyNode_GetAllocatedBlob(This,guidKey,ppBuf,pcbSize)	\
    ( (This)->lpVtbl -> GetAllocatedBlob(This,guidKey,ppBuf,pcbSize) ) 

#define IMFTopologyNode_GetUnknown(This,guidKey,riid,ppv)	\
    ( (This)->lpVtbl -> GetUnknown(This,guidKey,riid,ppv) ) 

#define IMFTopologyNode_SetItem(This,guidKey,Value)	\
    ( (This)->lpVtbl -> SetItem(This,guidKey,Value) ) 

#define IMFTopologyNode_DeleteItem(This,guidKey)	\
    ( (This)->lpVtbl -> DeleteItem(This,guidKey) ) 

#define IMFTopologyNode_DeleteAllItems(This)	\
    ( (This)->lpVtbl -> DeleteAllItems(This) ) 

#define IMFTopologyNode_SetUINT32(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT32(This,guidKey,unValue) ) 

#define IMFTopologyNode_SetUINT64(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT64(This,guidKey,unValue) ) 

#define IMFTopologyNode_SetDouble(This,guidKey,fValue)	\
    ( (This)->lpVtbl -> SetDouble(This,guidKey,fValue) ) 

#define IMFTopologyNode_SetGUID(This,guidKey,guidValue)	\
    ( (This)->lpVtbl -> SetGUID(This,guidKey,guidValue) ) 

#define IMFTopologyNode_SetString(This,guidKey,wszValue)	\
    ( (This)->lpVtbl -> SetString(This,guidKey,wszValue) ) 

#define IMFTopologyNode_SetBlob(This,guidKey,pBuf,cbBufSize)	\
    ( (This)->lpVtbl -> SetBlob(This,guidKey,pBuf,cbBufSize) ) 

#define IMFTopologyNode_SetUnknown(This,guidKey,pUnknown)	\
    ( (This)->lpVtbl -> SetUnknown(This,guidKey,pUnknown) ) 

#define IMFTopologyNode_LockStore(This)	\
    ( (This)->lpVtbl -> LockStore(This) ) 

#define IMFTopologyNode_UnlockStore(This)	\
    ( (This)->lpVtbl -> UnlockStore(This) ) 

#define IMFTopologyNode_GetCount(This,pcItems)	\
    ( (This)->lpVtbl -> GetCount(This,pcItems) ) 

#define IMFTopologyNode_GetItemByIndex(This,unIndex,pguidKey,pValue)	\
    ( (This)->lpVtbl -> GetItemByIndex(This,unIndex,pguidKey,pValue) ) 

#define IMFTopologyNode_CopyAllItems(This,pDest)	\
    ( (This)->lpVtbl -> CopyAllItems(This,pDest) ) 


#define IMFTopologyNode_SetObject(This,pObject)	\
    ( (This)->lpVtbl -> SetObject(This,pObject) ) 

#define IMFTopologyNode_GetObject(This,ppObject)	\
    ( (This)->lpVtbl -> GetObject(This,ppObject) ) 

#define IMFTopologyNode_GetNodeType(This,pType)	\
    ( (This)->lpVtbl -> GetNodeType(This,pType) ) 

#define IMFTopologyNode_GetTopoNodeID(This,pID)	\
    ( (This)->lpVtbl -> GetTopoNodeID(This,pID) ) 

#define IMFTopologyNode_SetTopoNodeID(This,ullTopoID)	\
    ( (This)->lpVtbl -> SetTopoNodeID(This,ullTopoID) ) 

#define IMFTopologyNode_GetInputCount(This,pcInputs)	\
    ( (This)->lpVtbl -> GetInputCount(This,pcInputs) ) 

#define IMFTopologyNode_GetOutputCount(This,pcOutputs)	\
    ( (This)->lpVtbl -> GetOutputCount(This,pcOutputs) ) 

#define IMFTopologyNode_ConnectOutput(This,dwOutputIndex,pDownstreamNode,dwInputIndexOnDownstreamNode)	\
    ( (This)->lpVtbl -> ConnectOutput(This,dwOutputIndex,pDownstreamNode,dwInputIndexOnDownstreamNode) ) 

#define IMFTopologyNode_DisconnectOutput(This,dwOutputIndex)	\
    ( (This)->lpVtbl -> DisconnectOutput(This,dwOutputIndex) ) 

#define IMFTopologyNode_GetInput(This,dwInputIndex,ppUpstreamNode,pdwOutputIndexOnUpstreamNode)	\
    ( (This)->lpVtbl -> GetInput(This,dwInputIndex,ppUpstreamNode,pdwOutputIndexOnUpstreamNode) ) 

#define IMFTopologyNode_GetOutput(This,dwOutputIndex,ppDownstreamNode,pdwInputIndexOnDownstreamNode)	\
    ( (This)->lpVtbl -> GetOutput(This,dwOutputIndex,ppDownstreamNode,pdwInputIndexOnDownstreamNode) ) 

#define IMFTopologyNode_SetOutputPrefType(This,dwOutputIndex,pType)	\
    ( (This)->lpVtbl -> SetOutputPrefType(This,dwOutputIndex,pType) ) 

#define IMFTopologyNode_GetOutputPrefType(This,dwOutputIndex,ppType)	\
    ( (This)->lpVtbl -> GetOutputPrefType(This,dwOutputIndex,ppType) ) 

#define IMFTopologyNode_SetInputPrefType(This,dwInputIndex,pType)	\
    ( (This)->lpVtbl -> SetInputPrefType(This,dwInputIndex,pType) ) 

#define IMFTopologyNode_GetInputPrefType(This,dwInputIndex,ppType)	\
    ( (This)->lpVtbl -> GetInputPrefType(This,dwInputIndex,ppType) ) 

#define IMFTopologyNode_CloneFrom(This,pNode)	\
    ( (This)->lpVtbl -> CloneFrom(This,pNode) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFTopologyNode_RemoteGetOutputPrefType_Proxy( 
    IMFTopologyNode * This,
    /* [in] */ DWORD dwOutputIndex,
    /* [out] */ __RPC__out DWORD *pcbData,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbData) BYTE **ppbData);


void __RPC_STUB IMFTopologyNode_RemoteGetOutputPrefType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFTopologyNode_RemoteGetInputPrefType_Proxy( 
    IMFTopologyNode * This,
    /* [in] */ DWORD dwInputIndex,
    /* [out] */ __RPC__out DWORD *pcbData,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbData) BYTE **ppbData);


void __RPC_STUB IMFTopologyNode_RemoteGetInputPrefType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMFTopologyNode_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0009 */
/* [local] */ 

typedef 
enum _MF_TOPONODE_FLUSH_MODE
    {	MF_TOPONODE_FLUSH_ALWAYS	= 0,
	MF_TOPONODE_FLUSH_SEEK	= ( MF_TOPONODE_FLUSH_ALWAYS + 1 ) ,
	MF_TOPONODE_FLUSH_NEVER	= ( MF_TOPONODE_FLUSH_SEEK + 1 ) 
    } 	MF_TOPONODE_FLUSH_MODE;

EXTERN_GUID( MF_TOPONODE_FLUSH, 0x494bbce8, 0xb031,  0x4e38,  0x97, 0xc4, 0xd5, 0x42, 0x2d, 0xd6, 0x18, 0xdc);
typedef 
enum _MF_TOPONODE_DRAIN_MODE
    {	MF_TOPONODE_DRAIN_DEFAULT	= 0,
	MF_TOPONODE_DRAIN_ALWAYS	= ( MF_TOPONODE_DRAIN_DEFAULT + 1 ) ,
	MF_TOPONODE_DRAIN_NEVER	= ( MF_TOPONODE_DRAIN_ALWAYS + 1 ) 
    } 	MF_TOPONODE_DRAIN_MODE;

EXTERN_GUID( MF_TOPONODE_DRAIN, 0x494bbce9, 0xb031,  0x4e38,  0x97, 0xc4, 0xd5, 0x42, 0x2d, 0xd6, 0x18, 0xdc);
EXTERN_GUID( MF_TOPONODE_D3DAWARE, 0x494bbced, 0xb031,  0x4e38,  0x97, 0xc4, 0xd5, 0x42, 0x2d, 0xd6, 0x18, 0xdc);
EXTERN_GUID( MF_TOPOLOGY_RESOLUTION_STATUS, 0x494bbcde, 0xb031,  0x4e38,  0x97, 0xc4, 0xd5, 0x42, 0x2d, 0xd6, 0x18, 0xdc);
EXTERN_GUID( MF_TOPONODE_ERRORCODE, 0x494bbcee, 0xb031,  0x4e38,  0x97, 0xc4, 0xd5, 0x42, 0x2d, 0xd6, 0x18, 0xdc);
EXTERN_GUID( MF_TOPONODE_CONNECT_METHOD, 0x494bbcf1, 0xb031,  0x4e38,  0x97, 0xc4, 0xd5, 0x42, 0x2d, 0xd6, 0x18, 0xdc);
EXTERN_GUID( MF_TOPONODE_LOCKED, 0x494bbcf7, 0xb031,  0x4e38,  0x97, 0xc4, 0xd5, 0x42, 0x2d, 0xd6, 0x18, 0xdc);
EXTERN_GUID( MF_TOPONODE_WORKQUEUE_ID, 0x494bbcf8, 0xb031,  0x4e38,  0x97, 0xc4, 0xd5, 0x42, 0x2d, 0xd6, 0x18, 0xdc);
EXTERN_GUID( MF_TOPONODE_WORKQUEUE_MMCSS_CLASS, 0x494bbcf9, 0xb031,  0x4e38,  0x97, 0xc4, 0xd5, 0x42, 0x2d, 0xd6, 0x18, 0xdc);
EXTERN_GUID( MF_TOPONODE_DECRYPTOR, 0x494bbcfa, 0xb031,  0x4e38,  0x97, 0xc4, 0xd5, 0x42, 0x2d, 0xd6, 0x18, 0xdc);
EXTERN_GUID( MF_TOPONODE_DISCARDABLE, 0x494bbcfb, 0xb031,  0x4e38,  0x97, 0xc4, 0xd5, 0x42, 0x2d, 0xd6, 0x18, 0xdc);
EXTERN_GUID( MF_TOPONODE_ERROR_MAJORTYPE, 0x494bbcfd, 0xb031,  0x4e38,  0x97, 0xc4, 0xd5, 0x42, 0x2d, 0xd6, 0x18, 0xdc);
EXTERN_GUID( MF_TOPONODE_ERROR_SUBTYPE, 0x494bbcfe, 0xb031,  0x4e38,  0x97, 0xc4, 0xd5, 0x42, 0x2d, 0xd6, 0x18, 0xdc);
EXTERN_GUID( MF_TOPONODE_WORKQUEUE_MMCSS_TASKID, 0x494bbcff, 0xb031,  0x4e38,  0x97, 0xc4, 0xd5, 0x42, 0x2d, 0xd6, 0x18, 0xdc);
EXTERN_GUID( MF_TOPONODE_MARKIN_HERE, 0x494bbd00, 0xb031,  0x4e38,  0x97, 0xc4, 0xd5, 0x42, 0x2d, 0xd6, 0x18, 0xdc);
EXTERN_GUID( MF_TOPONODE_MARKOUT_HERE, 0x494bbd01, 0xb031,  0x4e38,  0x97, 0xc4, 0xd5, 0x42, 0x2d, 0xd6, 0x18, 0xdc);
EXTERN_GUID( MF_TOPONODE_DECODER, 0x494bbd02, 0xb031,  0x4e38,  0x97, 0xc4, 0xd5, 0x42, 0x2d, 0xd6, 0x18, 0xdc);
EXTERN_GUID( MF_TOPONODE_MEDIASTART, 0x835c58ea, 0xe075, 0x4bc7, 0xbc, 0xba, 0x4d, 0xe0, 0x00, 0xdf, 0x9a, 0xe6);
EXTERN_GUID( MF_TOPONODE_MEDIASTOP, 0x835c58eb, 0xe075, 0x4bc7, 0xbc, 0xba, 0x4d, 0xe0, 0x00, 0xdf, 0x9a, 0xe6);
EXTERN_GUID( MF_TOPONODE_SOURCE, 0x835c58ec, 0xe075, 0x4bc7, 0xbc, 0xba, 0x4d, 0xe0, 0x00, 0xdf, 0x9a, 0xe6);
EXTERN_GUID( MF_TOPONODE_PRESENTATION_DESCRIPTOR, 0x835c58ed, 0xe075, 0x4bc7, 0xbc, 0xba, 0x4d, 0xe0, 0x00, 0xdf, 0x9a, 0xe6);
EXTERN_GUID( MF_TOPONODE_STREAM_DESCRIPTOR, 0x835c58ee, 0xe075, 0x4bc7, 0xbc, 0xba, 0x4d, 0xe0, 0x00, 0xdf, 0x9a, 0xe6);
EXTERN_GUID( MF_TOPONODE_SEQUENCE_ELEMENTID, 0x835c58ef, 0xe075, 0x4bc7, 0xbc, 0xba, 0x4d, 0xe0, 0x00, 0xdf, 0x9a, 0xe6);
EXTERN_GUID( MF_TOPONODE_TRANSFORM_OBJECTID, 0x88dcc0c9, 0x293e, 0x4e8b, 0x9a, 0xeb, 0xa, 0xd6, 0x4c, 0xc0, 0x16, 0xb0);
EXTERN_GUID( MF_TOPONODE_STREAMID, 0x14932f9b, 0x9087, 0x4bb4, 0x84, 0x12, 0x51, 0x67, 0x14, 0x5c, 0xbe, 0x04);
EXTERN_GUID( MF_TOPONODE_NOSHUTDOWN_ON_REMOVE, 0x14932f9c, 0x9087, 0x4bb4, 0x84, 0x12, 0x51, 0x67, 0x14, 0x5c, 0xbe, 0x04);
EXTERN_GUID( MF_TOPONODE_RATELESS, 0x14932f9d, 0x9087, 0x4bb4, 0x84, 0x12, 0x51, 0x67, 0x14, 0x5c, 0xbe, 0x04);
EXTERN_GUID( MF_TOPONODE_DISABLE_PREROLL, 0x14932f9e, 0x9087, 0x4bb4, 0x84, 0x12, 0x51, 0x67, 0x14, 0x5c, 0xbe, 0x04);
EXTERN_GUID( MF_TOPONODE_PRIMARYOUTPUT, 0x6304ef99, 0x16b2, 0x4ebe, 0x9d, 0x67, 0xe4, 0xc5, 0x39, 0xb3, 0xa2, 0x59);
STDAPI MFCreateTopologyNode(
    MF_TOPOLOGY_TYPE NodeType,
    __out IMFTopologyNode ** ppNode );


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0009_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0009_v0_0_s_ifspec;

#ifndef __IMFGetService_INTERFACE_DEFINED__
#define __IMFGetService_INTERFACE_DEFINED__

/* interface IMFGetService */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFGetService;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("fa993888-4383-415a-a930-dd472a8cf6f7")
    IMFGetService : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetService( 
            /* [in] */ __RPC__in REFGUID guidService,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ __RPC__deref_out_opt LPVOID *ppvObject) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFGetServiceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFGetService * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFGetService * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFGetService * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetService )( 
            IMFGetService * This,
            /* [in] */ __RPC__in REFGUID guidService,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ __RPC__deref_out_opt LPVOID *ppvObject);
        
        END_INTERFACE
    } IMFGetServiceVtbl;

    interface IMFGetService
    {
        CONST_VTBL struct IMFGetServiceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFGetService_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFGetService_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFGetService_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFGetService_GetService(This,guidService,riid,ppvObject)	\
    ( (This)->lpVtbl -> GetService(This,guidService,riid,ppvObject) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFGetService_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0010 */
/* [local] */ 

STDAPI MFGetService(
    IUnknown* punkObject,
    REFGUID guidService,
    REFIID riid,
    __out LPVOID* ppvObject
    );
typedef LONGLONG MFTIME;

typedef 
enum _MFCLOCK_CHARACTERISTICS_FLAGS
    {	MFCLOCK_CHARACTERISTICS_FLAG_FREQUENCY_10MHZ	= 0x2,
	MFCLOCK_CHARACTERISTICS_FLAG_ALWAYS_RUNNING	= 0x4,
	MFCLOCK_CHARACTERISTICS_FLAG_IS_SYSTEM_CLOCK	= 0x8
    } 	MFCLOCK_CHARACTERISTICS_FLAGS;

typedef 
enum _MFCLOCK_STATE
    {	MFCLOCK_STATE_INVALID	= 0,
	MFCLOCK_STATE_RUNNING	= ( MFCLOCK_STATE_INVALID + 1 ) ,
	MFCLOCK_STATE_STOPPED	= ( MFCLOCK_STATE_RUNNING + 1 ) ,
	MFCLOCK_STATE_PAUSED	= ( MFCLOCK_STATE_STOPPED + 1 ) 
    } 	MFCLOCK_STATE;

typedef 
enum _MFCLOCK_RELATIONAL_FLAGS
    {	MFCLOCK_RELATIONAL_FLAG_JITTER_NEVER_AHEAD	= 0x1
    } 	MFCLOCK_RELATIONAL_FLAGS;

typedef struct _MFCLOCK_PROPERTIES
    {
    unsigned __int64 qwCorrelationRate;
    GUID guidClockId;
    DWORD dwClockFlags;
    unsigned __int64 qwClockFrequency;
    DWORD dwClockTolerance;
    DWORD dwClockJitter;
    } 	MFCLOCK_PROPERTIES;

#define MFCLOCK_FREQUENCY_HNS       10000000
#define MFCLOCK_TOLERANCE_UNKNOWN   50000
#define MFCLOCK_JITTER_ISR          1000
#define MFCLOCK_JITTER_DPC          4000
#define MFCLOCK_JITTER_PASSIVE      10000


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0010_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0010_v0_0_s_ifspec;

#ifndef __IMFClock_INTERFACE_DEFINED__
#define __IMFClock_INTERFACE_DEFINED__

/* interface IMFClock */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFClock;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2eb1e945-18b8-4139-9b1a-d5d584818530")
    IMFClock : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetClockCharacteristics( 
            /* [out] */ __RPC__out DWORD *pdwCharacteristics) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCorrelatedTime( 
            /* [in] */ DWORD dwReserved,
            /* [out] */ __RPC__out LONGLONG *pllClockTime,
            /* [out] */ __RPC__out MFTIME *phnsSystemTime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetContinuityKey( 
            /* [out] */ __RPC__out DWORD *pdwContinuityKey) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetState( 
            /* [in] */ DWORD dwReserved,
            /* [out] */ __RPC__out MFCLOCK_STATE *peClockState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProperties( 
            /* [out] */ __RPC__out MFCLOCK_PROPERTIES *pClockProperties) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFClockVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFClock * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFClock * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFClock * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetClockCharacteristics )( 
            IMFClock * This,
            /* [out] */ __RPC__out DWORD *pdwCharacteristics);
        
        HRESULT ( STDMETHODCALLTYPE *GetCorrelatedTime )( 
            IMFClock * This,
            /* [in] */ DWORD dwReserved,
            /* [out] */ __RPC__out LONGLONG *pllClockTime,
            /* [out] */ __RPC__out MFTIME *phnsSystemTime);
        
        HRESULT ( STDMETHODCALLTYPE *GetContinuityKey )( 
            IMFClock * This,
            /* [out] */ __RPC__out DWORD *pdwContinuityKey);
        
        HRESULT ( STDMETHODCALLTYPE *GetState )( 
            IMFClock * This,
            /* [in] */ DWORD dwReserved,
            /* [out] */ __RPC__out MFCLOCK_STATE *peClockState);
        
        HRESULT ( STDMETHODCALLTYPE *GetProperties )( 
            IMFClock * This,
            /* [out] */ __RPC__out MFCLOCK_PROPERTIES *pClockProperties);
        
        END_INTERFACE
    } IMFClockVtbl;

    interface IMFClock
    {
        CONST_VTBL struct IMFClockVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFClock_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFClock_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFClock_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFClock_GetClockCharacteristics(This,pdwCharacteristics)	\
    ( (This)->lpVtbl -> GetClockCharacteristics(This,pdwCharacteristics) ) 

#define IMFClock_GetCorrelatedTime(This,dwReserved,pllClockTime,phnsSystemTime)	\
    ( (This)->lpVtbl -> GetCorrelatedTime(This,dwReserved,pllClockTime,phnsSystemTime) ) 

#define IMFClock_GetContinuityKey(This,pdwContinuityKey)	\
    ( (This)->lpVtbl -> GetContinuityKey(This,pdwContinuityKey) ) 

#define IMFClock_GetState(This,dwReserved,peClockState)	\
    ( (This)->lpVtbl -> GetState(This,dwReserved,peClockState) ) 

#define IMFClock_GetProperties(This,pClockProperties)	\
    ( (This)->lpVtbl -> GetProperties(This,pClockProperties) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFClock_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0011 */
/* [local] */ 

STDAPI_(MFTIME) 
MFGetSystemTime(
    );
#define PRESENTATION_CURRENT_POSITION   0x7fffffffffffffff




extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0011_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0011_v0_0_s_ifspec;

#ifndef __IMFPresentationClock_INTERFACE_DEFINED__
#define __IMFPresentationClock_INTERFACE_DEFINED__

/* interface IMFPresentationClock */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFPresentationClock;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("868CE85C-8EA9-4f55-AB82-B009A910A805")
    IMFPresentationClock : public IMFClock
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetTimeSource( 
            /* [in] */ __RPC__in_opt IMFPresentationTimeSource *pTimeSource) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTimeSource( 
            /* [out] */ __RPC__deref_out_opt IMFPresentationTimeSource **ppTimeSource) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTime( 
            /* [out] */ __RPC__out MFTIME *phnsClockTime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddClockStateSink( 
            /* [in] */ __RPC__in_opt IMFClockStateSink *pStateSink) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveClockStateSink( 
            /* [in] */ __RPC__in_opt IMFClockStateSink *pStateSink) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Start( 
            /* [in] */ LONGLONG llClockStartOffset) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Pause( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFPresentationClockVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFPresentationClock * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFPresentationClock * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFPresentationClock * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetClockCharacteristics )( 
            IMFPresentationClock * This,
            /* [out] */ __RPC__out DWORD *pdwCharacteristics);
        
        HRESULT ( STDMETHODCALLTYPE *GetCorrelatedTime )( 
            IMFPresentationClock * This,
            /* [in] */ DWORD dwReserved,
            /* [out] */ __RPC__out LONGLONG *pllClockTime,
            /* [out] */ __RPC__out MFTIME *phnsSystemTime);
        
        HRESULT ( STDMETHODCALLTYPE *GetContinuityKey )( 
            IMFPresentationClock * This,
            /* [out] */ __RPC__out DWORD *pdwContinuityKey);
        
        HRESULT ( STDMETHODCALLTYPE *GetState )( 
            IMFPresentationClock * This,
            /* [in] */ DWORD dwReserved,
            /* [out] */ __RPC__out MFCLOCK_STATE *peClockState);
        
        HRESULT ( STDMETHODCALLTYPE *GetProperties )( 
            IMFPresentationClock * This,
            /* [out] */ __RPC__out MFCLOCK_PROPERTIES *pClockProperties);
        
        HRESULT ( STDMETHODCALLTYPE *SetTimeSource )( 
            IMFPresentationClock * This,
            /* [in] */ __RPC__in_opt IMFPresentationTimeSource *pTimeSource);
        
        HRESULT ( STDMETHODCALLTYPE *GetTimeSource )( 
            IMFPresentationClock * This,
            /* [out] */ __RPC__deref_out_opt IMFPresentationTimeSource **ppTimeSource);
        
        HRESULT ( STDMETHODCALLTYPE *GetTime )( 
            IMFPresentationClock * This,
            /* [out] */ __RPC__out MFTIME *phnsClockTime);
        
        HRESULT ( STDMETHODCALLTYPE *AddClockStateSink )( 
            IMFPresentationClock * This,
            /* [in] */ __RPC__in_opt IMFClockStateSink *pStateSink);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveClockStateSink )( 
            IMFPresentationClock * This,
            /* [in] */ __RPC__in_opt IMFClockStateSink *pStateSink);
        
        HRESULT ( STDMETHODCALLTYPE *Start )( 
            IMFPresentationClock * This,
            /* [in] */ LONGLONG llClockStartOffset);
        
        HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IMFPresentationClock * This);
        
        HRESULT ( STDMETHODCALLTYPE *Pause )( 
            IMFPresentationClock * This);
        
        END_INTERFACE
    } IMFPresentationClockVtbl;

    interface IMFPresentationClock
    {
        CONST_VTBL struct IMFPresentationClockVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFPresentationClock_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFPresentationClock_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFPresentationClock_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFPresentationClock_GetClockCharacteristics(This,pdwCharacteristics)	\
    ( (This)->lpVtbl -> GetClockCharacteristics(This,pdwCharacteristics) ) 

#define IMFPresentationClock_GetCorrelatedTime(This,dwReserved,pllClockTime,phnsSystemTime)	\
    ( (This)->lpVtbl -> GetCorrelatedTime(This,dwReserved,pllClockTime,phnsSystemTime) ) 

#define IMFPresentationClock_GetContinuityKey(This,pdwContinuityKey)	\
    ( (This)->lpVtbl -> GetContinuityKey(This,pdwContinuityKey) ) 

#define IMFPresentationClock_GetState(This,dwReserved,peClockState)	\
    ( (This)->lpVtbl -> GetState(This,dwReserved,peClockState) ) 

#define IMFPresentationClock_GetProperties(This,pClockProperties)	\
    ( (This)->lpVtbl -> GetProperties(This,pClockProperties) ) 


#define IMFPresentationClock_SetTimeSource(This,pTimeSource)	\
    ( (This)->lpVtbl -> SetTimeSource(This,pTimeSource) ) 

#define IMFPresentationClock_GetTimeSource(This,ppTimeSource)	\
    ( (This)->lpVtbl -> GetTimeSource(This,ppTimeSource) ) 

#define IMFPresentationClock_GetTime(This,phnsClockTime)	\
    ( (This)->lpVtbl -> GetTime(This,phnsClockTime) ) 

#define IMFPresentationClock_AddClockStateSink(This,pStateSink)	\
    ( (This)->lpVtbl -> AddClockStateSink(This,pStateSink) ) 

#define IMFPresentationClock_RemoveClockStateSink(This,pStateSink)	\
    ( (This)->lpVtbl -> RemoveClockStateSink(This,pStateSink) ) 

#define IMFPresentationClock_Start(This,llClockStartOffset)	\
    ( (This)->lpVtbl -> Start(This,llClockStartOffset) ) 

#define IMFPresentationClock_Stop(This)	\
    ( (This)->lpVtbl -> Stop(This) ) 

#define IMFPresentationClock_Pause(This)	\
    ( (This)->lpVtbl -> Pause(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFPresentationClock_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0012 */
/* [local] */ 

STDAPI MFCreatePresentationClock(
    __out IMFPresentationClock** ppPresentationClock
    );


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0012_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0012_v0_0_s_ifspec;

#ifndef __IMFPresentationTimeSource_INTERFACE_DEFINED__
#define __IMFPresentationTimeSource_INTERFACE_DEFINED__

/* interface IMFPresentationTimeSource */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFPresentationTimeSource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7FF12CCE-F76F-41c2-863B-1666C8E5E139")
    IMFPresentationTimeSource : public IMFClock
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetUnderlyingClock( 
            /* [out] */ __RPC__deref_out_opt IMFClock **ppClock) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFPresentationTimeSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFPresentationTimeSource * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFPresentationTimeSource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFPresentationTimeSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetClockCharacteristics )( 
            IMFPresentationTimeSource * This,
            /* [out] */ __RPC__out DWORD *pdwCharacteristics);
        
        HRESULT ( STDMETHODCALLTYPE *GetCorrelatedTime )( 
            IMFPresentationTimeSource * This,
            /* [in] */ DWORD dwReserved,
            /* [out] */ __RPC__out LONGLONG *pllClockTime,
            /* [out] */ __RPC__out MFTIME *phnsSystemTime);
        
        HRESULT ( STDMETHODCALLTYPE *GetContinuityKey )( 
            IMFPresentationTimeSource * This,
            /* [out] */ __RPC__out DWORD *pdwContinuityKey);
        
        HRESULT ( STDMETHODCALLTYPE *GetState )( 
            IMFPresentationTimeSource * This,
            /* [in] */ DWORD dwReserved,
            /* [out] */ __RPC__out MFCLOCK_STATE *peClockState);
        
        HRESULT ( STDMETHODCALLTYPE *GetProperties )( 
            IMFPresentationTimeSource * This,
            /* [out] */ __RPC__out MFCLOCK_PROPERTIES *pClockProperties);
        
        HRESULT ( STDMETHODCALLTYPE *GetUnderlyingClock )( 
            IMFPresentationTimeSource * This,
            /* [out] */ __RPC__deref_out_opt IMFClock **ppClock);
        
        END_INTERFACE
    } IMFPresentationTimeSourceVtbl;

    interface IMFPresentationTimeSource
    {
        CONST_VTBL struct IMFPresentationTimeSourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFPresentationTimeSource_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFPresentationTimeSource_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFPresentationTimeSource_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFPresentationTimeSource_GetClockCharacteristics(This,pdwCharacteristics)	\
    ( (This)->lpVtbl -> GetClockCharacteristics(This,pdwCharacteristics) ) 

#define IMFPresentationTimeSource_GetCorrelatedTime(This,dwReserved,pllClockTime,phnsSystemTime)	\
    ( (This)->lpVtbl -> GetCorrelatedTime(This,dwReserved,pllClockTime,phnsSystemTime) ) 

#define IMFPresentationTimeSource_GetContinuityKey(This,pdwContinuityKey)	\
    ( (This)->lpVtbl -> GetContinuityKey(This,pdwContinuityKey) ) 

#define IMFPresentationTimeSource_GetState(This,dwReserved,peClockState)	\
    ( (This)->lpVtbl -> GetState(This,dwReserved,peClockState) ) 

#define IMFPresentationTimeSource_GetProperties(This,pClockProperties)	\
    ( (This)->lpVtbl -> GetProperties(This,pClockProperties) ) 


#define IMFPresentationTimeSource_GetUnderlyingClock(This,ppClock)	\
    ( (This)->lpVtbl -> GetUnderlyingClock(This,ppClock) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFPresentationTimeSource_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0013 */
/* [local] */ 

STDAPI
MFCreateSystemTimeSource(
    __out IMFPresentationTimeSource** ppSystemTimeSource
    );


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0013_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0013_v0_0_s_ifspec;

#ifndef __IMFClockStateSink_INTERFACE_DEFINED__
#define __IMFClockStateSink_INTERFACE_DEFINED__

/* interface IMFClockStateSink */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFClockStateSink;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F6696E82-74F7-4f3d-A178-8A5E09C3659F")
    IMFClockStateSink : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnClockStart( 
            /* [in] */ MFTIME hnsSystemTime,
            /* [in] */ LONGLONG llClockStartOffset) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnClockStop( 
            /* [in] */ MFTIME hnsSystemTime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnClockPause( 
            /* [in] */ MFTIME hnsSystemTime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnClockRestart( 
            /* [in] */ MFTIME hnsSystemTime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnClockSetRate( 
            /* [in] */ MFTIME hnsSystemTime,
            /* [in] */ float flRate) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFClockStateSinkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFClockStateSink * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFClockStateSink * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFClockStateSink * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnClockStart )( 
            IMFClockStateSink * This,
            /* [in] */ MFTIME hnsSystemTime,
            /* [in] */ LONGLONG llClockStartOffset);
        
        HRESULT ( STDMETHODCALLTYPE *OnClockStop )( 
            IMFClockStateSink * This,
            /* [in] */ MFTIME hnsSystemTime);
        
        HRESULT ( STDMETHODCALLTYPE *OnClockPause )( 
            IMFClockStateSink * This,
            /* [in] */ MFTIME hnsSystemTime);
        
        HRESULT ( STDMETHODCALLTYPE *OnClockRestart )( 
            IMFClockStateSink * This,
            /* [in] */ MFTIME hnsSystemTime);
        
        HRESULT ( STDMETHODCALLTYPE *OnClockSetRate )( 
            IMFClockStateSink * This,
            /* [in] */ MFTIME hnsSystemTime,
            /* [in] */ float flRate);
        
        END_INTERFACE
    } IMFClockStateSinkVtbl;

    interface IMFClockStateSink
    {
        CONST_VTBL struct IMFClockStateSinkVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFClockStateSink_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFClockStateSink_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFClockStateSink_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFClockStateSink_OnClockStart(This,hnsSystemTime,llClockStartOffset)	\
    ( (This)->lpVtbl -> OnClockStart(This,hnsSystemTime,llClockStartOffset) ) 

#define IMFClockStateSink_OnClockStop(This,hnsSystemTime)	\
    ( (This)->lpVtbl -> OnClockStop(This,hnsSystemTime) ) 

#define IMFClockStateSink_OnClockPause(This,hnsSystemTime)	\
    ( (This)->lpVtbl -> OnClockPause(This,hnsSystemTime) ) 

#define IMFClockStateSink_OnClockRestart(This,hnsSystemTime)	\
    ( (This)->lpVtbl -> OnClockRestart(This,hnsSystemTime) ) 

#define IMFClockStateSink_OnClockSetRate(This,hnsSystemTime,flRate)	\
    ( (This)->lpVtbl -> OnClockSetRate(This,hnsSystemTime,flRate) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFClockStateSink_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0014 */
/* [local] */ 

EXTERN_GUID( MF_PD_PMPHOST_CONTEXT, 0x6c990d31, 0xbb8e, 0x477a, 0x85, 0x98, 0xd, 0x5d, 0x96, 0xfc, 0xd8, 0x8a );
EXTERN_GUID( MF_PD_APP_CONTEXT, 0x6c990d32, 0xbb8e, 0x477a, 0x85, 0x98, 0xd, 0x5d, 0x96, 0xfc, 0xd8, 0x8a );
EXTERN_GUID( MF_PD_DURATION, 0x6c990d33, 0xbb8e, 0x477a, 0x85, 0x98, 0xd, 0x5d, 0x96, 0xfc, 0xd8, 0x8a );
EXTERN_GUID( MF_PD_TOTAL_FILE_SIZE, 0x6c990d34, 0xbb8e, 0x477a, 0x85, 0x98, 0xd, 0x5d, 0x96, 0xfc, 0xd8, 0x8a );
EXTERN_GUID( MF_PD_AUDIO_ENCODING_BITRATE, 0x6c990d35, 0xbb8e, 0x477a, 0x85, 0x98, 0xd, 0x5d, 0x96, 0xfc, 0xd8, 0x8a );
EXTERN_GUID( MF_PD_VIDEO_ENCODING_BITRATE, 0x6c990d36, 0xbb8e, 0x477a, 0x85, 0x98, 0xd, 0x5d, 0x96, 0xfc, 0xd8, 0x8a );
EXTERN_GUID( MF_PD_MIME_TYPE, 0x6c990d37, 0xbb8e, 0x477a, 0x85, 0x98, 0xd, 0x5d, 0x96, 0xfc, 0xd8, 0x8a );
EXTERN_GUID(MF_PD_LAST_MODIFIED_TIME, 0x6c990d38, 0xbb8e, 0x477a, 0x85, 0x98, 0xd, 0x5d, 0x96, 0xfc, 0xd8, 0x8a );


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0014_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0014_v0_0_s_ifspec;

#ifndef __IMFPresentationDescriptor_INTERFACE_DEFINED__
#define __IMFPresentationDescriptor_INTERFACE_DEFINED__

/* interface IMFPresentationDescriptor */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFPresentationDescriptor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("03cb2711-24d7-4db6-a17f-f3a7a479a536")
    IMFPresentationDescriptor : public IMFAttributes
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetStreamDescriptorCount( 
            /* [out] */ __RPC__out DWORD *pdwDescriptorCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStreamDescriptorByIndex( 
            /* [in] */ DWORD dwIndex,
            /* [out] */ __RPC__out BOOL *pfSelected,
            /* [out] */ __RPC__deref_out_opt IMFStreamDescriptor **ppDescriptor) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelectStream( 
            /* [in] */ DWORD dwDescriptorIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeselectStream( 
            /* [in] */ DWORD dwDescriptorIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ __RPC__deref_out_opt IMFPresentationDescriptor **ppPresentationDescriptor) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFPresentationDescriptorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFPresentationDescriptor * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFPresentationDescriptor * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFPresentationDescriptor * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetItem )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [full][out][in] */ __RPC__inout_opt PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemType )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out MF_ATTRIBUTE_TYPE *pType);
        
        HRESULT ( STDMETHODCALLTYPE *CompareItem )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFPROPVARIANT Value,
            /* [out] */ __RPC__out BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *Compare )( 
            IMFPresentationDescriptor * This,
            __RPC__in_opt IMFAttributes *pTheirs,
            MF_ATTRIBUTES_MATCH_TYPE MatchType,
            /* [out] */ __RPC__out BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT32 )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT64 )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT64 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetDouble )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out double *pfValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetGUID )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out GUID *pguidValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetStringLength )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetString )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][out] */ __RPC__out_ecount_full(cchBufSize) LPWSTR pwszValue,
            UINT32 cchBufSize,
            /* [full][out][in] */ __RPC__inout_opt UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedString )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(( *pcchLength + 1 ) ) LPWSTR *ppwszValue,
            /* [out] */ __RPC__out UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlobSize )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlob )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][out] */ __RPC__out_ecount_full(cbBufSize) UINT8 *pBuf,
            UINT32 cbBufSize,
            /* [full][out][in] */ __RPC__inout_opt UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedBlob )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbSize) UINT8 **ppBuf,
            /* [out] */ __RPC__out UINT32 *pcbSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetUnknown )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFIID riid,
            /* [iid_is][out] */ __RPC__deref_out_opt LPVOID *ppv);
        
        HRESULT ( STDMETHODCALLTYPE *SetItem )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFPROPVARIANT Value);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteItem )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteAllItems )( 
            IMFPresentationDescriptor * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT32 )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            UINT32 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT64 )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            UINT64 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetDouble )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            double fValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetGUID )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFGUID guidValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetString )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [string][in] */ __RPC__in LPCWSTR wszValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetBlob )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][in] */ __RPC__in_ecount_full(cbBufSize) const UINT8 *pBuf,
            UINT32 cbBufSize);
        
        HRESULT ( STDMETHODCALLTYPE *SetUnknown )( 
            IMFPresentationDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [in] */ __RPC__in_opt IUnknown *pUnknown);
        
        HRESULT ( STDMETHODCALLTYPE *LockStore )( 
            IMFPresentationDescriptor * This);
        
        HRESULT ( STDMETHODCALLTYPE *UnlockStore )( 
            IMFPresentationDescriptor * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IMFPresentationDescriptor * This,
            /* [out] */ __RPC__out UINT32 *pcItems);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemByIndex )( 
            IMFPresentationDescriptor * This,
            UINT32 unIndex,
            /* [out] */ __RPC__out GUID *pguidKey,
            /* [full][out][in] */ __RPC__inout_opt PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *CopyAllItems )( 
            IMFPresentationDescriptor * This,
            /* [in] */ __RPC__in_opt IMFAttributes *pDest);
        
        HRESULT ( STDMETHODCALLTYPE *GetStreamDescriptorCount )( 
            IMFPresentationDescriptor * This,
            /* [out] */ __RPC__out DWORD *pdwDescriptorCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetStreamDescriptorByIndex )( 
            IMFPresentationDescriptor * This,
            /* [in] */ DWORD dwIndex,
            /* [out] */ __RPC__out BOOL *pfSelected,
            /* [out] */ __RPC__deref_out_opt IMFStreamDescriptor **ppDescriptor);
        
        HRESULT ( STDMETHODCALLTYPE *SelectStream )( 
            IMFPresentationDescriptor * This,
            /* [in] */ DWORD dwDescriptorIndex);
        
        HRESULT ( STDMETHODCALLTYPE *DeselectStream )( 
            IMFPresentationDescriptor * This,
            /* [in] */ DWORD dwDescriptorIndex);
        
        HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IMFPresentationDescriptor * This,
            /* [out] */ __RPC__deref_out_opt IMFPresentationDescriptor **ppPresentationDescriptor);
        
        END_INTERFACE
    } IMFPresentationDescriptorVtbl;

    interface IMFPresentationDescriptor
    {
        CONST_VTBL struct IMFPresentationDescriptorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFPresentationDescriptor_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFPresentationDescriptor_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFPresentationDescriptor_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFPresentationDescriptor_GetItem(This,guidKey,pValue)	\
    ( (This)->lpVtbl -> GetItem(This,guidKey,pValue) ) 

#define IMFPresentationDescriptor_GetItemType(This,guidKey,pType)	\
    ( (This)->lpVtbl -> GetItemType(This,guidKey,pType) ) 

#define IMFPresentationDescriptor_CompareItem(This,guidKey,Value,pbResult)	\
    ( (This)->lpVtbl -> CompareItem(This,guidKey,Value,pbResult) ) 

#define IMFPresentationDescriptor_Compare(This,pTheirs,MatchType,pbResult)	\
    ( (This)->lpVtbl -> Compare(This,pTheirs,MatchType,pbResult) ) 

#define IMFPresentationDescriptor_GetUINT32(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT32(This,guidKey,punValue) ) 

#define IMFPresentationDescriptor_GetUINT64(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT64(This,guidKey,punValue) ) 

#define IMFPresentationDescriptor_GetDouble(This,guidKey,pfValue)	\
    ( (This)->lpVtbl -> GetDouble(This,guidKey,pfValue) ) 

#define IMFPresentationDescriptor_GetGUID(This,guidKey,pguidValue)	\
    ( (This)->lpVtbl -> GetGUID(This,guidKey,pguidValue) ) 

#define IMFPresentationDescriptor_GetStringLength(This,guidKey,pcchLength)	\
    ( (This)->lpVtbl -> GetStringLength(This,guidKey,pcchLength) ) 

#define IMFPresentationDescriptor_GetString(This,guidKey,pwszValue,cchBufSize,pcchLength)	\
    ( (This)->lpVtbl -> GetString(This,guidKey,pwszValue,cchBufSize,pcchLength) ) 

#define IMFPresentationDescriptor_GetAllocatedString(This,guidKey,ppwszValue,pcchLength)	\
    ( (This)->lpVtbl -> GetAllocatedString(This,guidKey,ppwszValue,pcchLength) ) 

#define IMFPresentationDescriptor_GetBlobSize(This,guidKey,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlobSize(This,guidKey,pcbBlobSize) ) 

#define IMFPresentationDescriptor_GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize) ) 

#define IMFPresentationDescriptor_GetAllocatedBlob(This,guidKey,ppBuf,pcbSize)	\
    ( (This)->lpVtbl -> GetAllocatedBlob(This,guidKey,ppBuf,pcbSize) ) 

#define IMFPresentationDescriptor_GetUnknown(This,guidKey,riid,ppv)	\
    ( (This)->lpVtbl -> GetUnknown(This,guidKey,riid,ppv) ) 

#define IMFPresentationDescriptor_SetItem(This,guidKey,Value)	\
    ( (This)->lpVtbl -> SetItem(This,guidKey,Value) ) 

#define IMFPresentationDescriptor_DeleteItem(This,guidKey)	\
    ( (This)->lpVtbl -> DeleteItem(This,guidKey) ) 

#define IMFPresentationDescriptor_DeleteAllItems(This)	\
    ( (This)->lpVtbl -> DeleteAllItems(This) ) 

#define IMFPresentationDescriptor_SetUINT32(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT32(This,guidKey,unValue) ) 

#define IMFPresentationDescriptor_SetUINT64(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT64(This,guidKey,unValue) ) 

#define IMFPresentationDescriptor_SetDouble(This,guidKey,fValue)	\
    ( (This)->lpVtbl -> SetDouble(This,guidKey,fValue) ) 

#define IMFPresentationDescriptor_SetGUID(This,guidKey,guidValue)	\
    ( (This)->lpVtbl -> SetGUID(This,guidKey,guidValue) ) 

#define IMFPresentationDescriptor_SetString(This,guidKey,wszValue)	\
    ( (This)->lpVtbl -> SetString(This,guidKey,wszValue) ) 

#define IMFPresentationDescriptor_SetBlob(This,guidKey,pBuf,cbBufSize)	\
    ( (This)->lpVtbl -> SetBlob(This,guidKey,pBuf,cbBufSize) ) 

#define IMFPresentationDescriptor_SetUnknown(This,guidKey,pUnknown)	\
    ( (This)->lpVtbl -> SetUnknown(This,guidKey,pUnknown) ) 

#define IMFPresentationDescriptor_LockStore(This)	\
    ( (This)->lpVtbl -> LockStore(This) ) 

#define IMFPresentationDescriptor_UnlockStore(This)	\
    ( (This)->lpVtbl -> UnlockStore(This) ) 

#define IMFPresentationDescriptor_GetCount(This,pcItems)	\
    ( (This)->lpVtbl -> GetCount(This,pcItems) ) 

#define IMFPresentationDescriptor_GetItemByIndex(This,unIndex,pguidKey,pValue)	\
    ( (This)->lpVtbl -> GetItemByIndex(This,unIndex,pguidKey,pValue) ) 

#define IMFPresentationDescriptor_CopyAllItems(This,pDest)	\
    ( (This)->lpVtbl -> CopyAllItems(This,pDest) ) 


#define IMFPresentationDescriptor_GetStreamDescriptorCount(This,pdwDescriptorCount)	\
    ( (This)->lpVtbl -> GetStreamDescriptorCount(This,pdwDescriptorCount) ) 

#define IMFPresentationDescriptor_GetStreamDescriptorByIndex(This,dwIndex,pfSelected,ppDescriptor)	\
    ( (This)->lpVtbl -> GetStreamDescriptorByIndex(This,dwIndex,pfSelected,ppDescriptor) ) 

#define IMFPresentationDescriptor_SelectStream(This,dwDescriptorIndex)	\
    ( (This)->lpVtbl -> SelectStream(This,dwDescriptorIndex) ) 

#define IMFPresentationDescriptor_DeselectStream(This,dwDescriptorIndex)	\
    ( (This)->lpVtbl -> DeselectStream(This,dwDescriptorIndex) ) 

#define IMFPresentationDescriptor_Clone(This,ppPresentationDescriptor)	\
    ( (This)->lpVtbl -> Clone(This,ppPresentationDescriptor) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFPresentationDescriptor_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0015 */
/* [local] */ 

STDAPI MFCreatePresentationDescriptor(
    DWORD cStreamDescriptors,
    __in_ecount_opt( cStreamDescriptors ) IMFStreamDescriptor** apStreamDescriptors,
    __deref_out IMFPresentationDescriptor** ppPresentationDescriptor
    );

STDAPI MFRequireProtectedEnvironment(
     IMFPresentationDescriptor* pPresentationDescriptor
     );

STDAPI MFSerializePresentationDescriptor(
    __in IMFPresentationDescriptor * pPD,
    __out DWORD * pcbData,
    __deref_out_bcount(*pcbData) BYTE ** ppbData);

STDAPI MFDeserializePresentationDescriptor(
    __in DWORD cbData,
    __in_ecount( cbData ) BYTE * pbData,
    __deref_out IMFPresentationDescriptor ** ppPD);

EXTERN_GUID(MF_SD_LANGUAGE, 0xaf2180, 0xbdc2, 0x423c, 0xab, 0xca, 0xf5, 0x3, 0x59, 0x3b, 0xc1, 0x21);
EXTERN_GUID(MF_SD_PROTECTED, 0xaf2181, 0xbdc2, 0x423c, 0xab, 0xca, 0xf5, 0x3, 0x59, 0x3b, 0xc1, 0x21);


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0015_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0015_v0_0_s_ifspec;

#ifndef __IMFStreamDescriptor_INTERFACE_DEFINED__
#define __IMFStreamDescriptor_INTERFACE_DEFINED__

/* interface IMFStreamDescriptor */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFStreamDescriptor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56c03d9c-9dbb-45f5-ab4b-d80f47c05938")
    IMFStreamDescriptor : public IMFAttributes
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetStreamIdentifier( 
            /* [out] */ __RPC__out DWORD *pdwStreamIdentifier) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMediaTypeHandler( 
            /* [out] */ __RPC__deref_out_opt IMFMediaTypeHandler **ppMediaTypeHandler) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFStreamDescriptorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFStreamDescriptor * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFStreamDescriptor * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFStreamDescriptor * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetItem )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [full][out][in] */ __RPC__inout_opt PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemType )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out MF_ATTRIBUTE_TYPE *pType);
        
        HRESULT ( STDMETHODCALLTYPE *CompareItem )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFPROPVARIANT Value,
            /* [out] */ __RPC__out BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *Compare )( 
            IMFStreamDescriptor * This,
            __RPC__in_opt IMFAttributes *pTheirs,
            MF_ATTRIBUTES_MATCH_TYPE MatchType,
            /* [out] */ __RPC__out BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT32 )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT64 )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT64 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetDouble )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out double *pfValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetGUID )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out GUID *pguidValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetStringLength )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetString )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][out] */ __RPC__out_ecount_full(cchBufSize) LPWSTR pwszValue,
            UINT32 cchBufSize,
            /* [full][out][in] */ __RPC__inout_opt UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedString )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(( *pcchLength + 1 ) ) LPWSTR *ppwszValue,
            /* [out] */ __RPC__out UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlobSize )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlob )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][out] */ __RPC__out_ecount_full(cbBufSize) UINT8 *pBuf,
            UINT32 cbBufSize,
            /* [full][out][in] */ __RPC__inout_opt UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedBlob )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbSize) UINT8 **ppBuf,
            /* [out] */ __RPC__out UINT32 *pcbSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetUnknown )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFIID riid,
            /* [iid_is][out] */ __RPC__deref_out_opt LPVOID *ppv);
        
        HRESULT ( STDMETHODCALLTYPE *SetItem )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFPROPVARIANT Value);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteItem )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteAllItems )( 
            IMFStreamDescriptor * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT32 )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            UINT32 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT64 )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            UINT64 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetDouble )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            double fValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetGUID )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFGUID guidValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetString )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [string][in] */ __RPC__in LPCWSTR wszValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetBlob )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][in] */ __RPC__in_ecount_full(cbBufSize) const UINT8 *pBuf,
            UINT32 cbBufSize);
        
        HRESULT ( STDMETHODCALLTYPE *SetUnknown )( 
            IMFStreamDescriptor * This,
            __RPC__in REFGUID guidKey,
            /* [in] */ __RPC__in_opt IUnknown *pUnknown);
        
        HRESULT ( STDMETHODCALLTYPE *LockStore )( 
            IMFStreamDescriptor * This);
        
        HRESULT ( STDMETHODCALLTYPE *UnlockStore )( 
            IMFStreamDescriptor * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IMFStreamDescriptor * This,
            /* [out] */ __RPC__out UINT32 *pcItems);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemByIndex )( 
            IMFStreamDescriptor * This,
            UINT32 unIndex,
            /* [out] */ __RPC__out GUID *pguidKey,
            /* [full][out][in] */ __RPC__inout_opt PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *CopyAllItems )( 
            IMFStreamDescriptor * This,
            /* [in] */ __RPC__in_opt IMFAttributes *pDest);
        
        HRESULT ( STDMETHODCALLTYPE *GetStreamIdentifier )( 
            IMFStreamDescriptor * This,
            /* [out] */ __RPC__out DWORD *pdwStreamIdentifier);
        
        HRESULT ( STDMETHODCALLTYPE *GetMediaTypeHandler )( 
            IMFStreamDescriptor * This,
            /* [out] */ __RPC__deref_out_opt IMFMediaTypeHandler **ppMediaTypeHandler);
        
        END_INTERFACE
    } IMFStreamDescriptorVtbl;

    interface IMFStreamDescriptor
    {
        CONST_VTBL struct IMFStreamDescriptorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFStreamDescriptor_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFStreamDescriptor_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFStreamDescriptor_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFStreamDescriptor_GetItem(This,guidKey,pValue)	\
    ( (This)->lpVtbl -> GetItem(This,guidKey,pValue) ) 

#define IMFStreamDescriptor_GetItemType(This,guidKey,pType)	\
    ( (This)->lpVtbl -> GetItemType(This,guidKey,pType) ) 

#define IMFStreamDescriptor_CompareItem(This,guidKey,Value,pbResult)	\
    ( (This)->lpVtbl -> CompareItem(This,guidKey,Value,pbResult) ) 

#define IMFStreamDescriptor_Compare(This,pTheirs,MatchType,pbResult)	\
    ( (This)->lpVtbl -> Compare(This,pTheirs,MatchType,pbResult) ) 

#define IMFStreamDescriptor_GetUINT32(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT32(This,guidKey,punValue) ) 

#define IMFStreamDescriptor_GetUINT64(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT64(This,guidKey,punValue) ) 

#define IMFStreamDescriptor_GetDouble(This,guidKey,pfValue)	\
    ( (This)->lpVtbl -> GetDouble(This,guidKey,pfValue) ) 

#define IMFStreamDescriptor_GetGUID(This,guidKey,pguidValue)	\
    ( (This)->lpVtbl -> GetGUID(This,guidKey,pguidValue) ) 

#define IMFStreamDescriptor_GetStringLength(This,guidKey,pcchLength)	\
    ( (This)->lpVtbl -> GetStringLength(This,guidKey,pcchLength) ) 

#define IMFStreamDescriptor_GetString(This,guidKey,pwszValue,cchBufSize,pcchLength)	\
    ( (This)->lpVtbl -> GetString(This,guidKey,pwszValue,cchBufSize,pcchLength) ) 

#define IMFStreamDescriptor_GetAllocatedString(This,guidKey,ppwszValue,pcchLength)	\
    ( (This)->lpVtbl -> GetAllocatedString(This,guidKey,ppwszValue,pcchLength) ) 

#define IMFStreamDescriptor_GetBlobSize(This,guidKey,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlobSize(This,guidKey,pcbBlobSize) ) 

#define IMFStreamDescriptor_GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize) ) 

#define IMFStreamDescriptor_GetAllocatedBlob(This,guidKey,ppBuf,pcbSize)	\
    ( (This)->lpVtbl -> GetAllocatedBlob(This,guidKey,ppBuf,pcbSize) ) 

#define IMFStreamDescriptor_GetUnknown(This,guidKey,riid,ppv)	\
    ( (This)->lpVtbl -> GetUnknown(This,guidKey,riid,ppv) ) 

#define IMFStreamDescriptor_SetItem(This,guidKey,Value)	\
    ( (This)->lpVtbl -> SetItem(This,guidKey,Value) ) 

#define IMFStreamDescriptor_DeleteItem(This,guidKey)	\
    ( (This)->lpVtbl -> DeleteItem(This,guidKey) ) 

#define IMFStreamDescriptor_DeleteAllItems(This)	\
    ( (This)->lpVtbl -> DeleteAllItems(This) ) 

#define IMFStreamDescriptor_SetUINT32(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT32(This,guidKey,unValue) ) 

#define IMFStreamDescriptor_SetUINT64(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT64(This,guidKey,unValue) ) 

#define IMFStreamDescriptor_SetDouble(This,guidKey,fValue)	\
    ( (This)->lpVtbl -> SetDouble(This,guidKey,fValue) ) 

#define IMFStreamDescriptor_SetGUID(This,guidKey,guidValue)	\
    ( (This)->lpVtbl -> SetGUID(This,guidKey,guidValue) ) 

#define IMFStreamDescriptor_SetString(This,guidKey,wszValue)	\
    ( (This)->lpVtbl -> SetString(This,guidKey,wszValue) ) 

#define IMFStreamDescriptor_SetBlob(This,guidKey,pBuf,cbBufSize)	\
    ( (This)->lpVtbl -> SetBlob(This,guidKey,pBuf,cbBufSize) ) 

#define IMFStreamDescriptor_SetUnknown(This,guidKey,pUnknown)	\
    ( (This)->lpVtbl -> SetUnknown(This,guidKey,pUnknown) ) 

#define IMFStreamDescriptor_LockStore(This)	\
    ( (This)->lpVtbl -> LockStore(This) ) 

#define IMFStreamDescriptor_UnlockStore(This)	\
    ( (This)->lpVtbl -> UnlockStore(This) ) 

#define IMFStreamDescriptor_GetCount(This,pcItems)	\
    ( (This)->lpVtbl -> GetCount(This,pcItems) ) 

#define IMFStreamDescriptor_GetItemByIndex(This,unIndex,pguidKey,pValue)	\
    ( (This)->lpVtbl -> GetItemByIndex(This,unIndex,pguidKey,pValue) ) 

#define IMFStreamDescriptor_CopyAllItems(This,pDest)	\
    ( (This)->lpVtbl -> CopyAllItems(This,pDest) ) 


#define IMFStreamDescriptor_GetStreamIdentifier(This,pdwStreamIdentifier)	\
    ( (This)->lpVtbl -> GetStreamIdentifier(This,pdwStreamIdentifier) ) 

#define IMFStreamDescriptor_GetMediaTypeHandler(This,ppMediaTypeHandler)	\
    ( (This)->lpVtbl -> GetMediaTypeHandler(This,ppMediaTypeHandler) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFStreamDescriptor_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0016 */
/* [local] */ 

STDAPI MFCreateStreamDescriptor(
    DWORD dwStreamIdentifier,
    DWORD cMediaTypes,
    __in_ecount(cMediaTypes) IMFMediaType** apMediaTypes,
    __out IMFStreamDescriptor** ppDescriptor
    );



extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0016_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0016_v0_0_s_ifspec;

#ifndef __IMFMediaTypeHandler_INTERFACE_DEFINED__
#define __IMFMediaTypeHandler_INTERFACE_DEFINED__

/* interface IMFMediaTypeHandler */
/* [uuid][object] */ 






EXTERN_C const IID IID_IMFMediaTypeHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e93dcf6c-4b07-4e1e-8123-aa16ed6eadf5")
    IMFMediaTypeHandler : public IUnknown
    {
    public:
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE IsMediaTypeSupported( 
            /* [in] */ IMFMediaType *pMediaType,
            /* [out] */ 
            __out  IMFMediaType **ppMediaType) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetMediaTypeCount( 
            /* [out] */ 
            __out  DWORD *pdwTypeCount) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetMediaTypeByIndex( 
            /* [in] */ DWORD dwIndex,
            /* [out] */ 
            __out  IMFMediaType **ppType) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE SetCurrentMediaType( 
            /* [in] */ IMFMediaType *pMediaType) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetCurrentMediaType( 
            /* [out] */ 
            __out  IMFMediaType **ppMediaType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMajorType( 
            /* [out] */ 
            __out  GUID *pguidMajorType) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFMediaTypeHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFMediaTypeHandler * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFMediaTypeHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFMediaTypeHandler * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *IsMediaTypeSupported )( 
            IMFMediaTypeHandler * This,
            /* [in] */ IMFMediaType *pMediaType,
            /* [out] */ 
            __out  IMFMediaType **ppMediaType);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *GetMediaTypeCount )( 
            IMFMediaTypeHandler * This,
            /* [out] */ 
            __out  DWORD *pdwTypeCount);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *GetMediaTypeByIndex )( 
            IMFMediaTypeHandler * This,
            /* [in] */ DWORD dwIndex,
            /* [out] */ 
            __out  IMFMediaType **ppType);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentMediaType )( 
            IMFMediaTypeHandler * This,
            /* [in] */ IMFMediaType *pMediaType);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *GetCurrentMediaType )( 
            IMFMediaTypeHandler * This,
            /* [out] */ 
            __out  IMFMediaType **ppMediaType);
        
        HRESULT ( STDMETHODCALLTYPE *GetMajorType )( 
            IMFMediaTypeHandler * This,
            /* [out] */ 
            __out  GUID *pguidMajorType);
        
        END_INTERFACE
    } IMFMediaTypeHandlerVtbl;

    interface IMFMediaTypeHandler
    {
        CONST_VTBL struct IMFMediaTypeHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFMediaTypeHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFMediaTypeHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFMediaTypeHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFMediaTypeHandler_IsMediaTypeSupported(This,pMediaType,ppMediaType)	\
    ( (This)->lpVtbl -> IsMediaTypeSupported(This,pMediaType,ppMediaType) ) 

#define IMFMediaTypeHandler_GetMediaTypeCount(This,pdwTypeCount)	\
    ( (This)->lpVtbl -> GetMediaTypeCount(This,pdwTypeCount) ) 

#define IMFMediaTypeHandler_GetMediaTypeByIndex(This,dwIndex,ppType)	\
    ( (This)->lpVtbl -> GetMediaTypeByIndex(This,dwIndex,ppType) ) 

#define IMFMediaTypeHandler_SetCurrentMediaType(This,pMediaType)	\
    ( (This)->lpVtbl -> SetCurrentMediaType(This,pMediaType) ) 

#define IMFMediaTypeHandler_GetCurrentMediaType(This,ppMediaType)	\
    ( (This)->lpVtbl -> GetCurrentMediaType(This,ppMediaType) ) 

#define IMFMediaTypeHandler_GetMajorType(This,pguidMajorType)	\
    ( (This)->lpVtbl -> GetMajorType(This,pguidMajorType) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFMediaTypeHandler_RemoteGetCurrentMediaType_Proxy( 
    IMFMediaTypeHandler * This,
    /* [size_is][size_is][out] */ 
    __out  BYTE **ppbData,
    /* [out] */ __RPC__out DWORD *pcbData);


void __RPC_STUB IMFMediaTypeHandler_RemoteGetCurrentMediaType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMFMediaTypeHandler_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0017 */
/* [local] */ 

STDAPI MFCreateSimpleTypeHandler(
    __out IMFMediaTypeHandler ** ppHandler );
typedef 
enum MFTIMER_FLAGS
    {	MFTIMER_RELATIVE	= 0x1
    } 	MFTIMER_FLAGS;



extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0017_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0017_v0_0_s_ifspec;

#ifndef __IMFTimer_INTERFACE_DEFINED__
#define __IMFTimer_INTERFACE_DEFINED__

/* interface IMFTimer */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFTimer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e56e4cbd-8f70-49d8-a0f8-edb3d6ab9bf2")
    IMFTimer : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetTimer( 
            /* [in] */ DWORD dwFlags,
            /* [in] */ LONGLONG llClockTime,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState,
            /* [out] */ IUnknown **ppunkKey) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CancelTimer( 
            /* [in] */ IUnknown *punkKey) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFTimerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFTimer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFTimer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFTimer * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetTimer )( 
            IMFTimer * This,
            /* [in] */ DWORD dwFlags,
            /* [in] */ LONGLONG llClockTime,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState,
            /* [out] */ IUnknown **ppunkKey);
        
        HRESULT ( STDMETHODCALLTYPE *CancelTimer )( 
            IMFTimer * This,
            /* [in] */ IUnknown *punkKey);
        
        END_INTERFACE
    } IMFTimerVtbl;

    interface IMFTimer
    {
        CONST_VTBL struct IMFTimerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFTimer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFTimer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFTimer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFTimer_SetTimer(This,dwFlags,llClockTime,pCallback,punkState,ppunkKey)	\
    ( (This)->lpVtbl -> SetTimer(This,dwFlags,llClockTime,pCallback,punkState,ppunkKey) ) 

#define IMFTimer_CancelTimer(This,punkKey)	\
    ( (This)->lpVtbl -> CancelTimer(This,punkKey) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFTimer_INTERFACE_DEFINED__ */


#ifndef __IMFActivate_INTERFACE_DEFINED__
#define __IMFActivate_INTERFACE_DEFINED__

/* interface IMFActivate */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMFActivate;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7FEE9E9A-4A89-47a6-899C-B6A53A70FB67")
    IMFActivate : public IMFAttributes
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ActivateObject( 
            /* [in] */ __RPC__in REFIID riid,
            /* [retval][iid_is][out] */ __RPC__deref_out_opt void **ppv) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ShutdownObject( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DetachObject( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFActivateVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFActivate * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFActivate * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFActivate * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetItem )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            /* [full][out][in] */ __RPC__inout_opt PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemType )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out MF_ATTRIBUTE_TYPE *pType);
        
        HRESULT ( STDMETHODCALLTYPE *CompareItem )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFPROPVARIANT Value,
            /* [out] */ __RPC__out BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *Compare )( 
            IMFActivate * This,
            __RPC__in_opt IMFAttributes *pTheirs,
            MF_ATTRIBUTES_MATCH_TYPE MatchType,
            /* [out] */ __RPC__out BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT32 )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT64 )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT64 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetDouble )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out double *pfValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetGUID )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out GUID *pguidValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetStringLength )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetString )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][out] */ __RPC__out_ecount_full(cchBufSize) LPWSTR pwszValue,
            UINT32 cchBufSize,
            /* [full][out][in] */ __RPC__inout_opt UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedString )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(( *pcchLength + 1 ) ) LPWSTR *ppwszValue,
            /* [out] */ __RPC__out UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlobSize )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            /* [out] */ __RPC__out UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlob )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][out] */ __RPC__out_ecount_full(cbBufSize) UINT8 *pBuf,
            UINT32 cbBufSize,
            /* [full][out][in] */ __RPC__inout_opt UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedBlob )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbSize) UINT8 **ppBuf,
            /* [out] */ __RPC__out UINT32 *pcbSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetUnknown )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFIID riid,
            /* [iid_is][out] */ __RPC__deref_out_opt LPVOID *ppv);
        
        HRESULT ( STDMETHODCALLTYPE *SetItem )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFPROPVARIANT Value);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteItem )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteAllItems )( 
            IMFActivate * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT32 )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            UINT32 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT64 )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            UINT64 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetDouble )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            double fValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetGUID )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            __RPC__in REFGUID guidValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetString )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            /* [string][in] */ __RPC__in LPCWSTR wszValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetBlob )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            /* [size_is][in] */ __RPC__in_ecount_full(cbBufSize) const UINT8 *pBuf,
            UINT32 cbBufSize);
        
        HRESULT ( STDMETHODCALLTYPE *SetUnknown )( 
            IMFActivate * This,
            __RPC__in REFGUID guidKey,
            /* [in] */ __RPC__in_opt IUnknown *pUnknown);
        
        HRESULT ( STDMETHODCALLTYPE *LockStore )( 
            IMFActivate * This);
        
        HRESULT ( STDMETHODCALLTYPE *UnlockStore )( 
            IMFActivate * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IMFActivate * This,
            /* [out] */ __RPC__out UINT32 *pcItems);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemByIndex )( 
            IMFActivate * This,
            UINT32 unIndex,
            /* [out] */ __RPC__out GUID *pguidKey,
            /* [full][out][in] */ __RPC__inout_opt PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *CopyAllItems )( 
            IMFActivate * This,
            /* [in] */ __RPC__in_opt IMFAttributes *pDest);
        
        HRESULT ( STDMETHODCALLTYPE *ActivateObject )( 
            IMFActivate * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [retval][iid_is][out] */ __RPC__deref_out_opt void **ppv);
        
        HRESULT ( STDMETHODCALLTYPE *ShutdownObject )( 
            IMFActivate * This);
        
        HRESULT ( STDMETHODCALLTYPE *DetachObject )( 
            IMFActivate * This);
        
        END_INTERFACE
    } IMFActivateVtbl;

    interface IMFActivate
    {
        CONST_VTBL struct IMFActivateVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFActivate_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFActivate_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFActivate_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFActivate_GetItem(This,guidKey,pValue)	\
    ( (This)->lpVtbl -> GetItem(This,guidKey,pValue) ) 

#define IMFActivate_GetItemType(This,guidKey,pType)	\
    ( (This)->lpVtbl -> GetItemType(This,guidKey,pType) ) 

#define IMFActivate_CompareItem(This,guidKey,Value,pbResult)	\
    ( (This)->lpVtbl -> CompareItem(This,guidKey,Value,pbResult) ) 

#define IMFActivate_Compare(This,pTheirs,MatchType,pbResult)	\
    ( (This)->lpVtbl -> Compare(This,pTheirs,MatchType,pbResult) ) 

#define IMFActivate_GetUINT32(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT32(This,guidKey,punValue) ) 

#define IMFActivate_GetUINT64(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT64(This,guidKey,punValue) ) 

#define IMFActivate_GetDouble(This,guidKey,pfValue)	\
    ( (This)->lpVtbl -> GetDouble(This,guidKey,pfValue) ) 

#define IMFActivate_GetGUID(This,guidKey,pguidValue)	\
    ( (This)->lpVtbl -> GetGUID(This,guidKey,pguidValue) ) 

#define IMFActivate_GetStringLength(This,guidKey,pcchLength)	\
    ( (This)->lpVtbl -> GetStringLength(This,guidKey,pcchLength) ) 

#define IMFActivate_GetString(This,guidKey,pwszValue,cchBufSize,pcchLength)	\
    ( (This)->lpVtbl -> GetString(This,guidKey,pwszValue,cchBufSize,pcchLength) ) 

#define IMFActivate_GetAllocatedString(This,guidKey,ppwszValue,pcchLength)	\
    ( (This)->lpVtbl -> GetAllocatedString(This,guidKey,ppwszValue,pcchLength) ) 

#define IMFActivate_GetBlobSize(This,guidKey,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlobSize(This,guidKey,pcbBlobSize) ) 

#define IMFActivate_GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize) ) 

#define IMFActivate_GetAllocatedBlob(This,guidKey,ppBuf,pcbSize)	\
    ( (This)->lpVtbl -> GetAllocatedBlob(This,guidKey,ppBuf,pcbSize) ) 

#define IMFActivate_GetUnknown(This,guidKey,riid,ppv)	\
    ( (This)->lpVtbl -> GetUnknown(This,guidKey,riid,ppv) ) 

#define IMFActivate_SetItem(This,guidKey,Value)	\
    ( (This)->lpVtbl -> SetItem(This,guidKey,Value) ) 

#define IMFActivate_DeleteItem(This,guidKey)	\
    ( (This)->lpVtbl -> DeleteItem(This,guidKey) ) 

#define IMFActivate_DeleteAllItems(This)	\
    ( (This)->lpVtbl -> DeleteAllItems(This) ) 

#define IMFActivate_SetUINT32(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT32(This,guidKey,unValue) ) 

#define IMFActivate_SetUINT64(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT64(This,guidKey,unValue) ) 

#define IMFActivate_SetDouble(This,guidKey,fValue)	\
    ( (This)->lpVtbl -> SetDouble(This,guidKey,fValue) ) 

#define IMFActivate_SetGUID(This,guidKey,guidValue)	\
    ( (This)->lpVtbl -> SetGUID(This,guidKey,guidValue) ) 

#define IMFActivate_SetString(This,guidKey,wszValue)	\
    ( (This)->lpVtbl -> SetString(This,guidKey,wszValue) ) 

#define IMFActivate_SetBlob(This,guidKey,pBuf,cbBufSize)	\
    ( (This)->lpVtbl -> SetBlob(This,guidKey,pBuf,cbBufSize) ) 

#define IMFActivate_SetUnknown(This,guidKey,pUnknown)	\
    ( (This)->lpVtbl -> SetUnknown(This,guidKey,pUnknown) ) 

#define IMFActivate_LockStore(This)	\
    ( (This)->lpVtbl -> LockStore(This) ) 

#define IMFActivate_UnlockStore(This)	\
    ( (This)->lpVtbl -> UnlockStore(This) ) 

#define IMFActivate_GetCount(This,pcItems)	\
    ( (This)->lpVtbl -> GetCount(This,pcItems) ) 

#define IMFActivate_GetItemByIndex(This,unIndex,pguidKey,pValue)	\
    ( (This)->lpVtbl -> GetItemByIndex(This,unIndex,pguidKey,pValue) ) 

#define IMFActivate_CopyAllItems(This,pDest)	\
    ( (This)->lpVtbl -> CopyAllItems(This,pDest) ) 


#define IMFActivate_ActivateObject(This,riid,ppv)	\
    ( (This)->lpVtbl -> ActivateObject(This,riid,ppv) ) 

#define IMFActivate_ShutdownObject(This)	\
    ( (This)->lpVtbl -> ShutdownObject(This) ) 

#define IMFActivate_DetachObject(This)	\
    ( (This)->lpVtbl -> DetachObject(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFActivate_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0019 */
/* [local] */ 

EXTERN_GUID( MF_ACTIVATE_CUSTOM_VIDEO_MIXER_CLSID,          0xba491360, 0xbe50, 0x451e, 0x95, 0xab, 0x6d, 0x4a, 0xcc, 0xc7, 0xda, 0xd8 );
EXTERN_GUID( MF_ACTIVATE_CUSTOM_VIDEO_MIXER_ACTIVATE,       0xba491361, 0xbe50, 0x451e, 0x95, 0xab, 0x6d, 0x4a, 0xcc, 0xc7, 0xda, 0xd8 );
EXTERN_GUID( MF_ACTIVATE_CUSTOM_VIDEO_MIXER_FLAGS,          0xba491362, 0xbe50, 0x451e, 0x95, 0xab, 0x6d, 0x4a, 0xcc, 0xc7, 0xda, 0xd8 );
EXTERN_GUID( MF_ACTIVATE_CUSTOM_VIDEO_PRESENTER_CLSID,      0xba491364, 0xbe50, 0x451e, 0x95, 0xab, 0x6d, 0x4a, 0xcc, 0xc7, 0xda, 0xd8 );
EXTERN_GUID( MF_ACTIVATE_CUSTOM_VIDEO_PRESENTER_ACTIVATE,   0xba491365, 0xbe50, 0x451e, 0x95, 0xab, 0x6d, 0x4a, 0xcc, 0xc7, 0xda, 0xd8 );
EXTERN_GUID( MF_ACTIVATE_CUSTOM_VIDEO_PRESENTER_FLAGS,      0xba491366, 0xbe50, 0x451e, 0x95, 0xab, 0x6d, 0x4a, 0xcc, 0xc7, 0xda, 0xd8 );

enum __MIDL___MIDL_itf_mfidl_0000_0019_0001
    {	MF_ACTIVATE_CUSTOM_MIXER_ALLOWFAIL	= 0x1
    } ;

enum __MIDL___MIDL_itf_mfidl_0000_0019_0002
    {	MF_ACTIVATE_CUSTOM_PRESENTER_ALLOWFAIL	= 0x1
    } ;
EXTERN_GUID( MF_ACTIVATE_MFT_LOCKED,  0xc1f6093c, 0x7f65, 0x4fbd, 0x9e, 0x39, 0x5f, 0xae, 0xc3, 0xc4, 0xfb, 0xd7 );
EXTERN_GUID( MF_ACTIVATE_VIDEO_WINDOW, 0x9a2dbbdd, 0xf57e, 0x4162, 0x82, 0xb9, 0x68, 0x31, 0x37, 0x76, 0x82, 0xd3 );
typedef 
enum _MFSHUTDOWN_STATUS
    {	MFSHUTDOWN_INITIATED	= 0,
	MFSHUTDOWN_COMPLETED	= ( MFSHUTDOWN_INITIATED + 1 ) 
    } 	MFSHUTDOWN_STATUS;



extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0019_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0019_v0_0_s_ifspec;

#ifndef __IMFShutdown_INTERFACE_DEFINED__
#define __IMFShutdown_INTERFACE_DEFINED__

/* interface IMFShutdown */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFShutdown;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("97ec2ea4-0e42-4937-97ac-9d6d328824e1")
    IMFShutdown : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Shutdown( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetShutdownStatus( 
            /* [out] */ __RPC__out MFSHUTDOWN_STATUS *pStatus) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFShutdownVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFShutdown * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFShutdown * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFShutdown * This);
        
        HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            IMFShutdown * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetShutdownStatus )( 
            IMFShutdown * This,
            /* [out] */ __RPC__out MFSHUTDOWN_STATUS *pStatus);
        
        END_INTERFACE
    } IMFShutdownVtbl;

    interface IMFShutdown
    {
        CONST_VTBL struct IMFShutdownVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFShutdown_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFShutdown_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFShutdown_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFShutdown_Shutdown(This)	\
    ( (This)->lpVtbl -> Shutdown(This) ) 

#define IMFShutdown_GetShutdownStatus(This,pStatus)	\
    ( (This)->lpVtbl -> GetShutdownStatus(This,pStatus) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFShutdown_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0020 */
/* [local] */ 

STDAPI 
MFShutdownObject(
    IUnknown * pUnk );
STDAPI
MFCreateAudioRenderer(
    IMFAttributes* pAudioAttributes,
    __out IMFMediaSink** ppSink
    );
STDAPI
MFCreateAudioRendererActivate( 
    __out IMFActivate ** ppActivate 
    );
EXTERN_GUID( MF_AUDIO_RENDERER_ATTRIBUTE_FLAGS, 0xede4b5e0, 0xf805, 0x4d6c, 0x99, 0xb3, 0xdb, 0x01, 0xbf, 0x95, 0xdf, 0xab);
#define    MF_AUDIO_RENDERER_ATTRIBUTE_FLAGS_CROSSPROCESS          0x00000001
#define    MF_AUDIO_RENDERER_ATTRIBUTE_FLAGS_NOPERSIST          0x00000002
EXTERN_GUID( MF_AUDIO_RENDERER_ATTRIBUTE_SESSION_ID, 0xede4b5e3, 0xf805, 0x4d6c, 0x99, 0xb3, 0xdb, 0x01, 0xbf, 0x95, 0xdf, 0xab);
EXTERN_GUID( MF_AUDIO_RENDERER_ATTRIBUTE_ENDPOINT_ID, 0xb10aaec3, 0xef71, 0x4cc3, 0xb8, 0x73, 0x5, 0xa9, 0xa0, 0x8b, 0x9f, 0x8e);
EXTERN_GUID( MF_AUDIO_RENDERER_ATTRIBUTE_ENDPOINT_ROLE, 0x6ba644ff, 0x27c5, 0x4d02, 0x98, 0x87, 0xc2, 0x86, 0x19, 0xfd, 0xb9, 0x1b);
STDAPI
MFCreateVideoRendererActivate(
    __in HWND hwndVideo, 
    __out IMFActivate ** ppActivate 
    );


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0020_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0020_v0_0_s_ifspec;

#ifndef __IMFTopoLoader_INTERFACE_DEFINED__
#define __IMFTopoLoader_INTERFACE_DEFINED__

/* interface IMFTopoLoader */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFTopoLoader;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("DE9A6157-F660-4643-B56A-DF9F7998C7CD")
    IMFTopoLoader : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ IMFTopology *pInputTopo,
            /* [out] */ 
            __out  IMFTopology **ppOutputTopo,
            /* [in] */ IMFTopology *pCurrentTopo) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFTopoLoaderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFTopoLoader * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFTopoLoader * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFTopoLoader * This);
        
        HRESULT ( STDMETHODCALLTYPE *Load )( 
            IMFTopoLoader * This,
            /* [in] */ IMFTopology *pInputTopo,
            /* [out] */ 
            __out  IMFTopology **ppOutputTopo,
            /* [in] */ IMFTopology *pCurrentTopo);
        
        END_INTERFACE
    } IMFTopoLoaderVtbl;

    interface IMFTopoLoader
    {
        CONST_VTBL struct IMFTopoLoaderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFTopoLoader_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFTopoLoader_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFTopoLoader_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFTopoLoader_Load(This,pInputTopo,ppOutputTopo,pCurrentTopo)	\
    ( (This)->lpVtbl -> Load(This,pInputTopo,ppOutputTopo,pCurrentTopo) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFTopoLoader_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0021 */
/* [local] */ 

STDAPI MFCreateTopoLoader(
    __out IMFTopoLoader ** ppObj );


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0021_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0021_v0_0_s_ifspec;

#ifndef __IMFContentProtectionManager_INTERFACE_DEFINED__
#define __IMFContentProtectionManager_INTERFACE_DEFINED__

/* interface IMFContentProtectionManager */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMFContentProtectionManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("ACF92459-6A61-42bd-B57C-B43E51203CB0")
    IMFContentProtectionManager : public IUnknown
    {
    public:
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE BeginEnableContent( 
            /* [in] */ IMFActivate *pEnablerActivate,
            /* [in] */ IMFTopology *pTopo,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE EndEnableContent( 
            /* [in] */ IMFAsyncResult *pResult) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFContentProtectionManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFContentProtectionManager * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFContentProtectionManager * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFContentProtectionManager * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *BeginEnableContent )( 
            IMFContentProtectionManager * This,
            /* [in] */ IMFActivate *pEnablerActivate,
            /* [in] */ IMFTopology *pTopo,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *EndEnableContent )( 
            IMFContentProtectionManager * This,
            /* [in] */ IMFAsyncResult *pResult);
        
        END_INTERFACE
    } IMFContentProtectionManagerVtbl;

    interface IMFContentProtectionManager
    {
        CONST_VTBL struct IMFContentProtectionManagerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFContentProtectionManager_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFContentProtectionManager_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFContentProtectionManager_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFContentProtectionManager_BeginEnableContent(This,pEnablerActivate,pTopo,pCallback,punkState)	\
    ( (This)->lpVtbl -> BeginEnableContent(This,pEnablerActivate,pTopo,pCallback,punkState) ) 

#define IMFContentProtectionManager_EndEnableContent(This,pResult)	\
    ( (This)->lpVtbl -> EndEnableContent(This,pResult) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFContentProtectionManager_RemoteBeginEnableContent_Proxy( 
    IMFContentProtectionManager * This,
    /* [in] */ __RPC__in REFCLSID clsidType,
    /* [size_is][in] */ __RPC__in_ecount_full(cbData) BYTE *pbData,
    /* [in] */ DWORD cbData,
    /* [in] */ __RPC__in_opt IMFRemoteAsyncCallback *pCallback);


void __RPC_STUB IMFContentProtectionManager_RemoteBeginEnableContent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFContentProtectionManager_RemoteEndEnableContent_Proxy( 
    IMFContentProtectionManager * This,
    /* [in] */ __RPC__in_opt IUnknown *pResult);


void __RPC_STUB IMFContentProtectionManager_RemoteEndEnableContent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMFContentProtectionManager_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0022 */
/* [local] */ 

typedef /* [public][public] */ 
enum __MIDL___MIDL_itf_mfidl_0000_0022_0001
    {	MF_LICENSE_URL_UNTRUSTED	= 0,
	MF_LICENSE_URL_TRUSTED	= ( MF_LICENSE_URL_UNTRUSTED + 1 ) ,
	MF_LICENSE_URL_TAMPERED	= ( MF_LICENSE_URL_TRUSTED + 1 ) 
    } 	MF_URL_TRUST_STATUS;



extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0022_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0022_v0_0_s_ifspec;

#ifndef __IMFContentEnabler_INTERFACE_DEFINED__
#define __IMFContentEnabler_INTERFACE_DEFINED__

/* interface IMFContentEnabler */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMFContentEnabler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D3C4EF59-49CE-4381-9071-D5BCD044C770")
    IMFContentEnabler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetEnableType( 
            /* [out] */ __RPC__out GUID *pType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEnableURL( 
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcchURL) LPWSTR *ppwszURL,
            /* [out] */ __RPC__out DWORD *pcchURL,
            /* [unique][out][in] */ __RPC__inout_opt MF_URL_TRUST_STATUS *pTrustStatus) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEnableData( 
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbData) BYTE **ppbData,
            /* [out] */ __RPC__out DWORD *pcbData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsAutomaticSupported( 
            /* [out] */ __RPC__out BOOL *pfAutomatic) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AutomaticEnable( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MonitorEnable( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Cancel( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFContentEnablerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFContentEnabler * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFContentEnabler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFContentEnabler * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetEnableType )( 
            IMFContentEnabler * This,
            /* [out] */ __RPC__out GUID *pType);
        
        HRESULT ( STDMETHODCALLTYPE *GetEnableURL )( 
            IMFContentEnabler * This,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcchURL) LPWSTR *ppwszURL,
            /* [out] */ __RPC__out DWORD *pcchURL,
            /* [unique][out][in] */ __RPC__inout_opt MF_URL_TRUST_STATUS *pTrustStatus);
        
        HRESULT ( STDMETHODCALLTYPE *GetEnableData )( 
            IMFContentEnabler * This,
            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbData) BYTE **ppbData,
            /* [out] */ __RPC__out DWORD *pcbData);
        
        HRESULT ( STDMETHODCALLTYPE *IsAutomaticSupported )( 
            IMFContentEnabler * This,
            /* [out] */ __RPC__out BOOL *pfAutomatic);
        
        HRESULT ( STDMETHODCALLTYPE *AutomaticEnable )( 
            IMFContentEnabler * This);
        
        HRESULT ( STDMETHODCALLTYPE *MonitorEnable )( 
            IMFContentEnabler * This);
        
        HRESULT ( STDMETHODCALLTYPE *Cancel )( 
            IMFContentEnabler * This);
        
        END_INTERFACE
    } IMFContentEnablerVtbl;

    interface IMFContentEnabler
    {
        CONST_VTBL struct IMFContentEnablerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFContentEnabler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFContentEnabler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFContentEnabler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFContentEnabler_GetEnableType(This,pType)	\
    ( (This)->lpVtbl -> GetEnableType(This,pType) ) 

#define IMFContentEnabler_GetEnableURL(This,ppwszURL,pcchURL,pTrustStatus)	\
    ( (This)->lpVtbl -> GetEnableURL(This,ppwszURL,pcchURL,pTrustStatus) ) 

#define IMFContentEnabler_GetEnableData(This,ppbData,pcbData)	\
    ( (This)->lpVtbl -> GetEnableData(This,ppbData,pcbData) ) 

#define IMFContentEnabler_IsAutomaticSupported(This,pfAutomatic)	\
    ( (This)->lpVtbl -> IsAutomaticSupported(This,pfAutomatic) ) 

#define IMFContentEnabler_AutomaticEnable(This)	\
    ( (This)->lpVtbl -> AutomaticEnable(This) ) 

#define IMFContentEnabler_MonitorEnable(This)	\
    ( (This)->lpVtbl -> MonitorEnable(This) ) 

#define IMFContentEnabler_Cancel(This)	\
    ( (This)->lpVtbl -> Cancel(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFContentEnabler_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0023 */
/* [local] */ 


EXTERN_GUID( MFENABLETYPE_WMDRMV1_LicenseAcquisition, 0x4ff6eeaf, 0xb43, 0x4797, 0x9b, 0x85, 0xab, 0xf3, 0x18, 0x15, 0xe7, 0xb0);
EXTERN_GUID( MFENABLETYPE_WMDRMV7_LicenseAcquisition, 0x3306df, 0x4a06, 0x4884,0xa0, 0x97, 0xef, 0x6d, 0x22, 0xec, 0x84, 0xa3);
EXTERN_GUID( MFENABLETYPE_WMDRMV7_Individualization, 0xacd2c84a, 0xb303, 0x4f65, 0xbc, 0x2c, 0x2c, 0x84, 0x8d, 0x1, 0xa9, 0x89);
EXTERN_GUID( MFENABLETYPE_MF_UpdateRevocationInformation, 0xe558b0b5, 0xb3c4, 0x44a0, 0x92, 0x4c, 0x50, 0xd1, 0x78, 0x93, 0x23, 0x85);
EXTERN_GUID( MFENABLETYPE_MF_UpdateUntrustedComponent, 0x9879f3d6, 0xcee2, 0x48e6, 0xb5, 0x73, 0x97, 0x67, 0xab, 0x17, 0x2f, 0x16);
EXTERN_GUID( MFENABLETYPE_MF_RebootRequired, 0x6d4d3d4b, 0x0ece, 0x4652, 0x8b, 0x3a, 0xf2, 0xd2, 0x42, 0x60, 0xd8, 0x87);
// 
// Structs that contain information about revoked or unsigned binaries, 
// returned by the IMFContentEnabler::GetEnableData() method of  
// the Revocation content enabler 
// 
#ifndef MFRR_INFO_VERSION
#define MFRR_INFO_VERSION 0
#endif
// 
// The values for MFRR_COMPONENT_HASH_INFO.ulReason 
// 
#define MF_USER_MODE_COMPONENT_LOAD        0x00000001
#define MF_KERNEL_MODE_COMPONENT_LOAD      0x00000002
#define MF_GRL_LOAD_FAILED                 0x00000010
#define MF_INVALID_GRL_SIGNATURE           0x00000020
#define MF_GRL_ABSENT                      0x00001000
#define MF_COMPONENT_REVOKED               0x00002000
#define MF_COMPONENT_INVALID_EKU           0x00004000
#define MF_COMPONENT_CERT_REVOKED          0x00008000
#define MF_COMPONENT_INVALID_ROOT          0x00010000
#define MF_COMPONENT_HS_CERT_REVOKED       0x00020000
#define MF_COMPONENT_LS_CERT_REVOKED       0x00040000
#define MF_BOOT_DRIVER_VERIFICATION_FAILED 0x00100000
#define MF_TEST_SIGNED_COMPONENT_LOADING   0x01000000
#define MF_MINCRYPT_FAILURE                0x10000000
// 
// STR_HASH_LEN: Number of characters required to represent a SHA-1 hash  
// as a string of the form "0x5a3b53463b672a4f..." 
// Each byte of a SHA-1 hash takes two characters to represent, and 
// we add in two leading characters "0x" as well as the NULL terminator 
// 
#define SHA_HASH_LEN   20 
#define STR_HASH_LEN   (SHA_HASH_LEN*2 + 3) 
typedef struct _MFRR_COMPONENT_HASH_INFO 
{ 
    // Reason for failure (revoked or unsigned or badly signed).   
    DWORD ulReason; 
 
    // Header hash of the component 
    WCHAR rgHeaderHash[STR_HASH_LEN];  
 
    // Hash of public key if one of the certificates  
    // in the signing certificate chain is revoked 
    WCHAR rgPublicKeyHash[STR_HASH_LEN];  
 
    // Component name (full path name) 
    WCHAR wszName[MAX_PATH];     
 
}   MFRR_COMPONENT_HASH_INFO, *PMFRR_COMPONENT_HASH_INFO; 
typedef struct _MFRR_COMPONENTS 
{ 
 
    // Version number  
    DWORD dwRRInfoVersion; 
 
    // Number of components in list 
    DWORD dwRRComponents; 
 
    // points to the end of this structure that has  
    // allocated memory for the array of component info structures 
    PMFRR_COMPONENT_HASH_INFO pRRComponents;  
 
}   MFRR_COMPONENTS, *PMFRR_COMPONENTS; 
#pragma pack ( push ) 
#pragma pack ( 1 ) 
typedef struct _ASFFlatPicture 
{ 
    // 
    // Direct mapped fields 
    // 
    BYTE bPictureType; 
    DWORD dwDataLen; 
}   ASF_FLAT_PICTURE; 
#pragma pack ( pop ) 
#pragma pack ( push ) 
#pragma pack ( 1 ) 
typedef struct _ASFFlatSynchronisedLyrics 
{ 
    // 
    // Direct mapped fields 
    // 
    BYTE bTimeStampFormat; 
    BYTE bContentType; 
    DWORD dwLyricsLen; 
}   ASF_FLAT_SYNCHRONISED_LYRICS; 
#pragma pack ( pop ) 


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0023_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0023_v0_0_s_ifspec;

#ifndef __IMFMetadata_INTERFACE_DEFINED__
#define __IMFMetadata_INTERFACE_DEFINED__

/* interface IMFMetadata */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFMetadata;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F88CFB8C-EF16-4991-B450-CB8C69E51704")
    IMFMetadata : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetLanguage( 
            /* [in] */ __RPC__in LPCWSTR pwszRFC1766) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLanguage( 
            /* [out] */ __RPC__deref_out_opt LPWSTR *ppwszRFC1766) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAllLanguages( 
            /* [out] */ __RPC__out PROPVARIANT *ppvLanguages) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetProperty( 
            /* [in] */ __RPC__in LPCWSTR pwszName,
            /* [in] */ __RPC__in const PROPVARIANT *ppvValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProperty( 
            /* [in] */ __RPC__in LPCWSTR pwszName,
            /* [out] */ __RPC__out PROPVARIANT *ppvValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteProperty( 
            /* [in] */ __RPC__in LPCWSTR pwszName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAllPropertyNames( 
            /* [out] */ __RPC__out PROPVARIANT *ppvNames) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFMetadataVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFMetadata * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFMetadata * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFMetadata * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetLanguage )( 
            IMFMetadata * This,
            /* [in] */ __RPC__in LPCWSTR pwszRFC1766);
        
        HRESULT ( STDMETHODCALLTYPE *GetLanguage )( 
            IMFMetadata * This,
            /* [out] */ __RPC__deref_out_opt LPWSTR *ppwszRFC1766);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllLanguages )( 
            IMFMetadata * This,
            /* [out] */ __RPC__out PROPVARIANT *ppvLanguages);
        
        HRESULT ( STDMETHODCALLTYPE *SetProperty )( 
            IMFMetadata * This,
            /* [in] */ __RPC__in LPCWSTR pwszName,
            /* [in] */ __RPC__in const PROPVARIANT *ppvValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetProperty )( 
            IMFMetadata * This,
            /* [in] */ __RPC__in LPCWSTR pwszName,
            /* [out] */ __RPC__out PROPVARIANT *ppvValue);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteProperty )( 
            IMFMetadata * This,
            /* [in] */ __RPC__in LPCWSTR pwszName);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllPropertyNames )( 
            IMFMetadata * This,
            /* [out] */ __RPC__out PROPVARIANT *ppvNames);
        
        END_INTERFACE
    } IMFMetadataVtbl;

    interface IMFMetadata
    {
        CONST_VTBL struct IMFMetadataVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFMetadata_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFMetadata_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFMetadata_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFMetadata_SetLanguage(This,pwszRFC1766)	\
    ( (This)->lpVtbl -> SetLanguage(This,pwszRFC1766) ) 

#define IMFMetadata_GetLanguage(This,ppwszRFC1766)	\
    ( (This)->lpVtbl -> GetLanguage(This,ppwszRFC1766) ) 

#define IMFMetadata_GetAllLanguages(This,ppvLanguages)	\
    ( (This)->lpVtbl -> GetAllLanguages(This,ppvLanguages) ) 

#define IMFMetadata_SetProperty(This,pwszName,ppvValue)	\
    ( (This)->lpVtbl -> SetProperty(This,pwszName,ppvValue) ) 

#define IMFMetadata_GetProperty(This,pwszName,ppvValue)	\
    ( (This)->lpVtbl -> GetProperty(This,pwszName,ppvValue) ) 

#define IMFMetadata_DeleteProperty(This,pwszName)	\
    ( (This)->lpVtbl -> DeleteProperty(This,pwszName) ) 

#define IMFMetadata_GetAllPropertyNames(This,ppvNames)	\
    ( (This)->lpVtbl -> GetAllPropertyNames(This,ppvNames) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFMetadata_INTERFACE_DEFINED__ */


#ifndef __IMFMetadataProvider_INTERFACE_DEFINED__
#define __IMFMetadataProvider_INTERFACE_DEFINED__

/* interface IMFMetadataProvider */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFMetadataProvider;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56181D2D-E221-4adb-B1C8-3CEE6A53F76F")
    IMFMetadataProvider : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetMFMetadata( 
            /* [in] */ __RPC__in_opt IMFPresentationDescriptor *pPresentationDescriptor,
            /* [in] */ DWORD dwStreamIdentifier,
            /* [in] */ DWORD dwFlags,
            /* [out] */ __RPC__deref_out_opt IMFMetadata **ppMFMetadata) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFMetadataProviderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFMetadataProvider * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFMetadataProvider * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFMetadataProvider * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetMFMetadata )( 
            IMFMetadataProvider * This,
            /* [in] */ __RPC__in_opt IMFPresentationDescriptor *pPresentationDescriptor,
            /* [in] */ DWORD dwStreamIdentifier,
            /* [in] */ DWORD dwFlags,
            /* [out] */ __RPC__deref_out_opt IMFMetadata **ppMFMetadata);
        
        END_INTERFACE
    } IMFMetadataProviderVtbl;

    interface IMFMetadataProvider
    {
        CONST_VTBL struct IMFMetadataProviderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFMetadataProvider_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFMetadataProvider_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFMetadataProvider_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFMetadataProvider_GetMFMetadata(This,pPresentationDescriptor,dwStreamIdentifier,dwFlags,ppMFMetadata)	\
    ( (This)->lpVtbl -> GetMFMetadata(This,pPresentationDescriptor,dwStreamIdentifier,dwFlags,ppMFMetadata) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFMetadataProvider_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0025 */
/* [local] */ 

EXTERN_GUID( MF_METADATA_PROVIDER_SERVICE, 0xdb214084, 0x58a4, 0x4d2e, 0xb8, 0x4f, 0x6f, 0x75, 0x5b, 0x2f, 0x7a, 0xd);
typedef 
enum _MFRATE_DIRECTION
    {	MFRATE_FORWARD	= 0,
	MFRATE_REVERSE	= ( MFRATE_FORWARD + 1 ) 
    } 	MFRATE_DIRECTION;



extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0025_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0025_v0_0_s_ifspec;

#ifndef __IMFRateSupport_INTERFACE_DEFINED__
#define __IMFRateSupport_INTERFACE_DEFINED__

/* interface IMFRateSupport */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IMFRateSupport;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0a9ccdbc-d797-4563-9667-94ec5d79292d")
    IMFRateSupport : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetSlowestRate( 
            /* [in] */ MFRATE_DIRECTION eDirection,
            /* [in] */ BOOL fThin,
            /* [out] */ __RPC__out float *pflRate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFastestRate( 
            /* [in] */ MFRATE_DIRECTION eDirection,
            /* [in] */ BOOL fThin,
            /* [out] */ __RPC__out float *pflRate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsRateSupported( 
            /* [in] */ BOOL fThin,
            /* [in] */ float flRate,
            /* [unique][out][in] */ __RPC__inout_opt float *pflNearestSupportedRate) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFRateSupportVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFRateSupport * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFRateSupport * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFRateSupport * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetSlowestRate )( 
            IMFRateSupport * This,
            /* [in] */ MFRATE_DIRECTION eDirection,
            /* [in] */ BOOL fThin,
            /* [out] */ __RPC__out float *pflRate);
        
        HRESULT ( STDMETHODCALLTYPE *GetFastestRate )( 
            IMFRateSupport * This,
            /* [in] */ MFRATE_DIRECTION eDirection,
            /* [in] */ BOOL fThin,
            /* [out] */ __RPC__out float *pflRate);
        
        HRESULT ( STDMETHODCALLTYPE *IsRateSupported )( 
            IMFRateSupport * This,
            /* [in] */ BOOL fThin,
            /* [in] */ float flRate,
            /* [unique][out][in] */ __RPC__inout_opt float *pflNearestSupportedRate);
        
        END_INTERFACE
    } IMFRateSupportVtbl;

    interface IMFRateSupport
    {
        CONST_VTBL struct IMFRateSupportVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFRateSupport_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFRateSupport_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFRateSupport_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFRateSupport_GetSlowestRate(This,eDirection,fThin,pflRate)	\
    ( (This)->lpVtbl -> GetSlowestRate(This,eDirection,fThin,pflRate) ) 

#define IMFRateSupport_GetFastestRate(This,eDirection,fThin,pflRate)	\
    ( (This)->lpVtbl -> GetFastestRate(This,eDirection,fThin,pflRate) ) 

#define IMFRateSupport_IsRateSupported(This,fThin,flRate,pflNearestSupportedRate)	\
    ( (This)->lpVtbl -> IsRateSupported(This,fThin,flRate,pflNearestSupportedRate) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFRateSupport_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0026 */
/* [local] */ 

EXTERN_GUID( MF_RATE_CONTROL_SERVICE, 0x866fa297, 0xb802, 0x4bf8, 0x9d, 0xc9, 0x5e, 0x3b, 0x6a, 0x9f, 0x53, 0xc9);


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0026_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0026_v0_0_s_ifspec;

#ifndef __IMFRateControl_INTERFACE_DEFINED__
#define __IMFRateControl_INTERFACE_DEFINED__

/* interface IMFRateControl */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IMFRateControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("88ddcd21-03c3-4275-91ed-55ee3929328f")
    IMFRateControl : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetRate( 
            /* [in] */ BOOL fThin,
            /* [in] */ float flRate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRate( 
            /* [unique][out][in] */ __RPC__inout_opt BOOL *pfThin,
            /* [unique][out][in] */ __RPC__inout_opt float *pflRate) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFRateControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFRateControl * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFRateControl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFRateControl * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetRate )( 
            IMFRateControl * This,
            /* [in] */ BOOL fThin,
            /* [in] */ float flRate);
        
        HRESULT ( STDMETHODCALLTYPE *GetRate )( 
            IMFRateControl * This,
            /* [unique][out][in] */ __RPC__inout_opt BOOL *pfThin,
            /* [unique][out][in] */ __RPC__inout_opt float *pflRate);
        
        END_INTERFACE
    } IMFRateControlVtbl;

    interface IMFRateControl
    {
        CONST_VTBL struct IMFRateControlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFRateControl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFRateControl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFRateControl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFRateControl_SetRate(This,fThin,flRate)	\
    ( (This)->lpVtbl -> SetRate(This,fThin,flRate) ) 

#define IMFRateControl_GetRate(This,pfThin,pflRate)	\
    ( (This)->lpVtbl -> GetRate(This,pfThin,pflRate) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFRateControl_INTERFACE_DEFINED__ */


#ifndef __IMFSimpleAudioVolume_INTERFACE_DEFINED__
#define __IMFSimpleAudioVolume_INTERFACE_DEFINED__

/* interface IMFSimpleAudioVolume */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFSimpleAudioVolume;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("089EDF13-CF71-4338-8D13-9E569DBDC319")
    IMFSimpleAudioVolume : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetMasterVolume( 
            /* [in] */ float fLevel) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMasterVolume( 
            /* [out] */ __RPC__out float *pfLevel) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetMute( 
            /* [in] */ const BOOL bMute) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMute( 
            /* [out] */ __RPC__out BOOL *pbMute) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFSimpleAudioVolumeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFSimpleAudioVolume * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFSimpleAudioVolume * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFSimpleAudioVolume * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetMasterVolume )( 
            IMFSimpleAudioVolume * This,
            /* [in] */ float fLevel);
        
        HRESULT ( STDMETHODCALLTYPE *GetMasterVolume )( 
            IMFSimpleAudioVolume * This,
            /* [out] */ __RPC__out float *pfLevel);
        
        HRESULT ( STDMETHODCALLTYPE *SetMute )( 
            IMFSimpleAudioVolume * This,
            /* [in] */ const BOOL bMute);
        
        HRESULT ( STDMETHODCALLTYPE *GetMute )( 
            IMFSimpleAudioVolume * This,
            /* [out] */ __RPC__out BOOL *pbMute);
        
        END_INTERFACE
    } IMFSimpleAudioVolumeVtbl;

    interface IMFSimpleAudioVolume
    {
        CONST_VTBL struct IMFSimpleAudioVolumeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFSimpleAudioVolume_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFSimpleAudioVolume_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFSimpleAudioVolume_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFSimpleAudioVolume_SetMasterVolume(This,fLevel)	\
    ( (This)->lpVtbl -> SetMasterVolume(This,fLevel) ) 

#define IMFSimpleAudioVolume_GetMasterVolume(This,pfLevel)	\
    ( (This)->lpVtbl -> GetMasterVolume(This,pfLevel) ) 

#define IMFSimpleAudioVolume_SetMute(This,bMute)	\
    ( (This)->lpVtbl -> SetMute(This,bMute) ) 

#define IMFSimpleAudioVolume_GetMute(This,pbMute)	\
    ( (This)->lpVtbl -> GetMute(This,pbMute) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFSimpleAudioVolume_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0028 */
/* [local] */ 

EXTERN_GUID( MR_POLICY_VOLUME_SERVICE, 0x1abaa2ac, 0x9d3b, 0x47c6, 0xab, 0x48, 0xc5, 0x95, 0x6, 0xde, 0x78, 0x4d);


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0028_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0028_v0_0_s_ifspec;

#ifndef __IMFAudioStreamVolume_INTERFACE_DEFINED__
#define __IMFAudioStreamVolume_INTERFACE_DEFINED__

/* interface IMFAudioStreamVolume */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFAudioStreamVolume;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("76B1BBDB-4EC8-4f36-B106-70A9316DF593")
    IMFAudioStreamVolume : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetChannelCount( 
            /* [out] */ __RPC__out UINT32 *pdwCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetChannelVolume( 
            /* [in] */ UINT32 dwIndex,
            /* [in] */ const float fLevel) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetChannelVolume( 
            /* [in] */ UINT32 dwIndex,
            /* [out] */ __RPC__out float *pfLevel) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetAllVolumes( 
            /* [in] */ UINT32 dwCount,
            /* [size_is][in] */ __RPC__in_ecount_full(dwCount) const float *pfVolumes) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAllVolumes( 
            /* [in] */ UINT32 dwCount,
            /* [size_is][out] */ __RPC__out_ecount_full(dwCount) float *pfVolumes) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFAudioStreamVolumeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFAudioStreamVolume * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFAudioStreamVolume * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFAudioStreamVolume * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetChannelCount )( 
            IMFAudioStreamVolume * This,
            /* [out] */ __RPC__out UINT32 *pdwCount);
        
        HRESULT ( STDMETHODCALLTYPE *SetChannelVolume )( 
            IMFAudioStreamVolume * This,
            /* [in] */ UINT32 dwIndex,
            /* [in] */ const float fLevel);
        
        HRESULT ( STDMETHODCALLTYPE *GetChannelVolume )( 
            IMFAudioStreamVolume * This,
            /* [in] */ UINT32 dwIndex,
            /* [out] */ __RPC__out float *pfLevel);
        
        HRESULT ( STDMETHODCALLTYPE *SetAllVolumes )( 
            IMFAudioStreamVolume * This,
            /* [in] */ UINT32 dwCount,
            /* [size_is][in] */ __RPC__in_ecount_full(dwCount) const float *pfVolumes);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllVolumes )( 
            IMFAudioStreamVolume * This,
            /* [in] */ UINT32 dwCount,
            /* [size_is][out] */ __RPC__out_ecount_full(dwCount) float *pfVolumes);
        
        END_INTERFACE
    } IMFAudioStreamVolumeVtbl;

    interface IMFAudioStreamVolume
    {
        CONST_VTBL struct IMFAudioStreamVolumeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFAudioStreamVolume_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFAudioStreamVolume_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFAudioStreamVolume_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFAudioStreamVolume_GetChannelCount(This,pdwCount)	\
    ( (This)->lpVtbl -> GetChannelCount(This,pdwCount) ) 

#define IMFAudioStreamVolume_SetChannelVolume(This,dwIndex,fLevel)	\
    ( (This)->lpVtbl -> SetChannelVolume(This,dwIndex,fLevel) ) 

#define IMFAudioStreamVolume_GetChannelVolume(This,dwIndex,pfLevel)	\
    ( (This)->lpVtbl -> GetChannelVolume(This,dwIndex,pfLevel) ) 

#define IMFAudioStreamVolume_SetAllVolumes(This,dwCount,pfVolumes)	\
    ( (This)->lpVtbl -> SetAllVolumes(This,dwCount,pfVolumes) ) 

#define IMFAudioStreamVolume_GetAllVolumes(This,dwCount,pfVolumes)	\
    ( (This)->lpVtbl -> GetAllVolumes(This,dwCount,pfVolumes) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFAudioStreamVolume_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0029 */
/* [local] */ 

EXTERN_GUID( MR_STREAM_VOLUME_SERVICE, 0xf8b5fa2f, 0x32ef, 0x46f5, 0xb1, 0x72, 0x13, 0x21, 0x21, 0x2f, 0xb2, 0xc4);


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0029_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0029_v0_0_s_ifspec;

#ifndef __IMFAudioPolicy_INTERFACE_DEFINED__
#define __IMFAudioPolicy_INTERFACE_DEFINED__

/* interface IMFAudioPolicy */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFAudioPolicy;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("a0638c2b-6465-4395-9ae7-a321a9fd2856")
    IMFAudioPolicy : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetGroupingParam( 
            /* [in] */ REFGUID rguidClass) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetGroupingParam( 
            /* [out] */ 
            __out  GUID *pguidClass) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDisplayName( 
            /* [in] */ LPCWSTR pszName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDisplayName( 
            /* [out] */ 
            __deref_out  LPWSTR *pszName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetIconPath( 
            /* [in] */ LPCWSTR pszPath) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetIconPath( 
            /* [out] */ 
            __deref_out  LPWSTR *pszPath) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFAudioPolicyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFAudioPolicy * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFAudioPolicy * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFAudioPolicy * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetGroupingParam )( 
            IMFAudioPolicy * This,
            /* [in] */ REFGUID rguidClass);
        
        HRESULT ( STDMETHODCALLTYPE *GetGroupingParam )( 
            IMFAudioPolicy * This,
            /* [out] */ 
            __out  GUID *pguidClass);
        
        HRESULT ( STDMETHODCALLTYPE *SetDisplayName )( 
            IMFAudioPolicy * This,
            /* [in] */ LPCWSTR pszName);
        
        HRESULT ( STDMETHODCALLTYPE *GetDisplayName )( 
            IMFAudioPolicy * This,
            /* [out] */ 
            __deref_out  LPWSTR *pszName);
        
        HRESULT ( STDMETHODCALLTYPE *SetIconPath )( 
            IMFAudioPolicy * This,
            /* [in] */ LPCWSTR pszPath);
        
        HRESULT ( STDMETHODCALLTYPE *GetIconPath )( 
            IMFAudioPolicy * This,
            /* [out] */ 
            __deref_out  LPWSTR *pszPath);
        
        END_INTERFACE
    } IMFAudioPolicyVtbl;

    interface IMFAudioPolicy
    {
        CONST_VTBL struct IMFAudioPolicyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFAudioPolicy_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFAudioPolicy_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFAudioPolicy_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFAudioPolicy_SetGroupingParam(This,rguidClass)	\
    ( (This)->lpVtbl -> SetGroupingParam(This,rguidClass) ) 

#define IMFAudioPolicy_GetGroupingParam(This,pguidClass)	\
    ( (This)->lpVtbl -> GetGroupingParam(This,pguidClass) ) 

#define IMFAudioPolicy_SetDisplayName(This,pszName)	\
    ( (This)->lpVtbl -> SetDisplayName(This,pszName) ) 

#define IMFAudioPolicy_GetDisplayName(This,pszName)	\
    ( (This)->lpVtbl -> GetDisplayName(This,pszName) ) 

#define IMFAudioPolicy_SetIconPath(This,pszPath)	\
    ( (This)->lpVtbl -> SetIconPath(This,pszPath) ) 

#define IMFAudioPolicy_GetIconPath(This,pszPath)	\
    ( (This)->lpVtbl -> GetIconPath(This,pszPath) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFAudioPolicy_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0030 */
/* [local] */ 

EXTERN_GUID( MR_AUDIO_POLICY_SERVICE, 0x911fd737, 0x6775, 0x4ab0, 0xa6, 0x14, 0x29, 0x78, 0x62, 0xfd, 0xac, 0x88);


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0030_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0030_v0_0_s_ifspec;

#ifndef __IMFSampleGrabberSinkCallback_INTERFACE_DEFINED__
#define __IMFSampleGrabberSinkCallback_INTERFACE_DEFINED__

/* interface IMFSampleGrabberSinkCallback */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFSampleGrabberSinkCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8C7B80BF-EE42-4b59-B1DF-55668E1BDCA8")
    IMFSampleGrabberSinkCallback : public IMFClockStateSink
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnSetPresentationClock( 
            /* [in] */ IMFPresentationClock *pPresentationClock) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnProcessSample( 
            /* [in] */ REFGUID guidMajorMediaType,
            /* [in] */ DWORD dwSampleFlags,
            /* [in] */ LONGLONG llSampleTime,
            /* [in] */ LONGLONG llSampleDuration,
            /* [in] */ 
            __in_bcount(dwSampleSize)  const BYTE *pSampleBuffer,
            /* [in] */ DWORD dwSampleSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnShutdown( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFSampleGrabberSinkCallbackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFSampleGrabberSinkCallback * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFSampleGrabberSinkCallback * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFSampleGrabberSinkCallback * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnClockStart )( 
            IMFSampleGrabberSinkCallback * This,
            /* [in] */ MFTIME hnsSystemTime,
            /* [in] */ LONGLONG llClockStartOffset);
        
        HRESULT ( STDMETHODCALLTYPE *OnClockStop )( 
            IMFSampleGrabberSinkCallback * This,
            /* [in] */ MFTIME hnsSystemTime);
        
        HRESULT ( STDMETHODCALLTYPE *OnClockPause )( 
            IMFSampleGrabberSinkCallback * This,
            /* [in] */ MFTIME hnsSystemTime);
        
        HRESULT ( STDMETHODCALLTYPE *OnClockRestart )( 
            IMFSampleGrabberSinkCallback * This,
            /* [in] */ MFTIME hnsSystemTime);
        
        HRESULT ( STDMETHODCALLTYPE *OnClockSetRate )( 
            IMFSampleGrabberSinkCallback * This,
            /* [in] */ MFTIME hnsSystemTime,
            /* [in] */ float flRate);
        
        HRESULT ( STDMETHODCALLTYPE *OnSetPresentationClock )( 
            IMFSampleGrabberSinkCallback * This,
            /* [in] */ IMFPresentationClock *pPresentationClock);
        
        HRESULT ( STDMETHODCALLTYPE *OnProcessSample )( 
            IMFSampleGrabberSinkCallback * This,
            /* [in] */ REFGUID guidMajorMediaType,
            /* [in] */ DWORD dwSampleFlags,
            /* [in] */ LONGLONG llSampleTime,
            /* [in] */ LONGLONG llSampleDuration,
            /* [in] */ 
            __in_bcount(dwSampleSize)  const BYTE *pSampleBuffer,
            /* [in] */ DWORD dwSampleSize);
        
        HRESULT ( STDMETHODCALLTYPE *OnShutdown )( 
            IMFSampleGrabberSinkCallback * This);
        
        END_INTERFACE
    } IMFSampleGrabberSinkCallbackVtbl;

    interface IMFSampleGrabberSinkCallback
    {
        CONST_VTBL struct IMFSampleGrabberSinkCallbackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFSampleGrabberSinkCallback_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFSampleGrabberSinkCallback_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFSampleGrabberSinkCallback_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFSampleGrabberSinkCallback_OnClockStart(This,hnsSystemTime,llClockStartOffset)	\
    ( (This)->lpVtbl -> OnClockStart(This,hnsSystemTime,llClockStartOffset) ) 

#define IMFSampleGrabberSinkCallback_OnClockStop(This,hnsSystemTime)	\
    ( (This)->lpVtbl -> OnClockStop(This,hnsSystemTime) ) 

#define IMFSampleGrabberSinkCallback_OnClockPause(This,hnsSystemTime)	\
    ( (This)->lpVtbl -> OnClockPause(This,hnsSystemTime) ) 

#define IMFSampleGrabberSinkCallback_OnClockRestart(This,hnsSystemTime)	\
    ( (This)->lpVtbl -> OnClockRestart(This,hnsSystemTime) ) 

#define IMFSampleGrabberSinkCallback_OnClockSetRate(This,hnsSystemTime,flRate)	\
    ( (This)->lpVtbl -> OnClockSetRate(This,hnsSystemTime,flRate) ) 


#define IMFSampleGrabberSinkCallback_OnSetPresentationClock(This,pPresentationClock)	\
    ( (This)->lpVtbl -> OnSetPresentationClock(This,pPresentationClock) ) 

#define IMFSampleGrabberSinkCallback_OnProcessSample(This,guidMajorMediaType,dwSampleFlags,llSampleTime,llSampleDuration,pSampleBuffer,dwSampleSize)	\
    ( (This)->lpVtbl -> OnProcessSample(This,guidMajorMediaType,dwSampleFlags,llSampleTime,llSampleDuration,pSampleBuffer,dwSampleSize) ) 

#define IMFSampleGrabberSinkCallback_OnShutdown(This)	\
    ( (This)->lpVtbl -> OnShutdown(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFSampleGrabberSinkCallback_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0031 */
/* [local] */ 

STDAPI
MFCreateSampleGrabberSinkActivate(
    IMFMediaType *pIMFMediaType,
    IMFSampleGrabberSinkCallback* pIMFSampleGrabberSinkCallback,
    __out IMFActivate** ppIActivate
    );
EXTERN_GUID( MF_SAMPLEGRABBERSINK_SAMPLE_TIME_OFFSET, 0x62e3d776, 0x8100, 0x4e03, 0xa6, 0xe8, 0xbd, 0x38, 0x57, 0xac, 0x9c, 0x47);
EXTERN_GUID( MF_QUALITY_SERVICES, 0xb7e2be11, 0x2f96, 0x4640, 0xb5, 0x2c, 0x28, 0x23, 0x65, 0xbd, 0xf1, 0x6c);


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0031_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0031_v0_0_s_ifspec;

#ifndef __IMFWorkQueueServices_INTERFACE_DEFINED__
#define __IMFWorkQueueServices_INTERFACE_DEFINED__

/* interface IMFWorkQueueServices */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFWorkQueueServices;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("35FE1BB8-A3A9-40fe-BBEC-EB569C9CCCA3")
    IMFWorkQueueServices : public IUnknown
    {
    public:
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE BeginRegisterTopologyWorkQueuesWithMMCSS( 
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *pState) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE EndRegisterTopologyWorkQueuesWithMMCSS( 
            /* [in] */ IMFAsyncResult *pResult) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE BeginUnregisterTopologyWorkQueuesWithMMCSS( 
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *pState) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE EndUnregisterTopologyWorkQueuesWithMMCSS( 
            /* [in] */ IMFAsyncResult *pResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTopologyWorkQueueMMCSSClass( 
            /* [in] */ DWORD dwTopologyWorkQueueId,
            /* [size_is][out] */ __RPC__out_ecount_full(*pcchClass) LPWSTR pwszClass,
            /* [out][in] */ __RPC__inout DWORD *pcchClass) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTopologyWorkQueueMMCSSTaskId( 
            /* [in] */ DWORD dwTopologyWorkQueueId,
            /* [out] */ __RPC__out DWORD *pdwTaskId) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE BeginRegisterPlatformWorkQueueWithMMCSS( 
            /* [in] */ DWORD dwPlatformWorkQueue,
            /* [in] */ LPCWSTR wszClass,
            /* [in] */ DWORD dwTaskId,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *pState) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE EndRegisterPlatformWorkQueueWithMMCSS( 
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  DWORD *pdwTaskId) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE BeginUnregisterPlatformWorkQueueWithMMCSS( 
            /* [in] */ DWORD dwPlatformWorkQueue,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *pState) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE EndUnregisterPlatformWorkQueueWithMMCSS( 
            /* [in] */ IMFAsyncResult *pResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPlaftormWorkQueueMMCSSClass( 
            /* [in] */ DWORD dwPlatformWorkQueueId,
            /* [size_is][out] */ __RPC__out_ecount_full(*pcchClass) LPWSTR pwszClass,
            /* [out][in] */ __RPC__inout DWORD *pcchClass) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPlatformWorkQueueMMCSSTaskId( 
            /* [in] */ DWORD dwPlatformWorkQueueId,
            /* [out] */ __RPC__out DWORD *pdwTaskId) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFWorkQueueServicesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFWorkQueueServices * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFWorkQueueServices * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFWorkQueueServices * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *BeginRegisterTopologyWorkQueuesWithMMCSS )( 
            IMFWorkQueueServices * This,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *pState);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *EndRegisterTopologyWorkQueuesWithMMCSS )( 
            IMFWorkQueueServices * This,
            /* [in] */ IMFAsyncResult *pResult);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *BeginUnregisterTopologyWorkQueuesWithMMCSS )( 
            IMFWorkQueueServices * This,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *pState);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *EndUnregisterTopologyWorkQueuesWithMMCSS )( 
            IMFWorkQueueServices * This,
            /* [in] */ IMFAsyncResult *pResult);
        
        HRESULT ( STDMETHODCALLTYPE *GetTopologyWorkQueueMMCSSClass )( 
            IMFWorkQueueServices * This,
            /* [in] */ DWORD dwTopologyWorkQueueId,
            /* [size_is][out] */ __RPC__out_ecount_full(*pcchClass) LPWSTR pwszClass,
            /* [out][in] */ __RPC__inout DWORD *pcchClass);
        
        HRESULT ( STDMETHODCALLTYPE *GetTopologyWorkQueueMMCSSTaskId )( 
            IMFWorkQueueServices * This,
            /* [in] */ DWORD dwTopologyWorkQueueId,
            /* [out] */ __RPC__out DWORD *pdwTaskId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *BeginRegisterPlatformWorkQueueWithMMCSS )( 
            IMFWorkQueueServices * This,
            /* [in] */ DWORD dwPlatformWorkQueue,
            /* [in] */ LPCWSTR wszClass,
            /* [in] */ DWORD dwTaskId,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *pState);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *EndRegisterPlatformWorkQueueWithMMCSS )( 
            IMFWorkQueueServices * This,
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  DWORD *pdwTaskId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *BeginUnregisterPlatformWorkQueueWithMMCSS )( 
            IMFWorkQueueServices * This,
            /* [in] */ DWORD dwPlatformWorkQueue,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *pState);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *EndUnregisterPlatformWorkQueueWithMMCSS )( 
            IMFWorkQueueServices * This,
            /* [in] */ IMFAsyncResult *pResult);
        
        HRESULT ( STDMETHODCALLTYPE *GetPlaftormWorkQueueMMCSSClass )( 
            IMFWorkQueueServices * This,
            /* [in] */ DWORD dwPlatformWorkQueueId,
            /* [size_is][out] */ __RPC__out_ecount_full(*pcchClass) LPWSTR pwszClass,
            /* [out][in] */ __RPC__inout DWORD *pcchClass);
        
        HRESULT ( STDMETHODCALLTYPE *GetPlatformWorkQueueMMCSSTaskId )( 
            IMFWorkQueueServices * This,
            /* [in] */ DWORD dwPlatformWorkQueueId,
            /* [out] */ __RPC__out DWORD *pdwTaskId);
        
        END_INTERFACE
    } IMFWorkQueueServicesVtbl;

    interface IMFWorkQueueServices
    {
        CONST_VTBL struct IMFWorkQueueServicesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFWorkQueueServices_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFWorkQueueServices_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFWorkQueueServices_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFWorkQueueServices_BeginRegisterTopologyWorkQueuesWithMMCSS(This,pCallback,pState)	\
    ( (This)->lpVtbl -> BeginRegisterTopologyWorkQueuesWithMMCSS(This,pCallback,pState) ) 

#define IMFWorkQueueServices_EndRegisterTopologyWorkQueuesWithMMCSS(This,pResult)	\
    ( (This)->lpVtbl -> EndRegisterTopologyWorkQueuesWithMMCSS(This,pResult) ) 

#define IMFWorkQueueServices_BeginUnregisterTopologyWorkQueuesWithMMCSS(This,pCallback,pState)	\
    ( (This)->lpVtbl -> BeginUnregisterTopologyWorkQueuesWithMMCSS(This,pCallback,pState) ) 

#define IMFWorkQueueServices_EndUnregisterTopologyWorkQueuesWithMMCSS(This,pResult)	\
    ( (This)->lpVtbl -> EndUnregisterTopologyWorkQueuesWithMMCSS(This,pResult) ) 

#define IMFWorkQueueServices_GetTopologyWorkQueueMMCSSClass(This,dwTopologyWorkQueueId,pwszClass,pcchClass)	\
    ( (This)->lpVtbl -> GetTopologyWorkQueueMMCSSClass(This,dwTopologyWorkQueueId,pwszClass,pcchClass) ) 

#define IMFWorkQueueServices_GetTopologyWorkQueueMMCSSTaskId(This,dwTopologyWorkQueueId,pdwTaskId)	\
    ( (This)->lpVtbl -> GetTopologyWorkQueueMMCSSTaskId(This,dwTopologyWorkQueueId,pdwTaskId) ) 

#define IMFWorkQueueServices_BeginRegisterPlatformWorkQueueWithMMCSS(This,dwPlatformWorkQueue,wszClass,dwTaskId,pCallback,pState)	\
    ( (This)->lpVtbl -> BeginRegisterPlatformWorkQueueWithMMCSS(This,dwPlatformWorkQueue,wszClass,dwTaskId,pCallback,pState) ) 

#define IMFWorkQueueServices_EndRegisterPlatformWorkQueueWithMMCSS(This,pResult,pdwTaskId)	\
    ( (This)->lpVtbl -> EndRegisterPlatformWorkQueueWithMMCSS(This,pResult,pdwTaskId) ) 

#define IMFWorkQueueServices_BeginUnregisterPlatformWorkQueueWithMMCSS(This,dwPlatformWorkQueue,pCallback,pState)	\
    ( (This)->lpVtbl -> BeginUnregisterPlatformWorkQueueWithMMCSS(This,dwPlatformWorkQueue,pCallback,pState) ) 

#define IMFWorkQueueServices_EndUnregisterPlatformWorkQueueWithMMCSS(This,pResult)	\
    ( (This)->lpVtbl -> EndUnregisterPlatformWorkQueueWithMMCSS(This,pResult) ) 

#define IMFWorkQueueServices_GetPlaftormWorkQueueMMCSSClass(This,dwPlatformWorkQueueId,pwszClass,pcchClass)	\
    ( (This)->lpVtbl -> GetPlaftormWorkQueueMMCSSClass(This,dwPlatformWorkQueueId,pwszClass,pcchClass) ) 

#define IMFWorkQueueServices_GetPlatformWorkQueueMMCSSTaskId(This,dwPlatformWorkQueueId,pdwTaskId)	\
    ( (This)->lpVtbl -> GetPlatformWorkQueueMMCSSTaskId(This,dwPlatformWorkQueueId,pdwTaskId) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_RemoteBeginRegisterTopologyWorkQueuesWithMMCSS_Proxy( 
    IMFWorkQueueServices * This,
    /* [in] */ __RPC__in_opt IMFRemoteAsyncCallback *pCallback);


void __RPC_STUB IMFWorkQueueServices_RemoteBeginRegisterTopologyWorkQueuesWithMMCSS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_RemoteEndRegisterTopologyWorkQueuesWithMMCSS_Proxy( 
    IMFWorkQueueServices * This,
    /* [in] */ __RPC__in_opt IUnknown *pResult);


void __RPC_STUB IMFWorkQueueServices_RemoteEndRegisterTopologyWorkQueuesWithMMCSS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_RemoteBeginUnregisterTopologyWorkQueuesWithMMCSS_Proxy( 
    IMFWorkQueueServices * This,
    /* [in] */ __RPC__in_opt IMFRemoteAsyncCallback *pCallback);


void __RPC_STUB IMFWorkQueueServices_RemoteBeginUnregisterTopologyWorkQueuesWithMMCSS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_RemoteEndUnregisterTopologyWorkQueuesWithMMCSS_Proxy( 
    IMFWorkQueueServices * This,
    /* [in] */ __RPC__in_opt IUnknown *pResult);


void __RPC_STUB IMFWorkQueueServices_RemoteEndUnregisterTopologyWorkQueuesWithMMCSS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_RemoteBeginRegisterPlatformWorkQueueWithMMCSS_Proxy( 
    IMFWorkQueueServices * This,
    /* [in] */ DWORD dwPlatformWorkQueue,
    /* [in] */ __RPC__in LPCWSTR wszClass,
    /* [in] */ DWORD dwTaskId,
    /* [in] */ __RPC__in_opt IMFRemoteAsyncCallback *pCallback);


void __RPC_STUB IMFWorkQueueServices_RemoteBeginRegisterPlatformWorkQueueWithMMCSS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_RemoteEndRegisterPlatformWorkQueueWithMMCSS_Proxy( 
    IMFWorkQueueServices * This,
    /* [in] */ __RPC__in_opt IUnknown *pResult,
    /* [out] */ __RPC__out DWORD *pdwTaskId);


void __RPC_STUB IMFWorkQueueServices_RemoteEndRegisterPlatformWorkQueueWithMMCSS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_RemoteBeginUnregisterPlatformWorkQueueWithMMCSS_Proxy( 
    IMFWorkQueueServices * This,
    /* [in] */ DWORD dwPlatformWorkQueue,
    /* [in] */ __RPC__in_opt IMFRemoteAsyncCallback *pCallback);


void __RPC_STUB IMFWorkQueueServices_RemoteBeginUnregisterPlatformWorkQueueWithMMCSS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_RemoteEndUnregisterPlatformWorkQueueWithMMCSS_Proxy( 
    IMFWorkQueueServices * This,
    /* [in] */ __RPC__in_opt IUnknown *pResult);


void __RPC_STUB IMFWorkQueueServices_RemoteEndUnregisterPlatformWorkQueueWithMMCSS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMFWorkQueueServices_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0032 */
/* [local] */ 

EXTERN_GUID( MF_WORKQUEUE_SERVICES, 0x8e37d489, 0x41e0, 0x413a, 0x90, 0x68, 0x28, 0x7c, 0x88, 0x6d, 0x8d, 0xda);
typedef 
enum _MF_QUALITY_DROP_MODE
    {	MF_DROP_MODE_NONE	= 0,
	MF_DROP_MODE_1	= 0x1,
	MF_DROP_MODE_2	= 0x2,
	MF_DROP_MODE_3	= 0x3,
	MF_DROP_MODE_4	= 0x4,
	MF_DROP_MODE_5	= 0x5,
	MF_NUM_DROP_MODES	= 0x6
    } 	MF_QUALITY_DROP_MODE;

typedef 
enum _MF_QUALITY_LEVEL
    {	MF_QUALITY_NORMAL	= 0,
	MF_QUALITY_NORMAL_MINUS_1	= 0x1,
	MF_QUALITY_NORMAL_MINUS_2	= 0x2,
	MF_QUALITY_NORMAL_MINUS_3	= 0x3,
	MF_QUALITY_NORMAL_MINUS_4	= 0x4,
	MF_QUALITY_NORMAL_MINUS_5	= 0x5,
	MF_NUM_QUALITY_LEVELS	= 0x6
    } 	MF_QUALITY_LEVEL;



extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0032_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0032_v0_0_s_ifspec;

#ifndef __IMFQualityManager_INTERFACE_DEFINED__
#define __IMFQualityManager_INTERFACE_DEFINED__

/* interface IMFQualityManager */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFQualityManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8D009D86-5B9F-4115-B1FC-9F80D52AB8AB")
    IMFQualityManager : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE NotifyTopology( 
            /* [in] */ IMFTopology *pTopology) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE NotifyPresentationClock( 
            /* [in] */ IMFPresentationClock *pClock) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE NotifyProcessInput( 
            /* [in] */ IMFTopologyNode *pNode,
            /* [in] */ long lInputIndex,
            /* [in] */ IMFSample *pSample) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE NotifyProcessOutput( 
            /* [in] */ IMFTopologyNode *pNode,
            /* [in] */ long lOutputIndex,
            /* [in] */ IMFSample *pSample) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE NotifyQualityEvent( 
            /* [in] */ IUnknown *pObject,
            /* [in] */ IMFMediaEvent *pEvent) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Shutdown( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFQualityManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFQualityManager * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFQualityManager * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFQualityManager * This);
        
        HRESULT ( STDMETHODCALLTYPE *NotifyTopology )( 
            IMFQualityManager * This,
            /* [in] */ IMFTopology *pTopology);
        
        HRESULT ( STDMETHODCALLTYPE *NotifyPresentationClock )( 
            IMFQualityManager * This,
            /* [in] */ IMFPresentationClock *pClock);
        
        HRESULT ( STDMETHODCALLTYPE *NotifyProcessInput )( 
            IMFQualityManager * This,
            /* [in] */ IMFTopologyNode *pNode,
            /* [in] */ long lInputIndex,
            /* [in] */ IMFSample *pSample);
        
        HRESULT ( STDMETHODCALLTYPE *NotifyProcessOutput )( 
            IMFQualityManager * This,
            /* [in] */ IMFTopologyNode *pNode,
            /* [in] */ long lOutputIndex,
            /* [in] */ IMFSample *pSample);
        
        HRESULT ( STDMETHODCALLTYPE *NotifyQualityEvent )( 
            IMFQualityManager * This,
            /* [in] */ IUnknown *pObject,
            /* [in] */ IMFMediaEvent *pEvent);
        
        HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            IMFQualityManager * This);
        
        END_INTERFACE
    } IMFQualityManagerVtbl;

    interface IMFQualityManager
    {
        CONST_VTBL struct IMFQualityManagerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFQualityManager_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFQualityManager_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFQualityManager_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFQualityManager_NotifyTopology(This,pTopology)	\
    ( (This)->lpVtbl -> NotifyTopology(This,pTopology) ) 

#define IMFQualityManager_NotifyPresentationClock(This,pClock)	\
    ( (This)->lpVtbl -> NotifyPresentationClock(This,pClock) ) 

#define IMFQualityManager_NotifyProcessInput(This,pNode,lInputIndex,pSample)	\
    ( (This)->lpVtbl -> NotifyProcessInput(This,pNode,lInputIndex,pSample) ) 

#define IMFQualityManager_NotifyProcessOutput(This,pNode,lOutputIndex,pSample)	\
    ( (This)->lpVtbl -> NotifyProcessOutput(This,pNode,lOutputIndex,pSample) ) 

#define IMFQualityManager_NotifyQualityEvent(This,pObject,pEvent)	\
    ( (This)->lpVtbl -> NotifyQualityEvent(This,pObject,pEvent) ) 

#define IMFQualityManager_Shutdown(This)	\
    ( (This)->lpVtbl -> Shutdown(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFQualityManager_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0033 */
/* [local] */ 

STDAPI MFCreateStandardQualityManager(
    __out IMFQualityManager **ppQualityManager );
EXTERN_GUID( MF_QUALITY_NOTIFY_PROCESSING_LATENCY, 0xf6b44af8, 0x604d, 0x46fe, 0xa9, 0x5d, 0x45, 0x47, 0x9b, 0x10, 0xc9, 0xbc );
EXTERN_GUID( MF_QUALITY_NOTIFY_SAMPLE_LAG, 0x30d15206, 0xed2a, 0x4760, 0xbe, 0x17, 0xeb, 0x4a, 0x9f, 0x12, 0x29, 0x5c );


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0033_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0033_v0_0_s_ifspec;

#ifndef __IMFQualityAdvise_INTERFACE_DEFINED__
#define __IMFQualityAdvise_INTERFACE_DEFINED__

/* interface IMFQualityAdvise */
/* [uuid][object] */ 






EXTERN_C const IID IID_IMFQualityAdvise;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EC15E2E9-E36B-4f7c-8758-77D452EF4CE7")
    IMFQualityAdvise : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetDropMode( 
            /* [in] */ MF_QUALITY_DROP_MODE eDropMode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetQualityLevel( 
            /* [in] */ MF_QUALITY_LEVEL eQualityLevel) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDropMode( 
            /* [out] */ 
            __out  MF_QUALITY_DROP_MODE *peDropMode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetQualityLevel( 
            /* [out] */ 
            __out  MF_QUALITY_LEVEL *peQualityLevel) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DropTime( 
            /* [in] */ LONGLONG hnsAmountToDrop) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFQualityAdviseVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFQualityAdvise * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFQualityAdvise * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFQualityAdvise * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetDropMode )( 
            IMFQualityAdvise * This,
            /* [in] */ MF_QUALITY_DROP_MODE eDropMode);
        
        HRESULT ( STDMETHODCALLTYPE *SetQualityLevel )( 
            IMFQualityAdvise * This,
            /* [in] */ MF_QUALITY_LEVEL eQualityLevel);
        
        HRESULT ( STDMETHODCALLTYPE *GetDropMode )( 
            IMFQualityAdvise * This,
            /* [out] */ 
            __out  MF_QUALITY_DROP_MODE *peDropMode);
        
        HRESULT ( STDMETHODCALLTYPE *GetQualityLevel )( 
            IMFQualityAdvise * This,
            /* [out] */ 
            __out  MF_QUALITY_LEVEL *peQualityLevel);
        
        HRESULT ( STDMETHODCALLTYPE *DropTime )( 
            IMFQualityAdvise * This,
            /* [in] */ LONGLONG hnsAmountToDrop);
        
        END_INTERFACE
    } IMFQualityAdviseVtbl;

    interface IMFQualityAdvise
    {
        CONST_VTBL struct IMFQualityAdviseVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFQualityAdvise_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFQualityAdvise_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFQualityAdvise_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFQualityAdvise_SetDropMode(This,eDropMode)	\
    ( (This)->lpVtbl -> SetDropMode(This,eDropMode) ) 

#define IMFQualityAdvise_SetQualityLevel(This,eQualityLevel)	\
    ( (This)->lpVtbl -> SetQualityLevel(This,eQualityLevel) ) 

#define IMFQualityAdvise_GetDropMode(This,peDropMode)	\
    ( (This)->lpVtbl -> GetDropMode(This,peDropMode) ) 

#define IMFQualityAdvise_GetQualityLevel(This,peQualityLevel)	\
    ( (This)->lpVtbl -> GetQualityLevel(This,peQualityLevel) ) 

#define IMFQualityAdvise_DropTime(This,hnsAmountToDrop)	\
    ( (This)->lpVtbl -> DropTime(This,hnsAmountToDrop) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFQualityAdvise_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0034 */
/* [local] */ 

STDAPI MFCreateQualityManager(
    __out IMFQualityManager** ppQualityManager
    );


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0034_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0034_v0_0_s_ifspec;

#ifndef __IMFRealTimeClient_INTERFACE_DEFINED__
#define __IMFRealTimeClient_INTERFACE_DEFINED__

/* interface IMFRealTimeClient */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFRealTimeClient;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2347D60B-3FB5-480c-8803-8DF3ADCD3EF0")
    IMFRealTimeClient : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE RegisterThreads( 
            /* [in] */ DWORD dwTaskIndex,
            /* [in] */ LPCWSTR wszClass) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnregisterThreads( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetWorkQueue( 
            /* [in] */ DWORD dwWorkQueueId) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFRealTimeClientVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFRealTimeClient * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFRealTimeClient * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFRealTimeClient * This);
        
        HRESULT ( STDMETHODCALLTYPE *RegisterThreads )( 
            IMFRealTimeClient * This,
            /* [in] */ DWORD dwTaskIndex,
            /* [in] */ LPCWSTR wszClass);
        
        HRESULT ( STDMETHODCALLTYPE *UnregisterThreads )( 
            IMFRealTimeClient * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetWorkQueue )( 
            IMFRealTimeClient * This,
            /* [in] */ DWORD dwWorkQueueId);
        
        END_INTERFACE
    } IMFRealTimeClientVtbl;

    interface IMFRealTimeClient
    {
        CONST_VTBL struct IMFRealTimeClientVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFRealTimeClient_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFRealTimeClient_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFRealTimeClient_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFRealTimeClient_RegisterThreads(This,dwTaskIndex,wszClass)	\
    ( (This)->lpVtbl -> RegisterThreads(This,dwTaskIndex,wszClass) ) 

#define IMFRealTimeClient_UnregisterThreads(This)	\
    ( (This)->lpVtbl -> UnregisterThreads(This) ) 

#define IMFRealTimeClient_SetWorkQueue(This,dwWorkQueueId)	\
    ( (This)->lpVtbl -> SetWorkQueue(This,dwWorkQueueId) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFRealTimeClient_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0035 */
/* [local] */ 

EXTERN_C const GUID MF_QUALITY_MANAGER_SERVICE;
typedef DWORD MFSequencerElementId;

#define	MFSEQUENCER_INVALID_ELEMENT_ID	( 0xffffffff )

typedef 
enum _MFSequencerTopologyFlags
    {	SequencerTopologyFlags_Last	= 0x1
    } 	MFSequencerTopologyFlags;



extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0035_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0035_v0_0_s_ifspec;

#ifndef __IMFSequencerSource_INTERFACE_DEFINED__
#define __IMFSequencerSource_INTERFACE_DEFINED__

/* interface IMFSequencerSource */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFSequencerSource;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("197CD219-19CB-4de1-A64C-ACF2EDCBE59E")
    IMFSequencerSource : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AppendTopology( 
            /* [in] */ IMFTopology *pTopology,
            /* [in] */ DWORD dwFlags,
            /* [out] */ 
            __out  MFSequencerElementId *pdwId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteTopology( 
            /* [in] */ MFSequencerElementId dwId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPresentationContext( 
            /* [in] */ IMFPresentationDescriptor *pPD,
            /* [optional][out] */ 
            __out_opt  MFSequencerElementId *pId,
            /* [optional][out] */ 
            __out_opt  IMFTopology **ppTopology) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UpdateTopology( 
            /* [in] */ MFSequencerElementId dwId,
            /* [in] */ IMFTopology *pTopology) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UpdateTopologyFlags( 
            /* [in] */ MFSequencerElementId dwId,
            /* [in] */ DWORD dwFlags) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFSequencerSourceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFSequencerSource * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFSequencerSource * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFSequencerSource * This);
        
        HRESULT ( STDMETHODCALLTYPE *AppendTopology )( 
            IMFSequencerSource * This,
            /* [in] */ IMFTopology *pTopology,
            /* [in] */ DWORD dwFlags,
            /* [out] */ 
            __out  MFSequencerElementId *pdwId);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteTopology )( 
            IMFSequencerSource * This,
            /* [in] */ MFSequencerElementId dwId);
        
        HRESULT ( STDMETHODCALLTYPE *GetPresentationContext )( 
            IMFSequencerSource * This,
            /* [in] */ IMFPresentationDescriptor *pPD,
            /* [optional][out] */ 
            __out_opt  MFSequencerElementId *pId,
            /* [optional][out] */ 
            __out_opt  IMFTopology **ppTopology);
        
        HRESULT ( STDMETHODCALLTYPE *UpdateTopology )( 
            IMFSequencerSource * This,
            /* [in] */ MFSequencerElementId dwId,
            /* [in] */ IMFTopology *pTopology);
        
        HRESULT ( STDMETHODCALLTYPE *UpdateTopologyFlags )( 
            IMFSequencerSource * This,
            /* [in] */ MFSequencerElementId dwId,
            /* [in] */ DWORD dwFlags);
        
        END_INTERFACE
    } IMFSequencerSourceVtbl;

    interface IMFSequencerSource
    {
        CONST_VTBL struct IMFSequencerSourceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFSequencerSource_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFSequencerSource_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFSequencerSource_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFSequencerSource_AppendTopology(This,pTopology,dwFlags,pdwId)	\
    ( (This)->lpVtbl -> AppendTopology(This,pTopology,dwFlags,pdwId) ) 

#define IMFSequencerSource_DeleteTopology(This,dwId)	\
    ( (This)->lpVtbl -> DeleteTopology(This,dwId) ) 

#define IMFSequencerSource_GetPresentationContext(This,pPD,pId,ppTopology)	\
    ( (This)->lpVtbl -> GetPresentationContext(This,pPD,pId,ppTopology) ) 

#define IMFSequencerSource_UpdateTopology(This,dwId,pTopology)	\
    ( (This)->lpVtbl -> UpdateTopology(This,dwId,pTopology) ) 

#define IMFSequencerSource_UpdateTopologyFlags(This,dwId,dwFlags)	\
    ( (This)->lpVtbl -> UpdateTopologyFlags(This,dwId,dwFlags) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFSequencerSource_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0036 */
/* [local] */ 

EXTERN_GUID( MF_TIME_FORMAT_SEGMENT_OFFSET, 0xc8b8be77, 0x869c, 0x431d, 0x81, 0x2e, 0x16, 0x96, 0x93, 0xf6, 0x5a, 0x39 );
STDAPI MFCreateSequencerSource(
    IUnknown *pReserved,
    __out IMFSequencerSource **ppSequencerSource
    );
STDAPI MFCreateSequencerSegmentOffset(
    MFSequencerElementId dwId,
    MFTIME hnsOffset,
    __out PROPVARIANT *pvarSegmentOffset
    );


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0036_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0036_v0_0_s_ifspec;

#ifndef __IMFMediaSourceTopologyProvider_INTERFACE_DEFINED__
#define __IMFMediaSourceTopologyProvider_INTERFACE_DEFINED__

/* interface IMFMediaSourceTopologyProvider */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFMediaSourceTopologyProvider;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0E1D6009-C9F3-442d-8C51-A42D2D49452F")
    IMFMediaSourceTopologyProvider : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetMediaSourceTopology( 
            /* [in] */ __RPC__in_opt IMFPresentationDescriptor *pPresentationDescriptor,
            /* [out] */ __RPC__deref_out_opt IMFTopology **ppTopology) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFMediaSourceTopologyProviderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFMediaSourceTopologyProvider * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFMediaSourceTopologyProvider * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFMediaSourceTopologyProvider * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetMediaSourceTopology )( 
            IMFMediaSourceTopologyProvider * This,
            /* [in] */ __RPC__in_opt IMFPresentationDescriptor *pPresentationDescriptor,
            /* [out] */ __RPC__deref_out_opt IMFTopology **ppTopology);
        
        END_INTERFACE
    } IMFMediaSourceTopologyProviderVtbl;

    interface IMFMediaSourceTopologyProvider
    {
        CONST_VTBL struct IMFMediaSourceTopologyProviderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFMediaSourceTopologyProvider_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFMediaSourceTopologyProvider_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFMediaSourceTopologyProvider_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFMediaSourceTopologyProvider_GetMediaSourceTopology(This,pPresentationDescriptor,ppTopology)	\
    ( (This)->lpVtbl -> GetMediaSourceTopology(This,pPresentationDescriptor,ppTopology) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFMediaSourceTopologyProvider_INTERFACE_DEFINED__ */


#ifndef __IMFMediaSourcePresentationProvider_INTERFACE_DEFINED__
#define __IMFMediaSourcePresentationProvider_INTERFACE_DEFINED__

/* interface IMFMediaSourcePresentationProvider */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFMediaSourcePresentationProvider;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0E1D600a-C9F3-442d-8C51-A42D2D49452F")
    IMFMediaSourcePresentationProvider : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ForceEndOfPresentation( 
            /* [in] */ __RPC__in_opt IMFPresentationDescriptor *pPresentationDescriptor) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFMediaSourcePresentationProviderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFMediaSourcePresentationProvider * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFMediaSourcePresentationProvider * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFMediaSourcePresentationProvider * This);
        
        HRESULT ( STDMETHODCALLTYPE *ForceEndOfPresentation )( 
            IMFMediaSourcePresentationProvider * This,
            /* [in] */ __RPC__in_opt IMFPresentationDescriptor *pPresentationDescriptor);
        
        END_INTERFACE
    } IMFMediaSourcePresentationProviderVtbl;

    interface IMFMediaSourcePresentationProvider
    {
        CONST_VTBL struct IMFMediaSourcePresentationProviderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFMediaSourcePresentationProvider_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFMediaSourcePresentationProvider_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFMediaSourcePresentationProvider_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFMediaSourcePresentationProvider_ForceEndOfPresentation(This,pPresentationDescriptor)	\
    ( (This)->lpVtbl -> ForceEndOfPresentation(This,pPresentationDescriptor) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFMediaSourcePresentationProvider_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0038 */
/* [local] */ 

EXTERN_GUID( MF_SOURCE_PRESENTATION_PROVIDER_SERVICE, 0xe002aadc, 0xf4af, 0x4ee5, 0x98, 0x47, 0x05, 0x3e, 0xdf, 0x84, 0x04, 0x26 );
typedef struct _MFTOPONODE_ATTRIBUTE_UPDATE
    {
    TOPOID NodeId;
    GUID guidAttributeKey;
    MF_ATTRIBUTE_TYPE attrType;
    union 
        {
        UINT32 u32;
        UINT32 u64;
        double d;
         /* Empty union arm */ 
        } 	;
    } 	MFTOPONODE_ATTRIBUTE_UPDATE;



extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0038_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0038_v0_0_s_ifspec;

#ifndef __IMFTopologyNodeAttributeEditor_INTERFACE_DEFINED__
#define __IMFTopologyNodeAttributeEditor_INTERFACE_DEFINED__

/* interface IMFTopologyNodeAttributeEditor */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFTopologyNodeAttributeEditor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("676aa6dd-238a-410d-bb99-65668d01605a")
    IMFTopologyNodeAttributeEditor : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE UpdateNodeAttributes( 
            /* [in] */ TOPOID TopoId,
            /* [in] */ DWORD cUpdates,
            /* [size_is][in] */ __RPC__in_ecount_full(cUpdates) MFTOPONODE_ATTRIBUTE_UPDATE *pUpdates) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFTopologyNodeAttributeEditorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFTopologyNodeAttributeEditor * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFTopologyNodeAttributeEditor * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFTopologyNodeAttributeEditor * This);
        
        HRESULT ( STDMETHODCALLTYPE *UpdateNodeAttributes )( 
            IMFTopologyNodeAttributeEditor * This,
            /* [in] */ TOPOID TopoId,
            /* [in] */ DWORD cUpdates,
            /* [size_is][in] */ __RPC__in_ecount_full(cUpdates) MFTOPONODE_ATTRIBUTE_UPDATE *pUpdates);
        
        END_INTERFACE
    } IMFTopologyNodeAttributeEditorVtbl;

    interface IMFTopologyNodeAttributeEditor
    {
        CONST_VTBL struct IMFTopologyNodeAttributeEditorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFTopologyNodeAttributeEditor_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFTopologyNodeAttributeEditor_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFTopologyNodeAttributeEditor_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFTopologyNodeAttributeEditor_UpdateNodeAttributes(This,TopoId,cUpdates,pUpdates)	\
    ( (This)->lpVtbl -> UpdateNodeAttributes(This,TopoId,cUpdates,pUpdates) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFTopologyNodeAttributeEditor_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0039 */
/* [local] */ 

EXTERN_GUID( MF_TOPONODE_ATTRIBUTE_EDITOR_SERVICE, 0x65656e1a, 0x077f, 0x4472, 0x83, 0xef, 0x31, 0x6f, 0x11, 0xd5, 0x08, 0x7a );
typedef /* [public] */ struct _MF_LEAKY_BUCKET_PAIR
    {
    DWORD dwBitrate;
    DWORD msBufferWindow;
    } 	MF_LEAKY_BUCKET_PAIR;

typedef /* [public] */ struct _MFBYTESTREAM_BUFFERING_PARAMS
    {
    QWORD cbTotalFileSize;
    QWORD cbPlayableDataSize;
    MF_LEAKY_BUCKET_PAIR *prgBuckets;
    DWORD cBuckets;
    QWORD qwNetBufferingTime;
    QWORD qwExtraBufferingTimeDuringSeek;
    QWORD qwPlayDuration;
    float dRate;
    } 	MFBYTESTREAM_BUFFERING_PARAMS;



extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0039_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0039_v0_0_s_ifspec;

#ifndef __IMFByteStreamBuffering_INTERFACE_DEFINED__
#define __IMFByteStreamBuffering_INTERFACE_DEFINED__

/* interface IMFByteStreamBuffering */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFByteStreamBuffering;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6d66d782-1d4f-4db7-8c63-cb8c77f1ef5e")
    IMFByteStreamBuffering : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetBufferingParams( 
            /* [in] */ 
            __in  MFBYTESTREAM_BUFFERING_PARAMS *pParams) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnableBuffering( 
            /* [in] */ BOOL fEnable) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE StopBuffering( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFByteStreamBufferingVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFByteStreamBuffering * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFByteStreamBuffering * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFByteStreamBuffering * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetBufferingParams )( 
            IMFByteStreamBuffering * This,
            /* [in] */ 
            __in  MFBYTESTREAM_BUFFERING_PARAMS *pParams);
        
        HRESULT ( STDMETHODCALLTYPE *EnableBuffering )( 
            IMFByteStreamBuffering * This,
            /* [in] */ BOOL fEnable);
        
        HRESULT ( STDMETHODCALLTYPE *StopBuffering )( 
            IMFByteStreamBuffering * This);
        
        END_INTERFACE
    } IMFByteStreamBufferingVtbl;

    interface IMFByteStreamBuffering
    {
        CONST_VTBL struct IMFByteStreamBufferingVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFByteStreamBuffering_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFByteStreamBuffering_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFByteStreamBuffering_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFByteStreamBuffering_SetBufferingParams(This,pParams)	\
    ( (This)->lpVtbl -> SetBufferingParams(This,pParams) ) 

#define IMFByteStreamBuffering_EnableBuffering(This,fEnable)	\
    ( (This)->lpVtbl -> EnableBuffering(This,fEnable) ) 

#define IMFByteStreamBuffering_StopBuffering(This)	\
    ( (This)->lpVtbl -> StopBuffering(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFByteStreamBuffering_INTERFACE_DEFINED__ */


#ifndef __IMFNetCredential_INTERFACE_DEFINED__
#define __IMFNetCredential_INTERFACE_DEFINED__

/* interface IMFNetCredential */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFNetCredential;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5b87ef6a-7ed8-434f-ba0e-184fac1628d1")
    IMFNetCredential : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetUser( 
            /* [size_is][in] */ 
            __in_bcount(cbData)  BYTE *pbData,
            /* [in] */ DWORD cbData,
            /* [in] */ BOOL fDataIsEncrypted) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetPassword( 
            /* [size_is][in] */ 
            __in_bcount(cbData)  BYTE *pbData,
            /* [in] */ DWORD cbData,
            /* [in] */ BOOL fDataIsEncrypted) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetUser( 
            /* [size_is][out] */ 
            __out_ecount_part_opt(*pcbData,*pcbData)  BYTE *pbData,
            /* [out][in] */ 
            __inout  DWORD *pcbData,
            /* [in] */ BOOL fEncryptData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPassword( 
            /* [size_is][out] */ 
            __out_ecount_part_opt(*pcbData,*pcbData)  BYTE *pbData,
            /* [out][in] */ 
            __inout  DWORD *pcbData,
            /* [in] */ BOOL fEncryptData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LoggedOnUser( 
            BOOL *pfLoggedOnUser) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFNetCredentialVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFNetCredential * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFNetCredential * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFNetCredential * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetUser )( 
            IMFNetCredential * This,
            /* [size_is][in] */ 
            __in_bcount(cbData)  BYTE *pbData,
            /* [in] */ DWORD cbData,
            /* [in] */ BOOL fDataIsEncrypted);
        
        HRESULT ( STDMETHODCALLTYPE *SetPassword )( 
            IMFNetCredential * This,
            /* [size_is][in] */ 
            __in_bcount(cbData)  BYTE *pbData,
            /* [in] */ DWORD cbData,
            /* [in] */ BOOL fDataIsEncrypted);
        
        HRESULT ( STDMETHODCALLTYPE *GetUser )( 
            IMFNetCredential * This,
            /* [size_is][out] */ 
            __out_ecount_part_opt(*pcbData,*pcbData)  BYTE *pbData,
            /* [out][in] */ 
            __inout  DWORD *pcbData,
            /* [in] */ BOOL fEncryptData);
        
        HRESULT ( STDMETHODCALLTYPE *GetPassword )( 
            IMFNetCredential * This,
            /* [size_is][out] */ 
            __out_ecount_part_opt(*pcbData,*pcbData)  BYTE *pbData,
            /* [out][in] */ 
            __inout  DWORD *pcbData,
            /* [in] */ BOOL fEncryptData);
        
        HRESULT ( STDMETHODCALLTYPE *LoggedOnUser )( 
            IMFNetCredential * This,
            BOOL *pfLoggedOnUser);
        
        END_INTERFACE
    } IMFNetCredentialVtbl;

    interface IMFNetCredential
    {
        CONST_VTBL struct IMFNetCredentialVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFNetCredential_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFNetCredential_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFNetCredential_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFNetCredential_SetUser(This,pbData,cbData,fDataIsEncrypted)	\
    ( (This)->lpVtbl -> SetUser(This,pbData,cbData,fDataIsEncrypted) ) 

#define IMFNetCredential_SetPassword(This,pbData,cbData,fDataIsEncrypted)	\
    ( (This)->lpVtbl -> SetPassword(This,pbData,cbData,fDataIsEncrypted) ) 

#define IMFNetCredential_GetUser(This,pbData,pcbData,fEncryptData)	\
    ( (This)->lpVtbl -> GetUser(This,pbData,pcbData,fEncryptData) ) 

#define IMFNetCredential_GetPassword(This,pbData,pcbData,fEncryptData)	\
    ( (This)->lpVtbl -> GetPassword(This,pbData,pcbData,fEncryptData) ) 

#define IMFNetCredential_LoggedOnUser(This,pfLoggedOnUser)	\
    ( (This)->lpVtbl -> LoggedOnUser(This,pfLoggedOnUser) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFNetCredential_INTERFACE_DEFINED__ */


#ifndef __IMFNetCredentialManager_INTERFACE_DEFINED__
#define __IMFNetCredentialManager_INTERFACE_DEFINED__

/* interface IMFNetCredentialManager */
/* [local][uuid][object] */ 

typedef struct _MFNetCredentialManagerGetParam
    {
    HRESULT hrOp;
    BOOL fAllowLoggedOnUser;
    BOOL fClearTextPackage;
    LPCWSTR pszUrl;
    LPCWSTR pszSite;
    LPCWSTR pszRealm;
    LPCWSTR pszPackage;
    LONG nRetries;
    } 	MFNetCredentialManagerGetParam;


EXTERN_C const IID IID_IMFNetCredentialManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5b87ef6b-7ed8-434f-ba0e-184fac1628d1")
    IMFNetCredentialManager : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE BeginGetCredentials( 
            /* [in] */ MFNetCredentialManagerGetParam *pParam,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *pState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndGetCredentials( 
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  IMFNetCredential **ppCred) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetGood( 
            /* [in] */ IMFNetCredential *pCred,
            /* [in] */ BOOL fGood) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFNetCredentialManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFNetCredentialManager * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFNetCredentialManager * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFNetCredentialManager * This);
        
        HRESULT ( STDMETHODCALLTYPE *BeginGetCredentials )( 
            IMFNetCredentialManager * This,
            /* [in] */ MFNetCredentialManagerGetParam *pParam,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *pState);
        
        HRESULT ( STDMETHODCALLTYPE *EndGetCredentials )( 
            IMFNetCredentialManager * This,
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  IMFNetCredential **ppCred);
        
        HRESULT ( STDMETHODCALLTYPE *SetGood )( 
            IMFNetCredentialManager * This,
            /* [in] */ IMFNetCredential *pCred,
            /* [in] */ BOOL fGood);
        
        END_INTERFACE
    } IMFNetCredentialManagerVtbl;

    interface IMFNetCredentialManager
    {
        CONST_VTBL struct IMFNetCredentialManagerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFNetCredentialManager_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFNetCredentialManager_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFNetCredentialManager_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFNetCredentialManager_BeginGetCredentials(This,pParam,pCallback,pState)	\
    ( (This)->lpVtbl -> BeginGetCredentials(This,pParam,pCallback,pState) ) 

#define IMFNetCredentialManager_EndGetCredentials(This,pResult,ppCred)	\
    ( (This)->lpVtbl -> EndGetCredentials(This,pResult,ppCred) ) 

#define IMFNetCredentialManager_SetGood(This,pCred,fGood)	\
    ( (This)->lpVtbl -> SetGood(This,pCred,fGood) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFNetCredentialManager_INTERFACE_DEFINED__ */


#ifndef __IMFNetCredentialCache_INTERFACE_DEFINED__
#define __IMFNetCredentialCache_INTERFACE_DEFINED__

/* interface IMFNetCredentialCache */
/* [local][uuid][object] */ 

typedef 
enum _MFNetCredentialRequirements
    {	REQUIRE_PROMPT	= 0x1
    } 	MFNetCredentialRequirements;

typedef 
enum _MFNetCredentialOptions
    {	MFNET_CREDENTIAL_SAVE	= 0x1,
	MFNET_CREDENTIAL_DONT_CACHE	= 0x2,
	MFNET_CREDENTIAL_ALLOW_CLEAR_TEXT	= 0x4
    } 	MFNetCredentialOptions;

typedef 
enum _MFNetAuthenticationFlags
    {	MFNET_AUTHENTICATION_PROXY	= 0x1,
	MFNET_AUTHENTICATION_CLEAR_TEXT	= 0x2,
	MFNET_AUTHENTICATION_LOGGED_ON_USER	= 0x4
    } 	MFNetAuthenticationFlags;


EXTERN_C const IID IID_IMFNetCredentialCache;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5b87ef6c-7ed8-434f-ba0e-184fac1628d1")
    IMFNetCredentialCache : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetCredential( 
            /* [in] */ LPCWSTR pszUrl,
            /* [in] */ LPCWSTR pszRealm,
            /* [in] */ DWORD dwAuthenticationFlags,
            /* [out] */ 
            __out  IMFNetCredential **ppCred,
            /* [out] */ 
            __out  DWORD *pdwRequirementsFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetGood( 
            /* [in] */ IMFNetCredential *pCred,
            /* [in] */ BOOL fGood) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetUserOptions( 
            /* [in] */ IMFNetCredential *pCred,
            /* [in] */ DWORD dwOptionsFlags) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFNetCredentialCacheVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFNetCredentialCache * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFNetCredentialCache * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFNetCredentialCache * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCredential )( 
            IMFNetCredentialCache * This,
            /* [in] */ LPCWSTR pszUrl,
            /* [in] */ LPCWSTR pszRealm,
            /* [in] */ DWORD dwAuthenticationFlags,
            /* [out] */ 
            __out  IMFNetCredential **ppCred,
            /* [out] */ 
            __out  DWORD *pdwRequirementsFlags);
        
        HRESULT ( STDMETHODCALLTYPE *SetGood )( 
            IMFNetCredentialCache * This,
            /* [in] */ IMFNetCredential *pCred,
            /* [in] */ BOOL fGood);
        
        HRESULT ( STDMETHODCALLTYPE *SetUserOptions )( 
            IMFNetCredentialCache * This,
            /* [in] */ IMFNetCredential *pCred,
            /* [in] */ DWORD dwOptionsFlags);
        
        END_INTERFACE
    } IMFNetCredentialCacheVtbl;

    interface IMFNetCredentialCache
    {
        CONST_VTBL struct IMFNetCredentialCacheVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFNetCredentialCache_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFNetCredentialCache_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFNetCredentialCache_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFNetCredentialCache_GetCredential(This,pszUrl,pszRealm,dwAuthenticationFlags,ppCred,pdwRequirementsFlags)	\
    ( (This)->lpVtbl -> GetCredential(This,pszUrl,pszRealm,dwAuthenticationFlags,ppCred,pdwRequirementsFlags) ) 

#define IMFNetCredentialCache_SetGood(This,pCred,fGood)	\
    ( (This)->lpVtbl -> SetGood(This,pCred,fGood) ) 

#define IMFNetCredentialCache_SetUserOptions(This,pCred,dwOptionsFlags)	\
    ( (This)->lpVtbl -> SetUserOptions(This,pCred,dwOptionsFlags) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFNetCredentialCache_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0043 */
/* [local] */ 

STDAPI 
MFCreateCredentialCache(
    __out IMFNetCredentialCache ** ppCache);


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0043_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0043_v0_0_s_ifspec;

#ifndef __IMFSourceOpenMonitor_INTERFACE_DEFINED__
#define __IMFSourceOpenMonitor_INTERFACE_DEFINED__

/* interface IMFSourceOpenMonitor */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFSourceOpenMonitor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("059054B3-027C-494C-A27D-9113291CF87F")
    IMFSourceOpenMonitor : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnSourceEvent( 
            /* [in] */ __RPC__in_opt IMFMediaEvent *pEvent) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFSourceOpenMonitorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFSourceOpenMonitor * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFSourceOpenMonitor * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFSourceOpenMonitor * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnSourceEvent )( 
            IMFSourceOpenMonitor * This,
            /* [in] */ __RPC__in_opt IMFMediaEvent *pEvent);
        
        END_INTERFACE
    } IMFSourceOpenMonitorVtbl;

    interface IMFSourceOpenMonitor
    {
        CONST_VTBL struct IMFSourceOpenMonitorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFSourceOpenMonitor_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFSourceOpenMonitor_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFSourceOpenMonitor_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFSourceOpenMonitor_OnSourceEvent(This,pEvent)	\
    ( (This)->lpVtbl -> OnSourceEvent(This,pEvent) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFSourceOpenMonitor_INTERFACE_DEFINED__ */


#ifndef __IMFNetProxyLocator_INTERFACE_DEFINED__
#define __IMFNetProxyLocator_INTERFACE_DEFINED__

/* interface IMFNetProxyLocator */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFNetProxyLocator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e9cd0383-a268-4bb4-82de-658d53574d41")
    IMFNetProxyLocator : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE FindFirstProxy( 
            /* [in] */ LPCWSTR pszHost,
            /* [in] */ LPCWSTR pszUrl,
            /* [in] */ BOOL fReserved) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FindNextProxy( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RegisterProxyResult( 
            /* [in] */ HRESULT hrOp) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentProxy( 
            /* [size_is][out] */ 
            __out_ecount_opt(*pcchStr)  LPWSTR pszStr,
            /* [out][in] */ DWORD *pcchStr) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IMFNetProxyLocator **ppProxyLocator) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFNetProxyLocatorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFNetProxyLocator * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFNetProxyLocator * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFNetProxyLocator * This);
        
        HRESULT ( STDMETHODCALLTYPE *FindFirstProxy )( 
            IMFNetProxyLocator * This,
            /* [in] */ LPCWSTR pszHost,
            /* [in] */ LPCWSTR pszUrl,
            /* [in] */ BOOL fReserved);
        
        HRESULT ( STDMETHODCALLTYPE *FindNextProxy )( 
            IMFNetProxyLocator * This);
        
        HRESULT ( STDMETHODCALLTYPE *RegisterProxyResult )( 
            IMFNetProxyLocator * This,
            /* [in] */ HRESULT hrOp);
        
        HRESULT ( STDMETHODCALLTYPE *GetCurrentProxy )( 
            IMFNetProxyLocator * This,
            /* [size_is][out] */ 
            __out_ecount_opt(*pcchStr)  LPWSTR pszStr,
            /* [out][in] */ DWORD *pcchStr);
        
        HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IMFNetProxyLocator * This,
            /* [out] */ IMFNetProxyLocator **ppProxyLocator);
        
        END_INTERFACE
    } IMFNetProxyLocatorVtbl;

    interface IMFNetProxyLocator
    {
        CONST_VTBL struct IMFNetProxyLocatorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFNetProxyLocator_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFNetProxyLocator_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFNetProxyLocator_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFNetProxyLocator_FindFirstProxy(This,pszHost,pszUrl,fReserved)	\
    ( (This)->lpVtbl -> FindFirstProxy(This,pszHost,pszUrl,fReserved) ) 

#define IMFNetProxyLocator_FindNextProxy(This)	\
    ( (This)->lpVtbl -> FindNextProxy(This) ) 

#define IMFNetProxyLocator_RegisterProxyResult(This,hrOp)	\
    ( (This)->lpVtbl -> RegisterProxyResult(This,hrOp) ) 

#define IMFNetProxyLocator_GetCurrentProxy(This,pszStr,pcchStr)	\
    ( (This)->lpVtbl -> GetCurrentProxy(This,pszStr,pcchStr) ) 

#define IMFNetProxyLocator_Clone(This,ppProxyLocator)	\
    ( (This)->lpVtbl -> Clone(This,ppProxyLocator) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFNetProxyLocator_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0045 */
/* [local] */ 

STDAPI MFCreateProxyLocator(
    LPCWSTR pszProtocol,
    IPropertyStore* pProxyConfig, 
    __out IMFNetProxyLocator** ppProxyLocator );


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0045_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0045_v0_0_s_ifspec;

#ifndef __IMFNetProxyLocatorFactory_INTERFACE_DEFINED__
#define __IMFNetProxyLocatorFactory_INTERFACE_DEFINED__

/* interface IMFNetProxyLocatorFactory */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFNetProxyLocatorFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e9cd0384-a268-4bb4-82de-658d53574d41")
    IMFNetProxyLocatorFactory : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateProxyLocator( 
            /* [in] */ LPCWSTR pszProtocol,
            /* [out] */ 
            __out  IMFNetProxyLocator **ppProxyLocator) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFNetProxyLocatorFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFNetProxyLocatorFactory * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFNetProxyLocatorFactory * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFNetProxyLocatorFactory * This);
        
        HRESULT ( STDMETHODCALLTYPE *CreateProxyLocator )( 
            IMFNetProxyLocatorFactory * This,
            /* [in] */ LPCWSTR pszProtocol,
            /* [out] */ 
            __out  IMFNetProxyLocator **ppProxyLocator);
        
        END_INTERFACE
    } IMFNetProxyLocatorFactoryVtbl;

    interface IMFNetProxyLocatorFactory
    {
        CONST_VTBL struct IMFNetProxyLocatorFactoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFNetProxyLocatorFactory_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFNetProxyLocatorFactory_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFNetProxyLocatorFactory_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFNetProxyLocatorFactory_CreateProxyLocator(This,pszProtocol,ppProxyLocator)	\
    ( (This)->lpVtbl -> CreateProxyLocator(This,pszProtocol,ppProxyLocator) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFNetProxyLocatorFactory_INTERFACE_DEFINED__ */


#ifndef __IMFSaveJob_INTERFACE_DEFINED__
#define __IMFSaveJob_INTERFACE_DEFINED__

/* interface IMFSaveJob */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFSaveJob;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e9931663-80bf-4c6e-98af-5dcf58747d1f")
    IMFSaveJob : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE BeginSave( 
            /* [in] */ IMFByteStream *pStream,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *pState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndSave( 
            /* [in] */ IMFAsyncResult *pResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CancelSave( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProgress( 
            /* [out] */ 
            __out  DWORD *pdwPercentComplete) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFSaveJobVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFSaveJob * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFSaveJob * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFSaveJob * This);
        
        HRESULT ( STDMETHODCALLTYPE *BeginSave )( 
            IMFSaveJob * This,
            /* [in] */ IMFByteStream *pStream,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *pState);
        
        HRESULT ( STDMETHODCALLTYPE *EndSave )( 
            IMFSaveJob * This,
            /* [in] */ IMFAsyncResult *pResult);
        
        HRESULT ( STDMETHODCALLTYPE *CancelSave )( 
            IMFSaveJob * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetProgress )( 
            IMFSaveJob * This,
            /* [out] */ 
            __out  DWORD *pdwPercentComplete);
        
        END_INTERFACE
    } IMFSaveJobVtbl;

    interface IMFSaveJob
    {
        CONST_VTBL struct IMFSaveJobVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFSaveJob_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFSaveJob_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFSaveJob_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFSaveJob_BeginSave(This,pStream,pCallback,pState)	\
    ( (This)->lpVtbl -> BeginSave(This,pStream,pCallback,pState) ) 

#define IMFSaveJob_EndSave(This,pResult)	\
    ( (This)->lpVtbl -> EndSave(This,pResult) ) 

#define IMFSaveJob_CancelSave(This)	\
    ( (This)->lpVtbl -> CancelSave(This) ) 

#define IMFSaveJob_GetProgress(This,pdwPercentComplete)	\
    ( (This)->lpVtbl -> GetProgress(This,pdwPercentComplete) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFSaveJob_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0047 */
/* [local] */ 

EXTERN_GUID( MFNET_SAVEJOB_SERVICE, 0xb85a587f, 0x3d02, 0x4e52, 0x95, 0x65, 0x55, 0xd3, 0xec, 0x1e, 0x7f, 0xf7 );
typedef 
enum _MFNETSOURCE_PROTOCOL_TYPE
    {	MFNETSOURCE_UNDEFINED	= 0,
	MFNETSOURCE_HTTP	= ( MFNETSOURCE_UNDEFINED + 1 ) ,
	MFNETSOURCE_RTSP	= ( MFNETSOURCE_HTTP + 1 ) ,
	MFNETSOURCE_MULTICAST	= ( MFNETSOURCE_RTSP + 1 ) ,
	MFNETSOURCE_RTP	= ( MFNETSOURCE_MULTICAST + 1 ) ,
	MFNETSOURCE_FILE	= ( MFNETSOURCE_RTP + 1 ) 
    } 	MFNETSOURCE_PROTOCOL_TYPE;



extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0047_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0047_v0_0_s_ifspec;

#ifndef __IMFNetSchemeHandlerConfig_INTERFACE_DEFINED__
#define __IMFNetSchemeHandlerConfig_INTERFACE_DEFINED__

/* interface IMFNetSchemeHandlerConfig */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFNetSchemeHandlerConfig;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7BE19E73-C9BF-468a-AC5A-A5E8653BEC87")
    IMFNetSchemeHandlerConfig : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetNumberOfSupportedProtocols( 
            /* [out] */ 
            __out  ULONG *pcProtocols) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSupportedProtocolType( 
            /* [in] */ ULONG nProtocolIndex,
            /* [out] */ 
            __out  MFNETSOURCE_PROTOCOL_TYPE *pnProtocolType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ResetProtocolRolloverSettings( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFNetSchemeHandlerConfigVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFNetSchemeHandlerConfig * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFNetSchemeHandlerConfig * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFNetSchemeHandlerConfig * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetNumberOfSupportedProtocols )( 
            IMFNetSchemeHandlerConfig * This,
            /* [out] */ 
            __out  ULONG *pcProtocols);
        
        HRESULT ( STDMETHODCALLTYPE *GetSupportedProtocolType )( 
            IMFNetSchemeHandlerConfig * This,
            /* [in] */ ULONG nProtocolIndex,
            /* [out] */ 
            __out  MFNETSOURCE_PROTOCOL_TYPE *pnProtocolType);
        
        HRESULT ( STDMETHODCALLTYPE *ResetProtocolRolloverSettings )( 
            IMFNetSchemeHandlerConfig * This);
        
        END_INTERFACE
    } IMFNetSchemeHandlerConfigVtbl;

    interface IMFNetSchemeHandlerConfig
    {
        CONST_VTBL struct IMFNetSchemeHandlerConfigVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFNetSchemeHandlerConfig_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFNetSchemeHandlerConfig_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFNetSchemeHandlerConfig_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFNetSchemeHandlerConfig_GetNumberOfSupportedProtocols(This,pcProtocols)	\
    ( (This)->lpVtbl -> GetNumberOfSupportedProtocols(This,pcProtocols) ) 

#define IMFNetSchemeHandlerConfig_GetSupportedProtocolType(This,nProtocolIndex,pnProtocolType)	\
    ( (This)->lpVtbl -> GetSupportedProtocolType(This,nProtocolIndex,pnProtocolType) ) 

#define IMFNetSchemeHandlerConfig_ResetProtocolRolloverSettings(This)	\
    ( (This)->lpVtbl -> ResetProtocolRolloverSettings(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFNetSchemeHandlerConfig_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0048 */
/* [local] */ 

STDAPI MFCreateNetSchemePlugin(
    REFIID riid, 
    LPVOID *ppvHandler );
typedef 
enum _MFNETSOURCE_TRANSPORT_TYPE
    {	MFNETSOURCE_UDP	= 0,
	MFNETSOURCE_TCP	= ( MFNETSOURCE_UDP + 1 ) 
    } 	MFNETSOURCE_TRANSPORT_TYPE;

typedef 
enum _MFNETSOURCE_CACHE_STATE
    {	MFNETSOURCE_CACHE_UNAVAILABLE	= 0,
	MFNETSOURCE_CACHE_ACTIVE_WRITING	= ( MFNETSOURCE_CACHE_UNAVAILABLE + 1 ) ,
	MFNETSOURCE_CACHE_ACTIVE_COMPLETE	= ( MFNETSOURCE_CACHE_ACTIVE_WRITING + 1 ) 
    } 	MFNETSOURCE_CACHE_STATE;

typedef 
enum _MFNETSOURCE_STATISTICS_IDS
    {	MFNETSOURCE_RECVPACKETS_ID	= 0,
	MFNETSOURCE_LOSTPACKETS_ID	= ( MFNETSOURCE_RECVPACKETS_ID + 1 ) ,
	MFNETSOURCE_RESENDSREQUESTED_ID	= ( MFNETSOURCE_LOSTPACKETS_ID + 1 ) ,
	MFNETSOURCE_RESENDSRECEIVED_ID	= ( MFNETSOURCE_RESENDSREQUESTED_ID + 1 ) ,
	MFNETSOURCE_RECOVEREDBYECCPACKETS_ID	= ( MFNETSOURCE_RESENDSRECEIVED_ID + 1 ) ,
	MFNETSOURCE_RECOVEREDBYRTXPACKETS_ID	= ( MFNETSOURCE_RECOVEREDBYECCPACKETS_ID + 1 ) ,
	MFNETSOURCE_OUTPACKETS_ID	= ( MFNETSOURCE_RECOVEREDBYRTXPACKETS_ID + 1 ) ,
	MFNETSOURCE_RECVRATE_ID	= ( MFNETSOURCE_OUTPACKETS_ID + 1 ) ,
	MFNETSOURCE_AVGBANDWIDTHBPS_ID	= ( MFNETSOURCE_RECVRATE_ID + 1 ) ,
	MFNETSOURCE_BYTESRECEIVED_ID	= ( MFNETSOURCE_AVGBANDWIDTHBPS_ID + 1 ) ,
	MFNETSOURCE_PROTOCOL_ID	= ( MFNETSOURCE_BYTESRECEIVED_ID + 1 ) ,
	MFNETSOURCE_TRANSPORT_ID	= ( MFNETSOURCE_PROTOCOL_ID + 1 ) ,
	MFNETSOURCE_CACHE_STATE_ID	= ( MFNETSOURCE_TRANSPORT_ID + 1 ) ,
	MFNETSOURCE_LINKBANDWIDTH_ID	= ( MFNETSOURCE_CACHE_STATE_ID + 1 ) ,
	MFNETSOURCE_CONTENTBITRATE_ID	= ( MFNETSOURCE_LINKBANDWIDTH_ID + 1 ) ,
	MFNETSOURCE_SPEEDFACTOR_ID	= ( MFNETSOURCE_CONTENTBITRATE_ID + 1 ) ,
	MFNETSOURCE_BUFFERSIZE_ID	= ( MFNETSOURCE_SPEEDFACTOR_ID + 1 ) ,
	MFNETSOURCE_BUFFERPROGRESS_ID	= ( MFNETSOURCE_BUFFERSIZE_ID + 1 ) ,
	MFNETSOURCE_LASTBWSWITCHTS_ID	= ( MFNETSOURCE_BUFFERPROGRESS_ID + 1 ) ,
	MFNETSOURCE_SEEKRANGESTART_ID	= ( MFNETSOURCE_LASTBWSWITCHTS_ID + 1 ) ,
	MFNETSOURCE_SEEKRANGEEND_ID	= ( MFNETSOURCE_SEEKRANGESTART_ID + 1 ) ,
	MFNETSOURCE_BUFFERINGCOUNT_ID	= ( MFNETSOURCE_SEEKRANGEEND_ID + 1 ) ,
	MFNETSOURCE_INCORRECTLYSIGNEDPACKETS_ID	= ( MFNETSOURCE_BUFFERINGCOUNT_ID + 1 ) ,
	MFNETSOURCE_SIGNEDSESSION_ID	= ( MFNETSOURCE_INCORRECTLYSIGNEDPACKETS_ID + 1 ) ,
	MFNETSOURCE_MAXBITRATE_ID	= ( MFNETSOURCE_SIGNEDSESSION_ID + 1 ) ,
	MFNETSOURCE_RECEPTION_QUALITY_ID	= ( MFNETSOURCE_MAXBITRATE_ID + 1 ) ,
	MFNETSOURCE_RECOVEREDPACKETS_ID	= ( MFNETSOURCE_RECEPTION_QUALITY_ID + 1 ) ,
	MFNETSOURCE_VBR_ID	= ( MFNETSOURCE_RECOVEREDPACKETS_ID + 1 ) ,
	MFNETSOURCE_DOWNLOADPROGRESS_ID	= ( MFNETSOURCE_VBR_ID + 1 ) 
    } 	MFNETSOURCE_STATISTICS_IDS;

EXTERN_GUID( MFNETSOURCE_STATISTICS_SERVICE, 0x3cb1f275, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_STATISTICS, 0x3cb1f274, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_BUFFERINGTIME, 0x3cb1f276, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_ACCELERATEDSTREAMINGDURATION, 0x3cb1f277, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_MAXUDPACCELERATEDSTREAMINGDURATION, 0x4aab2879, 0xbbe1, 0x4994, 0x9f, 0xf0, 0x54, 0x95, 0xbd, 0x25, 0x1, 0x29 );
EXTERN_GUID( MFNETSOURCE_MAXBUFFERTIMEMS, 0x408b24e6, 0x4038, 0x4401, 0xb5, 0xb2, 0xfe, 0x70, 0x1a, 0x9e, 0xbf, 0x10 );
EXTERN_GUID( MFNETSOURCE_CONNECTIONBANDWIDTH, 0x3cb1f278, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_CACHEENABLED, 0x3cb1f279, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_AUTORECONNECTLIMIT, 0x3cb1f27a, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_RESENDSENABLED, 0x3cb1f27b, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_THINNINGENABLED, 0x3cb1f27c, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_PROTOCOL, 0x3cb1f27d, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_TRANSPORT, 0x3cb1f27e, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_CREDENTIAL_MANAGER, 0x3cb1f280, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_PPBANDWIDTH, 0x3cb1f281, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_AUTORECONNECTPROGRESS, 0x3cb1f282, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_PROXYLOCATORFACTORY, 0x3cb1f283, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_BROWSERUSERAGENT, 0x3cb1f28b, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_BROWSERWEBPAGE, 0x3cb1f28c, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_PLAYERVERSION, 0x3cb1f28d, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_PLAYERID, 0x3cb1f28e, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_HOSTEXE, 0x3cb1f28f, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_HOSTVERSION, 0x3cb1f291, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_PLAYERUSERAGENT, 0x3cb1f292, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_LOGURL, 0x3cb1f293, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_ENABLE_UDP, 0x3cb1f294, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_ENABLE_TCP, 0x3cb1f295, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_ENABLE_MSB, 0x3cb1f296, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_ENABLE_RTPM, 0x3cb1f297, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_ENABLE_RTSP, 0x3cb1f298, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_ENABLE_HTTP, 0x3cb1f299, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_ENABLE_STREAMING, 0x3cb1f29c, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_ENABLE_DOWNLOAD, 0x3cb1f29d, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_UDP_PORT_RANGE, 0x3cb1f29a, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_PROXYINFO, 0x3cb1f29b, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_DRMNET_LICENSE_REPRESENTATION, 0x47eae1bd, 0xbdfe, 0x42e2, 0x82, 0xf3, 0x54, 0xa4, 0x8c, 0x17, 0x96, 0x2d );
EXTERN_GUID( MFNETSOURCE_PROXYSETTINGS, 0x3cb1f287, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_PROXYHOSTNAME, 0x3cb1f284, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_PROXYPORT, 0x3cb1f288, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_PROXYEXCEPTIONLIST, 0x3cb1f285, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_PROXYBYPASSFORLOCAL, 0x3cb1f286, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
EXTERN_GUID( MFNETSOURCE_PROXYRERUNAUTODETECTION, 0x3cb1f289, 0x0505, 0x4c5d, 0xae, 0x71, 0x0a, 0x55, 0x63, 0x44, 0xef, 0xa1 );
typedef 
enum _MFNET_PROXYSETTINGS
    {	MFNET_PROXYSETTING_NONE	= 0,
	MFNET_PROXYSETTING_MANUAL	= 1,
	MFNET_PROXYSETTING_AUTO	= 2,
	MFNET_PROXYSETTING_BROWSER	= 3
    } 	MFNET_PROXYSETTINGS;



extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0048_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0048_v0_0_s_ifspec;

#ifndef __IMFSchemeHandler_INTERFACE_DEFINED__
#define __IMFSchemeHandler_INTERFACE_DEFINED__

/* interface IMFSchemeHandler */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFSchemeHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6D4C7B74-52A0-4bb7-B0DB-55F29F47A668")
    IMFSchemeHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE BeginCreateObject( 
            /* [in] */ LPCWSTR pwszURL,
            /* [in] */ DWORD dwFlags,
            /* [in] */ IPropertyStore *pProps,
            /* [out] */ 
            __out_opt  IUnknown **ppIUnknownCancelCookie,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndCreateObject( 
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  MF_OBJECT_TYPE *pObjectType,
            /* [out] */ 
            __out  IUnknown **ppObject) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CancelObjectCreation( 
            /* [in] */ IUnknown *pIUnknownCancelCookie) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFSchemeHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFSchemeHandler * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFSchemeHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFSchemeHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *BeginCreateObject )( 
            IMFSchemeHandler * This,
            /* [in] */ LPCWSTR pwszURL,
            /* [in] */ DWORD dwFlags,
            /* [in] */ IPropertyStore *pProps,
            /* [out] */ 
            __out_opt  IUnknown **ppIUnknownCancelCookie,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState);
        
        HRESULT ( STDMETHODCALLTYPE *EndCreateObject )( 
            IMFSchemeHandler * This,
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  MF_OBJECT_TYPE *pObjectType,
            /* [out] */ 
            __out  IUnknown **ppObject);
        
        HRESULT ( STDMETHODCALLTYPE *CancelObjectCreation )( 
            IMFSchemeHandler * This,
            /* [in] */ IUnknown *pIUnknownCancelCookie);
        
        END_INTERFACE
    } IMFSchemeHandlerVtbl;

    interface IMFSchemeHandler
    {
        CONST_VTBL struct IMFSchemeHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFSchemeHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFSchemeHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFSchemeHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFSchemeHandler_BeginCreateObject(This,pwszURL,dwFlags,pProps,ppIUnknownCancelCookie,pCallback,punkState)	\
    ( (This)->lpVtbl -> BeginCreateObject(This,pwszURL,dwFlags,pProps,ppIUnknownCancelCookie,pCallback,punkState) ) 

#define IMFSchemeHandler_EndCreateObject(This,pResult,pObjectType,ppObject)	\
    ( (This)->lpVtbl -> EndCreateObject(This,pResult,pObjectType,ppObject) ) 

#define IMFSchemeHandler_CancelObjectCreation(This,pIUnknownCancelCookie)	\
    ( (This)->lpVtbl -> CancelObjectCreation(This,pIUnknownCancelCookie) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFSchemeHandler_INTERFACE_DEFINED__ */


#ifndef __IMFByteStreamHandler_INTERFACE_DEFINED__
#define __IMFByteStreamHandler_INTERFACE_DEFINED__

/* interface IMFByteStreamHandler */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFByteStreamHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BB420AA4-765B-4a1f-91FE-D6A8A143924C")
    IMFByteStreamHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE BeginCreateObject( 
            /* [in] */ IMFByteStream *pByteStream,
            /* [in] */ LPCWSTR pwszURL,
            /* [in] */ DWORD dwFlags,
            /* [in] */ IPropertyStore *pProps,
            /* [out] */ 
            __out_opt  IUnknown **ppIUnknownCancelCookie,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndCreateObject( 
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  MF_OBJECT_TYPE *pObjectType,
            /* [out] */ 
            __out  IUnknown **ppObject) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CancelObjectCreation( 
            /* [in] */ IUnknown *pIUnknownCancelCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMaxNumberOfBytesRequiredForResolution( 
            /* [out] */ 
            __out  QWORD *pqwBytes) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFByteStreamHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFByteStreamHandler * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFByteStreamHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFByteStreamHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *BeginCreateObject )( 
            IMFByteStreamHandler * This,
            /* [in] */ IMFByteStream *pByteStream,
            /* [in] */ LPCWSTR pwszURL,
            /* [in] */ DWORD dwFlags,
            /* [in] */ IPropertyStore *pProps,
            /* [out] */ 
            __out_opt  IUnknown **ppIUnknownCancelCookie,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState);
        
        HRESULT ( STDMETHODCALLTYPE *EndCreateObject )( 
            IMFByteStreamHandler * This,
            /* [in] */ IMFAsyncResult *pResult,
            /* [out] */ 
            __out  MF_OBJECT_TYPE *pObjectType,
            /* [out] */ 
            __out  IUnknown **ppObject);
        
        HRESULT ( STDMETHODCALLTYPE *CancelObjectCreation )( 
            IMFByteStreamHandler * This,
            /* [in] */ IUnknown *pIUnknownCancelCookie);
        
        HRESULT ( STDMETHODCALLTYPE *GetMaxNumberOfBytesRequiredForResolution )( 
            IMFByteStreamHandler * This,
            /* [out] */ 
            __out  QWORD *pqwBytes);
        
        END_INTERFACE
    } IMFByteStreamHandlerVtbl;

    interface IMFByteStreamHandler
    {
        CONST_VTBL struct IMFByteStreamHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFByteStreamHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFByteStreamHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFByteStreamHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFByteStreamHandler_BeginCreateObject(This,pByteStream,pwszURL,dwFlags,pProps,ppIUnknownCancelCookie,pCallback,punkState)	\
    ( (This)->lpVtbl -> BeginCreateObject(This,pByteStream,pwszURL,dwFlags,pProps,ppIUnknownCancelCookie,pCallback,punkState) ) 

#define IMFByteStreamHandler_EndCreateObject(This,pResult,pObjectType,ppObject)	\
    ( (This)->lpVtbl -> EndCreateObject(This,pResult,pObjectType,ppObject) ) 

#define IMFByteStreamHandler_CancelObjectCreation(This,pIUnknownCancelCookie)	\
    ( (This)->lpVtbl -> CancelObjectCreation(This,pIUnknownCancelCookie) ) 

#define IMFByteStreamHandler_GetMaxNumberOfBytesRequiredForResolution(This,pqwBytes)	\
    ( (This)->lpVtbl -> GetMaxNumberOfBytesRequiredForResolution(This,pqwBytes) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFByteStreamHandler_INTERFACE_DEFINED__ */


#ifndef __IMFTrustedInput_INTERFACE_DEFINED__
#define __IMFTrustedInput_INTERFACE_DEFINED__

/* interface IMFTrustedInput */
/* [helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMFTrustedInput;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("542612C4-A1B8-4632-B521-DE11EA64A0B0")
    IMFTrustedInput : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetInputTrustAuthority( 
            /* [in] */ DWORD dwStreamID,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ __RPC__deref_out_opt IUnknown **ppunkObject) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFTrustedInputVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFTrustedInput * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFTrustedInput * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFTrustedInput * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetInputTrustAuthority )( 
            IMFTrustedInput * This,
            /* [in] */ DWORD dwStreamID,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ __RPC__deref_out_opt IUnknown **ppunkObject);
        
        END_INTERFACE
    } IMFTrustedInputVtbl;

    interface IMFTrustedInput
    {
        CONST_VTBL struct IMFTrustedInputVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFTrustedInput_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFTrustedInput_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFTrustedInput_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFTrustedInput_GetInputTrustAuthority(This,dwStreamID,riid,ppunkObject)	\
    ( (This)->lpVtbl -> GetInputTrustAuthority(This,dwStreamID,riid,ppunkObject) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFTrustedInput_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0051 */
/* [local] */ 

typedef 
enum _MFPOLICYMANAGER_ACTION
    {	PEACTION_NO	= 0,
	PEACTION_PLAY	= 1,
	PEACTION_COPY	= 2,
	PEACTION_EXPORT	= 3,
	PEACTION_EXTRACT	= 4,
	PEACTION_RESERVED1	= 5,
	PEACTION_RESERVED2	= 6,
	PEACTION_RESERVED3	= 7,
	PEACTION_LAST	= 7
    } 	MFPOLICYMANAGER_ACTION;

typedef struct _MFINPUTTRUSTAUTHORITY_ACTION
    {
    MFPOLICYMANAGER_ACTION Action;
    BYTE *pbTicket;
    DWORD cbTicket;
    } 	MFINPUTTRUSTAUTHORITY_ACCESS_ACTION;

typedef struct _MFINPUTTRUSTAUTHORITY_ACCESS_PARAMS
    {
    DWORD dwSize;
    DWORD dwVer;
    DWORD cbSignatureOffset;
    DWORD cbSignatureSize;
    DWORD cbExtensionOffset;
    DWORD cbExtensionSize;
    DWORD cActions;
    MFINPUTTRUSTAUTHORITY_ACCESS_ACTION rgOutputActions[ 1 ];
    } 	MFINPUTTRUSTAUTHORITY_ACCESS_PARAMS;




extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0051_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0051_v0_0_s_ifspec;

#ifndef __IMFInputTrustAuthority_INTERFACE_DEFINED__
#define __IMFInputTrustAuthority_INTERFACE_DEFINED__

/* interface IMFInputTrustAuthority */
/* [helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMFInputTrustAuthority;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D19F8E98-B126-4446-890C-5DCB7AD71453")
    IMFInputTrustAuthority : public IUnknown
    {
    public:
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetDecrypter( 
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __out  void **ppv) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE RequestAccess( 
            /* [in] */ MFPOLICYMANAGER_ACTION Action,
            /* [out] */ 
            __out  IMFActivate **ppContentEnablerActivate) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetPolicy( 
            /* [in] */ MFPOLICYMANAGER_ACTION Action,
            /* [out] */ 
            __out  IMFOutputPolicy **ppPolicy) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE BindAccess( 
            /* [in] */ 
            __in  MFINPUTTRUSTAUTHORITY_ACCESS_PARAMS *pParam) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE UpdateAccess( 
            /* [in] */ 
            __in  MFINPUTTRUSTAUTHORITY_ACCESS_PARAMS *pParam) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFInputTrustAuthorityVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFInputTrustAuthority * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFInputTrustAuthority * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFInputTrustAuthority * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *GetDecrypter )( 
            IMFInputTrustAuthority * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __out  void **ppv);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *RequestAccess )( 
            IMFInputTrustAuthority * This,
            /* [in] */ MFPOLICYMANAGER_ACTION Action,
            /* [out] */ 
            __out  IMFActivate **ppContentEnablerActivate);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *GetPolicy )( 
            IMFInputTrustAuthority * This,
            /* [in] */ MFPOLICYMANAGER_ACTION Action,
            /* [out] */ 
            __out  IMFOutputPolicy **ppPolicy);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *BindAccess )( 
            IMFInputTrustAuthority * This,
            /* [in] */ 
            __in  MFINPUTTRUSTAUTHORITY_ACCESS_PARAMS *pParam);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *UpdateAccess )( 
            IMFInputTrustAuthority * This,
            /* [in] */ 
            __in  MFINPUTTRUSTAUTHORITY_ACCESS_PARAMS *pParam);
        
        HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IMFInputTrustAuthority * This);
        
        END_INTERFACE
    } IMFInputTrustAuthorityVtbl;

    interface IMFInputTrustAuthority
    {
        CONST_VTBL struct IMFInputTrustAuthorityVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFInputTrustAuthority_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFInputTrustAuthority_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFInputTrustAuthority_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFInputTrustAuthority_GetDecrypter(This,riid,ppv)	\
    ( (This)->lpVtbl -> GetDecrypter(This,riid,ppv) ) 

#define IMFInputTrustAuthority_RequestAccess(This,Action,ppContentEnablerActivate)	\
    ( (This)->lpVtbl -> RequestAccess(This,Action,ppContentEnablerActivate) ) 

#define IMFInputTrustAuthority_GetPolicy(This,Action,ppPolicy)	\
    ( (This)->lpVtbl -> GetPolicy(This,Action,ppPolicy) ) 

#define IMFInputTrustAuthority_BindAccess(This,pParam)	\
    ( (This)->lpVtbl -> BindAccess(This,pParam) ) 

#define IMFInputTrustAuthority_UpdateAccess(This,pParam)	\
    ( (This)->lpVtbl -> UpdateAccess(This,pParam) ) 

#define IMFInputTrustAuthority_Reset(This)	\
    ( (This)->lpVtbl -> Reset(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFInputTrustAuthority_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0052 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0052_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0052_v0_0_s_ifspec;

#ifndef __IMFTrustedOutput_INTERFACE_DEFINED__
#define __IMFTrustedOutput_INTERFACE_DEFINED__

/* interface IMFTrustedOutput */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMFTrustedOutput;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D19F8E95-B126-4446-890C-5DCB7AD71453")
    IMFTrustedOutput : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetOutputTrustAuthorityCount( 
            /* [out] */ 
            __out  DWORD *pcOutputTrustAuthorities) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetOutputTrustAuthorityByIndex( 
            /* [in] */ DWORD dwIndex,
            /* [out] */ 
            __out  IMFOutputTrustAuthority **ppauthority) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsFinal( 
            /* [out] */ 
            __out  BOOL *pfIsFinal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFTrustedOutputVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFTrustedOutput * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFTrustedOutput * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFTrustedOutput * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetOutputTrustAuthorityCount )( 
            IMFTrustedOutput * This,
            /* [out] */ 
            __out  DWORD *pcOutputTrustAuthorities);
        
        HRESULT ( STDMETHODCALLTYPE *GetOutputTrustAuthorityByIndex )( 
            IMFTrustedOutput * This,
            /* [in] */ DWORD dwIndex,
            /* [out] */ 
            __out  IMFOutputTrustAuthority **ppauthority);
        
        HRESULT ( STDMETHODCALLTYPE *IsFinal )( 
            IMFTrustedOutput * This,
            /* [out] */ 
            __out  BOOL *pfIsFinal);
        
        END_INTERFACE
    } IMFTrustedOutputVtbl;

    interface IMFTrustedOutput
    {
        CONST_VTBL struct IMFTrustedOutputVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFTrustedOutput_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFTrustedOutput_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFTrustedOutput_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFTrustedOutput_GetOutputTrustAuthorityCount(This,pcOutputTrustAuthorities)	\
    ( (This)->lpVtbl -> GetOutputTrustAuthorityCount(This,pcOutputTrustAuthorities) ) 

#define IMFTrustedOutput_GetOutputTrustAuthorityByIndex(This,dwIndex,ppauthority)	\
    ( (This)->lpVtbl -> GetOutputTrustAuthorityByIndex(This,dwIndex,ppauthority) ) 

#define IMFTrustedOutput_IsFinal(This,pfIsFinal)	\
    ( (This)->lpVtbl -> IsFinal(This,pfIsFinal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFTrustedOutput_INTERFACE_DEFINED__ */


#ifndef __IMFOutputTrustAuthority_INTERFACE_DEFINED__
#define __IMFOutputTrustAuthority_INTERFACE_DEFINED__

/* interface IMFOutputTrustAuthority */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMFOutputTrustAuthority;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D19F8E94-B126-4446-890C-5DCB7AD71453")
    IMFOutputTrustAuthority : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetAction( 
            /* [out] */ 
            __out  MFPOLICYMANAGER_ACTION *pAction) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetPolicy( 
            /* [in] */ 
            __in_ecount_opt(nPolicy)  IMFOutputPolicy **ppPolicy,
            /* [in] */ DWORD nPolicy,
            /* [size_is][size_is][unique][out] */ 
            __deref_opt_out_bcount(*pcbTicket)  BYTE **ppbTicket,
            /* [out] */ 
            __out_opt  DWORD *pcbTicket) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFOutputTrustAuthorityVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFOutputTrustAuthority * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFOutputTrustAuthority * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFOutputTrustAuthority * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetAction )( 
            IMFOutputTrustAuthority * This,
            /* [out] */ 
            __out  MFPOLICYMANAGER_ACTION *pAction);
        
        HRESULT ( STDMETHODCALLTYPE *SetPolicy )( 
            IMFOutputTrustAuthority * This,
            /* [in] */ 
            __in_ecount_opt(nPolicy)  IMFOutputPolicy **ppPolicy,
            /* [in] */ DWORD nPolicy,
            /* [size_is][size_is][unique][out] */ 
            __deref_opt_out_bcount(*pcbTicket)  BYTE **ppbTicket,
            /* [out] */ 
            __out_opt  DWORD *pcbTicket);
        
        END_INTERFACE
    } IMFOutputTrustAuthorityVtbl;

    interface IMFOutputTrustAuthority
    {
        CONST_VTBL struct IMFOutputTrustAuthorityVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFOutputTrustAuthority_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFOutputTrustAuthority_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFOutputTrustAuthority_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFOutputTrustAuthority_GetAction(This,pAction)	\
    ( (This)->lpVtbl -> GetAction(This,pAction) ) 

#define IMFOutputTrustAuthority_SetPolicy(This,ppPolicy,nPolicy,ppbTicket,pcbTicket)	\
    ( (This)->lpVtbl -> SetPolicy(This,ppPolicy,nPolicy,ppbTicket,pcbTicket) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFOutputTrustAuthority_INTERFACE_DEFINED__ */


#ifndef __IMFOutputPolicy_INTERFACE_DEFINED__
#define __IMFOutputPolicy_INTERFACE_DEFINED__

/* interface IMFOutputPolicy */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMFOutputPolicy;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7F00F10A-DAED-41AF-AB26-5FDFA4DFBA3C")
    IMFOutputPolicy : public IMFAttributes
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GenerateRequiredSchemas( 
            /* [in] */ DWORD dwAttributes,
            /* [in] */ GUID guidOutputSubType,
            /* [in] */ GUID *rgGuidProtectionSchemasSupported,
            /* [in] */ DWORD cProtectionSchemasSupported,
            /* [out] */ 
            __out  IMFCollection **ppRequiredProtectionSchemas) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetOriginatorID( 
            /* [out] */ 
            __out  GUID *pguidOriginatorID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMinimumGRLVersion( 
            /* [out] */ 
            __out  DWORD *pdwMinimumGRLVersion) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFOutputPolicyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFOutputPolicy * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFOutputPolicy * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFOutputPolicy * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetItem )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            /* [full][out][in] */ PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemType )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            /* [out] */ MF_ATTRIBUTE_TYPE *pType);
        
        HRESULT ( STDMETHODCALLTYPE *CompareItem )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            REFPROPVARIANT Value,
            /* [out] */ BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *Compare )( 
            IMFOutputPolicy * This,
            IMFAttributes *pTheirs,
            MF_ATTRIBUTES_MATCH_TYPE MatchType,
            /* [out] */ BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT32 )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            /* [out] */ UINT32 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT64 )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            /* [out] */ UINT64 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetDouble )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            /* [out] */ double *pfValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetGUID )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            /* [out] */ GUID *pguidValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetStringLength )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            /* [out] */ UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetString )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            /* [size_is][out] */ LPWSTR pwszValue,
            UINT32 cchBufSize,
            /* [full][out][in] */ UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedString )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            /* [size_is][size_is][out] */ LPWSTR *ppwszValue,
            /* [out] */ UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlobSize )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            /* [out] */ UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlob )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            /* [size_is][out] */ UINT8 *pBuf,
            UINT32 cbBufSize,
            /* [full][out][in] */ UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedBlob )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            /* [size_is][size_is][out] */ UINT8 **ppBuf,
            /* [out] */ UINT32 *pcbSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetUnknown )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            REFIID riid,
            /* [iid_is][out] */ LPVOID *ppv);
        
        HRESULT ( STDMETHODCALLTYPE *SetItem )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            REFPROPVARIANT Value);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteItem )( 
            IMFOutputPolicy * This,
            REFGUID guidKey);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteAllItems )( 
            IMFOutputPolicy * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT32 )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            UINT32 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT64 )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            UINT64 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetDouble )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            double fValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetGUID )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            REFGUID guidValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetString )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            /* [string][in] */ LPCWSTR wszValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetBlob )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            /* [size_is][in] */ const UINT8 *pBuf,
            UINT32 cbBufSize);
        
        HRESULT ( STDMETHODCALLTYPE *SetUnknown )( 
            IMFOutputPolicy * This,
            REFGUID guidKey,
            /* [in] */ IUnknown *pUnknown);
        
        HRESULT ( STDMETHODCALLTYPE *LockStore )( 
            IMFOutputPolicy * This);
        
        HRESULT ( STDMETHODCALLTYPE *UnlockStore )( 
            IMFOutputPolicy * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IMFOutputPolicy * This,
            /* [out] */ UINT32 *pcItems);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemByIndex )( 
            IMFOutputPolicy * This,
            UINT32 unIndex,
            /* [out] */ GUID *pguidKey,
            /* [full][out][in] */ PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *CopyAllItems )( 
            IMFOutputPolicy * This,
            /* [in] */ IMFAttributes *pDest);
        
        HRESULT ( STDMETHODCALLTYPE *GenerateRequiredSchemas )( 
            IMFOutputPolicy * This,
            /* [in] */ DWORD dwAttributes,
            /* [in] */ GUID guidOutputSubType,
            /* [in] */ GUID *rgGuidProtectionSchemasSupported,
            /* [in] */ DWORD cProtectionSchemasSupported,
            /* [out] */ 
            __out  IMFCollection **ppRequiredProtectionSchemas);
        
        HRESULT ( STDMETHODCALLTYPE *GetOriginatorID )( 
            IMFOutputPolicy * This,
            /* [out] */ 
            __out  GUID *pguidOriginatorID);
        
        HRESULT ( STDMETHODCALLTYPE *GetMinimumGRLVersion )( 
            IMFOutputPolicy * This,
            /* [out] */ 
            __out  DWORD *pdwMinimumGRLVersion);
        
        END_INTERFACE
    } IMFOutputPolicyVtbl;

    interface IMFOutputPolicy
    {
        CONST_VTBL struct IMFOutputPolicyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFOutputPolicy_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFOutputPolicy_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFOutputPolicy_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFOutputPolicy_GetItem(This,guidKey,pValue)	\
    ( (This)->lpVtbl -> GetItem(This,guidKey,pValue) ) 

#define IMFOutputPolicy_GetItemType(This,guidKey,pType)	\
    ( (This)->lpVtbl -> GetItemType(This,guidKey,pType) ) 

#define IMFOutputPolicy_CompareItem(This,guidKey,Value,pbResult)	\
    ( (This)->lpVtbl -> CompareItem(This,guidKey,Value,pbResult) ) 

#define IMFOutputPolicy_Compare(This,pTheirs,MatchType,pbResult)	\
    ( (This)->lpVtbl -> Compare(This,pTheirs,MatchType,pbResult) ) 

#define IMFOutputPolicy_GetUINT32(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT32(This,guidKey,punValue) ) 

#define IMFOutputPolicy_GetUINT64(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT64(This,guidKey,punValue) ) 

#define IMFOutputPolicy_GetDouble(This,guidKey,pfValue)	\
    ( (This)->lpVtbl -> GetDouble(This,guidKey,pfValue) ) 

#define IMFOutputPolicy_GetGUID(This,guidKey,pguidValue)	\
    ( (This)->lpVtbl -> GetGUID(This,guidKey,pguidValue) ) 

#define IMFOutputPolicy_GetStringLength(This,guidKey,pcchLength)	\
    ( (This)->lpVtbl -> GetStringLength(This,guidKey,pcchLength) ) 

#define IMFOutputPolicy_GetString(This,guidKey,pwszValue,cchBufSize,pcchLength)	\
    ( (This)->lpVtbl -> GetString(This,guidKey,pwszValue,cchBufSize,pcchLength) ) 

#define IMFOutputPolicy_GetAllocatedString(This,guidKey,ppwszValue,pcchLength)	\
    ( (This)->lpVtbl -> GetAllocatedString(This,guidKey,ppwszValue,pcchLength) ) 

#define IMFOutputPolicy_GetBlobSize(This,guidKey,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlobSize(This,guidKey,pcbBlobSize) ) 

#define IMFOutputPolicy_GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize) ) 

#define IMFOutputPolicy_GetAllocatedBlob(This,guidKey,ppBuf,pcbSize)	\
    ( (This)->lpVtbl -> GetAllocatedBlob(This,guidKey,ppBuf,pcbSize) ) 

#define IMFOutputPolicy_GetUnknown(This,guidKey,riid,ppv)	\
    ( (This)->lpVtbl -> GetUnknown(This,guidKey,riid,ppv) ) 

#define IMFOutputPolicy_SetItem(This,guidKey,Value)	\
    ( (This)->lpVtbl -> SetItem(This,guidKey,Value) ) 

#define IMFOutputPolicy_DeleteItem(This,guidKey)	\
    ( (This)->lpVtbl -> DeleteItem(This,guidKey) ) 

#define IMFOutputPolicy_DeleteAllItems(This)	\
    ( (This)->lpVtbl -> DeleteAllItems(This) ) 

#define IMFOutputPolicy_SetUINT32(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT32(This,guidKey,unValue) ) 

#define IMFOutputPolicy_SetUINT64(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT64(This,guidKey,unValue) ) 

#define IMFOutputPolicy_SetDouble(This,guidKey,fValue)	\
    ( (This)->lpVtbl -> SetDouble(This,guidKey,fValue) ) 

#define IMFOutputPolicy_SetGUID(This,guidKey,guidValue)	\
    ( (This)->lpVtbl -> SetGUID(This,guidKey,guidValue) ) 

#define IMFOutputPolicy_SetString(This,guidKey,wszValue)	\
    ( (This)->lpVtbl -> SetString(This,guidKey,wszValue) ) 

#define IMFOutputPolicy_SetBlob(This,guidKey,pBuf,cbBufSize)	\
    ( (This)->lpVtbl -> SetBlob(This,guidKey,pBuf,cbBufSize) ) 

#define IMFOutputPolicy_SetUnknown(This,guidKey,pUnknown)	\
    ( (This)->lpVtbl -> SetUnknown(This,guidKey,pUnknown) ) 

#define IMFOutputPolicy_LockStore(This)	\
    ( (This)->lpVtbl -> LockStore(This) ) 

#define IMFOutputPolicy_UnlockStore(This)	\
    ( (This)->lpVtbl -> UnlockStore(This) ) 

#define IMFOutputPolicy_GetCount(This,pcItems)	\
    ( (This)->lpVtbl -> GetCount(This,pcItems) ) 

#define IMFOutputPolicy_GetItemByIndex(This,unIndex,pguidKey,pValue)	\
    ( (This)->lpVtbl -> GetItemByIndex(This,unIndex,pguidKey,pValue) ) 

#define IMFOutputPolicy_CopyAllItems(This,pDest)	\
    ( (This)->lpVtbl -> CopyAllItems(This,pDest) ) 


#define IMFOutputPolicy_GenerateRequiredSchemas(This,dwAttributes,guidOutputSubType,rgGuidProtectionSchemasSupported,cProtectionSchemasSupported,ppRequiredProtectionSchemas)	\
    ( (This)->lpVtbl -> GenerateRequiredSchemas(This,dwAttributes,guidOutputSubType,rgGuidProtectionSchemasSupported,cProtectionSchemasSupported,ppRequiredProtectionSchemas) ) 

#define IMFOutputPolicy_GetOriginatorID(This,pguidOriginatorID)	\
    ( (This)->lpVtbl -> GetOriginatorID(This,pguidOriginatorID) ) 

#define IMFOutputPolicy_GetMinimumGRLVersion(This,pdwMinimumGRLVersion)	\
    ( (This)->lpVtbl -> GetMinimumGRLVersion(This,pdwMinimumGRLVersion) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFOutputPolicy_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0055 */
/* [local] */ 

#define MFOUTPUTATTRIBUTE_DIGITAL               ((DWORD) 0x00000001)
#define MFOUTPUTATTRIBUTE_NONSTANDARDIMPLEMENTATION	((DWORD) 0x00000002)
#define MFOUTPUTATTRIBUTE_VIDEO                 ((DWORD) 0x00000004)
#define MFOUTPUTATTRIBUTE_COMPRESSED            ((DWORD) 0x00000008)
#define MFOUTPUTATTRIBUTE_SOFTWARE              ((DWORD) 0x00000010)
#define MFOUTPUTATTRIBUTE_BUS                   ((DWORD) 0x00000020)
#define MFOUTPUTATTRIBUTE_BUSIMPLEMENTATION     ((DWORD) 0x0000FF00)
EXTERN_GUID( MFCONNECTOR_SPDIF, 0xb94a712, 0xad3e, 0x4cee, 0x83, 0xce, 0xce, 0x32, 0xe3, 0xdb, 0x65, 0x22);

EXTERN_GUID( MFCONNECTOR_UNKNOWN, 0xac3aef5c, 0xce43, 0x11d9, 0x92, 0xdb, 0x00, 0x0b, 0xdb, 0x28, 0xff, 0x98);
EXTERN_GUID( MFCONNECTOR_PCI,  0xac3aef5d, 0xce43, 0x11d9, 0x92, 0xdb, 0x00, 0x0b, 0xdb, 0x28, 0xff, 0x98);
EXTERN_GUID( MFCONNECTOR_PCIX, 0xac3aef5e, 0xce43, 0x11d9, 0x92, 0xdb, 0x00, 0x0b, 0xdb, 0x28, 0xff, 0x98);
EXTERN_GUID( MFCONNECTOR_PCI_Express, 0xac3aef5f, 0xce43, 0x11d9, 0x92, 0xdb, 0x00, 0x0b, 0xdb, 0x28, 0xff, 0x98);
EXTERN_GUID( MFCONNECTOR_AGP, 0xac3aef60, 0xce43, 0x11d9, 0x92, 0xdb, 0x00, 0x0b, 0xdb, 0x28, 0xff, 0x98);


EXTERN_GUID( MFCONNECTOR_VGA, 0x57cd5968, 0xce47, 0x11d9, 0x92, 0xdb, 0x00, 0x0b, 0xdb, 0x28, 0xff, 0x98);
EXTERN_GUID( MFCONNECTOR_SVIDEO, 0x57cd5969, 0xce47, 0x11d9, 0x92, 0xdb, 0x00, 0x0b, 0xdb, 0x28, 0xff, 0x98);
EXTERN_GUID( MFCONNECTOR_COMPOSITE, 0x57cd596a, 0xce47, 0x11d9, 0x92, 0xdb, 0x00, 0x0b, 0xdb, 0x28, 0xff, 0x98);
EXTERN_GUID( MFCONNECTOR_COMPONENT, 0x57cd596b, 0xce47, 0x11d9, 0x92, 0xdb, 0x00, 0x0b, 0xdb, 0x28, 0xff, 0x98);
EXTERN_GUID( MFCONNECTOR_DVI, 0x57cd596c, 0xce47, 0x11d9, 0x92, 0xdb, 0x00, 0x0b, 0xdb, 0x28, 0xff, 0x98);
EXTERN_GUID( MFCONNECTOR_HDMI, 0x57cd596d, 0xce47, 0x11d9, 0x92, 0xdb, 0x00, 0x0b, 0xdb, 0x28, 0xff, 0x98);
EXTERN_GUID( MFCONNECTOR_LVDS, 0x57cd596e, 0xce47, 0x11d9, 0x92, 0xdb, 0x00, 0x0b, 0xdb, 0x28, 0xff, 0x98);
EXTERN_GUID( MFCONNECTOR_D_JPN, 0x57cd5970, 0xce47, 0x11d9, 0x92, 0xdb, 0x00, 0x0b, 0xdb, 0x28, 0xff, 0x98);
EXTERN_GUID( MFCONNECTOR_SDI, 0x57cd5971, 0xce47, 0x11d9, 0x92, 0xdb, 0x00, 0x0b, 0xdb, 0x28, 0xff, 0x98);
EXTERN_GUID( MFCONNECTOR_DISPLAYPORT_EXTERNAL, 0x57cd5972, 0xce47, 0x11d9, 0x92, 0xdb, 0x00, 0x0b, 0xdb, 0x28, 0xff, 0x98);
EXTERN_GUID( MFCONNECTOR_DISPLAYPORT_EMBEDDED, 0x57cd5973, 0xce47, 0x11d9, 0x92, 0xdb, 0x00, 0x0b, 0xdb, 0x28, 0xff, 0x98);
EXTERN_GUID( MFCONNECTOR_UDI_EXTERNAL, 0x57cd5974, 0xce47, 0x11d9, 0x92, 0xdb, 0x00, 0x0b, 0xdb, 0x28, 0xff, 0x98);
EXTERN_GUID( MFCONNECTOR_UDI_EMBEDDED, 0x57cd5975, 0xce47, 0x11d9, 0x92, 0xdb, 0x00, 0x0b, 0xdb, 0x28, 0xff, 0x98);



extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0055_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0055_v0_0_s_ifspec;

#ifndef __IMFOutputSchema_INTERFACE_DEFINED__
#define __IMFOutputSchema_INTERFACE_DEFINED__

/* interface IMFOutputSchema */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMFOutputSchema;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7BE0FC5B-ABD9-44FB-A5C8-F50136E71599")
    IMFOutputSchema : public IMFAttributes
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetSchemaType( 
            /* [out] */ 
            __out  GUID *pguidSchemaType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetConfigurationData( 
            /* [out] */ 
            __out  DWORD *pdwVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetOriginatorID( 
            /* [out] */ 
            __out  GUID *pguidOriginatorID) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFOutputSchemaVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFOutputSchema * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFOutputSchema * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFOutputSchema * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetItem )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            /* [full][out][in] */ PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemType )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            /* [out] */ MF_ATTRIBUTE_TYPE *pType);
        
        HRESULT ( STDMETHODCALLTYPE *CompareItem )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            REFPROPVARIANT Value,
            /* [out] */ BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *Compare )( 
            IMFOutputSchema * This,
            IMFAttributes *pTheirs,
            MF_ATTRIBUTES_MATCH_TYPE MatchType,
            /* [out] */ BOOL *pbResult);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT32 )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            /* [out] */ UINT32 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetUINT64 )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            /* [out] */ UINT64 *punValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetDouble )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            /* [out] */ double *pfValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetGUID )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            /* [out] */ GUID *pguidValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetStringLength )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            /* [out] */ UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetString )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            /* [size_is][out] */ LPWSTR pwszValue,
            UINT32 cchBufSize,
            /* [full][out][in] */ UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedString )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            /* [size_is][size_is][out] */ LPWSTR *ppwszValue,
            /* [out] */ UINT32 *pcchLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlobSize )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            /* [out] */ UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetBlob )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            /* [size_is][out] */ UINT8 *pBuf,
            UINT32 cbBufSize,
            /* [full][out][in] */ UINT32 *pcbBlobSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetAllocatedBlob )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            /* [size_is][size_is][out] */ UINT8 **ppBuf,
            /* [out] */ UINT32 *pcbSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetUnknown )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            REFIID riid,
            /* [iid_is][out] */ LPVOID *ppv);
        
        HRESULT ( STDMETHODCALLTYPE *SetItem )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            REFPROPVARIANT Value);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteItem )( 
            IMFOutputSchema * This,
            REFGUID guidKey);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteAllItems )( 
            IMFOutputSchema * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT32 )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            UINT32 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetUINT64 )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            UINT64 unValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetDouble )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            double fValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetGUID )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            REFGUID guidValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetString )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            /* [string][in] */ LPCWSTR wszValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetBlob )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            /* [size_is][in] */ const UINT8 *pBuf,
            UINT32 cbBufSize);
        
        HRESULT ( STDMETHODCALLTYPE *SetUnknown )( 
            IMFOutputSchema * This,
            REFGUID guidKey,
            /* [in] */ IUnknown *pUnknown);
        
        HRESULT ( STDMETHODCALLTYPE *LockStore )( 
            IMFOutputSchema * This);
        
        HRESULT ( STDMETHODCALLTYPE *UnlockStore )( 
            IMFOutputSchema * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IMFOutputSchema * This,
            /* [out] */ UINT32 *pcItems);
        
        HRESULT ( STDMETHODCALLTYPE *GetItemByIndex )( 
            IMFOutputSchema * This,
            UINT32 unIndex,
            /* [out] */ GUID *pguidKey,
            /* [full][out][in] */ PROPVARIANT *pValue);
        
        HRESULT ( STDMETHODCALLTYPE *CopyAllItems )( 
            IMFOutputSchema * This,
            /* [in] */ IMFAttributes *pDest);
        
        HRESULT ( STDMETHODCALLTYPE *GetSchemaType )( 
            IMFOutputSchema * This,
            /* [out] */ 
            __out  GUID *pguidSchemaType);
        
        HRESULT ( STDMETHODCALLTYPE *GetConfigurationData )( 
            IMFOutputSchema * This,
            /* [out] */ 
            __out  DWORD *pdwVal);
        
        HRESULT ( STDMETHODCALLTYPE *GetOriginatorID )( 
            IMFOutputSchema * This,
            /* [out] */ 
            __out  GUID *pguidOriginatorID);
        
        END_INTERFACE
    } IMFOutputSchemaVtbl;

    interface IMFOutputSchema
    {
        CONST_VTBL struct IMFOutputSchemaVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFOutputSchema_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFOutputSchema_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFOutputSchema_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFOutputSchema_GetItem(This,guidKey,pValue)	\
    ( (This)->lpVtbl -> GetItem(This,guidKey,pValue) ) 

#define IMFOutputSchema_GetItemType(This,guidKey,pType)	\
    ( (This)->lpVtbl -> GetItemType(This,guidKey,pType) ) 

#define IMFOutputSchema_CompareItem(This,guidKey,Value,pbResult)	\
    ( (This)->lpVtbl -> CompareItem(This,guidKey,Value,pbResult) ) 

#define IMFOutputSchema_Compare(This,pTheirs,MatchType,pbResult)	\
    ( (This)->lpVtbl -> Compare(This,pTheirs,MatchType,pbResult) ) 

#define IMFOutputSchema_GetUINT32(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT32(This,guidKey,punValue) ) 

#define IMFOutputSchema_GetUINT64(This,guidKey,punValue)	\
    ( (This)->lpVtbl -> GetUINT64(This,guidKey,punValue) ) 

#define IMFOutputSchema_GetDouble(This,guidKey,pfValue)	\
    ( (This)->lpVtbl -> GetDouble(This,guidKey,pfValue) ) 

#define IMFOutputSchema_GetGUID(This,guidKey,pguidValue)	\
    ( (This)->lpVtbl -> GetGUID(This,guidKey,pguidValue) ) 

#define IMFOutputSchema_GetStringLength(This,guidKey,pcchLength)	\
    ( (This)->lpVtbl -> GetStringLength(This,guidKey,pcchLength) ) 

#define IMFOutputSchema_GetString(This,guidKey,pwszValue,cchBufSize,pcchLength)	\
    ( (This)->lpVtbl -> GetString(This,guidKey,pwszValue,cchBufSize,pcchLength) ) 

#define IMFOutputSchema_GetAllocatedString(This,guidKey,ppwszValue,pcchLength)	\
    ( (This)->lpVtbl -> GetAllocatedString(This,guidKey,ppwszValue,pcchLength) ) 

#define IMFOutputSchema_GetBlobSize(This,guidKey,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlobSize(This,guidKey,pcbBlobSize) ) 

#define IMFOutputSchema_GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize)	\
    ( (This)->lpVtbl -> GetBlob(This,guidKey,pBuf,cbBufSize,pcbBlobSize) ) 

#define IMFOutputSchema_GetAllocatedBlob(This,guidKey,ppBuf,pcbSize)	\
    ( (This)->lpVtbl -> GetAllocatedBlob(This,guidKey,ppBuf,pcbSize) ) 

#define IMFOutputSchema_GetUnknown(This,guidKey,riid,ppv)	\
    ( (This)->lpVtbl -> GetUnknown(This,guidKey,riid,ppv) ) 

#define IMFOutputSchema_SetItem(This,guidKey,Value)	\
    ( (This)->lpVtbl -> SetItem(This,guidKey,Value) ) 

#define IMFOutputSchema_DeleteItem(This,guidKey)	\
    ( (This)->lpVtbl -> DeleteItem(This,guidKey) ) 

#define IMFOutputSchema_DeleteAllItems(This)	\
    ( (This)->lpVtbl -> DeleteAllItems(This) ) 

#define IMFOutputSchema_SetUINT32(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT32(This,guidKey,unValue) ) 

#define IMFOutputSchema_SetUINT64(This,guidKey,unValue)	\
    ( (This)->lpVtbl -> SetUINT64(This,guidKey,unValue) ) 

#define IMFOutputSchema_SetDouble(This,guidKey,fValue)	\
    ( (This)->lpVtbl -> SetDouble(This,guidKey,fValue) ) 

#define IMFOutputSchema_SetGUID(This,guidKey,guidValue)	\
    ( (This)->lpVtbl -> SetGUID(This,guidKey,guidValue) ) 

#define IMFOutputSchema_SetString(This,guidKey,wszValue)	\
    ( (This)->lpVtbl -> SetString(This,guidKey,wszValue) ) 

#define IMFOutputSchema_SetBlob(This,guidKey,pBuf,cbBufSize)	\
    ( (This)->lpVtbl -> SetBlob(This,guidKey,pBuf,cbBufSize) ) 

#define IMFOutputSchema_SetUnknown(This,guidKey,pUnknown)	\
    ( (This)->lpVtbl -> SetUnknown(This,guidKey,pUnknown) ) 

#define IMFOutputSchema_LockStore(This)	\
    ( (This)->lpVtbl -> LockStore(This) ) 

#define IMFOutputSchema_UnlockStore(This)	\
    ( (This)->lpVtbl -> UnlockStore(This) ) 

#define IMFOutputSchema_GetCount(This,pcItems)	\
    ( (This)->lpVtbl -> GetCount(This,pcItems) ) 

#define IMFOutputSchema_GetItemByIndex(This,unIndex,pguidKey,pValue)	\
    ( (This)->lpVtbl -> GetItemByIndex(This,unIndex,pguidKey,pValue) ) 

#define IMFOutputSchema_CopyAllItems(This,pDest)	\
    ( (This)->lpVtbl -> CopyAllItems(This,pDest) ) 


#define IMFOutputSchema_GetSchemaType(This,pguidSchemaType)	\
    ( (This)->lpVtbl -> GetSchemaType(This,pguidSchemaType) ) 

#define IMFOutputSchema_GetConfigurationData(This,pdwVal)	\
    ( (This)->lpVtbl -> GetConfigurationData(This,pdwVal) ) 

#define IMFOutputSchema_GetOriginatorID(This,pguidOriginatorID)	\
    ( (This)->lpVtbl -> GetOriginatorID(This,pguidOriginatorID) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFOutputSchema_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0056 */
/* [local] */ 


EXTERN_GUID( MFPROTECTION_DISABLE, 0x8cc6d81b, 0xfec6, 0x4d8f, 0x96, 0x4b, 0xcf, 0xba, 0x0b, 0x0d, 0xad, 0x0d);
EXTERN_GUID( MFPROTECTION_CONSTRICTVIDEO, 0x193370ce, 0xc5e4, 0x4c3a, 0x8a, 0x66, 0x69, 0x59, 0xb4, 0xda, 0x44, 0x42);
EXTERN_GUID( MFPROTECTION_CONSTRICTAUDIO, 0xffc99b44, 0xdf48, 0x4e16, 0x8e, 0x66, 0x09, 0x68, 0x92, 0xc1, 0x57, 0x8a);
EXTERN_GUID( MFPROTECTION_TRUSTEDAUDIODRIVERS, 0x65bdf3d2, 0x0168, 0x4816, 0xa5, 0x33, 0x55, 0xd4, 0x7b, 0x02, 0x71, 0x01);
EXTERN_GUID( MFPROTECTION_HDCP, 0xAE7CC03D, 0xC828, 0x4021, 0xac, 0xb7, 0xd5, 0x78, 0xd2, 0x7a, 0xaf, 0x13);
EXTERN_GUID( MFPROTECTION_CGMSA, 0xE57E69E9, 0x226B, 0x4d31, 0xB4, 0xE3, 0xD3, 0xDB, 0x00, 0x87, 0x36, 0xDD);
EXTERN_GUID( MFPROTECTION_ACP, 0xc3fd11c6, 0xf8b7, 0x4d20, 0xb0, 0x08, 0x1d, 0xb1, 0x7d, 0x61, 0xf2, 0xda);
EXTERN_GUID( MFPROTECTION_WMDRMOTA, 0xa267a6a1, 0x362e, 0x47d0, 0x88, 0x05, 0x46, 0x28, 0x59, 0x8a, 0x23, 0xe4);
EXTERN_GUID( MFPROTECTION_FFT, 0x462a56b2, 0x2866, 0x4bb6, 0x98, 0x0d, 0x6d, 0x8d, 0x9e, 0xdb, 0x1a, 0x8c);


EXTERN_GUID( MFPROTECTIONATTRIBUTE_CONSTRICTVIDEO_IMAGESIZE, 0x8476fc, 0x4b58, 0x4d80, 0xa7, 0x90, 0xe7, 0x29, 0x76, 0x73, 0x16, 0x1d);
EXTERN_GUID( MFPROTECTIONATTRIBUTE_HDCP_SRM, 0x6f302107, 0x3477, 0x4468, 0x8a, 0x8, 0xee, 0xf9, 0xdb, 0x10, 0xe2, 0xf);


#define MAKE_MFPROTECTIONDATA_DISABLE(Disable)  \
    ((DWORD)(Disable ? 0x00000001 : 0))

#define EXTRACT_MFPROTECTIONDATA_DISABLE_ON(Data) \
    (0 != ((Data) & 0x00000001))

#define EXTRACT_MFPROTECTIONDATA_DISABLE_RESERVED(Data) \
    (((DWORD)((Data) & 0xFFFFFFFE)) >> 1)


#define MAKE_MFPROTECTIONDATA_CONSTRICTAUDIO(Level) \
    ((DWORD)(Level))

#define EXTRACT_MFPROTECTIONDATA_CONSTRICTAUDIO_LEVEL(Data) \
    ((DWORD)((Data) & 0x000000FF))

#define EXTRACT_MFPROTECTIONDATA_CONSTRICTAUDIO_RESERVED(Data) \
    (((DWORD)((Data) & 0xFFFFFF00)) >> 8)


#define MAKE_MFPROTECTIONDATA_TRUSTEDAUDIODRIVERS(TestCertificateEnable, DigitalOutputDisable, DrmLevel) \
    (((DWORD)((TestCertificateEnable) ? 0x00020000 : 0)) | \
     ((DWORD)((DigitalOutputDisable) ? 0x00010000 : 0)) | \
     ((DWORD)(DrmLevel)))

#define EXTRACT_MFPROTECTIONDATA_TRUSTEDAUDIODRIVERS_DRMLEVEL(Data) \
    ((DWORD)((Data) & 0x0000FFFF))

#define EXTRACT_MFPROTECTIONDATA_TRUSTEDAUDIODRIVERS_DIGITALOUTPUTDISABLE(Data) \
    (0 != ((Data) & 0x00010000))

#define EXTRACT_MFPROTECTIONDATA_TRUSTEDAUDIODRIVERS_TESTCERTIFICATEENABLE(Data) \
    (0 != ((Data) & 0x00020000))

#define EXTRACT_MFPROTECTIONDATA_TRUSTEDAUDIODRIVERS_RESERVED(Data) \
    (((DWORD)((Data) & 0xFFFC0000)) >> 18)



extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0056_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0056_v0_0_s_ifspec;

#ifndef __IMFSecureChannel_INTERFACE_DEFINED__
#define __IMFSecureChannel_INTERFACE_DEFINED__

/* interface IMFSecureChannel */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFSecureChannel;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("d0ae555d-3b12-4d97-b060-0990bc5aeb67")
    IMFSecureChannel : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetCertificate( 
            /* [out] */ 
            __deref_out_bcount(*pcbCert)  BYTE **ppCert,
            /* [out] */ 
            __out  DWORD *pcbCert) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetupSession( 
            /* [in] */ 
            __in_bcount(cbSessionKey)  BYTE *pbEncryptedSessionKey,
            /* [in] */ DWORD cbSessionKey) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFSecureChannelVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFSecureChannel * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFSecureChannel * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFSecureChannel * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCertificate )( 
            IMFSecureChannel * This,
            /* [out] */ 
            __deref_out_bcount(*pcbCert)  BYTE **ppCert,
            /* [out] */ 
            __out  DWORD *pcbCert);
        
        HRESULT ( STDMETHODCALLTYPE *SetupSession )( 
            IMFSecureChannel * This,
            /* [in] */ 
            __in_bcount(cbSessionKey)  BYTE *pbEncryptedSessionKey,
            /* [in] */ DWORD cbSessionKey);
        
        END_INTERFACE
    } IMFSecureChannelVtbl;

    interface IMFSecureChannel
    {
        CONST_VTBL struct IMFSecureChannelVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFSecureChannel_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFSecureChannel_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFSecureChannel_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFSecureChannel_GetCertificate(This,ppCert,pcbCert)	\
    ( (This)->lpVtbl -> GetCertificate(This,ppCert,pcbCert) ) 

#define IMFSecureChannel_SetupSession(This,pbEncryptedSessionKey,cbSessionKey)	\
    ( (This)->lpVtbl -> SetupSession(This,pbEncryptedSessionKey,cbSessionKey) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFSecureChannel_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0057 */
/* [local] */ 

typedef 
enum SAMPLE_PROTECTION_VERSION
    {	SAMPLE_PROTECTION_VERSION_NO	= 0,
	SAMPLE_PROTECTION_VERSION_BASIC_LOKI	= 1,
	SAMPLE_PROTECTION_VERSION_SCATTER	= 2,
	SAMPLE_PROTECTION_VERSION_RC4	= 3
    } 	SAMPLE_PROTECTION_VERSION;

EXTERN_GUID( MF_SampleProtectionSalt, 0x5403deee, 0xb9ee, 0x438f, 0xaa, 0x83, 0x38, 0x4, 0x99, 0x7e, 0x56, 0x9d);


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0057_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0057_v0_0_s_ifspec;

#ifndef __IMFSampleProtection_INTERFACE_DEFINED__
#define __IMFSampleProtection_INTERFACE_DEFINED__

/* interface IMFSampleProtection */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFSampleProtection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8e36395f-c7b9-43c4-a54d-512b4af63c95")
    IMFSampleProtection : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetInputProtectionVersion( 
            /* [out] */ 
            __out  DWORD *pdwVersion) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetOutputProtectionVersion( 
            /* [out] */ 
            __out  DWORD *pdwVersion) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProtectionCertificate( 
            /* [in] */ DWORD dwVersion,
            /* [out] */ 
            __deref_out_bcount(*pcbCert)  BYTE **ppCert,
            /* [out] */ 
            __out  DWORD *pcbCert) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InitOutputProtection( 
            /* [in] */ DWORD dwVersion,
            /* [in] */ DWORD dwOutputId,
            /* [in] */ BYTE *pbCert,
            /* [in] */ DWORD cbCert,
            /* [out] */ BYTE **ppbSeed,
            /* [out] */ DWORD *pcbSeed) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InitInputProtection( 
            /* [in] */ DWORD dwVersion,
            /* [in] */ DWORD dwInputId,
            /* [in] */ BYTE *pbSeed,
            /* [in] */ DWORD cbSeed) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFSampleProtectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFSampleProtection * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFSampleProtection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFSampleProtection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetInputProtectionVersion )( 
            IMFSampleProtection * This,
            /* [out] */ 
            __out  DWORD *pdwVersion);
        
        HRESULT ( STDMETHODCALLTYPE *GetOutputProtectionVersion )( 
            IMFSampleProtection * This,
            /* [out] */ 
            __out  DWORD *pdwVersion);
        
        HRESULT ( STDMETHODCALLTYPE *GetProtectionCertificate )( 
            IMFSampleProtection * This,
            /* [in] */ DWORD dwVersion,
            /* [out] */ 
            __deref_out_bcount(*pcbCert)  BYTE **ppCert,
            /* [out] */ 
            __out  DWORD *pcbCert);
        
        HRESULT ( STDMETHODCALLTYPE *InitOutputProtection )( 
            IMFSampleProtection * This,
            /* [in] */ DWORD dwVersion,
            /* [in] */ DWORD dwOutputId,
            /* [in] */ BYTE *pbCert,
            /* [in] */ DWORD cbCert,
            /* [out] */ BYTE **ppbSeed,
            /* [out] */ DWORD *pcbSeed);
        
        HRESULT ( STDMETHODCALLTYPE *InitInputProtection )( 
            IMFSampleProtection * This,
            /* [in] */ DWORD dwVersion,
            /* [in] */ DWORD dwInputId,
            /* [in] */ BYTE *pbSeed,
            /* [in] */ DWORD cbSeed);
        
        END_INTERFACE
    } IMFSampleProtectionVtbl;

    interface IMFSampleProtection
    {
        CONST_VTBL struct IMFSampleProtectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFSampleProtection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFSampleProtection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFSampleProtection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFSampleProtection_GetInputProtectionVersion(This,pdwVersion)	\
    ( (This)->lpVtbl -> GetInputProtectionVersion(This,pdwVersion) ) 

#define IMFSampleProtection_GetOutputProtectionVersion(This,pdwVersion)	\
    ( (This)->lpVtbl -> GetOutputProtectionVersion(This,pdwVersion) ) 

#define IMFSampleProtection_GetProtectionCertificate(This,dwVersion,ppCert,pcbCert)	\
    ( (This)->lpVtbl -> GetProtectionCertificate(This,dwVersion,ppCert,pcbCert) ) 

#define IMFSampleProtection_InitOutputProtection(This,dwVersion,dwOutputId,pbCert,cbCert,ppbSeed,pcbSeed)	\
    ( (This)->lpVtbl -> InitOutputProtection(This,dwVersion,dwOutputId,pbCert,cbCert,ppbSeed,pcbSeed) ) 

#define IMFSampleProtection_InitInputProtection(This,dwVersion,dwInputId,pbSeed,cbSeed)	\
    ( (This)->lpVtbl -> InitInputProtection(This,dwVersion,dwInputId,pbSeed,cbSeed) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFSampleProtection_INTERFACE_DEFINED__ */


#ifndef __IMFMediaSinkPreroll_INTERFACE_DEFINED__
#define __IMFMediaSinkPreroll_INTERFACE_DEFINED__

/* interface IMFMediaSinkPreroll */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFMediaSinkPreroll;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5dfd4b2a-7674-4110-a4e6-8a68fd5f3688")
    IMFMediaSinkPreroll : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE NotifyPreroll( 
            /* [in] */ MFTIME hnsUpcomingStartTime) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFMediaSinkPrerollVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFMediaSinkPreroll * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFMediaSinkPreroll * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFMediaSinkPreroll * This);
        
        HRESULT ( STDMETHODCALLTYPE *NotifyPreroll )( 
            IMFMediaSinkPreroll * This,
            /* [in] */ MFTIME hnsUpcomingStartTime);
        
        END_INTERFACE
    } IMFMediaSinkPrerollVtbl;

    interface IMFMediaSinkPreroll
    {
        CONST_VTBL struct IMFMediaSinkPrerollVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFMediaSinkPreroll_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFMediaSinkPreroll_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFMediaSinkPreroll_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFMediaSinkPreroll_NotifyPreroll(This,hnsUpcomingStartTime)	\
    ( (This)->lpVtbl -> NotifyPreroll(This,hnsUpcomingStartTime) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFMediaSinkPreroll_INTERFACE_DEFINED__ */


#ifndef __IMFFinalizableMediaSink_INTERFACE_DEFINED__
#define __IMFFinalizableMediaSink_INTERFACE_DEFINED__

/* interface IMFFinalizableMediaSink */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFFinalizableMediaSink;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EAECB74A-9A50-42ce-9541-6A7F57AA4AD7")
    IMFFinalizableMediaSink : public IMFMediaSink
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE BeginFinalize( 
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndFinalize( 
            /* [in] */ IMFAsyncResult *pResult) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFFinalizableMediaSinkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFFinalizableMediaSink * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFFinalizableMediaSink * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFFinalizableMediaSink * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCharacteristics )( 
            IMFFinalizableMediaSink * This,
            /* [out] */ DWORD *pdwCharacteristics);
        
        HRESULT ( STDMETHODCALLTYPE *AddStreamSink )( 
            IMFFinalizableMediaSink * This,
            /* [in] */ DWORD dwStreamSinkIdentifier,
            /* [in] */ IMFMediaType *pMediaType,
            /* [out] */ IMFStreamSink **ppStreamSink);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveStreamSink )( 
            IMFFinalizableMediaSink * This,
            /* [in] */ DWORD dwStreamSinkIdentifier);
        
        HRESULT ( STDMETHODCALLTYPE *GetStreamSinkCount )( 
            IMFFinalizableMediaSink * This,
            /* [out] */ DWORD *pcStreamSinkCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetStreamSinkByIndex )( 
            IMFFinalizableMediaSink * This,
            /* [in] */ DWORD dwIndex,
            /* [out] */ IMFStreamSink **ppStreamSink);
        
        HRESULT ( STDMETHODCALLTYPE *GetStreamSinkById )( 
            IMFFinalizableMediaSink * This,
            /* [in] */ DWORD dwStreamSinkIdentifier,
            /* [out] */ IMFStreamSink **ppStreamSink);
        
        HRESULT ( STDMETHODCALLTYPE *SetPresentationClock )( 
            IMFFinalizableMediaSink * This,
            /* [in] */ IMFPresentationClock *pPresentationClock);
        
        HRESULT ( STDMETHODCALLTYPE *GetPresentationClock )( 
            IMFFinalizableMediaSink * This,
            /* [out] */ IMFPresentationClock **ppPresentationClock);
        
        HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            IMFFinalizableMediaSink * This);
        
        HRESULT ( STDMETHODCALLTYPE *BeginFinalize )( 
            IMFFinalizableMediaSink * This,
            /* [in] */ IMFAsyncCallback *pCallback,
            /* [in] */ IUnknown *punkState);
        
        HRESULT ( STDMETHODCALLTYPE *EndFinalize )( 
            IMFFinalizableMediaSink * This,
            /* [in] */ IMFAsyncResult *pResult);
        
        END_INTERFACE
    } IMFFinalizableMediaSinkVtbl;

    interface IMFFinalizableMediaSink
    {
        CONST_VTBL struct IMFFinalizableMediaSinkVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFFinalizableMediaSink_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFFinalizableMediaSink_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFFinalizableMediaSink_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFFinalizableMediaSink_GetCharacteristics(This,pdwCharacteristics)	\
    ( (This)->lpVtbl -> GetCharacteristics(This,pdwCharacteristics) ) 

#define IMFFinalizableMediaSink_AddStreamSink(This,dwStreamSinkIdentifier,pMediaType,ppStreamSink)	\
    ( (This)->lpVtbl -> AddStreamSink(This,dwStreamSinkIdentifier,pMediaType,ppStreamSink) ) 

#define IMFFinalizableMediaSink_RemoveStreamSink(This,dwStreamSinkIdentifier)	\
    ( (This)->lpVtbl -> RemoveStreamSink(This,dwStreamSinkIdentifier) ) 

#define IMFFinalizableMediaSink_GetStreamSinkCount(This,pcStreamSinkCount)	\
    ( (This)->lpVtbl -> GetStreamSinkCount(This,pcStreamSinkCount) ) 

#define IMFFinalizableMediaSink_GetStreamSinkByIndex(This,dwIndex,ppStreamSink)	\
    ( (This)->lpVtbl -> GetStreamSinkByIndex(This,dwIndex,ppStreamSink) ) 

#define IMFFinalizableMediaSink_GetStreamSinkById(This,dwStreamSinkIdentifier,ppStreamSink)	\
    ( (This)->lpVtbl -> GetStreamSinkById(This,dwStreamSinkIdentifier,ppStreamSink) ) 

#define IMFFinalizableMediaSink_SetPresentationClock(This,pPresentationClock)	\
    ( (This)->lpVtbl -> SetPresentationClock(This,pPresentationClock) ) 

#define IMFFinalizableMediaSink_GetPresentationClock(This,ppPresentationClock)	\
    ( (This)->lpVtbl -> GetPresentationClock(This,ppPresentationClock) ) 

#define IMFFinalizableMediaSink_Shutdown(This)	\
    ( (This)->lpVtbl -> Shutdown(This) ) 


#define IMFFinalizableMediaSink_BeginFinalize(This,pCallback,punkState)	\
    ( (This)->lpVtbl -> BeginFinalize(This,pCallback,punkState) ) 

#define IMFFinalizableMediaSink_EndFinalize(This,pResult)	\
    ( (This)->lpVtbl -> EndFinalize(This,pResult) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFFinalizableMediaSink_INTERFACE_DEFINED__ */


#ifndef __IMFRemoteProxy_INTERFACE_DEFINED__
#define __IMFRemoteProxy_INTERFACE_DEFINED__

/* interface IMFRemoteProxy */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFRemoteProxy;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("994e23ad-1cc2-493c-b9fa-46f1cb040fa4")
    IMFRemoteProxy : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetRemoteObject( 
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __out  void **ppv) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRemoteHost( 
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __out  void **ppv) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFRemoteProxyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFRemoteProxy * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFRemoteProxy * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFRemoteProxy * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetRemoteObject )( 
            IMFRemoteProxy * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __out  void **ppv);
        
        HRESULT ( STDMETHODCALLTYPE *GetRemoteHost )( 
            IMFRemoteProxy * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __out  void **ppv);
        
        END_INTERFACE
    } IMFRemoteProxyVtbl;

    interface IMFRemoteProxy
    {
        CONST_VTBL struct IMFRemoteProxyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFRemoteProxy_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFRemoteProxy_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFRemoteProxy_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFRemoteProxy_GetRemoteObject(This,riid,ppv)	\
    ( (This)->lpVtbl -> GetRemoteObject(This,riid,ppv) ) 

#define IMFRemoteProxy_GetRemoteHost(This,riid,ppv)	\
    ( (This)->lpVtbl -> GetRemoteHost(This,riid,ppv) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFRemoteProxy_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0061 */
/* [local] */ 

EXTERN_GUID( MF_REMOTE_PROXY, 0x2f00c90e, 0xd2cf, 0x4278, 0x8b, 0x6a, 0xd0, 0x77, 0xfa, 0xc3, 0xa2, 0x5f);


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0061_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0061_v0_0_s_ifspec;

#ifndef __IMFObjectReferenceStream_INTERFACE_DEFINED__
#define __IMFObjectReferenceStream_INTERFACE_DEFINED__

/* interface IMFObjectReferenceStream */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFObjectReferenceStream;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("09EF5BE3-C8A7-469e-8B70-73BF25BB193F")
    IMFObjectReferenceStream : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SaveReference( 
            /* [in] */ REFIID riid,
            /* [in] */ IUnknown *pUnk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE LoadReference( 
            /* [in] */ REFIID riid,
            /* [out] */ void **ppv) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFObjectReferenceStreamVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFObjectReferenceStream * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFObjectReferenceStream * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFObjectReferenceStream * This);
        
        HRESULT ( STDMETHODCALLTYPE *SaveReference )( 
            IMFObjectReferenceStream * This,
            /* [in] */ REFIID riid,
            /* [in] */ IUnknown *pUnk);
        
        HRESULT ( STDMETHODCALLTYPE *LoadReference )( 
            IMFObjectReferenceStream * This,
            /* [in] */ REFIID riid,
            /* [out] */ void **ppv);
        
        END_INTERFACE
    } IMFObjectReferenceStreamVtbl;

    interface IMFObjectReferenceStream
    {
        CONST_VTBL struct IMFObjectReferenceStreamVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFObjectReferenceStream_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFObjectReferenceStream_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFObjectReferenceStream_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFObjectReferenceStream_SaveReference(This,riid,pUnk)	\
    ( (This)->lpVtbl -> SaveReference(This,riid,pUnk) ) 

#define IMFObjectReferenceStream_LoadReference(This,riid,ppv)	\
    ( (This)->lpVtbl -> LoadReference(This,riid,ppv) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFObjectReferenceStream_INTERFACE_DEFINED__ */


#ifndef __IMFPMPHost_INTERFACE_DEFINED__
#define __IMFPMPHost_INTERFACE_DEFINED__

/* interface IMFPMPHost */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFPMPHost;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F70CA1A9-FDC7-4782-B994-ADFFB1C98606")
    IMFPMPHost : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE LockProcess( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnlockProcess( void) = 0;
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE CreateObjectByCLSID( 
            /* [in] */ REFCLSID clsid,
            /* [unique][in] */ IStream *pStream,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppv) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFPMPHostVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFPMPHost * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFPMPHost * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFPMPHost * This);
        
        HRESULT ( STDMETHODCALLTYPE *LockProcess )( 
            IMFPMPHost * This);
        
        HRESULT ( STDMETHODCALLTYPE *UnlockProcess )( 
            IMFPMPHost * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *CreateObjectByCLSID )( 
            IMFPMPHost * This,
            /* [in] */ REFCLSID clsid,
            /* [unique][in] */ IStream *pStream,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppv);
        
        END_INTERFACE
    } IMFPMPHostVtbl;

    interface IMFPMPHost
    {
        CONST_VTBL struct IMFPMPHostVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFPMPHost_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFPMPHost_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFPMPHost_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFPMPHost_LockProcess(This)	\
    ( (This)->lpVtbl -> LockProcess(This) ) 

#define IMFPMPHost_UnlockProcess(This)	\
    ( (This)->lpVtbl -> UnlockProcess(This) ) 

#define IMFPMPHost_CreateObjectByCLSID(This,clsid,pStream,riid,ppv)	\
    ( (This)->lpVtbl -> CreateObjectByCLSID(This,clsid,pStream,riid,ppv) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFPMPHost_RemoteCreateObjectByCLSID_Proxy( 
    IMFPMPHost * This,
    /* [in] */ __RPC__in REFCLSID clsid,
    /* [size_is][unique][in] */ __RPC__in_ecount_full_opt(cbData) BYTE *pbData,
    /* [in] */ DWORD cbData,
    /* [in] */ __RPC__in REFIID riid,
    /* [iid_is][out] */ __RPC__deref_out_opt void **ppv);


void __RPC_STUB IMFPMPHost_RemoteCreateObjectByCLSID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMFPMPHost_INTERFACE_DEFINED__ */


#ifndef __IMFPMPClient_INTERFACE_DEFINED__
#define __IMFPMPClient_INTERFACE_DEFINED__

/* interface IMFPMPClient */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFPMPClient;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6C4E655D-EAD8-4421-B6B9-54DCDBBDF820")
    IMFPMPClient : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetPMPHost( 
            /* [in] */ IMFPMPHost *pPMPHost) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFPMPClientVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFPMPClient * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFPMPClient * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFPMPClient * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetPMPHost )( 
            IMFPMPClient * This,
            /* [in] */ IMFPMPHost *pPMPHost);
        
        END_INTERFACE
    } IMFPMPClientVtbl;

    interface IMFPMPClient
    {
        CONST_VTBL struct IMFPMPClientVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFPMPClient_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFPMPClient_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFPMPClient_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFPMPClient_SetPMPHost(This,pPMPHost)	\
    ( (This)->lpVtbl -> SetPMPHost(This,pPMPHost) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFPMPClient_INTERFACE_DEFINED__ */


#ifndef __IMFPMPServer_INTERFACE_DEFINED__
#define __IMFPMPServer_INTERFACE_DEFINED__

/* interface IMFPMPServer */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMFPMPServer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("994e23af-1cc2-493c-b9fa-46f1cb040fa4")
    IMFPMPServer : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE LockProcess( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnlockProcess( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateObjectByCLSID( 
            /* [in] */ __RPC__in REFCLSID clsid,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ __RPC__deref_out_opt void **ppObject) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFPMPServerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFPMPServer * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFPMPServer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFPMPServer * This);
        
        HRESULT ( STDMETHODCALLTYPE *LockProcess )( 
            IMFPMPServer * This);
        
        HRESULT ( STDMETHODCALLTYPE *UnlockProcess )( 
            IMFPMPServer * This);
        
        HRESULT ( STDMETHODCALLTYPE *CreateObjectByCLSID )( 
            IMFPMPServer * This,
            /* [in] */ __RPC__in REFCLSID clsid,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ __RPC__deref_out_opt void **ppObject);
        
        END_INTERFACE
    } IMFPMPServerVtbl;

    interface IMFPMPServer
    {
        CONST_VTBL struct IMFPMPServerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFPMPServer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFPMPServer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFPMPServer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFPMPServer_LockProcess(This)	\
    ( (This)->lpVtbl -> LockProcess(This) ) 

#define IMFPMPServer_UnlockProcess(This)	\
    ( (This)->lpVtbl -> UnlockProcess(This) ) 

#define IMFPMPServer_CreateObjectByCLSID(This,clsid,riid,ppObject)	\
    ( (This)->lpVtbl -> CreateObjectByCLSID(This,clsid,riid,ppObject) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFPMPServer_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0065 */
/* [local] */ 

STDAPI MFCreatePMPServer(
    DWORD dwCreationFlags,
    IMFPMPServer** ppPMPServer
    );


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0065_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0065_v0_0_s_ifspec;

#ifndef __IMFRemoteDesktopPlugin_INTERFACE_DEFINED__
#define __IMFRemoteDesktopPlugin_INTERFACE_DEFINED__

/* interface IMFRemoteDesktopPlugin */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFRemoteDesktopPlugin;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1cde6309-cae0-4940-907e-c1ec9c3d1d4a")
    IMFRemoteDesktopPlugin : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE UpdateTopology( 
            /* [out][in] */ IMFTopology *pTopology) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFRemoteDesktopPluginVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFRemoteDesktopPlugin * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFRemoteDesktopPlugin * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFRemoteDesktopPlugin * This);
        
        HRESULT ( STDMETHODCALLTYPE *UpdateTopology )( 
            IMFRemoteDesktopPlugin * This,
            /* [out][in] */ IMFTopology *pTopology);
        
        END_INTERFACE
    } IMFRemoteDesktopPluginVtbl;

    interface IMFRemoteDesktopPlugin
    {
        CONST_VTBL struct IMFRemoteDesktopPluginVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFRemoteDesktopPlugin_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFRemoteDesktopPlugin_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFRemoteDesktopPlugin_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFRemoteDesktopPlugin_UpdateTopology(This,pTopology)	\
    ( (This)->lpVtbl -> UpdateTopology(This,pTopology) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFRemoteDesktopPlugin_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0066 */
/* [local] */ 

STDAPI MFCreateRemoteDesktopPlugin(
    __out IMFRemoteDesktopPlugin** ppPlugin );
EXTERN_C HRESULT STDAPICALLTYPE CreateNamedPropertyStore(
        __out INamedPropertyStore **ppStore
        );



extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0066_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0066_v0_0_s_ifspec;

#ifndef __IMFSAMIStyle_INTERFACE_DEFINED__
#define __IMFSAMIStyle_INTERFACE_DEFINED__

/* interface IMFSAMIStyle */
/* [local][uuid][object] */ 


EXTERN_C const IID IID_IMFSAMIStyle;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A7E025DD-5303-4a62-89D6-E747E1EFAC73")
    IMFSAMIStyle : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetStyleCount( 
            /* [out] */ 
            __out  DWORD *pdwCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStyles( 
            /* [out] */ 
            __out  PROPVARIANT *pPropVarStyleArray) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSelectedStyle( 
            /* [in] */ 
            __in  LPCWSTR pwszStyle) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSelectedStyle( 
            /* [out] */ 
            __deref_out  LPWSTR *ppwszStyle) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMFSAMIStyleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMFSAMIStyle * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMFSAMIStyle * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMFSAMIStyle * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetStyleCount )( 
            IMFSAMIStyle * This,
            /* [out] */ 
            __out  DWORD *pdwCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetStyles )( 
            IMFSAMIStyle * This,
            /* [out] */ 
            __out  PROPVARIANT *pPropVarStyleArray);
        
        HRESULT ( STDMETHODCALLTYPE *SetSelectedStyle )( 
            IMFSAMIStyle * This,
            /* [in] */ 
            __in  LPCWSTR pwszStyle);
        
        HRESULT ( STDMETHODCALLTYPE *GetSelectedStyle )( 
            IMFSAMIStyle * This,
            /* [out] */ 
            __deref_out  LPWSTR *ppwszStyle);
        
        END_INTERFACE
    } IMFSAMIStyleVtbl;

    interface IMFSAMIStyle
    {
        CONST_VTBL struct IMFSAMIStyleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMFSAMIStyle_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMFSAMIStyle_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMFSAMIStyle_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMFSAMIStyle_GetStyleCount(This,pdwCount)	\
    ( (This)->lpVtbl -> GetStyleCount(This,pdwCount) ) 

#define IMFSAMIStyle_GetStyles(This,pPropVarStyleArray)	\
    ( (This)->lpVtbl -> GetStyles(This,pPropVarStyleArray) ) 

#define IMFSAMIStyle_SetSelectedStyle(This,pwszStyle)	\
    ( (This)->lpVtbl -> SetSelectedStyle(This,pwszStyle) ) 

#define IMFSAMIStyle_GetSelectedStyle(This,ppwszStyle)	\
    ( (This)->lpVtbl -> GetSelectedStyle(This,ppwszStyle) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMFSAMIStyle_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mfidl_0000_0067 */
/* [local] */ 

EXTERN_GUID( MF_SAMI_SERVICE, 0x49a89ae7, 0xb4d9, 0x4ef2, 0xaa, 0x5c, 0xf6, 0x5a, 0x3e, 0x5, 0xae, 0x4e );
EXTERN_GUID( MF_PD_SAMI_STYLELIST, 0xe0b73c7f, 0x486d, 0x484e, 0x98, 0x72, 0x4d, 0xe5, 0x19, 0x2a, 0x7b, 0xf8 );
EXTERN_GUID( MF_SD_SAMI_LANGUAGE, 0x36fcb98a, 0x6cd0, 0x44cb, 0xac, 0xb9, 0xa8, 0xf5, 0x60, 0xd, 0xd0, 0xbb );


extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0067_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mfidl_0000_0067_v0_0_s_ifspec;

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

/* [local] */ HRESULT STDMETHODCALLTYPE IMFSourceResolver_BeginCreateObjectFromURL_Proxy( 
    IMFSourceResolver * This,
    /* [in] */ LPCWSTR pwszURL,
    /* [in] */ DWORD dwFlags,
    /* [in] */ IPropertyStore *pProps,
    /* [out] */ 
    __out_opt  IUnknown **ppIUnknownCancelCookie,
    /* [in] */ IMFAsyncCallback *pCallback,
    /* [in] */ IUnknown *punkState);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFSourceResolver_BeginCreateObjectFromURL_Stub( 
    IMFSourceResolver * This,
    /* [string][in] */ __RPC__in LPCWSTR pwszURL,
    /* [in] */ DWORD dwFlags,
    /* [in] */ __RPC__in_opt IMFRemoteAsyncCallback *pCallback);

/* [local] */ HRESULT STDMETHODCALLTYPE IMFSourceResolver_EndCreateObjectFromURL_Proxy( 
    IMFSourceResolver * This,
    /* [in] */ IMFAsyncResult *pResult,
    /* [out] */ 
    __out  MF_OBJECT_TYPE *pObjectType,
    /* [out] */ 
    __out  IUnknown **ppObject);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFSourceResolver_EndCreateObjectFromURL_Stub( 
    IMFSourceResolver * This,
    /* [in] */ __RPC__in_opt IUnknown *pResult,
    /* [out] */ __RPC__out MF_OBJECT_TYPE *pObjectType,
    /* [out] */ __RPC__deref_out_opt IUnknown **ppObject);

/* [local] */ HRESULT STDMETHODCALLTYPE IMFSourceResolver_BeginCreateObjectFromByteStream_Proxy( 
    IMFSourceResolver * This,
    /* [in] */ IMFByteStream *pByteStream,
    /* [in] */ LPCWSTR pwszURL,
    /* [in] */ DWORD dwFlags,
    /* [in] */ IPropertyStore *pProps,
    /* [out] */ 
    __out_opt  IUnknown **ppIUnknownCancelCookie,
    /* [in] */ IMFAsyncCallback *pCallback,
    /* [in] */ IUnknown *punkState);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFSourceResolver_BeginCreateObjectFromByteStream_Stub( 
    IMFSourceResolver * This,
    /* [in] */ __RPC__in_opt IMFByteStream *pByteStream,
    /* [in] */ __RPC__in LPCWSTR pwszURL,
    /* [in] */ DWORD dwFlags,
    /* [in] */ __RPC__in_opt IMFRemoteAsyncCallback *pCallback);

/* [local] */ HRESULT STDMETHODCALLTYPE IMFSourceResolver_EndCreateObjectFromByteStream_Proxy( 
    IMFSourceResolver * This,
    /* [in] */ IMFAsyncResult *pResult,
    /* [out] */ 
    __out  MF_OBJECT_TYPE *pObjectType,
    /* [out] */ 
    __out  IUnknown **ppObject);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFSourceResolver_EndCreateObjectFromByteStream_Stub( 
    IMFSourceResolver * This,
    /* [in] */ __RPC__in_opt IUnknown *pResult,
    /* [out] */ __RPC__out MF_OBJECT_TYPE *pObjectType,
    /* [out] */ __RPC__deref_out_opt IUnknown **ppObject);

/* [local] */ HRESULT STDMETHODCALLTYPE IMFMediaSource_CreatePresentationDescriptor_Proxy( 
    IMFMediaSource * This,
    /* [out] */ 
    __out  IMFPresentationDescriptor **ppPresentationDescriptor);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFMediaSource_CreatePresentationDescriptor_Stub( 
    IMFMediaSource * This,
    /* [out] */ __RPC__out DWORD *pcbPD,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbPD) BYTE **pbPD,
    /* [out] */ __RPC__deref_out_opt IMFPresentationDescriptor **ppRemotePD);

/* [local] */ HRESULT STDMETHODCALLTYPE IMFMediaStream_RequestSample_Proxy( 
    IMFMediaStream * This,
    /* [in] */ IUnknown *pToken);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFMediaStream_RequestSample_Stub( 
    IMFMediaStream * This);

/* [local] */ HRESULT STDMETHODCALLTYPE IMFTopologyNode_GetOutputPrefType_Proxy( 
    IMFTopologyNode * This,
    /* [in] */ DWORD dwOutputIndex,
    /* [out] */ 
    __out  IMFMediaType **ppType);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFTopologyNode_GetOutputPrefType_Stub( 
    IMFTopologyNode * This,
    /* [in] */ DWORD dwOutputIndex,
    /* [out] */ __RPC__out DWORD *pcbData,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbData) BYTE **ppbData);

/* [local] */ HRESULT STDMETHODCALLTYPE IMFTopologyNode_GetInputPrefType_Proxy( 
    IMFTopologyNode * This,
    /* [in] */ DWORD dwInputIndex,
    /* [out] */ 
    __out  IMFMediaType **ppType);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFTopologyNode_GetInputPrefType_Stub( 
    IMFTopologyNode * This,
    /* [in] */ DWORD dwInputIndex,
    /* [out] */ __RPC__out DWORD *pcbData,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbData) BYTE **ppbData);

/* [local] */ HRESULT STDMETHODCALLTYPE IMFMediaTypeHandler_GetCurrentMediaType_Proxy( 
    IMFMediaTypeHandler * This,
    /* [out] */ 
    __out  IMFMediaType **ppMediaType);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFMediaTypeHandler_GetCurrentMediaType_Stub( 
    IMFMediaTypeHandler * This,
    /* [size_is][size_is][out] */ 
    __out  BYTE **ppbData,
    /* [out] */ __RPC__out DWORD *pcbData);

/* [local] */ HRESULT STDMETHODCALLTYPE IMFContentProtectionManager_BeginEnableContent_Proxy( 
    IMFContentProtectionManager * This,
    /* [in] */ IMFActivate *pEnablerActivate,
    /* [in] */ IMFTopology *pTopo,
    /* [in] */ IMFAsyncCallback *pCallback,
    /* [in] */ IUnknown *punkState);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFContentProtectionManager_BeginEnableContent_Stub( 
    IMFContentProtectionManager * This,
    /* [in] */ __RPC__in REFCLSID clsidType,
    /* [size_is][in] */ __RPC__in_ecount_full(cbData) BYTE *pbData,
    /* [in] */ DWORD cbData,
    /* [in] */ __RPC__in_opt IMFRemoteAsyncCallback *pCallback);

/* [local] */ HRESULT STDMETHODCALLTYPE IMFContentProtectionManager_EndEnableContent_Proxy( 
    IMFContentProtectionManager * This,
    /* [in] */ IMFAsyncResult *pResult);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFContentProtectionManager_EndEnableContent_Stub( 
    IMFContentProtectionManager * This,
    /* [in] */ __RPC__in_opt IUnknown *pResult);

/* [local] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_BeginRegisterTopologyWorkQueuesWithMMCSS_Proxy( 
    IMFWorkQueueServices * This,
    /* [in] */ IMFAsyncCallback *pCallback,
    /* [in] */ IUnknown *pState);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_BeginRegisterTopologyWorkQueuesWithMMCSS_Stub( 
    IMFWorkQueueServices * This,
    /* [in] */ __RPC__in_opt IMFRemoteAsyncCallback *pCallback);

/* [local] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_EndRegisterTopologyWorkQueuesWithMMCSS_Proxy( 
    IMFWorkQueueServices * This,
    /* [in] */ IMFAsyncResult *pResult);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_EndRegisterTopologyWorkQueuesWithMMCSS_Stub( 
    IMFWorkQueueServices * This,
    /* [in] */ __RPC__in_opt IUnknown *pResult);

/* [local] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_BeginUnregisterTopologyWorkQueuesWithMMCSS_Proxy( 
    IMFWorkQueueServices * This,
    /* [in] */ IMFAsyncCallback *pCallback,
    /* [in] */ IUnknown *pState);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_BeginUnregisterTopologyWorkQueuesWithMMCSS_Stub( 
    IMFWorkQueueServices * This,
    /* [in] */ __RPC__in_opt IMFRemoteAsyncCallback *pCallback);

/* [local] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_EndUnregisterTopologyWorkQueuesWithMMCSS_Proxy( 
    IMFWorkQueueServices * This,
    /* [in] */ IMFAsyncResult *pResult);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_EndUnregisterTopologyWorkQueuesWithMMCSS_Stub( 
    IMFWorkQueueServices * This,
    /* [in] */ __RPC__in_opt IUnknown *pResult);

/* [local] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_BeginRegisterPlatformWorkQueueWithMMCSS_Proxy( 
    IMFWorkQueueServices * This,
    /* [in] */ DWORD dwPlatformWorkQueue,
    /* [in] */ LPCWSTR wszClass,
    /* [in] */ DWORD dwTaskId,
    /* [in] */ IMFAsyncCallback *pCallback,
    /* [in] */ IUnknown *pState);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_BeginRegisterPlatformWorkQueueWithMMCSS_Stub( 
    IMFWorkQueueServices * This,
    /* [in] */ DWORD dwPlatformWorkQueue,
    /* [in] */ __RPC__in LPCWSTR wszClass,
    /* [in] */ DWORD dwTaskId,
    /* [in] */ __RPC__in_opt IMFRemoteAsyncCallback *pCallback);

/* [local] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_EndRegisterPlatformWorkQueueWithMMCSS_Proxy( 
    IMFWorkQueueServices * This,
    /* [in] */ IMFAsyncResult *pResult,
    /* [out] */ 
    __out  DWORD *pdwTaskId);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_EndRegisterPlatformWorkQueueWithMMCSS_Stub( 
    IMFWorkQueueServices * This,
    /* [in] */ __RPC__in_opt IUnknown *pResult,
    /* [out] */ __RPC__out DWORD *pdwTaskId);

/* [local] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_BeginUnregisterPlatformWorkQueueWithMMCSS_Proxy( 
    IMFWorkQueueServices * This,
    /* [in] */ DWORD dwPlatformWorkQueue,
    /* [in] */ IMFAsyncCallback *pCallback,
    /* [in] */ IUnknown *pState);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_BeginUnregisterPlatformWorkQueueWithMMCSS_Stub( 
    IMFWorkQueueServices * This,
    /* [in] */ DWORD dwPlatformWorkQueue,
    /* [in] */ __RPC__in_opt IMFRemoteAsyncCallback *pCallback);

/* [local] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_EndUnregisterPlatformWorkQueueWithMMCSS_Proxy( 
    IMFWorkQueueServices * This,
    /* [in] */ IMFAsyncResult *pResult);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFWorkQueueServices_EndUnregisterPlatformWorkQueueWithMMCSS_Stub( 
    IMFWorkQueueServices * This,
    /* [in] */ __RPC__in_opt IUnknown *pResult);

/* [local] */ HRESULT STDMETHODCALLTYPE IMFPMPHost_CreateObjectByCLSID_Proxy( 
    IMFPMPHost * This,
    /* [in] */ REFCLSID clsid,
    /* [unique][in] */ IStream *pStream,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ void **ppv);


/* [call_as] */ HRESULT STDMETHODCALLTYPE IMFPMPHost_CreateObjectByCLSID_Stub( 
    IMFPMPHost * This,
    /* [in] */ __RPC__in REFCLSID clsid,
    /* [size_is][unique][in] */ __RPC__in_ecount_full_opt(cbData) BYTE *pbData,
    /* [in] */ DWORD cbData,
    /* [in] */ __RPC__in REFIID riid,
    /* [iid_is][out] */ __RPC__deref_out_opt void **ppv);



/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


