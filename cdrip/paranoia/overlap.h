/***
 * CopyPolicy: GNU Public License 2 applies
 * Copyright (C) by Monty (xiphmont@mit.edu)
 ***/

#ifndef _OVERLAP_H_
#define _OVERLAP_H_

extern void offset_add_value(cdrom_paranoia *p,offsets *o,long value,
			     void(*callback)(long,int));
extern void offset_clear_settings(offsets *o);
extern void offset_adjust_settings(cdrom_paranoia *p, 
				   void(*callback)(long,int));
extern void i_paranoia_trim(cdrom_paranoia *p,long beginword,long endword);
extern void paranoia_resetall(cdrom_paranoia *p);
extern void paranoia_resetcache(cdrom_paranoia *p);

#endif
