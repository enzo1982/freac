 /* fre:ac - free audio converter
  * Copyright (C) 2001-2021 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "writer.h"
#include "systeminfo.h"
#include "config.h"

using namespace smooth::IO;

Array<BoCA::ProtocolData>	 BoCA::ProtocolWriter::protocolData;
Int				 BoCA::ProtocolWriter::instanceCount = 0;

BoCA::ProtocolData::ProtocolData()
{
	conversionID = -1;

	fileStream   = NIL;
	copyStream   = NIL;

	trackList    = NIL;
}

BoCA::ProtocolData::~ProtocolData()
{
	/* Close stream handles.
	 */
	if (fileStream != NIL) delete fileStream;
	if (copyStream != NIL) delete copyStream;
}

BoCA::ProtocolWriter::ProtocolWriter()
{
	if (Threads::Access::Increment(instanceCount) > 1) return;

	/* Connect slots.
	 */
	Protocol::onUpdateProtocolList.Connect(&ProtocolWriter::OnUpdateProtocolList, this);
	Protocol::onUpdateProtocol.Connect(&ProtocolWriter::OnUpdateProtocol, this);

	Engine	*engine = Engine::Get();

	engine->onStartConversion.Connect(&ProtocolWriter::OnStartConversion, this);
	engine->onSingleFileConversion.Connect(&ProtocolWriter::OnSingleFileConversion, this);
	engine->onFinishConversion.Connect(&ProtocolWriter::OnFinishConversion, this);
	engine->onCancelConversion.Connect(&ProtocolWriter::OnFinishConversion, this);
}

BoCA::ProtocolWriter::~ProtocolWriter()
{
	if (Threads::Access::Decrement(instanceCount) > 0) return;

	/* Disconnect slots.
	 */
	Protocol::onUpdateProtocolList.Disconnect(&ProtocolWriter::OnUpdateProtocolList, this);
	Protocol::onUpdateProtocol.Disconnect(&ProtocolWriter::OnUpdateProtocol, this);

	Engine	*engine = Engine::Get();

	engine->onStartConversion.Disconnect(&ProtocolWriter::OnStartConversion, this);
	engine->onSingleFileConversion.Disconnect(&ProtocolWriter::OnSingleFileConversion, this);
	engine->onFinishConversion.Disconnect(&ProtocolWriter::OnFinishConversion, this);
	engine->onCancelConversion.Disconnect(&ProtocolWriter::OnFinishConversion, this);
}

Void BoCA::ProtocolWriter::OnUpdateProtocolList()
{
	/* Check whether new protocol was added.
	 */
	Protocol	*protocol = Protocol::Get().GetLast();
	Int		 index	  = Int64(protocol) & 0xFFFFFFFF;

	if (protocol == NIL || protocolData.Get(index).fileName != NIL) return;

	/* Add or update protocol data.
	 */
	if (protocolData.Length() > 0 && protocolData.GetLast().conversionID >= 0)
	{
		ProtocolData	 data = protocolData.GetLast();

		protocolData.RemoveNth(protocolData.Length() - 1);
		protocolData.Add(data, index);
	}
	else
	{
		protocolData.Add(ProtocolData(), index);
	}

	/* Format protocol header.
	 */
	FormatHeader(protocol);

	/* Save protocol.
	 */
	const Config	*config	  = Config::Get();

	if (config->GetIntValue(ConfigureProtocols::ConfigID, "ArchiveLogs", True)) SaveProtocol(protocol, GetProtocolFileName(protocol));

	/* Check whether to save an additional copy
	 * of conversion logs with the audio files.
	 */
	ProtocolData	&data = protocolData.GetReference(index);

	if (config->GetIntValue(ConfigureProtocols::ConfigID, "SaveConversionLogsWithFiles", False) && data.trackList != NIL)
	{
		Bool	 createAdditionalCopy = True;

		/* Check whether it's a CD ripping job.
		 */
		if (config->GetIntValue(ConfigureProtocols::ConfigID, "SaveRippingLogsOnly", True))
		{
			createAdditionalCopy = False;

			foreach (const Track &track, *data.trackList)
			{
				if (track.isCDTrack) { createAdditionalCopy = True; break; }
			}
		}

		/* Save additional log copy.
		 */
		if (createAdditionalCopy) SaveProtocol(protocol, GetAdditionalCopyName(protocol));
	}
}

