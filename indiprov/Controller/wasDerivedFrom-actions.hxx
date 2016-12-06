#ifndef WASDERIVEDFROM_ACTIONS_HXX
#define WASDERIVEDFROM_ACTIONS_HXX

#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;

#include "database.hxx" // create_database

#include "model/wasDerivedFrom.hxx"
#include "model/wasDerivedFrom-odb.hxx"

#include "model/entity.hxx"
#include "model/entity-odb.hxx"

using namespace std;
using namespace odb::core;

int createWasDerivedFrom(auto_ptr<database>& db, string derivedEntityName, string sourceEntityName) {
	transaction t(db->begin());

	shared_ptr<Entity> derivedEntity(
		db->query_one<Entity>(query<Entity>::name == derivedEntityName));
	if (derivedEntity.get() == 0) {
		cerr << "No such derived entity!";
		t.commit();
		return -1;
	}

	shared_ptr<Entity> sourceEntity(
		db->query_one<Entity>(query<Entity>::name == sourceEntityName));
	if (sourceEntity.get() == 0) {
		cerr << "No such source entity!";
		t.commit();
		return -1;
	}

	WasDerivedFrom wasDerivedFrom(derivedEntity, sourceEntity);
	long id = db->persist(wasDerivedFrom);
	t.commit();
	return id;
}

bool deleteWasDerivedFrom(auto_ptr<database>& db, long id) {
	bool deleted = false;
	transaction t(db->begin());
	auto_ptr<WasDerivedFrom> wasDerivedFrom(
		db->query_one<WasDerivedFrom>(query<WasDerivedFrom>::id == id));
	if (wasDerivedFrom.get() != 0) {
		db->erase<WasDerivedFrom>(*wasDerivedFrom);
		deleted = true;
	}
	t.commit();
	return deleted;
}

#endif // WASDERIVEDFROM_ACTIONS_HXX