 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dllinterfaces.h>

#include <input/filter-in-wma.h>

using namespace smooth::Threads;

BonkEnc::FilterInWMA::FilterInWMA(Config *config, Track *format) : InputFilter(config, format)
{
	m_pReader = NIL;

	packageSize = 0;

	/* Init the Microsoft COM library.
	 */
	CoInitialize(NIL);
}

BonkEnc::FilterInWMA::~FilterInWMA()
{
	/* Uninit the Microsoft COM library.
	 */
	CoUninitialize();
}

Bool BonkEnc::FilterInWMA::Activate()
{
	HRESULT	 hr = NIL;

	hr = ex_WMCreateReader(NIL, WMT_RIGHT_PLAYBACK, &m_pReader);

	hr = m_pReader->QueryInterface(IID_IWMReaderAdvanced, (void **) &m_pReaderAdvanced);

	readerCallback = new WMAReader();
	readerCallback->SetReaderAdvanced(m_pReaderAdvanced);
	readerCallback->SetSamplesBuffer(&samplesBuffer, &samplesBufferMutex);

	m_hAsyncEvent = readerCallback->GetAsyncEventHandle();

	hr = m_pReader->Open(format->origFilename, readerCallback, NIL);

	/* Wait for the Open call to complete. The event is set in the
	 * OnStatus callback when the reader reports completion.
	 */
	if (!FAILED(hr)) WaitForEvent(m_hAsyncEvent);

	hr = m_pReaderAdvanced->SetUserProvidedClock(true);

	DWORD	 cOutputs = 0;

	/* Find out the output count
	 */
	hr = m_pReader->GetOutputCount(&cOutputs);

	/* Find the first audio output.
	 */
	for (DWORD i = 0; i < cOutputs; i++)
	{
		IWMOutputMediaProps	*pProps = NIL;
		WM_MEDIA_TYPE		*pMediaType = NIL;
		ULONG			 cbType = 0;

		/* Set the first output format as it is
		 * the one with the highest quality.
		 */
		hr = m_pReader->GetOutputFormat(i, 0, &pProps);
		hr = m_pReader->SetOutputProps(i, pProps);

		/* Find out the space needed for pMediaType
		 */
		hr = pProps->GetMediaType(NIL, &cbType);

		pMediaType = (WM_MEDIA_TYPE *) new BYTE[cbType];

		/* Get the value for MediaType
		 */
		hr = pProps->GetMediaType(pMediaType, &cbType);

		if (pMediaType->majortype == WMMEDIATYPE_Audio)
		{
			readerCallback->SetAudioOutputNum(i);

			hr = m_pReader->Start(0, 0, 1.0, NIL);

			/* Wait for the Start call to complete.
			 */
			if (!FAILED(hr)) WaitForEvent(m_hAsyncEvent);
		}

		pProps->Release();

		delete [] pMediaType;
	}

	return True;
}

Bool BonkEnc::FilterInWMA::Deactivate()
{
	readerCallback->SetActive(False);

	HRESULT	 hr = NIL;

	hr = m_pReader->Stop();

	/* Wait for the reader to stop.
	 */
	if (!FAILED(hr)) WaitForEvent(m_hAsyncEvent);

	hr = m_pReader->Close();

	/* Wait for the reader to deliver a WMT_CLOSED event to the
	 * OnStatus callback.
	 */
	if (!FAILED(hr)) WaitForEvent(m_hAsyncEvent);

	m_pReaderAdvanced->Release();
	m_pReader->Release();
	readerCallback->Release();

	return True;
}

Int BonkEnc::FilterInWMA::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	if (!readerCallback->IsActive() && samplesBuffer.Size() <= 0) return -1;

	while (readerCallback->IsActive() && samplesBuffer.Size() <= 0) Sleep(0);

	/* Copy any data from the sample buffer.
	 */
	samplesBufferMutex.Lock();

	data.Resize(samplesBuffer.Size());

	memcpy(data, samplesBuffer, samplesBuffer.Size());

	samplesBuffer.Resize(0);

	samplesBufferMutex.Release();

	/* Update inBytes to indicate progress.
	 */
	inBytes += format->fileSize * data.Size() / (format->approxLength * (format->bits / 8));

	return data.Size();
}

