#ifndef ACTIVITY_ACTIONS_HXX
#define ACTIVITY_ACTIONS_HXX

#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx" // create_database

#include "model/activity.hxx"
#include "model/activity-odb.hxx"

using namespace std;
using namespace odb::core;

int createActivity(auto_ptr<database>& db, string name, unsigned long start, unsigned long end) {
	Activity activity(name, start, end);
	transaction t(db->begin());
	int id = db->persist(activity);
	t.commit();
	return id;
}

#endif ACTIVITY_ACTIONS_HXX