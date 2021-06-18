/*

Elias Sims, Section 1, elisims@byu.edu
Lab 3: Relational Database

*/

#include "Relation.h"

#include <string>
#include <sstream>
#include <iostream>

using namespace std;

Relation::Relation(string id, Scheme scheme) {
	this->id = id;
	this->scheme = scheme;
}

Relation::Relation() {
	this->id = "";
}

string Relation::getIdentifier() {
	return id;
}

Scheme Relation::getScheme() {
	return scheme;
}

set<Tuple> Relation::getTuple() {
	return myTuples;
}

int Relation::getSize() {
	return myTuples.size();
}

void Relation::addTuple(Tuple tuple) {
	myTuples.insert(tuple);
}

void Relation::setTuples(set<Tuple> tuples) {
	this->myTuples = tuples;
}

void Relation::setScheme(Scheme scheme) {
	this->scheme = scheme;
}

string Relation::toString() {
	stringstream ss;
	string holder = "";

	for (Tuple tuple : myTuples) {
		if (tuple.size() > 0) {
			ss << "  ";
		}

		for (unsigned int i = 0; i < tuple.size(); i++) {
			if (holder == scheme[i]) {
				ss << scheme[i + 1] << "=" << tuple.at(i);
				if (i < tuple.size() - 1) {
					ss << ", ";
				}
				holder = scheme[i + 1];
			}
			else {
				ss << scheme[i] << "=" << tuple.at(i);
				if (i < tuple.size() - 1) {
					ss << ", ";
				}
				holder = scheme[i];
			}
			
		}
		if (tuple.size() > 0) {
			ss << endl;
		}
	}

	return ss.str();
}