 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-winamp.h>

#include <dllinterfaces.h>

int		 channels		= 0;
int		 rate			= 0;
int		 bits			= 16;

int		 get_more_samples	= 0;
int		 n_samples		= 0;
Buffer<char>	 sampleBuffer;

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
void		 SetVolume(int);
void		 SetPan(int);

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

BonkEnc::FilterInWinamp::FilterInWinamp(Config *config, Track *format, In_Module *iPlugin) : InputFilter(config, format)
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
	plugin->SetVolume		= SetVolume;
	plugin->SetPan			= SetPan;

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

	channels		= 0;
	rate			= 0;
	bits			= 16;

	get_more_samples	= 0;
	n_samples		= 0;

	packageSize		= 0;
}

BonkEnc::FilterInWinamp::~FilterInWinamp()
{
	delete plugin->outMod;
}

Bool BonkEnc::FilterInWinamp::Activate()
{
	plugin->Play(format->origFilename);

	return true;
}

Bool BonkEnc::FilterInWinamp::Deactivate()
{
	plugin->Stop();

	return true;
}

Int BonkEnc::FilterInWinamp::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	get_more_samples	= 32768;
	n_samples		= 0;

	Int	 count = 0;

	while (get_more_samples > 0)
	{
		if (++count == 10)
		{
			if (n_samples == 0)	return -1;
			else			break;
		}

		Sleep(10);
	}

	get_more_samples = 0;

	size = n_samples * (bits / 8) * channels;

	data.Resize(size);

	memcpy(data, sampleBuffer, size);

	return size;
}

BonkEnc::Track *BonkEnc::FilterInWinamp::GetFileInfo(const String &inFile)
{
	Track		*nFormat = new Track;
	InStream	*f_in = new InStream(STREAM_FILE, inFile, IS_READONLY);

	nFormat->order		= BYTE_INTEL;
	nFormat->fileSize	= f_in->Size();

	delete f_in;

	plugin->Play(inFile);

	get_more_samples = 1;

	while (rate == 0) Sleep(10);

	int	 length_ms;
	char	*title = new char [1024];

	plugin->GetFileInfo(NIL, title, &length_ms);

	plugin->Stop();

	nFormat->rate		= rate;
	nFormat->channels	= channels;
	nFormat->bits		= bits;

	nFormat->length		= (Int) (Float(length_ms) * Float(rate * channels) / 1000.0);

	String	 trackTitle = title;

	delete [] title;

	Int	 artistComplete = 0;

	if (trackTitle != inFile)
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

	return nFormat;
}

void SetInfo(int bitrate, int srate, int stereo, int synched)
{
}

void VSASetInfo(int nch, int srate)
{
	/* Some plugins pass the sampling rate as the first   *
	 * parameter, so we just assume that the larger value *
	 * is the sampling rate, the other is the number of   *
	 * channels.					      */

	channels	= Math::Min((Int) nch, srate);
	rate		= Math::Max((Int) nch, srate);
}

void VSAAddPCMData(void *PCMData, int nch, int bps, int timestamp)
{
}

int VSAGetMode(int *specNch, int *waveNch)
{
	return 0;
}

void VSAAdd(void *data, int timestamp)
{
}

void SAVSAInit(int latency, int srate)
{
}

void SAVSADeInit()
{
}

void SAAddPCMData(void *PCMData, int nch, int bps, int timestamp)
{
}

int SAGetMode()
{
	return 0;
}

void SAAdd(void *data, int timestamp, int csa)
{
}

int dsp_isactive()
{
	return get_more_samples;
}

int dsp_dosamples(short int *samples, int numsamples, int bps, int nch, int srate)
{
	if (n_samples == 0) sampleBuffer.Resize(32768 * (bps / 8) * nch);

	memcpy(sampleBuffer + n_samples * (bps / 8) * nch, samples, numsamples * (bps / 8) * nch);

	get_more_samples -= numsamples;
	n_samples	 += numsamples;

	if (get_more_samples <= 8192) get_more_samples = 0;

	return numsamples;
}

void SetVolume(int vol)
{
}

void SetPan(int pan)
{
}

int Out_Open(int samplerate, int numchannels, int bitspersamp, int bufferlenms, int prebufferms)
{
	bits = bitspersamp;

	return 25;
}

void Out_Close()
{
}

void Out_Flush(int t)
{
}

int Out_Write(char *buf, int len)
{
	return 0;
}

int Out_CanWrite()
{
	return get_more_samples;
}

int Out_IsPlaying()
{
	return 1;
}

int Out_Pause(int pause)
{
	return 0;
}

void Out_SetVolume(int vol)
{
}

void Out_SetPan(int pan)
{
}

int Out_GetOutputTime()
{
	return 0;
}

int Out_GetWrittenTime()
{
	return 0;
}
