#ifndef __BONK__
#define __BONK__

#include <vector>
#include <iolib-cxx.h>
#include "utility.h"

extern "C"
{
	__declspec (dllexport) void		*bonk_create_encoder(OutStream *, uint32, uint32, int, bool, bool, int, int, int, double);
	__declspec (dllexport) bool		 bonk_close_encoder(void *);
	__declspec (dllexport) bool		 bonk_encode_packet(void *, vector<int> &);

	__declspec (dllexport) void		*bonk_create_decoder(InStream *, uint32 *, uint32 *, int *);
	__declspec (dllexport) bool		 bonk_close_decoder(void *);
	__declspec (dllexport) bool		 bonk_decode_packet(void *, vector<int> &);

	__declspec (dllexport) const char	*bonk_get_version_string();
}

#endif
