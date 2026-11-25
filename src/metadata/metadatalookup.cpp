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

#include <metadata/metadatalookup.h>

#include <config.h>

using namespace BoCA;
using namespace BoCA::AS;

static Array<DiscMetadata>	 recordCache;
static Threads::Mutex		 recordCacheMutex;

Bool freac::MetadataLookup::GetCachedRecord(const DiscMetadata &known, DiscMetadata &record)
{
	Threads::Lock	 lock(recordCacheMutex);
	String		 key = GetDiscKey(known.disc);

	foreach (const DiscMetadata &cached, recordCache)
	{
		if (!IsSameDisc(GetDiscKey(cached.disc), key)) continue;

		record = cached;

		return True;
	}

	return False;
}

Void freac::MetadataLookup::CacheRecord(const DiscMetadata &record)
{
	/* Hold the lock over both steps, so that two threads caching the
	 * same disc do not leave two records. The mutex is recursive.
	 */
	Threads::Lock	 lock(recordCacheMutex);

	RemoveCachedRecord(record);

	recordCache.Add(record);
}

Void freac::MetadataLookup::RemoveCachedRecord(const DiscMetadata &disc)
{
	Threads::Lock	 lock(recordCacheMutex);
	String		 key = GetDiscKey(disc.disc);

	for (Int i = recordCache.Length() - 1; i >= 0; i--)
	{
		if (IsSameDisc(GetDiscKey(recordCache.GetNth(i).disc), key)) recordCache.RemoveNth(i);
	}
}

Bool freac::MetadataLookup::IsInJoblist(const DiscMetadata &disc)
{
	DiscMetadata	 known;

	return GetJoblistKnownData(disc, known);
}

/* What fre:ac knows about the disc of a queue entry from its first track
 * in the joblist. This TOC is exact, while a queue entry that was
 * converted from the old CDDB queue has its lead-out in whole seconds
 * only. A disc with the same key is preferred over one that is close.
 */
Bool freac::MetadataLookup::GetJoblistKnownData(const DiscMetadata &disc, DiscMetadata &known)
{
	String			 key	 = GetDiscKey(disc.disc);
	const Array<Track>	*joblist = BoCA::JobList::Get()->getTrackList.Call();

	for (Int pass = 0; pass < 2; pass++)
	{
		foreach (const Track &track, *joblist)
		{
			String	 trackKey = GetDiscKey(track.GetInfo());

			if (pass == 0 && !IsSameDisc(key, trackKey))	   continue;
			if (pass == 1 && !IsSameQueuedDisc(key, trackKey)) continue;

			known = GetKnownData(track);

			return True;
		}
	}

	return False;
}

freac::MetadataLookup::MetadataLookup()
{
	autoSelect	= BoCA::Config::Get()->GetIntValue(Config::CategoryMetadataID, Config::MetadataAutoSelectID, Config::MetadataAutoSelectDefault);

	cancel		= False;

	activeProvider	= NIL;
}

freac::MetadataLookup::~MetadataLookup()
{
}

/* Remember the provider that Lookup uses now, so that Cancel can stop
 * its request. A provider that becomes active after a Cancel is
 * canceled at once.
 */
Void freac::MetadataLookup::SetActiveProvider(MetadataComponent *provider)
{
	Threads::Lock	 lock(activeProviderMutex);

	activeProvider = provider;

	if (cancel && activeProvider != NIL) activeProvider->Cancel();
}

Void freac::MetadataLookup::Cancel()
{
	Threads::Lock	 lock(activeProviderMutex);

	cancel = True;

	if (activeProvider != NIL) activeProvider->Cancel();
}

MetadataComponent *freac::MetadataLookup::CreateProvider(const String &providerID)
{
	Registry	&boca	   = Registry::Get();
	Component	*component = boca.CreateComponentByID(providerID);

	if (component == NIL) return NIL;

	if (component->GetType() != BoCA::COMPONENT_TYPE_METADATA) { boca.DeleteComponent(component); return NIL; }

	return (MetadataComponent *) component;
}

