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

#ifndef H_DONATE_LAYER_PAYPAL
#define H_DONATE_LAYER_PAYPAL

#include "method.h"

namespace BoCA
{
	class LayerPayPal : public Layer
	{
		private:
			Text		*text_donate;
			Text		*text_donate_direct;

			ComboBox	*combo_amount;
			CheckBox	*check_monthly;

			Hyperlink	*link_donate;
			Text		*text_donate_now;

			Bool		 monthly;
		public:
					 LayerPayPal();
					~LayerPayPal();
		slots:
			Void		 OnChangeSize(const Size &);
			Void		 OnChangeSettings();
	};

	class MethodPayPal : public PaymentMethod
	{
		private:
			Layer	*layer;
		public:
				 MethodPayPal();
				~MethodPayPal();

			Layer	*GetLayer();
	};
};

#endif
