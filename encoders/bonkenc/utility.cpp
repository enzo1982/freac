#include "utility.h"
#include <vector>

using namespace std;

int bits_to_store(uint32 i)
{
  int result = 0;
  while(i) {
    result++;
    i >>= 1;
  }
  return result;
}

bool read_list(vector<int> &list,bool base_2_part, bitstream_in &in) {
  //Read a list of ints as encoded by "write_list"

  //Read base 2 part:

  int low_bits = (base_2_part ? in.read_uint(4) : 0);
  int i;

  for(i=0;i<(int)list.size();i++) 
    list[i] = in.read_uint(low_bits);
    
  // Decode bitstream:

  int n_zeros = 0;
  int step = 256;
  bool dominant = false;
  vector<uint8> bits;
  while(n_zeros < (int)list.size()) {
    int steplet = step>>8;
  
    if (!in.read()) {
      for(int i=0;i<steplet;i++)
        bits.push_back(dominant);
      
      if (!dominant)
        n_zeros += steplet;
	
      step += step / adapt_level;
    } else {
      int actual_run = in.read_uint_max(steplet-1);
      
      for(int i=0;i<actual_run;i++)
        bits.push_back(dominant);
      bits.push_back(!dominant);

      if (!dominant)
        n_zeros += actual_run;
      if (dominant)
        n_zeros++;
    
      step -= step / adapt_level;
    }

    if (step < 256) {
      step = 65536 / step;
      dominant = !dominant;
    }
  }

  // Reconstruct (unsigned) values:

  n_zeros = 0;
  int pos   = 0;
  int level = 0;
  for(i=0;n_zeros<(int)list.size();i++) {
    for(;;) {
      if (pos >= (int)list.size()) {
        pos = 0;
	level += 1<<low_bits;
     }

      if (list[pos] >= level)
        break;

      pos++;
    }
      
    if (bits[i])
      list[pos] += 1<<low_bits;
    else
      n_zeros++;

    pos++;
  }

  // Read signs:

  for(i=0;i<(int)list.size();i++)
    if (list[i] && in.read())
      list[i] = -list[i];

  return true;
}

void write_list(const vector<int> &list,bool base_2_part, bitstream_out &out)
{
  //Store a list of integers concisely

  //If large magnitude, store low order bits in base 2

  int low_bits = 0;

  if (base_2_part) {
    int energy = 0;
    for(int i=0;i<list.size();i++)
      energy += abs(list[i]);
  
    low_bits = bits_to_store(energy/(list.size()*2));
    if (low_bits > 15) low_bits = 15;

    out.write_uint(low_bits, 4);
  }

  vector<int> copy(list.size());
  int i;
  for(i=0;i<list.size();i++) {
    out.write_uint(abs(list[i]),low_bits);
    copy[i] = abs(list[i]) >> low_bits;
  }

  //Convert list into bitstream:
  
  vector<uint8> bits;

  int max = 0;
  for(i=0;i<copy.size();i++)
    if (copy[i] > max)
      max = abs(copy[i]);
  
  for(i=0;i<=max;i++) 
    for(int j=0;j<copy.size();j++)
      if (copy[j] >= i)
        bits.push_back(copy[j] > i);
  
  // Store bitstream:
  
  int step = 256;
  int pos = 0;
  bool dominant = false;
  while(pos < bits.size()) {
    int steplet = step>>8;
    if (pos+steplet > bits.size())
      steplet = bits.size()-pos;

    bool any = false;;
    for(int i=0;i<steplet;i++)
      if (bits[i+pos] != (unsigned char) dominant)
        any = true;

    out.write(any);

    if (!any) {
      pos  += steplet;
      step += step / adapt_level;
    } else {
      int interloper=0;
      while(bits[pos+interloper] == (unsigned char) dominant) interloper++;

      //Note change.
      out.write_uint_max(interloper,(step>>8)-1);
      pos += interloper+1;
      step -= step / adapt_level;
    }

    if (step < 256) {
      step = 65536 / step;
      dominant = !dominant;
    }
  }

  // Store signs:

  for(i=0;i<list.size();i++)
    if (list[i])
      out.write(list[i]<0);
}
