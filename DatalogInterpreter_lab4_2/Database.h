/*

Elias Sims, Section 1, elisims@byu.edu
Lab 3: Relational Database &
Lab 4: Datalog Interpreter

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
	vector<Rule> rules;
	vector<Predicate> queries;
	stringstream* ss = new stringstream();
	bool usable = false;

	void analyzeQueries();
	void analyzeRules();
	void select(Relation& relation, Scheme& query);
	void project(Relation& relation, Scheme& query);
	void rename(Relation& relation, Scheme& query);
	void projectJoined(Relation& relation, Scheme& query);

	Scheme createScheme(Predicate predicate);
	Tuple createTuple(Predicate predicate);
	vector <Scheme> createScheme(vector <Predicate> predicates);
	vector <Tuple> createTuple(vector <Predicate> predicates);
	vector <Relation> getRelations(vector <Scheme> schemes);
	Relation getOriginal(Relation alphaRelation, Relation betaRelation);
	Relation joinAlgorithm(Relation alphaRelation, Relation betaRelation);
	Relation unionAlgorithm(Relation alphaRelation, Relation betaRelation);

public:
	Database(DataLog dataLog);
	~Database() { delete ss; }
};

#endif /* DATABASE_H_ */