/* What fre:ac knows about the disc of a track: the TOC and, if the
 * track has them, the album artist and album from CD-Text or tags.
 */
DiscMetadata freac::MetadataLookup::GetKnownData(const Track &track)
{
	const Info	&info = track.GetInfo();
	DiscMetadata	 known;

	known.disc.mcdi	   = info.mcdi;
	known.disc.offsets = info.offsets;

	known.disc.artist  = info.GetOtherInfo(INFO_ALBUMARTIST) != NIL ? info.GetOtherInfo(INFO_ALBUMARTIST) : info.artist;
	known.disc.album   = info.album;

	return known;
}

/* The IDs of the enabled providers, in the order that the user set.
 */
Array<String> freac::MetadataLookup::GetProviders()
{
	BoCA::Config		*config = BoCA::Config::Get();
	Registry		&boca	= Registry::Get();

	const Array<String>	&ids	= config->GetStringValue(Config::CategoryMetadataID, Config::MetadataProvidersID, Config::MetadataProvidersDefault).Explode(",");
	Array<String>		 providers;

	foreach (const String &id, ids)
	{
		String	 providerID = id.Trim();

		if (providerID != NIL && boca.ComponentExists(providerID)) providers.Add(providerID);
	}

	return providers;
}

Int freac::MetadataLookup::GetAvailableKeys(const DiscMetadata &known)
{
	Int	 keys = 0;

	if (known.disc.mcdi.IsValid() || known.disc.offsets != NIL)		   keys |= METADATA_QUERY_KEY_TOC;
	if (known.disc.artist != NIL && known.disc.album != NIL)		   keys |= METADATA_QUERY_KEY_TEXT;
	if (known.disc.GetOtherInfo(INFO_METADATA_RECORDID) != NIL)		   keys |= METADATA_QUERY_KEY_ID;

	return keys;
}

/* Tracks of the same disc have the same key: the offsets string of their TOC.
 */
String freac::MetadataLookup::GetDiscKey(const Info &info)
{
	if	(info.mcdi.IsValid()) return info.mcdi.GetOffsetString();
	else if (info.offsets != NIL) return info.offsets.ToUpper();

	return NIL;
}

/* Disc keys match if they are the same. Two pressings of a disc can
 * differ only in their lead-out, so a lead-out that is close is not
 * enough.
 */
Bool freac::MetadataLookup::IsSameDisc(const String &key1, const String &key2)
{
	if (key1 == NIL || key2 == NIL) return False;

	return key1 == key2;
}

/* A queue entry that was converted from the old CDDB queue counts data
 * tracks too and has its lead-out in whole seconds. Such an entry matches
 * a disc with the same track offsets and a lead-out less than a second
 * away. Other entries must match exactly.
 */
Bool freac::MetadataLookup::IsSameQueuedDisc(const String &queueKey, const String &key)
{
	if (IsSameDisc(queueKey, key))	       return True;
	if (queueKey == NIL || key == NIL)     return False;

	const Array<String>	&values1 = queueKey.Explode("+");
	const Array<String>	&values2 = key.Explode("+");

	if (values1.Length() != values2.Length() || values1.Length() < 3) return False;

	for (Int i = 1; i < values1.Length() - 1; i++)
	{
		if (values1.GetNth(i) != values2.GetNth(i)) return False;
	}

	Int64	 leadOut1 = (Int64) Number::FromHexString(values1.GetLast());
	Int64	 leadOut2 = (Int64) Number::FromHexString(values2.GetLast());

	if (leadOut1 % FRAMES_PER_SECOND != 0) return False;

	return Math::Abs(leadOut1 - leadOut2) < FRAMES_PER_SECOND;
}

/* The index of the TOC entry of a track, or -1.
 */
Int freac::MetadataLookup::GetTOCIndex(const Track &track)
{
	const Info	&info = track.GetInfo();

	if (info.mcdi.IsValid())
	{
		Int	 number = track.isCDTrack ? track.cdTrack : info.track;

		for (Int i = 0; i < info.mcdi.GetNumberOfEntries(); i++)
		{
			if (info.mcdi.GetNthEntryTrackNumber(i) == number) return i;
		}

		return -1;
	}

	if (info.offsets != NIL && info.track > 0) return info.track - 1;

	return -1;
}

