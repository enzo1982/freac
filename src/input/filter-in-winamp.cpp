 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <iolib-cxx.h>
#include <input/filter-in-winamp.h>

int	 channels = 0;
int	 rate = 0;

int	 get_more_samples = 0;
int	 n_samples = 0;
char	*sample_buffer;

void	 SetInfo(int, int, int, int);
void	 VSASetInfo(int, int);
void	 VSAAddPCMData(void *, int, int, int);
int	 VSAGetMode(int *, int *);
void	 VSAAdd(void *, int);
void	 SAVSAInit(int, int);
void	 SAVSADeInit();
void	 SAAddPCMData(void *, int, int, int);
int	 SAGetMode();
void	 SAAdd(void *, int, int);
int	 dsp_isactive();
int	 dsp_dosamples(short int *, int, int, int, int);
void	 SetVolume(int);
void	 SetPan(int);

int	 Out_Open(int, int, int, int, int);
void	 Out_Close();
void	 Out_Flush(int);
int	 Out_Write(char *, int);
int	 Out_CanWrite();
int	 Out_IsPlaying();
void	 Out_SetVolume(int);
void	 Out_SetPan(int);

FilterInWinamp::FilterInWinamp(bonkEncConfig *config, In_Module *iPlugin) : InputFilter(config)
{
	plugin = iPlugin;
	setup = False;

	plugin->SetInfo = SetInfo;
	plugin->VSASetInfo = VSASetInfo;
	plugin->VSAAddPCMData = VSAAddPCMData;
	plugin->VSAGetMode = VSAGetMode;
	plugin->VSAAdd = VSAAdd;
	plugin->SAVSAInit = SAVSAInit;
	plugin->SAVSADeInit = SAVSADeInit;
	plugin->SAAddPCMData = SAAddPCMData;
	plugin->SAGetMode = SAGetMode;
	plugin->SAAdd = SAAdd;
	plugin->dsp_isactive = dsp_isactive;
	plugin->dsp_dosamples = dsp_dosamples;
	plugin->SetVolume = SetVolume;
	plugin->SetPan = SetPan;

	plugin->outMod = new Out_Module();

	plugin->outMod->Open = Out_Open;
	plugin->outMod->Close = Out_Close;
	plugin->outMod->Flush = Out_Flush;
	plugin->outMod->Write = Out_Write;
	plugin->outMod->CanWrite = Out_CanWrite;
	plugin->outMod->IsPlaying = Out_IsPlaying;
	plugin->outMod->SetVolume = Out_SetVolume;
	plugin->outMod->SetPan = Out_SetPan;

	packageSize = 0;
}

FilterInWinamp::~FilterInWinamp()
{
	delete plugin->outMod;
}

bool FilterInWinamp::Activate()
{
	return true;
}

bool FilterInWinamp::Deactivate()
{
	plugin->Stop();

	return true;
}

int FilterInWinamp::ReadData(unsigned char **data, int size)
{
	if (!setup)
	{
		setup = True;

		char  file[MAX_PATH];

		strcpy(file, driver->GetStreamID());

		plugin->Play(file);
	}

	get_more_samples = 32768;
	n_samples = 0;

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

	size = n_samples * 2 * channels;

	*data = new unsigned char [size];

	memcpy((void *) *data, (void *) sample_buffer, size);

	delete [] sample_buffer;

	return size;
}

bonkFormatInfo *FilterInWinamp::GetFileInfo(String inFile)
{
	bonkFormatInfo	*nFormat = new bonkFormatInfo;
	InStream	*f_in = new InStream(STREAM_FILE, inFile, IS_READONLY);

	nFormat->order = BYTE_INTEL;
	nFormat->bits = 16;
	nFormat->trackInfo = NIL;
	nFormat->fileSize = f_in->Size();

	delete f_in;

	plugin->Play(inFile);
	plugin->Stop();

	nFormat->rate = rate;
	nFormat->channels = channels;

	int	 length_ms;

	plugin->GetFileInfo(inFile, NIL, &length_ms);

	nFormat->length = (Int) (Float(length_ms) * Float(rate * channels) / 1000.0);

	return nFormat;
}

void SetInfo(int bitrate, int srate, int stereo, int synched)
{
}

void VSASetInfo(int srate, int nch)
{
	channels = nch;
	rate = srate;
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
	if (n_samples == 0) sample_buffer = new char [32768 * (bps / 8) * nch];

	memcpy((void *) (sample_buffer + n_samples * (bps / 8) * nch), (void *) samples, numsamples * (bps / 8) * nch);

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

void Out_SetVolume(int vol)
{
}

void Out_SetPan(int pan)
{
}
