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
  lruTree = new CLruTree(numLines);
  //l3Line = new L3Line[numLines];  //
  //lruBits = "000000000000000";

}

// destructor
L3Set::~L3Set()
{
  numLines = 0;
  delete lruTree;
  //for(int i=0; i<numLines; i++) {
  //  l3Line[i].~L3Line();
  //}
  //delete[] l3Line;
  //lruBits = "000000000000000";
}

/**
 * DESC: search lines for a matching tag
 * PARAM: tag - binary string of tag bits
 * RETURN: return "hit" or "miss"
 */
L3Line* L3Set::readData(string tag)
{
	L3Line* temp = lruTree->getLine(tag);
	if (temp != NULL)
	{   // matching tag found
		return temp;
	}
	else
	{   // matching tag not found
		return NULL;
	}
	return NULL;  // function should not get to this point
}

/**
 * DESC: writes to a line
 * PARAM: tag - write to tag bits in line
 *        mesifState - the current state of the line
 * RETURN: "done" when operation completed
 */
string L3Set::writeData(string tag, int mesifState)
{
	// create new line to be written
	L3Line* temp = new L3Line;
	temp->setTag(tag);
	temp->setMESIF(mesifState);
	int ret = lruTree->setLine(temp);  // place line in cache
	L3Line* holdVictim = NULL;  // store the victim line

	if (ret == 0)
	{   // space found for new line
		if (mesifState == 0) temp->setDirtyBit(1);
		return "done\n";
	}
	else if (ret < 0)
	{   // line needs to be evicted

		if ((holdVictim = lruTree->evictLine(temp)) == NULL)
		{   // eviction failed
			if (!SILENT) std::cout << "ERROR - Eviction Failed\n";
			return "Efailed\n";
		}
		if (mesifState == 0) holdVictim->setDirtyBit(1);
		return holdVictim->getTag();  // return tag of evicted line

		
		/*int evictedLineNum = getLRU();
		evict(evictedLineNum);
		l3Line[evictedLineNum].setTag(tag);
		l3Line[evictedLineNum].setMESIF(mesifState);
		if (mesifState == 0) l3Line[evictedLineNum].setDirtyBit(1);
		changeLRUBits(evictedLineNum);
		return "done\n";// return the address to write back it to Memo.*/
	}
	else
	{
		return "done\n";  // function should not get to this point
	}
}


/**
 * DESC: check if a line is in the set
 * PARAM: tag - used to compare lines in set
 * RETURN: "hit" if match found
 *         "miss" if match not found
 */
string L3Set::checkHit(string tag)
{
	L3Line* ret = lruTree->getLine(tag);
	if (ret != NULL)
	{   // match found
		return "hit";
	}
	else if (ret == NULL)
	{   // tag not found
		return "miss";
	}
	else
	{   // an error must have occurred
		return "miss";  
	}
}

/* LOOK AT FUNCTION BELOW */
/**
 * DESC: change mesif state of a line based on the lines
 *       current state.  First the line must be found
 * PARAM: tag - tag bits used to find matching line
 *        mesifState - state bits
 * RETURN: "done\n" when operation finished
 *         "miss\n" if matching tag not found
 */
string L3Set::mesifStateModifier(string tag, int mesifState)
{
	L3Line* holdVictim = NULL;
	L3Line* ret = lruTree->getLine(tag);

	// if matching tag is found
	if (ret != NULL)
	{
		if ( mesifState == 3 )
		{ // 3 is invalid state.
			holdVictim = lruTree->evictSpecificLine(tag);
			if (holdVictim == NULL)
			{
				if (!SILENT) std::cout << "ERROR - specified line not found\n";
				return "error";
			}
		 
			holdVictim->setMESIF(mesifState); 
			return holdVictim->getTag();// return the address to write back to Memo.
        }
		else if (ret == NULL) // if miss, then it is error.
        {
			return "miss";
        }
    }
	return "miss";  // function should not get to this point
}


/**
 * DESC: evict a line from the set
 * PARAM: evictedLineNum - location of line in set
 * RETURN: return tag of evicted line
 */
/*string L3Set::evict(int evictedLineNum)
{
  string s = l3Line[evictedLineNum].getTag();
  l3Line[evictedLineNum].setTag(0);
  l3Line[evictedLineNum].setMESIF(3);
  l3Line[evictedLineNum].setDirtyBit(0);
  return s;
}
*/


