/*************************************
Ryan Kennell
Lexical.cpp
Winter 2016
**************************************/
#include "Parser.h"


using namespace std;


Parser::Parser(string file)
{
	//SymbolTable symbol();

	ifstream infile(file);

	string symbol;
	string id;
	while (!infile.eof())
	{
		getline(infile, symbol);
		getline(infile, id);
		m_TokenList.push_back(Token(symbol, id));
	}

	vector<string> stat_IdentifierList;
	MaxScope = 0;
	NumSymbols = 0;
	CurrentScope = 0;
	NumIfStatements = 0;
	NumWhileLoops = 0;
	NumForLoops = 0;
	NumInOutput = 0;
	NumFunctions = 0;
	TotalNumStatements = 0;
	AvgIdLength = 0;


	int avgCount = 0;
	//const char * tempchar;
	
	for (TokenIter iter = m_TokenList.begin(); iter != m_TokenList.end(); ++iter)
	{
		if ((*(iter)).value == "cout" || (*(iter)).value == "cin")
			++NumInOutput;

		if ((*(iter)).ID == "Identifier")
		{
			AvgIdLength += ((*(iter)).value).length();
			++avgCount;
		}

		if ((*(iter)).value == "{")
		{
			++CurrentScope;
			if (CurrentScope > MaxScope)
				MaxScope = CurrentScope;
		}

		if ((*(iter)).value == "}")
			--CurrentScope;


		if (symbols.isOperator(((*(iter)).ID).c_str()) || symbols.isRelational(((*(iter)).ID).c_str()) || symbols.isDelimiter(((*(iter)).ID).c_str()))
			++NumSymbols;
		(*(iter)).setScope(CurrentScope);

	}
	AvgIdLength /= avgCount;

	m_TokenIter = m_TokenList.begin();
}

void StatementTree(Tree parseTree, int &s)
{
	if (parseTree.treeTitle == "<Statement>")
		++s;

	for (auto i = parseTree.children.begin(); i != parseTree.children.end(); i++)
	{
		auto temp = i;
		temp++;
		StatementTree(*i, s);
	}
}

void Parser::DisplayStats()
{
	ParseTree.PrintTree();
	ParseTree.ExportToFile();
	vector<int> v_Statements;
	
	int st = 0;
	int s = 0;

	StatementTree(ParseTree, TotalNumStatements);
	
	//add up v_statements divided by length

	cout << "Total number of symbols in the program: " << NumSymbols << endl;
	cout << "Average length of identifiers in the program: " << AvgIdLength << endl;
	cout << "Total number of statements in program: " << TotalNumStatements << endl;
	cout << "Maximum number of scope levels for identifiers: " << MaxScope << endl;
	cout << "Number of if statements in the program: " << NumIfStatements << endl;
	cout << "Number of for loops in the program: " << NumForLoops << endl;
	cout << "Number of while loops in the program: " << NumWhileLoops << endl;
	cout << "Number of input/output statements in the program: " << NumInOutput << endl;
	cout << "Number of compound statements in the program: " << "TODO" << endl;
	cout << "Number of functions in the program: " << NumFunctions << endl << endl;

	//string quit;
	//cout << "Press any button to display parse tree and export to file: ";
	getchar();
	
}

bool Parser::Parse()
{
	Tree parent;
	TokenIter tIter = m_TokenList.begin();
	if (Program(tIter, parent))
	{
		ParseTree = parent;
		
		return true;
	}
	return false;
}

bool Parser::Program(TokenIter &iter, Tree &parent)
{
	Tree child("<Program>");
	if (Definition(iter, child))
	{
		parent.addChild(child);
		return true;
	}
	return false;
}

bool Parser::Definition(TokenIter &iter, Tree &parent)
{
	Tree child("<Definition>");
	while (iter != m_TokenList.end())
	{
		if (!Function_Definition(iter, child))
			return false;
	}
	parent.addChild(child);	
	return true;
}

bool Parser::Data_Definition_List(TokenIter &iter, Tree &parent)
{
	Tree child("<Data_Definition_List>");
	TokenIter temp = iter;

	if (Data_Definition(temp++, child))
	{
		if ((*(++temp)).ID == ",")
		{
			child.addChild(",");
			if (Data_Definition_List(++temp, child))
			{
				parent.addChild(child);
				iter = temp;
				return true;
			}
		}
	}

	child = Tree("<Data_Definition_List>");
	temp = iter;
	if (Data_Definition(temp, child))
	{
		parent.addChild(child);
		iter = temp;
		return true;
	}



	return false;
}

