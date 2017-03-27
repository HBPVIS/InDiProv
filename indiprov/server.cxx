#define DATABASE_MYSQL

#include <memory>   // std::auto_ptr
#include <iostream>
#include <thread>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx" // create_database

#include "controller/vertex-actions.hxx"
#include "controller/edge-actions.hxx"

#include <nett/nett.h>
#include "model/creation_messages.pb.h"

using namespace odb::core;

const std::string endpoint("tcp://127.0.0.1:6555");
const std::string altEndpoint("tcp://127.0.0.1:6556");
shared_ptr<std::thread> pollingThread;
shared_ptr<nett::slot_in<Creation>> slotIn;
std::auto_ptr<database> db;

void handleMessage();

int main (int argc, char* argv[]) {
	std::cout << "Initializing server..." << std::endl;
	nett::initialize(altEndpoint);
	try {
		db = create_database(argc, argv);
	} catch(const odb::exception& e) {
		std::cerr << e.what () << std::endl;
		Sleep(5000);
		return 1;
	}
	slotIn = nett::make_slot_in<Creation>();
	slotIn->connect(endpoint, "creation");
	std::cout << "Server startup complete." << std::endl;

	//for some reason this is not working from a secondary thread
	//pollingThread = std::make_shared<std::thread>(&handleMessage);
	handleMessage();

	return 0;
}


void handleMessage() {
	while (true) {
		std::cout << "Listening..." << std::endl;
		Creation msg = slotIn->receive();
		std::cout << "Message received." << std::endl;
		for (int i = 0; i < msg.vertices_size(); i++) {
			auto vert = msg.vertices(i);
			std::string name = msg.clientname() + "_" + vert.name();
			int id = createVertex(db, (vertexType)vert.type(), name, vert.start(), vert.end());
			if (id != -1) {
				std::cout << vert.name() << std::endl;
			}
		}
		for (int i = 0; i < msg.edges_size(); i++) {
			auto edge = msg.edges(i);
			std::string name1 = msg.clientname() + "_" + edge.firstvertname();
			std::string name2 = msg.clientname() + "_" + edge.secondvertname();
			int id = createEdge(db, (edgeType)edge.type(), name1, name2);
			if (id != -1) {
				std::cout << edge.firstvertname() << " " << edgeTypeToString((edgeType)edge.type()) << " " << edge.secondvertname() << std::endl;
			}
		}
		std::cout << std::endl;
	}
}
