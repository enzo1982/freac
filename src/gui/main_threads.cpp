 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <gui/main_threads.h>
#include <jobs/job.h>

using namespace smooth::GUI::Dialogs;

BonkEnc::LayerThreads::LayerThreads() : Layer("Jobs")
{
	text_progress	= new Text("Progress:", Point(7, 11));

	list_threads	= new ListBox(Point(7, 35), Size(500, 360));
	list_threads->onSelectEntry.Connect(&LayerThreads::OnSelectJob, this);
	list_threads->EnableLocking();

	text_errors	= new Text("Errors / Warnings:", Point(7, 25));
	text_errors->SetOrientation(OR_LOWERLEFT);

	edit_errors	= new EditBox(NIL, Point(text_errors->GetUnscaledTextWidth() + 14, 28), Size(25, 0));
	edit_errors->SetOrientation(OR_LOWERLEFT);
	edit_errors->Deactivate();

	combo_errors	= new ComboBox(Point(text_errors->GetUnscaledTextWidth() + 47, 28), Size(250, 0));
	combo_errors->SetOrientation(OR_LOWERLEFT);

	button_details	= new Button("Details", NIL, Point(87, 29), Size(80, 0));
	button_details->onAction.Connect(&LayerThreads::ShowDetails, this);
	button_details->SetOrientation(OR_LOWERRIGHT);

	Add(text_progress);
	Add(list_threads);

	Add(text_errors);
	Add(edit_errors);
	Add(combo_errors);
	Add(button_details);

	OnChangeJobs();

	onChangeSize.Connect(&LayerThreads::OnChangeSize, this);

	Job::onChange.Connect(&LayerThreads::OnChangeJobs, this);
}

BonkEnc::LayerThreads::~LayerThreads()
{
	Job::onChange.Disconnect(&LayerThreads::OnChangeJobs, this);

	DeleteObject(text_progress);
	DeleteObject(list_threads);

	DeleteObject(text_errors);
	DeleteObject(edit_errors);
	DeleteObject(combo_errors);
	DeleteObject(button_details);
}

Void BonkEnc::LayerThreads::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	list_threads->SetSize(clientSize - Size(15, 72));

	combo_errors->SetWidth(clientSize.cx - text_errors->GetUnscaledTextWidth() - 142);
}

Void BonkEnc::LayerThreads::OnChangeJobs()
{
	list_threads->RemoveAllEntries();

	text_errors->Deactivate();
	edit_errors->SetText("0/0");

	combo_errors->RemoveAllEntries();
	combo_errors->Deactivate();

	button_details->Deactivate();

	const Array<Job *>	&jobs = Job::GetAllJobs();

	foreach (Job *job, jobs)
	{
		list_threads->Add(job);

		if (job->IsSelected()) OnSelectJob(job);
	}
}

Void BonkEnc::LayerThreads::OnSelectJob(ListEntry *entry)
{
	Job	*job = (Job *) entry;

	const Array<String>	errors	 = job->GetErrors();
	const Array<String>	warnings = job->GetWarnings();

	text_errors->Activate();
	edit_errors->SetText(String::FromInt(errors.Length()).Append("/").Append(String::FromInt(warnings.Length())));

	combo_errors->RemoveAllEntries();

	if (errors.Length() > 0 || warnings.Length() > 0)
	{
		combo_errors->Activate();
		button_details->Activate();

		foreach (const String &error, errors)	  combo_errors->AddEntry(error);
		foreach (const String &warning, warnings) combo_errors->AddEntry(warning);

		combo_errors->Paint(SP_PAINT);
	}
	else
	{
		combo_errors->Deactivate();
		button_details->Deactivate();
	}
}

Void BonkEnc::LayerThreads::ShowDetails()
{
	QuickMessage(combo_errors->GetSelectedEntry()->GetText(), "Error details", Message::Buttons::Ok, Message::Icon::Information);
}
