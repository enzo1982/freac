/* (c)Copyright 1996-2000 NTT Cyber Space Laboratories */
/*                Released on 2000.05.22 by N. Iwakami */
/*                Released on 2000.09.06 by N. Iwakami */
/*          Ported to IOLib on 2002.08.25 by R. Kausch */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include "twinvq.h"
#include "bstream_e.h"
#include "tvqenc.h"
#include "Chunk.h"
#include "ChunkHelper.h"

int	 bsptr = 0;
char	 bsbuf = 0;

int put_strm(int	 data,  /* input data */
	     int	 nbits, /* number of bits */
	     OutStream	*bfp)   /* bit file pointer */
{
	int	 rtflag = 0;

	if ((0x1 << nbits) <= data) rtflag = 1;

	for (int ibit = 0; ibit < nbits; ibit++)
	{
		bsbuf |= ((data >> (nbits - 1 - ibit)) & 1) << (BYTE_BITS - bsptr - 1);

		if (++bsptr == 8)
		{
			bfp->OutputNumber(bsbuf, 1);

			bsbuf = 0;
			bsptr = 0;
		}
	}

	return rtflag;
}

void TvqFinishBsOutput(OutStream *bfp)   /* bit file pointer */
{
	if (bsptr != 0)
	{
		put_strm(0, 8 - bsptr, bfp);
	}
}

/*----------------------------------------------------------------------------*/
/* bits and length table for interleaved VQ                                   */
/*----------------------------------------------------------------------------*/
static int *bits_0[N_INTR_TYPE], *bits_1[N_INTR_TYPE];

/*----------------------------------------------------------------------------*/
/* lookup parameters                                                          */
/*----------------------------------------------------------------------------*/
static tvqConfInfo cf;

/*----------------------------------------------------------------------------*/
/* definitions                                                                */
/*----------------------------------------------------------------------------*/
#if defined (_MSC_VER)
#  define stat _stat
#endif

/*----------------------------------------------------------------------------*/
/* Name:        put_string()                                                  */
/* Description: put string into a bitstream file                              */
/* Return:      none                                                          */
/* Access:      static                                                        */
/*----------------------------------------------------------------------------*/
static
void put_string (char *buf, int nbytes, OutStream *bfp)
{
	while (nbytes--)
		put_strm(*buf++, BYTE_BITS, bfp);
	return;
}

static
void put_string( std::string theString, OutStream *bfp )
{
	std::string::iterator it;

	for ( it=theString.begin(); it!=theString.end(); it++ ) {
		put_strm( *it, BYTE_BITS, bfp );
	}

	return;
}

/*----------------------------------------------------------------------------*/
/* Name:        ChunkWriteToFile()                                            */
/* Description: Write a chunk into a bitstream file                           */
/* Return:      (int) returns 1 when errors or 0                              */
/* Access:      static                                                        */
/*----------------------------------------------------------------------------*/
int ChunkWriteToFile( CChunk& twinChunk, OutStream *bfp )
{
	CChunk::CChunkData theData = twinChunk.GetData();
	CChunk::CChunkData::iterator it;

	put_string( twinChunk.GetID(), bfp ); // チャンクIDを記録
	put_strm( twinChunk.GetSize(), BYTE_BITS * sizeof(unsigned long), bfp ); // チャンクサイズを記録
	for ( it=theData.begin(); it!=theData.end(); it++ ) {					// チャンクデータを記録
		put_strm( *it, BYTE_BITS, bfp );
	}

	return 0;
}

/*============================================================================*/
/* Name:        TvqPutBsHeaderInfo()                                          */
/* Description: put bitstream header into a bitstream file                    */
/* Return:      none                                                          */
/* Access:      external                                                      */
/*============================================================================*/
void TvqPutBsHeaderInfo(OutStream *bfp,
						CChunkChunk& twinChunk)
{

	/* write and terminate the "TWIN" chunk */
	ChunkWriteToFile( twinChunk, bfp );

	/* write keyword of the "DATA" chunk */
	put_string("DATA", KEYWORD_BYTES, bfp);
	
	return;
}

/*============================================================================*/
/* Name:        TvqInitBsWriter()                                             */
/* Description: initialize the bitstream writer                               */
/* Return:      none                                                          */
/* Access:      external                                                      */
/*============================================================================*/
void TvqInitBsWriter()
{
	
	ex_TvqEncGetConfInfo(&cf);

	ex_TvqEncGetVectorInfo( bits_0, bits_1 );
	
}