bool Parser::Data_Definition(TokenIter &iter, Tree &parent)
{
	Tree child("<Data_Definition>");
	TokenIter temp = iter;

	//if (symbols.isDataType((*temp++).ID))
	if (DataType( temp++, child) )
	{
		Tree child2 = child;
		TokenIter temp2 = temp;


		if (Identifier(temp, child))
		{
			if (symbols.isDelimiter((*(++temp)).ID))
			{
				parent.addChild(child);
				iter = temp;
				return true;
			}
		}

		if (Expression_Call(temp2, child2))
		{
			parent.addChild(child2);
			iter = temp2;
			return true;
		}

		
		

		else 
			throw ParseException("Expected expression after datatype declaration");
	}

	return false;
}

bool Parser::Function_Definition(TokenIter &iter, Tree &parent)
{
	Tree child("<Function_Definition>");
	//if ((*iter).ID == "int"  &&  Function_Header(++iter)  &&  Function_Body(iter))
		//return true;
	if (DataType(iter, child) && Function_Header(++iter, child) && Function_Body(iter, child))
	{
		++NumFunctions;
		parent.addChild(child);
		return true;
	}
	else 
		return false;
}

bool Parser::Function_Header(TokenIter &iter, Tree &parent)
{
	Tree child("<Function_Header>");
	if (Identifier(iter, child) && Parameter_List(++iter, child))
	{
		parent.addChild(child);
		return true;
	}
	return false;
}

bool Parser::Parameter_List(TokenIter &iter, Tree &parent)
{
	Tree child("<Parameter_List>");
	TokenIter temp = iter;
	if ((*temp++).ID == "(")
	{
		child.addChild("(");
		iter = temp;
		if ((*(temp)).ID == ")")
		{
			child.addChild(")");
			parent.addChild(child);
			iter = ++temp;
			return true;
		}

		if (Data_Definition_List(temp, child))
		{
			if ((*(temp++)).ID == ")")
			{
				child.addChild(")");
				parent.addChild(child);
				iter = temp;
				return true;
			}

			else
			{
				string except = (*(temp - 2)).value + " " + (*(temp - 1)).value;
				throw ParseException("Expected closing ')' after " + except);
			}
		}

		temp = iter;
		if (Identifier_List(temp, child))
		{
			if ((*(temp++)).ID == ")")
			{
				child.addChild(")");
				parent.addChild(child);
				iter = temp;
				return true;
			}
				
			else
			{
				string except = (*(temp - 2)).value + " " + (*(temp - 1)).value;
				throw ParseException("Expected closing ')' after " + except);
			}
		}

		else
		{
			iter = temp;
			return false;
		}
			
	}
	else
	{
		return false;
		string except = (*(temp-2)).value + " " + (*(temp-1)).value;
		throw ParseException("Expected opening '(' after " + except);
	}

}

bool Parser::Identifier_List(TokenIter &iter, Tree &parent)
{
	Tree child("<Identifier_List>");
	TokenIter temp = iter;
	
	if (Identifier(temp++, child))
	{
		if ((*temp).ID == ")")
		{
			//child.addChild(")");
			parent.addChild(child);
			iter = temp;
			return true;
		}

		if ((*temp++).ID == ",")
		{
			child.addChild(",");
			if (Identifier_List(temp, child))
			{
				parent.addChild(child);
				iter = temp;
				return true;
			}

		}
		else 
			throw ParseException("Expected seperating comma ',' ");
	}

	temp = iter;
	if (Identifier(temp++, Tree()))
	{
		iter = temp;
		return true;
	}
	 
		return false;
}

bool Parser::Function_Body(TokenIter &iter, Tree &parent)
{
	Tree child("<Function_Body>");
	if ((*iter++).ID == "{")
	{
		child.addChild("{");
		if (Statement_List(iter, child))
		{
			if ((*(iter++)).ID == "}")
			{
				child.addChild("}");
				parent.addChild(child);
				return true;
			}
			else
				throw ParseException("Expected closing '}' in function body");
		}
		else return false;
	}
	else throw ParseException("Expected opening '{' in function body");

	false;
}

