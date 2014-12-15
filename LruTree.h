/******************************************************************************
* AUTHORS: Sean Hendrickson, Khalid ALkhulayfi, Meng Lei
* FILE: LruTree.h
* LAST MODIFIED: 12/14/2014
* DESCRIPTION: This file contains the class definition for the CLruTree class.
*              This class defines a BTree to manage a pseudoLRU algorithm.
*****************************************************************************/
#pragma once
#ifndef SILENT
#define SILENT 1
#endif

#include "L3Line.h"
#include <stdlib.h>
#include <cmath>
#include <string>
#include <iostream>


struct BTreeNode
{
	int bit;
	int level;
	BTreeNode *left;
	BTreeNode *right;
	L3Line *line;
	bool leaf;
};

class CLruTree
{
public:
	CLruTree(int numLines);
	~CLruTree();
	
	// find if a line with a matching tag is in the set
	// RET: 0 if match found
	//     -1 if no match found
	int getLine(std::string tag);

	// return the number of nodes + lines in tree
	int getNumNodes(void) { return nodesInTree; }

	// Add a line to the set
	// RET: 0 if line added
	//     -1 if no room in cache
	int setLine(L3Line* newLine);

	// adds a newLine to the cache by evicting LRU
	// RET: pointer to evicted line
	//      NULL if failed
	L3Line* evictLine(L3Line* newLine);

	// Print entire tree
	void printTree();

	// Print only the lines
	void printSet();

private:
	// search tree for a line with matching tag
	// RETURN: 0 if match found, else -1 if no match found
	int findLine(std::string tag, BTreeNode* current);

	// this function creates the tree
	int initializeTree(int numLines);

	// create a full tree based on the number of lines passed
	// The number of nodes in the tree needs to be n - 1, where
	// n is the number of lines passed
	// return 0 if inserted or -1 if no space available 
	int insert(BTreeNode* current, BTreeNode* newNode,
		int currentLevel, int maxLevel);

	// remove all nodes in tree
	void deleteTree(BTreeNode* current);

	// evict a line by replacing it with a new line
	// RET: pointer to evicted line on success
	//      NULL if failed
	L3Line* evict(BTreeNode* current, L3Line* newLine, L3Line* holdVictim);

	// add a line by searching for an empty space in the leafs
	// RETURNS: 0 if new line placed, or -1 if all space full
	int addLine(BTreeNode* current, L3Line* newLine);

	// print the contents of all nodes in the tree
	void printTree(BTreeNode* current);

	// print the lines in the tree
	void printSet(BTreeNode* current);


	BTreeNode* root;
	int nodesInTree;
};

