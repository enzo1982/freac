 /* Bonk - Lossy/lossless audio codec
  * Copyright (C) 2001 Paul Francis Harrison
  * Portions Copyright (C) 2004-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __BONK__
#define __BONK__

extern "C"
{
	__declspec (dllexport) void		*bonk_encoder_create();
	__declspec (dllexport) bool		 bonk_encoder_init(void *, unsigned int, unsigned int, int, bool, bool, int, int, int, double);
	__declspec (dllexport) int		 bonk_encoder_encode_packet(void *, signed short *, int, unsigned char *, int);
	__declspec (dllexport) int		 bonk_encoder_finish(void *, unsigned char *, int);
	__declspec (dllexport) bool		 bonk_encoder_close(void *);

	__declspec (dllexport) int		 bonk_encoder_get_sample_count(void *);
	__declspec (dllexport) int		 bonk_encoder_get_sample_count_offset(void *);

	__declspec (dllexport) bool		 bonk_encoder_set_id3_data(void *, unsigned char *, int);

	__declspec (dllexport) void		*bonk_decoder_create();
	__declspec (dllexport) bool		 bonk_decoder_init(void *, unsigned char *, int, unsigned int *, unsigned int *, int *);
	__declspec (dllexport) int		 bonk_decoder_decode_packet(void *, unsigned char *, int, signed short *, int);
	__declspec (dllexport) int		 bonk_decoder_finish(void *);
	__declspec (dllexport) bool		 bonk_decoder_close(void *);

	__declspec (dllexport) bool		 bonk_decoder_get_id3_data(void *, unsigned char **, int *);

	__declspec (dllexport) bool		 bonk_decoder_init_seektable(void *, unsigned char *, int);
	__declspec (dllexport) bool		 bonk_decoder_seek_to(void *, int);

	__declspec (dllexport) const char	*bonk_get_version_string();
}

#endif
