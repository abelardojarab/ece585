/******************************************************************************
 * AUTHORS: Sean Hendrickson, Khalid Alkhulayfi
 * FILE: L3Line.h
 * LAST MODIFIED: 11/24/2014
 * DESCRIPTION: This file defines the L3Line class.
 *****************************************************************************/

#pragma once
#include <string>
#include <cstring>

using namespace std;

class L3Line
{
  public:
    /**
    * DESC: constructor to set a lines TAG bits and its current
    *       state using MESIF.
    */
    L3Line();

    /**
    * DESC: constructor to set a lines TAG bits and its current
    *       state using MESIF.
    * PARAM: tag - the cache lines tag bits
	*        mesifState - the state bits for a line
    * POST-CONDITION: sets the number of state bits
    */
    L3Line(unsigned int tag, int mesifState);

    // destructor
    ~L3Line();

    /**
    * DESC:  Used to change the MESIF state of the line
    * PARAM: state - changes the mesifState of the line
    */
    void setMESIF(int state);

    /**
    * DESC: Used to return the MESIF state of a line
    * RETURN: mesif state of line 
    */
    int getMESIF();

    /**
    * DESC: set tag bits of line
    * PARAM: tag - binary string of tag bits
    */
    void setTag(string tag);


    /**
    * DESC: Returns the line's TAG bits
    * RETURN: binary string of tag bits
    */
    string getTag();

    /**
    * DESC: Used to alter the dirty bit
    * PARAM: x - changes value of dirty bit
    */
    void setDirtyBit(int x);

    /**
    * DESC: Used to return the dirty bit
    * RETURN: returns value of dirty bit
    */
    int getDirtyBit();

    /**
    * DESC: prints changes to the line
    */
    void print();

  private:
      string tagBits;
      int mesifState; // M=0, E=1, S=2,I=3, F=4
      int dirtyBit;
};
