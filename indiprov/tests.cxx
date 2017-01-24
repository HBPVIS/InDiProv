#define DATABASE_MYSQL

#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx" // create_database

#include "controller/vertex-actions.hxx"
#include "controller/edge-actions.hxx"

#include <nett/nett.h>
#include "model/creation_messages.pb.h"

using namespace std;
using namespace odb::core;

int main (int argc, char* argv[]) {
	const std::string endpoint("tcp://127.0.0.1:6555");

	nett::initialize(endpoint);

	try {
		auto_ptr<database> db(create_database (argc, argv));

		long id1 = createVertex(db, Agent, "cyremur", 0, 13);
		long id2 = createVertex(db, Agent, "mercury", 0, 13);

		long id3 = createEdge(db, actedOnBehalfOf, id1, id2);

		long id4 = createVertex(db, Entity, "InDiProv", 0, 0);

		long id5 = createEdge(db, wasAttributedTo, id1, id4);

		transaction t(db->begin());
		auto vert = db->load<Vertex>(id1);

		result<Edge> r(db->query<Edge>(query<Edge>::first == id1));

		for (result<Edge>::iterator i(r.begin()); i != r.end(); ++i) {
			result<Vertex> res(db->query<Vertex>(query<Vertex>::id == i->getSecond()->GetId()));
			for (result<Vertex>::iterator j(res.begin()); j != res.end(); ++j) {
				cout << vert->GetName() << " " << i->getTypeString() << " " << j->GetName() << endl;
			}
		}
		t.commit();
		cout << std::endl << "Tests successful" << std::endl;

	} catch(const odb::exception& e) {
		cerr << e.what () << endl;
		string nope;
		cin >> nope;
		return 1;
	}
	string nope;
	cin >> nope;
	return 0;
}
