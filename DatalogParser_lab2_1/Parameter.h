/*

Elias Sims, Section 1, elisims@byu.edu
Lab 2: Parser

*/

#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <string>

#include "Token.h"

using namespace std;

class Parameter {
public:
	Parameter(Token token) { this->token = token; }
	~Parameter() {};

	int getLineNum();
	string getIdentifier();
	string getContent();
	string toString();

private:
	Token token;
};

#endif /* PARAMETER_H_ */