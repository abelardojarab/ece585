/******************************************************************************
* AUTHORS: Sean Hendrickson, Khalid ALkhulayfi, Meng Lei
* FILE: LruTree.cpp
* LAST MODIFIED: 12/14/2014
* DESCRIPTION: This file contains the class definition for the CLruTree class.
*              This class defines a BTree to manage a pseudoLRU algorithm.
*****************************************************************************/

#include "LruTree.h"


CLruTree::CLruTree(int numLines)
{
	root = NULL;
	nodesInTree = 0;
	initializeTree(numLines);
}

CLruTree::~CLruTree()
{
	deleteTree(root);
	root = NULL;
	nodesInTree = 0;
}

// find if a line with a matching tag is in the set
// RET: 0 if match found
//     -1 if no match found
int CLruTree::getLine(std::string tag)
{
	return findLine(tag, root);
}

// Add a line to the set
// RET: 0 if line added
//     -1 if no room in cache
int CLruTree::setLine(L3Line* newLine)
{
	return addLine(root, newLine);
}

// adds a newLine to the cache by evicting LRU
// RET: pointer to evicted line
//      NULL if failed
L3Line* CLruTree::evictLine(L3Line* newLine)
{
	L3Line* holdVictim = NULL;
	return evict(root, newLine, holdVictim);
}

// Print entire tree
void CLruTree::printTree()
{
	return printTree(root);
}

// Print only the lines
void CLruTree::printSet()
{
	return printSet(root);
}

// search tree for a line with matching tag
// RETURN: 0 if match found, else -1 if no match found
int CLruTree::findLine(std::string tag, BTreeNode* current)
{
	// make sure current not null
	if (current == NULL)
	{
		return -1;
	}

	// check if node is a leaf
	if (current->leaf == false)
	{
		// check if current node is leaf
		if ((current->leaf) == false)
		{   // continue navigating tree
			if (findLine(tag, current->right) == 0)
			{   // MRU found on right, set bit left=0
				current->bit = 0;
				return 0;
			}
			else if (findLine(tag, current->left) == 0)
			{   // MRU found on left, set bit right=1
				current->bit = 1;
				return 0;
			}
			else
			{   // no match found on left or right sides
				return -1;
			}
		}
	}
	// node is a leaf
	else
	{
		if (current->line->getTag() == tag)
		{   // line found
			return 0;
		}
		else
		{   // line not found
			return -1;
		}
	}
	return -1;
}

// this function creates the tree
int CLruTree::initializeTree(int numLines)
{
	int numNodes = (numLines - 1) + numLines;  // nodes to create in tree
	// remember lines are nodes as well
	int maxLevels = 0;                         // levels of tree
	int currentLevel = 1;

	// find number of levels
	maxLevels = static_cast<int>(ceil((log2(static_cast<double>(numLines)))));

	for (int i = 0; i < numNodes; ++i)
	{
		// create and initialize new node
		BTreeNode* temp = new BTreeNode;
		temp->bit = 0;
		temp->leaf = false;
		temp->level = 0;
		temp->right = NULL;
		temp->left = NULL;
		temp->line = NULL;

		// check if tree is empty
		if (root == NULL)
		{
			root = temp;
			temp->level = 1;
			++nodesInTree;
		}
		else
		{   // tree not empty
			if (insert(root, temp, currentLevel, maxLevels) < 0)
			{   // no room left so de-allocate memory and return
				delete temp;
				if (!SILENT) std::cout << "No more space in tree\n";
				return -1;
			}
			else
			{   // node added to tree
				++nodesInTree;
			}
		}
	}
	return 0;
}

// create a full tree based on the number of lines passed
// The number of nodes in the tree needs to be n - 1, where
// n is the number of lines passed
// return 0 if inserted or -1 if no space available
int CLruTree::insert(BTreeNode* current, BTreeNode* newNode,
	int currentLevel, int maxLevel)
{
	// not on the last available level
	if (currentLevel != maxLevel)
	{
		if (current->left == NULL)
		{   // space on left, add new node to next level
			current->left = newNode;
			newNode->level = current->level + 1;
			return 0;
		}
		else if (current->right == NULL)
		{   // space on right, add new node to next level
			current->right = newNode;
			newNode->level = current->level + 1;
			return 0;
		}

		else
		{   // move to next level since no available space
			if ((insert(current->left, newNode, currentLevel + 1, maxLevel) >= 0)
				|| (insert(current->right, newNode, currentLevel + 1, maxLevel) >= 0))
			{   // one of the paths inserted so return success
				return 0;
			}
		}
	}
	else
	{   // on the last level
		if (current->right == NULL)
		{   // space on right, add new node to next level
			current->right = newNode;
			newNode->level = current->level + 1;
			newNode->leaf = true;
			return 0;
		}
		else if (current->left == NULL)
		{   // space on left, add new node to next level
			current->left = newNode;
			newNode->level = current->level + 1;
			newNode->leaf = true;
			return 0;
		}
		else
		{   // no space here so return
			return -1;
		}
	}
	return -1;
}

