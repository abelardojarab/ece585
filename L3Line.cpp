/******************************************************************************
 * AUTHORS: Sean Hendrickson, Khalid ALkhulayfi
 * FILE: L3Line.cpp
 * LAST MODIFIED: 11/24/2014
 * DESCRIPTION: This file implements the L3Line class.
 *****************************************************************************/
 
#include "L3Line.h" 
#include <iostream>
#include <bitset>

using namespace std;

/**
* DESC: constructor to set a lines TAG bits and its current
*       state using MESIF.  The MESIF is 6 bits due to 64 Byte
*       data. The TAG is 58 bits since 64 byte address.
* PARAM:
* RETURN:
* PRE-CONDITION:
* POST-CONDITION:
*/
L3Line::L3Line()
{
	tagBits = '0';
	mesifState = 3;
	dirtyBit = 0;
}

/**
* DESC: constructor to set a lines TAG bits and its current
*       state using MESIF.  The MESIF is 6 bits due to 64 Byte
*       data. The TAG is 58 bits since 64 byte address.
* PARAM:
* RETURN:
* PRE-CONDITION:
* POST-CONDITION:
*/
L3Line::L3Line(unsigned int tag, int mesifState)
{
	tagBits = tag;
	this->mesifState = mesifState;
	dirtyBit = 0;
}

// destructor
L3Line::~L3Line()
{
	tagBits = '0';
	mesifState = 3;
	dirtyBit = 0;
}

/**
* DESC:  Used to change the MESIF state of the line
* PARAM:
* RETURN:
* PRE-CONDITION:
* POST-CONDITION:
*/
void L3Line::setMESIF(int state)
{
	mesifState = state;
}

/**
* DESC: Used to return the MESIF state of the line
* PARAM:
* RETURN:
* PRE-CONDITION:
* POST-CONDITION:
*/
int L3Line::getMESIF()
{
	return mesifState;
}

/**
* DESC: 
* PARAM:
* RETURN:
* PRE-CONDITION:
* POST-CONDITION:
*/
void L3Line::setTag(string tag)
{
	tagBits = tag;
}

/**
* DESC: Returns the line's TAG bit
* PARAM:
* RETURN:
* PRE-CONDITION:
* POST-CONDITION:
*/
string L3Line::getTag()
{
	return tagBits;
}

/**
* DESC: Used to toggle the dirty bit
* PARAM:
* RETURN:
* PRE-CONDITION:
* POST-CONDITION:
*/
void L3Line::setDirtyBit(int x)
{
	dirtyBit = x;
}

/**
* DESC: Used to return the dirty bit
* PARAM:
* RETURN:
* PRE-CONDITION:
* POST-CONDITION:
*/
int L3Line::getDirtyBit()
{
	return dirtyBit;
}

/**
* DESC: prints changes to the line
* PARAM:
* RETURN:
* PRE-CONDITION:
* POST-CONDITION:
*/
void L3Line::print()
{
	cout << "tag = " << tagBits << ", mesif = " << mesifState
		<< ", dirty = " << dirtyBit << endl;
}

