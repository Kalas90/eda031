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
	int readNumber(const std::shared_ptr<Connection>& conn);
	void writeString(const std::shared_ptr<Connection>& conn, const std::string& s);
};

#endif
