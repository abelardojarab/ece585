/******************************************************************************
 * AUTHORS: Sean Hendrickson, Khalid ALkhulayfi
 * FILE: L3Set.cpp
 * LAST MODIFIED: 11/24/2014
 * DESCRIPTION: This file implements the L3Set class.
 *****************************************************************************/
 
#include "L3Set.h"
#include "L3Line.h" 

// constructors
L3Set::L3Set(const int numLines)
{
	this->numLines = numLines;
	l3Line = new L3Line[numLines - 1];
	lruBits = "000000000000000";
	
}

// destructor
L3Set::~L3Set()
{
	delete l3Line;
	numLines = 0;
	lruBits = "000000000000000";
}

/**
* DESC:
* PARAM:
* RETURN:
* PRE-CONDITION:
* POST-CONDITION:
*/
string L3Set::readData(unsigned int tag)
{
	for(int i = 0; i < numLines; ++i)
	{
		if (l3Line[i].getTag() == tag)
		{
			return "hit\n";
			changeLRUBits(i);
			break;
		}
		else if (i == (numLines - 1))
		{
			return "miss\n";
		}
	}
}

/**
* DESC:
* PARAM:
* RETURN:
* PRE-CONDITION:
* POST-CONDITION:
*/
string L3Set::writeData(unsigned int tag, int mesifState)
{
	for (int i = 0; i < numLines; ++i)
	{
		if (l3Line[i].getTag() == 0)
		{
			l3Line[i].setTag(tag);
			l3Line[i].setMESIF(mesifState);
			if (mesifState == 0) l3Line[i].setDirtyBit(1);
			changeLRUBits(i);
			return "done\n";
		}
		else if (i == (numLines - 1))
		{
			return "No Space\n";
			int evictedLineNum = getLRU();
			evict(evictedLineNum);
			l3Line[evictedLineNum].setTag(tag);
			l3Line[evictedLineNum].setMESIF(mesifState);
			if (mesifState == 0) l3Line[evictedLineNum].setDirtyBit(1);
			changeLRUBits(evictedLineNum);
			return "done\n";// return the address to write back it to Memo.
		}
	}
}


/**
* DESC:
* PARAM:
* RETURN:
* PRE-CONDITION:
* POST-CONDITION:
*/
string L3Set::checkHit(unsigned int tag)
{
	for (int i = 0; i < numLines; ++i)
	{
		if (l3Line[i].getTag() == tag)
		{
			return "hit\n";
		}
		else if (i == (numLines - 1))
		{
			return "miss\n";
		}
	}
}

/**
* DESC:
* PARAM:
* RETURN:
* PRE-CONDITION:
* POST-CONDITION:
*/
string L3Set::mesifStateModifier(unsigned int tag, int mesifState)
{
	for (int i = 0; i < numLines; ++i)
	{
		if (l3Line[i].getTag() == tag)
		{
			if ( mesifState == 3 ){ // 3 is invalidate state.
				evict(i);
			}else{ l3Line[i].setMESIF(mesifState); }
			return "done\n";// return the address to write back it to Memo.
		}
		else if (i == (numLines - 1)) // if miss, then it is error.
		{
			return "miss\n";
		}
	}
}


/**
* DESC:
* PARAM:
* RETURN:
* PRE-CONDITION:
* POST-CONDITION:
*/
int L3Set::getLRU()
{
	if ( lruBits.at(0) == 0){
		if ( lruBits.at(1) == 0){
			if ( lruBits.at(3) == 0){
				if ( lruBits.at(7) == 0){
					return 0;
				}else if ( lruBits.at(7) == 1){
					return 1;
				}
			}else if(lruBits.at(3) == 1){
				if ( lruBits.at(8) == 0){
					return 2;
				}else if ( lruBits.at(8) == 1){
					return 3;
				}
			}
		}else if(lruBits.at(1) == 1){
			if ( lruBits.at(4) == 0){
				if ( lruBits.at(9) == 0){
					return 4;
				}else if ( lruBits.at(9) == 1){
					return 5;
				}
			}else if(lruBits.at(4) == 1){
				if ( lruBits.at(10) == 0){
					return 6;
				}else if ( lruBits.at(10) == 1){
					return 7;
				}
			}
		}
	}else if ( lruBits.at(0) == 1){
		if ( lruBits.at(2) == 0){
			if ( lruBits.at(5) == 0){
				if ( lruBits.at(11) == 0){
					return 8;
				}else if ( lruBits.at(11) == 1){
					return 9;
				}
			}else if(lruBits.at(5) == 1){
				if ( lruBits.at(12) == 0){
					return 10;
				}else if ( lruBits.at(12) == 1){
					return 11;
				}
			}
		}else if(lruBits.at(2) == 1){
			if ( lruBits.at(6) == 0){
				if ( lruBits.at(13) == 0){
					return 12;
				}else if ( lruBits.at(13) == 1){
					return 13;
				}
			}else if(lruBits.at(6) == 1){
				if ( lruBits.at(14) == 0){
					return 14;
				}else if ( lruBits.at(14) == 1){
					return 15;
				}
			}
		}
	}
}