Void BoCA::ProtocolWriter::OnUpdateProtocol(const String &name)
{
	/* Check if there is a file stream associated with this protocol.
	 */
	Protocol	*protocol = Protocol::Get(name);

	Int		 index	  = Int64(protocol) & 0xFFFFFFFF;
	ProtocolData	&data	  = protocolData.GetReference(index);

	if (data.fileStream == NIL && data.copyStream == NIL) return;

	/* Get new message and output it.
	 */
	String		 message  = protocol->GetMessages().GetLast();

	if (data.fileStream != NIL) OutputMessage(data.fileStream, message);
	if (data.copyStream != NIL) OutputMessage(data.copyStream, message);
}

Void BoCA::ProtocolWriter::OnStartConversion(Int conversionID, const Array<Track> &tracks)
{
	ProtocolData	 data;

	data.conversionID = conversionID;
	data.trackList	  = &tracks;

	protocolData.Add(data);
}

Void BoCA::ProtocolWriter::OnSingleFileConversion(Int conversionID, const String &fileName)
{
	foreach (ProtocolData &data, protocolData)
	{
		if (data.conversionID != conversionID) continue;

		data.singleFileName = fileName;

		break;
	}
}

Void BoCA::ProtocolWriter::OnFinishConversion(Int conversionID)
{
	foreach (ProtocolData &data, protocolData)
	{
		if (data.conversionID != conversionID) continue;

		if (data.fileStream != NIL) { delete data.fileStream; data.fileStream = NIL; }
		if (data.copyStream != NIL) { delete data.copyStream; data.copyStream = NIL; }

		data.trackList = NIL;

		break;
	}
}

String BoCA::ProtocolWriter::FormatHeader(const Protocol *protocol)
{
	Int		 index = Int64(protocol) & 0xFFFFFFFF;
	ProtocolData	&data  = protocolData.GetReference(index);

	if (data.fileHeader != NIL) return data.fileHeader;

	/* Format new file header.
	 */
	const String	&operatingSystem = SystemInfo::GetOperatingSystem();
	const String	&architecture	 = SystemInfo::GetArchitecture();
	const String	&cpuModel	 = SystemInfo::GetCPUModel();
	const String	&installedRAM	 = SystemInfo::GetInstalledRAM();
	const String	&newLine	 = SystemInfo::GetNewLineCharacters();

	Application	*app		 = Application::Get();
	DateTime	 date		 = DateTime::Current();

	data.fileHeader.Append(protocol->GetName()).Append(newLine)
		       .Append(newLine)
		       .Append("Client:  ").Append(app->getScreenName.Call()).Append(newLine)
		       .Append("Version: ").Append(app->getClientVersion.Call()).Append(" (").Append(architecture).Append(")").Append(newLine).Append(newLine)
		       .Append("System:  ").Append(operatingSystem).Append(newLine)
		       .Append("CPU:     ").Append(cpuModel).Append(newLine)
		       .Append("RAM:     ").Append(installedRAM).Append(newLine).Append(newLine)
		       .Append("Date:    ").Append(String::FromInt(date.GetYear())).Append("-")
					   .Append(date.GetMonth()  < 10 ? "0" : NIL).Append(String::FromInt(date.GetMonth())).Append("-")
					   .Append(date.GetDay()    < 10 ? "0" : NIL).Append(String::FromInt(date.GetDay())).Append(newLine)
		       .Append("Time:    ").Append(date.GetHour()   < 10 ? "0" : NIL).Append(String::FromInt(date.GetHour())).Append(":")
					   .Append(date.GetMinute() < 10 ? "0" : NIL).Append(String::FromInt(date.GetMinute())).Append(":")
					   .Append(date.GetSecond() < 10 ? "0" : NIL).Append(String::FromInt(date.GetSecond())).Append(newLine)
		       .Append(newLine)
		       .Append("hh:mm:ss.fff").Append(newLine)
		       .Append("------------").Append(newLine);

	return data.fileHeader;
}

