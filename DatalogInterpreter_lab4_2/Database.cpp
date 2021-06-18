/*

Elias Sims, Section 1, elisims@byu.edu
Lab 3: Relational Database &
Lab 4: Datalog Interpreter

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
	vector<Scheme> schemes = createScheme(dataLog.getSchemes());

	for (Scheme scheme : schemes) {
		string name = scheme.getIdentifier();
		relations.emplace(name, Relation(name, scheme));
	}

	vector<Tuple> tuples = createTuple(dataLog.getFacts());

	for (Tuple tuple : tuples) {
		string id = tuple.getIdentifier();
		relations.at(id).addTuple(tuple);
	}

	rules = dataLog.getRules();
	
	unsigned int i;

	for (i = 0; !usable; i++) {
		analyzeRules();
	}

	/* Report how many iterations used to populate schemes */
	cout << "Schemes populated after " << i << " passes through the Rules." << endl;

	queries = dataLog.getQueries();
	analyzeQueries();
}

void Database::analyzeRules() {
	usable = true;

	for (Rule rule : rules) {
		Scheme result = createScheme(rule.getHeadPredicate());
		vector<Scheme> premises = createScheme(rule.getPredicateList());
		vector<Relation> relations;

		for (Scheme premise : premises) {
			Relation relation = this->relations.at(premise.getIdentifier());
			select(relation, premise);
			relations.push_back(relation);
		}

		Relation newRelation;

		/* Handle of final iteration */
		if (premises.size() == 1) {
			newRelation = relations.at(0);
			rename(newRelation, premises.at(0));
			select(newRelation, premises.at(0));
			projectJoined(newRelation, result);
			Scheme resultScheme = this->relations.at(result.getIdentifier()).getScheme();
			rename(newRelation, resultScheme);
		}

		else {
			while (premises.size() > 1) {
				Relation alphaRelation = relations.at(0);
				Relation betaRelation = relations.at(1);
				Scheme alphaScheme = premises.at(0);
				Scheme betaScheme = premises.at(1);

				/* Clean up */
				premises.erase(premises.begin());
				premises.erase(premises.begin());
				relations.erase(relations.begin());
				relations.erase(relations.begin());

				/* Set relations up for Join */
				alphaRelation.setScheme(alphaScheme);
				betaRelation.setScheme(betaScheme);

				/* Attempt to Join relations */
				Relation naturalJoin = joinAlgorithm(alphaRelation, betaRelation);
				relations.push_back(naturalJoin);
				premises.push_back(naturalJoin.getScheme());
			}

			Scheme newScheme = this->relations.at(result.getIdentifier()).getScheme();
			newRelation = relations.at(0);
			select(newRelation, result);
			projectJoined(newRelation, result);
			rename(newRelation, result);
			newRelation.setScheme(newScheme);
		}

		Relation ogRelation = this->relations.at(newRelation.getIdentifier());
		newRelation = getOriginal(newRelation, ogRelation);

		if (newRelation.getTuple().size() != 0) {
			usable = false;
		}

		this->relations.erase(newRelation.getIdentifier());
		this->relations.emplace(newRelation.getIdentifier(), unionAlgorithm(newRelation, ogRelation));
	}
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
			cout << ss->str();
		}
		
		else {
			cout << "No" << endl;
		}

		ss->clear();
	}
}

Relation Database::getOriginal(Relation alphaRelation, Relation betaRelation) {
	set<Tuple> ogTuples;

	if (alphaRelation.getIdentifier() == betaRelation.getIdentifier()) {
		set<Tuple> newTuples = alphaRelation.getTuple();
		set<Tuple> oldTuples = betaRelation.getTuple();

		for (Tuple tuple : newTuples) {
			if (oldTuples.find(tuple) == oldTuples.end()) {
				ogTuples.insert(tuple);
			}
		}
	}

	else {
		throw logic_error("*Relation match error*");
	}

	alphaRelation.setTuples(ogTuples);
	return alphaRelation;
}

Scheme Database::createScheme(Predicate predicate) {
	Scheme scheme;
	scheme.setIdentifier(predicate.getIDContent());
	
	for (Parameter param : predicate.getParamList()) {
		scheme.push_back(param.getContent());
	}

	return scheme;
}

vector<Scheme> Database::createScheme(vector<Predicate> predicates) {
	vector<Scheme> schemes;

	for (Predicate predicate : predicates) {
		schemes.push_back(createScheme(predicate));
	}

	return schemes;
}

