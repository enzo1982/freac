/* Bonk - lossy/lossless audio compressor
   Copyright (C) 2001  Paul Francis Harrison

   Slightly modified by Robert Kausch for use with BonkEnc.

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the
  Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  675 Mass Ave, Cambridge, MA 02139, USA.

  The author may be contacted at:
    pfh@csse.monash.edu.au
  or
    3 Currajong St., Oakleigh East, 3166, Melbourne, Australia

  See also
    http://yoyo.cc.monash.edu.au/~pfh/
*/

#define __THROW_BAD_ALLOC exit(1)

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <algorithm>
#include "encoder.h"
#include "utility.h"

//Accuracy of fixed point calculations
const int lattice_shift  = 10,
          sample_shift   = 4,
          lattice_factor = 1<<lattice_shift,
          sample_factor  = 1<<sample_shift,

//Maximum allowable taps
          max_tap        = 2048;

//Default quantization level
const double base_quant     = 0.6,

//Amount of bit rate variation
             rate_variation = 3.0;

const int tap_quant[max_tap] = { //int(sqrt(i+1))
   1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4,
   4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6,
   6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
   7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
   9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,10,
  10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
  11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
  11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
  12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,13,13,13,
  13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,14,14,14,14,14,
  14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
  14,14,14,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
  15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,16,16,16,16,16,
  16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
  16,16,16,16,16,16,16,16,17,17,17,17,17,17,17,17,17,17,17,17,
  17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,
  17,17,17,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
  18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
  19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,
  19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,20,
  20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
  20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
  21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,
  21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,
  21,21,21,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,
  22,22,22,22,22,22,22,22,22,22,22,22
};

inline int divide(int a,int b) {
  if (a < 0)
    return -( (-a + b/2)/b );
  else
    return (a + b/2)/b;
}

inline int shift(int a,int b) {
  return a+(1<<b-1) >> b;
}

inline int shift_down(int a,int b) {
  return (a>>b)+(a<0?1:0);
}

// Heavily modified Levinson-Durbin algorithm which
// copes better with quantization, and calculates the
// actual whitened result as it goes.

void modified_levinson_durbin(vector<int> &x, 
                              int channels, bool lossless,
                              vector<int> &k_out) {
  vector<int> state = x;

  for(int i=0;i<k_out.size();i++) {
    int step = (i+1)*channels;

    double xx=0.0, xy=0.0;
    int n         = x.size()-step,
       *x_ptr     = &(x[step]),
       *state_ptr = &(state[0]);
    for(;n>=0;n--,x_ptr++,state_ptr++) {
      double x_value     = *x_ptr,
             state_value = *state_ptr;
      xx += state_value*state_value;
      xy += x_value*state_value;
    }

    int k;
    if (xx == 0.0)
      k = 0;
    else
      k = int(floor( -xy/xx *double(lattice_factor)/double(tap_quant[i]) +0.5 ));

    if (k  > lattice_factor/tap_quant[i]) k =   lattice_factor/tap_quant[i];
    if (-k > lattice_factor/tap_quant[i]) k = -(lattice_factor/tap_quant[i]);

    k_out[i] = k;
    k *= tap_quant[i];

    n         = x.size()-step;
    x_ptr     = &(x[step]);
    state_ptr = &(state[0]);
    for(;n>=0;n--,x_ptr++,state_ptr++) {
      int x_value     = *x_ptr,
          state_value = *state_ptr;
      *x_ptr     = x_value     + shift_down(k*state_value,lattice_shift);
      *state_ptr = state_value + shift_down(k*x_value,lattice_shift);
    }
  }
}

void BONKencoder::begin(OutStream *_f_out, uint32 _length, uint32 _rate, int _channels, bool _lossless, bool _mid_side, int _n_taps, int _down_sampling, int _samples_per_packet, double _quant_level)
{
	f_out			= _f_out;
	channels		= _channels;
	rate			= _rate;
	lossless		= _lossless;
	mid_side		= _mid_side;
	n_taps			= _n_taps;
	down_sampling		= _down_sampling;
	samples_per_packet	= _samples_per_packet;
	quant_level		= _quant_level;
	length			= _length;

	if (n_taps > max_tap)
		throw error("Number of taps is very silly");

	if (mid_side && channels <= 1)
		mid_side = false;

	if (samples_per_packet <= 0)
		throw error("Bad packet size specified");

	samples_size = channels * samples_per_packet * down_sampling;

	infoData = new unsigned char [5 * int((length + samples_size - 1) / samples_size)];

	i_out = new OutStream(STREAM_BUFFER, (void *) infoData, 5 * int((length + samples_size - 1) / samples_size));

	f_out->OutputNumber(0, 1);
	f_out->OutputString("BONK");
	f_out->OutputNumber(0, 1); // version

	f_out->OutputNumber(length, 4);
	f_out->OutputNumber(rate, 4);
	f_out->OutputNumber(channels, 1);
	f_out->OutputNumber(lossless?1:0, 1);
	f_out->OutputNumber(mid_side?1:0, 1);
	f_out->OutputNumber(n_taps, 2);
	f_out->OutputNumber(down_sampling, 1);
	f_out->OutputNumber(samples_per_packet, 2);

	data_pos = 0;

	tail.resize(n_taps * channels);

	for(int i = 0; i < tail.size(); i++) tail[i] = 0;

	output_samples.resize(channels);

	for(int i=0;i<channels;i++)
		output_samples[i].resize(samples_per_packet);

	sample_count = 0;

	bit_out.setup(f_out);
}

