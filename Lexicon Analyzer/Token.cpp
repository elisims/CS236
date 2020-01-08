/*

Elias Sims, Section 1, elisims@byu.edu
Lab 1: Lexical Analyzer

*/

#include <sstream>

#include "Token.h"

using namespace std;

/*Simple toString() function used to print out the final output in main.cpp*/
string Token::toString() {
	stringstream ss;
	ss << '(' << identifier << ",\"" << content << "\"," << lineNumber << ')' << '\n';
	return ss.str();
}