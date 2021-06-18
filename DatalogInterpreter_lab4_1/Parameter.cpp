/*

Elias Sims, Section 1, elisims@byu.edu
Lab 2: Parser

*/

#include <string>

#include "Parameter.h"
#include "Token.h"

int Parameter::getLineNum() {
	return token.getLine();
}

string Parameter::getIdentifier() {
	return token.getIdentifier();
}

string Parameter::getContent() {
	return token.getContent();
}

string Parameter::toString() {
	return token.getContent();
}