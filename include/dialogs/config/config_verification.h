 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CONFIG_VERIFICATION
#define H_FREAC_CONFIG_VERIFICATION

#include <smooth.h>
#include <boca.h>

namespace BonkEnc
{
	class ConfigureVerification : public BoCA::ConfigLayer
	{
		private:
			GroupBox	*group_verification;
			CheckBox	*check_input;
			Text		*text_input;
			CheckBox	*check_output;
			Text		*text_output;

			Bool		 verifyInput;
			Bool		 verifyOutput;
		public:
					 ConfigureVerification();
					~ConfigureVerification();

			Int		 SaveSettings();
	};
};

#endif
