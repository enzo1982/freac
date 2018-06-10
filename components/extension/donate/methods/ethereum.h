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

#ifndef H_DONATE_LAYER_ETHEREUM
#define H_DONATE_LAYER_ETHEREUM

#include "method.h"

namespace BoCA
{
	class LayerEthereum : public Layer
	{
		private:
			Hyperlink	*link_qrcode;

			Text		*text_donate;

			Cursor		*cursor_ether;

			Text		*text_other;
		public:
					 LayerEthereum();
					~LayerEthereum();
	};

	class MethodEthereum : public PaymentMethod
	{
		private:
			Layer	*layer;
		public:
				 MethodEthereum();
				~MethodEthereum();

			Layer	*GetLayer();
	};
};

#endif
