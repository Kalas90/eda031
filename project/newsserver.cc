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

/*
 * Read an integer from a client.
 */
int NewsServer::read_number(const std::shared_ptr<Connection>& conn) {
	unsigned char byte1 = conn->read();
	unsigned char byte2 = conn->read();
	unsigned char byte3 = conn->read();
	unsigned char byte4 = conn->read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

/*
 * Read a string from the server.
 */
std::string NewsServer::read_string(const std::shared_ptr<Connection>& conn, int size) {
	std::string s;
	for (int i = 0; i<size; ++i) {
		s += conn->read();
	}
	return s;
}

/*
 * Send a string to a client.
 */
void NewsServer::write_string(const std::shared_ptr<Connection>& conn, const std::string& s) {
	write_number(conn, s.size());
	for (char c : s) {
		conn->write(c);
	}
}

/*
 * Send an integer to the server as four bytes.
 */
void NewsServer::write_number(const std::shared_ptr<Connection>& conn, int value) {
	conn->write((value >> 24) & 0xFF);
	conn->write((value >> 16) & 0xFF);
	conn->write((value >> 8)	 & 0xFF);
	conn->write(value & 0xFF);
}
	
void NewsServer::listen() {
	while (true) {
		auto conn = server.waitForActivity();
		if (conn != nullptr) {
			try {
				int nbr = read_number(conn);
				switch (nbr) {
					case Protocol::COM_LIST_NG:
						ans_list_ng(conn);
						break;
					case Protocol::COM_CREATE_NG:
						ans_success(create_ng(conn), conn);
						break;
					case Protocol::COM_DELETE_NG:
						ans_success(delete_ng(conn), conn); // Continue here
						break;
					case Protocol::COM_LIST_ART:
						ans_list_art(conn);
						break;
					case Protocol::COM_CREATE_ART:
						ans_success(create_art(conn), conn);
						break;
					case Protocol::COM_DELETE_ART:
						ans_success(delete_art(conn), conn);
						break;
					case Protocol::COM_GET_ART:
						ans_get_art(conn);
						break;
					default:
						throw ConnectionClosedException(); // Should we send something to client? Syntax error?
						break;
				}
			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				std::cout << "Client closed connection" << std::endl;
			}
			// We could catch some other ProtocolViolationException
		} else {
			conn = std::make_shared<Connection>();
			server.registerConnection(conn);
			std::cout << "New client connects" << std::endl;
		}
	}
}

void NewsServer::ans_success(bool success, const std::shared_ptr<Connection>& conn) {
	if (success) {
		conn->write(Protocol::ANS_ACK);
	} else {
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_ALREADY_EXISTS);
	}
	
}

void NewsServer::ans_list_ng(const std::shared_ptr<Connection>& conn) {
	std::vector<Newsgroup> list = ngp.list_news_groups();

	conn->write(Protocol::ANS_LIST_NG);
	conn->write(Protocol::PAR_NUM);
	write_number(conn, list.size());

	for (auto g : list) {
		conn->write(Protocol::PAR_NUM);
		write_number(conn, g.get_id());
		conn->write(Protocol::PAR_STRING);
		write_string(conn, g.get_name());
	}

	conn->write(Protocol::ANS_END);
}

bool NewsServer::create_ng(const std::shared_ptr<Connection>& conn) {
	unsigned char par_string = conn->read();
	if (par_string != Protocol::PAR_STRING)
		throw ConnectionClosedException();

	int size = read_number(conn);
	std::string group_name = read_string(conn, size);

	return (ngp.create_newsgroup(group_name));
}

bool NewsServer::delete_ng(const std::shared_ptr<Connection>& conn) {
	unsigned char par_num = conn->read();
	if (par_num != Protocol::PAR_NUM)
		throw ConnectionClosedException();

	int newsgroup_id = read_number(conn);

	return ngp.remove_newsgroup(newsgroup_id);
}

void NewsServer::ans_list_art(const std::shared_ptr<Connection>& conn) {
	unsigned char par_num = conn->read();
	if (par_num != Protocol::PAR_NUM)
		throw ConnectionClosedException();

	int newsgroup_id = read_number(conn);
	vector<Article> list = ngp.list_articles(newsgroup_id);  // Assume that newsgroup_id exist in ngp. Need fix.

	conn->write(Protocol::ANS_LIST_ART);

	// Case newsgroup exist
	conn->write(Protocol::ANS_ACK);
	conn->write(Protocol::PAR_NUM);
	write_number(conn, list.size());

	for (auto a : list) {
		conn->write(Protocol::PAR_NUM);
		write_number(conn, a.get_id());
		conn->write(Protocol::PAR_STRING);
		write_string(conn, a.get_title());
	}

	conn->write(Protocol::ANS_END);
}

bool NewsServer::create_art(const std::shared_ptr<Connection>& conn) {
	return false;
}

bool NewsServer::delete_art(const std::shared_ptr<Connection>& conn) {
	return false;
}

void NewsServer::ans_get_art(const std::shared_ptr<Connection>& conn) {

}