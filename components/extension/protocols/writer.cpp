 /* fre:ac - free audio converter
  * Copyright (C) 2001-2020 Robert Kausch <robert.kausch@freac.org>
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

Array<String>	 BoCA::ProtocolWriter::fileNames;
Array<String>	 BoCA::ProtocolWriter::fileHeaders;

Array<String>	 BoCA::ProtocolWriter::copyNames;

BoCA::ProtocolWriter::ProtocolWriter()
{
	trackList = NIL;

	/* Connect slots.
	 */
	Protocol::onUpdateProtocolList.Connect(&ProtocolWriter::OnUpdateProtocolList, this);
	Protocol::onUpdateProtocol.Connect(&ProtocolWriter::OnUpdateProtocol, this);

	Engine	*engine = Engine::Get();

	engine->onStartConversion.Connect(&ProtocolWriter::OnStartConversion, this);
	engine->onSingleFileConversion.Connect(&ProtocolWriter::OnSingleFileConversion, this);
}

BoCA::ProtocolWriter::~ProtocolWriter()
{
	/* Disconnect slots.
	 */
	Protocol::onUpdateProtocolList.Disconnect(&ProtocolWriter::OnUpdateProtocolList, this);
	Protocol::onUpdateProtocol.Disconnect(&ProtocolWriter::OnUpdateProtocol, this);

	Engine	*engine = Engine::Get();

	engine->onStartConversion.Disconnect(&ProtocolWriter::OnStartConversion, this);
	engine->onSingleFileConversion.Disconnect(&ProtocolWriter::OnSingleFileConversion, this);
}

Void BoCA::ProtocolWriter::OnUpdateProtocolList()
{
	/* Format protocol header.
	 */
	Protocol	*protocol = Protocol::Get().GetLast();

	FormatHeader(protocol);

	/* Save protocol.
	 */
	const Config	*config	  = Config::Get();

	if (config->GetIntValue(ConfigureProtocols::ConfigID, "ArchiveLogs", True)) SaveProtocol(protocol, GetProtocolFileName(protocol));

	/* Check whether to save an additional copy
	 * of conversion logs with the audio files.
	 */
	if (config->GetIntValue(ConfigureProtocols::ConfigID, "SaveConversionLogsWithFiles", False) && trackList != NIL)
	{
		Bool	 createAdditionalCopy = True;

		/* Check whether it's a CD ripping job.
		 */
		if (config->GetIntValue(ConfigureProtocols::ConfigID, "SaveRippingLogsOnly", True))
		{
			createAdditionalCopy = False;

			foreach (const Track &track, *trackList)
			{
				if (track.isCDTrack) { createAdditionalCopy = True; break; }
			}
		}

		/* Save additional log copy.
		 */
		if (createAdditionalCopy) SaveProtocol(protocol, GetAdditionalCopyName(protocol));

		trackList      = NIL;
		singleFileName = NIL;
	}
}

Void BoCA::ProtocolWriter::OnUpdateProtocol(const String &name)
{
	/* Check if there is a file name associated with this protocol.
	 */
	Protocol	*protocol = Protocol::Get(name);

	Int		 index	  = Int64(protocol) & 0xFFFFFFFF;
	String		 fileName = fileNames.Get(index);
	String		 copyName = copyNames.Get(index);

	if (fileName == NIL && copyName == NIL) return;

	/* Get new message and output it.
	 */
	String		 message  = protocol->GetMessages().GetLast();

	if (fileName != NIL) OutputMessage(fileName, message);
	if (copyName != NIL) OutputMessage(copyName, message);
}

Void BoCA::ProtocolWriter::OnStartConversion(Int conversionID, const Array<Track> &tracks)
{
	trackList = &tracks;
}

Void BoCA::ProtocolWriter::OnSingleFileConversion(Int conversionID, const String &fileName)
{
	singleFileName = fileName;
}

