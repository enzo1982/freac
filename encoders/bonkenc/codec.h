#ifndef __BONK_ENC__
#define __BONK_ENC__

#define __THROW_BAD_ALLOC exit(1)

#include "utility.h"
#include <iolib-cxx.h>
#include <vector>

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
  return (a+(1<<(b-1))) >> b;
}

inline int shift_down(int a,int b) {
  return (a>>b)+(a<0?1:0);
}

struct lattice {
  int order;
  vector<int> k, state;

  void init(int _order) {
    order = _order;
    k.resize(order);
    state.resize(order);
    for(int i=0;i<order;i++) {
      state[i] = 0;
      k[i] = 0;
    }
  }

  void dequantize() { 
    for(int i=0;i<order;i++)
      k[i] *= tap_quant[i];
  }

  void init_state() {
    for(int i=order-2;i>=0;i--) {
      int x = state[i], new_x;
      for(int j=0,p=i+1;p<order;j++,p++) {
        new_x = x + shift_down(k[j]*state[p],lattice_shift);
        state[p] += shift_down(k[j]*x,lattice_shift);
        x = new_x;
      }
    }
  }

  int advance_by_error(int error) { // returns value
    int x = error;
    x -= shift_down(k[order-1]*state[order-1],lattice_shift);

    int *k_ptr     = &(k[order-2]),
        *state_ptr = &(state[order-2]);
    for(int i=order-2;i>=0;i--,k_ptr--,state_ptr--) {
      int k_value     = *k_ptr,
          state_value = *state_ptr;
      x -= shift_down(k_value*state_value,lattice_shift);
      state_ptr[1] = state_value+shift_down(k_value*x,lattice_shift);
    }
    
    //Don't drift too far, to avoid overflows 
    if (x >  (sample_factor<<16)) x =  (sample_factor<<16);
    if (x < -(sample_factor<<16)) x = -(sample_factor<<16);

    state[0] = x;

    return x;
  }
};

class BONKencoder
{
	private:
		OutStream	*f_out;
		OutStream	*i_out;
		bitstream_out	 bit_out;
		int		 samples_size;
		int		 channels, rate;
		bool		 lossless;
		bool		 mid_side;
		int		 n_taps;
		int		 down_sampling;
		int		 samples_per_packet;
		double		 quant_level;
		int		 sample_count;
		unsigned char	*infoData;
		int		 data_pos;
		int		 bonk_tag;
		uint32		 length;
		vector<int>	 tail;

		vector< vector<int> > output_samples;
	public:
		void		 begin(OutStream *, uint32, uint32, int, bool, bool, int, int, int, double);
		void		 finish();

		void		 store_packet(vector<int> &);
};

class BONKdecoder
{
	private:
		bitstream_in	 bit_in;
		int		 length;
		int		 length_remaining;
		int		 rate;
		int		 channels;
		bool		 lossless;
		bool		 mid_side;
		int		 n_taps;
		int		 down_sampling;
		int		 samples_per_packet;
		double		 quant_level;
		lattice		 predictor;
		bool		 isBonk;

		vector< vector<int> > predictor_initer;
	public:
		bool		 begin(InStream *, uint32 *, uint32 *, int *);
		void		 finish();

		bool		 read_packet(vector<int> &);
};

#endif
