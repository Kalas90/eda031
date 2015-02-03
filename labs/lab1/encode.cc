#include <iostream>
#include <fstream>
#include <string>

#include "coding.h"
#include "coding.cc"

using namespace std;

#define FILE string("file")

int main (int argc, char* argv[]) {
    string in = FILE, out = FILE + ".enc";
    if (argc > 1) {
        in = argv[1];
        out = in + ".enc";
    }

    ifstream inFile(in);
    ofstream outFile(out);
    char c;

    if (!inFile)
        cerr << "File " << in << " not found!" << endl;

    if (!outFile)
        cerr << "File " << out << " not found!" << endl;

    while (inFile.get(c)) {
        outFile.put(encode(c));
    }

    inFile.close();
    outFile.close();

}