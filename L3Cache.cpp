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

// constructors
L3Cache::L3Cache()
{

  numLines = 256;
  numSets = 16;

}

L3Cache::L3Cache(int numLines, int numSets)
{

  this->numLines = numLines;
  this->numSets = numSets;

}

// destructor
L3Cache::~L3Cache()
{

}
