const char *version = "0.9";

#include "codec.h"
#include "bonk.h"

void *bonk_create_encoder(OutStream *f_out, uint32 length, uint32 _rate, int _channels, bool _lossless, bool _mid_side, int _n_taps, int _down_sampling, int _samples_per_packet, double _quant_level)
{
	BONKencoder *encoder = new BONKencoder();

	encoder->begin(f_out, length, _rate, _channels, _lossless, _mid_side, _n_taps, _down_sampling, _samples_per_packet, _quant_level);

	return (void *) encoder;
}

bool bonk_close_encoder(void *encoder)
{
	((BONKencoder *) encoder)->finish();

	delete (BONKencoder *) encoder;

	return true;
}

bool bonk_encode_packet(void *encoder, vector<int> &samples)
{
	((BONKencoder *) encoder)->store_packet(samples);

	return true;
}

void *bonk_create_decoder(InStream *f_in, uint32 *length, uint32 *_rate, int *_channels)
{
	BONKdecoder *decoder = new BONKdecoder();

	decoder->begin(f_in, length, _rate, _channels);

	return (void *) decoder;
}

bool bonk_close_decoder(void *decoder)
{
	((BONKdecoder *) decoder)->finish();

	delete (BONKdecoder *) decoder;

	return true;
}

bool bonk_decode_packet(void *decoder, vector<int> &samples)
{
	return ((BONKdecoder *) decoder)->read_packet(samples);
}

const char *bonk_get_version_string()
{
	return version;
}
