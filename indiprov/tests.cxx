#define DATABASE_MYSQL

#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx" // create_database

#include "Controller/vertex-actions.hxx"
#include "Controller/edge-actions.hxx"

#include <nett/nett.h>
#include "Model/creation_messages.pb.h"

using namespace odb::core;

int main (int argc, char* argv[]) {
	const std::string endpoint("tcp://127.0.0.1:6555");

	nett::initialize(endpoint);
	auto slotOut = nett::make_slot_out<Creation>("creation");
	auto slotIn = nett::make_slot_in<Creation>();
	slotIn->connect(endpoint, "creation");

	std::this_thread::sleep_for(std::chrono::seconds(1));

	Creation message;
	Creation::NewVertex* vert1 = message.add_vertices();
	vert1->set_type(Creation::Agent);
	vert1->set_name("cyrus");
	vert1->set_start(0);
	vert1->set_end(0);

	Creation::NewVertex* vert2 = message.add_vertices();
	vert2->set_type(Creation::Agent);
	vert2->set_name("cyriel");
	vert2->set_start(0);
	vert2->set_end(0);

	slotOut->send(message);


	

	try {
		std::auto_ptr<database> db(create_database (argc, argv));

		auto msg = slotIn->receive();
		for (int i = 0; i < msg.vertices_size(); i++) {
			auto vert = msg.vertices(i);
			createVertex(db, (vertexType)vert.type(), vert.name(), vert.start(), vert.end());
		}

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
				std::cout << vert->GetName() << " " << i->getTypeString() << " " << j->GetName() << std::endl;
			}
		}
		t.commit();
		std::cout << std::endl << "Tests successful" << std::endl;

	} catch(const odb::exception& e) {
		std::cerr << e.what () << std::endl;
		std::string nope;
		std::cin >> nope;
		return 1;
	}
	std::string nope;
	std::cin >> nope;
	return 0;
}
