/*

Elias Sims, Section 1, elisims@byu.edu
Lab 2: Parser

*/

#ifndef DATALOG_H_
#define DATALOG_H_

#include <string>
#include <vector>
#include <set>

#include "Token.h"
#include "Parameter.h"
#include "Predicate.h"
#include "Rule.h"

using namespace std;

class DataLog {
public:
	void pushScheme(Predicate scheme);
	void pushFact(Predicate fact);
	void pushRule(Rule rule);
	void pushQuery(Predicate querie);
	void pushDomain(string content);

	vector<Predicate> getSchemes();
	vector<Predicate> getFacts();
	vector<Rule> getRules();
	vector<Predicate> getQueries();
	set<string> getDomain();
	string toString();

private:
	vector<Predicate> schemes;
	vector<Predicate> facts;
	vector<Rule> rules;
	vector<Predicate> queries;
	set<string> domain;
};

#endif /* DATALOG_H_ */