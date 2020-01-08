/*

Elias Sims, Section 1, elisims@byu.edu
Lab 1: Lexical Analyzer

*/

#include <string>
#include <fstream>
#include <iostream>

#include "Token.h"
#include "Lexer.h"

using namespace std;

int main(int argc, const char* argv[]) {

	Lexer input(argv[1]);

	vector<Token> automata = input.generateTokens();

	for (unsigned int i = 0; i < automata.size(); i++) {
		cout << automata[i].toString();
	}

	cout << "Total Tokens = " << automata.size() << endl;

	return 0;
}