/* Keys that identify the physical disc or recording. The disc or the
 * file carries the right value, while a record has the value of one
 * release or pressing, so a lookup fills these only if the track has
 * none.
 */
static Bool IsIdentifierKey(const String &key)
{
	return key == INFO_CATALOGNUMBER || key == INFO_BARCODE;
}

/* The fields that lookups wrote. The next lookup puts back the value
 * that a field had before, so that the track does not mix two records,
 * but only if the field still has the value that the lookup wrote: a
 * value that the user changed stays. The taggers write only the keys
 * that they know, so this list stays in memory.
 *
 * An entry holds the name of the field, the value that the lookup wrote
 * and the value before, separated by FilledSeparator. The name is the
 * key of an other field, or one of the names below for a basic field.
 * The values of a key with several values are joined with
 * ValueSeparator.
 */
static const char	*FilledFieldsKey = "Metadata filled fields";
static const char	*FilledSeparator = "\x1F";
static const char	*ValueSeparator	 = "\x1E";

static const char	*FilledArtist	 = "<artist>";
static const char	*FilledTitle	 = "<title>";
static const char	*FilledAlbum	 = "<album>";
static const char	*FilledGenre	 = "<genre>";
static const char	*FilledComment	 = "<comment>";
static const char	*FilledLabel	 = "<label>";
static const char	*FilledYear	 = "<year>";
static const char	*FilledDisc	 = "<disc>";
static const char	*FilledNumDiscs	 = "<number of discs>";
static const char	*FilledISRC	 = "<isrc>";

namespace freac
{
	struct FilledField
	{
		String	 name;
		String	 written;
		String	 before;
	};
};

static Bool IsTextField(const String &name)
{
	return name == FilledArtist || name == FilledTitle || name == FilledAlbum || name == FilledGenre || name == FilledComment;
}

static String NumberValue(Int number)
{
	return number > 0 ? String::FromInt(number) : String();
}

/* Every value of a field as one string. The artist, genre, label and
 * ISRC can have several values, like other keys.
 */
static String GetListValue(const Info &info, const String &key)
{
	return String::Implode(Info::GetInfoList(&info, key), ValueSeparator);
}

/* The value of a field as one string.
 */
static String GetFieldValue(const Info &info, const String &name)
{
	if (name == FilledArtist)   return GetListValue(info, INFO_ARTISTS);
	if (name == FilledTitle)    return info.title;
	if (name == FilledAlbum)    return info.album;
	if (name == FilledGenre)    return GetListValue(info, INFO_GENRES);
	if (name == FilledComment)  return info.comment;
	if (name == FilledLabel)    return GetListValue(info, INFO_LABELS);
	if (name == FilledYear)	    return NumberValue(info.year);
	if (name == FilledDisc)	    return NumberValue(info.disc);
	if (name == FilledNumDiscs) return NumberValue(info.numDiscs);
	if (name == FilledISRC)	    return GetListValue(info, INFO_ISRCS);

	return GetListValue(info, name);
}

/* Set a field to a value from GetFieldValue.
 */
static Void SetFieldValue(Info &info, const String &name, const String &value)
{
	Array<String>	 values;

	if (value != NIL) values = value.Explode(ValueSeparator);

	if	(name == FilledArtist)	 Info::SetInfoList(&info, INFO_ARTISTS, values);
	else if (name == FilledTitle)	 info.title    = value;
	else if (name == FilledAlbum)	 info.album    = value;
	else if (name == FilledGenre)	 Info::SetInfoList(&info, INFO_GENRES, values);
	else if (name == FilledComment)	 info.comment  = value;
	else if (name == FilledLabel)	 Info::SetInfoList(&info, INFO_LABELS, values);
	else if (name == FilledYear)	 info.year     = value != NIL ? value.ToInt() : -1;
	else if (name == FilledDisc)	 info.disc     = value != NIL ? value.ToInt() : 0;
	else if (name == FilledNumDiscs) info.numDiscs = value != NIL ? value.ToInt() : 0;
	else if (name == FilledISRC)	 Info::SetInfoList(&info, INFO_ISRCS, values);
	else				 Info::SetInfoList(&info, name, values);
}

