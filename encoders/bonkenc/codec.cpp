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

#include <smooth/array.h>
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
#include "codec.h"
#include "utility.h"

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

	if (length == 0)	infoData = new unsigned char [524288];
	else			infoData = new unsigned char [5 * int((length + samples_size - 1) / samples_size)];

	if (length == 0)	i_out = new OutStream(STREAM_BUFFER, (void *) infoData, 524288);
	else			i_out = new OutStream(STREAM_BUFFER, (void *) infoData, 5 * int((length + samples_size - 1) / samples_size));

	bonk_tag = f_out->GetPos();

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

	for(int j = 0; j < channels; j++)
		output_samples[j].resize(samples_per_packet);

	sample_count = 0;

	bit_out.setup(f_out);
}

void BONKencoder::finish()
{
	if (bit_out.bit_no != 0) data_pos++;

	bit_out.flush();

	f_out->Flush();

	f_out->OutputNumber(data_pos + 30, 4);
	f_out->OutputString("bonk");

	f_out->OutputNumber(0, 1);
	f_out->OutputString("INFO");
	f_out->OutputNumber(0, 1); // info tag version

	samples_size = channels * samples_per_packet * down_sampling;

	delete i_out;

	f_out->OutputData((void *) infoData, 5 * int((sample_count + samples_size - 1) / samples_size));

	f_out->OutputNumber(14 + 5 * int((sample_count + samples_size - 1) / samples_size), 4);
	f_out->OutputString("info");

	if (length == 0)
	{
		f_out->Flush();
		f_out->Seek(bonk_tag + 6);
		f_out->OutputNumber(sample_count, 4);
	}

	delete [] infoData;
}

void BONKencoder::store_packet(vector<int> &samples)
{
	sample_count += samples.size();

	// save encoder state

	int out_pos = f_out->GetPos();
	int i;
	int channel;

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
		for (i = 0; i < samples.size(); i++)
			samples[i] <<= sample_shift;

	if (mid_side)
		for (i = 0; i < samples.size(); i += channels)
		{
			samples[i]	+= samples[i + 1];
			samples[i + 1]	-= shift(samples[i], 1);
		}  

	vector<int> window(tail.size() * 2 + samples_size);
	int *ptr = &(window[0]);

	for (i = 0; i < tail.size(); i++)
		*(ptr++) = tail[i];

	for (i = 0; i < samples_size; i++)
		*(ptr++) = samples[i];
   
	for (i = 0; i < tail.size(); i++)
		*(ptr++) = 0;
    
	for (i = 0; i < tail.size(); i++)
		tail[i] = samples[samples_size - tail.size() + i];

	vector<int> k(n_taps);
	modified_levinson_durbin(window, channels, lossless, k);

	write_list(k, false, bit_out);

	for(channel=0;channel<channels;channel++)
	{
		ptr = &(window[tail.size()+channel]);

		for(i=0;i<samples_per_packet;i++)
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
			for(i=0;i<samples_per_packet;i++)
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

	for(channel=0;channel<channels;channel++)
	{
		if (!lossless)
			for(i=0;i<samples_per_packet;i++)
				output_samples[channel][i] = divide(output_samples[channel][i],quant);

		write_list(output_samples[channel],true, bit_out);
	}

	data_pos += (f_out->GetPos() - out_pos);
}

bool BONKdecoder::begin(InStream *f_in, uint32 *_length, uint32 *_rate, int *_channels)
{
	isBonk = false;

	bit_in.f_in = f_in;

	if (bit_in.f_in->Size() == 0) return false;

	vector<char>	 buffer;
	int		 i;

	for(i = 0; i < 6; i++) buffer.push_back(bit_in.f_in->InputNumber(1));

	for(i = 0; i < bit_in.f_in->Size() - 6; i++)
	{
		if (buffer[buffer.size()-6] == 0   &&
		    buffer[buffer.size()-5] == 'B' &&
		    buffer[buffer.size()-4] == 'O' &&
		    buffer[buffer.size()-3] == 'N' &&
		    buffer[buffer.size()-2] == 'K' &&
		    buffer[buffer.size()-1] == 0)
		{
			isBonk = true;
			break;
		}

		buffer.push_back(bit_in.f_in->InputNumber(1));
	}

	if (!isBonk) return false;

	length			= uint32(bit_in.f_in->InputNumber(4));
	rate			= uint32(bit_in.f_in->InputNumber(4)); 
	channels		= uint8(bit_in.f_in->InputNumber(1));
	lossless		= uint8(bit_in.f_in->InputNumber(1)) != 0;
	mid_side		= uint8(bit_in.f_in->InputNumber(1)) != 0;
	n_taps			= uint16(bit_in.f_in->InputNumber(2));
	down_sampling		= uint8(bit_in.f_in->InputNumber(1));
	samples_per_packet	= uint16(bit_in.f_in->InputNumber(2));

	*_length = length;
	*_rate = rate;
	*_channels = channels;

	predictor.init(n_taps);
	predictor_initer.resize(channels);

	for(i = 0; i < channels; i++)
	{
		predictor_initer[i].resize(n_taps);

		for(int j = 0; j < n_taps; j++)
		{
			predictor_initer[i][j] = 0;
		}
	}

	bit_in.setup();

	length_remaining	= length;

	return true;
}

void BONKdecoder::finish()
{
}

bool BONKdecoder::read_packet(vector<int> &samples)
{
	if (!isBonk) return false;
	if (!length_remaining) return false;

	samples.resize(samples_per_packet * down_sampling * channels);

	vector<int> input_samples(samples_per_packet);

	read_list(predictor.k, false, bit_in);

	predictor.dequantize();

	int quant = (lossless ? 1 : bit_in.read_uint(16) * sample_factor);

	for(int channel = 0; channel < channels; channel++)
	{
		int *sample = &(samples[channel]);

		predictor.state = predictor_initer[channel];
		predictor.init_state();

		read_list(input_samples, true, bit_in);
      
		for(int i = 0; i < samples_per_packet; i++)
		{
			for(int j = 0; j < down_sampling - 1; j++)
			{
				*sample = predictor.advance_by_error(0);
				sample += channels;
			}

			*sample = predictor.advance_by_error(input_samples[i] * quant);
			sample += channels;
		}

		for(int j = 0; j < n_taps; j++)
		{
			predictor_initer[channel][j] = samples[samples.size() - channels + channel - j * channels];
		}
	}

	if (mid_side)
	{
		for(int i = 0; i < (int) samples.size(); i += channels)
		{
			samples[i + 1]	+= shift(samples[i], 1);
			samples[i]	-= samples[i + 1];
		}  
	}

	if (!lossless)
	{
		for(int i = 0; i < (int) samples.size(); i++)
		{
			samples[i] = shift(samples[i], sample_shift);
		}
	}

	if (length_remaining < (int) samples.size())
	{
		samples.resize(length_remaining);
		length_remaining = 0;
	}
	else
	{
		length_remaining -= samples.size();
	}

	return true;
}
