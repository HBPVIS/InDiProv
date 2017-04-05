#define DATABASE_MYSQL

#include <memory>   // std::auto_ptr
#include <string>
#include <iostream>
#include <thread>
#include <ctime>

std::string clientname;
bool running = true;

void loginPrompt();
void mainMenuPrompt();
void newVertexPrompt();
void newEdgePrompt();

int main(int argc, char* argv[]) {
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

	std::string name;
	switch (ans) {
	case '0':
		std::cout << "Please enter Activity name" << std::endl;
		break;
	case '1':
		std::cout << "Please enter Agent name" << std::endl;
		break;
	case '2':
		std::cout << "Please enter Entity name" << std::endl;
		break;
	case 'q':
		return;
	default:
		std::cout << "Invalid input." << std::endl;
		return;
	}
	std::getline(std::cin, name);
	
	auto t = time(nullptr);

	std::cout << "Sending Vertex log info: " << name << std::endl << std::endl;
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
	switch (ans) {
	case '0':
		break;
	case '1':
		break;
	case '2':
		break;
	case '3':
		break;
	case '4':
		break;
	case '5':
		break;
	case '6':
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

	//std::cout << "Sending Edge log info: " << name1 << " " << edgeTypeToString((edgeType)edge->type()) << " " << name2 << std::endl << std::endl;
}