static Array<freac::FilledField> GetFilledFields(const Info &info)
{
	Array<freac::FilledField>	 fields;
	Array<String>			 entries = Info::GetOtherInfoList(&info, FilledFieldsKey);

	foreach (const String &entry, entries)
	{
		Int	 first	= entry.Find(FilledSeparator);
		String	 rest	= first >= 0 ? entry.Tail(entry.Length() - first - 1) : String();
		Int	 second = rest.Find(FilledSeparator);

		if (first < 0 || second < 0) continue;

		freac::FilledField	 field;

		field.name    = entry.Head(first);
		field.written = rest.Head(second);
		field.before  = rest.Tail(rest.Length() - second - 1);

		fields.Add(field);
	}

	return fields;
}

static Void SetFilledFields(Info &info, const Array<freac::FilledField> &fields)
{
	info.SetOtherInfo(FilledFieldsKey, NIL);

	foreach (const freac::FilledField &field, fields)
	{
		Info::AddOtherInfo(&info, FilledFieldsKey, String(field.name).Append(FilledSeparator).Append(field.written).Append(FilledSeparator).Append(field.before));
	}
}

static Bool IsFilled(const Info &info, const String &name)
{
	Array<freac::FilledField>	 fields = GetFilledFields(info);

	foreach (const freac::FilledField &field, fields) if (field.name == name) return True;

	return False;
}

/* Note that a lookup wrote a field that had the value before. If an
 * earlier lookup wrote the field, the value before that lookup is kept.
 */
static Void NoteFilled(Info &info, const String &name, const String &before)
{
	Array<freac::FilledField>	 fields = GetFilledFields(info);
	freac::FilledField		 field;

	field.name    = name;
	field.written = GetFieldValue(info, name);
	field.before  = before;

	for (Int i = 0; i < fields.Length(); i++)
	{
		if (fields.GetNth(i).name != name) continue;

		field.before = fields.GetNth(i).before;

		fields.RemoveNth(i);

		break;
	}

	fields.Add(field);

	SetFilledFields(info, fields);
}

/* Set a field to a record value. Without merge, an empty record value
 * keeps the current value, and with keepCDText, so does a current value
 * that no lookup wrote. With merge, an empty record value is set only
 * with setEmpty.
 */
static Void ApplyField(Info &info, const String &name, const String &value, Bool keepCDText, Bool merge, Bool setEmpty = False)
{
	String	 current = GetFieldValue(info, name);

	if (value == NIL && !(merge && setEmpty)) return;
	if (!merge && keepCDText && current != NIL && !IsFilled(info, name)) return;

	SetFieldValue(info, name, value);

	if (!merge) NoteFilled(info, name, current);
}

/* Apply the fields of a record to a track. A field that the record has
 * replaces the field of the track; a field that the record does not
 * have keeps its value, so that data from tags is not lost. The ISRC,
 * catalog number and barcode are filled only if the track has none,
 * so those that the disc carries stay.
 *
 * With merge, the record holds what the user edited: its artist, title,
 * album, genre, track comment and year replace those of the track even
 * if empty.
 *
 * Without merge, the record becomes the provider record of the track.
 * Fields that the last lookup wrote get back their values from before
 * that lookup, so that the track does not mix two records. A text field
 * keeps the value of the last lookup if the new record does not have
 * it, and a field that the user changed after the lookup stays. If the
 * user prefers CD-Text to online data, every value that no lookup wrote
 * stays.
 *
 * The hidden track before track 1 (cdTrack == 0) has no TOC entry:
 * it gets the disc fields and keeps its title.
 */
