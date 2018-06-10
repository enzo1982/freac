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

#ifndef H_DONATE_LAYER_SEPA
#define H_DONATE_LAYER_SEPA

#include "method.h"

namespace BoCA
{
	class LayerSEPA : public Layer
	{
		private:
			Text	*text_donate;

			Image	*image_sepa;

			Text	*text_owner;
			Text	*text_iban;
			Text	*text_bic;

			Cursor	*cursor_owner;
			Cursor	*cursor_iban;
			Cursor	*cursor_bic;
		public:
				 LayerSEPA();
				~LayerSEPA();
	};

	class MethodSEPA : public PaymentMethod
	{
		private:
			Layer	*layer;
		public:
				 MethodSEPA();
				~MethodSEPA();

			Layer	*GetLayer();
	};
};

#endif
