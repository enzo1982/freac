 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2007 Robert Kausch <robert.kausch@bonkenc.org>
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

BonkEnc::Debug::Debug(const String &fileName)
{
	tabLevel = 0;

#ifdef DEBUG
	driver_out = new DriverPOSIX(fileName, OS_APPEND);
#else
	driver_out = new DriverZero();
#endif

	file_out = new OutStream(STREAM_DRIVER, driver_out);

	file_out->SetPackageSize(1);
}

BonkEnc::Debug::~Debug()
{
	delete file_out;

	delete driver_out;
}

Int BonkEnc::Debug::OutputString(const String &string)
{
	for (Int i = 0; i < tabLevel; i++) file_out->OutputString("\t");

	file_out->OutputString(string);

	return Success();
}

Int BonkEnc::Debug::OutputLine(const String &string)
{
	return OutputString(String(string).Append("\n"));
}

Int BonkEnc::Debug::OutputVariable(const String &name, Int value)
{
	for (Int i = 0; i < tabLevel; i++) file_out->OutputString("\t");

	file_out->OutputLine(String("Integer variable \'").Append(name).Append("\': ").Append(String::FromInt(value)));

	return Success();
}

Int BonkEnc::Debug::OutputVariable(const String &name, const String &value)
{
	for (Int i = 0; i < tabLevel; i++) file_out->OutputString("\t");

	file_out->OutputLine(String("String variable \'").Append(name).Append("\': ").Append(value));

	return Success();
}

Int BonkEnc::Debug::EnterMethod(const String &name)
{
	for (Int i = 0; i < tabLevel; i++) file_out->OutputString("\t");

	methods.Add(name);

	file_out->OutputString(String("Entering method \'").Append(name).Append("\'.\n"));

	tabLevel++;

	return Success();
}

Int BonkEnc::Debug::LeaveMethod()
{
	tabLevel--;

	for (Int i = 0; i < tabLevel; i++) file_out->OutputString("\t");

	file_out->OutputString(String("Leaving method \'").Append(methods.GetLast()).Append("\'.\n"));

	methods.Remove(methods.GetNthIndex(methods.Length() - 1));

	return Success();
}
