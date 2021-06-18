/*

Elias Sims, Section 1, elisims@byu.edu
Lab 3: Relational Database &
Lab 4: Datalog Interpreter

*/

#include <sstream>

#include "Scheme.h"

using namespace std;

void Scheme::setIdentifier(string id) {
	this->id = id;
}

string Scheme::getIdentifier() {
	return id;
}

string const Scheme::toString() {
	stringstream ss;
	for (unsigned int i = 0; i < this->size(); i++) {
		ss << this->at(i) << ' ';
	}
	ss << endl;
	return ss.str();
}