/*

Elias Sims, Section 1, elisims@byu.edu
Lab 2: Parser

*/

#include <string>
#include <vector>
#include <sstream>

#include "Predicate.h"
#include "Rule.h"

using namespace std;

Predicate Rule::getHeadPredicate() {
	return predicate;
}

vector<Predicate> Rule::getPredicateList() {
	return predicateList;
}

void Rule::setPredicateList(vector<Predicate> predicateList) {
	this->predicateList = predicateList;
}

string Rule::toString() {
	stringstream ss;

	ss << predicate.toString() << " :- ";

	for (unsigned int i = 0; i < predicateList.size(); i++) {
		ss << predicateList[i].toString();
		if (i < predicateList.size() - 1) {
			ss << ',';
		}
	}

	return ss.str();
}