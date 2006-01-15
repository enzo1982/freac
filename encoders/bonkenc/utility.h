#ifndef UTILITY_H
#define UTILITY_H

#include <stdlib.h>

#include <smooth/io/instream.h>
#include <smooth/io/outstream.h>

using namespace smooth::IO;

typedef unsigned long  uint32;
typedef unsigned short uint16;
typedef unsigned char  uint8;
typedef short          int16;
typedef unsigned int   uint;

const int adapt_level = 8;

int bits_to_store(uint32);

struct error { 
  char *message; 
  error(char *_message) : message(_message) { } 
};

struct bitstream_in {
  InStream *f_in;
  int byte;
  int bit_no;

  void setup() {
    bit_no = 8;
  }

  int read() {
    if (bit_no == 8) {
      byte = f_in->InputNumber(1);
      bit_no = 0;
    }

    return (byte & (1<<bit_no++) ? 1 : 0);
  }

  uint32 read_uint(int bits) {
    uint32 value = 0;
    for(int i=0;i<bits;i++)
      value += read()<<i;
    return value;
  }

  uint read_uint_max(int max) {
    if (!max) return 0;
    int bits = bits_to_store(max);

    uint value = 0;
    for(int i=0;i<bits-1;i++)
      if (read())
        value += 1<<i;
   
    if ( ((int)value | (1<<(bits-1))) <= max )
      if (read())
        value += 1<<(bits-1);

    return value;
  }
};

struct bitstream_out {
  OutStream *f_out;
  int byte;
  int prevbyte;
  int bytes_written;
  int bit_no;

  void setup(OutStream *_f_out) {
    f_out = _f_out;
    byte = 0;
    prevbyte = 0;
    bytes_written = 0;
    bit_no = 0;
  }

  void write(int bit) {
    if (bit)
      byte = (byte | (1<<bit_no));

    bit_no++;

    if (bit_no == 8) {
      f_out->OutputNumber(byte, 1);
      bytes_written++;
      prevbyte = byte;
      byte = 0;
      bit_no = 0;
    }
  }

  void write_uint(uint value,int bits) {
    for(int i=0;i<bits;i++)
      write(value & (1<<i));
  }

  void write_uint_max(uint value,int max) {
    if (!max) return;
    int bits = bits_to_store(max);
    for(int i=0;i<bits-1;i++)
      write(value & (1<<i));
   
    if ( (value | (1<<(bits-1))) <= (uint) max )
      write(value & (1<<(bits-1)));
  }

  void flush() {
    while(bit_no != 0) 
      write(0);
  }
};

#endif
