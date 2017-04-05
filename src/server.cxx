#define DATABASE_MYSQL

#include "log-service.hxx"

int main (int argc, char* argv[]) {
	std::cout << "Initializing server..." << std::endl;

	Net::Port port(9080);
	int thr = 2;

	Net::Address addr(Net::Ipv4::any(), port);

	std::cout << "Cores = " << hardware_concurrency() << std::endl;
	std::cout << "Using " << thr << " threads" << std::endl;

	LogService logger(addr);

  try {
		logger.init(thr, argc, argv);
	} catch(const odb::exception& e) {
		std::cerr << e.what () << std::endl;
		return;
	}
	logger.start();

	logger.shutdown();

	std::cout << "Server startup complete." << std::endl;

	return 0;
}
