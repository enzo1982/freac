/* (c)Copyright 1996-2000 NTT Cyber Space Laboratories */
/*                Released on 2000.05.22 by N. Iwakami */
/*                Released on 2000.09.06 by N. Iwakami */
/*          Ported to IOLib on 2002.08.25 by R. Kausch */

#ifndef bitstream_e_h
#define bitstream_e_h

#include "Chunk.h"
#include <iolib-cxx.h>

#define	BYTE_BITS	8
#define	BBUFSIZ		1
#define	BBUFLEN		(BBUFSIZ * BYTE_BITS)

extern int put_strm(int		 data,  /* Input: input data */
		    int		 nbits, /* Input: number of bits */
		    OutStream	*bfp);  /* Input: bit file pointer */

extern void TvqInitBsWriter();
extern void TvqWriteBsFrame(INDEX *indexp, OutStream *bfp);
extern void TvqFinishBsOutput(OutStream *bfp);
extern void TvqPutBsHeaderInfo(OutStream *bfp, CChunkChunk &twinChunk);

#endif
