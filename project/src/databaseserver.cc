#include "newsserver.h"
#include "database.h"
#include <sstream>

int main(int argc,char *argv[]) {
    // Usage: runserver <port>

    if (argc < 2 || argc > 3) {
        std::cout << "Usage: ./runserver <port> <verbose>" << std::endl;
        std::cout << "    port: number" << std::endl;
        std::cout << "    verbose: 0|1" << std::endl;
        exit(1);
    }

    bool verbose = false;
    if (argc == 3) {
        verbose = std::string(argv[2]) == std::string("1");
    }

    std::cout << "verbose: " << verbose << std::endl;

    std::istringstream ss(argv[1]);
    int port;
    if (!(ss >> port)) {
        std::cerr << "Invalid number " << argv[1] << std::endl;
        exit(1);
    }

    NewsgroupDatabase ndb("db.sqlite3");
    NewsServer ns(port, ndb, verbose);
    std::cout << "Server running on port " << port << std::endl;
    ns.listen();
}
