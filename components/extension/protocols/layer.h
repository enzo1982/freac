 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_PROTOCOLS_LAYER
#define H_PROTOCOLS_LAYER

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA;

namespace BoCA
{
	class LayerProtocols : public Layer
	{
		private:
			Text		*text_protocol;
			ComboBox	*combo_protocol;

			MultiEdit	*edit_protocol;
			EditBox		*edit_status;

			Text		*text_errors;
			EditBox		*edit_errors;
			ComboBox	*combo_errors;
			Button		*button_details;

			Bool		 needReselect;
		slots:
			Void		 OnChangeSize(const Size &);

			Void		 UpdateProtocolList();
			Void		 UpdateProtocol(const String &);

			Void		 SelectProtocol();
			Void		 ShowDetails();
		public:
					 LayerProtocols();
					~LayerProtocols();

			Int		 Show();
	};
};

#endif
