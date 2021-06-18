/*

Elias Sims, Section 1, elisims@byu.edu
Lab 2: Parser

*/

#include <vector>
#include <iostream>
#include <stdexcept>
#include <sstream>

#include "Predicate.h"
#include "Parameter.h"
#include "Parser.h"
#include "Token.h"

using namespace std;

bool ISSCHEME = false;
bool ISFACT = false;
bool ISRULE = false;
bool ISQUERY = false;



Parser::Parser(vector<Token> tokens) {
	position = -1;
	this->automata = tokens;
	finalToken = Token();
}

/* Allows for targeting specific positions in Automata Vector */
Token Parser::scan(int holder) {
	unsigned int offset = 1;

	if ((position + holder + offset) >= automata.size()) {
		throw automata.at(position + holder + offset);
	}

	if (automata.at(position + holder + offset).isValid("COMMENT")) {
		return scan(holder + offset);
	}

	return automata.at(position + holder + offset);
}

/* Properly parses and distributes tokens collected utitlizing try/catch logic */
bool Parser::parse() {
	this->automata = automata;

	try {
		checkScheme();
		checkFact();
		checkRule();
		checkQuery();

		if (!scan().isValid("EOF")) {
			throw nextToken();
		}

		nextToken();
	}

	catch (Token & finalToken) {
		this->finalToken = finalToken;
		return false;
	}

	catch (out_of_range & fault) {
		this->finalToken = automata.back();
		return false;
	}

	if (finalToken.isValid("UNDEFINED")) {
		this->finalToken = automata.back();
	}

	return true;
}

Token Parser::currentPosition() {
	return automata.at(position);
}

/* Handles the functionality of moving throughout the vector of tokens obtained */
Token Parser::nextToken() {
	unsigned int offset = 1;

	if (position + offset >= automata.size()) {
		throw automata.at(position);
	}

	if (badExpression == true) {
		badExpression = false;
		return automata.at(position);
	}

	position++;
	if (automata.at(position).isValid("COMMENT")) {
		return nextToken();
	}

	return automata.at(position);
}

/* Triggers the schemeList() function if the qualifications are met */
void Parser::checkScheme() {
	if (!scan().isValid("SCHEMES")) {
		throw nextToken();
	}

	nextToken();
	if (!scan().isValid("COLON")) {
		throw nextToken();
	}

	nextToken();
	ISSCHEME = true;
	schemeList();
}

/* Triggers the factList() function if the qualifications are met */
void Parser::checkFact() {
	if (!scan().isValid("FACTS")) {
		throw nextToken();
	}

	nextToken();
	if (!scan().isValid("COLON")) {
		throw nextToken();
	}

	nextToken();
	ISSCHEME = false;
	ISFACT = true;
	factList();
}

/* Triggers the ruleList() function if the qualifications are met */
void Parser::checkRule() {
	if (!scan().isValid("RULES")) {
		throw nextToken();
	}

	nextToken();
	if (!scan().isValid("COLON")) {
		throw nextToken();
	}

	nextToken();
	ISFACT = false;
	ISRULE = true;
	ruleList();
}

/* Triggers the queryList() function if the qualifications are met */
void Parser::checkQuery() {
	if (!scan().isValid("QUERIES")) {
		throw nextToken();
	}

	nextToken();
	if (!scan().isValid("COLON")) {
		throw nextToken();
	}

	nextToken();
	ISRULE = false;
	ISQUERY = true;
	queryList();
}

/* schemeList	->	scheme schemeList | lambda */
void Parser::schemeList() {
	scheme();
	if (!scan().isValid("FACTS")) {
		schemeList();
	}

	return;
}

/* scheme   	-> 	ID LEFT_PAREN ID idList RIGHT_PAREN */
void Parser::scheme() {
	Predicate scheme = predicate();
	dataLog.pushScheme(scheme);

	return;
}

/* factList	->	fact factList | lambda */
void Parser::factList() {
	if (!scan().isValid("RULES")) {
		fact();
		factList();
	}

	return;
}

/* fact    	-> 	ID LEFT_PAREN STRING stringList RIGHT_PAREN PERIOD */
void Parser::fact() {
	Predicate fact = predicate();
	if (!scan().isValid("PERIOD")) {
		throw nextToken();
	}

	nextToken();
	dataLog.pushFact(fact);
	return;
}

/* ruleList	->	rule ruleList | lambda */
void Parser::ruleList() {
	if (!scan().isValid("QUERIES")) {
		rule();
		ruleList();
	}

	return;
}

/* rule    	->	headPredicate COLON_DASH predicate predicateList PERIOD */
void Parser::rule() {
	Rule rule(predicate());
	if (!scan().isValid("COLON_DASH")) {
		throw nextToken();
	}

	nextToken();
	vector<Predicate> ruleList;
	predicateList(ruleList);
	rule.setPredicateList(ruleList);
	if (!scan().isValid("PERIOD")) {
		throw nextToken();
	}

	nextToken();
	dataLog.pushRule(rule);
	return;
}

/* queryList	->	query queryList | lambda */
void Parser::queryList() {
	query();
	if (!scan().isValid("EOF")) {
		queryList();
	}

	return;
}

/* query	        ->      predicate Q_MARK */
void Parser::query() {
	Predicate query = predicate();
	if (!scan().isValid("Q_MARK")) {
		throw nextToken();
	}

	nextToken();
	dataLog.pushQuery(query);
	return;
}

