 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <iolib/drivers/driver_zero.h>
#include <iolib/drivers/driver_posix.h>

#include <debug.h>

bonkEncDebug::bonkEncDebug(String fileName)
{
	tabLevel = 0;

#ifdef DEBUG
	driver_out = new IOLibDriverPOSIX(fileName, OS_APPEND);
#else
	driver_out = new IOLibDriverZero();
#endif

	file_out = new OutStream(STREAM_DRIVER, driver_out);

	file_out->SetPackageSize(1);
}

bonkEncDebug::~bonkEncDebug()
{
	delete file_out;

	delete driver_out;
}

Int bonkEncDebug::OutputString(String string)
{
	for (Int i = 0; i < tabLevel; i++) file_out->OutputString("\t");

	file_out->OutputString(string);

	return Success;
}

Int bonkEncDebug::OutputLine(String string)
{
	return OutputString(string.Append("\n"));
}

Int bonkEncDebug::OutputVariable(String name, Int value)
{
	for (Int i = 0; i < tabLevel; i++) file_out->OutputString("\t");

	file_out->OutputLine(String("Integer variable \'").Append(name).Append("\': ").Append(String::FromInt(value)));

	return Success;
}

Int bonkEncDebug::OutputVariable(String name, String value)
{
	for (Int i = 0; i < tabLevel; i++) file_out->OutputString("\t");

	file_out->OutputLine(String("String variable \'").Append(name).Append("\': ").Append(value));

	return Success;
}

Int bonkEncDebug::EnterMethod(String name)
{
	for (Int i = 0; i < tabLevel; i++) file_out->OutputString("\t");

	methods.AddEntry(name);

	file_out->OutputString(String("Entering method \'").Append(name).Append("\'.\n"));

	tabLevel++;

	return Success;
}

Int bonkEncDebug::LeaveMethod()
{
	tabLevel--;

	for (Int i = 0; i < tabLevel; i++) file_out->OutputString("\t");

	file_out->OutputString(String("Leaving method \'").Append(methods.GetLastEntry()).Append("\'.\n"));

	methods.RemoveEntry(methods.GetNthEntryIndex(methods.GetNOfEntries() - 1));

	return Success;
}
