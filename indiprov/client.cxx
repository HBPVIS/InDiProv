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

const string endpoint("tcp://127.0.0.1:6555");
shared_ptr<nett::slot_out<Creation>> slotOut;

void mainMenuPrompt();
void newVertexPrompt();
void newEdgePrompt();

int main(int argc, char* argv[]) {
	nett::initialize(endpoint);
	slotOut = nett::make_slot_out<Creation>("creation");

	while (true) {
		mainMenuPrompt();
	}

	return 0;
}

void mainMenuPrompt() {
	cout << "Welcome to InDiProv Logging. Please choose an option." << endl;
	cout << "1: new Vertex" << endl;
	cout << "2: new Edge" << endl;
	char ans = getchar();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl;
	switch (ans) {
	case '1':
		newVertexPrompt();
		break;
	case '2':
		newEdgePrompt();
		break;
	default:
		cout << "Invalid input." << endl << endl;
		return;
	}
}

void newVertexPrompt() {
	cout << "Please choose a Vertex Type." << endl;
	cout << "1: Activity" << endl;
	cout << "2: Agent" << endl;
	cout << "3: Entity" << endl;
	char ans = getchar();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl;
	Creation message;
	Creation::NewVertex* vert = message.add_vertices();
	string name;
	switch (ans) {
	case '1':
		vert->set_type(Creation::Activity);
		cout << "Please enter Activity name" << endl;
		break;
	case '2':
		vert->set_type(Creation::Agent);
		cout << "Please enter Agent name" << endl;
		break;
	case '3':
		vert->set_type(Creation::Entity);
		cout << "Please enter Entity name" << endl;
		break;
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
	cout << "Not yet implemented." << endl << endl;
}