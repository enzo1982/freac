 /* fre:ac - free audio converter
  * Copyright (C) 2026 Shen-Ta Hsieh
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_TAGEDIT_VALUELIST
#define H_TAGEDIT_VALUELIST

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;
using namespace smooth::GUI::Dialogs;

namespace BoCA
{
	class ValueListDialog : public Dialog
	{
		private:
			GUI::Window			*window;
			Titlebar			*titlebar;

			ListBox				*list_values;
			EditBox				*edit_value;

			Button				*button_add;
			Button				*button_remove;
			Button				*button_up;
			Button				*button_down;
			Button				*button_close;

			Array<String>			 values;
			Bool				 proposed;

			Void				 UpdateList(Int);
			Void				 Changed();
		public:
							 ValueListDialog(const String &, const Array<String> &, const String & = NIL);
							~ValueListDialog();

			const Error			&ShowDialog();
		signals:
			Signal1<Void, const Array<String> &> onChangeValues;
		slots:
			Void				 OnSelectValue();
			Void				 OnEditValue(const String &);
			Void				 OnAdd();
			Void				 OnRemove();
			Void				 OnUp();
			Void				 OnDown();
			Void				 OnClose();
	};
};

#endif