Bool freac::MetadataLookup::ApplyRecord(Track &track, const DiscMetadata &record, Bool merge)
{
	if (!IsSameDisc(GetDiscKey(track.GetInfo()), GetDiscKey(record.disc))) return False;

	Bool	 hidden = (track.cdTrack == 0);
	Int	 index	= GetTOCIndex(track);

	if (!hidden && (index < 0 || index >= record.tracks.Length())) return False;

	const Info	&disc	     = record.disc;
	Info		 blank;
	const Info	&recordTrack = hidden ? blank : record.tracks.GetNth(index);
	Info		 info	     = track.GetInfo();

	Bool		 keepCDText  = False;

	if (!merge)
	{
		/* Put back the values from before the last lookup. A field
		 * that the user changed since then is no longer a lookup field.
		 * A text field keeps its value and stays a lookup field.
		 */
		Array<FilledField>	 filled = GetFilledFields(info);
		Array<FilledField>	 textFields;

		foreach (const FilledField &field, filled)
		{
			if (GetFieldValue(info, field.name) != field.written) continue;

			if (IsTextField(field.name)) textFields.Add(field);
			else			     SetFieldValue(info, field.name, field.before);
		}

		SetFilledFields(info, textFields);

		keepCDText = track.isCDTrack && !BoCA::Config::Get()->GetIntValue(Config::CategoryMetadataID, Config::MetadataOverwriteCDTextID, Config::MetadataOverwriteCDTextDefault);
	}

	/* Basic fields; a track field wins over a disc field. For a hidden
	 * track, recordTrack is blank, so these all fall back to the disc
	 * field, and the title is kept. After a submit (merge), the comment
	 * of a hidden track is kept too, because a blank record comment
	 * would clear it.
	 */
	ApplyField(info, FilledArtist, GetListValue(recordTrack.artist != NIL ? recordTrack : disc, INFO_ARTISTS), keepCDText, merge, True);
	ApplyField(info, FilledAlbum,  recordTrack.album  != NIL ? recordTrack.album  : disc.album,  keepCDText, merge, True);
	ApplyField(info, FilledGenre,  GetListValue(recordTrack.genre != NIL ? recordTrack : disc, INFO_GENRES), keepCDText, merge, True);

	if (!hidden || !merge) ApplyField(info, FilledComment, recordTrack.comment != NIL || merge ? recordTrack.comment : disc.comment, keepCDText, merge, True);
	if (!hidden)	       ApplyField(info, FilledTitle,   recordTrack.title, keepCDText, merge, True);

	ApplyField(info, FilledLabel, GetListValue(recordTrack.label != NIL ? recordTrack : disc, INFO_LABELS), keepCDText, merge);

	if (info.isrc == NIL) ApplyField(info, FilledISRC, GetListValue(recordTrack, INFO_ISRCS), keepCDText, merge);

	ApplyField(info, FilledYear,	 NumberValue(recordTrack.year > 0 ? recordTrack.year : disc.year), keepCDText, merge, True);
	ApplyField(info, FilledDisc,	 NumberValue(disc.disc),	 keepCDText, merge);
	ApplyField(info, FilledNumDiscs, NumberValue(disc.numDiscs),	 keepCDText, merge);

	if (!hidden)
	{
		if (recordTrack.track	  > 0) info.track     = recordTrack.track;
		if (recordTrack.numTracks > 0) info.numTracks = recordTrack.numTracks;
	}

	/* Other fields; the values of a key replace those of the track, and a
	 * key of the record track wins over the same key of the disc.
	 */
	Array<String>	 keys = Info::GetOtherInfoKeys(&disc);

	/* foreach evaluates its array for each element, so it must not
	 * get a temporary.
	 */
	Array<String>	 trackKeys = Info::GetOtherInfoKeys(&recordTrack);

	if (!hidden) foreach (const String &key, trackKeys) keys.Add(key);

	foreach (const String &key, keys)
	{
		if (key == INFO_METADATA_PROVIDER || key == INFO_METADATA_RECORDID || key == FilledFieldsKey) continue;

		/* The value lists of the basic fields were applied above.
		 */
		if (Info::IsBasicInfoList(key)) continue;

		if (IsIdentifierKey(key) && info.HasOtherInfo(key) && !merge) continue;

		const Array<String>	&values = (!hidden && recordTrack.HasOtherInfo(key)) ? Info::GetOtherInfoList(&recordTrack, key) : Info::GetOtherInfoList(&disc, key);

		ApplyField(info, key, String::Implode(values, ValueSeparator), keepCDText, merge, True);
	}

	if (!merge)
	{
		info.SetOtherInfo(INFO_METADATA_PROVIDER, record.provider);
		info.SetOtherInfo(INFO_METADATA_RECORDID, record.recordID);
	}

	track.SetInfo(info);

	track.outputFile = NIL;

	return True;
}

