#include "newsclient.h"
#include "protocol.h"
#include "malformattedinputexception.h"
#include "malformattedresponseexception.h"
#include <iostream>
#include <stdexcept>
#include <utility>

void NewsClient::receive() {
  char ch;
  while ((ch = conn.read()) != Protocol::ANS_END) {
    response += ch;
  }
}

void bad_resp() {
  std::cout << "Bad response!" << std::endl;
}

void NewsClient::reset_response_pointer() {
  resp_p = 0;
}

int NewsClient::read_num() {
	unsigned char byte1 = response[resp_p++];
	unsigned char byte2 = response[resp_p++];
	unsigned char byte3 = response[resp_p++];
	unsigned char byte4 = response[resp_p++];
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

char NewsClient::read_byte() {
  return response[resp_p++];
}

int NewsClient::read_num_p() {
  if (read_byte() == Protocol::PAR_NUM)
    return read_num();
  else
    throw MalformattedResponseException();
}

std::string NewsClient::read_string_p() {
  if (read_byte() == Protocol::PAR_STRING) {
    int size = read_num();

    std::string s;
    for (int i = 0; i < size; ++i) {
      s += read_byte();
    }

    return s;
  } else {
    throw MalformattedResponseException();
  }
}
 

void NewsClient::print_response() {
  reset_response_pointer();

  switch (read_byte()) {
    case Protocol::ANS_LIST_NG:
      print_list_newsgroups();
      break;
    case Protocol::ANS_CREATE_NG:
      print_create_newsgroup();
      break;
    case Protocol::ANS_DELETE_NG:
      print_delete_newsgroup();
      break;
    case Protocol::ANS_LIST_ART:
      print_list_articles();
      break;
    case Protocol::ANS_CREATE_ART:
      print_create_article();
      break;
    case Protocol::ANS_DELETE_ART:
      print_delete_article();
      break;
    case Protocol::ANS_GET_ART:
      print_get_article();
      break;
    default:
      bad_resp();
      return;
  }
}

void NewsClient::print_list_newsgroups() {
  int n_newsgroups = read_num_p();
  for (int i = 0; i < n_newsgroups; ++i)
    os << read_num_p() << ". " << read_string_p() << std::endl;
}

void NewsClient::print_create_newsgroup() {
  if (read_byte() == Protocol::ANS_ACK)
    os << "Successfully created newsgroup!" << std::endl;
  else
    os << "Failed to create newsgroup: a newsgroup with that name already exists." << std::endl;
}

void NewsClient::print_delete_newsgroup() {
  if (read_byte() == Protocol::ANS_ACK)
    os << "Successfully deleted newsgroup!" << std::endl;
  else
    os << "Failed to delete newsgroup: no newsgroup with that name exists." << std::endl;
}

void NewsClient::print_list_articles() {
  if (read_byte() == Protocol::ANS_ACK) {
    int n_articles = read_num_p();
    for (int i = 0; i < n_articles; ++i)
      os << read_num_p() << ". " << read_string_p() << std::endl;
  } else {
    os << "The selected newsgroup does not exist." << std::endl;
  }
}

void NewsClient::print_delete_article() {
  if (read_byte() == Protocol::ANS_ACK) {
    os << "Successfully deleted article!" << std::endl;
  } else {
    if (read_byte() == Protocol::ERR_NG_DOES_NOT_EXIST)
      os << "Failed to delete article: the selected newsgroup does not exist." << std::endl;
    else
      os << "Failed to delete article: the article does not exist." << std::endl;
  }
}

void NewsClient::print_create_article() {
  if (read_byte() == Protocol::ANS_ACK)
    os << "Successfully created article!" << std::endl;
  else
    os << "Failed to create article: the selected newsgroup does not exist." << std::endl;
}

void NewsClient::print_get_article() {
  if (read_byte() == Protocol::ANS_ACK) {
    os << "Title:  " << read_string_p() << std::endl;
    os << "Author: " << read_string_p() << std::endl;
    os << "Text:   " << read_string_p() << std::endl;
  } else {
    if (read_byte() == Protocol::ERR_NG_DOES_NOT_EXIST)
      os << "Failed to delete article: the selected newsgroup does not exist." << std::endl;
    else
      os << "Failed to delete article: the article does not exist." << std::endl;
  }
}

size_t find_citation_mark(const std::string& s, size_t start) {
  size_t pos = s.find("\"", start);
  while (pos > 0 && s[pos-1] == '\\')
    pos = s.find("\"", pos+1);
  return pos;
}

std::string unescape_citation_marks(const std::string& s) {
  std::string unescaped = s;
  for (size_t i = 0; i < s.size()-1; ++i) {
    if (s[i] == '\\' && s[i+1] == '"')
      unescaped.erase(i, 1);
  }
  return unescaped;
}

/*
 * Read a word from a string. If encapsulated in citation marks, return the 
 * encapsulated content. Also returns the start position of the next word.
 */
std::pair<std::string, size_t> read_word(const std::string s, size_t start) {
  // Make sure we have an even number of citation marks from start to end
  int marks = 0;
  for (size_t i = start; i < s.size(); ++i) {
    if (i == 0 && s[i] == '"') {
      ++marks;
    } else {
      if (s[i] == '"' && s[i-1] != '\\')
        ++marks;
    }
  }
  
  if (marks%2 != 0)
    throw MalformattedInputException();

  // Find opening and closing citation marks
  size_t open = find_citation_mark(s, start);
  size_t close = find_citation_mark(s, (open == std::string::npos) ? start+1 : open+1);

  std::string name;
  if (open != start) {
    // Not a multiword, return what's between start and (the first space token || end of line)
    close = s.find(" ", start+1);
    if (close == std::string::npos)
      return std::make_pair(unescape_citation_marks(s.substr(start, s.size() - start)), s.size());
    else
      return std::make_pair(unescape_citation_marks(s.substr(start, close - start)), close + 1);
  } else if (open == start && close != std::string::npos) {
    // A multiword, return what's between the citation marks
    return std::make_pair(unescape_citation_marks(s.substr(open + 1, close - open - 1)), close + 2);
  } else {
    throw MalformattedInputException();
  }
}

void bad_req() {
  std::cerr << "Malformatted request." << std::endl;
}

void NewsClient::parse_and_execute_command(const std::string& input) {
  try {
    if (input == "exit") { 
      exit();

    } else if (selected_newsgroup < 0 && input.find("list newsgroups") == 0) {
      send_list_newsgroups();

    } else if (selected_newsgroup < 0 && input.find("create newsgroup ") == 0) {
      auto word = read_word(input, 17);

      if (word.first.size() < 1) 
        throw MalformattedInputException();
      else
        send_create_newsgroup(word.first);

    } else if (selected_newsgroup < 0 && input.find("delete newsgroup ") == 0) {
      auto word = read_word(input, 17);

      int ng_id;
      try {
        ng_id = std::stoi(word.first);
      } catch(std::exception& e) {
        throw MalformattedInputException();
      }
      
      send_delete_newsgroup(ng_id);
    
    } else if (selected_newsgroup < 0 && input.find("select newsgroup ") == 0) {
      auto word = read_word(input, 17);

      try {
        selected_newsgroup = std::stoi(word.first);
      } catch(std::exception& e) {
        throw MalformattedInputException();
      }

      os << "Selected newsgroup " << selected_newsgroup << "." << std::endl;
      print_help();

    } else if (selected_newsgroup > 0 && input.find("leave newsgroup") == 0) {
      os << "Exited newsgroup " << selected_newsgroup << "." << std::endl;
      selected_newsgroup = -1;
      print_help();

    } else if (selected_newsgroup > 0 && input.find("create article ") == 0) {
      auto title = read_word(input, 15);
      auto author = read_word(input, title.second);
      auto text = read_word(input, author.second);
     
      if (title.first.size() < 1 || author.first.size() < 1 || text.first.size() < 1)
        throw MalformattedInputException();

      send_create_article(selected_newsgroup, title.first, author.first, text.first);

    } else if (selected_newsgroup > 0 && input.find("delete article ") == 0) {
      auto word = read_word(input, 15);

      int art_id;
      try {
        art_id = std::stoi(word.first);
      } catch(std::exception& e) {
        throw MalformattedInputException();
      }
      
      send_delete_article(selected_newsgroup, art_id);

    } else if (selected_newsgroup > 0 && input.find("read article ") == 0) {
      auto word = read_word(input, 13);

      int art_id;
      try {
        art_id = std::stoi(word.first);
      } catch(std::exception& e) {
        throw MalformattedInputException();
      }
      
      send_get_article(selected_newsgroup, art_id);

    } else if (selected_newsgroup > 0 && input.find("list articles") == 0) {
      send_list_articles(selected_newsgroup);

    } else {
      throw MalformattedInputException();
    }
  } catch(MalformattedInputException e) {
    bad_req();
    return;
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

bool NewsClient::wait_for_response() const {
  return active;
}
 
void NewsClient::exit() {
  active = false;
}

void NewsClient::print_help() const {
  using namespace std;
  if (selected_newsgroup < 0) {
    os << "The following commands are available:" << endl;
    os << "  list newsgroups" << endl;
    os << "      Lists all the newsgroups." << endl;
    os << "  create newsgroup <name>" << endl;
    os << "      Create a newsgroup with the name <name>." << endl;
    os << "      If it's a multiword name, encapsulate it with citation marks. " << endl;
    os << "      Escape citation marks with \\\"" << endl;
    os << "  delete newsgroup <id>" << endl;
    os << "      Delete the newsgroup with identification number <id>." << endl;
    os << "  select newsgroup <id>" << endl;
    os << "      Select the newsgroup with identification number <id> as the active newsgroup." << endl;
    os << "  exit" << endl;
    os << "      Close the program." << endl;
  } else {
    os << "The follow commands are available:" << endl;
    os << "  list articles" << endl;
    os << "      Lists all the articles in the newsgroup " << selected_newsgroup << "." << endl;
    os << "  read article <id>" << endl;
    os << "      Read the article with identification number <id>." << endl;
    os << "  create article <title> <author> <text>" << endl;
    os << "      Create a new article in the newsgroup " << selected_newsgroup << "." << endl;
    os << "      If you have multiword arguments, encapsulate them with citation marks. " << endl;
    os << "      Escape citation marks with \\\"" << endl;
    os << "  delete article <id>" << endl;
    os << "      Delete the article with identification number <id>." << endl;
    os << "  leave newsgroup" << endl;
    os << "      Leave the newsgroup. This enables you to select another newsgroup." << endl;
    os << "  exit" << endl;
    os << "      Close the program." << endl;
  }
}

void NewsClient::print_prompt() const { 
  if (selected_newsgroup > 0)
    os << "newsgroup " << selected_newsgroup << ">";
  else
    os << ">";
}
