 /* fre:ac - free audio converter
  * Copyright (C) 2001-2023 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_TAGEDIT_ALBUM_ART_DISPLAY
#define H_TAGEDIT_ALBUM_ART_DISPLAY

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;
using namespace smooth::GUI::Dialogs;

namespace BoCA
{
	class AlbumArtDisplay : public Dialog
	{
		private:
			GUI::Window	*window;

			Image		*image;
		public:
					 AlbumArtDisplay(const Bitmap &);
					~AlbumArtDisplay();

			const Error	&ShowDialog();
		slots:
			Void		 Close();
	};
};

#endif
