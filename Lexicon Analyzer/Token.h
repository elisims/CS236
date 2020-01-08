/*

Elias Sims, Section 1, elisims@byu.edu
Lab 1: Lexical Analyzer

*/

#ifndef Token_H_
#define Token_H_

#include <string>
#include <ostream>

using namespace std;

class Token {
private:
	int lineNumber;
	string content;
	string identifier;

public:
	Token(int lineNumber = -1, string content = "", string identifier = "UNDEFINED") {
		this->lineNumber = lineNumber;
		this->content = content;
		this->identifier = identifier;
	}

	~Token() {}

	int getLine() { return lineNumber; }
	string getContent() { return content; }
	string getIdentifier() { return identifier; }

	string toString();
};

#endif