Tuple Database::createTuple(Predicate predicate) {
	Tuple tuple;
	tuple.setIdentifier(predicate.getIDContent());

	for (Parameter param : predicate.getParamList()) {
		tuple.push_back(param.getContent());
	}

	return tuple;
}

vector<Tuple> Database::createTuple(vector<Predicate> predicates) {
	vector<Tuple> tuples;

	for (Predicate predicate : predicates) {
		tuples.push_back(createTuple(predicate));
	}

	return tuples;
}

vector<Relation> Database::getRelations(vector<Scheme> schemes) {
	vector<Relation> relations;

	for (Scheme scheme : schemes) {
		relations.push_back(this->relations.at(scheme.getIdentifier()));
	}

	return relations;
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

	if (query.size() == scheme.size()) {
		for (unsigned int i = 0; i < query.size(); i++) {
			if (query[i] != scheme[i]) {
				scheme[i] = query[i];
			}
		}
	}

	relation.setScheme(scheme);
}

Relation Database::joinAlgorithm(Relation alphaRelation, Relation betaRelation) {
	/* Initiate the needed data structures to attempt a Join */
	set<Tuple> tuplesResult;
	set<Tuple> alphaTuples = alphaRelation.getTuple();
	set<Tuple> betaTuples = betaRelation.getTuple();
	Scheme schemeResult;
	Scheme alphaScheme = alphaRelation.getScheme();
	Scheme betaScheme = betaRelation.getScheme();

	/* Attempt to Join the given relations */
	for (Tuple alphaTuple : alphaTuples) {
		for (Tuple betaTuple : betaTuples) {
			map<string, string> usedVars;
			Tuple newTuple;
			Scheme newScheme;
			bool usable = true;

			for (unsigned int i = 0; i < alphaScheme.size(); i++) {
				usedVars.emplace(alphaScheme.at(i), alphaTuple.at(i));
				newTuple.push_back(alphaTuple.at(i));
				newScheme.push_back(alphaScheme.at(i));
			}

			for (unsigned int i = 0; i < betaScheme.size(); i++) {
				string var = "";

				/* Check if used before */
				try {
					var = usedVars.at(betaScheme.at(i));
				}

				/* Search map, if not found- Insert */
				catch (out_of_range & error) {
					usedVars.emplace(betaScheme.at(i), betaTuple.at(i));
					newTuple.push_back(betaTuple.at(i));
					newScheme.push_back(betaScheme.at(i));
					var = betaTuple.at(i);
				}

				if (var != betaTuple.at(i)) {
					usable = false;
				}
			}

			/* If the Joined Tuple is usable, add to set */
			if (usable) {
				schemeResult = newScheme;
				tuplesResult.insert(newTuple);
				schemeResult = newScheme;
			}
		}
	}

	Relation relationResult = Relation("", schemeResult);
	relationResult.setTuples(tuplesResult);
	return relationResult;
}

Relation Database::unionAlgorithm(Relation alphaRelation, Relation betaRelation) {
	set<Tuple> allTuples;

	if (alphaRelation.getIdentifier() == betaRelation.getIdentifier()) {
		allTuples = alphaRelation.getTuple();
		set<Tuple> tempTuples = betaRelation.getTuple();
		allTuples.insert(tempTuples.begin(), tempTuples.end());
	}

	else {
		throw logic_error("*Relation match error*");
	}

	alphaRelation.setTuples(allTuples);
	return alphaRelation;
}

void Database::projectJoined(Relation& relation, Scheme& query) {
	string id = query.getIdentifier();

	Scheme scheme = relation.getScheme();
	Scheme newScheme;
	newScheme.setIdentifier(id);

	set<Tuple> tuples = relation.getTuple();
	set<Tuple> newTuples;

	vector<int> replacementList;

	for (string var : query) {
		if (var[0] != '\'') {
			for (unsigned int i = 0; i < scheme.size(); i++) {
				if (var == scheme.at(i)) {
					replacementList.push_back(i);
					newScheme.push_back(scheme.at(i));
				}
			}
		}
	}

	for (Tuple tuple : tuples) {
		Tuple newTuple;
		newTuple.setIdentifier(query.getIdentifier());

		for (int position : replacementList) {
			newTuple.push_back(tuple.at(position));
		}

		newTuple.setIdentifier(id);
		newTuples.insert(newTuple);
	}

	relation.setIdentifier(id);
	relation.setScheme(newScheme);
	relation.setTuples(newTuples);
}