String BoCA::ProtocolWriter::GetProtocolFileName(const Protocol *protocol)
{
	Int		 index = Int64(protocol) & 0xFFFFFFFF;
	ProtocolData	&data  = protocolData.GetReference(index);

	if (data.fileName != NIL) return data.fileName;

	const Config	*config = Config::Get();

	/* Find file name.
	 */
	DateTime	 date	     = DateTime::Current();
	String		 logsFolder  = Utilities::GetAbsolutePathName(config->GetStringValue(ConfigureProtocols::ConfigID, "LogsFolder", String(config->cacheDir).Append("logs")));

	if (!logsFolder.EndsWith(Directory::GetDirectoryDelimiter())) logsFolder.Append(Directory::GetDirectoryDelimiter());

	data.fileName	= String(logsFolder).Append("[<date> <time>] <logname>.log");

	data.fileName.Replace("<date>", String::FromInt(date.GetYear()).Append(date.GetMonth() < 10 ? "0" : NIL).Append(String::FromInt(date.GetMonth())).Append(date.GetDay() < 10 ? "0" : NIL).Append(String::FromInt(date.GetDay())));
	data.fileName.Replace("<time>", String(date.GetHour() < 10 ? "0" : NIL).Append(String::FromInt(date.GetHour())).Append(date.GetMinute() < 10 ? "0" : NIL).Append(String::FromInt(date.GetMinute())).Append(date.GetSecond() < 10 ? "0" : NIL).Append(String::FromInt(date.GetSecond())));
	data.fileName.Replace("<logname>", Utilities::ReplaceIncompatibleCharacters(protocol->GetName()));

	data.fileName	= Utilities::NormalizeFileName(data.fileName);
	data.fileStream	= new OutStream(STREAM_FILE, Utilities::CreateDirectoryForFile(data.fileName), OS_REPLACE);

	return data.fileName;
}

