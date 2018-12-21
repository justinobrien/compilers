/*************************************
Ryan Kennell
Lexical.h
Winter 2016
**************************************/



#define EXPECTING_CLOSING "Expected closing"

#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Token.h"
#include <exception>
#include "SymbolTable.h"
#include "Tree.h"

class Parser
{


	typedef std::vector<Token>::iterator TokenIter;
	typedef std::vector<Token> TokenList;
public:
	Parser(std::string filename);
	void DisplayStats();
	bool Parse();

private:

	bool Program(TokenIter &iter, Tree &parent);
	bool Definition(TokenIter &iter, Tree &parent);
	bool Data_Definition_List(TokenIter &iter, Tree &parent);
	bool Data_Definition(TokenIter &iter, Tree &parent);
	bool Function_Definition(TokenIter &iter, Tree &parent);
	bool Function_Header(TokenIter &iter, Tree &parent);
	bool Parameter_List(TokenIter &iter, Tree &parent);
	bool Identifier_List(TokenIter &iter, Tree &parent);
	bool Function_Body(TokenIter &iter, Tree &parent);
	bool Statement(TokenIter &iter, Tree &parent);
	bool Statement_List(TokenIter &iter, Tree &parent);
	bool Expression_Call(TokenIter &iter, Tree &parent);
	bool Expression(TokenIter &iter, Tree &parent);
	bool Unary_Expression(TokenIter &iter, Tree &parent);
	bool Primary(TokenIter &iter, Tree &parent);
	bool Identifier(TokenIter &iter, Tree &parent);
	bool Constant(TokenIter &iter, Tree &parent);
	bool DataType(TokenIter &iter, Tree &parent);

	bool checkSymbol(std::string input, std::string symbol, Tree &parent);
	bool isRelational(std::string input, Tree &parent);
	bool isOperator(std::string input, Tree &parent);

	TokenList m_TokenList;
	TokenIter m_TokenIter;
	SymbolTable symbols;

	// Stats
	std::vector<std::string> stat_IdentifierList;
	int CurrentScope;
	int MaxScope; // - Maximum number of scope levels for identifiers.
	int AvgIdLength; //  - Average length of identifiers in the program.
	int NumSymbols; // - Total number of symbols in the program.
	int NumIfStatements; // - Number of if statements in the program.
	int NumWhileLoops; // - Number of while loops in the program.
	int NumForLoops;
	int TotalNumStatements; // - Average number of statements for compound statements.
	int NumInOutput; // - Number of input / output statements in the program.
	int NumFunctions; // - Number of functions in the program.
	Tree ParseTree;
		
		
		
		
		


};

struct ParseException: public std::exception
{
	ParseException(){ m_exc = ""; }
	ParseException(std::string exc){ m_exc = exc; }
	const std::string description() const throw()
	{
		return m_exc;
	}
	std::string m_exc;
};

#endif 
