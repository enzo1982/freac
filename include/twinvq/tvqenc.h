/* (c)Copyright 1996-2000 NTT Cyber Space Laboratories */
/*                Released on 2000.05.22 by N. Iwakami */
/*                Modified on 2000.05.25 by N. Iwakami */
/*                Modified on 2000.09.06 by N. Iwakami */

#ifndef tvqenc_h
#define tvqenc_h

#include <time.h>

typedef struct{
	int N_CAN_GLOBAL;
} encSpecificInfo;

#ifdef _MSC_VER
#	ifdef DLL_MODULE
#		define DllPort    __declspec( dllexport )
#	else
#		define DllPort    __declspec( dllimport )
#	endif
#else
#	define DllPort
#endif

#ifdef __cplusplus
extern "C" {  // only need to import/export C interface if used by C++ source code
#endif

// TwinVQ encoder initialization functions
DllPort int  TvqEncInitialize( headerInfo *setupInfo, encSpecificInfo *encInfo, INDEX *index, int dispErrorMessageBox );
DllPort void TvqEncTerminate( INDEX *index );
DllPort void TvqEncGetVectorInfo(int *bits0[], int *bits1[]);
DllPort void TvqEncResetFrameCounter();

// TwinVQ encoder function
DllPort void TvqEncodeFrame( float sig_in[], INDEX  *index );
DllPort void TvqEncUpdateVectorInfo(int varbits, int *ndiv, int bits0[], int bits1[]);
DllPort void TvqEncSetFrameCounter( int position );

// TwinVQ query functions
DllPort int   TvqEncGetFrameSize();
DllPort int   TvqEncGetNumChannels();
DllPort int   TvqEncGetNumFixedBitsPerFrame();
DllPort void  TvqEncGetSetupInfo( headerInfo *setupInfo );
DllPort float TvqEncGetSamplingRate();
DllPort int   TvqEncGetBitRate();
DllPort void  TvqEncGetConfInfo( tvqConfInfo *cf );
DllPort int   TvqEncGetNumFrames();
DllPort int   TvqGetVersionID( int versionNum, char* versionString );
DllPort int   TvqEncCheckVersion( char *strTvqID );
DllPort int   TvqEncGetModuleVersion( char* versionString );

#ifdef __cplusplus
}
#endif

#endif

