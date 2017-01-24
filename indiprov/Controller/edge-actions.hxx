#ifndef EDGE_ACTIONS_HXX
#define EDGE_ACTIONS_HXX

#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;

#include "database.hxx" // create_database

#include "model/edge.hxx"
#include "model/edge-odb.hxx"

using namespace std;
using namespace odb::core;

int createEdge(auto_ptr<database>& db, edgeType type, long firstId, long secondId) {
	transaction t(db->begin());

	shared_ptr<Vertex> first(
		db->query_one<Vertex>(query<Vertex>::id == firstId));
	if (first.get() == 0 || first.get()->GetType() != getVertexType(type, true)) {
		cerr << "First vertex invalid!" << endl;
		t.commit();
		return -1;
	}

	shared_ptr<Vertex> second(
		db->query_one<Vertex>(query<Vertex>::id == secondId));
	if (second.get() == 0 || second.get()->GetType() != getVertexType(type, false)) {
		cerr << "Second vertex invalid!" << endl;
		t.commit();
		return -1;
	}

	Edge edge(type, first, second);
	long id = db->persist(edge);
	t.commit();
	return id;
}

bool deleteEdge(auto_ptr<database>& db, long id) {
	bool deleted = false;
	transaction t(db->begin());
	auto_ptr<Edge> edge(
		db->query_one<Edge>(query<Edge>::id == id));
	if (edge.get() != 0) {
		db->erase<Edge>(*edge);
		deleted = true;
	}
	t.commit();
	return deleted;
}


#endif // EDGE_ACTIONS_HXX