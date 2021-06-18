/*

Elias Sims, Section 1, elisims@byu.edu
Lab 3: Relational Database

*/

#ifndef DATABASE_H_
#define DATABASE_H_

using namespace std;

#include <string>
#include <map>
#include <vector>
#include <sstream>

#include "DataLog.h"
#include "Predicate.h"
#include "Parameter.h"
#include "Scheme.h"
#include "Relation.h"
#include "Tuple.h"

class Database {
private:
	map<string, Relation> relations;

	vector<Predicate> queries;
	stringstream* ss = new stringstream();

	void analyzeQueries();
	void select(Relation& relation, Scheme& query);
	void project(Relation& relation, Scheme& query);
	void rename(Relation& relation, Scheme& query);

public:
	Database(DataLog dataLog);
	~Database() { delete ss; }
};

#endif /* DATABASE_H_ */