/***
 * CopyPolicy: GNU Public License 2 applies
 * Copyright (C) by Monty (xiphmont@mit.edu)
 ***/

#ifndef _ISORT_H_
#define _ISORT_H_

typedef short int16_t;
typedef short size16;

#ifndef inline
#define inline 
#endif

typedef struct sort_link{
  struct sort_link *next;
} sort_link;

typedef struct sort_info{
  int16_t *vector;                /* vector (storage doesn't belong to us) */

  long  *abspos;                 /* pointer for side effects */
  long  size;                    /* vector size */

  long  maxsize;                 /* maximum vector size */

  long sortbegin;                /* range of contiguous sorted area */
  long lo,hi;                    /* current post, overlap range */
  int  val;                      /* ...and val */

  /* sort structs */
  sort_link **head;           /* sort buckets (65536) */

  long *bucketusage;          /*  of used buckets (65536) */
  long lastbucket;
  sort_link *revindex;

} sort_info;

extern sort_info *sort_alloc(long size);
extern void sort_unsortall(sort_info *i);
extern void sort_setup(sort_info *i,int16_t *vector,long *abspos,long size,
		       long sortlo, long sorthi);
extern void sort_free(sort_info *i);
extern sort_link *sort_getmatch(sort_info *i,long post,long overlap,int value);
extern sort_link *sort_nextmatch(sort_info *i,sort_link *prev);

#define is(i) (i->size)
#define ib(i) (*i->abspos)
#define ie(i) (i->size+*i->abspos)
#define iv(i) (i->vector)
#define ipos(i,l) (l-i->revindex)

#endif

