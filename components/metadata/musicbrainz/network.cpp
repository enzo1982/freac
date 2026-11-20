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

#include "network.h"

#if defined __APPLE__
#	include <mach/mach_time.h>
#elif !defined __WIN32__
#	include <time.h>
#endif

Threads::Mutex	 BoCA::MusicBrainzNetwork::mutex;
UnsignedInt64	 BoCA::MusicBrainzNetwork::lastRequestTime = 0;

/* Requests to musicbrainz.org are at least this far apart. On status
 * 503, a request is sent once more after the same time.
 */
static const Int	 RequestInterval	= 1000;	// Milliseconds.
static const Int	 MaxAttempts		= 2;

static const Int	 StatusServiceUnavailable = 503;

/* Milliseconds from a fixed start. On POSIX systems, smooth's Clock
 * follows the wall time, which can go back. The waits here use a clock
 * that does not go back, so that a clock change does not stop requests.
 * macOS has clock_gettime only from 10.12, so mach_absolute_time is
 * used there.
 */
static UnsignedInt64 MonotonicClock()
{
#if defined __WIN32__
	return S::System::System::Clock();
#elif defined __APPLE__
	static mach_timebase_info_data_t	 timebase = { 0, 0 };

	if (timebase.denom == 0) mach_timebase_info(&timebase);

	return BoCA::MusicBrainzNetwork::TicksToMilliseconds(mach_absolute_time(), timebase.numer, timebase.denom);
#else
	timespec	 ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	return ts.tv_sec * UnsignedInt64(1000) + ts.tv_nsec / 1000000;
#endif
}

/* Convert ticks of a clock with a timebase of numer/denom nanoseconds
 * per tick to milliseconds. It divides first, so that a large numer, as
 * on PowerPC Macs, does not overflow. The remainder keeps the result
 * exact.
 */
UnsignedInt64 BoCA::MusicBrainzNetwork::TicksToMilliseconds(UnsignedInt64 ticks, UnsignedInt numer, UnsignedInt denom)
{
	if (denom == 0) return 0;

	return (ticks / denom * numer + ticks % denom * numer / denom) / 1000000;
}

/* The end of the scheme in a URL, as in https://.
 */
static const String	 SchemeEnd		= "://";

/* The contact for the operators of MusicBrainz in the user agent.
 */
static const String	 ContactURL		= "https://www.freac.org/";

/* The path of the web service, which the query paths start with.
 */
static const String	 WebServicePath		= "/ws/2";

/* A server URL as typed by the user, without spaces, without
 * slashes at the end and without the web service path at the end,
 * so that "https://musicbrainz.org/ws/2/" gives the same server
 * as "https://musicbrainz.org".
 */
String BoCA::MusicBrainzNetwork::NormalizeServerURL(const String &url)
{
	String	 server = url.Trim();

	while (server.EndsWith("/")) server = server.Head(server.Length() - 1);

	if (server.ToLower().EndsWith(WebServicePath))
	{
		server = server.Head(server.Length() - WebServicePath.Length());

		while (server.EndsWith("/")) server = server.Head(server.Length() - 1);
	}

	return server;
}

/* The host name of a URL, in lower case, without port.
 */
String BoCA::MusicBrainzNetwork::GetHost(const String &url)
{
	String	 host	= url.Trim().ToLower();
	Int	 scheme = host.Find(SchemeEnd);

	if (scheme >= 0) host = host.Tail(host.Length() - scheme - SchemeEnd.Length());

	for (Int i = 0; i < host.Length(); i++)
	{
		if (host[i] == '/' || host[i] == ':') { host = host.Head(i); break; }
	}

	return host;
}

/* The user agent that MusicBrainz asks clients to send, of the form
 * "name/version ( contact )". The name loses spaces and colons, the
 * version its "v" and the build date that can follow it, so that
 * fre:ac v1.1.7 (20260926) sends "freac/1.1.7 ( https://www.freac.org/ )".
 */
String BoCA::MusicBrainzNetwork::GetUserAgent(const String &clientName, const String &clientVersion)
{
	String	 client	 = clientName != NIL ? clientName : String("fre:ac");
	String	 name;
	String	 version = clientVersion;

	for (Int i = 0; i < client.Length(); i++)
	{
		if (client[i] != ' ' && client[i] != ':') name[name.Length()] = client[i];
	}

	if (version.Contains(" "))   version = version.Head(version.Find(" "));
	if (version.StartsWith("v")) version = version.Tail(version.Length() - 1);
	if (version == NIL)	     version = "unknown";

	return name.Append("/").Append(version).Append(" ( ").Append(ContactURL).Append(" )");
}

/* Whether a host is musicbrainz.org itself or one of its subdomains.
 */
Bool BoCA::MusicBrainzNetwork::IsRateLimited(const String &host)
{
	return host == "musicbrainz.org" || host.EndsWith(".musicbrainz.org");
}

/* Wait for a number of milliseconds in short steps. Returns False
 * as soon as the flag that cancel points to is True.
 */