String BoCA::ProtocolWriter::GetAdditionalCopyName(const Protocol *protocol)
{
	Int		 index = Int64(protocol) & 0xFFFFFFFF;
	ProtocolData	&data  = protocolData.GetReference(index);

	if (data.copyName != NIL) return data.copyName;

	const Config	*config = Config::Get();

	/* Find folder for additional copy.
	 */
	const Track	&firstTrack	 = data.trackList->GetFirst();
	String		 inputFolder	 = File(firstTrack.fileName).GetFilePath();
	String		 outputFolder	 = Utilities::GetAbsolutePathName(config->GetStringValue("Settings", "EncoderOutDir", NIL));
	Bool		 writeToInputDir = config->GetIntValue("Settings", "WriteToInputDirectory", False);

	if (writeToInputDir && !firstTrack.isCDTrack && Utilities::IsFolderWritable(inputFolder)) outputFolder = inputFolder;

	if (!outputFolder.EndsWith(Directory::GetDirectoryDelimiter())) outputFolder.Append(Directory::GetDirectoryDelimiter());

	/* Find additional copy file name.
	 */
	String	 defaultPattern	 = String("<albumartist> - <album>").Append(Directory::GetDirectoryDelimiter()).Append("<albumartist> - <album>");

	data.copyName = String(outputFolder).Append(config->GetStringValue(ConfigureProtocols::ConfigID, "ConversionLogPattern", defaultPattern)).Append(".log");

	if (data.singleFileName != NIL)
	{
		data.copyName = data.singleFileName.Head(data.singleFileName.FindLast(".")).Append(".log");
	}
	else if (data.copyName != NIL)
	{
		I18n		*i18n = I18n::Get();

		if (data.copyName.Trim() == NIL) data.copyName = defaultPattern;

		const Info	&info = firstTrack.GetInfo();
		DateTime	 date = DateTime::Current();

		data.copyName.Replace("<artist>", Utilities::ReplaceIncompatibleCharacters(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist")));
		data.copyName.Replace("<album>", Utilities::ReplaceIncompatibleCharacters(info.album.Length() > 0 ? info.album : i18n->TranslateString("unknown album")));
		data.copyName.Replace("<genre>", Utilities::ReplaceIncompatibleCharacters(info.genre.Length() > 0 ? info.genre : i18n->TranslateString("unknown genre")));
		data.copyName.Replace("<year>", Utilities::ReplaceIncompatibleCharacters(info.year > 0 ? String::FromInt(info.year) : i18n->TranslateString("unknown year")));

		data.copyName.Replace("<date>", String::FromInt(date.GetYear()).Append(date.GetMonth() < 10 ? "0" : NIL).Append(String::FromInt(date.GetMonth())).Append(date.GetDay() < 10 ? "0" : NIL).Append(String::FromInt(date.GetDay())));
		data.copyName.Replace("<time>", String(date.GetHour() < 10 ? "0" : NIL).Append(String::FromInt(date.GetHour())).Append(date.GetMinute() < 10 ? "0" : NIL).Append(String::FromInt(date.GetMinute())).Append(date.GetSecond() < 10 ? "0" : NIL).Append(String::FromInt(date.GetSecond())));
		data.copyName.Replace("<logname>", Utilities::ReplaceIncompatibleCharacters(protocol->GetName()));

		/* Replace <disc> pattern.
		 */
		data.copyName.Replace("<disc>", String::FromInt(info.disc < 0 ? 0 : info.disc));

		for (Int i = 1; i <= 4; i++)
		{
			String	 pattern = String("<disc(").Append(String::FromInt(i)).Append(")>");

			data.copyName.Replace(pattern, String().FillN('0', i - ((Int) Math::Log10(info.disc > 0 ? info.disc : 1) + 1)).Append(String::FromInt(info.disc < 0 ? 0 : info.disc)));
		}

		/* Replace other text fields.
		 */
		foreach (const String &pair, info.other)
		{
			String	 key   = pair.Head(pair.Find(":"));
			String	 value = pair.Tail(pair.Length() - pair.Find(":") - 1);

			if (value == NIL) continue;

			if	(key == INFO_ALBUMARTIST) data.copyName.Replace("<albumartist>", Utilities::ReplaceIncompatibleCharacters(value));
			else if	(key == INFO_CONDUCTOR)	  data.copyName.Replace("<conductor>", Utilities::ReplaceIncompatibleCharacters(value));
			else if	(key == INFO_COMPOSER)	  data.copyName.Replace("<composer>", Utilities::ReplaceIncompatibleCharacters(value));
		}

		if (info.artist.Length() > 0) data.copyName.Replace("<albumartist>", Utilities::ReplaceIncompatibleCharacters(info.artist));

		data.copyName.Replace("<albumartist>", Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown album artist")));
		data.copyName.Replace("<conductor>", Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown conductor")));
		data.copyName.Replace("<composer>", Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown composer")));
	}
	else
	{
		data.copyName = protocol->GetName();
	}

	data.copyName	= Utilities::NormalizeFileName(data.copyName);
	data.copyStream	= new OutStream(STREAM_FILE, Utilities::CreateDirectoryForFile(data.copyName), OS_REPLACE);

	return data.copyName;
}

Void BoCA::ProtocolWriter::SaveProtocol(const Protocol *protocol, const String &fileName)
{
	/* Check whether to use an existing stream.
	 */
	Int		 index	  = Int64(protocol) & 0xFFFFFFFF;
	ProtocolData	&data	  = protocolData.GetReference(index);

	if	(fileName == data.fileName) SaveProtocol(protocol, data.fileStream);
	else if (fileName == data.copyName) SaveProtocol(protocol, data.copyStream);

	/* Save log to a new file.
	 */
	if (fileName != data.fileName && fileName != data.copyName)
	{
		OutStream	 fileStream(STREAM_FILE, Utilities::CreateDirectoryForFile(fileName), OS_REPLACE);

		SaveProtocol(protocol, &fileStream);
	}
}

Void BoCA::ProtocolWriter::SaveProtocol(const Protocol *protocol, OutStream *out)
{
	/* Write log header.
	 */
	String::OutputFormat	 outFormat("UTF-8");

	out->OutputString(FormatHeader(protocol));

	/* Write log messages.
	 */
	const Array<String>	&messages = protocol->GetMessages();

	foreach (const String &message, messages)
	{
		out->OutputLine(message.SubString(0, 12).Append(" ").Append(message.Tail(message.Length() - 15)));
	}
}

Void BoCA::ProtocolWriter::OutputMessage(OutStream *out, const String &message)
{
	/* Open file and output message.
	 */
	String::OutputFormat	 outFormat("UTF-8");

	out->OutputLine(message.SubString(0, 12).Append(" ").Append(message.Tail(message.Length() - 15)));
	out->Flush();
}
