 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/job.h>
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
	progressLabel	= new Text("Progress:", Point(7, 23));

	progress	= new Progressbar(Point(progressLabel->GetX() + progressLabel->textSize.cx + 7, progressLabel->GetY() - 3), Size(200, 0), OR_HORZ, PB_NOTEXT, 0, 1000, 0);

	timeValue	= new EditBox("00:00", Point(43, progress->GetY()), Size(35, 0), 0);
	timeValue->SetOrientation(OR_UPPERRIGHT);
	timeValue->Deactivate();

	timeLabel	= new Text("Time left:", Point(0, progressLabel->GetY()));
	timeLabel->SetX(timeLabel->textSize.cx + timeValue->GetWidth() + 15);
	timeLabel->SetOrientation(OR_UPPERRIGHT);

	progressValue	= new EditBox("0%", Point(timeLabel->GetX() + 41, progress->GetY()), Size(34, 0), 0);
	progressValue->SetOrientation(OR_UPPERRIGHT);
	progressValue->Deactivate();

	Add(progressLabel);
	Add(progress);
	Add(progressValue);

	Add(timeLabel);
	Add(timeValue);

	SetHeight(50);

	onChangeSize.Connect(&Job::OnChangeSize, this);

	onMouseOver.Connect(&Job::OnMouseOver, this);
	onMouseOut.Connect(&Job::OnMouseOut, this);

	all.Add(this, GetHandle());

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

	startTicks = clock();

	Perform();

	running.Remove(GetHandle());

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

	progress->SetWidth(clientSize.cx - progressLabel->textSize.cx - progressValue->GetWidth() - timeLabel->textSize.cx - timeValue->GetWidth() - 36);
}

Void BonkEnc::Job::OnMouseOver()
{
	GUI::Font	 font = progressLabel->GetFont();

	font.SetColor(Setup::GradientTextColor);

	progressLabel->SetFont(font);

	Paint(SP_PAINT);
}

Void BonkEnc::Job::OnMouseOut()
{
	GUI::Font	 font = progressLabel->GetFont();

	font.SetColor(Setup::TextColor);

	progressLabel->SetFont(font);

	Paint(SP_PAINT);
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
	progressValue->SetText(String::FromInt(Math::Round(Float(nValue) / 10.0)).Append("%"));

	Int	 ticks = clock() - startTicks;
	Int	 secondsLeft = (Int) (ticks * ((Float(CLOCKS_PER_SEC) - nValue) / nValue)) / CLOCKS_PER_SEC + (nValue < 1000 ? 1 : 0);

	String	 buffer = String::FromInt(secondsLeft / 60);
	String	 text = "0";

	if (buffer.Length() == 1) text.Append(buffer);
	else			  text.Copy(buffer);

	text.Append(":");

	buffer = String::FromInt(secondsLeft % 60);

	if (buffer.Length() == 1) text.Append(String("0").Append(buffer));
	else			  text.Append(buffer);

	timeValue->SetText(text);

	return Success();
}

Int BonkEnc::Job::GetProgress()
{
	return progress->GetValue();
}