/* predicateList	->	COMMA predicate predicateList | lambda */
void Parser::predicateList(vector<Predicate> & predicateVector) {
	predicateVector.push_back(predicate());
	if (scan().isValid("COMMA")) {
		nextToken();
		predicateList(predicateVector);
	}
}

/* predicate	->	ID LEFT_PAREN parameter parameterList RIGHT_PAREN */
Predicate Parser::predicate() {
	if (!scan().isValid("ID")) {
		throw nextToken();
	}

	Predicate predicate = Predicate(nextToken());
	if (!scan().isValid("LEFT_PAREN")) {
		throw nextToken();
	}

	nextToken();
	vector<Parameter> paramList;
	parameterList(paramList);
	predicate.setParamList(paramList);
	if (!scan().isValid("RIGHT_PAREN")) {
		throw nextToken();
	}

	nextToken();
	return predicate;
}

/* parameterList	-> 	COMMA parameter parameterList | lambda */
void Parser::parameterList(vector<Parameter> & paramList) {
	paramList.push_back(parameter());
	if (scan().isValid("COMMA")) {
		nextToken();
		parameterList(paramList);
	}

	return;
}

/* parameter	->	STRING | ID | expression */
Parameter Parser::parameter() {
	filterParam();

	if (scan().isValid("LEFT_PAREN")) {
		throw nextToken();
		/* For Lab 3+, no longer need to worry about expressions */
		//return Parameter(expression());
	}

	if (!scan().isValid("STRING") && !scan().isValid("ID")) {
		throw nextToken();
	}

	return Parameter(nextToken());
}

/* expression	-> 	LEFT_PAREN parameter operator parameter RIGHT_PAREN */
//Parameter Parser::expression() {
//	unsigned int leftParenCount = 1;
//	unsigned int rightParenCount = 0;
//	unsigned int paramCount = 0;
//	unsigned int opCount = 0;
//	stringstream ss;
//
//	ss.clear();
//	ss << scan().getContent();
//	nextToken();
//
//	while ((rightParenCount < leftParenCount) && (!scan().isValid("EOF"))) {
//
//		/* Handles errors for right-asssociative expressions ex. (E+C(Y*C)) */
//		if ((paramCount >= 2) && (rightParenCount == 0)) {
//			throw nextToken();
//		}
//
//		if (scan().isValid("LEFT_PAREN")) {
//			ss << scan().getContent();
//			leftParenCount++;
//			nextToken();
//		}
//
//		if (scan().isValid("STRING")) {
//			paramCount++;
//			ss << scan().getContent();
//			nextToken();
//		}
//
//		if (scan().isValid("ID")) {
//			paramCount++;
//			ss << scan().getContent();
//			nextToken();
//			if (checkOperator() == true) {
//				opCount++;
//				ss << scan().getContent();
//				nextToken();
//				if (scan().isValid("ID")) {
//					paramCount++;
//					ss << scan().getContent();
//					nextToken();
//				}
//				else if (scan().isValid("LEFT_PAREN")) {
//					continue;
//				}
//				else {
//					throw nextToken();
//				}
//			}
//			else if (!checkOperator()) {
//				if (opCount < leftParenCount) {
//					throw nextToken();
//				}
//			}
//
//			else if (scan().isValid("RIGHT_PAREN")) {
//				continue;
//			}
//			else {
//				throw nextToken();
//			}
//		}
//
//		if (scan().isValid("RIGHT_PAREN")) {
//			if (rightParenCount < leftParenCount) {
//				ss << scan().getContent();
//				nextToken();
//				rightParenCount++;
//			}
//
//			if (paramCount < rightParenCount) {
//				throw nextToken();
//			}
//
//			if (scan().isValid("COMMA")) {
//				break;
//			}
//
//			if (checkOperator()) {
//				opCount++;
//				ss << scan().getContent();
//				nextToken();
//				if (scan().isValid("ID")) {
//					continue;
//				}
//				if (scan().isValid("LEFT_PAREN")) {
//					continue;
//				}
//				else {
//					throw nextToken();
//				}
//			}
//
//			else if (!checkOperator()) {
//				if (scan().isValid("RIGHT_PAREN")) {
//					continue;
//				}
//				else if (opCount < leftParenCount) {
//					throw nextToken();
//				}
//			}
//
//			else if ((rightParenCount == leftParenCount) && (!scan().isValid("RIGHT_PAREN")) && ((paramCount / 2) == opCount)) {
//				badExpression = true;
//				throw nextToken();
//			}
//		}
//
//		if (opCount != leftParenCount) {
//			if (scan().isValid("LEFT_PAREN")) {
//				continue;
//			}
//			if (scan().isValid("RIGHT_PAREN")) {
//				continue;
//			}
//			else {
//				throw nextToken();
//			}
//		}
//	}
//
//	return Parameter(Token(1, ss.str(), "EXPRESSION"));
//}

/* operator	->	ADD | MULTIPLY */
bool Parser::checkOperator() {
	if (scan().isValid("MULTIPLY")) {
		return true;
	}

	if (scan().isValid("ADD")) {
		return true;
	}
	else {
		return false;
	}
}

void Parser::filterParam() {
	if ((scan().isValid("STRING")) && (ISFACT)) {
		dataLog.pushDomain(scan().getContent());
	}

	if ((((scan().isValid("ID")) && (ISFACT)) || ((scan().isValid("STRING")) && (ISSCHEME)))) {
		throw nextToken();
	}

	return;
}