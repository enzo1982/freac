/***
 * CopyPolicy: GNU Public License 2 applies
 * Copyright (C) by Monty (xiphmont@mit.edu)
 ***/

#ifndef _GAP_H_
#define _GAP_H_

#ifndef _MSC_VER
#include <unistd.h>
#endif

extern long i_paranoia_overlap_r(int16_t *buffA,int16_t *buffB,
				 long offsetA, long offsetB);
extern long i_paranoia_overlap_f(int16_t *buffA,int16_t *buffB,
				 long offsetA, long offsetB,
				 long sizeA,long sizeB);
extern int i_stutter_or_gap(int16_t *A, int16_t *B,long offA, long offB,
			    long gap);
extern void i_analyze_rift_f(int16_t *A,int16_t *B,
			     long sizeA, long sizeB,
			     long aoffset, long boffset, 
			     long *matchA,long *matchB,long *matchC);
extern void i_analyze_rift_r(int16_t *A,int16_t *B,
			     long sizeA, long sizeB,
			     long aoffset, long boffset, 
			     long *matchA,long *matchB,long *matchC);

extern void analyze_rift_silence_f(int16_t *A,int16_t *B,long sizeA,long sizeB,
				   long aoffset, long boffset,
				   long *matchA, long *matchB);
#endif
