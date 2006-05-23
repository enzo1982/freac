 /* Bonk - Lossy/lossless audio codec
  * Copyright (C) 2001 Paul Francis Harrison
  * Portions Copyright (C) 2004-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This header file is in the public domain; you can redistribute
  * it and/or modify it freely without any limitations.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __BONK__
#define __BONK__

#ifdef __WIN32__
#	ifdef __BONK_LIBRARY_BUILD__
#		define BONKAPI __declspec (dllexport) _stdcall
#	else
#		define BONKAPI __declspec (dllimport) _stdcall
#	endif
#else
#	define BONKAPI
#endif

extern "C"
{
/* void *bonk_encoder_create()
 * Creates a Bonk encoder context.
 *
 * Call this function as the first one and use the returned context
 * handle as the first argument to any other bonk_encoder_* call.
 */
	BONKAPI void		*bonk_encoder_create();

/* bool bonk_encoder_init(void *encoder, unsigned int length, unsigned int rate, int channels, bool lossless, bool mid_side, int n_taps, int down_sampling, int samples_per_packet, double quantization)
 * Initializes the Bonk encoder.
 *
 * This function initializes the encoder and sets all encoding parameters.
 */
	BONKAPI bool		 bonk_encoder_init(void *, unsigned int, unsigned int, int, bool, bool, int, int, int, double);

/* int bonk_encoder_encode_packet(void *encoder, signed short *samples, int n_samples, unsigned char *out_buffer, int out_size)
 * Writes a packet of audio samples to the encoded Bonk stream.
 *
 * The function returns the number of bytes written to out_buffer.
 */

	BONKAPI int		 bonk_encoder_encode_packet(void *, signed short *, int, unsigned char *, int);

/* int bonk_encoder_finish(void *encoder, unsigned char *out_buffer, int out_size)
 * Finishes encoding of an audio stream.
 *
 * Call bonk_encoder_finish after you have written all samples using
 * bonk_encoder_encode_packet. The function will write any remaining
 * data to the buffer given as a parameter. The number of bytes written
 * will be returned.
 */
	BONKAPI int		 bonk_encoder_finish(void *, unsigned char *, int);

/* bool bonk_encoder_close(void *encoder)
 * Frees a Bonk encoder context.
 *
 * Call this function after you have finished using the encoder context.
 */
	BONKAPI bool		 bonk_encoder_close(void *);

/* int bonk_encoder_get_sample_count(void *encoder)
 * Returns the number of samples processed by the encoder.
 *
 * You have to write the number of samples to the output file at
 * the position indicated by bonk_encoder_get_sample_count_offset.
 */
	BONKAPI int		 bonk_encoder_get_sample_count(void *);

/* int bonk_encoder_get_sample_count_offset(void *encoder)
 * Returns the offset of the samples count in the output file.
 *
 * You have to write the number of samples to this position.
 */
	BONKAPI int		 bonk_encoder_get_sample_count_offset(void *);

/* bool bonk_encoder_set_id3_data(void *encoder, unsigned char *id3data, int bytes)
 * Set an ID3v2 tag to write to the output stream.
 *
 * This function has to be called before bonk_encoder_init if you want
 * to add ID3v2 information to your Bonk file.
 */
	BONKAPI bool		 bonk_encoder_set_id3_data(void *, unsigned char *, int);

/* void *bonk_decoder_create()
 * Creates a Bonk decoder context.
 *
 * Call this function as the first one and use the returned context
 * handle as the first argument to any other bonk_decoder_* call.
 */
	BONKAPI void		*bonk_decoder_create();

/* bool bonk_decoder_init(void *decoder, unsigned char *in_buffer, int in_bytes, unsigned int *length, unsigned int *rate, int *channels)
 * Initializes the Bonk audio decoder.
 *
 * This function initializes the decoder. You must provide enough bytes
 * from the beginning of the file to include the header of the BONK tag.
 * Be aware that a Bonk file might have an ID3 tag prepended before the
 * BONK tag.
 * The function will write the number of samples, sampling rate and the
 * number of channels to the supplied pointers.
 */
	BONKAPI bool		 bonk_decoder_init(void *, unsigned char *, int, unsigned int *, unsigned int *, int *);

/* int bonk_decoder_decode_packet(void *decoder, unsigned char *in_buffer, int in_bytes, signed short *samples, int samples_size)
 * Decodes a packet of samples from the Bonk audio stream.
 *
 * This function decodes one block at once. You should provide enough data
 * in in_buffer. If you have provided all data to the Bonk decoder, but there
 * are still packets left to decode, simply set in_bytes to 0 and continue
 * calling bonk_decoder_decode_packet until all packets are read.
 * The function will return the number of samples written to samples.
 */
	BONKAPI int		 bonk_decoder_decode_packet(void *, unsigned char *, int, signed short *, int);

/* int bonk_decoder_finish(void *decoder)
 * Finishes decoding of an audio stream.
 *
 * bonk_decoder_finish should be called after all blocks of a Bonk stream
 * have been processed. It should return 0.
 */
	BONKAPI int		 bonk_decoder_finish(void *);

/* bool bonk_decoder_close(void *decoder)
 * Frees a Bonk decoder context.
 *
 * Call this function after you have finished using the decoder context.
 */
	BONKAPI bool		 bonk_decoder_close(void *);

/* bool bonk_decoder_get_id3_data(void *decoder, unsigned char **id3data, int *bytes)
 * Get ID3v2 tag from a Bonk stream.
 *
 * This function will return the raw ID3v2 tag of the Bonk audio stream.
 * The tag data is written to id3data, the number of bytes in id3data is
 * returned in the parameter bytes.
 */
	BONKAPI bool		 bonk_decoder_get_id3_data(void *, unsigned char **, int *);

/* bool bonk_decoder_init_seektable(void *decoder, unsigned char *buffer, int bytes)
 * Initialize the seektable to prepare for seeking.
 *
 * This function must be called before bonk_decoder_seek_to can be used.
 * The function will look for an INFO tag in the supplied buffer.
 * You should provide enough data from the end of the Bonk file to
 * include the whole INFO tag.
 */
	BONKAPI bool		 bonk_decoder_init_seektable(void *, unsigned char *, int);

/* bool bonk_decoder_seek_to(void *decoder, int sample)
 * Seek to the given sample number.
 *
 * The function will seek to the given sample number. It only works after
 * the seektable has been initialized by bonk_decoder_init_seektable.
 * Seeking is not exact but will seek to the nearest packet start.
 */
	BONKAPI bool		 bonk_decoder_seek_to(void *, int);

/* const char *bonk_get_version_string()
 * Returns the version number of the Bonk library as a string.
 *
 * The result will be something like "0.10" or "1.0 beta 2".
 */
	BONKAPI const char	*bonk_get_version_string();
}

#endif
