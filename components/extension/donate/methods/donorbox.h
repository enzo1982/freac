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

#ifndef H_DONATE_LAYER_DONORBOX
#define H_DONATE_LAYER_DONORBOX

#include "method.h"

namespace BoCA
{
	class LayerDonorbox : public Layer
	{
		private:
			Text		*text_donate;

			Hyperlink	*link_donate;

			Image		*image_box;
			Text		*text_box;
		public:
					 LayerDonorbox();
					~LayerDonorbox();
		slots:
			Void		 OnChangeSize(const Size &);
	};

	class MethodDonorbox : public PaymentMethod
	{
		private:
			Layer	*layer;
		public:
				 MethodDonorbox();
				~MethodDonorbox();

			Layer	*GetLayer();
	};
};

#endif
