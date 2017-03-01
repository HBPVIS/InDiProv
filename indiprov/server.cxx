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

using namespace std;
using namespace odb::core;

const string endpoint("tcp://127.0.0.1:6555");
const string altEndpoint("tcp://127.0.0.1:6556");
shared_ptr<thread> pollingThread;
shared_ptr<nett::slot_in<Creation>> slotIn;
auto_ptr<database> db;

void handleMessage();

int main (int argc, char* argv[]) {
	cout << "Initializing server..." << endl;
	nett::initialize(altEndpoint);
	try {
		db = create_database(argc, argv);
	} catch(const odb::exception& e) {
		cerr << e.what () << endl;
		Sleep(5000);
		return 1;
	}
	slotIn = nett::make_slot_in<Creation>();
	slotIn->connect(endpoint, "creation");
	cout << "Server startup complete." << endl;

	//for some reason this is not working from a secondary thread
	//pollingThread = std::make_shared<std::thread>(&handleMessage);
	handleMessage();

	return 0;
}


void handleMessage() {
	while (true) {
		cout << "Listening..." << endl;
		Creation msg = slotIn->receive();
		cout << "Message received." << endl;
		for (int i = 0; i < msg.vertices_size(); i++) {
			auto vert = msg.vertices(i);
			string name = msg.clientname() + "_" + vert.name();
			int id = createVertex(db, (vertexType)vert.type(), name, vert.start(), vert.end());
			if (id != -1) {
				cout << vert.name() << endl;
			}
		}
		for (int i = 0; i < msg.edges_size(); i++) {
			auto edge = msg.edges(i);
			string name1 = msg.clientname() + "_" + edge.firstvertname();
			string name2 = msg.clientname() + "_" + edge.secondvertname();
			int id = createEdge(db, (edgeType)edge.type(), name1, name2);
			if (id != -1) {
				cout << edge.firstvertname() << " " << edgeTypeToString((edgeType)edge.type()) << " " << edge.secondvertname() << endl;
			}
		}
		cout << endl;
	}
}
