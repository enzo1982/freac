 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "converter.h"

#ifdef __WIN32__
#	include <windows.h>
#else
#	include <sys/wait.h>
#endif

Array<BoCA::Converter *>	 BoCA::Converter::converters;

Array<BoCA::Converter *> &BoCA::Converter::Get()
{
	if (converters.Length() == 0)
	{
		Directory		 dir(Utilities::GetBoCADirectory().Append("../freac/freac.extension.youtube"));
		const Array<File>	&files = dir.GetFilesByPattern("converter_*.xml");

		foreach (const File &file, files)
		{
			Converter	*converter = new Converter(file);

			if (converter->IsSane())
			{
				converters.Add(converter);
			}
			else
			{
				delete converter;
			}
		}
	}

	return converters;
}

Bool BoCA::Converter::Free()
{
	foreach (Converter *converter, converters) delete converter;

	converters.RemoveAll();

	return True;
}

BoCA::Converter::Converter(const String &fileName)
{
	ParseXML(fileName);
}

BoCA::Converter::~Converter()
{
}

Bool BoCA::Converter::IsSane()
{
	if (!File(external_command).Exists()) return False;
	else				      return True;
}

Int BoCA::Converter::ParseXML(const String &fileName)
{
	XML::Document	*document = new XML::Document();

	document->LoadFile(fileName);

	XML::Node	*root = document->GetRootNode();

	for (Int i = 0; i < root->GetNOfNodes(); i++)
	{
		XML::Node	*node = root->GetNthNode(i);

		if 	(node->GetName() == "name")    name    = node->GetContent();
		else if (node->GetName() == "version") version = node->GetContent();
		else if (node->GetName() == "decoder") decoder = node->GetContent();
		else if (node->GetName() == "format")
		{
			for (Int j = 0; j < node->GetNOfNodes(); j++)
			{
				XML::Node	*node2 = node->GetNthNode(j);

				if	(node2->GetName() == "name")	  format.SetName(node2->GetContent());
				else if (node2->GetName() == "extension") format.AddExtension(node2->GetContent());
				else if (node2->GetName() == "tag")
				{
					AS::TagFormat	 tagFormat;

					tagFormat.SetName(node2->GetContent());

					if (node2->GetAttributeByName("id") != NIL) tagFormat.SetTagger(node2->GetAttributeByName("id")->GetContent());

					if (node2->GetAttributeByName("mode") != NIL)
					{
						if	(node2->GetAttributeByName("mode")->GetContent() == "prepend")	tagFormat.SetMode(TAG_MODE_PREPEND);
						else if (node2->GetAttributeByName("mode")->GetContent() == "append")	tagFormat.SetMode(TAG_MODE_APPEND);
						else if (node2->GetAttributeByName("mode")->GetContent() == "other")	tagFormat.SetMode(TAG_MODE_OTHER);
					}

					format.AddTagFormat(tagFormat);
				}
			}
		}
		else if (node->GetName() == "external")
		{
			for (Int j = 0; j < node->GetNOfNodes(); j++)
			{
				XML::Node	*node2 = node->GetNthNode(j);

				if (node2->GetName() == "command")
				{
					external_command = node2->GetContent();

					if (node2->GetAttributeByName("ignoreExitCode") != NIL)
					{
						external_ignoreExitCode = (node2->GetAttributeByName("ignoreExitCode")->GetContent() == "true");
					}
				}
				else if (node2->GetName() == "arguments")
				{
					external_arguments = node2->GetContent();
				}
			}
		}
	}

	delete document;

	/* Check if external command actually exists.
	 */
	if (external_command[0] != '/' && external_command[1] != ':')
	{
#if defined __WIN32__
		static const char	*places[] = { "%APPDIR\\codecs\\cmdline\\%COMMAND", "%APPDIR\\codecs\\cmdline\\%COMMAND.exe", NIL };
#elif defined __APPLE__
		static const char	*places[] = { "%APPDIR\\codecs\\cmdline\\%COMMAND", "/usr/bin/%COMMAND", "/usr/local/bin/%COMMAND", "/opt/local/bin/%COMMAND", "/sw/bin/%COMMAND", NIL };
#elif defined __HAIKU__
		static const char	*places[] = { "/boot/common/bin/%COMMAND", NIL };
#elif defined __NetBSD__
		static const char	*places[] = { "/usr/bin/%COMMAND", "/usr/local/bin/%COMMAND", "/usr/pkg/bin/%COMMAND", NIL };
#else
		static const char	*places[] = { "/usr/bin/%COMMAND", "/usr/local/bin/%COMMAND", NIL };
#endif

		for (Int i = 0; places[i] != NIL; i++)
		{
			String	 delimiter = Directory::GetDirectoryDelimiter();
			String	 file	   = String(places[i]).Replace("%APPDIR", GUI::Application::GetApplicationDirectory()).Replace("%COMMAND", external_command).Replace(String(delimiter).Append(delimiter), delimiter);

			if (File(file).Exists()) { external_command = file; break; }
		}

		if (external_command[0] != '/' && external_command[1] != ':') return False;
	}

	if (!File(external_command).Exists()) return Error();

	return Success();
}