String BoCA::ProtocolWriter::FormatHeader(const Protocol *protocol)
{
	Int	 index	    = Int64(protocol) & 0xFFFFFFFF;
	String	 fileHeader = fileHeaders.Get(index);

	if (fileHeader != NIL) return fileHeader;

	/* Format new file header.
	 */
	const String	&operatingSystem = SystemInfo::GetOperatingSystem();
	const String	&architecture	 = SystemInfo::GetArchitecture();
	const String	&cpuModel	 = SystemInfo::GetCPUModel();
	const String	&installedRAM	 = SystemInfo::GetInstalledRAM();
	const String	&newLine	 = SystemInfo::GetNewLineCharacters();

	Application	*app		 = Application::Get();
	DateTime	 date		 = DateTime::Current();

	fileHeader.Append(protocol->GetName()).Append(newLine)
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

	fileHeaders.Add(fileHeader, index);

	return fileHeader;
}

String BoCA::ProtocolWriter::GetProtocolFileName(const Protocol *protocol)
{
	Int	 index	  = Int64(protocol) & 0xFFFFFFFF;
	String	 fileName = fileNames.Get(index);

	if (fileName != NIL) return fileName;

	const Config	*config = Config::Get();

	/* Find file name.
	 */
	DateTime	 date	     = DateTime::Current();
	String		 logsFolder  = Utilities::GetAbsolutePathName(config->GetStringValue(ConfigureProtocols::ConfigID, "LogsFolder", String(config->cacheDir).Append("logs")));

	if (!logsFolder.EndsWith(Directory::GetDirectoryDelimiter())) logsFolder.Append(Directory::GetDirectoryDelimiter());

	fileName = String(logsFolder).Append("[<date> <time>] <logname>.log");

	fileName.Replace("<date>", String::FromInt(date.GetYear()).Append(date.GetMonth() < 10 ? "0" : NIL).Append(String::FromInt(date.GetMonth())).Append(date.GetDay() < 10 ? "0" : NIL).Append(String::FromInt(date.GetDay())));
	fileName.Replace("<time>", String(date.GetHour() < 10 ? "0" : NIL).Append(String::FromInt(date.GetHour())).Append(date.GetMinute() < 10 ? "0" : NIL).Append(String::FromInt(date.GetMinute())).Append(date.GetSecond() < 10 ? "0" : NIL).Append(String::FromInt(date.GetSecond())));
	fileName.Replace("<logname>", Utilities::ReplaceIncompatibleCharacters(protocol->GetName()));

	fileName = Utilities::NormalizeFileName(fileName);

	fileNames.Add(fileName, index);

	return fileName;
}

