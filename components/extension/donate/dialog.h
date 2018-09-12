 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_DONATE_DIALOG
#define H_DONATE_DIALOG

#include <smooth.h>
#include <boca.h>

#include "methods/method.h"

using namespace smooth::GUI::Dialogs;

namespace BoCA
{
	class DonateDialog : public Dialog
	{
		private:
			static Array<PaymentMethod *(*)()>		*factories;

			GUI::Window					*mainWnd;
			Titlebar					*mainWnd_titlebar;

			Divider						*divider;

			Button						*button_close;

			Array<PaymentMethod *>				 methods;

			TabWidget					*tab_methods;

			Text						*text_intro;
			Text						*text_donate;
			Text						*text_donate_other;
			Text						*text_thanks;

			CheckBox					*check_remind;

			Bool						 remind;
		public:
			static Bool					 RegisterPaymentMethod(PaymentMethod *(*)());

			static const Array<PaymentMethod *(*)()>	&GetPaymentMethodFactories();
			static Bool					 FreePaymentMethodFactories();

									 DonateDialog();
									~DonateDialog();

			const Error					&ShowDialog();
			const Error					&ShowDialog(Bool);
		slots:
			Void						 Close();
	};
};

#endif
