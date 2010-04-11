 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dllinterfaces.h>

#include <output/filter-out-wma.h>

namespace BonkEnc
{
	Error RenderWMAItem(const String &id, const String &value, Void *headerInfo)
	{
		HRESULT	 hr = ((IWMHeaderInfo3 *) headerInfo)->AddAttribute(0, id, NIL, WMT_TYPE_STRING, 0, (BYTE *) (wchar_t *) value, wcslen(value) * 2 + 2);

		if (hr == S_OK) return Success();
		else		return Error();
	}
}

BonkEnc::FilterOutWMA::FilterOutWMA(Config *config, Track *format) : OutputFilter(config, format)
{
	samplesWritten = 0;

	/* Init the Microsoft COM library.
	 */
	CoInitialize(NIL);
}

BonkEnc::FilterOutWMA::~FilterOutWMA()
{
	/* Uninit the Microsoft COM library.
	 */
	CoUninitialize();
}

Bool BonkEnc::FilterOutWMA::Activate()
{
	HRESULT	 hr = S_OK;

	hr = ex_WMCreateWriter(NIL, &m_pWriter);
	hr = m_pWriter->QueryInterface(IID_IWMWriterAdvanced, (void **) &m_pWriterAdvanced);

	hr = ex_WMCreateWriterFileSink(&m_pWriterFileSink);

	hr = m_pWriterFileSink->Open(String(format->outfile).Append(".out"));

	hr = m_pWriterAdvanced->AddSink(m_pWriterFileSink);

	hr = ex_WMCreateProfileManager(&m_pProfileManager);

	hr = m_pProfileManager->CreateEmptyProfile(WMT_VER_9_0, &m_pProfile);

	IWMCodecInfo3	*pCodecInfo = NIL;

	hr = m_pProfileManager->QueryInterface(IID_IWMCodecInfo3, (void **) &pCodecInfo);

	if (currentConfig->wma_uncompressed)
	{
		hr = m_pProfile->CreateNewStream(WMMEDIATYPE_Audio, &m_pStreamConfig);
	}
	else if (!currentConfig->wma_autoFormat)
	{
		BOOL	 falseValue = FALSE;
		BOOL	 trueValue = TRUE;
		DWORD	 oneValue = 1;
		DWORD	 twoValue = 2;

		if (currentConfig->wma_vbr)   hr = pCodecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, currentConfig->wma_codec, g_wszVBREnabled, WMT_TYPE_BOOL, (BYTE *) &trueValue,  sizeof(BOOL));
		else			      hr = pCodecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, currentConfig->wma_codec, g_wszVBREnabled, WMT_TYPE_BOOL, (BYTE *) &falseValue, sizeof(BOOL));

		if (currentConfig->wma_2pass) hr = pCodecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, currentConfig->wma_codec, g_wszNumPasses, WMT_TYPE_DWORD, (BYTE *) &twoValue, sizeof(DWORD));
		else			      hr = pCodecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, currentConfig->wma_codec, g_wszNumPasses, WMT_TYPE_DWORD, (BYTE *) &oneValue, sizeof(DWORD));

		hr = pCodecInfo->GetCodecFormat(WMMEDIATYPE_Audio, currentConfig->wma_codec, currentConfig->wma_codecFormat, &m_pStreamConfig);
	}
	else
	{
		m_pStreamConfig = GetBestCodecFormat(pCodecInfo, currentConfig->wma_codec, format);
	}

	hr = m_pStreamConfig->SetStreamNumber(1);
	hr = m_pStreamConfig->SetStreamName(String("Audio"));
	hr = m_pStreamConfig->SetConnectionName(String("Audio"));

	hr = m_pProfile->AddStream(m_pStreamConfig);

	hr = m_pWriter->SetProfile(m_pProfile);

	if (SetInputFormat(m_pWriter, format) == False) errorState = True;

	pCodecInfo->Release();

	if (!errorState)
	{
		hr = m_pWriter->BeginWriting();
	}
	else
	{
		m_pStreamConfig->Release();

		m_pProfile->Release();
		m_pProfileManager->Release();

		m_pWriterFileSink->Release();

		m_pWriterAdvanced->Release();
		m_pWriter->Release();

		errorString = "Could not initialize encoder.";
	}

	return !errorState;
}

