 /* fre:ac - free audio converter
  * Copyright (C) 2001-2019 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/job.h>
#include <dialogs/error.h>

#include <time.h>

Array<freac::Job *>		 freac::Job::scheduled;
Array<freac::Job *>		 freac::Job::planned;
Array<freac::Job *>		 freac::Job::running;

Array<freac::Job *>		 freac::Job::all;

Threads::Mutex			 freac::Job::mutex;

Signal0<Void>			 freac::Job::onChange;

Signal1<Void, freac::Job *>	 freac::Job::onPlanJob;
Signal1<Void, freac::Job *>	 freac::Job::onRunJob;
Signal1<Void, freac::Job *>	 freac::Job::onFinishJob;

freac::Job::Job() : ListEntry("Job")
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Jobs");

	configuration	= BoCA::Config::Copy();

	progressLabel	= new Text(i18n->TranslateString("%1:", "Characters").Replace("%1", i18n->TranslateString("Progress")), Point(7, 23));

	progress	= new Progressbar(Point(progressLabel->GetX() + progressLabel->GetUnscaledTextWidth() + 7, progressLabel->GetY() - 3), Size(200, 0), OR_HORZ, PB_NOTEXT, 0, 1000, 0);

	timeValue	= new EditBox("00:00", Point(42, progress->GetY()), Size(34, 0), 0);
	timeValue->SetOrientation(OR_UPPERRIGHT);
	timeValue->Deactivate();

	timeLabel	= new Text(i18n->TranslateString("%1:", "Characters").Replace("%1", i18n->TranslateString("Time left")), Point(0, progressLabel->GetY()));
	timeLabel->SetX(timeLabel->GetUnscaledTextWidth() + timeValue->GetWidth() + 15);
	timeLabel->SetOrientation(OR_UPPERRIGHT);

	progressValue	= new EditBox(i18n->TranslateString("%1%", "Technical").Replace("%1", "0"), Point(timeLabel->GetX() + 41, progress->GetY()), Size(34, 0), 0);
	progressValue->SetOrientation(OR_UPPERRIGHT);
	progressValue->Deactivate();

	closeHotspot	= new Hotspot(Point(12, 3), Size(9, 9));
	closeHotspot->SetOrientation(OR_UPPERRIGHT);
	closeHotspot->Deactivate();
	closeHotspot->onLeftButtonClick.Connect(&Job::OnClickToClose, this);

	Add(progressLabel);
	Add(progress);
	Add(progressValue);

	Add(timeLabel);
	Add(timeValue);

	Add(closeHotspot);

	SetHeight(51);

	onChangeSize.Connect(&Job::OnChangeSize, this);
	onLeftButtonDoubleClick.Connect(&Job::OnDoubleClick, this);

	all.Add(this, GetHandle());

	errors.EnableLocking();

	startTicks	    = 0;
	previousSecondsLeft = 0;

	/* Notify about jobs change.
	 */
	mutex.Lock();

	onChange.Emit();

	mutex.Release();
}

freac::Job::~Job()
{
	BoCA::Config::Free(configuration);

	scheduled.Remove(GetHandle());
	planned.Remove(GetHandle());
	running.Remove(GetHandle());

	all.Remove(GetHandle());

	/* Notify about jobs change.
	 */
	mutex.Lock();

	onChange.Emit();

	mutex.Release();

	/* Delete sub-objects.
	 */
	DeleteObject(progressLabel);
	DeleteObject(progress);
	DeleteObject(progressValue);

	DeleteObject(timeLabel);
	DeleteObject(timeValue);

	DeleteObject(closeHotspot);
}

Int freac::Job::Schedule()
{
	scheduled.InsertAtPos(0, this, GetHandle());

	return Success();
}

Int freac::Job::RunPrecheck()
{
	scheduled.Remove(GetHandle());

	if (Precheck() != Success()) return Error();

	planned.Add(this, GetHandle());

	/* Notify about planned job.
	 */
	EnterProtectedRegion();

	mutex.Lock();

	onPlanJob.Emit(this);

	mutex.Release();

	LeaveProtectedRegion();

	return Success();
}

Int freac::Job::Run()
{
	planned.Remove(GetHandle());
	running.Add(this, GetHandle());

	/* Notify about running job.
	 */
	EnterProtectedRegion();

	mutex.Lock();

	onRun.Emit();
	onRunJob.Emit(this);

	mutex.Release();

	LeaveProtectedRegion();

	/* Actually run job.
	 */
	startTicks	    = S::System::System::Clock();
	previousSecondsLeft = 0;

	Perform();

	running.Remove(GetHandle());

	closeHotspot->Activate();

	/* Notify about finished job.
	 */
	EnterProtectedRegion();

	mutex.Lock();

	onFinish.Emit();
	onFinishJob.Emit(this);

	mutex.Release();

	LeaveProtectedRegion();

	return Success();
}

Error freac::Job::Precheck()
{
	return Success();
}

Bool freac::Job::ReadyToRun()
{
	return True;
}