String BoCA::ProtocolWriter::GetAdditionalCopyName(const Protocol *protocol)
{
	Int	 index	  = Int64(protocol) & 0xFFFFFFFF;
	String	 copyName = copyNames.Get(index);

	if (copyName != NIL) return copyName;

	const Config	*config = Config::Get();

	/* Find additional copy file name.
	 */
	String	 defaultPattern = String("<albumartist> - <album>").Append(Directory::GetDirectoryDelimiter()).Append("<albumartist> - <album>");
	String	 outputFolder	= Utilities::GetAbsolutePathName(config->GetStringValue("Settings", "EncoderOutDir", NIL));

	if (!outputFolder.EndsWith(Directory::GetDirectoryDelimiter())) outputFolder.Append(Directory::GetDirectoryDelimiter());

	copyName = String(outputFolder).Append(config->GetStringValue(ConfigureProtocols::ConfigID, "ConversionLogPattern", defaultPattern)).Append(".log");

	if (singleFileName != NIL)
	{
		copyName = singleFileName.Head(singleFileName.FindLast(".")).Append(".log");
	}
	else if (copyName != NIL)
	{
		I18n		*i18n = I18n::Get();

		if (copyName.Trim() == NIL) copyName = defaultPattern;

		const Info	&info = trackList->GetFirst().GetInfo();
		DateTime	 date = DateTime::Current();

		copyName.Replace("<artist>", Utilities::ReplaceIncompatibleCharacters(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist")));
		copyName.Replace("<album>", Utilities::ReplaceIncompatibleCharacters(info.album.Length() > 0 ? info.album : i18n->TranslateString("unknown album")));
		copyName.Replace("<genre>", Utilities::ReplaceIncompatibleCharacters(info.genre.Length() > 0 ? info.genre : i18n->TranslateString("unknown genre")));
		copyName.Replace("<year>", Utilities::ReplaceIncompatibleCharacters(info.year > 0 ? String::FromInt(info.year) : i18n->TranslateString("unknown year")));

		copyName.Replace("<date>", String::FromInt(date.GetYear()).Append(date.GetMonth() < 10 ? "0" : NIL).Append(String::FromInt(date.GetMonth())).Append(date.GetDay() < 10 ? "0" : NIL).Append(String::FromInt(date.GetDay())));
		copyName.Replace("<time>", String(date.GetHour() < 10 ? "0" : NIL).Append(String::FromInt(date.GetHour())).Append(date.GetMinute() < 10 ? "0" : NIL).Append(String::FromInt(date.GetMinute())).Append(date.GetSecond() < 10 ? "0" : NIL).Append(String::FromInt(date.GetSecond())));
		copyName.Replace("<logname>", Utilities::ReplaceIncompatibleCharacters(protocol->GetName()));

		/* Replace <disc> pattern.
		 */
		copyName.Replace("<disc>", String::FromInt(info.disc < 0 ? 0 : info.disc));

		for (Int i = 1; i <= 4; i++)
		{
			String	 pattern = String("<disc(").Append(String::FromInt(i)).Append(")>");

			copyName.Replace(pattern, String().FillN('0', i - ((Int) Math::Log10(info.disc > 0 ? info.disc : 1) + 1)).Append(String::FromInt(info.disc < 0 ? 0 : info.disc)));
		}

		/* Replace other text fields.
		 */
		foreach (const String &pair, info.other)
		{
			String	 key   = pair.Head(pair.Find(":"));
			String	 value = pair.Tail(pair.Length() - pair.Find(":") - 1);

			if (value == NIL) continue;

			if	(key == INFO_ALBUMARTIST) copyName.Replace("<albumartist>", Utilities::ReplaceIncompatibleCharacters(value));
			else if	(key == INFO_CONDUCTOR)	  copyName.Replace("<conductor>", Utilities::ReplaceIncompatibleCharacters(value));
			else if	(key == INFO_COMPOSER)	  copyName.Replace("<composer>", Utilities::ReplaceIncompatibleCharacters(value));
		}

		if (info.artist.Length() > 0) copyName.Replace("<albumartist>", Utilities::ReplaceIncompatibleCharacters(info.artist));

		copyName.Replace("<albumartist>", Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown album artist")));
		copyName.Replace("<conductor>", Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown conductor")));
		copyName.Replace("<composer>", Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown composer")));
	}
	else
	{
		copyName = protocol->GetName();
	}

	copyName = Utilities::NormalizeFileName(copyName);

	copyNames.Add(copyName, index);

	return copyName;
}

Void BoCA::ProtocolWriter::SaveProtocol(const Protocol *protocol, const String &fileName)
{
	/* Create file and output header.
	 */
	OutStream		 out(STREAM_FILE, Utilities::CreateDirectoryForFile(fileName), OS_REPLACE);
	String::OutputFormat	 outFormat("UTF-8");

	out.OutputString(FormatHeader(protocol));

	/* Save log messages.
	 */
	const Array<String>	&messages = protocol->GetMessages();

	foreach (const String &message, messages)
	{
		out.OutputLine(message.SubString(0, 12).Append(" ").Append(message.Tail(message.Length() - 15)));
	}
}

Void BoCA::ProtocolWriter::OutputMessage(const String &fileName, const String &message)
{
	/* Open file and output message.
	 */
	OutStream		 out(STREAM_FILE, fileName, OS_APPEND);
	String::OutputFormat	 outFormat("UTF-8");

	out.OutputLine(message.SubString(0, 12).Append(" ").Append(message.Tail(message.Length() - 15)));
}