/*----------------------------------------------------------------------------*/
/* Name:        PutVQInfo()                                                   */
/* Description: put VQ info                                                   */
/* Return:      (int) number of used bits                                     */
/* Access:      static                                                        */
/*----------------------------------------------------------------------------*/
static
int PutVQInfo(tvqConfInfoSubBlock *cfg,
			  int   bits0[],
			  int   bits1[],
			  int   variable_side_bits,
			  INDEX *index,
			  OutStream *bfp)
{
	int idiv;
	int bitcount = 0;

	/* re-calculate interleave table */
	if ( index->btype == BLK_LONG ){
		ex_TvqEncUpdateVectorInfo( variable_side_bits, &cfg->ndiv, bits0, bits1 );
	}

	/* put information */
	for ( idiv=0; idiv<cfg->ndiv; idiv++ ){
		if ( put_strm( index->wvq[idiv], bits0[idiv], bfp ) ){
			printf( "shape0 %d, %d %d\n", idiv, bits0[idiv], index->wvq[idiv] );
		}
		if ( put_strm( index->wvq[idiv+cfg->ndiv], bits1[idiv], bfp ) ){
			printf( "shape1 %d, %d %d\n", idiv, bits1[idiv], index->wvq[idiv+cfg->ndiv] );
		}
		bitcount += bits0[idiv] + bits1[idiv];
	}

	return bitcount;
}

/*----------------------------------------------------------------------------*/
/* Name:        PutBseInfo()                                                  */
/* Description: put BSE info                                                  */
/* Return:      (int) number of used bits                                     */
/* Access:      static                                                        */
/*----------------------------------------------------------------------------*/
static
int PutBseInfo( tvqConfInfo *cf, tvqConfInfoSubBlock *cfg, INDEX *index, OutStream *bfp )
{
	int i_sup, isf, idiv, itmp;
	int bitcount = 0;

	for ( i_sup=0; i_sup<cf->N_CH; i_sup++ ){
		for ( isf=0; isf<cfg->nsf; isf++ ){
			for ( idiv=0; idiv<cfg->fw_ndiv; idiv++ ){
				itmp = idiv + ( isf + i_sup * cfg->nsf ) * cfg->fw_ndiv;
				if ( put_strm( index->fw[itmp], cfg->fw_nbit, bfp ) ){
					printf( "forward short %d, %d, %d, %d\n", i_sup,isf,idiv,index->fw[itmp]);
				}
				bitcount += cfg->fw_nbit;
			}
		}
	}
	for ( i_sup=0; i_sup<cf->N_CH; i_sup++ ){
		for ( isf=0; isf<cfg->nsf; isf++ ){
			itmp = i_sup * cfg->nsf + isf;
			if ( put_strm( index->fw_alf[itmp], cf->FW_ARSW_BITS, bfp ) ){
				printf( "forward AR param. ch: %d, sh_no: %d, ind: %d\n", i_sup, isf, index->fw_alf[itmp] );
			}
			bitcount += cf->FW_ARSW_BITS;
		}
	}
	return bitcount;
}

/*----------------------------------------------------------------------------*/
/* Name:        PutGainInfo()                                                 */
/* Description: put gain info                                                 */
/* Return:      (int) number of used bits                                     */
/* Access:      static                                                        */
/*----------------------------------------------------------------------------*/
static
int PutGainInfo( tvqConfInfo *cf, tvqConfInfoSubBlock *cfg, INDEX *index, OutStream *bfp )
{
	int i_sup, isf, iptop;
	int bitcount = 0;

	for ( i_sup=0; i_sup<cf->N_CH; i_sup++ ){
		iptop = ( cfg->nsubg + 1 ) * i_sup;
		if ( put_strm( index->pow[iptop], cf->GAIN_BITS, bfp ) ){
			printf( "power  %d %d\n",  cf->GAIN_BITS, index->pow[iptop] );
		}
		bitcount += cf->GAIN_BITS;
		for ( isf=0; isf<cfg->nsubg; isf++ ){
			if ( put_strm( index->pow[iptop+isf+1], cf->SUB_GAIN_BITS, bfp ) ){
				printf( "power %d, %d %d\n", isf, cf->SUB_GAIN_BITS, index->pow[iptop+isf+1] );
			}
			bitcount += cf->SUB_GAIN_BITS;
		}
	}
	return bitcount;
}

/*----------------------------------------------------------------------------*/
/* Name:        PutLspInfo()                                                  */
/* Description: put LSP info                                                  */
/* Return:      (int) number of used bits                                     */
/* Access:      static                                                        */
/*----------------------------------------------------------------------------*/
static
int PutLspInfo( tvqConfInfo *cf, INDEX *index, OutStream *bfp )
{
	int i_sup, itmp;
	int bitcount = 0;

	for ( i_sup=0; i_sup<cf->N_CH; i_sup++ ){
		if ( put_strm( index->lsp[i_sup][0], cf->LSP_BIT0, bfp ) ){/*pred. switch*/
			printf("lsp0 %d %d\n",cf->LSP_BIT0,index->lsp[i_sup][0]);
		}
		if ( put_strm( index->lsp[i_sup][1], cf->LSP_BIT1, bfp ) ){ /*first stage*/
			printf("lsp1 %d %d\n",cf->LSP_BIT1,index->lsp[i_sup][1]);
		}
		bitcount += cf->LSP_BIT0 + cf->LSP_BIT1;
		for ( itmp=0; itmp<cf->LSP_SPLIT; itmp++ ){  /* second stage */
			if ( put_strm( index->lsp[i_sup][itmp+2], cf->LSP_BIT2, bfp ) ){
				printf( "lsp2 %d, %d %d\n", itmp, cf->LSP_BIT2, index->lsp[i_sup][itmp+2] );
			}
			bitcount += cf->LSP_BIT2;
		}
	}

	return bitcount;
}

