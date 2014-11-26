/******************************************************************************
 * AUTHORS: Sean Hendrickson
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
	lruBits = 0;
	
}

// destructor
L3Set::~L3Set()
{
	delete l3Line;
	numLines = 0;
	lruBits = 0;
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
		if (l3Line[i].getTag == tag)
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
		if (l3Line[i].getTag == 0)
		{
			l3Line[i].setTag = tag;
			l3Line[i].setMESIF = mesifState;
			if (mesifState = 0) l3Line[i].setDirtyBit;
			changeLRUBits(i);
			return "done\n";
			break;
		}
		else if (i == (numLines - 1))
		{
			return "No Space\n";
			int evictedLineNum = getLRU();
			evict(evictedLineNum);
			l3Line[evictedLineNum].setTag = tag;
			l3Line[evictedLineNum].setMESIF = mesifState;
			if (mesifState = 0) l3Line[evictedLineNum].setDirtyBit;
			changeLRUBits(evictedLineNum);
			return "done\n";
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
		if (l3Line[i].getTag == tag)
		{
			return "hit\n";
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
string L3Set::mesifStateModifier(unsigned int tag, int mesifState)
{
	for (int i = 0; i < numLines; ++i)
	{
		if (l3Line[i].getTag == tag)
		{
			l3Line[i].setMESIF = mesifState;
			return "done\n";
			break;
		}
		else if (i == (numLines - 1)) // if miss, then it is error.
		{
			return "miss\n";
		}
	}
}

/**
* DESC: Print the type of miss that occured
* PARAM:
* RETURN:
* PRE-CONDITION:
* POST-CONDITION:
*/
void L3Set::printMissType()
{}

/**
* DESC:
* PARAM:
* RETURN:
* PRE-CONDITION:
* POST-CONDITION:
*/
int L3Set::getLRU()
{

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
	case 1: 
		b = toggleBit(b);
		b = toggleBit(b);
		b = toggleBit(b);
		b = toggleBit(b);
		break;
	case 2: 
	case 3:
		b = toggleBit(b);
		b = toggleBit(b);
		b = toggleBit(b);
		b = toggleBit(b);
		break;
	case 4:
	case 5:
		b = toggleBit(b);
		b = toggleBit(b);
		b = toggleBit(b);
		b = toggleBit(b);
		break;
	case 6:
	case 7:
		b = toggleBit(b);
		b = toggleBit(b);
		b = toggleBit(b);
		b = toggleBit(b);
		break;
	case 8:
	case 9:
		b = toggleBit(b);
		b = toggleBit(b);
		b = toggleBit(b);
		b = toggleBit(b);
		break;
	case 10:
	case 11:
		b = toggleBit(b);
		b = toggleBit(b);
		b = toggleBit(b);
		b = toggleBit(b);
		break;
	case 12:
	case 13:
		b = toggleBit(b);
		b = toggleBit(b);
		b = toggleBit(b);
		b = toggleBit(b);
		break;
	case 14:
	case 15:
		b = toggleBit(b);
		b = toggleBit(b);
		b = toggleBit(b);
		b = toggleBit(b);		
		break;
	default: 
		break;
	}
}

int L3Set::toggleBit(int bit)
{
	if (bit == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
* DESC:
* PARAM:
* RETURN:
* PRE-CONDITION:
* POST-CONDITION:
*/
unsigned int L3Set::evict(int evictedLineNum)
{}