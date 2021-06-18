/*

Elias Sims, Section 1, elisims@byu.edu
Lab 3: Relational Database &
Lab 4: Datalog Interpreter

*/

#ifndef SCHEME_H_
#define SCHEME_H_

#include <vector>
#include <string>

using namespace std;

class Scheme: public vector<string> { 
private:
	string id = "";

public:
	void setIdentifier(string id);
	string getIdentifier();
	string const toString();
};

#endif /* SCHEME_H_ */