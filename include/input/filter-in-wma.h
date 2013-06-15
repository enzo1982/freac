 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FILTER_IN_WMA_
#define _H_FILTER_IN_WMA_

#include "inputfilter.h"

#include <3rdparty/wmsdk/wmsdk.h>

namespace BonkEnc
{
	class WMAReader;

	class BEEXPORT FilterInWMA : public InputFilter
	{
		friend class WMAReader;

		private:
			WMAReader		*readerCallback;

			Buffer<UnsignedByte>	 samplesBuffer;
			Mutex			 samplesBufferMutex;

			HANDLE			 m_hAsyncEvent;

			IWMReader		*m_pReader;
			IWMReaderAdvanced	*m_pReaderAdvanced;

			HRESULT			 GetHeaderAttribute(IWMHeaderInfo *, LPCWSTR, BYTE **);

			Void			 WaitForEvent(HANDLE, DWORD = INFINITE);
		public:
						 FilterInWMA(Config *, Track *);
						~FilterInWMA();

			Bool			 Activate();
			Bool			 Deactivate();

			Int			 ReadData(Buffer<UnsignedByte> &, Int);

			Track			*GetFileInfo(const String &);
	};

	class WMAReader : public IWMReaderCallback,
			  public IWMReaderCallbackAdvanced
	{
		private:
			LONG				 m_cRef;

			BOOL				 m_fEOF;
			QWORD				 m_qwTime;
			DWORD				 m_dwAudioOutputNum;

			HANDLE				 m_hAsyncEvent;

			IWMReaderAdvanced		*m_pReaderAdvanced;

			Bool				 active;
			Bool				 error;

			String				 errorString;

			Buffer<UnsignedByte>		*samplesBuffer;
			Mutex				*samplesBufferMutex;
		public:
							 WMAReader();
			virtual				~WMAReader();

			HRESULT STDMETHODCALLTYPE	 QueryInterface(REFIID, void **);
			ULONG STDMETHODCALLTYPE		 AddRef();
			ULONG STDMETHODCALLTYPE		 Release();

			HRESULT STDMETHODCALLTYPE	 OnStatus(WMT_STATUS, HRESULT, WMT_ATTR_DATATYPE, BYTE *, void *);
			HRESULT STDMETHODCALLTYPE	 OnSample(DWORD, QWORD, QWORD, DWORD, INSSBuffer *, void *);
			HRESULT STDMETHODCALLTYPE	 OnTime(QWORD, void *);

			HRESULT STDMETHODCALLTYPE	 OnStreamSample(WORD wStreamNum, QWORD cnsSampleTime, QWORD cnsSampleDuration, DWORD dwFlags, INSSBuffer *pSample, void *pvContext)
							{ return S_OK; }

			HRESULT STDMETHODCALLTYPE	 OnStreamSelection(WORD wStreamCount, WORD *pStreamNumbers, WMT_STREAM_SELECTION *pSelections, void *pvContext)
							{ return S_OK; }

			HRESULT STDMETHODCALLTYPE	 OnOutputPropsChanged(DWORD dwOutputNum, WM_MEDIA_TYPE *pMediaType, void *pvContext)
							{ return S_OK; }

			HRESULT STDMETHODCALLTYPE	 AllocateForStream(WORD wStreamNum, DWORD cbBuffer, INSSBuffer **ppBuffer, void *pvContext)
							{ return E_NOTIMPL; }

			HRESULT STDMETHODCALLTYPE	 AllocateForOutput(DWORD dwOutputNum, DWORD cbBuffer, INSSBuffer **ppBuffer, void *pvContext)
							{ return E_NOTIMPL; }

			Void				 SetAsyncEvent(HRESULT hrAsync);
		accessors:
			Bool				 IsActive();
			Void				 SetActive(Bool);

			Bool				 IsError();
			const String			&GetErrorString();

			HANDLE				 GetAsyncEventHandle() const;

			Void				 SetReaderAdvanced(IWMReaderAdvanced *);
			Void				 SetAudioOutputNum(DWORD);

			Void				 SetSamplesBuffer(Buffer<UnsignedByte> *, Mutex *);
	};
};

#endif
