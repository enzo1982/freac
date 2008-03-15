 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_MAIN_PROTOCOL_
#define _H_MAIN_PROTOCOL_

#include <bonkenc.h>

namespace BonkEnc
{
	class LayerProtocol : public Layer
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
		slots:
			Void		 OnChangeSize(const Size &);

			Void		 SelectProtocol();
			Void		 ShowDetails();
		public:
					 LayerProtocol();
					~LayerProtocol();
	};
};

#endif
