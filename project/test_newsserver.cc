#include "newsserver.h"
#include "memorynewsgroupprovider.h"
#include <sstream>

int main(int argc,char *argv[]) {
    // Usage: runserver <port>

    if (argc != 2) {
        std::cout << "Usage: ./runserver <port>" << std::endl;
        exit(1);
    }   

    std::istringstream ss(argv[1]);
    int port;
    if (!(ss >> port)) {
        std::cerr << "Invalid number " << argv[1] << std::endl;
        exit(1);
    }

	MemoryNewsgroupProvider mnsg;
	NewsServer ns(port, mnsg);
    std::cout << "Server running on port " << port << std::endl;
    ns.listen();
}
