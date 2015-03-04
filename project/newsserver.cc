#include "server.h"
#include "article.h"
#include "newsserver.h"
#include "newsgroup.h"
#include "newsgroupprovider.h"
#include "protocol.h"
#include "connectionclosedexception.h"

#include <memory>

NewsServer::NewsServer(int p, NewsgroupProvider& n) : port(p), ngp(n), server(p) {
	if (!server.isReady()) {
		std::cerr << "Server initialization error." << std::endl;
		exit(1);
	}
}

void NewsServer::listen() {
	while (true) {
		auto conn = server.waitForActivity();
		if (conn != nullptr) {
			try {
				int nbr = readNumber(conn);
				std::string result;
				switch (nbr) {
					case Protocol::COM_LIST_NG:
						break;
					case Protocol::COM_CREATE_NG:
						break;
					case Protocol::COM_DELETE_NG:
						break;
					case Protocol::COM_LIST_ART:
						break;
					case Protocol::COM_CREATE_ART:
						break;
					case Protocol::COM_DELETE_ART:
						break;
					case Protocol::COM_GET_ART:
						break;
					case Protocol::COM_END:
						break;
				}
				writeString(conn, result);
			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				std::cout << "Client closed connection" << std::endl;
			}
		} else {
			conn = std::make_shared<Connection>();
			server.registerConnection(conn);
			std::cout << "New client connects" << std::endl;
		}
	}
}

/*
 * Read an integer from a client.
 */
int NewsServer::readNumber(const std::shared_ptr<Connection>& conn) {
	unsigned char byte1 = conn->read();
	unsigned char byte2 = conn->read();
	unsigned char byte3 = conn->read();
	unsigned char byte4 = conn->read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

/*
 * Send a string to a client.
 */
void NewsServer::writeString(const std::shared_ptr<Connection>& conn, const std::string& s) {
	for (char c : s) {
		conn->write(c);
	}
	conn->write('$');
}
	