/* Apply a record to each joblist track of its disc.
 * Returns the number of tracks that changed.
 */
Int freac::MetadataLookup::ApplyToJoblist(const DiscMetadata &record, Bool setOriginalInfo, Bool merge)
{
	String			 key	 = GetDiscKey(record.disc);
	const Array<Track>	*joblist = BoCA::JobList::Get()->getTrackList.Call();
	Int			 changed = 0;

	if (key == NIL) return 0;

	for (Int i = 0; i < joblist->Length(); i++)
	{
		Track	 track = joblist->GetNth(i);

		if (!IsSameDisc(GetDiscKey(track.GetInfo()), key)) continue;

		if (!ApplyRecord(track, record, merge)) continue;

		if (setOriginalInfo) track.SetOriginalInfo(track.GetInfo());

		BoCA::JobList::Get()->onComponentModifyTrack.Emit(track);

		changed++;
	}

	return changed;
}

Void freac::MetadataLookup::SetFailedProviders(const Array<String> &providers)
{
	failedProviders = providers;
}

Bool freac::MetadataLookup::IsFailedProvider(const String &providerID) const
{
	foreach (const String &failedProvider, failedProviders) if (failedProvider == providerID) return True;

	return False;
}

/* Write an error from a failed Query or Read to the protocol and record
 * it in errors and failedProviders. Used by Lookup for both.
 */
Void freac::MetadataLookup::RecordProviderError(MetadataComponent *provider, const String &providerID, BoCA::Protocol *protocol)
{
	/* A canceled request fails, but that is no error of the provider.
	 */
	if (cancel) return;

	String	 error = String(provider->GetName()).Append(": ").Append(provider->GetErrorString());

	protocol->Write(error, MessageTypeError);

	errors.Add(error);
	failedProviders.Add(providerID);
}

/* Query the providers in order and stop at the first one with a match.
 * Providers that failed earlier with this object are skipped.
 *
 * When matches are selected automatically, an inexact match is used
 * only if no later provider has an exact one. If a provider failed and
 * no record was found, the result is LOOKUP_ERROR, so that the caller
 * can offer to repeat the query later.
 */