void BONKencoder::finish()
{
	if (bit_out.bit_no != 0) data_pos++;

	bit_out.flush();

	f_out->OutputNumber(0, 1);
	f_out->OutputString("INFO");
	f_out->OutputNumber(0, 1); // info tag version

	samples_size = channels * samples_per_packet * down_sampling;

	delete i_out;

	f_out->OutputData((void *) infoData, 5 * int((length + samples_size - 1) / samples_size));

	f_out->OutputNumber(0, 1);
	f_out->OutputString("META");
	f_out->OutputNumber(0, 1);
	f_out->OutputString("bonk");
	f_out->OutputNumber(0, 4);
	f_out->OutputString("info");
	f_out->OutputNumber(data_pos + 22, 4);
	f_out->OutputNumber(30, 4);
	f_out->OutputString("meta");

	f_out->Flush();

	delete [] infoData;
}

void BONKencoder::store_packet(vector<int> &samples)
{
	// save encoder state

	int out_pos = f_out->GetPos();

	if (bit_out.bit_no == 0)
	{
		i_out->OutputNumber(data_pos, 4);
		i_out->OutputNumber(8, 1);
	}
	else
	{
		i_out->OutputNumber(data_pos + 1, 4);
		i_out->OutputNumber(bit_out.bit_no, 1);
	}

	//samples must be correct size (samples_size)

	if (!lossless)
		for(int i=0;i<samples.size();i++)
			samples[i] <<= sample_shift;

	if (mid_side)
		for(int i=0;i<samples.size();i+=channels)
		{
			samples[i]   += samples[i+1];
			samples[i+1] -= shift(samples[i],1);
		}  

	vector<int> window(tail.size()*2+samples_size);
	int *ptr = &(window[0]);

	for(int i=0;i<tail.size();i++)
		*(ptr++) = tail[i];

	for(int i=0;i<samples_size;i++)
		*(ptr++) = samples[i];
   
	for(int i=0;i<tail.size();i++)
		*(ptr++) = 0;
    
	for(int i=0;i<tail.size();i++)
		tail[i] = samples[samples_size-tail.size()+i];

	vector<int> k(n_taps);
	modified_levinson_durbin(window,channels,lossless,k);

	write_list(k, false, bit_out);

	for(int channel=0;channel<channels;channel++)
	{
		ptr = &(window[tail.size()+channel]);

		for(int i=0;i<samples_per_packet;i++)
		{
			int sum = 0;

			for(int j=0;j<down_sampling;j++,ptr += channels)
				sum += *ptr;

			output_samples[channel][i] = sum;
		}
	}

	int quant;

	if (!lossless)
	{
		double energy1 = 0.0;
		double energy2 = 0.0;

		for(int channel=0;channel<channels;channel++)
		{
			for(int i=0;i<samples_per_packet;i++)
			{
				double sample = output_samples[channel][i];
				energy2 += sample*sample;
				energy1 += fabs(sample);
			}
		}

		energy2 = sqrt(energy2/(channels*samples_per_packet));
		energy1 = sqrt(2.0)*energy1/(channels*samples_per_packet);

		if (energy2 > energy1) energy2 += (energy2-energy1)*rate_variation;
      
		quant = int(base_quant * quant_level * energy2 / sample_factor);

		if (quant < 1) 
			quant = 1;
		if (quant > 65535)
			quant = 65535;

		bit_out.write_uint(quant,16);

		quant *= sample_factor;
	}

	for(int channel=0;channel<channels;channel++)
	{
		if (!lossless)
			for(int i=0;i<samples_per_packet;i++)
				output_samples[channel][i] = divide(output_samples[channel][i],quant);

		write_list(output_samples[channel],true, bit_out);
	}

	data_pos += (f_out->GetPos() - out_pos);

	sample_count += samples_size;
}