Bool BoCA::MusicBrainzNetwork::Pause(Int milliseconds, const Bool *cancel)
{
	UnsignedInt64	 end = MonotonicClock() + milliseconds;

	while (True)
	{
		if (cancel != NIL && *cancel) return False;

		UnsignedInt64	 now = MonotonicClock();

		if (now >= end) break;

		S::System::System::Sleep(Int(Math::Min(end - now, (UnsignedInt64) 10)));
	}

	return cancel == NIL || !*cancel;
}

/* Keep all requests to musicbrainz.org and its subdomains at least
 * one second apart. Requests to other servers, such as mirrors, do
 * not wait.
 *
 * The time slot of a request is reserved under the mutex, and the
 * wait for it happens after the mutex is released. So no thread
 * holds the mutex while it waits, and a canceled request stops its
 * wait at once and gives its slot back when it can. Returns False if
 * the request was canceled.
 */
Bool BoCA::MusicBrainzNetwork::WaitForTurn(const String &url, const Bool *cancel)
{
	if (!IsRateLimited(GetHost(url))) return True;

	Int		 wait	  = 0;
	UnsignedInt64	 next	  = 0;
	UnsignedInt64	 previous = 0;

	{
		Threads::Lock	 lock(mutex);

		UnsignedInt64	 now  = MonotonicClock();

		next = now;

		if (lastRequestTime != 0 && lastRequestTime + RequestInterval > now) next = lastRequestTime + RequestInterval;

		wait		= Int(next - now);
		previous	= lastRequestTime;
		lastRequestTime = next;
	}

	if (Pause(wait, cancel)) return True;

	/* Give the slot back if no other request reserved a later one.
	 */
	Threads::Lock	 lock(mutex);

	if (lastRequestTime == next) lastRequestTime = previous;

	return False;
}

/* Use the proxy settings that fre:ac keeps in the freedb category.
 */
Void BoCA::MusicBrainzNetwork::ApplyProxy(const Config *config, Net::Protocols::HTTP &http)
{
	Int	 mode = config->GetIntValue(ProxyConfig::ConfigID, ProxyConfig::ModeID, ProxyConfig::ModeDefault);

	if (mode == 0) return;

	http.SetProxy(config->GetStringValue(ProxyConfig::ConfigID, ProxyConfig::ServerID, ProxyConfig::ServerDefault), config->GetIntValue(ProxyConfig::ConfigID, ProxyConfig::PortID, ProxyConfig::PortDefault));
	http.SetProxyAuth(config->GetStringValue(ProxyConfig::ConfigID, ProxyConfig::UserID, NIL), config->GetStringValue(ProxyConfig::ConfigID, ProxyConfig::PasswordID, NIL));

	switch (mode)
	{
		case 1: http.SetProxyMode(Net::Protocols::HTTP_PROXY_HTTP);   break;
		case 2: http.SetProxyMode(Net::Protocols::HTTP_PROXY_HTTPS);  break;
		case 3: http.SetProxyMode(Net::Protocols::HTTP_PROXY_SOCKS4); break;
		case 4: http.SetProxyMode(Net::Protocols::HTTP_PROXY_SOCKS5); break;
	}
}

/* Send a GET request. On status 503, wait and send it once more.
 * Returns the HTTP status, or 0 if the server did not answer or the
 * request was canceled. If the flag that cancel points to becomes
 * True, a running request stops too.
 */
Int BoCA::MusicBrainzNetwork::Get(const Config *config, const String &url, Buffer<UnsignedByte> &result, const Bool *cancel)
{
	Protocol	*protocol = Protocol::Get("MusicBrainz communication");
	String		 agent	  = GetUserAgent(Application::Get()->getClientName.Call(), Application::Get()->getClientVersion.Call());
	Int		 status	  = 0;

	MusicBrainzCancelFlag	 cancelFlag(cancel);

	for (Int attempt = 1; attempt <= MaxAttempts; attempt++)
	{
		if (cancel != NIL && *cancel) return 0;

		if (attempt > 1 && !Pause(RequestInterval, cancel)) return 0;

		if (!WaitForTurn(url, cancel)) return 0;

		protocol->Write(String("MusicBrainz: GET ").Append(url));

		Net::Protocols::HTTP	 http(url);

		http.SetHeaderField("User-Agent", agent);

		/* The protocol connects a constant False; replace it.
		 */
		http.doCancelDownload.DisconnectAll();
		http.doCancelDownload.Connect(&MusicBrainzCancelFlag::IsSet, &cancelFlag);

		ApplyProxy(config, http);

		if (http.DownloadToBuffer(result) == Error())
		{
			if (cancel != NIL && *cancel) protocol->Write(String("MusicBrainz: Canceled ").Append(url));
			else			      protocol->Write(String("MusicBrainz: Error connecting to ").Append(url), MessageTypeError);

			return 0;
		}

		status = http.GetResponseHeaderField("Status-Code").ToInt();

		protocol->Write(String("MusicBrainz: Status ").Append(String::FromInt(status)));

		if (status != StatusServiceUnavailable) break;
	}

	return status;
}
