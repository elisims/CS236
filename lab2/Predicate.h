/*

Elias Sims, Section 1, elisims@byu.edu
Lab 2: Parser

*/

#ifndef PREDICATE_H_
#define PREDICATE_H_

#include <string>
#include <vector>

#include "Token.h"
#include "Parameter.h"

using namespace std;

class Predicate {
public:
	Predicate(Token tempIdentifier = Token()) {
		this->identifier = tempIdentifier;
	}
	~Predicate(){}

	int getIDLine();
	string getIDContent();
	string getIDIdentifier();
	vector<Parameter> getParamList();
	void addParameter(Parameter parameter);
	void setParamList(vector<Parameter> paramList);
	string toString();

private:
	Token identifier;
	vector<Parameter> parameterList;
};

#endif /* PREDICATE_H_ */
