/*---- DXhead.h --------------------------------------------


decoder MPEG Layer III

handle Xing header


Copyright 1998 Xing Technology Corp.
-----------------------------------------------------------*/
// A Xing header may be present in the ancillary
// data field of the first frame of an mp3 bitstream
// The Xing header (optionally) contains
//      frames      total number of audio frames in the bitstream
//      bytes       total number of bytes in the bitstream
//      toc         table of contents

// toc (table of contents) gives seek points
// for random access
// the ith entry determines the seek point for
// i-percent duration
// seek point in bytes = (toc[i]/256.0) * total_bitstream_bytes
// e.g. half duration seek point = (toc[50]/256.0) * total_bitstream_bytes


#define FRAMES_FLAG     0x0001
#define BYTES_FLAG      0x0002
#define TOC_FLAG        0x0004
#define VBR_SCALE_FLAG  0x0008

#define FRAMES_AND_BYTES (FRAMES_FLAG | BYTES_FLAG)

// structure to receive extracted header
// toc may be NULL
typedef struct {
    int h_id;       // from MPEG header, 0=MPEG2, 1=MPEG1
    int samprate;   // determined from MPEG header
    int flags;      // from Xing header data
    int frames;     // total bit stream frames from Xing header data
    int bytes;      // total bit stream bytes from Xing header data
    int vbr_scale;  // encoded vbr scale from Xing header data
    unsigned char *toc;  // pointer to unsigned char toc_buffer[100]
                         // may be NULL if toc not desired
}   XHEADDATA;

#ifdef __cplusplus
extern "C" {
    int GetXingHeader(XHEADDATA *X,  unsigned char *buf);
    // return 0=fail, 1=success
    // X   structure to receive header data (output)
    // buf bitstream input 


    int SeekPoint(unsigned char TOC[100], int file_bytes, float percent);
    // return seekpoint in bytes (may be at eof if percent=100.0)
    // TOC = table of contents from Xing header
    // file_bytes = number of bytes in mp3 file
    // percent = play time percentage of total playtime. May be
    //           fractional (e.g. 87.245)
}
#endif

/*---- DXhead.c --------------------------------------------


decoder MPEG Layer III

handle Xing header

mod 12/7/98 add vbr scale

Copyright 1998 Xing Technology Corp.
-----------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

// 4   Xing
// 4   flags
// 4   frames
// 4   bytes
// 100 toc

/*-------------------------------------------------------------*/
static int ExtractI4(unsigned char *buf)
{
int x;
// big endian extract

x = buf[0];
x <<= 8;
x |= buf[1];
x <<= 8;
x |= buf[2];
x <<= 8;
x |= buf[3];

return x;
}
/*-------------------------------------------------------------*/
int GetXingHeader(XHEADDATA *X,  unsigned char *buf)
{
int i, head_flags;
int h_id, h_mode, h_sr_index;
static int sr_table[4] = { 44100, 48000, 32000, 99999 };

// get Xing header data


X->flags = 0;     // clear to null incase fail


// get selected MPEG header data
h_id       = (buf[1] >> 3) & 1;
h_sr_index = (buf[2] >> 2) & 3;
h_mode     = (buf[3] >> 6) & 3;


// determine offset of header
if( h_id ) {        // mpeg1
    if( h_mode != 3 ) buf+=(32+4);
    else              buf+=(17+4);
}
else {      // mpeg2
    if( h_mode != 3 ) buf+=(17+4);
    else              buf+=(9+4);
}

if( (buf[0] != 'X' || buf[1] != 'i' || buf[2] != 'n' || buf[3] != 'g') &&          // fail
    (buf[0] != 'I' || buf[1] != 'n' || buf[2] != 'f' || buf[3] != 'o') ) return 0; // header not found
buf+=4;

X->h_id = h_id;
X->samprate = sr_table[h_sr_index];
if( h_id == 0 ) X->samprate >>= 1;

head_flags = X->flags = ExtractI4(buf); buf+=4;      // get flags

if( head_flags & FRAMES_FLAG ) {X->frames   = ExtractI4(buf); buf+=4;}
if( head_flags & BYTES_FLAG )  {X->bytes = ExtractI4(buf); buf+=4;}

if( head_flags & TOC_FLAG ) {
    if( X->toc != NULL ) {
        for(i=0;i<100;i++) X->toc[i] = buf[i];
    }
    buf+=100;
}

X->vbr_scale = -1;
if( head_flags & VBR_SCALE_FLAG )  {X->vbr_scale = ExtractI4(buf); buf+=4;}

//if( X->toc != NULL ) {
//for(i=0;i<100;i++) {
//    if( (i%10) == 0 ) printf("\n");
//    printf(" %3d", (int)(X->toc[i]));
//}
//}

return 1;       // success
}
/*-------------------------------------------------------------*/
int SeekPoint(unsigned char TOC[100], int file_bytes, float percent)
{
// interpolate in TOC to get file seek point in bytes
int a, seekpoint;
float fa, fb, fx;


if( percent < 0.0f )   percent = 0.0f;
if( percent > 100.0f ) percent = 100.0f;

a = (int)percent;
if( a > 99 ) a = 99;
fa = TOC[a];
if( a < 99 ) {
    fb = TOC[a+1];
}
else {
    fb = 256.0f;
}


fx = fa + (fb-fa)*(percent-a);

seekpoint = (int)((1.0f/256.0f)*fx*file_bytes); 


return seekpoint;
}
/*-------------------------------------------------------------*/
