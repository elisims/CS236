/*

Elias Sims, Section 1, elisims@byu.edu
Lab 3: Relational Database &
Lab 4: Datalog Interpreter

*/

#ifndef TUPLE_H_
#define TUPLE_H_

#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Tuple: public vector<string> {
private:
	string id = "";

public:
	void setIdentifier(string id);
	string getIdentifier();
	string const toString();
};

#endif /* TUPLE_H_ */