/**
* DESC:
* PARAM:
* RETURN:
* PRE-CONDITION:
* POST-CONDITION:
*/
void L3Set::changeLRUBits(int numLine)
{
	switch (numLine)
	{
	case 0: 
		lruBits.at(0) = "1";
		lruBits.at(1) = "1";
		lruBits.at(3) = "1";
		lruBits.at(7) = "1";
		break;
	case 1: 
		lruBits.at(0) = "1";
		lruBits.at(1) = "1";
		lruBits.at(3) = "1";
		lruBits.at(7) = "0";
		break;
	case 2: 
		lruBits.at(0) = "1";
		lruBits.at(1) = "1";
		lruBits.at(3) = "0";
		lruBits.at(8) = "1";
		break;
	case 3:
		lruBits.at(0) = "1";
		lruBits.at(1) = "1";
		lruBits.at(3) = "0";
		lruBits.at(8) = "0";
		break;
	case 4:
		lruBits.at(0) = "1";
		lruBits.at(1) = "0";
		lruBits.at(4) = "1";
		lruBits.at(9) = "1";
		break;
	case 5:
		lruBits.at(0) = "1";
		lruBits.at(1) = "0";
		lruBits.at(4) = "1";
		lruBits.at(9) = "0";
		break;
	case 6:
		lruBits.at(0) = "1";
		lruBits.at(1) = "0";
		lruBits.at(4) = "0";
		lruBits.at(10) = "1";
		break;
	case 7:
		lruBits.at(0) = "1";
		lruBits.at(1) = "0";
		lruBits.at(4) = "0";
		lruBits.at(10) = "0";
		break;
	case 8:
		lruBits.at(0) = "0";
		lruBits.at(2) = "1";
		lruBits.at(5) = "1";
		lruBits.at(11) = "1";
		break;
	case 9:
		lruBits.at(0) = "0";
		lruBits.at(2) = "1";
		lruBits.at(5) = "1";
		lruBits.at(11) = "0";
		break;
	case 10:
		lruBits.at(0) = "0";
		lruBits.at(2) = "1";
		lruBits.at(5) = "0";
		lruBits.at(12) = "1";
		break;
	case 11:
		lruBits.at(0) = "0";
		lruBits.at(2) = "1";
		lruBits.at(5) = "0";
		lruBits.at(12) = "0";
		break;
	case 12:
		lruBits.at(0) = "0";
		lruBits.at(2) = "0";
		lruBits.at(6) = "1";
		lruBits.at(13) = "1";
		break;
	case 13:
		lruBits.at(0) = "0";
		lruBits.at(2) = "0";
		lruBits.at(6) = "1";
		lruBits.at(13) = "0";
		break;
	case 14:
		lruBits.at(0) = "0";
		lruBits.at(2) = "0";
		lruBits.at(6) = "0";
		lruBits.at(14) = "1";
		break;
	case 15:
		lruBits.at(0) = "0";
		lruBits.at(2) = "0";
		lruBits.at(6) = "0";
		lruBits.at(14) = "0";
		break;
	default: 
		break;
	}
}


/**
* DESC:
* PARAM:
* RETURN:
* PRE-CONDITION:
* POST-CONDITION:
*/
string L3Set::evict(int evictedLineNum)
{
	string s = l3Line[evictedLineNum].getTag();
	l3Line[evictedLineNum].setTag(0);
	l3Line[evictedLineNum].setMESIF(3);
	l3Line[evictedLineNum].setDirtyBit(0);
	return s;
}
