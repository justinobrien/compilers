/*************************************
	Ryan Kennell
	CST 229: Lexical Analyzer
	Symbol.h
	Fall 2015
**************************************/

#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <vector>


class Token{
public:
	Token(std::string Symbol, std::string identifier){ value = Symbol, ID = identifier, scope = 0;}

	void setValue(std::string svalue){value = svalue;}
	void setID(std::string svalue){ID = svalue;}
	void setScope(int svalue){ scope = svalue; }
	std::string value;
	std::string ID;
	int scope;


};

#endif