/******************************************************************************
 * AUTHORS: Sean Hendrickson, Khalid ALkhulayfi
 * FILE: L3Set.h
 * LAST MODIFIED: 11/24/2014
 * DESCRIPTION: This file contains the class definition for the L3Set class.
 *              This class defines the object of set for an L3 cache.
 *****************************************************************************/
 
#pragma once
#include <stdlib.h>
#include <string>
using namespace std;

class L3Set
{
  public:
    // constructors
	L3Set(const int numLines);
	
	// destructor
	~L3Set();
	
    /**
	* DESC:
	* PARAM:
	* RETURN:
	* PRE-CONDITION:
	* POST-CONDITION:
	*/	
	string readData(unsigned int tag);
	
	/**
	* DESC:
	* PARAM:
	* RETURN:
	* PRE-CONDITION:
	* POST-CONDITION:
	*/
	string writeData(unsigned int tag, int mesifState);

	/**
	* DESC:
	* PARAM:
	* RETURN:
	* PRE-CONDITION:
	* POST-CONDITION:
	*/
	string checkHit(unsigned int tag);

	/**
	* DESC:
	* PARAM:
	* RETURN:
	* PRE-CONDITION:
	* POST-CONDITION:
	*/
	string mesifStateModifier(unsigned int tag, int mesifState);

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
	* DESC:
	* PARAM:
	* RETURN:
	* PRE-CONDITION:
	* POST-CONDITION:
	*/	
	unsigned int evict(int evictedLineNum);
	
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

