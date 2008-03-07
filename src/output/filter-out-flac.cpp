 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
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
	FLAC__StreamEncoderWriteStatus	 FLACStreamEncoderWriteCallback(const FLAC__StreamEncoder *, const FLAC__byte[], size_t, unsigned, unsigned, void *);
	FLAC__StreamEncoderSeekStatus	 FLACStreamEncoderSeekCallback(const FLAC__StreamEncoder *, FLAC__uint64, void *);
	FLAC__StreamEncoderTellStatus	 FLACStreamEncoderTellCallback(const FLAC__StreamEncoder *, FLAC__uint64 *, void *);
};

BonkEnc::FilterOutFLAC::FilterOutFLAC(Config *config, Track *format) : OutputFilter(config, format)
{
	if (format->channels > 2)
	{
		Utilities::ErrorMessage("BonkEnc does not support more than 2 channels!");

		errorState = True;

		return;
	}
}

BonkEnc::FilterOutFLAC::~FilterOutFLAC()
{
}

Bool BonkEnc::FilterOutFLAC::Activate()
{
	encoder = ex_FLAC__stream_encoder_new();

	if (currentConfig->enable_vctags)
	{
		char	*prevOutFormat = String::SetOutputFormat(currentConfig->vctag_encoding);

		FLAC__StreamMetadata				*vorbiscomment = ex_FLAC__metadata_object_new(FLAC__METADATA_TYPE_VORBIS_COMMENT);
		FLAC__StreamMetadata_VorbisComment_Entry	 comment;

		metadata.Add(vorbiscomment);

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

	if (currentConfig->copy_picture_tags)
	{
		for (Int i = 0; i < format->pictures.Length(); i++)
		{
			FLAC__StreamMetadata	*picture = ex_FLAC__metadata_object_new(FLAC__METADATA_TYPE_PICTURE);
			Picture			*picInfo = format->pictures.GetNth(i);

			metadata.Add(picture);

			if (picInfo->description != NIL) ex_FLAC__metadata_object_picture_set_description(picture, (FLAC__byte *) picInfo->description.ConvertTo("UTF-8"), true);

			ex_FLAC__metadata_object_picture_set_mime_type(picture, picInfo->mime, true);
			ex_FLAC__metadata_object_picture_set_data(picture, picInfo->data, picInfo->data.Size(), true);

			picture->data.picture.type = (FLAC__StreamMetadata_Picture_Type) picInfo->type;
		}
	}

	FLAC__StreamMetadata	*padding = ex_FLAC__metadata_object_new(FLAC__METADATA_TYPE_PADDING);

	padding->length = 4096;

	metadata.Add(padding);

	// Put metadata in an array and hand it to the encoder
	{
		FLAC__StreamMetadata	**metadataArray = new FLAC__StreamMetadata * [metadata.Length()];

		for (Int i = 0; i < metadata.Length(); i++) metadataArray[i] = metadata.GetNth(i);

		ex_FLAC__stream_encoder_set_metadata(encoder, metadataArray, metadata.Length());

		delete [] metadataArray;
	}

	ex_FLAC__stream_encoder_set_channels(encoder, format->channels);
	ex_FLAC__stream_encoder_set_sample_rate(encoder, format->rate);
	ex_FLAC__stream_encoder_set_bits_per_sample(encoder, format->bits == 32 ? 24 : format->bits);

	if (currentConfig->flac_preset < 0)
	{
		ex_FLAC__stream_encoder_set_streamable_subset(encoder, currentConfig->flac_streamable_subset);
		ex_FLAC__stream_encoder_set_do_mid_side_stereo(encoder, currentConfig->flac_do_mid_side_stereo);
		ex_FLAC__stream_encoder_set_loose_mid_side_stereo(encoder, currentConfig->flac_loose_mid_side_stereo);
		ex_FLAC__stream_encoder_set_blocksize(encoder, currentConfig->flac_blocksize);
		ex_FLAC__stream_encoder_set_apodization(encoder, currentConfig->flac_apodization);
		ex_FLAC__stream_encoder_set_max_lpc_order(encoder, currentConfig->flac_max_lpc_order);
		ex_FLAC__stream_encoder_set_qlp_coeff_precision(encoder, currentConfig->flac_qlp_coeff_precision);
		ex_FLAC__stream_encoder_set_do_qlp_coeff_prec_search(encoder, currentConfig->flac_do_qlp_coeff_prec_search);
		ex_FLAC__stream_encoder_set_do_exhaustive_model_search(encoder, currentConfig->flac_do_exhaustive_model_search);
		ex_FLAC__stream_encoder_set_min_residual_partition_order(encoder, currentConfig->flac_min_residual_partition_order);
		ex_FLAC__stream_encoder_set_max_residual_partition_order(encoder, currentConfig->flac_max_residual_partition_order);
	}
	else
	{
		ex_FLAC__stream_encoder_set_streamable_subset(encoder, true);
		ex_FLAC__stream_encoder_set_compression_level(encoder, currentConfig->flac_preset);

		if (currentConfig->flac_preset < 3) ex_FLAC__stream_encoder_set_blocksize(encoder, 1152);
		else				    ex_FLAC__stream_encoder_set_blocksize(encoder, 4608);
	}

	bytesWritten = 0;

	if (ex_FLAC__stream_encoder_init_stream(encoder, &FLACStreamEncoderWriteCallback, &FLACStreamEncoderSeekCallback, &FLACStreamEncoderTellCallback, NIL, this) != FLAC__STREAM_ENCODER_INIT_STATUS_OK)
	{
		Utilities::ErrorMessage("Could not initialize %1 encoder! Please check the configuration!", "FLAC");

		ex_FLAC__stream_encoder_delete(encoder);

		return false;
	}

	return true;
}

Bool BonkEnc::FilterOutFLAC::Deactivate()
{
	ex_FLAC__stream_encoder_finish(encoder);
	ex_FLAC__stream_encoder_delete(encoder);

	for (Int i = 0; i < metadata.Length(); i++) ex_FLAC__metadata_object_delete(metadata.GetNth(i));

	return true;
}

Int BonkEnc::FilterOutFLAC::WriteData(Buffer<UnsignedByte> &data, Int size)
{
	bytesWritten = 0;

	buffer.Resize(size / (format->bits / 8));

	for (Int i = 0; i < size / (format->bits / 8); i++)
	{
		if (format->bits == 8)		buffer[i] = data[i] - 128;
		else if (format->bits == 16)	buffer[i] = ((Short *) (unsigned char *) data)[i];
		else if (format->bits == 24)	buffer[i] = data[3 * i] + 256 * data[3 * i + 1] + 65536 * data[3 * i + 2] - (data[3 * i + 2] & 128 ? 16777216 : 0);
		else if (format->bits == 32)	buffer[i] = ((Int32 *) (unsigned char *) data)[i] / 256;
	}

	ex_FLAC__stream_encoder_process_interleaved(encoder, buffer, size / (format->bits / 8) / format->channels);

	return bytesWritten;
}

FLAC__StreamEncoderWriteStatus BonkEnc::FLACStreamEncoderWriteCallback(const FLAC__StreamEncoder *encoder, const FLAC__byte buffer[], size_t bytes, unsigned samples, unsigned current_frame, void *client_data)
{
	FilterOutFLAC	*filter = (FilterOutFLAC *) client_data;

	filter->driver->WriteData((UnsignedByte *) buffer, bytes);
	filter->bytesWritten += bytes;

	return FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
}

FLAC__StreamEncoderSeekStatus BonkEnc::FLACStreamEncoderSeekCallback(const FLAC__StreamEncoder *encoder, FLAC__uint64 absolute_byte_offset, void *client_data)
{
	FilterOutFLAC	*filter = (FilterOutFLAC *) client_data;

	filter->driver->Seek(absolute_byte_offset);

	return FLAC__STREAM_ENCODER_SEEK_STATUS_OK;
}

FLAC__StreamEncoderTellStatus BonkEnc::FLACStreamEncoderTellCallback(const FLAC__StreamEncoder *encoder, FLAC__uint64 *absolute_byte_offset, void *client_data)
{
	FilterOutFLAC	*filter = (FilterOutFLAC *) client_data;

	*absolute_byte_offset = filter->driver->GetPos();

	return FLAC__STREAM_ENCODER_TELL_STATUS_OK;
}
