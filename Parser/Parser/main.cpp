/*************************************
Ryan Kennell
main.cpp
Winter 2016
**************************************/

#include <iostream>
#include "Parser.h"
#include "lexical.h"
#include "Token.h"

using namespace std;


void LexicalA(string completePath)
{
	vector<string> vtokens;
	vector<Token> vtokenPair;

	Lexical Lex(completePath);
	vtokens = Lex.getvTokens(); //return all tokenized inputs

	//
	// Display raw Tokenize 
	//
	/*for (vector<string>::iterator iter = vtokens.begin(); iter != vtokens.end(); ++iter)
		cout << "Token: " << (*iter) << endl;*/


	//
	// Replace #define statement definitions
	//
	//cout << endl << "**********Replace #define statements************" << endl << endl;

	Lex.replacePreprocessor();
	vtokens = Lex.getvTokens();
	//for (vector<string>::iterator iter = vtokens.begin(); iter != vtokens.end(); ++iter)
	//	cout << "Token: " << (*iter) << endl;


	//
	// Display Tokens and Type
	//
	//cout << endl << "**********Match Token ID************" << endl << endl;

	Lex.AnalyzeTokens();
	vtokenPair = Lex.getvTokenPair();
	//for (vector<Token>::const_iterator iter = vtokenPair.begin(); iter != vtokenPair.end(); ++iter)
	//	cout << "Token: " << (*iter).value << "\t" << "Type: " << (*iter).ID << endl;


	cout << endl << "************************************" << endl << endl;

	//
	// Continue or Exit
	//
	cout << "Exporting list to text file" << endl << endl;

	Lex.exportListToFile();
}


void ParseFile(string filename)
{
	Parser parser = Parser(filename);
	bool result;


	cout << "Parsing Tokenized File: " << filename << endl << endl;


	try
	{
		result = parser.Parse();
	}
	catch (ParseException e)
	{
		result = false;
		cout << e.description() << endl << endl;
	}


	if (result == true)
	{
		cout << "Parse Sucessful" << endl << endl << endl;
		cout << "************** Program Statistics ****************" << endl << endl;
		parser.DisplayStats(); 
		cout << endl;
	}

	else
		cout << "Parse Failed" << endl << endl << endl;

	
}


int main()
{
	string filename;
	

	cout << "            Recursive Descent Parser              " << endl << endl << endl;

	cout << "************* Valid Demonstration  ***************" << endl << endl << endl;

	cout << "enter filename (test1.txt, test2.txt...): ";
		//cin >> filename;
		cout << endl << endl;


	LexicalA("Input.txt");
	ParseFile("output.txt");
	

	// cout << "************* End of Demonstration ***************" << endl << endl << endl;

	cin.clear();
	cin.get();

	return 0;

}