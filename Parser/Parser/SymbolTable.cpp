/*************************************
	Ryan Kennell
	menu.cpp
	Winter 2016
**************************************/

#include <iostream>
#include "SymbolTable.h"
#include <algorithm>



using namespace std;

SymbolTable::SymbolTable()
{
	v_Keywords.push_back("break");
	v_Keywords.push_back("const");
	v_Keywords.push_back("else");
	v_Keywords.push_back("enum");
	v_Keywords.push_back("for");
	v_Keywords.push_back("if");
	v_Keywords.push_back("return");
	v_Keywords.push_back("typedef");
	v_Keywords.push_back("while");
	v_Keywords.push_back("switch");
	v_Keywords.push_back("case");


	v_singleSymbol.push_back('*');
	v_singleSymbol.push_back('%');
	v_singleSymbol.push_back('(');
	v_singleSymbol.push_back(')');
	v_singleSymbol.push_back('{');
	v_singleSymbol.push_back('}');
	v_singleSymbol.push_back('[');
	v_singleSymbol.push_back(']');
	v_singleSymbol.push_back(';');
	v_singleSymbol.push_back(',');

	v_doubleSymbol.push_back('+');
	v_doubleSymbol.push_back('-');
	v_doubleSymbol.push_back('=');
	v_doubleSymbol.push_back(':');
	v_doubleSymbol.push_back('<');
	v_doubleSymbol.push_back('>');
	v_doubleSymbol.push_back('!');
	v_doubleSymbol.push_back('&');
	v_doubleSymbol.push_back('|');
	v_doubleSymbol.push_back('/');

	v_operators.push_back("+");
	v_operators.push_back("-");
	v_operators.push_back("*");
	v_operators.push_back("/");
	v_operators.push_back("++");
	v_operators.push_back("--");
	v_operators.push_back("+=");
	v_operators.push_back("-=");
	v_operators.push_back("%");

	v_delimiter.push_back("(");
	v_delimiter.push_back(")");
	v_delimiter.push_back("[");
	v_delimiter.push_back("]");
	v_delimiter.push_back("{");
	v_delimiter.push_back("}");
	v_delimiter.push_back(";");
	v_delimiter.push_back(",");
	v_delimiter.push_back(":");

	v_relational.push_back("<");
	v_relational.push_back(">");
	v_relational.push_back("=");
	v_relational.push_back("==");
	v_relational.push_back(">=");
	v_relational.push_back("<=");
	v_relational.push_back("!=");
	v_relational.push_back("&&");
	v_relational.push_back("||");
	v_relational.push_back("<<");
	v_relational.push_back(">>");

	v_DataTypes.push_back("int");
	v_DataTypes.push_back("double");
	v_DataTypes.push_back("float");
	v_DataTypes.push_back("char");
	v_DataTypes.push_back("String");
	v_DataTypes.push_back("void");
	v_DataTypes.push_back("bool");
	v_DataTypes.push_back("var");
	v_DataTypes.push_back("long");
	v_DataTypes.push_back("short");
	v_DataTypes.push_back("unsigned");
}


void SymbolTable::addSymbol(Symbol symbol)
{
	SymbolList.push_back(symbol);
}

bool SymbolTable::isKeyword(std::string value)
{
	return find(v_Keywords.begin(), v_Keywords.end(), value) != v_Keywords.end();
}

bool SymbolTable::isOperator(std::string value)
{
	return find(v_operators.begin(), v_operators.end(), value) != v_operators.end();
}

bool SymbolTable::isRelational(std::string value)
{
	return find(v_relational.begin(), v_relational.end(), value) != v_relational.end();
}

bool SymbolTable::isDelimiter(std::string value)
{
	return find(v_delimiter.begin(), v_delimiter.end(), value) != v_delimiter.end();
}

bool SymbolTable::isDoubleSymbol(char value)
{
	return find(v_doubleSymbol.begin(), v_doubleSymbol.end(), value) != v_doubleSymbol.end();
}

bool SymbolTable::isSingleSymbol(char value)
{
	return find(v_singleSymbol.begin(), v_singleSymbol.end(), value) != v_singleSymbol.end();
}

bool SymbolTable::isDataType(std::string value)
{
	return find(v_DataTypes.begin(), v_DataTypes.end(), value) != v_DataTypes.end();
}

list<Symbol> SymbolTable::getSymbolList()
{
	return SymbolList;
}