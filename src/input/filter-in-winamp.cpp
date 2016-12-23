 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-winamp.h>

#include <dllinterfaces.h>
#include <utilities.h>

using namespace smooth::Threads;

namespace freac
{
	FilterInWinamp	*filter = NIL;

	void		 SetInfo(int, int, int, int);
	void		 VSASetInfo(int, int);
	void		 VSAAddPCMData(void *, int, int, int);
	int		 VSAGetMode(int *, int *);
	void		 VSAAdd(void *, int);
	void		 SAVSAInit(int, int);
	void		 SAVSADeInit();
	void		 SAAddPCMData(void *, int, int, int);
	int		 SAGetMode();
	void		 SAAdd(void *, int, int);
	int		 dsp_isactive();
	int		 dsp_dosamples(short int *, int, int, int, int);

	int		 Out_Open(int, int, int, int, int);
	void		 Out_Close();
	void		 Out_Flush(int);
	int		 Out_Write(char *, int);
	int		 Out_CanWrite();
	int		 Out_IsPlaying();
	int		 Out_Pause(int);
	void		 Out_SetVolume(int);
	void		 Out_SetPan(int);
	int		 Out_GetOutputTime();
	int		 Out_GetWrittenTime();
};

freac::FilterInWinamp::FilterInWinamp(Config *config, Track *format, In_Module *iPlugin) : InputFilter(config, format)
{
	plugin				= iPlugin;

	plugin->SetInfo			= SetInfo;
	plugin->VSASetInfo		= VSASetInfo;
	plugin->VSAAddPCMData		= VSAAddPCMData;
	plugin->VSAGetMode		= VSAGetMode;
	plugin->VSAAdd			= VSAAdd;
	plugin->SAVSAInit		= SAVSAInit;
	plugin->SAVSADeInit		= SAVSADeInit;
	plugin->SAAddPCMData		= SAAddPCMData;
	plugin->SAGetMode		= SAGetMode;
	plugin->SAAdd			= SAAdd;
	plugin->dsp_isactive		= dsp_isactive;
	plugin->dsp_dosamples		= dsp_dosamples;

	plugin->outMod			= new Out_Module();

	plugin->outMod->Open		= Out_Open;
	plugin->outMod->Close		= Out_Close;
	plugin->outMod->Flush		= Out_Flush;
	plugin->outMod->Write		= Out_Write;
	plugin->outMod->CanWrite	= Out_CanWrite;
	plugin->outMod->IsPlaying	= Out_IsPlaying;
	plugin->outMod->Pause		= Out_Pause;
	plugin->outMod->SetVolume	= Out_SetVolume;
	plugin->outMod->SetPan		= Out_SetPan;
	plugin->outMod->GetOutputTime	= Out_GetOutputTime;
	plugin->outMod->GetWrittenTime	= Out_GetWrittenTime;

	packageSize		= 0;

	infoTrack		= NIL;
	samplesBufferMutex	= NIL;
	samplesDone		= 0;
}

freac::FilterInWinamp::~FilterInWinamp()
{
	delete plugin->outMod;
}

Bool freac::FilterInWinamp::Activate()
{
	if (GetTempFile(format->origFilename) != format->origFilename)
	{
		File	 mp4File(format->origFilename);

		mp4File.Copy(GetTempFile(format->origFilename));
	}

	samplesBufferMutex = new Mutex();
	samplesBufferMutex->Lock();

	filter = this;

	plugin->Play(GetTempFile(format->origFilename));

	return true;
}

Bool freac::FilterInWinamp::Deactivate()
{
	samplesBufferMutex->Release();

	plugin->Stop();

	delete samplesBufferMutex;

	if (GetTempFile(format->origFilename) != format->origFilename)
	{
		File	 tempFile(GetTempFile(format->origFilename));

		tempFile.Delete();
	}

	return true;
}

Int freac::FilterInWinamp::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	samplesBufferMutex->Release();

	Int	 start = clock();

	while (clock() - start < CLOCKS_PER_SEC && samplesBuffer.Size() <= 0) S::System::System::Sleep(0);

	samplesBufferMutex->Lock();

	size = samplesBuffer.Size();

	data.Resize(size);

	memcpy(data, samplesBuffer, size);

	samplesBuffer.Resize(0);

	/* Set inBytes to a value that reflects
	 * our approximate position in the file.
	 */
	samplesDone += size / (format->bits / 8);
	inBytes = format->fileSize * samplesDone / format->approxLength;

	if (size == 0)	return -1;
	else		return size;
}

