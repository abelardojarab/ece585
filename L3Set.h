/******************************************************************************
 * AUTHORS: Sean Hendrickson, Khalid ALkhulayfi
 * FILE: L3Set.h
 * LAST MODIFIED: 11/24/2014
 * DESCRIPTION: This file contains the class definition for the L3Set class.
 *              This class defines the object of set for an L3 cache.
 *****************************************************************************/
 
#pragma once
#include <cstdlib>
#include <string>
#include <cstring>
#include "L3Line.h"

using namespace std;

class L3Set
{
  public:
    // constructors
	L3Set(const int numLines);
	
	// destructor
	~L3Set();
	
    /**
	* DESC: search lines for a matching tag
	* PARAM: tag - binary string of tag bits
	* RETURN: return "hit" or "miss"
	*/	
	string readData(string tag);
	
	/**
	* DESC: writes to a line
	* PARAM: tag - write to tag bits in line
	*        mesifState - the current state of the line
	* RETURN: "done" when operation completed
	*/
	string writeData(string tag, int mesifState);

	/**
	* DESC: check if a line is in the set
	* PARAM: tag - used to compare lines in set
	* RETURN: "hit" if match found
	*         "miss" if match not found
	*/
	string checkHit(string tag);

	/**
	* DESC: change mesif state of a line based on the lines
	*       current state.  First the line must be found
	* PARAM: tag - tag bits used to find matching line
	*        mesifState - state bits
	* RETURN: "done\n" when operation finished
	*         "miss\n" if matching tag not found 
	*/
	string mesifStateModifier(string tag, int mesifState);

  private:

    /**
	* DESC:
	* PARAM:
	* RETURN:
	* PRE-CONDITION:
	* POST-CONDITION:
	*/
	int getLRU();
	
    /**
	* DESC: evict a line from the set
	* PARAM: evictedLineNum - location of line in set
	* RETURN: return tag of evicted line
	*/	
	string evict(int evictedLineNum);
	
	/**
	* DESC:
	* PARAM:
	* RETURN:
	* PRE-CONDITION:
	* POST-CONDITION:
	*/
	void changeLRUBits(int numLine);
	
	int numLines;
	L3Line *l3Line;
	string lruBits;
}; 

