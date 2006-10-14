 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FLACCONFIG_
#define _H_FLACCONFIG_

#include <main.h>

namespace BonkEnc
{
	class ConfigureFLAC : public Dialog
	{
		private:
			TabWidget	*tabwidget;

			Layer		*layer_simple;

			GroupBox	*group_preset;
			Text		*text_preset;
			ComboBox	*combo_preset;

			GroupBox	*group_stereo;
			CheckBox	*check_mid_side_stereo;
			CheckBox	*check_loose_mid_side;

			Layer		*layer_advanced;

			GroupBox	*group_format;
			CheckBox	*check_streamable_subset;
			Text		*text_blocksize;
			Slider		*slider_blocksize;
			EditBox		*edit_blocksize;
			Text		*text_blocksize_bytes;

			GroupBox	*group_lpc;
			Text		*text_max_lpc_order;
			Slider		*slider_max_lpc_order;
			Text		*text_max_lpc_order_value;
			CheckBox	*check_exhaustive_model;
			Text		*text_qlp_precision;
			Slider		*slider_qlp_precision;
			Text		*text_qlp_precision_value;
			CheckBox	*check_qlp_precision_search;

			GroupBox	*group_rice;
			Text		*text_min_part_order;
			Slider		*slider_min_part_order;
			Text		*text_min_part_order_value;
			Text		*text_max_part_order;
			Slider		*slider_max_part_order;
			Text		*text_max_part_order_value;

			Divider		*divbar;

			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			Button		*btn_cancel;
			Button		*btn_ok;

			Int		 preset;
			Bool		 streamable_subset;
			Bool		 do_mid_side_stereo;
			Bool		 loose_mid_side_stereo;
			Int		 blocksize;
			Int		 max_lpc_order;
			Int		 qlp_coeff_precision;
			Bool		 do_qlp_coeff_prec_search;
			Bool		 do_exhaustive_model_search;
			Int		 min_residual_partition_order;
			Int		 max_residual_partition_order;

			Config		*currentConfig;

			Void		 OK();
			Void		 Cancel();
		public:
					 ConfigureFLAC();
					~ConfigureFLAC();

			const Error	&ShowDialog();
		slots:
			Void		 SetPreset();
			Void		 SetLPCOrder();
			Void		 SetQLPSearch();
			Void		 SetQLPPrecision();
			Void		 SetRiceOrder();
			Void		 SetStereoMode();
			Void		 SetStreamableSubset();
			Void		 SetBlockSize();
			Void		 EditBlockSize();
	};
};

#endif
