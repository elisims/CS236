/*

Elias Sims, Section 1, elisims@byu.edu
Lab 3: Relational Database

*/

#include <sstream>

#include "Tuple.h"

using namespace std;

/* Simple toString() function */
string const Tuple::toString() {
	stringstream ss;

	for (unsigned int i = 0; i < this->size(); i++) {
		ss << this->at(i) << ' ';
	}

	ss << endl;
	return ss.str();
}