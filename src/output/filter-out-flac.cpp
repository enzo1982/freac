 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <output/filter-out-flac.h>
#include <utilities.h>
#include <dllinterfaces.h>

namespace BonkEnc
{
	FLAC__StreamEncoderWriteStatus		 FLACSeekableStreamEncoderWriteCallback(const FLAC__SeekableStreamEncoder *, const FLAC__byte[], unsigned, unsigned, unsigned, void *);
	FLAC__SeekableStreamEncoderSeekStatus	 FLACSeekableStreamEncoderSeekCallback(const FLAC__SeekableStreamEncoder *, FLAC__uint64, void *);
	FLAC__SeekableStreamEncoderTellStatus	 FLACSeekableStreamEncoderTellCallback(const FLAC__SeekableStreamEncoder *, FLAC__uint64 *, void *);
};

BonkEnc::FilterOutFLAC::FilterOutFLAC(Config *config, Track *format) : OutputFilter(config, format)
{
	if (format->channels > 2)
	{
		Utilities::ErrorMessage("BonkEnc does not support more than 2 channels!");

		error = 1;

		return;
	}
}

BonkEnc::FilterOutFLAC::~FilterOutFLAC()
{
}

Bool BonkEnc::FilterOutFLAC::Activate()
{
	encoder = ex_FLAC__seekable_stream_encoder_new();

	ex_FLAC__seekable_stream_encoder_set_write_callback(encoder, &FLACSeekableStreamEncoderWriteCallback);
	ex_FLAC__seekable_stream_encoder_set_seek_callback(encoder, &FLACSeekableStreamEncoderSeekCallback);
	ex_FLAC__seekable_stream_encoder_set_tell_callback(encoder, &FLACSeekableStreamEncoderTellCallback);

	ex_FLAC__seekable_stream_encoder_set_client_data(encoder, this);

	numMetadata = 0;

	if (currentConfig->enable_vctags)
	{
		char	*prevOutFormat = String::SetOutputFormat(currentConfig->vctag_encoding);

		FLAC__StreamMetadata				*vorbiscomment = ex_FLAC__metadata_object_new(FLAC__METADATA_TYPE_VORBIS_COMMENT);
		FLAC__StreamMetadata_VorbisComment_Entry	 comment;

		metadata[numMetadata++] = vorbiscomment;

		if (currentConfig->default_comment != NIL)
		{
			ex_FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(&comment, "DESCRIPTION", currentConfig->default_comment);
			ex_FLAC__metadata_object_vorbiscomment_append_comment(vorbiscomment, comment, false);
		}

		if (format->artist != NIL || format->title != NIL)
		{
			FLAC__StreamMetadata_VorbisComment_Entry	 artist;
			FLAC__StreamMetadata_VorbisComment_Entry	 title;
			FLAC__StreamMetadata_VorbisComment_Entry	 album;
			FLAC__StreamMetadata_VorbisComment_Entry	 genre;
			FLAC__StreamMetadata_VorbisComment_Entry	 date;
			FLAC__StreamMetadata_VorbisComment_Entry	 track;

			if (format->artist != NIL)
			{
				ex_FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(&artist, "ARTIST", format->artist);
				ex_FLAC__metadata_object_vorbiscomment_append_comment(vorbiscomment, artist, false);
			}

			if (format->title != NIL)
			{
				ex_FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(&title, "TITLE", format->title);
				ex_FLAC__metadata_object_vorbiscomment_append_comment(vorbiscomment, title, false);
			}

			if (format->album != NIL)
			{
				ex_FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(&album, "ALBUM", format->album);
				ex_FLAC__metadata_object_vorbiscomment_append_comment(vorbiscomment, album, false);
			}

			if (format->genre != NIL)
			{
				ex_FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(&genre, "GENRE", format->genre);
				ex_FLAC__metadata_object_vorbiscomment_append_comment(vorbiscomment, genre, false);
			}

			if (format->year > 0)
			{
				ex_FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(&date, "DATE", String::FromInt(format->year));
				ex_FLAC__metadata_object_vorbiscomment_append_comment(vorbiscomment, date, false);
			}

			if (format->track > 0)
			{
				ex_FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(&track, "TRACKNUMBER", String(format->track < 10 ? "0" : "").Append(String::FromInt(format->track)));
				ex_FLAC__metadata_object_vorbiscomment_append_comment(vorbiscomment, track, false);
			}
		}

		String::SetOutputFormat(prevOutFormat);
	}

	FLAC__StreamMetadata	*padding = ex_FLAC__metadata_object_new(FLAC__METADATA_TYPE_PADDING);

	padding->length = 4096;

	metadata[numMetadata++] = padding;

	ex_FLAC__seekable_stream_encoder_set_metadata(encoder, metadata, numMetadata);
	ex_FLAC__seekable_stream_encoder_set_channels(encoder, format->channels);
	ex_FLAC__seekable_stream_encoder_set_sample_rate(encoder, format->rate);
	ex_FLAC__seekable_stream_encoder_set_bits_per_sample(encoder, format->bits);

	switch (currentConfig->flac_preset)
	{
		case 0:
			ex_FLAC__seekable_stream_encoder_set_streamable_subset(encoder, true);
			ex_FLAC__seekable_stream_encoder_set_do_mid_side_stereo(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_loose_mid_side_stereo(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_blocksize(encoder, 1152);
			ex_FLAC__seekable_stream_encoder_set_max_lpc_order(encoder, 0);
			ex_FLAC__seekable_stream_encoder_set_qlp_coeff_precision(encoder, 0);
			ex_FLAC__seekable_stream_encoder_set_do_qlp_coeff_prec_search(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_do_exhaustive_model_search(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_min_residual_partition_order(encoder, 2);
			ex_FLAC__seekable_stream_encoder_set_max_residual_partition_order(encoder, 2);

			break;
		case 1:
			ex_FLAC__seekable_stream_encoder_set_streamable_subset(encoder, true);
			ex_FLAC__seekable_stream_encoder_set_do_mid_side_stereo(encoder, true);
			ex_FLAC__seekable_stream_encoder_set_loose_mid_side_stereo(encoder, true);
			ex_FLAC__seekable_stream_encoder_set_blocksize(encoder, 1152);
			ex_FLAC__seekable_stream_encoder_set_max_lpc_order(encoder, 0);
			ex_FLAC__seekable_stream_encoder_set_qlp_coeff_precision(encoder, 0);
			ex_FLAC__seekable_stream_encoder_set_do_qlp_coeff_prec_search(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_do_exhaustive_model_search(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_min_residual_partition_order(encoder, 2);
			ex_FLAC__seekable_stream_encoder_set_max_residual_partition_order(encoder, 2);

			break;
		case 2:
			ex_FLAC__seekable_stream_encoder_set_streamable_subset(encoder, true);
			ex_FLAC__seekable_stream_encoder_set_do_mid_side_stereo(encoder, true);
			ex_FLAC__seekable_stream_encoder_set_loose_mid_side_stereo(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_blocksize(encoder, 1152);
			ex_FLAC__seekable_stream_encoder_set_max_lpc_order(encoder, 0);
			ex_FLAC__seekable_stream_encoder_set_qlp_coeff_precision(encoder, 0);
			ex_FLAC__seekable_stream_encoder_set_do_qlp_coeff_prec_search(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_do_exhaustive_model_search(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_min_residual_partition_order(encoder, 0);
			ex_FLAC__seekable_stream_encoder_set_max_residual_partition_order(encoder, 3);

			break;
		case 3:
			ex_FLAC__seekable_stream_encoder_set_streamable_subset(encoder, true);
			ex_FLAC__seekable_stream_encoder_set_do_mid_side_stereo(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_loose_mid_side_stereo(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_blocksize(encoder, 4608);
			ex_FLAC__seekable_stream_encoder_set_max_lpc_order(encoder, 6);
			ex_FLAC__seekable_stream_encoder_set_qlp_coeff_precision(encoder, 0);
			ex_FLAC__seekable_stream_encoder_set_do_qlp_coeff_prec_search(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_do_exhaustive_model_search(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_min_residual_partition_order(encoder, 3);
			ex_FLAC__seekable_stream_encoder_set_max_residual_partition_order(encoder, 3);

			break;
		case 4:
			ex_FLAC__seekable_stream_encoder_set_streamable_subset(encoder, true);
			ex_FLAC__seekable_stream_encoder_set_do_mid_side_stereo(encoder, true);
			ex_FLAC__seekable_stream_encoder_set_loose_mid_side_stereo(encoder, true);
			ex_FLAC__seekable_stream_encoder_set_blocksize(encoder, 4608);
			ex_FLAC__seekable_stream_encoder_set_max_lpc_order(encoder, 8);
			ex_FLAC__seekable_stream_encoder_set_qlp_coeff_precision(encoder, 0);
			ex_FLAC__seekable_stream_encoder_set_do_qlp_coeff_prec_search(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_do_exhaustive_model_search(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_min_residual_partition_order(encoder, 3);
			ex_FLAC__seekable_stream_encoder_set_max_residual_partition_order(encoder, 3);

			break;
		case 5:
			ex_FLAC__seekable_stream_encoder_set_streamable_subset(encoder, true);
			ex_FLAC__seekable_stream_encoder_set_do_mid_side_stereo(encoder, true);
			ex_FLAC__seekable_stream_encoder_set_loose_mid_side_stereo(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_blocksize(encoder, 4608);
			ex_FLAC__seekable_stream_encoder_set_max_lpc_order(encoder, 8);
			ex_FLAC__seekable_stream_encoder_set_qlp_coeff_precision(encoder, 0);
			ex_FLAC__seekable_stream_encoder_set_do_qlp_coeff_prec_search(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_do_exhaustive_model_search(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_min_residual_partition_order(encoder, 3);
			ex_FLAC__seekable_stream_encoder_set_max_residual_partition_order(encoder, 3);

			break;
		case 6:
			ex_FLAC__seekable_stream_encoder_set_streamable_subset(encoder, true);
			ex_FLAC__seekable_stream_encoder_set_do_mid_side_stereo(encoder, true);
			ex_FLAC__seekable_stream_encoder_set_loose_mid_side_stereo(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_blocksize(encoder, 4608);
			ex_FLAC__seekable_stream_encoder_set_max_lpc_order(encoder, 8);
			ex_FLAC__seekable_stream_encoder_set_qlp_coeff_precision(encoder, 0);
			ex_FLAC__seekable_stream_encoder_set_do_qlp_coeff_prec_search(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_do_exhaustive_model_search(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_min_residual_partition_order(encoder, 0);
			ex_FLAC__seekable_stream_encoder_set_max_residual_partition_order(encoder, 4);

			break;
		case 7:
			ex_FLAC__seekable_stream_encoder_set_streamable_subset(encoder, true);
			ex_FLAC__seekable_stream_encoder_set_do_mid_side_stereo(encoder, true);
			ex_FLAC__seekable_stream_encoder_set_loose_mid_side_stereo(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_blocksize(encoder, 4608);
			ex_FLAC__seekable_stream_encoder_set_max_lpc_order(encoder, 8);
			ex_FLAC__seekable_stream_encoder_set_qlp_coeff_precision(encoder, 0);
			ex_FLAC__seekable_stream_encoder_set_do_qlp_coeff_prec_search(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_do_exhaustive_model_search(encoder, true);
			ex_FLAC__seekable_stream_encoder_set_min_residual_partition_order(encoder, 0);
			ex_FLAC__seekable_stream_encoder_set_max_residual_partition_order(encoder, 6);

			break;
		case 8:
			ex_FLAC__seekable_stream_encoder_set_streamable_subset(encoder, true);
			ex_FLAC__seekable_stream_encoder_set_do_mid_side_stereo(encoder, true);
			ex_FLAC__seekable_stream_encoder_set_loose_mid_side_stereo(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_blocksize(encoder, 4608);
			ex_FLAC__seekable_stream_encoder_set_max_lpc_order(encoder, 12);
			ex_FLAC__seekable_stream_encoder_set_qlp_coeff_precision(encoder, 0);
			ex_FLAC__seekable_stream_encoder_set_do_qlp_coeff_prec_search(encoder, false);
			ex_FLAC__seekable_stream_encoder_set_do_exhaustive_model_search(encoder, true);
			ex_FLAC__seekable_stream_encoder_set_min_residual_partition_order(encoder, 0);
			ex_FLAC__seekable_stream_encoder_set_max_residual_partition_order(encoder, 6);

			break;
		default:
			ex_FLAC__seekable_stream_encoder_set_streamable_subset(encoder, currentConfig->flac_streamable_subset);
			ex_FLAC__seekable_stream_encoder_set_do_mid_side_stereo(encoder, currentConfig->flac_do_mid_side_stereo);
			ex_FLAC__seekable_stream_encoder_set_loose_mid_side_stereo(encoder, currentConfig->flac_loose_mid_side_stereo);
			ex_FLAC__seekable_stream_encoder_set_blocksize(encoder, currentConfig->flac_blocksize);
			ex_FLAC__seekable_stream_encoder_set_max_lpc_order(encoder, currentConfig->flac_max_lpc_order);
			ex_FLAC__seekable_stream_encoder_set_qlp_coeff_precision(encoder, currentConfig->flac_qlp_coeff_precision);
			ex_FLAC__seekable_stream_encoder_set_do_qlp_coeff_prec_search(encoder, currentConfig->flac_do_qlp_coeff_prec_search);
			ex_FLAC__seekable_stream_encoder_set_do_exhaustive_model_search(encoder, currentConfig->flac_do_exhaustive_model_search);
			ex_FLAC__seekable_stream_encoder_set_min_residual_partition_order(encoder, currentConfig->flac_min_residual_partition_order);
			ex_FLAC__seekable_stream_encoder_set_max_residual_partition_order(encoder, currentConfig->flac_max_residual_partition_order);

			break;
	}

	if (ex_FLAC__seekable_stream_encoder_init(encoder) != FLAC__SEEKABLE_STREAM_ENCODER_OK)
	{
		Utilities::ErrorMessage("Could not initialize %1 encoder! Please check the configuration!", "FLAC");

		ex_FLAC__seekable_stream_encoder_delete(encoder);

		return false;
	}

	return true;
}

Bool BonkEnc::FilterOutFLAC::Deactivate()
{
	ex_FLAC__seekable_stream_encoder_finish(encoder);
	ex_FLAC__seekable_stream_encoder_delete(encoder);

	for (Int i = 0; i < numMetadata; i++) ex_FLAC__metadata_object_delete(metadata[i]);

	return true;
}

Int BonkEnc::FilterOutFLAC::WriteData(UnsignedByte *data, Int size)
{
	bytesWritten = 0;

	buffer.Resize(size / (format->bits / 8));

	for (Int i = 0; i < size / (format->bits / 8); i++)
	{
		buffer[i] = ((UnsignedShort *) data)[i];
	}

	ex_FLAC__seekable_stream_encoder_process_interleaved(encoder, buffer, size / (format->bits / 8) / format->channels);

	return bytesWritten;
}

FLAC__StreamEncoderWriteStatus BonkEnc::FLACSeekableStreamEncoderWriteCallback(const FLAC__SeekableStreamEncoder *decoder, const FLAC__byte buffer[], unsigned bytes, unsigned samples, unsigned current_frame, void *client_data)
{
	FilterOutFLAC	*filter = (FilterOutFLAC *) client_data;

	filter->driver->WriteData((UnsignedByte *) buffer, bytes);
	filter->bytesWritten += bytes;

	return FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
}

FLAC__SeekableStreamEncoderSeekStatus BonkEnc::FLACSeekableStreamEncoderSeekCallback(const FLAC__SeekableStreamEncoder *decoder, FLAC__uint64 absolute_byte_offset, void *client_data)
{
	FilterOutFLAC	*filter = (FilterOutFLAC *) client_data;

	filter->driver->Seek(absolute_byte_offset);

	return FLAC__SEEKABLE_STREAM_ENCODER_SEEK_STATUS_OK;
}

FLAC__SeekableStreamEncoderTellStatus BonkEnc::FLACSeekableStreamEncoderTellCallback(const FLAC__SeekableStreamEncoder *decoder, FLAC__uint64 *absolute_byte_offset, void *client_data)
{
	FilterOutFLAC	*filter = (FilterOutFLAC *) client_data;

	*absolute_byte_offset = filter->driver->GetPos();

	return FLAC__SEEKABLE_STREAM_ENCODER_TELL_STATUS_OK;
}
