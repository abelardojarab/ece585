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
    * PARAM:
    * RETURN:
    * PRE-CONDITION:
    * POST-CONDITION:
    */
    L3Line();

    /**
    * DESC: constructor to set a lines TAG bits and its current
    *       state using MESIF.
    * PARAM:
    * RETURN:
    * PRE-CONDITION:
    * POST-CONDITION:
    */
    L3Line(unsigned int tag, int mesifState);

    // destructor
    ~L3Line();

    /**
    * DESC:  Used to change the MESIF state of the line
    * PARAM:
    * RETURN:
    * PRE-CONDITION:
    * POST-CONDITION:
    */
    void setMESIF(int state);

    /**
    * DESC: Used to return the MESIF state of the line
    * PARAM:
    * RETURN:
    * PRE-CONDITION:
    * POST-CONDITION:
    */
    int getMESIF();

    /**
    * DESC:
    * PARAM:
    * RETURN:
    * PRE-CONDITION:
    * POST-CONDITION:
    */
    void setTag(string tag);


    /**
    * DESC: Returns the line's TAG bit
    * PARAM:
    * RETURN:
    * PRE-CONDITION:
    * POST-CONDITION:
    */
    string getTag();

    /**
    * DESC: Used to toggle the dirty bit
    * PARAM:
    * RETURN:
    * PRE-CONDITION:
    * POST-CONDITION:
    */
    void setDirtyBit(int x);

    /**
    * DESC: Used to return the dirty bit
    * PARAM:
    * RETURN:
    * PRE-CONDITION:
    * POST-CONDITION:
    */
    int getDirtyBit();

    /**
    * DESC: prints changes to the line
    * PARAM:
    * RETURN:
    * PRE-CONDITION:
    * POST-CONDITION:
    */
    void print();

  private:
      string tagBits;
      int mesifState; // M=0, E=1, S=2,I=3, F=4
      int dirtyBit;
};
