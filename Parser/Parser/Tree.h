/*************************************
Ryan Kennell
CST 229: Lexical Analyzer
Symbol.h
Fall 2015
**************************************/

#ifndef TREE_H
#define TREE_H

#include <string>
#include <list>
#include <fstream>

class Tree
{
public:
	Tree(){ treeTitle = "root"; }
	Tree(std::string title){ treeTitle = title; }

	void ExportToFile();
	void PrintTree();
	
	void addChild(Tree child){ children.push_back(child); }
	void addChild(std::string title){ children.push_back(Tree(title)); }

	std::string treeTitle;
	std::list<Tree> children;
	void DisplayTree(std::string indent, bool last, int &depth);
	void Export(std::string indent, bool last, std::ofstream&);

};





#endif