bool Parser::Statement(TokenIter &iter, Tree &parent)
{
	Tree child("<Statement>");
	TokenIter temp = iter;
	if (Expression_Call(temp, child) && checkSymbol((*temp++).ID, ";", child))
	{
		parent.addChild(child);
		iter = temp;
		//TotalNumStatements += 1;
		return true;
	}

	child = Tree("<Statement>");
	temp = iter;
	if (Data_Definition(temp, child) && checkSymbol((*temp++).ID, ";", child))
	{
		parent.addChild(child);
		iter = temp;
		//TotalNumStatements += 1;
		return true;
	}

	child = Tree("<Statement>");
	temp = iter;
	if (Function_Header(temp, child) && checkSymbol((*temp++).ID, ";", child))
	{
		parent.addChild(child);
		iter = temp;
		//TotalNumStatements += 1;
		return true;
	}
	
	child = Tree("<Statement>");
	temp = iter;
	if (checkSymbol((*temp++).ID, "while", child))
	{
		if (checkSymbol((*temp++).ID, "(", child))
			if (Expression_Call(temp, child))
				if (checkSymbol((*temp++).ID, ")", child))
					if (checkSymbol((*temp++).ID, "{", child))
						if (Statement_List(temp, child))
							if (checkSymbol((*temp++).ID, "}", child))
							{
								parent.addChild(child);
								++NumWhileLoops;
								iter = temp;
								//TotalNumStatements += 1;
								return true;
							}
							else
								throw ParseException("Expected closing '}' in while definition ");
						else
							throw ParseException("Invalid body in while loop ");
					else
						throw ParseException("Expected opening '{' in while definition ");
				else
					throw ParseException("Expected closing ')' in while declaration");
			else
				throw ParseException("Invalid Expression in while declaration ");
		else 
			throw ParseException("Expected opening '(' in while declaration ");
	}

	child = Tree("<Statement>");
	temp = iter;
	if (checkSymbol((*temp++).ID, "if", child))
	{
		if (checkSymbol((*temp++).ID, "(", child))
			if (Expression_Call(temp, child))
				if (checkSymbol((*temp++).ID, ")", child))
					if (checkSymbol((*temp++).ID, "{", child))
						if (Statement_List(temp, child))
							if (checkSymbol((*temp++).ID, "}", child))
							{
								++NumIfStatements;
								TokenIter temp2 = temp;
								if (checkSymbol((*temp2++).ID, "else", child))
								{
									if(Statement_List(temp2, child))
										temp = temp2;
									else
										throw ParseException("Invalid else Expression ");
								}
								parent.addChild(child);
								//TotalNumStatements += 1;
								iter = temp;
								return true;
							}
							else
								throw ParseException("Expected closing '}' in if definition ");
						else
							throw ParseException("Invalid body in if definition ");
					else
						throw ParseException("Expected opening '{' in if definition ");
				else
					throw ParseException("Expected closing ')' in if declaration");
			else
				throw ParseException("Invalid Expression in if declaration ");
		else
			throw ParseException("Expected opening '(' in if declaration ");
	}

	child = Tree("<Statement>");
	temp = iter;
	if (checkSymbol((*temp++).ID, "for", child))
	{
		if (checkSymbol((*temp++).ID, "(", child))
			if (Data_Definition(temp, child) && checkSymbol((*temp++).ID, ";", child) &&
				Expression_Call(temp, child) && checkSymbol((*temp++).ID, ";", child) &&
				Expression_Call(temp, child))
				if (checkSymbol((*temp++).ID, ")", child))
					if (checkSymbol((*temp++).ID, "{", child))
						if (Statement_List(temp, child))
							if (checkSymbol((*temp++).ID, "}", child))
							{
								parent.addChild(child);
								//TotalNumStatements += 1;
								++NumForLoops;
								iter = temp;
								return true;
							}
							else
								throw ParseException("Expected closing '}' in for definition ");
						else
							throw ParseException("Invalid body in for loop ");
					else
						throw ParseException("Expected opening '{' in while definition ");
				else
					throw ParseException("Expected closing ')' in for declaration ");
			else
				throw ParseException("Invalid for delcaration");
		else
			throw ParseException("Expected opening '(' in for declaration ");
	}

	child = Tree("<Statement>");
	temp = iter;
	if (checkSymbol((*temp++).ID, "for", child))
	{
		if (checkSymbol((*temp++).ID, "(", child))
			if (Expression_Call(temp, child) && checkSymbol((*temp++).ID, ";", child) &&
				Expression_Call(temp, child) && checkSymbol((*temp++).ID, ";", child) &&
				Expression_Call(temp, child))
				if (checkSymbol((*temp++).ID, ")", child))
					if (checkSymbol((*temp++).ID, "{", child))
						if (Statement_List(temp, child))
							if (checkSymbol((*temp++).ID, "}", child))
							{
								parent.addChild(child);
								++NumForLoops;
								//TotalNumStatements += 1;
								iter = temp;
								return true;
							}
							else
								throw ParseException("Expected closing '}' in for definition ");
						else
							throw ParseException("Invalid body in for loop ");
					else
						throw ParseException("Expected opening '{' in while definition ");
				else
					throw ParseException("Expected closing ')' in for declaration ");
			else
				return false;
		else 
			throw ParseException("Expected opening '(' in for declaration ");
	}

	child = Tree("<Statement>");
	temp = iter;
	if (checkSymbol((*temp++).ID, "switch", child))
	{
		if (checkSymbol((*temp++).ID, "(", child))
			if (Expression_Call(temp, child))
				if (checkSymbol((*temp++).ID, ")", child))
					if (checkSymbol((*temp++).ID, "{", child))
					{
						if (Statement_List(temp, child))
							if (checkSymbol((*temp++).ID, "}", child))
							{
								parent.addChild(child);
								//TotalNumStatements += 1;
								iter = temp;
								return true;
							}
							else
								throw ParseException("Expected closing '}' in switch definition ");
						else
							throw ParseException("Invalid body in switch loop ");
					}
					else
						throw ParseException("Expected opening '{' in switch definition ");
				else
					throw ParseException("Expected closing ')' in switch declaration");
			else
				throw ParseException("Invalid Expression in switch declaration ");
		else
			throw ParseException("Expected opening '(' in switch declaration ");
	}

	child = Tree("<Statement>");
	temp = iter;
	if (checkSymbol((*temp++).ID, "case", child))
		if (Constant(temp++, child))
			if (checkSymbol((*temp++).ID, ":", child))
				if (Statement_List(temp, child))
					{
						parent.addChild(child);
						//TotalNumStatements += 1;
						iter = temp;
						return true;
					}

	child = Tree("<Statement>");
	temp = iter;
	if (checkSymbol((*temp++).ID, "break", child))
	{
		if (checkSymbol((*temp++).ID, ";", child))
		{
			parent.addChild(child);
			//TotalNumStatements += 1;
			iter = temp;
			return true;
		}
		else
			throw ParseException("Expected delimiter ';' after 'break' ");
		
	}

	child = Tree("<Statement>");
	temp = iter;
	if (checkSymbol((*temp++).ID, "continue", child))
	{
		if (checkSymbol((*temp++).ID, ";", child))
		{
			parent.addChild(child);
			TotalNumStatements += 1;
			iter = temp;
			return true;
		}
		else
			throw ParseException("Expected delimiter ';' after 'continue' ");
	}

	child = Tree("<Statement>");
	temp = iter;
	if (checkSymbol((*temp++).ID, "return", child))
	{
		if (Expression_Call(temp, child))
		{
			if (checkSymbol((*temp++).ID, ";", child))
			{
				parent.addChild(child);
				//TotalNumStatements += 1;
				iter = temp;
				return true;
			}
			else
				throw ParseException("Expected delimiter ';' after 'return' ");
		}
		else
			throw ParseException("Invalid expression after 'return' ");
	}

	return false;
}

