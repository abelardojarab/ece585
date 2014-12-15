/******************************************************************************
 * AUTHORS: Sean Hendrickson
 * FILE: L3Cache.h
 * LAST MODIFIED: 11/24/2014
 * DESCRIPTION: This file defines the L3Cache class.
 *****************************************************************************/

#pragma once
#include "L3Set.h"
#include <string>

class L3Cache
{
public:
  // constructors
  L3Cache();
  L3Cache(int lineSize, int numLines, int numSets);

  // destructor
  ~L3Cache();

  int L3CacheInit();


  int processOpcode(int opcode, string address);

  /**
   * DESC:
   * PARAM:
   * RETURN:
   * PRE-CONDITION:
   * POST-CONDITION:
   */
  int busOperation();

  /**
   * DESC:
   * PARAM: state - MESIF, M=0, E=1, S=2, I=3, F=4
   * RETURN:
   * PRE-CONDITION:
   * POST-CONDITION:
   */
  int getSnoopResult(string tag);

  int putSnoopResult(int address, int snoopResult);

  /**
   * DESC:
   * PARAM:
   * RETURN:
   * PRE-CONDITION:
   * POST-CONDITION:
   */
  void messageL2Cache(int busOp, int snoopResult);

  /**
   * DESC: reads the trace file
   * PARAM:
   * RETURN:
   * PRE-CONDITION:
   * POST-CONDITION:
   */
  int setEvent(int opcode, int address);

  /**
   * DESC: reads the trace file
   * PARAM:
   * RETURN:
   * PRE-CONDITION:
   * POST-CONDITION:
   */
  int getAddr(int address);

  /**
   * DESC:
   * PARAM:
   * RETURN:
   * PRE-CONDITION:
   * POST-CONDITION:
   */
  int setAddr(int address);

  /**
   * DESC:
   * PARAM:
   * RETURN:
   * PRE-CONDITION:
   * POST-CONDITION:
   */
  int mappingIndex();

private:
  int LRU_bits;
  int MESI_bits;
  int hit;
  int noHit;
  int lineSize;
  int numLines;
  int numSets;
  L3Set* l3Sets;
  void* raw_memory;
};
