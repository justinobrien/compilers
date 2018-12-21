/*************************************
	Ryan Kennell
	Lexical.cpp
	Winter 2016
**************************************/

#include "Lexical.h"
#define FIRST_CHAR 0

Lexical::Lexical(string file)
{ 
	ifstream t(file);
	
	//copy file into buffer	
	string buffer((istreambuf_iterator<char>(t)),
		istreambuf_iterator<char>());

	buffer += EOF;
	buffer += EOF;

	//Display(buffer); 
	initializeTables();
	Tokenize(buffer, FIRST_CHAR);

	return;
}

void Lexical::Tokenize(string buffer, int index){
	while (buffer.at(index) != EOF){
		char temp = buffer.at(index);
		
		if (isalpha(temp))
			isName(buffer, index);
		
		else if (isdigit(temp))
			isDigits(buffer, index);
		

		else if (temp == '_')
			isName(buffer, index);
		
		//
		// If symbol is only a single symbol eg. * ( ) { } %
		//
		else if (preProcessorSymbols.isSingleSymbol(temp))
			singleSymbol(buffer, index);
		
		//
		// If sysbol has potential to be a double symbol eg. ++ -- >= <= == 
		//
		else if (preProcessorSymbols.isDoubleSymbol(temp)) 
			doubleSymbol(buffer, index);

		//
		// Handles preprocessor statements
		//
		else if (buffer.at(index) == '#') 
			isDirective(buffer, index);

		//
		// String literals
		//
		else if (buffer.at(index) == '"')
			isStringLiteral(buffer, index);

		else if (buffer.at(index) == EOF)
			break;
		 
		++index;
	}

}

void Lexical::exportListToFile()
{
	ofstream outFile;
	outFile.open("Output.txt");

	for (vector<Token>::const_iterator iter = v_TokenPair.begin(); iter != v_TokenPair.end(); ++iter)
	{
		if (iter == v_TokenPair.begin())
			outFile << (*iter).value << endl << (*iter).ID;
		else
			outFile << endl << (*iter).value << endl << (*iter).ID;
	}
		
	outFile.close();

}

string Lexical::Identify(const string &value)
{
	if (preProcessorSymbols.isKeyword(value) || preProcessorSymbols.isDataType(value))
		return value;
	else if(isalpha(value[0]))
		return "Identifier";

	else if(isdigit(value[0]))
		return "Number";

	else if(value[0] == '"')
		return "String";

	else if (preProcessorSymbols.isOperator(value))
		return value;

	else if (preProcessorSymbols.isDelimiter(value))
		return value;

	else if (preProcessorSymbols.isRelational(value))
		return value;

	else 
		return "Unrecognized ID";

}


void Lexical::AnalyzeTokens()
{
	for (vector<string>::iterator iter = v_Tokens.begin(); iter != v_Tokens.end(); ++iter)
	{
		addTokenPair(*iter, Identify(*iter));
	}
}


void Lexical::replacePreprocessor()
{
	list<Symbol> temp = preProcessorSymbols.getSymbolList();

	for (list<Symbol>::iterator symbolIter = temp.begin(); symbolIter != temp.end(); ++symbolIter)
	{
		for (vector<string>::iterator tokenIter = v_Tokens.begin(); tokenIter != v_Tokens.end(); ++tokenIter)
			if( (*tokenIter) == (*symbolIter).ID)
				(*tokenIter) = (*symbolIter).value;

	}
}


void Lexical::addTokenPair(string token, string type)
{
	v_TokenPair.push_back( Token(token, type));
	
}


vector<Token> Lexical::getvTokenPair()
{
	return v_TokenPair;
}


void Lexical::singleSymbol(string buffer, int index)
{
	string tempStr;
	tempStr = buffer.at(index);
	addToken(tempStr);
}


void Lexical::doubleSymbol(string buffer, int &index)
{
	string tempStr = "";
	if(buffer.at(index) == '/' && buffer.at(index+1) == '/')
		isComment(buffer, index);
	
	else if (preProcessorSymbols.isDoubleSymbol(buffer.at(index + 1)))
	{
		tempStr += buffer.at(index);
		tempStr += buffer.at(index+1);
		addToken(tempStr);
	}
	else 
	{
		tempStr = buffer.at(index);
		addToken(tempStr);
	}
	
	++index;

}


void Lexical::isName(string buffer, int &index){
	string tempStr;

	while (isalpha(buffer.at(index)) || buffer.at(index) == '_'){
		tempStr += buffer.at(index);
		++index;
	}
	--index;
	addToken(tempStr);
}


void Lexical::isDigits(string buffer, int &index)
{ 
	string tempStr;

	while (isdigit(buffer.at(index)))
	{
		tempStr += buffer.at(index);
		++index;
	}
	--index;
	addToken(tempStr);

}


void Lexical::isDirective(string buffer, int &index)
{ 
	string tempStr;
	int j = 0;
	string symbol[2];

	while (buffer.at(index) != '\n')
	{
		if(buffer.at(index) == ' ')
		{
			symbol[j] = tempStr;
			++j;
			tempStr = "";
			++index;
		}
		tempStr += buffer.at(index);

		++index;
	}

	preProcessorSymbols.addSymbol( Symbol(symbol[1], tempStr));

}


void Lexical::isStringLiteral(string buffer, int &index)
{
	string tempStr;

	if (buffer.at(index) == '"')
	{
		tempStr += buffer.at(index);
		++index;
		while (buffer.at(index) != '"')
		{
			tempStr += buffer.at(index);
			++index;
		}
		tempStr += '"';
		//index++;

		addToken(tempStr);
	}

}


void Lexical::isComment(string buffer, int &index)
{
	while (buffer.at(index) != '\n')
		++index;
	--index;	
}


void Lexical::Display( string fileContents )
{
	cout << endl << "**************Display File******************" << endl;
	cout << fileContents << endl;
	cout << endl << "******************EOF***********************" << endl << endl;
}


void Lexical::addToken(string vToken)
{
	v_Tokens.push_back(vToken);
}


vector<string> Lexical::getvTokens()
{
	return v_Tokens;
}


void Lexical::initializeTables()
{

}
