/***
 * CopyPolicy: GNU Public License 2 applies
 * Copyright (C) by Monty (xiphmont@mit.edu)
 *
 ***/

#ifndef _CDROM_PARANOIA_
#define _CDROM_PARANOIA_

#define CD_FRAMEWORDS (CD_FRAMESIZE_RAW/2)

#define PARANOIA_CB_READ           0
#define PARANOIA_CB_VERIFY         1
#define PARANOIA_CB_FIXUP_EDGE     2
#define PARANOIA_CB_FIXUP_ATOM     3
#define PARANOIA_CB_SCRATCH        4
#define PARANOIA_CB_REPAIR         5
#define PARANOIA_CB_SKIP           6
#define PARANOIA_CB_DRIFT          7
#define PARANOIA_CB_BACKOFF        8
#define PARANOIA_CB_OVERLAP        9
#define PARANOIA_CB_FIXUP_DROPPED 10
#define PARANOIA_CB_FIXUP_DUPED   11
#define PARANOIA_CB_READERR       12

#define PARANOIA_MODE_FULL        0xff
#define PARANOIA_MODE_DISABLE     0

#define PARANOIA_MODE_VERIFY      1
#define PARANOIA_MODE_FRAGMENT    2
#define PARANOIA_MODE_OVERLAP     4
#define PARANOIA_MODE_SCRATCH     8
#define PARANOIA_MODE_REPAIR      16
#define PARANOIA_MODE_NEVERSKIP   32

#ifndef CDP_COMPILE
typedef void cdrom_paranoia;
#endif

extern cdrom_paranoia *paranoia_init(cdrom_drive *d);
extern void paranoia_modeset(cdrom_paranoia *p,int mode);
extern long paranoia_seek(cdrom_paranoia *p,long seek,int mode);
extern short *paranoia_read(cdrom_paranoia *p,void(*callback)(long,int), int* pbAbort );
extern short *paranoia_read_limited(cdrom_paranoia *p,void(*callback)(long,int),int maxretries, int* pbAbort);
extern void paranoia_free(cdrom_paranoia *p);
extern void paranoia_overlapset(cdrom_paranoia *p,long overlap);
extern void paranoia_set_range(cdrom_paranoia *p, long start, long end);

#endif
