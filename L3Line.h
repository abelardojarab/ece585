/******************************************************************************
 * AUTHORS: Sean Hendrickson
 * FILE: L3Line.h
 * LAST MODIFIED: 11/24/2014
 * DESCRIPTION: This file defines the L3Line class.
 *****************************************************************************/
 
#pragma once

class L3Line
{
  public:
    // constructors
	L3Cache();
	L3Cache(int numBits);
	
	// destructor
	~L3Set();
	
    /**
	* DESC:
	* PARAM:
	* RETURN:
	* PRE-CONDITION:
	* POST-CONDITION:
	*/	
	int getLine();
	
    /**
	* DESC:
	* PARAM:
	* RETURN:
	* PRE-CONDITION:
	* POST-CONDITION:
	*/	
	int setLine();
	
  private:
	// line size
	// line bits
}; 

