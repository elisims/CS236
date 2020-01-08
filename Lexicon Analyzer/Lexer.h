/*

Elias Sims, Section 1, elisims@byu.edu
Lab 1: Lexical Analyzer

*/

#ifndef Lexer_H_
#define Lexer_H_

#include <vector>
#include <string>
#include <fstream>

#include "Token.h"

using namespace std;

class Lexer {
private:
	int curToken;
	ifstream file;
	int lineNum;
	int tempLineNum;
	vector<Token> tokenVec;

public:
	Lexer(string input) {
		file.open(input.c_str());
		lineNum = 1;
		tempLineNum = lineNum;
		curToken = ' ';
	}

	~Lexer() {
		this->file.close();
	}

	Token getToken();
	vector <Token> generateTokens();
	void whiteSpace();
	void colonDash();
	void comment();
	void strings();
	void defaultSort();
	void sortWord(string id);
};

#endif