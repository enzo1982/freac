 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/job.h>
#include <dialogs/error.h>

#include <boca.h>
#include <time.h>

Array<BonkEnc::Job *>		 BonkEnc::Job::planned;
Array<BonkEnc::Job *>		 BonkEnc::Job::running;

Array<BonkEnc::Job *>		 BonkEnc::Job::all;

Signal0<Void>			 BonkEnc::Job::onChange;

Signal1<Void, BonkEnc::Job *>	 BonkEnc::Job::onPlanJob;
Signal1<Void, BonkEnc::Job *>	 BonkEnc::Job::onRunJob;
Signal1<Void, BonkEnc::Job *>	 BonkEnc::Job::onFinishJob;

BonkEnc::Job::Job() : ListEntry("Job")
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	progressLabel	= new Text("Progress:", Point(7, 23));

	progress	= new Progressbar(Point(progressLabel->GetX() + progressLabel->GetUnscaledTextWidth() + 7, progressLabel->GetY() - 3), Size(200, 0), OR_HORZ, PB_NOTEXT, 0, 1000, 0);

	timeValue	= new EditBox("00:00", Point(43, progress->GetY()), Size(35, 0), 0);
	timeValue->SetOrientation(OR_UPPERRIGHT);
	timeValue->Deactivate();

	timeLabel	= new Text("Time left:", Point(0, progressLabel->GetY()));
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

	onChange.Emit();
}

BonkEnc::Job::~Job()
{
	planned.Remove(GetHandle());
	running.Remove(GetHandle());

	all.Remove(GetHandle());

	onChange.Emit();

	DeleteObject(progressLabel);
	DeleteObject(progress);
	DeleteObject(progressValue);

	DeleteObject(timeLabel);
	DeleteObject(timeValue);

	DeleteObject(closeHotspot);
}

Int BonkEnc::Job::Schedule()
{
	planned.Add(this, GetHandle());

	EnterProtectedRegion();

	onPlanJob.Emit(this);

	LeaveProtectedRegion();

	return Success();
}

Int BonkEnc::Job::Run()
{
	planned.Remove(GetHandle());
	running.Add(this, GetHandle());

	EnterProtectedRegion();

	onRun.Emit();
	onRunJob.Emit(this);

	LeaveProtectedRegion();

	startTicks	    = S::System::System::Clock();
	previousSecondsLeft = 0;

	Perform();

	running.Remove(GetHandle());

	closeHotspot->Activate();

	EnterProtectedRegion();

	onFinish.Emit();
	onFinishJob.Emit(this);

	LeaveProtectedRegion();

	return Success();
}

Bool BonkEnc::Job::ReadyToRun()
{
	return True;
}

Void BonkEnc::Job::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	progress->SetWidth(clientSize.cx - progressLabel->GetUnscaledTextWidth() - progressValue->GetWidth() - timeLabel->GetUnscaledTextWidth() - timeValue->GetWidth() - 36);
}

Void BonkEnc::Job::OnDoubleClick()
{
	if (closeHotspot->IsActive() && closeHotspot->IsMouseOver()) return;

	ErrorDialog	 dialog(GetErrors());

	dialog.ShowDialog();
}

Void BonkEnc::Job::OnClickToClose()
{
	EnterProtectedRegion();

	all.Remove(GetHandle());
	onChange.Emit();

	Object::DeleteObject(this);

	LeaveProtectedRegion();
}

Int BonkEnc::Job::Paint(Int message)
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

Int BonkEnc::Job::SetText(const String &newText)
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

Int BonkEnc::Job::SetProgress(Int nValue)
{
	progress->SetValue(nValue);
	progressValue->SetText(BoCA::I18n::Get()->TranslateString("%1%", "Technical").Replace("%1", String::FromInt(Math::Round(Float(nValue) / 10.0))));

	Int	 totalTicks  = S::System::System::Clock() - startTicks;
	Int	 secondsLeft = (Int) (totalTicks * ((1000.0 - nValue) / nValue)) / 1000 + (nValue < 1000 ? 1 : 0);

	if (secondsLeft < previousSecondsLeft || secondsLeft >= previousSecondsLeft + 2)
	{
		String	 buffer = String::FromInt(secondsLeft / 60);
		String	 text	= "0";

		if (buffer.Length() == 1) text.Append(buffer);
		else			  text.Copy(buffer);

		text.Append(":");

		buffer = String::FromInt(secondsLeft % 60);

		if (buffer.Length() == 1) text.Append(String("0").Append(buffer));
		else			  text.Append(buffer);

		timeValue->SetText(text);

		previousSecondsLeft = secondsLeft;
	}

	return Success();
}

Int BonkEnc::Job::GetProgress()
{
	return progress->GetValue();
}
