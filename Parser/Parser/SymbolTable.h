/*************************************
	Ryan Kennell
	SymbolTable.h
	Winter 2016
**************************************/

#include <string>
#include <list>
#include <algorithm>
#include <iostream>
#include <vector>


#ifndef SYMBOL_H
#define SYMBOL_H

class Symbol
{
public:
	Symbol(std::string Symbol, std::string Value){ value = Value, ID = Symbol; }
	std::string ID;
	std::string value;
};


class SymbolTable
{

public:
	SymbolTable();

	// Adds symbol to list
	void addSymbol(Symbol symbol);

	bool isKeyword(std::string value);
	bool isOperator(std::string value);
	bool isRelational(std::string value);
	bool isDelimiter(std::string value);
	bool isDoubleSymbol(char value);
	bool isSingleSymbol(char value);
	bool isDataType(std::string value);
	

	std::list<Symbol> getSymbolList();

private:
	std::list<Symbol> SymbolList;

	std::vector<std::string> v_Keywords;
	std::vector<std::string> v_DataTypes;
	std::vector<char> v_singleSymbol;
	std::vector<char> v_doubleSymbol;
	std::vector<std::string> v_operators;
	std::vector<std::string> v_relational;
	std::vector<std::string> v_delimiter;

};

#endif