Bool BonkEnc::FilterOutWMA::Deactivate()
{
	if (errorState)
	{
		File(String(format->outfile).Append(".out")).Delete();

		return True;
	}

	HRESULT	 hr = S_OK;

	hr = m_pWriter->Flush();

	hr = m_pWriter->EndWriting();

	hr = m_pWriterAdvanced->RemoveSink(m_pWriterFileSink);

	m_pStreamConfig->Release();

	m_pProfile->Release();
	m_pProfileManager->Release();

	m_pWriterFileSink->Release();

	m_pWriterAdvanced->Release();
	m_pWriter->Release();

	/* Write metadata to file
	 */
	if (currentConfig->enable_wmatags)
	{
		if (format->artist != NIL || format->title != NIL)
		{
			IWMMetadataEditor	*metadataEditor = NIL;
			IWMMetadataEditor2	*metadataEditor2 = NIL;

			hr = ex_WMCreateEditor(&metadataEditor);

			hr = metadataEditor->QueryInterface(IID_IWMMetadataEditor2, (void **) &metadataEditor2);

			hr = metadataEditor2->OpenEx(String(format->outfile).Append(".out"), GENERIC_READ | GENERIC_WRITE, 0);

			if (hr == S_OK)
			{
				IWMHeaderInfo3	*pHeaderInfo = NIL;

				hr = metadataEditor2->QueryInterface(IID_IWMHeaderInfo3, (void **) &pHeaderInfo);

				if (format->artist != NIL) RenderWMAItem(g_wszWMAuthor,	     format->artist,		     pHeaderInfo);
				if (format->title  != NIL) RenderWMAItem(g_wszWMTitle,	     format->title,		     pHeaderInfo);
				if (format->album  != NIL) RenderWMAItem(g_wszWMAlbumTitle,  format->album,		     pHeaderInfo);
				if (format->track   >   0) RenderWMAItem(g_wszWMTrackNumber, String::FromInt(format->track), pHeaderInfo);
				if (format->year    >   0) RenderWMAItem(g_wszWMYear,	     String::FromInt(format->year),  pHeaderInfo);
				if (format->genre  != NIL) RenderWMAItem(g_wszWMGenre,	     format->genre,		     pHeaderInfo);
				if (format->label  != NIL) RenderWMAItem(g_wszWMPublisher,   format->label,		     pHeaderInfo);
				if (format->isrc   != NIL) RenderWMAItem(g_wszWMISRC,	     format->isrc,		     pHeaderInfo);

				if	(format->comment != NIL && !currentConfig->overwriteComments)	RenderWMAItem(g_wszWMDescription, format->comment, pHeaderInfo);
				else if (currentConfig->default_comment != NIL)				RenderWMAItem(g_wszWMDescription, currentConfig->default_comment, pHeaderInfo);

				/* Save cover art.
				*/
				if (currentConfig->copy_picture_tags)
				{
					foreach (const Picture *picInfo, format->pictures)
					{
						WM_PICTURE	 picture;

						picture.pwszMIMEType	= new WCHAR [picInfo->mime.Length() + 1];
						picture.bPictureType	= picInfo->type;
						picture.pwszDescription	= new WCHAR [picInfo->description.Length() + 1];
						picture.dwDataLen	= picInfo->data.Size();
						picture.pbData		= (BYTE *) (UnsignedByte *) picInfo->data;

						wcsncpy(picture.pwszMIMEType, picInfo->mime, picInfo->mime.Length() + 1);
						wcsncpy(picture.pwszDescription, picInfo->description, picInfo->description.Length() + 1);

						hr = pHeaderInfo->AddAttribute(0, g_wszWMPicture, NIL, WMT_TYPE_BINARY, 0, (BYTE *) &picture, sizeof(WM_PICTURE));

						delete [] picture.pwszMIMEType;
						delete [] picture.pwszDescription;
					}
				}

				pHeaderInfo->Release();
			}

			hr = metadataEditor2->Flush();

			metadataEditor->Release();
			metadataEditor2->Release();
		}
	}

	/* Stream contents of created WMA file to output driver
	 */
	InStream		 in(STREAM_FILE, String(format->outfile).Append(".out"), IS_READONLY);
	Buffer<UnsignedByte>	 buffer(1024);
	Int			 bytesLeft = in.Size();

	while (bytesLeft)
	{
		driver->WriteData((UnsignedByte *) in.InputData(buffer, Math::Min(1024, bytesLeft)), Math::Min(1024, bytesLeft));

		bytesLeft -= Math::Min(1024, bytesLeft);
	}

	in.Close();

	File(String(format->outfile).Append(".out")).Delete();

	return True;
}

Int BonkEnc::FilterOutWMA::WriteData(Buffer<UnsignedByte> &data, Int size)
{
	HRESULT	 hr;

	INSSBuffer	*pSample = NIL;

	hr = m_pWriter->AllocateSample(size, &pSample);

	BYTE	*buffer = NIL;

	hr = pSample->GetBuffer(&buffer);

	memcpy(buffer, data, size);

	pSample->SetLength(size);

	QWORD	 cnsSampleTime = samplesWritten * 10000000 / format->channels / format->rate;

	samplesWritten += size / (format->bits / 8);

        hr = m_pWriter->WriteSample(0, cnsSampleTime, WM_SF_CLEANPOINT, pSample);

	pSample->Release();

	return size;
}

