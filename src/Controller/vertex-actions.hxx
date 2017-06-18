#ifndef VERTEX_ACTIONS_HXX
#define VERTEX_ACTIONS_HXX

#include <iostream>
#include <vector>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "../Model/vertex.hxx"
#include "../Model/vertex-odb.hxx"

using namespace odb::core;

long createVertex(std::shared_ptr<database>& db, std::string client, std::string session, vertexType type, std::string name, unsigned long start, unsigned long end) {
	Vertex vertex(client, session, type, name, start, end);
	transaction t(db->begin());
	long id = db->persist(vertex);
	t.commit();
	return id;
}

bool deleteVertex(std::shared_ptr<database>& db, long id) {
	bool deleted = false;
	transaction t(db->begin());
	std::shared_ptr<Vertex> vertex(
		db->query_one<Vertex>(query<Vertex>::id == id));
	if (vertex.get() != 0) {
		db->erase<Vertex>(*vertex);
		deleted = true;
	}
	t.commit();
	return deleted;
}

std::vector<Vertex> getVertex(std::shared_ptr<database>& db, vertexType vType) {
	std::vector<Vertex> vec;
	Vertex vert;
	transaction t(db->begin());
	result<Vertex> r (db->query<Vertex>(query<Vertex>::type == vType));
	for (result<Vertex>::iterator i (r.begin()); i != r.end(); ++i) {
		i.load(vert);
		vec.push_back(vert);
	}
	t.commit();
	return vec;
}

#endif // VERTEX_ACTIONS_HXX
