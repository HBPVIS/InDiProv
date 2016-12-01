#ifndef ENTITY_ACTIONS_HXX
#define ENTITY_ACTIONS_HXX

#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx" // create_database

#include "model/entity.hxx"
#include "model/entity-odb.hxx"

using namespace std;
using namespace odb::core;

string createEntity(auto_ptr<database>& db, string name) {
	Entity entity(name);
	transaction t(db->begin());
	string id = db->persist(entity);
	t.commit();
	return id;
}

bool deleteEntity(auto_ptr<database>& db, string name) {
	bool deleted = false;
	transaction t(db->begin());
	auto_ptr<Entity> entity(
		db->query_one<Entity>(query<Entity>::name == name));
	if (entity.get() != 0) {
		db->erase<Entity>(*entity);
		deleted = true;
	}
	t.commit();
	return deleted;
}

#endif ENTITY_ACTIONS_HXX