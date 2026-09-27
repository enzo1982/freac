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

#ifndef H_TAGEDIT_MULTIVALUEEDITBOX
#define H_TAGEDIT_MULTIVALUEEDITBOX

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BoCA
{
	class MultiValueEditBox : public EditBox
	{
		private:
			String				 key;
			Text				*label;

			Array<String>			 values;
			String				 proposedValue;

			String				 GetDisplayText() const		{ return String::Implode(values, "; "); }

			PopupMenu			*GetMenu();
		public:
							 MultiValueEditBox(const String &, Text *, const Point &, const Size &);
			virtual				~MultiValueEditBox();

			virtual Int			 Process(Int, Int, Int);

			Void				 SetValues(const Array<String> &);
			const Array<String>		&GetValues() const		{ return values; }

			Bool				 IsReadOnly() const		{ return values.Length() > 1; }
			const String			&GetKey() const			{ return key; }
			const String			&GetProposedValue() const	{ return proposedValue; }
			String				 GetLabelText() const;
		signals:
			Signal1<Void, MultiValueEditBox *> onRequestValueList;
		slots:
			Void				 OnDoubleClick(const Point &);
			Void				 OnEditValues();
			Void				 OnInputText(const String &);
			Void				 OnSelectDropDownEntry(ListEntry *);
	};
};

#endif
