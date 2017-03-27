#ifndef VERTEX_ACTIONS_HXX
#define VERTEX_ACTIONS_HXX

#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx" // create_database

#include "../Model/vertex.hxx"
#include "../Model/vertex-odb.hxx"
#include "../Model/creation_messages.pb.h"

using namespace std;
using namespace odb::core;

long createVertex(auto_ptr<database>& db, vertexType type, string name, unsigned long start, unsigned long end) {
	Vertex vertex(type, name, start, end);
	transaction t(db->begin());
	long id = db->persist(vertex);
	t.commit();
	return id;
}

bool deleteVertex(auto_ptr<database>& db, long id) {
	bool deleted = false;
	transaction t(db->begin());
	auto_ptr<Vertex> vertex(
		db->query_one<Vertex>(query<Vertex>::id == id));
	if (vertex.get() != 0) {
		db->erase<Vertex>(*vertex);
		deleted = true;
	}
	t.commit();
	return deleted;
}

#endif VERTEX_ACTIONS_HXX
