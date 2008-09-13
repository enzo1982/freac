/******************************************************************
 * CopyPolicy: GNU Lesser General Public License 2.1 applies
 * Copyright (C) 2001-2008 Xiph.org
 * Original version by Heiko Eissfeldt heiko@escape.colossus.de
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

#include <sys/types.h>
#include <signal.h>
#include <stdint.h>

#define MAXTRK 100

/* interface types */
#define GENERIC_SCSI	 0
#define COOKED_IOCTL	 1
#define TEST_INTERFACE	 2
#define SGIO_SCSI	 3
#define SGIO_SCSI_BUGGY1 4

#define CDDA_MESSAGE_FORGETIT 0
#define CDDA_MESSAGE_PRINTIT 1
#define CDDA_MESSAGE_LOGIT 2

/* cdrom access function pointer */

typedef struct cdda_private_data cdda_private_data_t;

typedef struct cdrom_drive{
  long nsectors; /* number of sectors that can be read at once */
  void *cdr;     /* pointer to a CDExtract object */
} cdrom_drive;

#define IS_AUDIO(d,i) (!(d->disc_toc[i].bFlags & 0x04))

/******** Identification/autosense functions */

extern cdrom_drive *cdda_find_a_cdrom(int messagedest, char **message);
extern cdrom_drive *cdda_identify(const char *device, int messagedest,
				  char **message);
extern cdrom_drive *cdda_identify_cooked(const char *device,int messagedest,
					 char **message);
extern cdrom_drive *cdda_identify_scsi(const char *generic_device, 
				       const char *ioctl_device,
				       int messagedest, char **message);
#ifdef CDDA_TEST
extern cdrom_drive *cdda_identify_test(const char *filename,
				       int messagedest, char **message);
#endif

/******** Drive oriented functions */

extern char *cdda_version();
extern int cdda_speed_set(cdrom_drive *d, int speed);
extern void cdda_verbose_set(cdrom_drive *d,int err_action, int mes_action);
extern char *cdda_messages(cdrom_drive *d);
extern char *cdda_errors(cdrom_drive *d);

extern int cdda_close(cdrom_drive *d);
extern int cdda_open(cdrom_drive *d);
extern long cdda_read(cdrom_drive *d, void *buffer,
		       long beginsector, long sectors);
extern long cdda_read_timed(cdrom_drive *d, void *buffer,
			    long beginsector, long sectors, int *milliseconds);

extern long cdda_track_firstsector(cdrom_drive *d,int track);
extern long cdda_track_lastsector(cdrom_drive *d,int track);
extern long cdda_tracks(cdrom_drive *d);
extern int cdda_sector_gettrack(cdrom_drive *d,long sector);
extern int cdda_track_channels(cdrom_drive *d,int track);
extern int cdda_track_audiop(cdrom_drive *d,int track);
extern int cdda_track_copyp(cdrom_drive *d,int track);
extern int cdda_track_preemp(cdrom_drive *d,int track);
extern long cdda_disc_firstsector(cdrom_drive *d);
extern long cdda_disc_lastsector(cdrom_drive *d);

/* transport errors: */

#define TR_OK            0
#define TR_EWRITE        1  /* Error writing packet command (transport) */
#define TR_EREAD         2  /* Error reading packet data (transport) */
#define TR_UNDERRUN      3  /* Read underrun */
#define TR_OVERRUN       4  /* Read overrun */
#define TR_ILLEGAL       5  /* Illegal/rejected request */
#define TR_MEDIUM        6  /* Medium error */
#define TR_BUSY          7  /* Device busy */
#define TR_NOTREADY      8  /* Device not ready */
#define TR_FAULT         9  /* Devive failure */
#define TR_UNKNOWN      10  /* Unspecified error */
#define TR_STREAMING    11  /* loss of streaming */

/* Errors returned by lib: 

001: Unable to set CDROM to read audio mode
002: Unable to read table of contents lead-out
003: CDROM reporting illegal number of tracks
004: Unable to read table of contents header
005: Unable to read table of contents entry
006: Could not read any data from drive
007: Unknown, unrecoverable error reading data
008: Unable to identify CDROM model
009: CDROM reporting illegal table of contents
010: Unaddressable sector 

100: Interface not supported
101: Drive is neither a CDROM nor a WORM device
102: Permision denied on cdrom (ioctl) device
103: Permision denied on cdrom (data) device

300: Kernel memory error

400: Device not open
401: Invalid track number
402: Track not audio data
403: No audio tracks on disc
404: No medium present
405: Option not supported by drive

*/
#endif

