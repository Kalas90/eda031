#include "server.h"
#include "article.h"
#include "newsserver.h"
#include "newsgroup.h"
#include "newsgroupprovider.h"
#include "protocol.h"
#include "connectionclosedexception.h"
#include "protocolviolationexception.h"
#include "missingnewsgroupexception.h"
#include "missingarticleexception.h"

#include <memory>

NewsServer::NewsServer(int p, NewsgroupProvider& n, bool v) : port(p), ngp(n), server(p), verbose(v) {
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

int NewsServer::read_num_p(const std::shared_ptr<Connection>& conn) {
	unsigned char par_num = conn->read();
	if (par_num != Protocol::PAR_NUM)
		throw ProtocolViolationException();

	return read_number(conn);
}

std::string NewsServer::read_string_p(const std::shared_ptr<Connection>& conn) {
	unsigned char par_string = conn->read();
	if (par_string != Protocol::PAR_STRING)
		throw ProtocolViolationException();

	int size = read_number(conn);
	return read_string(conn, size);
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

void NewsServer::write_string_p(const std::shared_ptr<Connection>& conn, const std::string& s) {
	conn->write(Protocol::PAR_STRING);
	write_string(conn, s);
}

void NewsServer::write_num_p(const std::shared_ptr<Connection>& conn, int value) {
	conn->write(Protocol::PAR_NUM);
	write_number(conn, value);
}
	
void NewsServer::listen() {
	while (true) {
		auto conn = server.waitForActivity();
		if (conn != nullptr) {
			try {
				int nbr = conn->read();
                if (verbose) std::cout << "Recieved command: " << nbr << std::endl;
				switch (nbr) {
					case Protocol::COM_LIST_NG:
						ans_list_ng(conn);
						break;
					case Protocol::COM_CREATE_NG:
						ans_success(create_ng(conn), conn);
						break;
					case Protocol::COM_DELETE_NG:
						ans_success(delete_ng(conn), conn);
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
						throw ProtocolViolationException(); 
						break;
				}
                int com_end = conn->read();
                if (com_end != Protocol::COM_END)
                    throw ProtocolViolationException();
			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				std::cout << "Client closed connection" << std::endl;
			} catch (ProtocolViolationException&) {
                server.deregisterConnection(conn);
                std::cout << "Client violated the protocol. Connection closed" << std::endl;
            } catch (MissingNewsgroupException&) {
                conn->write(Protocol::ANS_NAK);
                conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
                conn->write(Protocol::ANS_END);
            } catch (MissingArticleException&) {
                conn->write(Protocol::ANS_NAK);
                conn->write(Protocol::ERR_ART_DOES_NOT_EXIST);
                conn->write(Protocol::ANS_END);
            }
		} else {
			conn = std::make_shared<Connection>();
			server.registerConnection(conn);
			std::cout << "New client connects" << std::endl;
		}
	}
}

void NewsServer::ans_success(bool success, const std::shared_ptr<Connection>& conn, char err) {
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
    write_num_p(conn, list.size());

	for (auto g : list) {
        write_num_p(conn, g.get_id());
        write_string_p(conn, g.get_name());
	}

	conn->write(Protocol::ANS_END);
}

bool NewsServer::create_ng(const std::shared_ptr<Connection>& conn) {
	return (ngp.create_newsgroup(read_string_p(conn)));
}

bool NewsServer::delete_ng(const std::shared_ptr<Connection>& conn) {
	return ngp.remove_newsgroup(read_num_p(conn));
}

void NewsServer::ans_list_art(const std::shared_ptr<Connection>& conn) {
	int newsgroup_id = read_num_p(conn);
    
    conn->write(Protocol::ANS_LIST_ART);
    
    try {
        std::vector<Article> list = ngp.list_articles(newsgroup_id);  // Assume that newsgroup_id exist in ngp. Need fix.

	    // Case newsgroup exist
	    conn->write(Protocol::ANS_ACK);
	    write_num_p(conn, list.size());

        for (auto a : list) {
            write_num_p(conn, a.get_id());
            write_string_p(conn, a.get_title());
        }

        conn->write(Protocol::ANS_END);

    } catch (MissingNewsgroupException&) {
        // Case newsgroup does not exist
        conn->write(Protocol::ANS_NAK);
        conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
        conn->write(Protocol::ANS_END);
    }
}

bool NewsServer::create_art(const std::shared_ptr<Connection>& conn) {
	return ngp.create_article(read_num_p(conn), read_string_p(conn), read_string_p(conn), read_string_p(conn));
}

bool NewsServer::delete_art(const std::shared_ptr<Connection>& conn) {
	return ngp.remove_article(read_num_p(conn), read_num_p(conn));
}

void NewsServer::ans_get_art(const std::shared_ptr<Connection>& conn) {
    conn->write(Protocol::ANS_GET_ART);
    
    try {
        Article a = ngp.article(read_num_p(conn), read_num_p(conn));

        // Case article exist
        conn->write(Protocol::ANS_ACK);
        write_string_p(conn, a.get_title());
        write_string_p(conn, a.get_author());
        write_string_p(conn, a.get_text());

        conn->write(Protocol::ANS_END);

    } catch (MissingArticleException&) {
        conn->write(Protocol::ANS_NAK);
        conn->write(Protocol::ERR_ART_DOES_NOT_EXIST);
        conn->write(Protocol::ANS_END);
    } catch (MissingNewsgroupException&) {
        conn->write(Protocol::ANS_NAK);
        conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
        conn->write(Protocol::ANS_END);
    }
}