/*----------------------------------------------------------------------------*/
/* Name:        PutPpcInfo()                                                  */
/* Description: put PPC info                                                  */
/* Return:      (int) number of used bits                                     */
/* Access:      static                                                        */
/*----------------------------------------------------------------------------*/
static
int PutPpcInfo( tvqConfInfo *cf, INDEX *index, OutStream *bfp )
{
	int i_sup, idiv;
	int bitcount = 0;

	for ( idiv=0; idiv<cf->N_DIV_P; idiv++ ){
		if ( put_strm( index->pls[idiv], bits_0[BLK_PPC][idiv], bfp ) ){
			printf( "codebook0_pit %d, %d %d\n", idiv, bits_0[BLK_PPC][idiv], index->pls[idiv] );
		}
		if ( put_strm( index->pls[idiv+cf->N_DIV_P], bits_1[BLK_PPC][idiv], bfp ) ){
			printf( "codebook1_pit %d, %d %d\n", idiv, bits_1[BLK_PPC][idiv], index->pls[idiv+cf->N_DIV_P] );
		}
		bitcount += bits_0[BLK_PPC][idiv] + bits_1[BLK_PPC][idiv];
	}
	for (i_sup=0; i_sup<cf->N_CH; i_sup++){
		if ( put_strm( index->pit[i_sup], cf->BASF_BIT, bfp ) ){
			printf( "index_pitch %d %d\n", cf->BASF_BIT, index->pit[i_sup] );
		}
		if ( put_strm( index->pgain[i_sup], cf->PGAIN_BIT, bfp ) ){
			printf( "index_pgain %d %d\n", cf->PGAIN_BIT, index->pgain[i_sup] );
		}
		bitcount += cf->BASF_BIT  + cf->PGAIN_BIT;
	}

	return bitcount;
}

/*----------------------------------------------------------------------------*/
/* Name:        PutEbcInfo()                                                  */
/* Description: put EBC info                                                  */
/* Return:      (int) number of used bits                                     */
/* Access:      static                                                        */
/*----------------------------------------------------------------------------*/
static
int PutEbcInfo( tvqConfInfo *cf, tvqConfInfoSubBlock *cfg, INDEX *index, OutStream *bfp )
{
	int i_sup, isf, itmp;
	int bitcount = 0;
	for ( i_sup=0; i_sup<cf->N_CH; i_sup++ ){
		for ( isf=0; isf<cfg->nsf; isf++){
			int indexSfOffset = isf * ( cfg->ncrb - cfg->ebc_crb_base ) - cfg->ebc_crb_base;
			for ( itmp=cfg->ebc_crb_base; itmp<cfg->ncrb; itmp++ ){
				if ( put_strm( index->bc[i_sup][itmp+indexSfOffset], cfg->ebc_bits, bfp ) ){
					printf( "index_pb %d %d", cfg->ebc_bits, index->bc[i_sup][itmp+indexSfOffset] );
				}
				bitcount += cfg->ebc_bits;
			}
		}
	}

	return bitcount;
}

/*============================================================================*/
/* Name:        TvqWriteBsFrame()                                             */
/* Description: write bitstream                                               */
/* Return:      none                                                          */
/* Access:      external                                                      */
/*============================================================================*/
void TvqWriteBsFrame(INDEX *index, OutStream *bfp)        /* Input:  Data file pointer */
{
	/*--- Initialization ---*/
	int bitcount = 0;
	int variableBits = 0;
	
	/*--- Window type ---*/
	put_strm( index->w_type, cf.BITS_WTYPE, bfp );
	bitcount += cf.BITS_WTYPE;
	int btype = index->btype;
	
	tvqConfInfoSubBlock *cfg = &cf.cfg[btype];
	
	/*--- VQ info ---*/
	bitcount += PutVQInfo( cfg, bits_0[btype], bits_1[btype], variableBits, index, bfp );
	
	/*--- Bark-scale envelope ---*/
	bitcount += PutBseInfo( &cf, cfg, index, bfp );
	
	/*--- Gain ---*/
	bitcount += PutGainInfo( &cf, cfg, index, bfp );
	
	/*--- LSP ---*/
	bitcount += PutLspInfo( &cf, index, bfp );
	
	/*--- Pitch excitation ---*/
	if ( cfg->ppc_enable ){
		bitcount += PutPpcInfo( &cf, index, bfp );
	}
	
	/*--- Energy Balance Calibration ---*/
	if ( cfg->ebc_enable ){
		bitcount += PutEbcInfo( &cf, cfg, index, bfp );
	}
	
}

