

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0498 */
/* Compiler settings for mediaobj.idl:
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

#ifndef __mediaobj_h__
#define __mediaobj_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMediaBuffer_FWD_DEFINED__
#define __IMediaBuffer_FWD_DEFINED__
typedef interface IMediaBuffer IMediaBuffer;
#endif 	/* __IMediaBuffer_FWD_DEFINED__ */


#ifndef __IMediaObject_FWD_DEFINED__
#define __IMediaObject_FWD_DEFINED__
typedef interface IMediaObject IMediaObject;
#endif 	/* __IMediaObject_FWD_DEFINED__ */


#ifndef __IEnumDMO_FWD_DEFINED__
#define __IEnumDMO_FWD_DEFINED__
typedef interface IEnumDMO IEnumDMO;
#endif 	/* __IEnumDMO_FWD_DEFINED__ */


#ifndef __IMediaObjectInPlace_FWD_DEFINED__
#define __IMediaObjectInPlace_FWD_DEFINED__
typedef interface IMediaObjectInPlace IMediaObjectInPlace;
#endif 	/* __IMediaObjectInPlace_FWD_DEFINED__ */


#ifndef __IDMOQualityControl_FWD_DEFINED__
#define __IDMOQualityControl_FWD_DEFINED__
typedef interface IDMOQualityControl IDMOQualityControl;
#endif 	/* __IDMOQualityControl_FWD_DEFINED__ */


#ifndef __IDMOVideoOutputOptimizations_FWD_DEFINED__
#define __IDMOVideoOutputOptimizations_FWD_DEFINED__
typedef interface IDMOVideoOutputOptimizations IDMOVideoOutputOptimizations;
#endif 	/* __IDMOVideoOutputOptimizations_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"
#include "objidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_mediaobj_0000_0000 */
/* [local] */ 

#ifdef __strmif_h__
typedef AM_MEDIA_TYPE DMO_MEDIA_TYPE;
#else
typedef struct _DMOMediaType
    {
    GUID majortype;
    GUID subtype;
    BOOL bFixedSizeSamples;
    BOOL bTemporalCompression;
    ULONG lSampleSize;
    GUID formattype;
    IUnknown *pUnk;
    ULONG cbFormat;
    BYTE *pbFormat;
    } 	DMO_MEDIA_TYPE;

typedef LONGLONG REFERENCE_TIME;

#endif

enum _DMO_INPUT_DATA_BUFFER_FLAGS
    {	DMO_INPUT_DATA_BUFFERF_SYNCPOINT	= 0x1,
	DMO_INPUT_DATA_BUFFERF_TIME	= 0x2,
	DMO_INPUT_DATA_BUFFERF_TIMELENGTH	= 0x4
    } ;

enum _DMO_OUTPUT_DATA_BUFFER_FLAGS
    {	DMO_OUTPUT_DATA_BUFFERF_SYNCPOINT	= 0x1,
	DMO_OUTPUT_DATA_BUFFERF_TIME	= 0x2,
	DMO_OUTPUT_DATA_BUFFERF_TIMELENGTH	= 0x4,
	DMO_OUTPUT_DATA_BUFFERF_INCOMPLETE	= 0x1000000
    } ;

enum _DMO_INPUT_STATUS_FLAGS
    {	DMO_INPUT_STATUSF_ACCEPT_DATA	= 0x1
    } ;

enum _DMO_INPUT_STREAM_INFO_FLAGS
    {	DMO_INPUT_STREAMF_WHOLE_SAMPLES	= 0x1,
	DMO_INPUT_STREAMF_SINGLE_SAMPLE_PER_BUFFER	= 0x2,
	DMO_INPUT_STREAMF_FIXED_SAMPLE_SIZE	= 0x4,
	DMO_INPUT_STREAMF_HOLDS_BUFFERS	= 0x8
    } ;

