/*

Elias Sims, Section 1, elisims@byu.edu
Lab 3: Relational Database

*/

#include <vector>
#include <map>
#include <string>
#include <ostream>
#include <fstream>
#include <iostream>

#include "Database.h"

using namespace std;

Database::Database(DataLog dataLog) {
	vector<Predicate> schemes = dataLog.getSchemes();

	for (unsigned int i = 0; i < schemes.size(); i++) {
		string id = schemes[i].getIDContent();
		vector<Parameter> parameters = schemes[i].getParamList();
		Scheme scheme;

		for (unsigned int j = 0; j < parameters.size(); j++) {
			scheme.push_back(parameters[j].getContent());
		}

		relations.emplace(id, Relation(id, scheme));
	}
	
	vector<Predicate> facts = dataLog.getFacts();

	for (unsigned int i = 0; i < facts.size(); i++) {
		string name = facts[i].getIDContent();
		vector<Parameter> parameters = facts[i].getParamList();
		Tuple tuple;
		for (unsigned int j = 0; j < parameters.size(); j++) {
			tuple.push_back(parameters[j].getContent());
		}

		relations.at(name).addTuple(tuple);
	}

	queries = dataLog.getQueries();
	analyzeQueries();
}

void Database::analyzeQueries() {
	for (unsigned int i = 0; i < queries.size(); i++) {
		ss->str("");

		/* Creates a relation utilizing tuples matching the query */
		Relation results = relations.at(queries[i].getIDContent());

		/* Creates scheme for the query */
		vector<Parameter> parameters = queries[i].getParamList();
		Scheme scheme;

		for (Parameter param : parameters) {
			scheme.push_back(param.getContent());
		}

		cout << queries.at(i).toString() << "? ";

		/* Initiates Selection */
		select(results, scheme);

		/* Initiates Projection */
		project(results, scheme);

		/* Initiates Rename */
		rename(results, scheme);

		/* Gathers results to be output */
		*ss << results.toString();

		if (results.getTuple().size() > 0) {
			cout << "Yes(" << results.getTuple().size() << ')' << endl;
			cout << ss->str() << endl;
		}
		
		else {
			cout << "No" << endl << endl;
		}

		ss->clear();
	}
}

/* Selection Algorithm */
void Database::select(Relation &relation, Scheme &query) {
	set<Tuple> tuples = relation.getTuple();
	set<Tuple> selection;

	for (Tuple tuple : tuples) {
		bool usable = true;
		map<string, string> usedVars;

		for (unsigned int i = 0; i < query.size(); i++) {

			/* Handles if no match is found */
			if (query.at(i).at(0) == '\'' && query.at(i) != tuple.at(i)) {
				usable = false;
				break;
			} 
			
			else if (query.at(i).at(0) != '\'') {
				string var = "";

				/* Iterates through variables, checks if used before */
				try {
					var = usedVars.at(query.at(i));
				} 
				
				/* If the variable is not found in the map, insert */
				catch (out_of_range &error) {
					usedVars.emplace(query.at(i), tuple.at(i));
					var = tuple.at(i);
				}

				/* Handles if repeat is found */
				if (var != tuple.at(i)) {
					usable = false;
					break;
				}
			}
		}

		/* Handles if match is found, creates new set */
		if (usable == true) {
			selection.insert(tuple);
		}
	}

	relation.setTuples(selection);
}

/* Projection Algorithm */
void Database::project(Relation &relation, Scheme &query) {
	Scheme scheme = relation.getScheme();
	set<Tuple> tuples = relation.getTuple();
	set<Tuple> newTuples;

	for (Tuple tuple : tuples) {
		set<string> usedVars;
		Tuple newTuple;

		for (unsigned int i = 0; i < query.size(); i++) {
			/* If project successful, copy */
			if (query.at(i).at(0) != '\'' && usedVars.find(query.at(i)) == usedVars.end()) {
				usedVars.insert(query.at(i));
				newTuple.push_back(tuple.at(i));
			}
		}

		newTuples.insert(newTuple);
	}

	for (unsigned int i = 0; i < query.size(); i++) {
		set<string> usedVars;

		if (query.at(i).at(0) == '\'' && (usedVars.find(query.at(i)) == usedVars.end())) {
			usedVars.insert(query.at(i));
			query.erase(query.begin() + i);
			scheme.erase(scheme.begin() + i);
			i--;
		}
	}

	relation.setScheme(scheme);
	relation.setTuples(newTuples);
}

void Database::rename(Relation &relation, Scheme &query) {
	Scheme scheme = relation.getScheme();

	for (unsigned int i = 0; i < query.size(); i++) {
		if (query[i] != scheme[i]) {
			scheme[i] = query[i];
		}
	}

	relation.setScheme(scheme);
}