Int freac::MetadataLookup::Lookup(const DiscMetadata &known, const Array<String> &providerIDs, DiscMetadata &record)
{
	Registry		&boca	  = Registry::Get();
	BoCA::Protocol		*protocol = BoCA::Protocol::Get("Metadata lookup");

	Int			 available = GetAvailableKeys(known);

	Int			 queried   = 0;
	Int			 failed	   = 0;

	/* The provider of the first inexact match stays alive until the
	 * end, because a provider reads a match only from its last query.
	 */
	MetadataComponent	*fuzzyProvider = NIL;
	String			 fuzzyProviderID;
	MetadataMatch		 fuzzyMatch;

	Int			 result	   = LOOKUP_NOT_FOUND;

	errors.RemoveAll();

	for (Int i = 0; i < providerIDs.Length() && !cancel; i++)
	{
		const String	&providerID = providerIDs.GetNth(i);

		/* Skip providers that failed before in this run.
		 */
		if (IsFailedProvider(providerID)) { failed++; continue; }

		/* Create the provider and check that it can use the known data.
		 */
		MetadataComponent	*provider = CreateProvider(providerID);

		if (provider == NIL) continue;

		if ((provider->GetQueryKeys() & available) == 0) { boca.DeleteComponent(provider); continue; }

		SetActiveProvider(provider);

		Array<MetadataMatch>	 matches;
		Bool			 fuzzy	= False;

		result = QueryProvider(provider, providerID, known, matches, fuzzy, protocol);

		queried++;

		Bool	 keep = False;

		if (result == LOOKUP_FOUND && fuzzy && autoSelect)
		{
			/* Keep the first inexact match in case no provider has an exact one.
			 */
			if (fuzzyProvider == NIL) { fuzzyProvider = provider; fuzzyProviderID = providerID; fuzzyMatch = matches.GetFirst(); keep = True; }

			result = LOOKUP_NOT_FOUND;
		}
		else if (result == LOOKUP_FOUND)
		{
			Int	 index = 0;

			if ((matches.Length() > 1 || fuzzy) && !autoSelect && !cancel) index = selectMatch.Call(provider->GetName(), matches, fuzzy);

			if	(cancel || index == MATCH_CANCELED)    result = LOOKUP_CANCELED;
			else if (index < 0 || index >= matches.Length()) result = LOOKUP_NOT_FOUND;
			else						 result = ReadRecord(provider, providerID, matches.GetNth(index).recordID, record, protocol);
		}

		SetActiveProvider(NIL);

		if (!keep) boca.DeleteComponent(provider);

		if (result == LOOKUP_ERROR) failed++;

		if (result == LOOKUP_FOUND || result == LOOKUP_CANCELED) break;
	}

	/* Read the inexact match if no provider had an exact one.
	 */
	if (fuzzyProvider != NIL)
	{
		if (result != LOOKUP_FOUND && result != LOOKUP_CANCELED && !cancel)
		{
			SetActiveProvider(fuzzyProvider);

			result = ReadRecord(fuzzyProvider, fuzzyProviderID, fuzzyMatch.recordID, record, protocol);

			SetActiveProvider(NIL);

			if (result == LOOKUP_ERROR) failed++;
		}

		boca.DeleteComponent(fuzzyProvider);
	}

	if (cancel || result == LOOKUP_CANCELED) return LOOKUP_CANCELED;

	if (result == LOOKUP_FOUND)	return LOOKUP_FOUND;

	if (failed > 0)			return LOOKUP_ERROR;
	if (queried == 0)		return LOOKUP_NO_PROVIDER;

	return LOOKUP_NOT_FOUND;
}

/* Query one provider. Returns LOOKUP_FOUND with at least one match,
 * LOOKUP_NOT_FOUND or LOOKUP_ERROR.
 */
Int freac::MetadataLookup::QueryProvider(MetadataComponent *provider, const String &providerID, const DiscMetadata &known, Array<MetadataMatch> &matches, Bool &fuzzy, BoCA::Protocol *protocol)
{
	provider->SetConfiguration(BoCA::Config::Get());

	onQueryProvider.Emit(provider->GetName());

	Int	 result = provider->Query(known, matches);

	if (result == METADATA_QUERY_RESULT_ERROR)
	{
		RecordProviderError(provider, providerID, protocol);

		return LOOKUP_ERROR;
	}

	if (result == METADATA_QUERY_RESULT_NONE || matches.Length() == 0)
	{
		protocol->Write(String(provider->GetName()).Append(": No record for this disc."));

		return LOOKUP_NOT_FOUND;
	}

	fuzzy = (result == METADATA_QUERY_RESULT_FUZZY);

	return LOOKUP_FOUND;
}

/* Read the record of a match. Returns LOOKUP_FOUND or LOOKUP_ERROR.
 */
Int freac::MetadataLookup::ReadRecord(MetadataComponent *provider, const String &providerID, const String &recordID, DiscMetadata &record, BoCA::Protocol *protocol)
{
	provider->SetConfiguration(BoCA::Config::Get());

	if (!provider->Read(recordID, record))
	{
		RecordProviderError(provider, providerID, protocol);

		return LOOKUP_ERROR;
	}

	if (record.provider == NIL) record.provider = providerID;

	protocol->Write(String(provider->GetName()).Append(": Record ").Append(record.recordID));

	return LOOKUP_FOUND;
}
