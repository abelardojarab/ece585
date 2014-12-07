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
	* DESC:
	* PARAM:
	* RETURN:
	* PRE-CONDITION:
	* POST-CONDITION:
	*/	
	string readData(string tag);
	
	/**
	* DESC:
	* PARAM:
	* RETURN:
	* PRE-CONDITION:
	* POST-CONDITION:
	*/
	string writeData(string tag, int mesifState);

	/**
	* DESC:
	* PARAM:
	* RETURN:
	* PRE-CONDITION:
	* POST-CONDITION:
	*/
	string checkHit(string tag);

	/**
	* DESC:
	* PARAM:
	* RETURN:
	* PRE-CONDITION:
	* POST-CONDITION:
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
	* DESC:
	* PARAM:
	* RETURN:
	* PRE-CONDITION:
	* POST-CONDITION:
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