Int BoCA::Converter::Run(const String &inFile, const String &outFile)
{
	/* Start 3rd party command line encoder
	 */
#if defined __WIN32__
	SHELLEXECUTEINFOA	 execInfo;

	ZeroMemory(&execInfo, sizeof(execInfo));

	execInfo.cbSize		= sizeof(execInfo);
	execInfo.fMask		= SEE_MASK_NOCLOSEPROCESS;
	execInfo.lpVerb		= "open";
	execInfo.lpFile		= String(external_command).Replace("/", Directory::GetDirectoryDelimiter());
	execInfo.lpParameters	= String(external_arguments).Replace("%INFILE", String("\"").Append(inFile).Append("\""))
							    .Replace("%OUTFILE", String("\"").Append(outFile).Append("\""));

	execInfo.lpDirectory	= GUI::Application::GetApplicationDirectory();
	execInfo.nShow		= SW_HIDE;

	ShellExecuteExA(&execInfo);

	/* Wait until the encoder exits
	 */
	unsigned long	 exitCode = 0;

	while (True)
	{
		GetExitCodeProcess(execInfo.hProcess, &exitCode);

		if (exitCode != STILL_ACTIVE) break;

		S::System::System::Sleep(10);
	}
#else
	String	 command   = String(external_command).Replace("/", Directory::GetDirectoryDelimiter());
	String	 arguments = String(external_arguments).Replace("%INFILE", inFile.Replace("\\", "\\\\").Replace(" ", "\\ ")
										 .Replace("\"", "\\\"").Replace("\'", "\\\'").Replace("`", "\\`")
										 .Replace("(", "\\(").Replace(")", "\\)").Replace("<", "\\<").Replace(">", "\\>")
										 .Replace("&", "\\&").Replace(";", "\\;").Replace("$", "\\$").Replace("|", "\\|"))
						       .Replace("%OUTFILE", outFile.Replace("\\", "\\\\").Replace(" ", "\\ ")
										   .Replace("\"", "\\\"").Replace("\'", "\\\'").Replace("`", "\\`")
										   .Replace("(", "\\(").Replace(")", "\\)").Replace("<", "\\<").Replace(">", "\\>")
										   .Replace("&", "\\&").Replace(";", "\\;").Replace("$", "\\$").Replace("|", "\\|"));

	FILE	*pipe	   = popen(String(command).Append(" ").Append(arguments), "r");

	/* Wait until the encoder exits
	 */
	unsigned long	 exitStatus = pclose(pipe);
	unsigned long	 exitCode   = WEXITSTATUS(exitStatus);

#endif

	/* Check if anything went wrong
	 */
	if (!external_ignoreExitCode && exitCode != 0)
	{
		/* Remove output file
		 */
		File(outFile).Delete();

		return Error();
	}

	return Success();
}
