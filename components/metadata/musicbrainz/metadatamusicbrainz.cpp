 /* fre:ac - free audio converter
  * Copyright (C) 2026 Shen-Ta Hsieh
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>

#include "metadatamusicbrainz.h"
#include "config.h"
#include "network.h"
#include "parser.h"

const String &BoCA::MetadataMusicBrainz::GetComponentSpecs()
{
	static String	 componentSpecs = "			\
								\
	  <?xml version=\"1.0\" encoding=\"UTF-8\"?>		\
	  <component>						\
	    <name>MusicBrainz</name>				\
	    <version>1.0</version>				\
	    <id>musicbrainz-metadata</id>			\
	    <type>metadata</type>				\
	  </component>						\
								\
	";

	return componentSpecs;
}

BoCA::MetadataMusicBrainz::MetadataMusicBrainz()
{
	configLayer = NIL;

	haveTOC	    = False;
	response    = NIL;
}

BoCA::MetadataMusicBrainz::~MetadataMusicBrainz()
{
	if (configLayer != NIL) Object::DeleteObject(configLayer);

	if (response != NIL) delete response;
}

BoCA::ConfigLayer *BoCA::MetadataMusicBrainz::GetConfigurationLayer()
{
	if (configLayer == NIL) configLayer = new ConfigureMusicBrainz();

	return configLayer;
}

/* The server URL without a slash at the end.
 */
String BoCA::MetadataMusicBrainz::GetServer() const
{
	String	 server = MusicBrainzNetwork::NormalizeServerURL(GetConfiguration()->GetStringValue(ConfigureMusicBrainz::ConfigID, ConfigureMusicBrainz::ServerID, ConfigureMusicBrainz::ServerDefault));

	if (server == NIL) server = ConfigureMusicBrainz::ServerDefault;

	return server;
}

Int BoCA::MetadataMusicBrainz::GetQueryKeys() const
{
	return METADATA_QUERY_KEY_TOC | METADATA_QUERY_KEY_ID;
}

/* The request path for a disc ID lookup.
 */
static String GetDiscIDPath(const BoCA::MusicBrainzTOC &toc)
{
	return String("/ws/2/discid/").Append(toc.GetDiscID()).Append("?inc=artist-credits+recordings+labels+release-groups+isrcs");
}

/* Query by release ID if the known data has one, else by disc ID.
 * The response stays in memory for Read.
 */
