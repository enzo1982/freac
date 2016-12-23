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

#include "scriptutilities.h"

using namespace smooth::GUI;
using namespace smooth::GUI::Dialogs;

using namespace smooth::Net;

void BoCA::Alert(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	String	 text = (char *) *v8::String::AsciiValue(args[0]);

	QuickMessage(text, "Information", Message::Buttons::Ok, Message::Icon::Information);
}

void BoCA::DownloadURL(const v8::FunctionCallbackInfo<v8::Value> &args)
{
	String	 url = (char *) *v8::String::AsciiValue(args[0]);

	if (url.StartsWith("http://") || url.StartsWith("https://"))
	{
		/* HTTPS is not supported yet, so try using HTTP.
		 */
		url.Replace("https://", "http://");

		Buffer<UnsignedByte>	 buffer;

		do
		{
			Protocols::Protocol	*protocol = Protocols::Protocol::CreateForURL(url);

			protocol->DownloadToBuffer(buffer);

			url = ((Protocols::HTTP *) protocol)->GetResponseHeaderField("Location");

			delete protocol;
		}
		while (url != NIL);

		if (buffer.Size() > 0)
		{
			String	 result;

			for (Int i = 0; i < buffer.Size(); i++) result[i] = buffer[i];

			args.GetReturnValue().Set(v8::String::New(result));
		}
	}
}
