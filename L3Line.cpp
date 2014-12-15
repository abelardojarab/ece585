/******************************************************************************
 * AUTHORS: Sean Hendrickson, Khalid ALkhulayfi
 * FILE: L3Line.cpp
 * LAST MODIFIED: 11/24/2014
 * DESCRIPTION: This file implements the L3Line class.
 *****************************************************************************/

#include "L3Line.h"
#include "LruTree.h"
#include <iostream>
#include <bitset>

using namespace std;

/**
* DESC: constructor to set a lines TAG bits and its current
*       state using MESIF.  The MESIF is 6 bits due to 64 Byte
*       data. The TAG is 58 bits since 64 byte address.
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
* PARAM: tag - the cache lines tag bits
*        mesifState - the state bits for a line
* POST-CONDITION: sets the number of state bits
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
* PARAM: state - changes the mesifState of the line
*/
void L3Line::setMESIF(int state)
{
    mesifState = state;
}

/**
* DESC: Used to return the MESIF state of a line
* RETURN: mesif state of line
*/
int L3Line::getMESIF()
{
    return mesifState;
}

/**
* DESC: set tag bits of line
* PARAM: tag - binary string of tag bits
*/
void L3Line::setTag(string tag)
{
    tagBits = tag;
}

/**
* DESC: Returns the line's TAG bits
* RETURN: binary string of tag bits
*/
string L3Line::getTag()
{
    return tagBits;
}

/**
* DESC: Used to alter the dirty bit
* PARAM: x - changes value of dirty bit
*/
void L3Line::setDirtyBit(int x)
{
    dirtyBit = x;
}

/**
* DESC: Used to return the dirty bit
* RETURN: returns value of dirty bit
*/
int L3Line::getDirtyBit()
{
    return dirtyBit;
}

/**
* DESC: prints changes to the line
*/
void L3Line::print()
{
    cout << "tag = " << tagBits << ", mesif = " << mesifState
        << ", dirty = " << dirtyBit << endl;
}



// evict a line by replacing it with a new line
// RET: pointer to evicted line on success
//      NULL if failed
L3Line* CLruTree::evict(BTreeNode* current, L3Line* newLine, L3Line* holdVictim)
{
  // check that current is not null
  if (current == NULL)
    {
      return NULL;
    }

  // navigate tree until leaf is found
  if (current->leaf != true)
    {
      // navigate left if bit is 0
      if ((current->bit == 0) && (current->left != NULL))
        {
          current->bit = 1;  // flip bit
          return evict(current->left, newLine, holdVictim);
        }
      // navigate right if bit is 1
      else if ((current->bit == 1) && (current->right != NULL))
        {
          current->bit = 0;  // flip bit
          return evict(current->right, newLine, holdVictim);
        }
      else
        {
          return NULL;
        }
    }
  // at the leaf so replace line
  else if (current->leaf == true)
    {
      // replace line in leaf with new line
      holdVictim = current->line;
      current->line = newLine;
      if (!SILENT) std::cout << "Line was replaced\n";
      return holdVictim;
    }
  else
    {   // error occurred getting to this point
      return NULL;
    }
}


// evict a line by without replacing it with a new line, the evicted
// line is the one with the matching tag
// RET: pointer to evicted line on success
//      NULL if failed
L3Line* evict_noReplacement(BTreeNode* current, std::string tag, L3Line* holdVictim)
{
  // make sure current not null
  if (current == NULL)
    {
      return NULL;
    }

  // check if node is a leaf
  if (current->leaf == false)
    {
      // check if current node is leaf
      if ((current->leaf) == false)
        {   // continue navigating tree
          if ((holdVictim = evict_noReplacement(current->right, tag, holdVictim)) != NULL)
            {   // MRU found on right, set bit left=0
              current->bit = 0;
              return holdVictim;
            }
          else if ((holdVictim = evict_noReplacement(current->left, tag, holdVictim)) != NULL)
            {   // MRU found on left, set bit right=1
              current->bit = 1;
              return holdVictim;
            }
          else
            {   // no match found on left or right sides
              return NULL;
            }
        }
    }
  // node is a leaf
  else
    {
      if (current->line->getTag() == tag)
        {   // line found
          holdVictim = current->line;  // hold evicted lnie for return
          current->line = NULL;  // empty the space in the cache
          return holdVictim;
        }
      else
        {   // line not found
          return NULL;
        }
    }
}
