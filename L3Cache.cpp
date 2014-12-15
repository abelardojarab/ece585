/******************************************************************************
 * AUTHORS: Sean Hendrickson, Khalid Alkhulayfi,
 * FILE: L3Cache.cpp
 * LAST MODIFIED: 11/24/2014
 * DESCRIPTION: This file implements the L3Cache class.
 *****************************************************************************/

#include "L3Cache.h"
#include "L3Set.h"
#include "L3Line.h"
#include <string>
#include <cstring>
#include <iostream>
#include <cmath>
#include <sstream>
#include <bitset>

using namespace std;

// constructors
L3Cache::L3Cache()
{

  lineSize = 64;
  numLines = 256;
  numSets = 16;

  L3CacheInit();

}

L3Cache::L3Cache(int lineSize, int numLines, int numSets)
{

  this->lineSize = lineSize;
  this->numLines = numLines;
  this->numSets = numSets;

  L3CacheInit();

}

// Allocates memory
int L3Cache::L3CacheInit() {

  raw_memory = operator new[](numSets*sizeof(L3Set));
  l3Sets = static_cast<L3Set*>( raw_memory );

  for (int i=0; i<numSets;i++) {
    new( &l3Sets[i] )L3Set(numLines);
  }

  indexMSBitIndex = 32-log2(lineSize)-log2(numSets)+1;
  indexLSBitIndex = 32-log2(lineSize);

  cout<<"tag MSB = "<<indexMSBitIndex<<", tag LSB = "<<indexLSBitIndex<<endl;
  return 0;
}

int L3Cache::processOpcode (int opcode, string address) {

  string indexstr;
  indexstr = address.substr(indexMSBitIndex,log2(numSets));

  std::bitset<32> bits(indexstr);
  unsigned int index=bits.to_ulong();

  string Result;          // string which will contain the result
  ostringstream convert;  // stream used for the conversion
  convert << index;       // insert the textual representation of 'Number' in the characters in the stream
  Result = convert.str(); // set 'Result' to the contents of the stream

  cout<<"Received opcode = "<<opcode<<", full address = "<<address<<", index = "<<Result<<endl;

  switch (opcode) {

  case 0: // Read request from data cache
    // look for the line to get if it is hit or miss
    l3Sets[index].readData(Result);
    // set this line to be MRU.
    // if the line in the invalidate do memory read then check if the snoop result is HIT then sniff
    // and put the line in the Forward State. if the get a snoop result that is NOHIT then put the
    // MESIF in the Exclusive state.
    /*readCount++;
    way = checkTag(index, tag);
    // if the tag exists
    if (way <= MAXWAY)
      {
        int MESI = L2cache[index][way].MESIbits;
        // if this tag exists and it's valid as per its MESI bits
        if (MESI == M || MESI == E || MESI == S)
          {
            hitCount++;
            updateLRU(index, way);
            // MESI remains unchanged
          }
        // if this tag exists but it's been invalidated and can't be used...
        // we fetch from DRAM and pass on to L1 cache, update to exclusive
        else
          {
            missCount++;
            updateLRU(index, way);
            L2cache[index][way].MESIbits = E;
          }
      }
    // this tag simply doesn't exist in the cache in any form
    else
      {
        missCount++;
        // use the LRU bits to determine which way to evict
        way = checkLRU(index);
        updateLRU(index, way);
        L2cache[index][way].tag = tag;
        L2cache[index][way].MESIbits = E;
      }
      L2cache[index][way].address = addr;*/
    break;

  case 1: // Write request from data cache
    // write to the line.
    l3Sets[index].writeData(Result,0);
    // brodcast invalidate signal in the bus
    break;

  case 2: // Read request from intruction cache
    // look for the line to get if it is hit or miss
    // l3Sets[index].readData(Result);
    // set this line to be MRU.
    // if the line in the invalidate do memory read otherwise
    break;

  case 3: // Snooped invalidate command
    // check if of the line is hit
      // invalidate the line
      // if the MESIF is ( F, E ) put snoop result HIT  and forward the line
      // if the MESIF is Shared do nothing
      // if the MESIF is modified then put HITM and write back the line
    // otherwise if the line is miss put snoop result NOHIT
    break;

  case 4: // Snooped read request
    // Check if the line is hit and check the MESIF state of the line
    // if hit
      // if MESIF is Forward then forward the line and put snoop result HIT change the MESIF to Shared
      // if MESIF is Shared then do nothing
      // if MESIF is Exclusive then Forward and change the line MESIF to Shared and put snoop result HIT
      // if MESIF is Modified then WriteBack the line and change the line MESIF to Shared and put snoop result HITM
        // the CPU that request that line should wait (HOLD signal) until the line is written back to memory
        // than that line can be sniffed
    // if NOHIT
      //Do nothing
    break;

  case 5: // Snooped write request
    // check if that line is missd to be soure that there is no error
    // check if that line has been requisted and the write buffer have just write it back to memory, so you need to sniff it.
    break;

  case 6: // Snooped read with intent to modify
    // check if of the line is hit
      // invalidate the line
      // if the MESIF is ( F, E ) put snoop result HIT  and forward the line
      // if the MESIF is Shared do nothing
      // if the MESIF is modified then put HITM and write back the line
    // otherwise if the line is miss put snoop result NOHIT
    break;

  case 8: // Clear the cache and reset all state
    // clear all the cache content and put all states to invalidate
    break;

  default: // this includes 9, which is print contents and state of each valid cache line (allow subsequent activity)
    // print the valid lines and the State of those lines.
    break;
  }

  return 0;
}


// destructor
L3Cache::~L3Cache()
{

  //delete[] l3Sets;
  for (int i=0; i<= numSets-1; i++) {
    l3Sets[i].~L3Set();
  }

  operator delete[]( raw_memory );

}

/**
 * DESC:
 * PARAM:
 * RETURN:
 * PRE-CONDITION:
 * POST-CONDITION:
 */
int busOperation()
{

  return 0;
}

/**
 * DESC:
 * PARAM: state - MESIF, M=0, E=1, S=2, I=3, F=4
 * RETURN:
 * PRE-CONDITION:
 * POST-CONDITION:
 */

int getSnoopResult(string tag)
{
  if ((tag.at(0)==0)&&(tag.at(1)==0))
    return 0; // Return HIT
  else if ((tag.at(0)==0)&&(tag.at(1)==0))
    return 1; // Return NOHIT
  else if ((tag.at(0)==1)&&(tag.at(1)==0))
    return 2; // Return HITM
  else
    return 3; // Not implemented
}

int putSnoopResult(int address, int snoopResult)
{

  return 0;
}

/**
 * DESC:
 * PARAM:
 * RETURN:
 * PRE-CONDITION:
 * POST-CONDITION:
 */
void messageL2Cache(int busOp, int snoopResult)
{

}

/**
 * DESC: reads the trace file
 * PARAM:
 * RETURN:
 * PRE-CONDITION:
 * POST-CONDITION:
 */
int setEvent(int opcode, int address)
{
  return 0;
}

/**
 * DESC: reads the trace file
 * PARAM:
 * RETURN:
 * PRE-CONDITION:
 * POST-CONDITION:
 */
int getAddr(int address)
{
    return 0;
}

/**
 * DESC:
 * PARAM:
 * RETURN:
 * PRE-CONDITION:
 * POST-CONDITION:
 */
int setAddr(int address)
{

  return 0;
}

/**
 * DESC:
 * PARAM:
 * RETURN:
 * PRE-CONDITION:
 * POST-CONDITION:
 */
int mappingIndex()
{
    return 0;
}
