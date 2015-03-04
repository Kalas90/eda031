#include "newsclient.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Usage: myclient host-name port-number" << endl;
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
	/*
  if (!conn.isConnected()) {
		cerr << "Connection attempt failed" << endl;
		exit(1);
	}
  */
  
  cout << "Successfully connected to " << argv[1] << ":" << argv[2] << endl;

  NewsClient nc(cout, conn);
  nc.print_help();
  nc.print_prompt();
    
	// Wait for and handle commands
  string input;
  while(getline(cin, input)) {
    nc.parse_and_execute_command(input);

    if (nc.wait_for_response()) {
      nc.receive();
      nc.print_response();
      nc.print_prompt();
    } else {
      break;
    }
  }

  cout << "Program closed." << endl;
  exit(0);
}