Void freac::Job::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	Surface	*surface    = GetDrawSurface();

	surface->StartPaint(Rect(GetRealPosition(), GetRealSize()));

	progress->Hide();
	progressValue->Hide();

	timeLabel->Hide();
	timeValue->Hide();

	progress->SetWidth(clientSize.cx - progressLabel->GetUnscaledTextWidth() - progressValue->GetWidth() - timeLabel->GetUnscaledTextWidth() - timeValue->GetWidth() - 36);
	progressValue->SetX(timeLabel->GetUnscaledTextWidth() + timeValue->GetWidth() + 56);

	timeLabel->SetX(timeLabel->GetUnscaledTextWidth() + timeValue->GetWidth() + 15);
	timeValue->SetX(timeValue->GetWidth() + 8);

	progress->Show();
	progressValue->Show();

	timeLabel->Show();
	timeValue->Show();

	surface->EndPaint();
}

Void freac::Job::OnDoubleClick()
{
	if (closeHotspot->IsActive() && closeHotspot->IsMouseOver()) return;

	ErrorDialog	 dialog(GetErrors());

	dialog.ShowDialog();
}

Void freac::Job::OnClickToClose()
{
	all.Remove(GetHandle());

	/* Notify about jobs change.
	 */
	EnterProtectedRegion();

	mutex.Lock();

	onChange.Emit();

	mutex.Release();

	Object::DeleteObject(this);

	LeaveProtectedRegion();
}

Int freac::Job::Paint(Int message)
{
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	Surface	*surface = GetDrawSurface();
	Rect	 frame	 = Rect(GetRealPosition(), GetRealSize());

	switch (message)
	{
		case SP_PAINT:
		case SP_MOUSEIN:
		case SP_MOUSEOUT:
			surface->StartPaint(GetVisibleArea());

			SetVisibleDirect(False);

			if (IsSelected() || IsMouseOver()) SetBackgroundColor(Setup::LightGrayColor);
			else				   SetBackgroundColor(Setup::BackgroundColor);

			SetVisibleDirect(True);

			surface->Box(frame - Size(0, 1), GetBackgroundColor(), Rect::Filled);
			surface->SetText(text, frame + Point(1, 1) - Size(2, 2), font);

			if (IsMouseOver() && closeHotspot->IsActive())
			{
				surface->Line(Point(frame.right - 10, frame.top +  4), Point(frame.right - 4, frame.top + 10), Setup::DividerDarkColor);
				surface->Line(Point(frame.right - 11, frame.top +  4), Point(frame.right - 4, frame.top + 11), Setup::TextColor);
				surface->Line(Point(frame.right - 11, frame.top +  5), Point(frame.right - 5, frame.top + 11), Setup::DividerDarkColor);

				surface->Line(Point(frame.right - 11, frame.top +  9), Point(frame.right - 5, frame.top +  3), Setup::DividerDarkColor);
				surface->Line(Point(frame.right - 11, frame.top + 10), Point(frame.right - 4, frame.top +  3), Setup::TextColor);
				surface->Line(Point(frame.right - 10, frame.top + 10), Point(frame.right - 4, frame.top +  4), Setup::DividerDarkColor);
			}

			Widget::Paint(SP_PAINT);

			surface->EndPaint();

			return Success();
	}

	return Widget::Paint(message);
}

Int freac::Job::SetText(const String &newText)
{
	if (text == newText) return Success();

	if (IsRegistered() && IsVisible())
	{
		Surface	*surface = container->GetDrawSurface();

		surface->StartPaint(GetVisibleArea());

		Widget::SetText(newText);

		surface->EndPaint();
	}
	else
	{
		Widget::SetText(newText);
	}

	return Success();
}

Int freac::Job::SetProgress(Int nValue)
{
	progress->SetValue(nValue);
	progressValue->SetText(BoCA::I18n::Get()->TranslateString("%1%", "Technical").Replace("%1", String::FromInt(Math::Round(Float(nValue) / 10.0))));

	Int64	 totalTicks  = S::System::System::Clock() - startTicks;
	Int	 secondsLeft = (Int) (totalTicks * ((1000.0 - nValue) / nValue)) / 1000 + (nValue < 1000 ? 1 : 0);

	if (secondsLeft < previousSecondsLeft || secondsLeft >= previousSecondsLeft + 2)
	{
		String	 secondsString = SecondsToString(secondsLeft);

		timeValue->SetText(secondsString);

		if (timeValue->GetWidth() != Math::Max(34, timeValue->GetUnscaledTextWidth() + 6))
		{
			timeValue->SetWidth(Math::Max(34, timeValue->GetUnscaledTextWidth() + 6));

			OnChangeSize(GetSize());
		}

		previousSecondsLeft = secondsLeft;
	}

	return Success();
}

Int freac::Job::GetProgress() const
{
	return progress->GetValue();
}

String freac::Job::SecondsToString(UnsignedInt seconds)
{
	if (seconds >= 360000) return "??:??:??";

	static String	 zeroString  = "0";
	static String	 colonString = ":";

	String	 buffer;
	String	 text;

	/* Append hours.
	 */
	if (seconds >= 3600)
	{
		buffer = String::FromInt(seconds / 3600);

		if (buffer.Length() == 1) text.Append(zeroString);

		text.Append(buffer);
		text.Append(colonString);
	}

	/* Append minutes.
	 */
	buffer = String::FromInt(seconds % 3600 / 60);

	if (buffer.Length() == 1) text.Append(zeroString);

	text.Append(buffer);
	text.Append(colonString);

	/* Append seconds.
	 */
	buffer = String::FromInt(seconds % 3600 % 60);

	if (buffer.Length() == 1) text.Append(zeroString);

	text.Append(buffer);

	return text;
}