bool Parser::Statement_List(TokenIter &iter, Tree &parent)
{
	// add stuff here for avg num of statements
	Tree child("<Statement_List>");
	TokenIter temp = iter;
	if (Statement(temp, child) && Statement_List(temp, child))
	{
		parent.addChild(child);
		iter = temp;
		return true;
	}

	child = Tree("<Statement_List>");
	temp = iter;
	if (Statement(temp, child))
	{
		parent.addChild(child);
		iter = temp;
		return true;
	}
	
	return false;
}

bool Parser::Expression_Call(TokenIter &iter, Tree &parent)
{
	Tree child("<Expression_Call>");
	TokenIter temp = iter;
	if (Unary_Expression(temp, child) )
	{
		if (Expression(temp, child))
		{
			parent.addChild(child);
			iter = temp;
			return true;
		}

	}

	child = Tree("<Expression_Call>");
	temp = iter;
	if (Unary_Expression(temp, child))
	{
		parent.addChild(child);
		iter = temp;
		return true;
	}
	return false;
}

bool Parser::Expression(TokenIter &iter, Tree &parent)
{
	//Tree child("<Expression>");
	if ((*iter).ID == ";" || (*iter).ID == ")")
	{
		//child.addChild((*iter).ID);
		//parent.addChild(child);
		return true;
	}

	Tree child("<Expression>");
	TokenIter temp = iter;

	if (isRelational((*temp++).ID, child))
	{
		if (Expression_Call(temp, child))
		{
			parent.addChild(child);
			iter = temp;
			return true;
		}
		
	}

	child = Tree("<Expression>");
	temp = iter;
	if (isOperator((*temp++).ID, child))
	{
		if (Expression_Call(temp, child))
		{
			parent.addChild(child);
			iter = temp;
			return true;
		}
	}

	child = Tree("<Expression>");
	temp = iter;
	if (Unary_Expression(temp, child))
	{
		parent.addChild(child);
		iter = temp;
		return true;
	}

	return false;
}