/* This method will return the format best matching
 * our requirements for a specified codec.
 */
IWMStreamConfig *BonkEnc::FilterOutWMA::GetBestCodecFormat(IWMCodecInfo3 *pCodecInfo, DWORD codecIndex, const Track *format)
{
	HRESULT		 hr = S_OK;

	BOOL		 falseValue = FALSE;
	BOOL		 trueValue = TRUE;
	DWORD		 oneValue = 1;
	DWORD		 twoValue = 2;

	Bool		 supportCBR	= False;
	Bool		 supportVBR	= False;
	Bool		 support1Pass	= True;
	Bool		 support2Pass	= False;

	/* Check if CBR is supported.
	 */
	{
		hr = pCodecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, codecIndex, g_wszVBREnabled, WMT_TYPE_BOOL, (BYTE *) &falseValue, sizeof(BOOL));
		hr = pCodecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, codecIndex, g_wszNumPasses, WMT_TYPE_DWORD, (BYTE *) &oneValue, sizeof(DWORD));

		DWORD	 numFormats = 0;

		hr = pCodecInfo->GetCodecFormatCount(WMMEDIATYPE_Audio, codecIndex, &numFormats);

		if (hr == S_OK && numFormats > 0) supportCBR = True;
	}

	/* Check if VBR is supported.
	 */
	{
		WMT_ATTR_DATATYPE	 valueType = WMT_TYPE_BOOL;
		DWORD			 valueSize = sizeof(BOOL);
		BOOL			 isVBRSupported = FALSE;

		hr = pCodecInfo->GetCodecProp(WMMEDIATYPE_Audio, codecIndex, g_wszIsVBRSupported, &valueType, (BYTE *) &isVBRSupported, &valueSize);

		if (isVBRSupported)
		{
			hr = pCodecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, codecIndex, g_wszVBREnabled, WMT_TYPE_BOOL, (BYTE *) &trueValue, sizeof(BOOL));
			hr = pCodecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, codecIndex, g_wszNumPasses, WMT_TYPE_DWORD, (BYTE *) &oneValue, sizeof(DWORD));

			if (hr == S_OK)
			{
				DWORD	 numFormats = 0;

				hr = pCodecInfo->GetCodecFormatCount(WMMEDIATYPE_Audio, codecIndex, &numFormats);

				if (hr == S_OK && numFormats > 0) supportVBR = True;
			}
		}
	}

	if (supportVBR && (!supportCBR || currentConfig->wma_vbr))	 hr = pCodecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, codecIndex, g_wszVBREnabled, WMT_TYPE_BOOL, (BYTE *) &trueValue,  sizeof(BOOL));
	else								 hr = pCodecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, codecIndex, g_wszVBREnabled, WMT_TYPE_BOOL, (BYTE *) &falseValue, sizeof(BOOL));

	if (support2Pass && (!support1Pass || currentConfig->wma_2pass)) hr = pCodecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, codecIndex, g_wszNumPasses, WMT_TYPE_DWORD, (BYTE *) &twoValue, sizeof(DWORD));
	else								 hr = pCodecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, codecIndex, g_wszNumPasses, WMT_TYPE_DWORD, (BYTE *) &oneValue, sizeof(DWORD));

	IWMStreamConfig	*result = NIL;

	Int		 targetBitrate = currentConfig->wma_bitrate * 1000;
	Int		 targetQuality = currentConfig->wma_quality;

	Bool		 useVBR = (supportVBR && currentConfig->wma_vbr);

	DWORD		 bestMatchBitrate	= 100000000;
	DWORD		 bestMatchQuality	= 100000000;
	DWORD		 bestMatchSampleRate	= 100000000;
	DWORD		 bestMatchBits		= 100000000;
	DWORD		 bestMatchChannels	= 100000000;

	DWORD		 numFormats = 0;

	hr = pCodecInfo->GetCodecFormatCount(WMMEDIATYPE_Audio, codecIndex, &numFormats);

	for (DWORD i = 0; i < numFormats; i++)
	{
		IWMStreamConfig	*pStreamConfig = NIL;
		IWMMediaProps	*pMediaProps = NIL;

		hr = pCodecInfo->GetCodecFormat(WMMEDIATYPE_Audio, codecIndex, i, &pStreamConfig);
		hr = pStreamConfig->QueryInterface(IID_IWMMediaProps, (void **) &pMediaProps);

		DWORD		 formatBitrate = 0;
		DWORD		 formatQuality = 0;

		hr = pStreamConfig->GetBitrate(&formatBitrate);

		DWORD		 mediaTypeSize = 0;

		hr = pMediaProps->GetMediaType(NIL, &mediaTypeSize);

		WM_MEDIA_TYPE	*mediaType = (WM_MEDIA_TYPE *) new BYTE [mediaTypeSize];

		hr = pMediaProps->GetMediaType(mediaType, &mediaTypeSize);

		if (mediaType->majortype  == WMMEDIATYPE_Audio &&
		    mediaType->formattype == WMFORMAT_WaveFormatEx)
		{
			Bool		 newBestMatch = False;
			WAVEFORMATEX	*waveFormat = (WAVEFORMATEX *) mediaType->pbFormat;

			formatQuality = waveFormat->nAvgBytesPerSec & 255;

			if	(Math::Abs(Int(waveFormat->nChannels	  - format->channels))	       < Math::Abs(Int(bestMatchChannels   - format->channels))) newBestMatch = True;
			else if (Math::Abs(Int(waveFormat->nChannels	  - format->channels))	      == Math::Abs(Int(bestMatchChannels   - format->channels)) &&
				 Math::Abs(Int(waveFormat->nSamplesPerSec - (Unsigned) format->rate))  < Math::Abs(Int(bestMatchSampleRate - (Unsigned) format->rate))) newBestMatch = True;
			else if (Math::Abs(Int(waveFormat->nChannels	  - format->channels))	      == Math::Abs(Int(bestMatchChannels   - format->channels)) &&
				 Math::Abs(Int(waveFormat->nSamplesPerSec - (Unsigned) format->rate)) == Math::Abs(Int(bestMatchSampleRate - (Unsigned) format->rate)) &&
				 Math::Abs(Int(waveFormat->wBitsPerSample - format->bits))	       < Math::Abs(Int(bestMatchBits	   - format->bits))) newBestMatch = True;
			else if (Math::Abs(Int(waveFormat->nChannels	  - format->channels))	      == Math::Abs(Int(bestMatchChannels   - format->channels)) &&
				 Math::Abs(Int(waveFormat->nSamplesPerSec - (Unsigned) format->rate)) == Math::Abs(Int(bestMatchSampleRate - (Unsigned) format->rate)) &&
				 Math::Abs(Int(waveFormat->wBitsPerSample - format->bits))	      == Math::Abs(Int(bestMatchBits	   - format->bits)) &&
				 ((!useVBR && Math::Abs(Int(formatBitrate - targetBitrate))	       < Math::Abs(Int(bestMatchBitrate	   - targetBitrate))) ||
				  ( useVBR && Math::Abs(Int(formatQuality - targetQuality))	       < Math::Abs(Int(bestMatchQuality	   - targetQuality))))) newBestMatch = True;

			if (newBestMatch)
			{
				if (result != NIL) result->Release();

				result = pStreamConfig;
				result->AddRef();

				bestMatchChannels	= waveFormat->nChannels;
				bestMatchBits		= waveFormat->wBitsPerSample;
				bestMatchSampleRate	= waveFormat->nSamplesPerSec;
				bestMatchBitrate	= formatBitrate;
				bestMatchQuality	= formatQuality;
			}
		}

		delete [] mediaType;

		pMediaProps->Release();
		pStreamConfig->Release();		
	}

	return result;
}

