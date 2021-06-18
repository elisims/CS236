/*

Elias Sims, Section 1, elisims@byu.edu
Lab 3: Relational Database &
Lab 4: Datalog Interpreter

*/

#ifndef RELATION_H_
#define RELATION_H_

#include <set>
#include <string>
#include <iostream>

#include "Tuple.h"
#include "Scheme.h"

using namespace std;

class Relation {
private:
	string id;
	Scheme scheme;
	set<Tuple> myTuples;

public:
	Relation(string id, Scheme scheme);
	Relation();

	string getIdentifier();
	Scheme getScheme();
	set<Tuple> getTuple();

	int getSize();
	void addTuple(Tuple tuple);
	void setTuples(set<Tuple> tuples);
	void setIdentifier(string id);
	void setScheme(Scheme scheme);

	string toString();
};

#endif /* RELATION_H_ */