/******************************************************************************
 * AUTHORS: Sean Hendrickson
 * FILE: L3Set.h
 * LAST MODIFIED: 11/24/2014
 * DESCRIPTION: This file contains the class definition for the L3Set class.
 *              This class defines the object of set for an L3 cache.
 *****************************************************************************/
 
#pragma once
#include <string.h>

class L3Set
{
  public:
    // constructors
	L3Set();
	L3Set(int numLines);
	
	// destructor
	~L3Set();
	
    /**
	* DESC:
	* PARAM:
	* RETURN:
	* PRE-CONDITION:
	* POST-CONDITION:
	*/	
	string getData(int address);
	
    /**
	* DESC: 
	* PARAM: state - MESIF, M=0, E=1, S=2, I=3, F=4
	* RETURN:
	* PRE-CONDITION:
	* POST-CONDITION:
	*/	
	unsigned int saveData(int address, int state)
	
	
  private:

    /**
	* DESC:
	* PARAM:
	* RETURN:
	* PRE-CONDITION:
	* POST-CONDITION:
	*/
	L3Line getLRU();
	
    /**
	* DESC:
	* PARAM:
	* RETURN:
	* PRE-CONDITION:
	* POST-CONDITION:
	*/	
	unsigned int evict();

	L3Line L3Line[numLines]
	LRUbits;
	numLines;
}; 

