#include "newsclient.h"
#include "protocol.h"
#include <iostream>

void NewsClient::print_help() const {
  using namespace std;
  os << "The following commands are available:" << endl;
  os << "  list" << endl;
  os << "    Lists all the news groups." << endl;
  os << "  list ng" << endl;
  os << "    Lists all the articles in the news group ng." << endl;
  os << "  read n" << endl;
  os << "    Prints the article with identification number n." << endl;
  os << "  exit" << endl;
  os << "    Close the program." << endl;
}

void NewsClient::print_prompt() const { 
  os << client;
}

void NewsClient::parse_and_execute_command(const std::string& input) {
  if (input.find("exit") == 0) exit();
  else if (input.find("list") == 0) send_list_newsgroups();
}

void NewsClient::write_string(const std::string& s) const {
  write_int(s.size());
  for (unsigned int i = 0; i < s.size(); ++i)
    conn.write(s[i]);
}

void NewsClient::write_int(const int n) const {
	conn.write((n >> 24) & 0xFF);
	conn.write((n >> 16) & 0xFF);
	conn.write((n >> 8)	 & 0xFF);
	conn.write((n)       & 0xFF);
}

void NewsClient::send_list_newsgroups() const {
  conn.write(Protocol::COM_LIST_NG);
  conn.write(Protocol::COM_END);
}

void NewsClient::send_create_newsgroup(const std::string& title) const {
  conn.write(Protocol::COM_CREATE_NG);
  conn.write(Protocol::PAR_STRING);
  write_string(title);
  conn.write(Protocol::COM_END);
}

void NewsClient::send_delete_newsgroup(const int ng_id) const {
  conn.write(Protocol::COM_DELETE_NG);
  conn.write(Protocol::PAR_NUM);
  write_int(ng_id);
  conn.write(Protocol::COM_END);
}

void NewsClient::send_list_articles(const int ng_id) const {
  conn.write(Protocol::COM_LIST_ART);
  conn.write(Protocol::PAR_NUM);
  write_int(ng_id);
  conn.write(Protocol::COM_END);
}


void NewsClient::send_create_article(const int ng_id, const std::string& title, 
        const std::string& author, const std::string& text) const {
  conn.write(Protocol::COM_CREATE_ART);
  conn.write(Protocol::PAR_NUM);
  write_int(ng_id);
  conn.write(Protocol::PAR_STRING);
  write_string(title);
  conn.write(Protocol::PAR_STRING);
  write_string(author);
  conn.write(Protocol::PAR_STRING);
  write_string(text);
  conn.write(Protocol::COM_END);
}

void NewsClient::send_delete_article(const int ng_id, const int art_id) const {
  conn.write(Protocol::COM_DELETE_ART);
  conn.write(Protocol::PAR_NUM);
  write_int(ng_id);
  conn.write(Protocol::PAR_NUM);
  write_int(art_id);
  conn.write(Protocol::COM_END);
}

void NewsClient::send_get_article(const int ng_id, const int art_id) const {
  conn.write(Protocol::COM_GET_ART);
  conn.write(Protocol::PAR_NUM);
  write_int(ng_id);
  conn.write(Protocol::PAR_NUM);
  write_int(art_id);
  conn.write(Protocol::COM_END);
}

void NewsClient::receive() {}

void NewsClient::print_response() const {}

bool NewsClient::wait_for_response() const {
  return active;
}
 
void NewsClient::exit() {
  active = false;
}
