#include "Tree.h"

#include <string>
#include <list>
#include <iostream>
#include <Windows.h>

enum TextColor { DARKBLUE = 1, DARKGREEN, DARKTEAL, DARKRED, DARKPINK, DARKYELLOW, GRAY, DARKGRAY, BLUE, GREEN, TEAL, RED, PINK, YELLOW, WHITE };

void color(TextColor c)
{
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int col = 12; 

		// color your text in Windows console mode
		// colors are 0=black 1=blue 2=green and so on to 15=white  
		// colorattribute = foreground + background * 16
		// to get red text on yellow use 4 + 14*16 = 228
		// light red on yellow would be 12 + 14*16 = 236

	FlushConsoleInputBuffer(hConsole);
	SetConsoleTextAttribute(hConsole, col);

	SetConsoleTextAttribute(hConsole, c);
}

void Tree::DisplayTree(std::string indent, bool last, int &depth)
{

	if (depth == 60)
	{
		std::cout<< std::endl << "Press any key to continue...";
		getchar();
		depth = 0;

	}

	color(DARKTEAL);
	std::cout << indent;
	if (last)
	{
		color(DARKTEAL);
		std::cout << "\\- ";
		indent += "  ";

		if (treeTitle.front() == '<' &&  treeTitle.back() == '>')
			color(DARKGRAY);
		else
			color(WHITE);

		std::cout << treeTitle << std::endl;
		color(WHITE);
		depth++;
	}
	else
	{
		color(DARKTEAL);
		std::cout << "|- ";
		indent += "| ";
		if (treeTitle.front() == '<' &&  treeTitle.back() == '>')
			color(DARKGRAY);
		else
			color(WHITE);

		std::cout << treeTitle << std::endl;
		color(WHITE);
		depth++;
	}

	for (auto i = children.begin(); i != children.end(); i++)
	{
		auto temp = i;
		temp++;
		(*(i)).DisplayTree(indent, temp == children.end(), depth);
	}
}

void Tree::PrintTree()
{
	int i = 0;
	DisplayTree("", true, i);
}

void Tree::ExportToFile()
{
	std::ofstream outFile;
	outFile.open("ParseTree.txt");
	Export("", true, outFile);
	outFile.close();
}

void Tree::Export(std::string indent, bool last, std::ofstream &outfile)
{
	outfile << indent;
	if (last)
	{
		outfile << "\\- ";
		indent += "  ";
		outfile << treeTitle << std::endl;
	}
	else
	{
		outfile << "|- ";
		indent += "| ";
		outfile << treeTitle << std::endl;
	}

	for (auto i = children.begin(); i != children.end(); i++)
	{
		auto temp = i;
		temp++;
		(*(i)).Export(indent, temp == children.end(), outfile);
	}
}