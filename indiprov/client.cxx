#define DATABASE_MYSQL

#include <memory>   // std::auto_ptr
#include <iostream>
#include <thread>
#include <ctime>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx" // create_database

#include "controller/vertex-actions.hxx"
#include "controller/edge-actions.hxx"

#include <nett/nett.h>
#include "model/creation_messages.pb.h"

using namespace std;
using namespace odb::core;

string clientname;
const string endpoint("tcp://127.0.0.1:6555");
shared_ptr<nett::slot_out<Creation>> slotOut;
bool running = true;

void loginPrompt();
void mainMenuPrompt();
void newVertexPrompt();
void newEdgePrompt();

int main(int argc, char* argv[]) {
	nett::initialize(endpoint);
	slotOut = nett::make_slot_out<Creation>("creation");

	loginPrompt();
	while (running) {
		mainMenuPrompt();
	}

	return 0;
}

void loginPrompt() {
	cout << "Welcome to InDiProv Logging. Please enter a unique client name." << endl;
	getline(cin, clientname);
	cout << endl;
}

void mainMenuPrompt() {
	cout << "Please choose an option or enter 'q' to quit." << endl;
	cout << "0: new Vertex" << endl;
	cout << "1: new Edge" << endl;
	char ans = getchar();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl;
	switch (ans) {
	case '0':
		newVertexPrompt();
		break;
	case '1':
		newEdgePrompt();
		break;
	case 'q':
		cout << "Shutting down." << endl;
		running = false;
		return;
	default:
		cout << "Invalid input." << endl << endl;
		return;
	}
}

void newVertexPrompt() {
	cout << "Please choose a Vertex Type or enter 'q' to quit." << endl;
	cout << "0: Activity" << endl;
	cout << "1: Agent" << endl;
	cout << "2: Entity" << endl;
	char ans = getchar();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl;
	Creation message;
	message.set_clientname(clientname);
	Creation::NewVertex* vert = message.add_vertices();
	string name;
	switch (ans) {
	case '0':
		vert->set_type(Creation::Activity);
		cout << "Please enter Activity name" << endl;
		break;
	case '1':
		vert->set_type(Creation::Agent);
		cout << "Please enter Agent name" << endl;
		break;
	case '2':
		vert->set_type(Creation::Entity);
		cout << "Please enter Entity name" << endl;
		break;
	case 'q':
		return;
	default:
		cout << "Invalid input." << endl;
		return;
	}
	getline(cin, name);
	vert->set_name(name);
	auto t = time(nullptr);
	vert->set_start(t);
	vert->set_end(t);

	cout << "Sending Vertex log info: " << name << endl << endl;
	slotOut->send(message);
}

void newEdgePrompt() {
	cout << "Please choose an Edge Type or enter 'q' to quit." << endl;
	cout << "0: wasGeneratedBy" << endl;
	cout << "1: wasDerivedFrom" << endl;
	cout << "2: wasAttributedTo" << endl;
	cout << "3: used" << endl;
	cout << "4: wasInformedBy" << endl;
	cout << "5: wasAssociatedWith" << endl;
	cout << "6: actedOnBehalfOf" << endl;
	char ans = getchar();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl;
	Creation message;
	message.set_clientname(clientname);
	Creation::NewEdge* edge = message.add_edges();
	switch (ans) {
	case '0':
		edge->set_type(Creation::wasGeneratedBy);
		break;
	case '1':
		edge->set_type(Creation::wasDerivedFrom);
		break;
	case '2':
		edge->set_type(Creation::wasAttributedTo);
		break;
	case '3':
		edge->set_type(Creation::used);
		break;
	case '4':
		edge->set_type(Creation::wasInformedBy);
		break;
	case '5':
		edge->set_type(Creation::wasAssociatedWith);
		break;
	case '6':
		edge->set_type(Creation::actedOnBehalfOf);
		break;
	case 'q':
		return;
	default:
		cout << "Invalid input." << endl;
		return;
	}
	string name1, name2;
	cout << "Please enter first Vertex name." << endl;
	getline(cin, name1);
	cout << endl;
	cout << "Please enter second Vertex name." << endl;
	getline(cin, name2);
	cout << endl;
	edge->set_firstvertname(name1);
	edge->set_secondvertname(name2);

	cout << "Sending Edge log info: " << name1 << " " << edgeTypeToString((edgeType)edge->type()) << " " << name2 << endl << endl;
}