HRESULT BonkEnc::FilterInWMA::GetHeaderAttribute(IWMHeaderInfo *pHeaderInfo, LPCWSTR pwszName, BYTE **ppbValue)
{
	HRESULT			 hr = S_OK;
	WMT_ATTR_DATATYPE	 wmtType;
	WORD			 wStreamNum = 0;
	WORD			 cbLength = 0;

	*ppbValue = NIL;

	/* Get the number of bytes to be allocated for pbValue
	 */
	hr = pHeaderInfo->GetAttributeByName(&wStreamNum, pwszName, &wmtType, NIL, &cbLength);

	/* No such an attribute, so return
	 */
	if (hr == ASF_E_NOTFOUND) return S_OK;

	if (FAILED(hr)) return hr;

	/* Allocate space and get the actual value
	 */
	*ppbValue = new BYTE[cbLength];

	hr = pHeaderInfo->GetAttributeByName(&wStreamNum, pwszName, &wmtType, *ppbValue, &cbLength);

	return S_OK;
}

Void BonkEnc::FilterInWMA::WaitForEvent(HANDLE hEvent, DWORD msMaxWaitTime)
{
	for (DWORD i = 0; i < msMaxWaitTime; i += 10)
	{
		MSG	 msg;

		if (PeekMessage(&msg, (HWND) NIL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (WaitForSingleObject(hEvent, 10) != WAIT_TIMEOUT) break;
	}

	if (readerCallback->IsError())
	{
		errorState = True;
		errorString = readerCallback->GetErrorString();
	}
}

BonkEnc::Track *BonkEnc::FilterInWMA::GetFileInfo(const String &inFile)
{
	Track		*nFormat = new Track;
	InStream	*f_in	 = new InStream(STREAM_FILE, inFile, IS_READ);

	nFormat->fileSize	= f_in->Size();
	nFormat->length		= -1;

	delete f_in;

	HRESULT	 hr = NIL;

	readerCallback = new WMAReader();

	m_hAsyncEvent = readerCallback->GetAsyncEventHandle();

	hr = ex_WMCreateReader(NIL, WMT_RIGHT_PLAYBACK, &m_pReader);

	hr = m_pReader->Open(inFile, readerCallback, NIL);

	/* Wait for the Open call to complete. The event is set in the
	 * OnStatus callback when the reader reports completion.
	 */
	if (!FAILED(hr)) WaitForEvent(m_hAsyncEvent);

	if (!errorState)
	{
		DWORD		 cOutputs = 0;
		WAVEFORMATEX	*pWfx = NIL;

		/* Find out the output count
		 */
		hr = m_pReader->GetOutputCount(&cOutputs);

		/* Find the first audio output.
		 */
		for (DWORD i = 0; i < cOutputs; i++)
		{
			IWMOutputMediaProps	*pProps = NIL;
			WM_MEDIA_TYPE		*pMediaType = NIL;
			ULONG			 cbType = 0;

			/* Set the first output format as it is
			 * the one with the highest quality.
			 */
			hr = m_pReader->GetOutputFormat(i, 0, &pProps);
			hr = m_pReader->SetOutputProps(i, pProps);

			/* Find out the space needed for pMediaType
			 */
			hr = pProps->GetMediaType(NIL, &cbType);

			pMediaType = (WM_MEDIA_TYPE *) new BYTE[cbType];

			/* Get the value for MediaType
			 */
			hr = pProps->GetMediaType(pMediaType, &cbType);

			if (pMediaType->majortype == WMMEDIATYPE_Audio)
			{
				/* Store the wave format for this output
				 */
				pWfx = (WAVEFORMATEX *) new BYTE[pMediaType->cbFormat];

				CopyMemory(pWfx, pMediaType->pbFormat, pMediaType->cbFormat);
			}

			pProps->Release();

			delete [] pMediaType;

			if (pWfx != NIL) break;
		}

		if (pWfx != NIL)
		{
			nFormat->order	  = BYTE_INTEL;
			nFormat->bits	  = pWfx->wBitsPerSample;
			nFormat->rate	  = pWfx->nSamplesPerSec;
			nFormat->channels = pWfx->nChannels;

			delete [] pWfx;
		}

		if (pWfx == NIL) { errorState = True; errorString = "Unknown file type"; }
	}

	if (!errorState)
	{
		IWMHeaderInfo	*pHeaderInfo = NIL;
		BYTE		*pbValue = NIL;

		hr = m_pReader->QueryInterface(IID_IWMHeaderInfo, (void **) &pHeaderInfo);

		/* Get attribute "Duration"
		 */
		hr = GetHeaderAttribute(pHeaderInfo, g_wszWMDuration, &pbValue);

		if (!FAILED(hr) && pbValue != NIL)
		{
			nFormat->length = -1;
			nFormat->approxLength = *(QWORD *) pbValue * nFormat->rate * nFormat->channels / 10000000;

			delete [] pbValue;
		}

		pHeaderInfo->Release();
	}

	hr = m_pReader->Close();

	/* Wait for the reader to deliver a WMT_CLOSED event to the
	 * OnStatus callback.
	 */
	if (!FAILED(hr)) WaitForEvent(m_hAsyncEvent);

	m_pReader->Release();
	readerCallback->Release();

	if (!errorState)
	{
		IWMMetadataEditor	*metadataEditor = NIL;
		IWMMetadataEditor2	*metadataEditor2 = NIL;

		hr = ex_WMCreateEditor(&metadataEditor);

		hr = metadataEditor->QueryInterface(IID_IWMMetadataEditor2, (void **) &metadataEditor2);

		hr = metadataEditor2->OpenEx(inFile, GENERIC_READ, FILE_SHARE_READ);

		if (hr == S_OK)
		{
			IWMHeaderInfo3	*pHeaderInfo = NIL;

			hr = metadataEditor2->QueryInterface(IID_IWMHeaderInfo3, (void **) &pHeaderInfo);

			WORD	 langIndex = 0;
			WORD	 numIndices = 0;

			hr = pHeaderInfo->GetAttributeIndices(0, NIL, &langIndex, NIL, &numIndices);

			WORD	*indices = new WORD [numIndices];

			hr = pHeaderInfo->GetAttributeIndices(0, NIL, &langIndex, indices, &numIndices);

			for (Int i = 0; i < numIndices; i++)
			{
				WORD			 nameLen  = 1024;
				LPWSTR			 name	  = new WCHAR [nameLen];
				DWORD			 cbLength = 0;

				hr = pHeaderInfo->GetAttributeByIndexEx(0, indices[i], name, &nameLen, NIL, NIL, NIL, &cbLength);

				WMT_ATTR_DATATYPE	 type	  = WMT_TYPE_DWORD;
				BYTE			*pbValue  = new BYTE [cbLength];

				hr = pHeaderInfo->GetAttributeByIndexEx(0, indices[i], name, &nameLen, &type, NIL, pbValue, &cbLength);

				if	(String(name) == g_wszWMAuthor)		nFormat->artist  = (LPWSTR) pbValue;
				else if (String(name) == g_wszWMTitle)		nFormat->title   = (LPWSTR) pbValue;
				else if (String(name) == g_wszWMAlbumTitle)	nFormat->album   = (LPWSTR) pbValue;
				else if (String(name) == g_wszWMYear)		nFormat->year    = String((LPWSTR) pbValue).ToInt();
				else if (String(name) == g_wszWMGenre)		nFormat->genre   = (LPWSTR) pbValue;
				else if (String(name) == g_wszWMDescription)	nFormat->comment = (LPWSTR) pbValue;
				else if (String(name) == g_wszWMPublisher)	nFormat->label   = (LPWSTR) pbValue;
				else if (String(name) == g_wszWMISRC)		nFormat->isrc    = (LPWSTR) pbValue;

				else if (String(name) == g_wszWMTrack)
				{
					if	(type == WMT_TYPE_DWORD)  nFormat->track = 1 + ((DWORD *) pbValue)[0];
					else if (type == WMT_TYPE_STRING) nFormat->track = 1 + String((LPWSTR) pbValue).ToInt();
				}
				else if (String(name) == g_wszWMTrackNumber)
				{
					if	(type == WMT_TYPE_DWORD)  nFormat->track = ((DWORD *) pbValue)[0];
					else if (type == WMT_TYPE_STRING) nFormat->track = String((LPWSTR) pbValue).ToInt();
				}
				else if (String(name) == g_wszWMPicture)
				{
					WM_PICTURE	*picData = (WM_PICTURE *) pbValue;
					Picture		*picture = new Picture();

					picture->mime = picData->pwszMIMEType;

					if	(picture->mime.ToLower() == "jpeg" || picture->mime.ToLower() == "jpg") picture->mime = "image/jpeg";
					else if (picture->mime.ToLower() == "png")					picture->mime = "image/png";

					picture->type = picData->bPictureType;
					picture->description = picData->pwszDescription;

					picture->data.Resize(picData->dwDataLen);

					memcpy(picture->data, picData->pbData, picture->data.Size());

					if (picture->data.Size() > 16 && picture->data[0] != 0 && picture->data[1] != 0) nFormat->pictures.Add(picture);
					else										 delete picture;
				}

				delete [] pbValue;
				delete [] name;
			}

			delete [] indices;

			pHeaderInfo->Release();
		}

		hr = metadataEditor2->Close();

		metadataEditor->Release();
		metadataEditor2->Release();
	}

	if (errorState)	{ delete nFormat; return NIL;	  }
	else		{		  return nFormat; }
}

BonkEnc::WMAReader::WMAReader()
{
	m_cRef = 1;

	m_fEOF = false;
	m_qwTime = 0;
	m_dwAudioOutputNum = 0;

	m_hAsyncEvent = CreateEvent(NIL, FALSE, FALSE, NIL);

	m_pReaderAdvanced = NIL;

	active = False;
	error = False;

	samplesBuffer = NIL;
	samplesBufferMutex = NIL;
}

BonkEnc::WMAReader::~WMAReader()
{
	CloseHandle(m_hAsyncEvent);
}

HRESULT BonkEnc::WMAReader::QueryInterface(REFIID riid, void **ppvObject) 
{
	if (ppvObject == NIL) return E_INVALIDARG;

	if (riid == IID_IWMReaderCallback)
	{
		*ppvObject = static_cast<IWMReaderCallback *>(this);
	}
	else if (riid == IID_IWMReaderCallbackAdvanced)
	{
		*ppvObject = static_cast<IWMReaderCallbackAdvanced *>(this);
	}
	else if (riid == IID_IUnknown)
	{
		*ppvObject = static_cast<IWMReaderCallback *>(this);
	}
	else
	{
		*ppvObject = NIL;

		return E_NOINTERFACE;
	}

	AddRef();

	return S_OK;
}

ULONG BonkEnc::WMAReader::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

ULONG BonkEnc::WMAReader::Release()
{
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete this;

		return 0;
	}

	return m_cRef;
}

HRESULT BonkEnc::WMAReader::OnStatus(WMT_STATUS status, HRESULT hr, WMT_ATTR_DATATYPE dwType, BYTE __RPC_FAR *pValue, void __RPC_FAR *pvContext)
{
	/* This switch checks for the important messages sent by the reader object.
	 */
	switch (status)
	{
		/* The reader is finished opening a file.
		 */
		case WMT_OPENED:
			SetAsyncEvent(hr);

			break;

		/* The reader is finished closing a file.
		 */
		case WMT_CLOSED:
			SetAsyncEvent(hr);

			break;

		/* Playback of the opened file has begun.
		 */
		case WMT_STARTED:
			m_qwTime = 10000000;

			m_pReaderAdvanced->DeliverTime(m_qwTime);

			active = True;

			SetAsyncEvent(hr);

			break;

		/* The previously playing reader has stopped.
		 */
		case WMT_STOPPED:
			active = False;

			SetAsyncEvent(hr);

			break;

		case WMT_EOF:
			active = False;

			m_fEOF = true;

			break;

		/* This class reacts to any errors by changing its state to stopped.
		 */
		case WMT_ERROR:
			active = False;
			error = True;

			break;

		case WMT_MISSING_CODEC:
			error = True;
			errorString = "Missing appropriate codec";

			break;

		case WMT_NO_RIGHTS:
		case WMT_NO_RIGHTS_EX:
			error = True;
			errorString = "Cannot open protected files";

			break;

		/* Do nothing on other events.
		 */
		default:
			break;
	}

	return S_OK;
}

HRESULT BonkEnc::WMAReader::OnSample(DWORD dwOutputNum, QWORD cnsSampleTime, QWORD cnsSampleDuration, DWORD dwFlags, INSSBuffer *pSample, void *pvContext)
{
	/* Check the output number of the sample against the stored output number.
	 * Because only the first audio output is stored, all other outputs,
	 * regardless of type, will be ignored.
	 */
	if (dwOutputNum != m_dwAudioOutputNum)
	{
		return S_OK;
	}

	BYTE	*pData = NIL;
	DWORD	 cbData = 0;
	HRESULT	 hr = S_OK;

	/* Get the sample from the buffer object.
	 */
	hr = pSample->GetBufferAndLength( &pData, &cbData );

	if (hr == S_OK)
	{
		while (IsActive() && samplesBuffer->Size() >= 131072) S::System::System::Sleep(0);

		/* Copy the sample to the sample buffer.
		 */
		if (IsActive())
		{
			samplesBufferMutex->Lock();

			samplesBuffer->Resize(samplesBuffer->Size() + cbData);

			memcpy((UnsignedByte *) *samplesBuffer + samplesBuffer->Size() - cbData, pData, cbData);

			samplesBufferMutex->Release();
		}
	}

	return hr;
}

HRESULT BonkEnc::WMAReader::OnTime(QWORD cnsCurrentTime, void *pvContext)
{
	HRESULT	 hr = S_OK;

	/*  Keep asking for the specific duration of the stream till EOF
	 */
	if (m_fEOF == false)
	{
		m_qwTime += 10000000; // 1 second

		hr = m_pReaderAdvanced->DeliverTime(m_qwTime);
	}

	return hr;
}

Void BonkEnc::WMAReader::SetAsyncEvent(HRESULT hrAsync)
{
	SetEvent(m_hAsyncEvent);
}

Bool BonkEnc::WMAReader::IsActive()
{
	return active;
}

Void BonkEnc::WMAReader::SetActive(Bool nActive)
{
	active = nActive;
}

Bool BonkEnc::WMAReader::IsError()
{
	return error;
}

const String &BonkEnc::WMAReader::GetErrorString()
{
	return errorString;
}

HANDLE BonkEnc::WMAReader::GetAsyncEventHandle() const
{
	return m_hAsyncEvent;
}

Void BonkEnc::WMAReader::SetReaderAdvanced(IWMReaderAdvanced *pReaderAdvanced)
{
	m_pReaderAdvanced = pReaderAdvanced;
}

Void BonkEnc::WMAReader::SetAudioOutputNum(DWORD dwAudioOutputNum)
{
	m_dwAudioOutputNum = dwAudioOutputNum;
}

Void BonkEnc::WMAReader::SetSamplesBuffer(Buffer<UnsignedByte> *buffer, Mutex *mutex)
{
	samplesBuffer = buffer;
	samplesBufferMutex = mutex;

	/* Allocate 128kB for the sample buffer.
	 */
	samplesBuffer->Resize(131072);
	samplesBuffer->Resize(0);
}
