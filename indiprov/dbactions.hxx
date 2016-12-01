#ifndef ACTIVITY_HXX
#define ACTIVITY_HXX

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
	agent ag(name);
	transaction t(db->begin());
	int id = db->persist(ag);
	t.commit();
	return id;
}

#endif