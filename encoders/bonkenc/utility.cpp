#include "utility.h"

int bits_to_store(uint32 i)
{
  int result = 0;
  while(i) {
    result++;
    i >>= 1;
  }
  return result;
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
  for(int i=0;i<list.size();i++) {
    out.write_uint(abs(list[i]),low_bits);
    copy[i] = abs(list[i]) >> low_bits;
  }

  //Convert list into bitstream:
  
  vector<uint8> bits;

  int max = 0;
  for(int i=0;i<copy.size();i++)
    if (copy[i] > max)
      max = abs(copy[i]);
  
  for(int i=0;i<=max;i++) 
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
      if (bits[i+pos] != dominant)
        any = true;

    out.write(any);

    if (!any) {
      pos  += steplet;
      step += step / adapt_level;
    } else {
      int interloper=0;
      while(bits[pos+interloper] == dominant) interloper++;

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

  for(int i=0;i<list.size();i++)
    if (list[i])
      out.write(list[i]<0);
}
