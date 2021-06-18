/*

Elias Sims, Section 1, elisims@byu.edu
Lab 2: Parser

*/

#include <string>
#include <vector>
#include <sstream>

#include "Predicate.h"
#include "Parameter.h"
#include "Token.h"

int Predicate::getIDLine() {
	return identifier.getLine();
}

string Predicate::getIDContent() {
	return identifier.getContent();
}

string Predicate::getIDIdentifier() {
	return identifier.getIdentifier();
}

vector<Parameter> Predicate::getParamList() {
	return parameterList;
}

void Predicate::addParameter(Parameter param) {
	parameterList.push_back(param);
}

void Predicate::setParamList(vector<Parameter> paramList) {
	this->parameterList = paramList;
}

string Predicate::toString() {
	stringstream ss;
	ss << identifier.getContent() << '(';
	for (unsigned int i = 0; i < parameterList.size(); i++) {
		ss << parameterList[i].toString();
		if (i < parameterList.size()-1)
			ss << ',';
	}

	ss << ")";
	return ss.str();
}