#include "tagremover.h"

#include <iostream>
#include <fstream>


#define HTML string("test.html")

using namespace std;

int main() {

    ifstream in(HTML);

    TagRemover tr(in);
    tr.print(cout);
}
