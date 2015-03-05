#ifndef NEWSSERVER_H
#define NEWSSERVER_H

#include "server.h"
#include "article.h"
#include "newsgroup.h"
#include "newsgroupprovider.h"

#include <memory>

class NewsServer {
public:
	NewsServer(int port, NewsgroupProvider& n);
	void listen();
private:
	int port;
	NewsgroupProvider& ngp;
	Server server;

	int read_number(const std::shared_ptr<Connection>& conn);
	std::string read_string(const std::shared_ptr<Connection>& conn, int size);
	int read_num_p(const std::shared_ptr<Connection>& conn);
	std::string read_string_p(const std::shared_ptr<Connection>& conn);
	void write_string(const std::shared_ptr<Connection>& conn, const std::string& s);
	void write_number(const std::shared_ptr<Connection>& conn, int value);
	void write_string_p(const std::shared_ptr<Connection>& conn, const std::string& s);
	void write_num_p(const std::shared_ptr<Connection>& conn, int value);

	bool create_ng(const std::shared_ptr<Connection>& conn);
	bool delete_ng(const std::shared_ptr<Connection>& conn);
	bool create_art(const std::shared_ptr<Connection>& conn);
	bool delete_art(const std::shared_ptr<Connection>& conn);
	void ans_list_ng(const std::shared_ptr<Connection>& conn);
	void ans_list_art(const std::shared_ptr<Connection>& conn);
	void ans_get_art(const std::shared_ptr<Connection>& conn);
	void ans_success(bool success, const std::shared_ptr<Connection>& conn);
};

#endif
