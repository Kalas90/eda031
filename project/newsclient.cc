#include "newsclient.h"
#include "protocol.h"
#include <iostream>

void NewsClient::print_help() const {
  using namespace std;
  os << "The following commands are available:" << endl;
  os << "  list newsgroups" << endl;
  os << "      Lists all the news groups." << endl;
  os << "  create newsgroup <name>" << endl;
  os << "      Create a newsgroup with the name <name>." << endl;
  os << "      If it's a multiword name, encapsulate it in citation marks. " << endl;
  os << "      Escape citation marks with \\\"" << endl;
  os << "  delete newsgroup <id>" << endl;
  os << "      Delete the newsgroup with identification number <id>." << endl;
  os << "  select newsgroup <id>" << endl;
  os << "      Select the newsgroup with identification number <id> as the active newsgroup." << endl;
  os << "  exit" << endl;
  os << "      Close the program." << endl;
}

void NewsClient::print_prompt() const { 
  os << client;
}

size_t find_citation_mark(const std::string& s, size_t start) {
  size_t pos = s.find("\"", start);
  while (pos > 0 && s[pos-1] == '\\')
    pos = s.find("\"", pos+1);
  return pos;
}

void bad_req() {
  std::cerr << "Malformatted request." << std::endl;
}

void NewsClient::parse_and_execute_command(const std::string& input) {
  if (input == "exit") { 
    exit();
  } else if (input == "list newsgroups") { 
    send_list_newsgroups();
  } else if (input.find("create newsgroup ") == 0) {
    auto first = std::string("create newsgroup ").size();
    auto last = input.size()-1;

    size_t open = find_citation_mark(input, first);
    size_t close = find_citation_mark(input, open+1);

    bool correct = true;
    std::string name;
    if (open == std::string::npos && close == std::string::npos) {
      name = input.substr(first, input.size() - first);
    } else if (open == first && close == last) {
      name = input.substr(first+1, input.size() - (first+1) - 1);
    } else {
      correct = false;
    }

    if (name.size() < 1) 
      correct = false;

    if (correct)
      send_create_newsgroup(name);
    else
      bad_req();
  } else if (input.find("delete newsgroup ") == 0) {
        
  } else {
    bad_req();
  }
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
