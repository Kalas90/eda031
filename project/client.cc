#include "newsclient.h"
#include "malformattedinputexception.h"
#include "malformattedresponseexception.h"
#include "connectionclosedexception.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Usage: client host-name port-number" << endl;
		exit(1);
	}
	
	int port = -1;
	try {
		port = stoi(argv[2]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}
	
  
	Connection conn(argv[1], port);
  if (!conn.isConnected()) {
		cerr << "Connection attempt failed" << endl;
		exit(1);
	}
  
  cout << "Successfully connected to " << argv[1] << ":" << argv[2] << endl;

  NewsClient nc(cout, conn);
  nc.print_help();
  nc.print_prompt();
    
	// Wait for and handle commands
  try {
    string input;
    while(getline(cin, input)) {
      try {
        nc.parse_and_execute_command(input);
      } catch (MalformattedInputException& e) {
        cout << "That is not a valid command." << endl;
        nc.print_prompt();
        continue;
      }

      if (nc.wait_for_response()) {
        try {
          nc.receive();
          nc.print_response();
        } catch (MalformattedResponseException& e) {
          cout << "Server sent a bad response. Unable to display result." << endl;
        }
      } 

      if (nc.should_exit()) {
        break;
      }
      
      nc.print_prompt();
    }
  } catch (ConnectionClosedException& e) {
    cerr << "The server unexpectedly closed the connection" << endl;
    exit(1);
  }

  cout << "Program closed." << endl;
  exit(0);
}

