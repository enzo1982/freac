/******************************************************************
 * CopyPolicy: GNU Public License 2 applies
 * Copyright (C) 1998 Monty xiphmont@mit.edu
 * and Heiko Eissfeldt heiko@escape.colossus.de
 *
 * Toplevel interface header; applications include this
 *
 ******************************************************************/

#ifndef _cdda_interface_h_
#define _cdda_interface_h_

#ifndef CD_FRAMESIZE
#define CD_FRAMESIZE 2048
#endif
#ifndef CD_FRAMESIZE_RAW
#define CD_FRAMESIZE_RAW 2352
#endif
#define CD_FRAMESAMPLES (CD_FRAMESIZE_RAW / 4)


typedef struct cdrom_drive{
  long nsectors; /* number of sectors that can be read at once */
  void *cdr;     /* pointer to a CDExtract object */
} cdrom_drive;


#ifdef __cplusplus
extern "C" {
#endif

extern long cdda_read(cdrom_drive *d, void *buffer,
		       long beginsector, long sectors);

#ifdef __cplusplus
}
#endif

#endif

