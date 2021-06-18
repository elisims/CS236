/*

Elias Sims, Section 1, elisims@byu.edu
Lab 2: Parser

*/

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <sstream>

#include "DataLog.h"
#include "Token.h"
#include "Parameter.h"
#include "Predicate.h"
#include "Rule.h"

using namespace std;

void DataLog::pushScheme(Predicate scheme) {
	schemes.push_back(scheme);
}

void DataLog::pushFact(Predicate fact) {
	facts.push_back(fact);
}

void DataLog::pushRule(Rule rule) {
	rules.push_back(rule);
}

void DataLog::pushQuery(Predicate querie) {
	queries.push_back(querie);
}

void DataLog::pushDomain(string content) {
	domain.insert(content); 
}

vector<Predicate> DataLog::getSchemes() { return schemes; }

vector<Predicate> DataLog::getFacts() { return facts; }

vector<Rule> DataLog::getRules() { return rules; }

vector<Predicate> DataLog::getQueries() { return queries; }

set<string> DataLog::getDomain() { return domain; }

string DataLog::toString() {
	stringstream ss;

	ss << "Schemes(" << schemes.size() << "):\n";
	for (unsigned int i = 0; i < schemes.size(); i++) {
		ss << "  " << schemes[i].toString() << '\n';
	}

	ss << "Facts(" << facts.size() << "):\n";
	for (unsigned int i = 0; i < facts.size(); i++) {
		ss << "  " << facts[i].toString() << "\n";
	}

	ss << "Rules(" << rules.size() << "):\n";
	for (unsigned int i = 0; i < rules.size(); i++) {
		ss << "  " << rules[i].toString();
	}

	ss << "Queries(" << queries.size() << "):\n";
	for (unsigned int i = 0; i < queries.size(); i++) {
		ss << "  " << queries[i].toString() << "\n" ;
	}

	ss << "Domain(" << domain.size() << "):\n";
	for (set<string>::iterator i = domain.begin(); i != domain.end(); i++) {
		ss << "  " << *i << '\n';
	}
	return ss.str();
}