// remove all nodes in tree
void CLruTree::deleteTree(BTreeNode* current)
{
	// check if tree is empty
	if (current != NULL)
	{   // traverse the tree until an end is reached
		if (current->left != NULL)
		{
			deleteTree(current->left);
		}
		if (current->right != NULL)
		{
			deleteTree(current->right);
		}

		// delete current node since left and right already held
		current->right = NULL;
		current->left = NULL;
		current->bit = 0;
		current->level = 0;
		current->leaf = false;
		if (current->line != NULL)
		{
			delete current->line;
			current->line = NULL;
		}
		else
		{
			current->line = NULL;
		}

		return;
	}
	else
	{   // reached end of path so return
		return;
	}
}

/*
// evict a line by replacing it with a new line
int BTree::evict(BTreeNode* current, CLine* newLine, CLine* lineToEvict)
{
// check that current is not null
if (current == NULL)
{
return -1;
}

// navigate tree until leaf is found
if (current->leaf != true)
{
// navigate left if bit is 0
if ((current->bit == 0) && (current->left != NULL))
{
evict(current->left, newLine, lineToEvict);
current->bit = 1;  // flip bit
return 0;
}
// navigate right if bit is 1
else if ((current->bit == 1) && (current->right != NULL))
{
evict(current->right, newLine, lineToEvict);
current->bit = 0;  // flip bit
return 0;
}
else
{
return -1;
}
}
// at the leaf so replace line
else if (current->leaf == true)
{
// replace line in leaf with new line
lineToEvict = current->line;
current->line = newLine;
if (!SILENT) std::cout << "Line was replaced\n";
return 0;
}
else
{   // error occurred getting to this point
return -1;
}
}
*/

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

// add a line by searching for an empty space in the leafs
// RETURNS: 0 if new line placed, or -1 if all space full
int CLruTree::addLine(BTreeNode* current, L3Line* newLine)
{
	if (current == NULL)
	{   // went to far
		return -1;
	}

	// check if node is a leaf
	if (current->leaf == false)
	{
		// continue navigating tree
		if ((addLine(current->right, newLine) == 0) & (current->right != NULL))
		{   // MRU found on right, set bit left=0
			current->bit = 0;
			return 0;
		}
		else if ((addLine(current->left, newLine) == 0) & (current->left != NULL))
		{   // MRU found on left, set bit right=1
			current->bit = 1;
			return 0;
		}
		else
		{   // neither returned 1 so return -1
			return -1;
		}
	}
	// node is a leaf
	else if (current->leaf == true)
	{
		// check if leaf is empty
		if (current->line == NULL)
		{   // space found so place line here
			current->line = newLine;
			return 0;
		}
		else
		{   // no space in this leaf
			return -1;
		}
	}
	return -1;  // error - cannot get to this point
}

// print the contents of all nodes in the tree
void CLruTree::printTree(BTreeNode* current)
{
	// display node if current is not null
	if (current != NULL)
	{
		// navigate right then left
		if (current->right != NULL)
		{
			printTree(current->right);
		}
		if (current->left != NULL)
		{
			printTree(current->left);
		}

		// display current node 
		if (current->leaf == true)
		{
			std::cout << "leaf = true , ";
		}
		else
		{
			std::cout << "leaf = false, ";
		}
		std::cout << "bit = " << current->bit << ", "
			<< "level = " << current->level << ", ";

		// check if line is empty, if not display tag
		if (current->line == NULL)
		{
			std::cout << "tag = EMPTY\n";
		}
		else
		{   // line occupied so display tag
			std::cout << "tag = " << current->line->getTag() << std::endl;
		}

		return;    // finished displaying
	}
	else
	{   // current does equal NULL, so return
		return;
	}

}

// print the lines in the tree
void CLruTree::printSet(BTreeNode* current)
{
	// display node if current is not null
	if (current != NULL)
	{
		if (current->leaf != true)
		{   // not a leaf so move to next nodes
			// navigate right then left
			printSet(current->right);
			printSet(current->left);
		}

		// check if node is a leaf
		else if (current->leaf == true)
		{   // display current node information
			std::cout << "leaf = true , ";
			std::cout << "bit = " << current->bit << ", "
				<< "level = " << current->level << ", ";
			if (current->line == NULL)
			{
				std::cout << "EMPTY" << std::endl;
			}
			else
			{   // line not empty
				std::cout << "TAG = " << current->line->getTag() << std::endl;
			}
		}
		else
		{
			return;    // finished displaying
		}
	}
	else
	{
		return;
	}

}

/* PUBLIC RAPPER FUNCTIONS */
