#ifndef __BONK_ENC__
#define __BONK_ENC__

#include "utility.h"
#include <iolib-cxx.h>
#include <vector>

class BONKencoder
{
	private:
		OutStream	*f_out;
		OutStream	*i_out;
		bitstream_out	 bit_out;
		int		 channels, rate;
		bool		 lossless;
		bool		 mid_side;
		int		 n_taps;
		int		 down_sampling, samples_per_packet;
		double		 quant_level;
		int		 sample_count;
		unsigned char	*infoData;
		int		 data_pos;
		uint32		 length;
		vector<int>	 tail;

		vector<vector<int> >	 output_samples;
	public:
		int		 samples_size;
		void		 begin(OutStream *, uint32, uint32, int, bool, bool, int, int, int, double);
		void		 finish();
		void		 store_packet(vector<int> &);
};

#endif
