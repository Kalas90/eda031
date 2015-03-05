#ifndef NEWSCLIENT_H
#define NEWSCLIENT_H

#include <string>
#include <iostream>
#include "connection.h"
#include "protocol.h"

class NewsClient {
  public:
    NewsClient(std::ostream& o, const Connection& c) : os(o), conn(c) {};
    void print_help() const;
    void print_prompt() const;
    void parse_and_execute_command(const std::string&);
    void receive();
    void print_response();
    bool wait_for_response() const;

  private:
    void send_list_newsgroups() const;
    void send_create_newsgroup(const std::string& title) const;
    void send_delete_newsgroup(const int ng_id) const;
    void send_list_articles(const int ng_id) const;
    void send_create_article(const int ng_id, const std::string& title, 
          const std::string& author, const std::string& text) const;
    void send_delete_article(const int ng_id, const int art_id) const;
    void send_get_article(const int ng_id, const int art_id) const;

    void print_list_newsgroups();
    void print_create_newsgroup();
    void print_delete_newsgroup();
    void print_list_articles();
    void print_create_article();
    void print_delete_article();
    void print_get_article();
    
    void write_int(const int) const;
    void write_string(const std::string&) const;
    void exit();

    char read_byte();
    int read_num_p();
    std::string read_string_p();
    int read_num();
    void reset_response_pointer();

    bool active = true;
    std::ostream& os;
    const Connection& conn;
    int selected_newsgroup = -1;
    
    std::string response;
    int resp_p; // response pointer
};

#endif