enum _DMO_OUTPUT_STREAM_INFO_FLAGS
    {	DMO_OUTPUT_STREAMF_WHOLE_SAMPLES	= 0x1,
	DMO_OUTPUT_STREAMF_SINGLE_SAMPLE_PER_BUFFER	= 0x2,
	DMO_OUTPUT_STREAMF_FIXED_SAMPLE_SIZE	= 0x4,
	DMO_OUTPUT_STREAMF_DISCARDABLE	= 0x8,
	DMO_OUTPUT_STREAMF_OPTIONAL	= 0x10
    } ;

enum _DMO_SET_TYPE_FLAGS
    {	DMO_SET_TYPEF_TEST_ONLY	= 0x1,
	DMO_SET_TYPEF_CLEAR	= 0x2
    } ;

enum _DMO_PROCESS_OUTPUT_FLAGS
    {	DMO_PROCESS_OUTPUT_DISCARD_WHEN_NO_BUFFER	= 0x1
    } ;


extern RPC_IF_HANDLE __MIDL_itf_mediaobj_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mediaobj_0000_0000_v0_0_s_ifspec;

#ifndef __IMediaBuffer_INTERFACE_DEFINED__
#define __IMediaBuffer_INTERFACE_DEFINED__

/* interface IMediaBuffer */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMediaBuffer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("59eff8b9-938c-4a26-82f2-95cb84cdc837")
    IMediaBuffer : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetLength( 
            DWORD cbLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMaxLength( 
            /* [out] */ __RPC__out DWORD *pcbMaxLength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBufferAndLength( 
            /* [out] */ __RPC__deref_out_opt BYTE **ppBuffer,
            /* [out] */ __RPC__out DWORD *pcbLength) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMediaBufferVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMediaBuffer * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMediaBuffer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMediaBuffer * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetLength )( 
            IMediaBuffer * This,
            DWORD cbLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetMaxLength )( 
            IMediaBuffer * This,
            /* [out] */ __RPC__out DWORD *pcbMaxLength);
        
        HRESULT ( STDMETHODCALLTYPE *GetBufferAndLength )( 
            IMediaBuffer * This,
            /* [out] */ __RPC__deref_out_opt BYTE **ppBuffer,
            /* [out] */ __RPC__out DWORD *pcbLength);
        
        END_INTERFACE
    } IMediaBufferVtbl;

    interface IMediaBuffer
    {
        CONST_VTBL struct IMediaBufferVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMediaBuffer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMediaBuffer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMediaBuffer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMediaBuffer_SetLength(This,cbLength)	\
    ( (This)->lpVtbl -> SetLength(This,cbLength) ) 

#define IMediaBuffer_GetMaxLength(This,pcbMaxLength)	\
    ( (This)->lpVtbl -> GetMaxLength(This,pcbMaxLength) ) 

#define IMediaBuffer_GetBufferAndLength(This,ppBuffer,pcbLength)	\
    ( (This)->lpVtbl -> GetBufferAndLength(This,ppBuffer,pcbLength) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMediaBuffer_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mediaobj_0000_0001 */
/* [local] */ 

typedef struct _DMO_OUTPUT_DATA_BUFFER
    {
    IMediaBuffer *pBuffer;
    DWORD dwStatus;
    REFERENCE_TIME rtTimestamp;
    REFERENCE_TIME rtTimelength;
    } 	DMO_OUTPUT_DATA_BUFFER;

typedef struct _DMO_OUTPUT_DATA_BUFFER *PDMO_OUTPUT_DATA_BUFFER;



extern RPC_IF_HANDLE __MIDL_itf_mediaobj_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mediaobj_0000_0001_v0_0_s_ifspec;

#ifndef __IMediaObject_INTERFACE_DEFINED__
#define __IMediaObject_INTERFACE_DEFINED__

/* interface IMediaObject */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMediaObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("d8ad0f58-5494-4102-97c5-ec798e59bcf4")
    IMediaObject : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetStreamCount( 
            /* [out] */ __RPC__out DWORD *pcInputStreams,
            /* [out] */ __RPC__out DWORD *pcOutputStreams) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetInputStreamInfo( 
            DWORD dwInputStreamIndex,
            /* [out] */ __RPC__out DWORD *pdwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetOutputStreamInfo( 
            DWORD dwOutputStreamIndex,
            /* [out] */ __RPC__out DWORD *pdwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetInputType( 
            DWORD dwInputStreamIndex,
            DWORD dwTypeIndex,
            /* [out] */ __RPC__out DMO_MEDIA_TYPE *pmt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetOutputType( 
            DWORD dwOutputStreamIndex,
            DWORD dwTypeIndex,
            /* [out] */ __RPC__out DMO_MEDIA_TYPE *pmt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetInputType( 
            DWORD dwInputStreamIndex,
            /* [in] */ __RPC__in const DMO_MEDIA_TYPE *pmt,
            DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetOutputType( 
            DWORD dwOutputStreamIndex,
            /* [in] */ __RPC__in const DMO_MEDIA_TYPE *pmt,
            DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetInputCurrentType( 
            DWORD dwInputStreamIndex,
            /* [out] */ __RPC__out DMO_MEDIA_TYPE *pmt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetOutputCurrentType( 
            DWORD dwOutputStreamIndex,
            /* [out] */ __RPC__out DMO_MEDIA_TYPE *pmt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetInputSizeInfo( 
            DWORD dwInputStreamIndex,
            /* [out] */ __RPC__out DWORD *pcbSize,
            /* [out] */ __RPC__out DWORD *pcbMaxLookahead,
            /* [out] */ __RPC__out DWORD *pcbAlignment) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetOutputSizeInfo( 
            DWORD dwOutputStreamIndex,
            /* [out] */ __RPC__out DWORD *pcbSize,
            /* [out] */ __RPC__out DWORD *pcbAlignment) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetInputMaxLatency( 
            DWORD dwInputStreamIndex,
            /* [out] */ __RPC__out REFERENCE_TIME *prtMaxLatency) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetInputMaxLatency( 
            DWORD dwInputStreamIndex,
            REFERENCE_TIME rtMaxLatency) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Flush( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Discontinuity( 
            DWORD dwInputStreamIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AllocateStreamingResources( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FreeStreamingResources( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetInputStatus( 
            DWORD dwInputStreamIndex,
            /* [out] */ __RPC__out DWORD *dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ProcessInput( 
            DWORD dwInputStreamIndex,
            __RPC__in_opt IMediaBuffer *pBuffer,
            DWORD dwFlags,
            REFERENCE_TIME rtTimestamp,
            REFERENCE_TIME rtTimelength) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ProcessOutput( 
            DWORD dwFlags,
            DWORD cOutputBufferCount,
            /* [size_is][out][in] */ __RPC__inout_ecount_full(cOutputBufferCount) DMO_OUTPUT_DATA_BUFFER *pOutputBuffers,
            /* [out] */ __RPC__out DWORD *pdwStatus) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Lock( 
            LONG bLock) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMediaObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMediaObject * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMediaObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMediaObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetStreamCount )( 
            IMediaObject * This,
            /* [out] */ __RPC__out DWORD *pcInputStreams,
            /* [out] */ __RPC__out DWORD *pcOutputStreams);
        
        HRESULT ( STDMETHODCALLTYPE *GetInputStreamInfo )( 
            IMediaObject * This,
            DWORD dwInputStreamIndex,
            /* [out] */ __RPC__out DWORD *pdwFlags);
        
        HRESULT ( STDMETHODCALLTYPE *GetOutputStreamInfo )( 
            IMediaObject * This,
            DWORD dwOutputStreamIndex,
            /* [out] */ __RPC__out DWORD *pdwFlags);
        
        HRESULT ( STDMETHODCALLTYPE *GetInputType )( 
            IMediaObject * This,
            DWORD dwInputStreamIndex,
            DWORD dwTypeIndex,
            /* [out] */ __RPC__out DMO_MEDIA_TYPE *pmt);
        
        HRESULT ( STDMETHODCALLTYPE *GetOutputType )( 
            IMediaObject * This,
            DWORD dwOutputStreamIndex,
            DWORD dwTypeIndex,
            /* [out] */ __RPC__out DMO_MEDIA_TYPE *pmt);
        
        HRESULT ( STDMETHODCALLTYPE *SetInputType )( 
            IMediaObject * This,
            DWORD dwInputStreamIndex,
            /* [in] */ __RPC__in const DMO_MEDIA_TYPE *pmt,
            DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE *SetOutputType )( 
            IMediaObject * This,
            DWORD dwOutputStreamIndex,
            /* [in] */ __RPC__in const DMO_MEDIA_TYPE *pmt,
            DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE *GetInputCurrentType )( 
            IMediaObject * This,
            DWORD dwInputStreamIndex,
            /* [out] */ __RPC__out DMO_MEDIA_TYPE *pmt);
        
        HRESULT ( STDMETHODCALLTYPE *GetOutputCurrentType )( 
            IMediaObject * This,
            DWORD dwOutputStreamIndex,
            /* [out] */ __RPC__out DMO_MEDIA_TYPE *pmt);
        
        HRESULT ( STDMETHODCALLTYPE *GetInputSizeInfo )( 
            IMediaObject * This,
            DWORD dwInputStreamIndex,
            /* [out] */ __RPC__out DWORD *pcbSize,
            /* [out] */ __RPC__out DWORD *pcbMaxLookahead,
            /* [out] */ __RPC__out DWORD *pcbAlignment);
        
        HRESULT ( STDMETHODCALLTYPE *GetOutputSizeInfo )( 
            IMediaObject * This,
            DWORD dwOutputStreamIndex,
            /* [out] */ __RPC__out DWORD *pcbSize,
            /* [out] */ __RPC__out DWORD *pcbAlignment);
        
        HRESULT ( STDMETHODCALLTYPE *GetInputMaxLatency )( 
            IMediaObject * This,
            DWORD dwInputStreamIndex,
            /* [out] */ __RPC__out REFERENCE_TIME *prtMaxLatency);
        
        HRESULT ( STDMETHODCALLTYPE *SetInputMaxLatency )( 
            IMediaObject * This,
            DWORD dwInputStreamIndex,
            REFERENCE_TIME rtMaxLatency);
        
        HRESULT ( STDMETHODCALLTYPE *Flush )( 
            IMediaObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *Discontinuity )( 
            IMediaObject * This,
            DWORD dwInputStreamIndex);
        
        HRESULT ( STDMETHODCALLTYPE *AllocateStreamingResources )( 
            IMediaObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *FreeStreamingResources )( 
            IMediaObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetInputStatus )( 
            IMediaObject * This,
            DWORD dwInputStreamIndex,
            /* [out] */ __RPC__out DWORD *dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE *ProcessInput )( 
            IMediaObject * This,
            DWORD dwInputStreamIndex,
            __RPC__in_opt IMediaBuffer *pBuffer,
            DWORD dwFlags,
            REFERENCE_TIME rtTimestamp,
            REFERENCE_TIME rtTimelength);
        
        HRESULT ( STDMETHODCALLTYPE *ProcessOutput )( 
            IMediaObject * This,
            DWORD dwFlags,
            DWORD cOutputBufferCount,
            /* [size_is][out][in] */ __RPC__inout_ecount_full(cOutputBufferCount) DMO_OUTPUT_DATA_BUFFER *pOutputBuffers,
            /* [out] */ __RPC__out DWORD *pdwStatus);
        
        HRESULT ( STDMETHODCALLTYPE *Lock )( 
            IMediaObject * This,
            LONG bLock);
        
        END_INTERFACE
    } IMediaObjectVtbl;

    interface IMediaObject
    {
        CONST_VTBL struct IMediaObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMediaObject_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMediaObject_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMediaObject_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMediaObject_GetStreamCount(This,pcInputStreams,pcOutputStreams)	\
    ( (This)->lpVtbl -> GetStreamCount(This,pcInputStreams,pcOutputStreams) ) 

#define IMediaObject_GetInputStreamInfo(This,dwInputStreamIndex,pdwFlags)	\
    ( (This)->lpVtbl -> GetInputStreamInfo(This,dwInputStreamIndex,pdwFlags) ) 

#define IMediaObject_GetOutputStreamInfo(This,dwOutputStreamIndex,pdwFlags)	\
    ( (This)->lpVtbl -> GetOutputStreamInfo(This,dwOutputStreamIndex,pdwFlags) ) 

#define IMediaObject_GetInputType(This,dwInputStreamIndex,dwTypeIndex,pmt)	\
    ( (This)->lpVtbl -> GetInputType(This,dwInputStreamIndex,dwTypeIndex,pmt) ) 

#define IMediaObject_GetOutputType(This,dwOutputStreamIndex,dwTypeIndex,pmt)	\
    ( (This)->lpVtbl -> GetOutputType(This,dwOutputStreamIndex,dwTypeIndex,pmt) ) 

#define IMediaObject_SetInputType(This,dwInputStreamIndex,pmt,dwFlags)	\
    ( (This)->lpVtbl -> SetInputType(This,dwInputStreamIndex,pmt,dwFlags) ) 

#define IMediaObject_SetOutputType(This,dwOutputStreamIndex,pmt,dwFlags)	\
    ( (This)->lpVtbl -> SetOutputType(This,dwOutputStreamIndex,pmt,dwFlags) ) 

#define IMediaObject_GetInputCurrentType(This,dwInputStreamIndex,pmt)	\
    ( (This)->lpVtbl -> GetInputCurrentType(This,dwInputStreamIndex,pmt) ) 

#define IMediaObject_GetOutputCurrentType(This,dwOutputStreamIndex,pmt)	\
    ( (This)->lpVtbl -> GetOutputCurrentType(This,dwOutputStreamIndex,pmt) ) 

#define IMediaObject_GetInputSizeInfo(This,dwInputStreamIndex,pcbSize,pcbMaxLookahead,pcbAlignment)	\
    ( (This)->lpVtbl -> GetInputSizeInfo(This,dwInputStreamIndex,pcbSize,pcbMaxLookahead,pcbAlignment) ) 

#define IMediaObject_GetOutputSizeInfo(This,dwOutputStreamIndex,pcbSize,pcbAlignment)	\
    ( (This)->lpVtbl -> GetOutputSizeInfo(This,dwOutputStreamIndex,pcbSize,pcbAlignment) ) 

#define IMediaObject_GetInputMaxLatency(This,dwInputStreamIndex,prtMaxLatency)	\
    ( (This)->lpVtbl -> GetInputMaxLatency(This,dwInputStreamIndex,prtMaxLatency) ) 

#define IMediaObject_SetInputMaxLatency(This,dwInputStreamIndex,rtMaxLatency)	\
    ( (This)->lpVtbl -> SetInputMaxLatency(This,dwInputStreamIndex,rtMaxLatency) ) 

#define IMediaObject_Flush(This)	\
    ( (This)->lpVtbl -> Flush(This) ) 

#define IMediaObject_Discontinuity(This,dwInputStreamIndex)	\
    ( (This)->lpVtbl -> Discontinuity(This,dwInputStreamIndex) ) 

#define IMediaObject_AllocateStreamingResources(This)	\
    ( (This)->lpVtbl -> AllocateStreamingResources(This) ) 

#define IMediaObject_FreeStreamingResources(This)	\
    ( (This)->lpVtbl -> FreeStreamingResources(This) ) 

#define IMediaObject_GetInputStatus(This,dwInputStreamIndex,dwFlags)	\
    ( (This)->lpVtbl -> GetInputStatus(This,dwInputStreamIndex,dwFlags) ) 

#define IMediaObject_ProcessInput(This,dwInputStreamIndex,pBuffer,dwFlags,rtTimestamp,rtTimelength)	\
    ( (This)->lpVtbl -> ProcessInput(This,dwInputStreamIndex,pBuffer,dwFlags,rtTimestamp,rtTimelength) ) 

#define IMediaObject_ProcessOutput(This,dwFlags,cOutputBufferCount,pOutputBuffers,pdwStatus)	\
    ( (This)->lpVtbl -> ProcessOutput(This,dwFlags,cOutputBufferCount,pOutputBuffers,pdwStatus) ) 

#define IMediaObject_Lock(This,bLock)	\
    ( (This)->lpVtbl -> Lock(This,bLock) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMediaObject_INTERFACE_DEFINED__ */


#ifndef __IEnumDMO_INTERFACE_DEFINED__
#define __IEnumDMO_INTERFACE_DEFINED__

/* interface IEnumDMO */
/* [uuid][object] */ 


EXTERN_C const IID IID_IEnumDMO;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2c3cd98a-2bfa-4a53-9c27-5249ba64ba0f")
    IEnumDMO : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Next( 
            DWORD cItemsToFetch,
            /* [length_is][size_is][out] */ __RPC__out_ecount_part(cItemsToFetch, *pcItemsFetched) CLSID *pCLSID,
            /* [string][length_is][size_is][out] */ __RPC__out_ecount_part(cItemsToFetch, *pcItemsFetched) WCHAR **Names,
            /* [out] */ __RPC__out DWORD *pcItemsFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            DWORD cItemsToSkip) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ __RPC__deref_out_opt IEnumDMO **ppEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumDMOVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEnumDMO * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEnumDMO * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEnumDMO * This);
        
        HRESULT ( STDMETHODCALLTYPE *Next )( 
            IEnumDMO * This,
            DWORD cItemsToFetch,
            /* [length_is][size_is][out] */ __RPC__out_ecount_part(cItemsToFetch, *pcItemsFetched) CLSID *pCLSID,
            /* [string][length_is][size_is][out] */ __RPC__out_ecount_part(cItemsToFetch, *pcItemsFetched) WCHAR **Names,
            /* [out] */ __RPC__out DWORD *pcItemsFetched);
        
        HRESULT ( STDMETHODCALLTYPE *Skip )( 
            IEnumDMO * This,
            DWORD cItemsToSkip);
        
        HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IEnumDMO * This);
        
        HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IEnumDMO * This,
            /* [out] */ __RPC__deref_out_opt IEnumDMO **ppEnum);
        
        END_INTERFACE
    } IEnumDMOVtbl;

    interface IEnumDMO
    {
        CONST_VTBL struct IEnumDMOVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumDMO_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IEnumDMO_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IEnumDMO_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IEnumDMO_Next(This,cItemsToFetch,pCLSID,Names,pcItemsFetched)	\
    ( (This)->lpVtbl -> Next(This,cItemsToFetch,pCLSID,Names,pcItemsFetched) ) 

#define IEnumDMO_Skip(This,cItemsToSkip)	\
    ( (This)->lpVtbl -> Skip(This,cItemsToSkip) ) 

#define IEnumDMO_Reset(This)	\
    ( (This)->lpVtbl -> Reset(This) ) 

#define IEnumDMO_Clone(This,ppEnum)	\
    ( (This)->lpVtbl -> Clone(This,ppEnum) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IEnumDMO_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mediaobj_0000_0003 */
/* [local] */ 


enum _DMO_INPLACE_PROCESS_FLAGS
    {	DMO_INPLACE_NORMAL	= 0,
	DMO_INPLACE_ZERO	= 0x1
    } ;


extern RPC_IF_HANDLE __MIDL_itf_mediaobj_0000_0003_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mediaobj_0000_0003_v0_0_s_ifspec;

#ifndef __IMediaObjectInPlace_INTERFACE_DEFINED__
#define __IMediaObjectInPlace_INTERFACE_DEFINED__

/* interface IMediaObjectInPlace */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMediaObjectInPlace;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("651b9ad0-0fc7-4aa9-9538-d89931010741")
    IMediaObjectInPlace : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Process( 
            /* [in] */ ULONG ulSize,
            /* [size_is][out][in] */ __RPC__inout_ecount_full(ulSize) BYTE *pData,
            /* [in] */ REFERENCE_TIME refTimeStart,
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ __RPC__deref_out_opt IMediaObjectInPlace **ppMediaObject) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLatency( 
            /* [out] */ __RPC__out REFERENCE_TIME *pLatencyTime) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMediaObjectInPlaceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMediaObjectInPlace * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMediaObjectInPlace * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMediaObjectInPlace * This);
        
        HRESULT ( STDMETHODCALLTYPE *Process )( 
            IMediaObjectInPlace * This,
            /* [in] */ ULONG ulSize,
            /* [size_is][out][in] */ __RPC__inout_ecount_full(ulSize) BYTE *pData,
            /* [in] */ REFERENCE_TIME refTimeStart,
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IMediaObjectInPlace * This,
            /* [out] */ __RPC__deref_out_opt IMediaObjectInPlace **ppMediaObject);
        
        HRESULT ( STDMETHODCALLTYPE *GetLatency )( 
            IMediaObjectInPlace * This,
            /* [out] */ __RPC__out REFERENCE_TIME *pLatencyTime);
        
        END_INTERFACE
    } IMediaObjectInPlaceVtbl;

    interface IMediaObjectInPlace
    {
        CONST_VTBL struct IMediaObjectInPlaceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMediaObjectInPlace_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMediaObjectInPlace_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMediaObjectInPlace_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMediaObjectInPlace_Process(This,ulSize,pData,refTimeStart,dwFlags)	\
    ( (This)->lpVtbl -> Process(This,ulSize,pData,refTimeStart,dwFlags) ) 

#define IMediaObjectInPlace_Clone(This,ppMediaObject)	\
    ( (This)->lpVtbl -> Clone(This,ppMediaObject) ) 

#define IMediaObjectInPlace_GetLatency(This,pLatencyTime)	\
    ( (This)->lpVtbl -> GetLatency(This,pLatencyTime) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMediaObjectInPlace_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mediaobj_0000_0004 */
/* [local] */ 


enum _DMO_QUALITY_STATUS_FLAGS
    {	DMO_QUALITY_STATUS_ENABLED	= 0x1
    } ;


extern RPC_IF_HANDLE __MIDL_itf_mediaobj_0000_0004_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mediaobj_0000_0004_v0_0_s_ifspec;

#ifndef __IDMOQualityControl_INTERFACE_DEFINED__
#define __IDMOQualityControl_INTERFACE_DEFINED__

/* interface IDMOQualityControl */
/* [uuid][object] */ 


EXTERN_C const IID IID_IDMOQualityControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("65abea96-cf36-453f-af8a-705e98f16260")
    IDMOQualityControl : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetNow( 
            /* [in] */ REFERENCE_TIME rtNow) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetStatus( 
            /* [in] */ DWORD dwFlags) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStatus( 
            /* [out] */ __RPC__out DWORD *pdwFlags) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDMOQualityControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDMOQualityControl * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDMOQualityControl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDMOQualityControl * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetNow )( 
            IDMOQualityControl * This,
            /* [in] */ REFERENCE_TIME rtNow);
        
        HRESULT ( STDMETHODCALLTYPE *SetStatus )( 
            IDMOQualityControl * This,
            /* [in] */ DWORD dwFlags);
        
        HRESULT ( STDMETHODCALLTYPE *GetStatus )( 
            IDMOQualityControl * This,
            /* [out] */ __RPC__out DWORD *pdwFlags);
        
        END_INTERFACE
    } IDMOQualityControlVtbl;

    interface IDMOQualityControl
    {
        CONST_VTBL struct IDMOQualityControlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDMOQualityControl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDMOQualityControl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDMOQualityControl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDMOQualityControl_SetNow(This,rtNow)	\
    ( (This)->lpVtbl -> SetNow(This,rtNow) ) 

#define IDMOQualityControl_SetStatus(This,dwFlags)	\
    ( (This)->lpVtbl -> SetStatus(This,dwFlags) ) 

#define IDMOQualityControl_GetStatus(This,pdwFlags)	\
    ( (This)->lpVtbl -> GetStatus(This,pdwFlags) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDMOQualityControl_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_mediaobj_0000_0005 */
/* [local] */ 


enum _DMO_VIDEO_OUTPUT_STREAM_FLAGS
    {	DMO_VOSF_NEEDS_PREVIOUS_SAMPLE	= 0x1
    } ;


extern RPC_IF_HANDLE __MIDL_itf_mediaobj_0000_0005_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_mediaobj_0000_0005_v0_0_s_ifspec;

#ifndef __IDMOVideoOutputOptimizations_INTERFACE_DEFINED__
#define __IDMOVideoOutputOptimizations_INTERFACE_DEFINED__

/* interface IDMOVideoOutputOptimizations */
/* [uuid][object] */ 


EXTERN_C const IID IID_IDMOVideoOutputOptimizations;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("be8f4f4e-5b16-4d29-b350-7f6b5d9298ac")
    IDMOVideoOutputOptimizations : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE QueryOperationModePreferences( 
            ULONG ulOutputStreamIndex,
            __RPC__in DWORD *pdwRequestedCapabilities) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetOperationMode( 
            ULONG ulOutputStreamIndex,
            DWORD dwEnabledFeatures) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentOperationMode( 
            ULONG ulOutputStreamIndex,
            __RPC__in DWORD *pdwEnabledFeatures) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentSampleRequirements( 
            ULONG ulOutputStreamIndex,
            __RPC__in DWORD *pdwRequestedFeatures) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDMOVideoOutputOptimizationsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDMOVideoOutputOptimizations * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDMOVideoOutputOptimizations * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDMOVideoOutputOptimizations * This);
        
        HRESULT ( STDMETHODCALLTYPE *QueryOperationModePreferences )( 
            IDMOVideoOutputOptimizations * This,
            ULONG ulOutputStreamIndex,
            __RPC__in DWORD *pdwRequestedCapabilities);
        
        HRESULT ( STDMETHODCALLTYPE *SetOperationMode )( 
            IDMOVideoOutputOptimizations * This,
            ULONG ulOutputStreamIndex,
            DWORD dwEnabledFeatures);
        
        HRESULT ( STDMETHODCALLTYPE *GetCurrentOperationMode )( 
            IDMOVideoOutputOptimizations * This,
            ULONG ulOutputStreamIndex,
            __RPC__in DWORD *pdwEnabledFeatures);
        
        HRESULT ( STDMETHODCALLTYPE *GetCurrentSampleRequirements )( 
            IDMOVideoOutputOptimizations * This,
            ULONG ulOutputStreamIndex,
            __RPC__in DWORD *pdwRequestedFeatures);
        
        END_INTERFACE
    } IDMOVideoOutputOptimizationsVtbl;

    interface IDMOVideoOutputOptimizations
    {
        CONST_VTBL struct IDMOVideoOutputOptimizationsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDMOVideoOutputOptimizations_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDMOVideoOutputOptimizations_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDMOVideoOutputOptimizations_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDMOVideoOutputOptimizations_QueryOperationModePreferences(This,ulOutputStreamIndex,pdwRequestedCapabilities)	\
    ( (This)->lpVtbl -> QueryOperationModePreferences(This,ulOutputStreamIndex,pdwRequestedCapabilities) ) 

#define IDMOVideoOutputOptimizations_SetOperationMode(This,ulOutputStreamIndex,dwEnabledFeatures)	\
    ( (This)->lpVtbl -> SetOperationMode(This,ulOutputStreamIndex,dwEnabledFeatures) ) 

#define IDMOVideoOutputOptimizations_GetCurrentOperationMode(This,ulOutputStreamIndex,pdwEnabledFeatures)	\
    ( (This)->lpVtbl -> GetCurrentOperationMode(This,ulOutputStreamIndex,pdwEnabledFeatures) ) 

#define IDMOVideoOutputOptimizations_GetCurrentSampleRequirements(This,ulOutputStreamIndex,pdwRequestedFeatures)	\
    ( (This)->lpVtbl -> GetCurrentSampleRequirements(This,ulOutputStreamIndex,pdwRequestedFeatures) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDMOVideoOutputOptimizations_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


