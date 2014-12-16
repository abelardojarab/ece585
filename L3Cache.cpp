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

  string tag;
  tag = address.substr(1,indexMSBitIndex-1);

  std::bitset<32> bits(indexstr);
  unsigned int index=bits.to_ulong();

  string Result;          // string which will contain the result
  ostringstream convert;  // stream used for the conversion
  convert << index;       // insert the textual representation of 'Number' in the characters in the stream
  Result = convert.str(); // set 'Result' to the contents of the stream

  cout<<"Received opcode = "<<opcode<<", full address = "<<address<<", index = "<<Result<<endl;
  L3Line* current;
  int MESIFstate;
  switch (opcode) {

  case 0: // Read request from data cache - 32b
    // look for the line to get if it is hit or miss
    current = l3Sets[index].readData(Result); //check
    //pull up the MESIF state for this line
    MESIFstate = current.getMESIF();

    // if the line in the invalidate
    if (MESIFstate == 3){
      // if getsnoopresult == HIT
      if (getSnoopResult(tag) == 0){//check
        // Bus operation :  memory read
        busOperation("Memory read",address,"Hit");//check
        // change the state to Forward
        current.setMESIF(4);
      }// if getsnoopresult == NOHIT
      if (getSnoopResult(tag) == 1){
        // Bus operation :  memory read
        busOperation("Memory read",address,"No hit");//check
        // change the state to Exclusive
        current.setMESIF(1);
      }
    }
    // if the line in the ( MFSE )
    // don't check the getsnoopresult
    // don't do any bus operations
    // don't modify the state
    break;

  case 1: // Write request from data cache
    // check if you have the line and save the MESIF state value
    current = l3Sets[index].readData(Result); //check
    MESIFstate = current.getMESIF();
    // write to the line and the MESIF state to modified.
    l3Sets[index].writeData(Result,0);
    // if the line MESIF state is IFS
    if (MESIFstate == 3 || MESIFstate == 4 || MESIFstate == 2){
      // Bus operation : brodcast invalidate signal.
      busOperation("Invalidate",address,"");//check
    }
    // if the line MESIF state is EM
    // do nothing
    break;

  case 2: // Read request from intruction cache - 24b
    // look for the line to get if it is hit or miss
    current = l3Sets[index].readData(Result); //check
    //pull up the MESIF state for this line
    MESIFstate = current.getMESIF();

    // if the line in the invalidate
    if (MESIFstate == 3){
      // if getsnoopresult == HIT
      if (getSnoopResult(tag) == 0){//check
        // Bus operation :  memory read
        busOperation("Memory read",address,"Hit");//check
        // change the state to Forward
        current.setMESIF(4);
      }// if getsnoopresult == NOHIT
      if (getSnoopResult(tag) == 1){
        // Bus operation :  memory read
        busOperation("Memory read",address,"No hit");//check
        // change the state to Exclusive
        current.setMESIF(1);
      }
    }
    // if the line in the ( MFSE )
    // don't check the getsnoopresult
    // don't do any bus operations
    // don't modify the state
    break;

  case 3: // Snooped invalidate command
    // check if of the line is hit and get the MESIF state of that line
    if (l3Sets[index].checkHit(Result) == "hit")
      {// check
        current = l3Sets[index].readData(Result);
        MESIFstate = current.getMESIF();
        // if MESIF == S
        if (MESIFstate == 2){
          //invalidate the line
          current.setMESIF(3);
        }
        // if MESIF == FE
        if (MESIFstate == 4 || MESIFstate == 1){
          // invalidate the line
          current.setMESIF(3);
          // put snoop reult HIT
          putSnoopResult(address, 0);
          // Bus operation : Forward
          busOperation("Forward",address,"Hit");
        }
        // if MESIF == M
        if (MESIFstate == 0)
          {
            // invalidate the line
            current.setMESIF(3);
            // put snoop reult HIT
            putSnoopResult(address, 0);
            // Bus operation : write the line back
            busOperation("WriteBack",address,"Hit");
          }
        // message to L2 cache to invalidate its copy
        messageL2Cache("evict the line",current);
        //check
      }// otherwise if the line is miss put snoop result NOHIT
    else {
      putSnoopResult(address,1);
    }
    break;

  case 4: // Snooped read request
    // Check if the line is hit and get the MESIF state of the line
    if (l3Sets[index].checkHit(Result) == "hit")
      {
        // if hit
        current = l3Sets[index].readData(Result);
        MESIFstate = current.getMESIF();
        // if MESIF is Forward
        if (MESIFstate == 0){
          // Bus operation : forward the line
          busOperation("WriteBack",address,"Hit Modified");
          // put snoop result HIT
          putSnoopResult(address, 0);
          // change the MESIF to Shared
          current.setMESIF(2);
        }
        // if MESIF is Shared then do nothing
        // Do nothing
        // if MESIF is Exclusive
        if (MESIFstate == 1){
          // Bus operation : Forward the line
          busOperation("Forward",address,"Hit");
          // change the line MESIF to Shared
          current.setMESIF(2);
          // put snoop result HIT
          putSnoopResult(address, 0);
        }// if MESIF is Modified
        if (MESIFstate == 0){
          // Bus operation : WriteBack the line
          busOperation("WriteBack",address,"Hit Modified");
          // change the line MESIF to Shared
          current.setMESIF(2);
          // put snoop result HITM
          putSnoopResult(address, 2);
        }
      }
    // the CPU that request that line should wait (HOLD signal) until the line is written back to memory
    // than that line can be sniffed

    // if MESIF is Invalidate
    // Do noting
    // if NOHIT
    //Do nothing
    break;

  case 5: // Snooped write request
    // check if that line is missd to be sure that there is no error
    if (l3Sets[index].checkHit(Result) == "hit"){// check
      current.setMESIF(3);
    }
    // check if that line has been requisted and the write buffer have just write it back to memory, so you need to sniff it.

    break;

  case 6: // Snooped read with intent to modify
    // check if of the line is hit and get the MESIF state of that line
    if (l3Sets[index].checkHit(Result) == "hit"){// check
      current = l3Sets[index].readData(Result);
      MESIFstate = current.getMESIF();
      // if MESIF == S
      if (MESIFstate == 2){
        //invalidate the line
        current.setMESIF(3);
      }// if MESIF == FE
      if (MESIFstate == 4 || MESIFstate == 1){
        // invalidate the line
        current.setMESIF(3);
        // put snoop reult HIT
        putSnoopResult(address, 0);
        // Bus operation : Forward
        busOperation("Forward",address,"Hit");
      }
      // if MESIF == M
      if (MESIFstate == 0){
        // invalidate the line
        current.setMESIF(3);
        // put snoop reult HIT
        putSnoopResult(address, 0);
        // Bus operation : write the line back
        busOperation("WriteBack",address,"Hit");
      }
      // message to L2 cache to invalidate its copy
      messageL2Cache("evict the line", current);//check
        }
    // otherwise if the line is miss put snoop result NOHIT
    break;

  case 8: // Clear the cache and reset all state
    // clear all the cache content and put all states to invalidate
    for (int i = 0; i < (numSets - 1); i++){
      L3Set[i].flush();
    }
    break;

  default: // this includes 9, which is print contents and state of each valid cache line (allow subsequent activity)
    // print the valid lines and the State of those lines.
    for (int i = 0; i < (numSets - 1); i++){
      L3Set[i].printSet();
    }
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

/*
Used to simulate a bus operation and to capture the snoop results of last
level caches of other processors
*/
void busOperation(std::string BusOp, std::string address, std::string snoopResult)
{
<<<<<<< HEAD

=======
	*snoopResult = getSnoopResult(address);
	#ifndef SILENT
	printf("BusOp: %s, Address: %s, Snoop Result: %s\n", *snoopResult);
	#endif
>>>>>>> 3faa195c9e18212b0e341bf100bd6cd870c109e6
  return 0;
}

/*
 * DESC: Used to simulate the reporting of snoop results by other caches
 * PARAM: state - MESIF, M=0, E=1, S=2, I=3, F=4
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

/*

*/
void putSnoopResult(int address, int snoopResult)
{
	#ifndef SILENT
	printf("SnoopResult: Address %h, SnoopResult: %d\n", address, snoopResult);
	#endif
}

/*
Used to simulate commin
*/
void messageL2Cache(int busOp, int snoopResult)
{
	#ifndef SILENT
	printf("L2: %d %h\n", busOperation, address);
	#endif
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

}