Int BoCA::MetadataMusicBrainz::Query(const DiscMetadata &nKnown, Array<MetadataMatch> &matches)
{
	I18n	*i18n	= I18n::Get();
	String	 server = GetServer();

	known	    = nKnown;
	haveTOC	    = toc.FromInfo(known.disc);

	errorState  = False;
	errorString = NIL;

	matches.RemoveAll();
	dataFirstRecords.RemoveAll();

	if (response != NIL) { delete response; response = NIL; }

	/* Build the request. The ID is a release MBID from NormalizeRecordID
	 * or the record ID release/position of an earlier MusicBrainz record.
	 * A release query returns each medium that fits the TOC, so the
	 * position is not used.
	 */
	String	 id	     = known.disc.GetOtherInfo(INFO_METADATA_RECORDID);
	Bool	 discIDQuery = (id == NIL);
	String	 path;

	if (!discIDQuery)
	{
		String	 releaseID;
		Int	 position = 0;

		if (!MusicBrainzParser::ParseRecordID(id, releaseID, position)) releaseID = id;

		releaseID = MusicBrainzTOC::NormalizeReleaseID(releaseID);

		if (releaseID == NIL)
		{
			errorState  = True;
			errorString = i18n->TranslateString("%1 is not a valid MusicBrainz release ID.", "Metadata::MusicBrainz::Errors").Replace("%1", id);

			return METADATA_QUERY_RESULT_ERROR;
		}

		path = String("/ws/2/release/").Append(releaseID).Append("?inc=artist-credits+recordings+labels+release-groups+isrcs+discids");
	}
	else if (haveTOC)
	{
		path = GetDiscIDPath(toc);
	}
	else
	{
		return METADATA_QUERY_RESULT_NONE;
	}

	/* Send it. If the disc has only an offsets string with data tracks
	 * and the server does not know the disc ID, try again with the data
	 * tracks at the start.
	 */
	Buffer<UnsignedByte>	 buffer;
	Int			 status = MusicBrainzNetwork::Get(GetConfiguration(), String(server).Append(path), buffer, &canceled);

	/* A canceled request is no error; it found nothing.
	 */
	if (canceled) return METADATA_QUERY_RESULT_NONE;

	if (status == 404 && !MusicBrainzParser::IsErrorDocument(buffer)) status = -1;

	if (status == 404 && discIDQuery && MusicBrainzTOC::IsDataTrackPlaceUnknown(known.disc))
	{
		toc.FromInfo(known.disc, True);

		path   = GetDiscIDPath(toc);
		status = MusicBrainzNetwork::Get(GetConfiguration(), String(server).Append(path), buffer, &canceled);

		if (canceled) return METADATA_QUERY_RESULT_NONE;

		if (status == 404 && !MusicBrainzParser::IsErrorDocument(buffer)) status = -1;
	}

	if (status == 404) return METADATA_QUERY_RESULT_NONE;

	if (status != 200)
	{
		errorState = True;

		if	(status == -1) errorString = i18n->TranslateString("The answer of %1 could not be read.", "Metadata::MusicBrainz::Errors").Replace("%1", server);
		else if (status ==  0) errorString = i18n->TranslateString("Unable to connect to %1.", "Metadata::MusicBrainz::Errors").Replace("%1", server);
		else		 errorString = i18n->TranslateString("The server %1 returned the HTTP status %2.", "Metadata::MusicBrainz::Errors").Replace("%1", server).Replace("%2", String::FromInt(status));

		return METADATA_QUERY_RESULT_ERROR;
	}

	/* Read the response.
	 */
	response = new XML::Document();
	response->ParseMemory((UnsignedByte *) buffer, buffer.Size());

	XML::Node	*metadata = MusicBrainzParser::GetMetadataNode(*response);

	if (metadata == NIL)
	{
		errorState  = True;
		errorString = i18n->TranslateString("The answer of %1 could not be read.", "Metadata::MusicBrainz::Errors").Replace("%1", server);

		return METADATA_QUERY_RESULT_ERROR;
	}

	/* A release query compares the media with the TOC, also with the
	 * data tracks at the start if the place of the data tracks is unknown.
	 */
	if (!discIDQuery) return MusicBrainzParser::GetReleaseMatches(metadata, known.disc, haveTOC ? &toc : NIL, matches, dataFirstTOC, dataFirstRecords);

	return MusicBrainzParser::GetMatches(metadata, haveTOC ? &toc : NIL, discIDQuery, matches);
}

/* Read a match from the response of the last query; no new request.
 */
Bool BoCA::MetadataMusicBrainz::Read(const String &recordID, DiscMetadata &record)
{
	I18n	*i18n = I18n::Get();

	errorState  = False;
	errorString = NIL;

	/* A match of the pass with the data tracks at the start is read
	 * with the TOC of that pass.
	 */
	const MusicBrainzTOC	*recordTOC = haveTOC ? &toc : NIL;

	foreach (const String &dataFirstRecord, dataFirstRecords)
	{
		if (dataFirstRecord == recordID) recordTOC = &dataFirstTOC;
	}

	if (response == NIL || !MusicBrainzParser::GetRecord(MusicBrainzParser::GetMetadataNode(*response), recordID, recordTOC, known.disc, record))
	{
		errorState  = True;
		errorString = i18n->TranslateString("The record %1 is not in the answer of the server.", "Metadata::MusicBrainz::Errors").Replace("%1", recordID);

		return False;
	}

	return True;
}

String BoCA::MetadataMusicBrainz::NormalizeRecordID(const String &input)
{
	return MusicBrainzTOC::NormalizeReleaseID(input);
}

Int BoCA::MetadataMusicBrainz::GetSubmitData() const
{
	return METADATA_SUBMIT_DATA_TOC;
}

Int BoCA::MetadataMusicBrainz::GetSubmitMethod() const
{
	return METADATA_SUBMIT_BY_USER;
}

/* Open the page on which the user attaches the disc ID to a release.
 */
Bool BoCA::MetadataMusicBrainz::Submit(const DiscMetadata &record)
{
	MusicBrainzTOC	 recordTOC;

	if (!recordTOC.FromInfo(record.disc))
	{
		errorState  = True;
		errorString = I18n::Get()->TranslateString("The disc has no table of contents.", "Metadata::MusicBrainz::Errors");

		return False;
	}

	return S::System::System::OpenURL(recordTOC.GetAttachURL(GetServer()));
}
