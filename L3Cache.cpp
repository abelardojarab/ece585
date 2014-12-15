/******************************************************************************
 * AUTHORS: Sean Hendrickson
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

// constructors
L3Cache::L3Cache()
{

  numLines = 256;
  numSets = 16;

  L3CacheInit();

}

L3Cache::L3Cache(int numLines, int numSets)
{

  this->numLines = numLines;
  this->numSets = numSets;

  L3CacheInit();

}

// Allocates memory
int L3Cache::L3CacheInit() {

  void* raw_memory = operator new[](numSets*sizeof(L3Set));
  l3Sets = static_cast<L3Set*>( raw_memory );

  for (int i=0; i<numSets;i++) {
    new( &l3Sets[i] )L3Set(numLines);
  }

  return 0;
}

int L3Cache::processOpcode (int opcode, string address) {

  cout<<"Received opcode = "<<opcode<<", address = "<<address<<endl;
  switch (opcode) {

  case 0:
    break;

  case 1:
    break;

  case 2:
    break;

  case 3:
    break;

  case 4:
    break;

  case 5:
    break;

  case 6:
    break;

  case 8:
    break;

  default: // this includes 9
    break;
  }

  return 0;
}


// destructor
L3Cache::~L3Cache()
{

  //delete[] l3Sets;
  for (int i=0; i<numSets; i++) {
    l3Sets[i].~L3Set();
  }

  operator delete[]( l3Sets );

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
