#ifndef EDGE_ACTIONS_HXX
#define EDGE_ACTIONS_HXX

#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;

#include "../Model/edge.hxx"
#include "../Model/edge-odb.hxx"

using namespace odb::core;

int createEdge(std::shared_ptr<database>& db, edgeType type, long firstId, long secondId) {
	transaction t(db->begin());

	shared_ptr<Vertex> first(
		db->query_one<Vertex>(query<Vertex>::id == firstId));
	if (first.get() == 0 || first.get()->GetType() != getVertexType(type, true)) {
		std::cerr << "First vertex invalid! " << edgeTypeToString(type) << " not created." << std::endl;
		t.commit();
		return -1;
	}

	shared_ptr<Vertex> second(
		db->query_one<Vertex>(query<Vertex>::id == secondId));
	if (second.get() == 0 || second.get()->GetType() != getVertexType(type, false)) {
		std::cerr << "Second vertex invalid!" << edgeTypeToString(type) << " not created." << std::endl;
		t.commit();
		return -1;
	}

	Edge edge(type, first, second);
	long id = db->persist(edge);
	t.commit();
	return id;
}

int createEdge(std::shared_ptr<database>& db, edgeType type, std::string firstVertName, std::string secondVertName) {
	transaction t(db->begin());

	shared_ptr<Vertex> first(
		db->query_one<Vertex>(query<Vertex>::name == firstVertName));
	if (first.get() == 0 || first.get()->GetType() != getVertexType(type, true)) {
		std::cerr << "First vertex invalid! " << edgeTypeToString(type) << " not created." << std::endl;
		t.commit();
		return -1;
	}

	shared_ptr<Vertex> second(
		db->query_one<Vertex>(query<Vertex>::name == secondVertName));
	if (second.get() == 0 || second.get()->GetType() != getVertexType(type, false)) {
		std::cerr << "Second vertex invalid!" << edgeTypeToString(type) << " not created." << std::endl;
		t.commit();
		return -1;
	}

	Edge edge(type, first, second);
	long id = db->persist(edge);
	t.commit();
	return id;
}

bool deleteEdge(std::shared_ptr<database>& db, long id) {
	bool deleted = false;
	transaction t(db->begin());
	std::auto_ptr<Edge> edge(
		db->query_one<Edge>(query<Edge>::id == id));
	if (edge.get() != 0) {
		db->erase<Edge>(*edge);
		deleted = true;
	}
	t.commit();
	return deleted;
}

std::vector<Edge> getEdges(std::shared_ptr<database>& db, std::vector<unsigned long> ids) {
	std::vector<Edge> vec;
	Edge edge;
	transaction t(db->begin());
	result<Edge> r (db->query<Edge>(
		query<Edge>::first.in_range(ids.begin(), ids.end())
		|| query<Edge>::second.in_range(ids.begin(), ids.end())
	));

	for (result<Edge>::iterator i (r.begin()); i != r.end(); ++i) {
		i.load(edge);
		vec.push_back(edge);
	}
	t.commit();
	return vec;
}

#endif // EDGE_ACTIONS_HXX
