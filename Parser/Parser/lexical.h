/*************************************
	Ryan Kennell
	Lexical.h
	Winter 2016
**************************************/

#ifndef LEXICAL_H
#define LEXICAL_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "SymbolTable.h"
#include "Token.h"

using namespace std;


class Lexical{


	public:

		Lexical(std::string file);
		

		/*******************************
		   Tokenizing Functions
		*******************************/
		std::string Identify(const std::string &value);		// Takes in token and returns ID 
		void Tokenize(string buffer, int index);			// Reads in buffer and calls tokenizing files.
		void AnalyzeTokens();								// Pairs tokens with IDs
		void Display(string fileContents);					// Displays file content
		void exportListToFile();
		
		void singleSymbol(string buffer, int index);		// Tokenizes single symbol read from file eg. ( ) & %
		void doubleSymbol(string buffer, int &index);		// Tokenizes symbol when read char is potentially paired as one value eg ++ <= ==
		void isName(string buffer, int &index);				// Tokenizes entire name when aplha is read
		void isDigits(string buffer, int &index);			// Tokenizes number name when digit is read
		void isDirective(string buffer, int &index);		// Adds #define statement to symbol table when # is read
		void isStringLiteral(string buffer, int &index);	// Tokenizes entire string when " is read
		void isComment(string buffer, int &index);			// Determines if comment or divide when / is read

		/*******************************
		   ID Vector get set Funct.s
		*******************************/
		
		void addTokenPair(string token, string type);		// Pushes TokenPair onto vector
		vector<Token> getvTokenPair();						// returns vTokenPairs (Token-ID Pair)

		void addToken(string vToken);						// Pushes Token onto vector
						
		std::vector<string> getvTokens();
		void replacePreprocessor();


	private:

		std::string fileContents;
		


		// Vectors of ID Classificatios
		std::vector<std::string> v_Keywords; 
		std::vector<char> v_singleSymbol;
		std::vector<char> v_doubleSymbol;
		std::vector<std::string> v_operators;
		std::vector<std::string> v_relational;
		std::vector<std::string> v_delimiter;
		
		// Populates Classifications Vectors
		void initializeTables();

		// Token Vectors
		std::vector<Token> v_TokenPair;
		std::vector<Token> v_PreprocessorSymbols;
		std::vector<std::string> v_Tokens;
		SymbolTable preProcessorSymbols;


};

#endif 


