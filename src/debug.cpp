 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/io/drivers/driver_zero.h>
#include <smooth/io/drivers/driver_posix.h>

#include <debug.h>
#include <freac.h>

#ifdef __WIN32__
#	include <windows.h>
#endif

freac::Debug::Debug(const String &fileName)
{
	config = freac::currentConfig;

	tabLevel = 0;

	if (config->enable_logging)
	{
		if (File(fileName).Exists()) File(fileName).Move(String(fileName).Append("-prev"));

		driver_out = new DriverPOSIX(fileName, OS_REPLACE);
		file_out = new OutStream(STREAM_DRIVER, driver_out);

		file_out->SetPackageSize(1);
	}
}

freac::Debug::~Debug()
{
	if (config->enable_logging)
	{
		delete file_out;

		delete driver_out;
	}
}

Int freac::Debug::OutputLine(const String &string)
{
	if (config->enable_logging)
	{
#ifdef __WIN32__
		SYSTEMTIME	 time;

		GetLocalTime(&time);

		file_out->OutputString(String(time.wHour   < 10 ? "0" : 0).Append(String::FromInt(time.wHour))
			  .Append(":").Append(time.wMinute < 10 ? "0" : 0).Append(String::FromInt(time.wMinute))
			  .Append(":").Append(time.wSecond < 10 ? "0" : 0).Append(String::FromInt(time.wSecond))
			  .Append(" - "));
#endif

		for (Int i = 0; i < tabLevel; i++) file_out->OutputString("\t");

		char	*stringUTF = string.ConvertTo("UTF-8");

		file_out->OutputData(stringUTF, strlen(stringUTF));
		file_out->OutputLine(NIL);
	}

	return Success();
}

Int freac::Debug::OutputVariable(const String &name, Int value)
{
#ifdef DEBUG
	OutputLine(String("Integer variable \'").Append(name).Append("\': ").Append(String::FromInt(value)));
#endif

	return Success();
}

Int freac::Debug::OutputVariable(const String &name, const String &value)
{
#ifdef DEBUG
	OutputLine(String("String variable \'").Append(name).Append("\': ").Append(value));
#endif

	return Success();
}

Int freac::Debug::EnterMethod(const String &name)
{
#ifdef DEBUG
	methods.Add(name);

	OutputLine(String("Entering method \'").Append(name).Append("\'."));

	tabLevel++;
#endif

	return Success();
}

Int freac::Debug::LeaveMethod()
{
#ifdef DEBUG
	tabLevel--;

	OutputLine(String("Leaving method \'").Append(methods.GetLast()).Append("\'."));

	methods.Remove(methods.GetNthIndex(methods.Length() - 1));
#endif

	return Success();
}
