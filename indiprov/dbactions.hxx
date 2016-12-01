#ifndef DBACTIONS_HXX
#define DBACTIONS_HXX

#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx" // create_database

#include "model/agent.hxx"
#include "model/agent-odb.hxx"

using namespace std;
using namespace odb::core;

int createAgent(auto_ptr<database>& db, string name) {
	Agent agent(name);
	transaction t(db->begin());
	int id = db->persist(agent);
	t.commit();
	return id;
}

#endif DBACTIONS_HXX