String freac::FilterInWinamp::GetTempFile(const String &oFileName)
{
	String	 rVal	= oFileName;
	Int	 lastBs	= -1;

	for (Int i = 0; i < rVal.Length(); i++)
	{
		if (rVal[i] > 255)			rVal[i] = '#';
		if (rVal[i] == '\\' || rVal[i] == '/')	lastBs = i;
	}

	if (rVal == oFileName) return rVal;

	String	 tempDir = S::System::System::GetTempDirectory();

	for (Int j = lastBs + 1; j < rVal.Length(); j++)
	{
		tempDir[tempDir.Length()] = rVal[j];
	}

	return tempDir.Append(".in.temp");
}

freac::Track *freac::FilterInWinamp::GetFileInfo(const String &inFile)
{
	if (GetTempFile(inFile) != inFile)
	{
		File	 origFile(inFile);

		origFile.Copy(GetTempFile(inFile));
	}

	Track		*nFormat = new Track;
	InStream	*f_in = new InStream(STREAM_FILE, GetTempFile(inFile), IS_READ);

	nFormat->order		= BYTE_INTEL;
	nFormat->fileSize	= f_in->Size();
	nFormat->length		= -1;

	delete f_in;

	samplesBufferMutex = new Mutex();

	filter = this;
	infoTrack = nFormat;

	String	 trackTitle;

	errorState = plugin->Play(GetTempFile(inFile));

	if (!errorState)
	{
		Int	 start = clock();

		while (clock() - start < CLOCKS_PER_SEC && samplesBuffer.Size() <= 0) S::System::System::Sleep(0);

		int	 length_ms = -1;
		char	*title = new char [1024];

		plugin->GetFileInfo(NIL, title, &length_ms);

		nFormat->approxLength = (Int) (Float(length_ms) * Float(nFormat->rate * nFormat->channels) / 1000.0);

		trackTitle = title;

		delete [] title;

		plugin->Stop();
	}

	delete samplesBufferMutex;

	if (GetTempFile(inFile) != inFile)
	{
		File	 tempFile(GetTempFile(inFile));

		tempFile.Delete();
	}

	Int	 artistComplete = 0;

	if (trackTitle != GetTempFile(inFile))
	{
		for (Int m = 0; m < trackTitle.Length(); m++)
		{
			if (trackTitle[  m  ] == ' ' &&
			    trackTitle[m + 1] == '-' &&
			    trackTitle[m + 2] == ' ')
			{
				artistComplete = (m += 3);

				nFormat->title = NIL;
			}

			if (!artistComplete)	nFormat->artist[m] = trackTitle[m];
			else			nFormat->title[m - artistComplete] = trackTitle[m];
		}
	}

	if (artistComplete == 0)
	{
		nFormat->artist = NIL;
		nFormat->title = NIL;
	}

	if (nFormat->rate     == 0 ||
	    nFormat->channels == 0 ||
	    nFormat->bits     == 0)
	{
		return NIL;
	}

	return nFormat;
}

void freac::SetInfo(int bitrate, int srate, int stereo, int synched)
{
}

void freac::VSASetInfo(int nch, int srate)
{
}

void freac::VSAAddPCMData(void *PCMData, int nch, int bps, int timestamp)
{
}

int freac::VSAGetMode(int *specNch, int *waveNch)
{
	return 0;
}

void freac::VSAAdd(void *data, int timestamp)
{
}

void freac::SAVSAInit(int latency, int srate)
{
}

void freac::SAVSADeInit()
{
}

void freac::SAAddPCMData(void *PCMData, int nch, int bps, int timestamp)
{
}

int freac::SAGetMode()
{
	return 0;
}

void freac::SAAdd(void *data, int timestamp, int csa)
{
}

int freac::dsp_isactive()
{
	return false;
}

int freac::dsp_dosamples(short int *samples, int numsamples, int bps, int nch, int srate)
{
	return numsamples;
}

int freac::Out_Open(int samplerate, int numchannels, int bitspersamp, int bufferlenms, int prebufferms)
{
	if (filter->infoTrack == NIL) return 0;

	filter->infoTrack->channels	= numchannels;
	filter->infoTrack->rate		= samplerate;
	filter->infoTrack->bits		= bitspersamp;

	return 0;
}

void freac::Out_Close()
{
}

void freac::Out_Flush(int t)
{
}

int freac::Out_Write(char *buf, int len)
{
	filter->samplesBufferMutex->Lock();

	Int	 oSize = filter->samplesBuffer.Size();

	filter->samplesBuffer.Resize(oSize + len);

	memcpy(filter->samplesBuffer + oSize, buf, len);

	filter->samplesBufferMutex->Release();

	return 0;
}

int freac::Out_CanWrite()
{
	return Math::Max(0, 32768 - filter->samplesBuffer.Size());
}

int freac::Out_IsPlaying()
{
	return true;
}

int freac::Out_Pause(int pause)
{
	return 0;
}

void freac::Out_SetVolume(int vol)
{
}

void freac::Out_SetPan(int pan)
{
}

int freac::Out_GetOutputTime()
{
	return 0;
}

int freac::Out_GetWrittenTime()
{
	return 0;
}