/* This method will set the input format to the
 * specified format.
 */
Bool BonkEnc::FilterOutWMA::SetInputFormat(IWMWriter *pWriter, const Track *format)
{
	HRESULT			 hr = S_OK;

	IWMInputMediaProps	*pInputProps = NIL;

	hr = pWriter->GetInputProps(0, &pInputProps);

	WM_MEDIA_TYPE		 mediaType;

	mediaType.majortype		= WMMEDIATYPE_Audio;
	mediaType.subtype		= WMMEDIASUBTYPE_PCM;
	mediaType.bFixedSizeSamples	= True;
	mediaType.bTemporalCompression	= False;
	mediaType.lSampleSize		= (format->bits / 8) * format->channels;
	mediaType.formattype		= WMFORMAT_WaveFormatEx;
	mediaType.pUnk			= NIL;

	WAVEFORMATEX		 waveFormat;

	waveFormat.wFormatTag		= WAVE_FORMAT_PCM;
	waveFormat.nChannels		= format->channels;
	waveFormat.nSamplesPerSec	= format->rate;
	waveFormat.nAvgBytesPerSec	= format->rate * (format->bits / 8) * format->channels;
	waveFormat.nBlockAlign		= (format->bits / 8) * format->channels;
	waveFormat.wBitsPerSample	= format->bits;
	waveFormat.cbSize		= 0;

	mediaType.cbFormat		= sizeof(waveFormat);
	mediaType.pbFormat		= (BYTE *) &waveFormat;

	hr = pInputProps->SetMediaType(&mediaType);

	hr = m_pWriter->SetInputProps(0, pInputProps);

	pInputProps->Release();

	return (hr == S_OK);
}