bool Parser::Unary_Expression(TokenIter &iter, Tree &parent)
{
	Tree child("<Unary_Expression>");
	TokenIter temp = iter;
	if ((*temp++).ID == "--" )
	{
		child.addChild("--");
		if (Identifier(temp++, child))
		{
			parent.addChild(child);
			iter = temp;
			return true;
		}
	}

	child = Tree("<Unary_Expression>");
	temp = iter;
	if ((*temp++).ID == "++")
	{
		child.addChild("++");
		if (Identifier(temp++, child))
		{
			parent.addChild(child);
			iter = temp;
			return true;
		}
	}

	child = Tree("<Unary_Expression>");
	temp = iter;
	if (Function_Header(temp++, child))
	{
		parent.addChild(child);
		iter = temp;
		return true;
	}

	child = Tree("<Unary_Expression>");
	temp = iter;
	if (Primary(temp++, child))
	{
		parent.addChild(child);
		iter = temp;
		return true;
	}


	return false;
}

bool Parser::Primary(TokenIter &iter, Tree &parent)
{
	Tree child("<Primary>");
	if (Identifier(iter, child))
	{
		parent.addChild(child);
		return true;
	}
	
	child = Tree("<Primary>");
	if (Constant(iter, child))
	{
		parent.addChild(child);
		return true;
	}
		
	child = Tree("<Primary>");
	if (symbols.isDataType((*iter).ID))
	{
		Tree grandchild("<" + (*iter).ID + ">");
		grandchild.addChild((*iter).value);
		child.addChild(grandchild);
		parent.addChild(child);
		return true;
	}


	return false;
}

bool Parser::Identifier(TokenIter &iter, Tree &parent)
{
	Tree child("<Identifier>");
	if ((*iter).ID == "Identifier")
	{
		child.addChild((*iter).value);
		parent.addChild(child);
		return true;
	}

	else
		return false;
}

bool Parser::Constant(TokenIter &iter, Tree &parent)
{
	Tree child("<Constant>");
	TokenIter temp = iter;

	if ((*temp++).ID == "Number")
	{
		child.addChild((*(iter)).value);
		parent.addChild(child);
		iter = temp;
		return true;
	}
	return false;
}

bool Parser::DataType(TokenIter &iter, Tree &parent)
{
	Tree child("<DataType>");
	if (symbols.isDataType((*iter).ID))
	{
		Tree child2((*iter).ID);
		child.addChild(child2);
		parent.addChild(child);
		return true;
	}
	return false;
}

bool Parser::checkSymbol(std::string input, std::string symbol, Tree &parent)
{
	if (input == symbol)
	{
		parent.addChild(input);
		return true;
	}
	return false;
}

bool Parser::isRelational(std::string input, Tree &parent)
{
	if (symbols.isRelational(input))
	{
		parent.addChild(input);
		return true;
	}
	return false;
}

bool Parser::isOperator(std::string input, Tree &parent)
{
	if (symbols.isOperator(input))
	{
		parent.addChild(input);
		return true;
	}
	return false;
}