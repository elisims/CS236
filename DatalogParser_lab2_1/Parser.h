/*

Elias Sims, Section 1, elisims@byu.edu
Lab 2: Parser

*/

#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <vector>

#include "Token.h"
#include "Predicate.h"
#include "DataLog.h"

using namespace std;

class Parser {
public:
	Parser(vector<Token> tokens);
	~Parser() {};
	bool parse();
	Token getLastToken() { return finalToken; };
	DataLog getDataLog() { return dataLog; };

private:
	vector<Token> automata;
	DataLog dataLog;
	Token finalToken;
	Token currentPosition();
	Token nextToken();
	Token scan(int holder = 0);
	int position;
	bool badExpression = false;

	void schemeList();
	void scheme();
	void factList();
	void fact();
	void ruleList();
	void rule();
	void queryList();
	void query();
	void checkScheme();
	void checkFact();
	void checkRule();
	void checkQuery();

	void parameterList(vector<Parameter>& paramList);
	void predicateList(vector<Predicate>& predicateList);
	Parameter parameter();
	Predicate predicate();

};

#endif /* PARSER_H_ */