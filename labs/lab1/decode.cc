#include <iostream>
#include <fstream>
#include <string>

#include "coding.h"
#include "coding.cc"

using namespace std;

#define FILE "file.enc"
#define DECFILE "file.dec"

int main (int argc, char* argv[]) {
    string in = FILE, out = DECFILE;
    if (argc > 1) {
        in = argv[1];
    }
    if (argc > 2) {
        out = argv[2];
    }

    ifstream inFile(in);
    ofstream outFile(out);
    char c;

    if (!inFile)
        cerr << "File " << in << " not found!" << endl;

    if (!outFile)
        cerr << "File " << out << " not found!" << endl;

    while (inFile.get(c)) {
        outFile.put(decode(c));
    }

    inFile.close();
    outFile.close();

}