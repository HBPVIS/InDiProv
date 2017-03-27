#define DATABASE_MYSQL

#include <memory>   // std::auto_ptr
#include <iostream>
#include <thread>
#include <ctime>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.hxx" // create_database

#include "Controller/vertex-actions.hxx"
#include "Controller/edge-actions.hxx"

#include <nett/nett.h>
#include "Model/creation_messages.pb.h"

using namespace odb::core;

std::string clientname;
const std::string endpoint("tcp://127.0.0.1:6555");
std::shared_ptr<nett::slot_out<Creation>> slotOut;
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
	std::cout << "Welcome to InDiProv Logging. Please enter a unique client name." << std::endl;
	std::getline(std::cin, clientname);
	std::cout << std::endl;
}

void mainMenuPrompt() {
	std::cout << "Please choose an option or enter 'q' to quit." << std::endl;
	std::cout << "0: new Vertex" << std::endl;
	std::cout << "1: new Edge" << std::endl;
	char ans = getchar();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout << std::endl;
	switch (ans) {
	case '0':
		newVertexPrompt();
		break;
	case '1':
		newEdgePrompt();
		break;
	case 'q':
		std::cout << "Shutting down." << std::endl;
		running = false;
		return;
	default:
		std::cout << "Invalid input." << std::endl << std::endl;
		return;
	}
}

void newVertexPrompt() {
	std::cout << "Please choose a Vertex Type or enter 'q' to quit." << std::endl;
	std::cout << "0: Activity" << std::endl;
	std::cout << "1: Agent" << std::endl;
	std::cout << "2: Entity" << std::endl;
	char ans = getchar();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout << std::endl;
	Creation message;
	message.set_clientname(clientname);
	Creation::NewVertex* vert = message.add_vertices();
	std::string name;
	switch (ans) {
	case '0':
		vert->set_type(Creation::Activity);
		std::cout << "Please enter Activity name" << std::endl;
		break;
	case '1':
		vert->set_type(Creation::Agent);
		std::cout << "Please enter Agent name" << std::endl;
		break;
	case '2':
		vert->set_type(Creation::Entity);
		std::cout << "Please enter Entity name" << std::endl;
		break;
	case 'q':
		return;
	default:
		std::cout << "Invalid input." << std::endl;
		return;
	}
	std::getline(std::cin, name);
	vert->set_name(name);
	auto t = time(nullptr);
	vert->set_start(t);
	vert->set_end(t);

	std::cout << "Sending Vertex log info: " << name << std::endl << std::endl;
	slotOut->send(message);
}

void newEdgePrompt() {
	std::cout << "Please choose an Edge Type or enter 'q' to quit." << std::endl;
	std::cout << "0: wasGeneratedBy" << std::endl;
	std::cout << "1: wasDerivedFrom" << std::endl;
	std::cout << "2: wasAttributedTo" << std::endl;
	std::cout << "3: used" << std::endl;
	std::cout << "4: wasInformedBy" << std::endl;
	std::cout << "5: wasAssociatedWith" << std::endl;
	std::cout << "6: actedOnBehalfOf" << std::endl;
	char ans = getchar();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout << std::endl;
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
		std::cout << "Invalid input." << std::endl;
		return;
	}
	std::string name1, name2;
	std::cout << "Please enter first Vertex name." << std::endl;
	std::getline(std::cin, name1);
	std::cout << std::endl;
	std::cout << "Please enter second Vertex name." << std::endl;
	std::getline(std::cin, name2);
	std::cout << std::endl;
	edge->set_firstvertname(name1);
	edge->set_secondvertname(name2);

	std::cout << "Sending Edge log info: " << name1 << " " << edgeTypeToString((edgeType)edge->type()) << " " << name2 << std::endl << std::endl;
	slotOut->send(message);
}
