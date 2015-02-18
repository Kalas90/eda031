#include "tagremover.h"
#include <iostream>
#include <regex>
using namespace std;

TagRemover::TagRemover(istream &in)
{
    string tmp;
    while (getline(in, tmp)) {
        str += tmp;
        str += "\n";
    }

    string::size_type s1, s2 = 0;

    // remove tags

    while (str.find("<", s1) != string::npos) {
        s1 = str.find("<", s1);
        s2 = str.find(">", s1);
        str.erase(s1, s2-s1+1);
    }

    while (str.find("&lt;") != string::npos) {
        str.replace(str.find("&lt;"), 4, "<");
    }

    while (str.find("&gt;") != string::npos) {
        str.replace(str.find("&gt;"), 4, ">");
    }

    while (str.find("&amp;") != string::npos) {
        str.replace(str.find("&amp;"), 5, "&");
    }

    while (str.find("&nbsp;") != string::npos) {
        str.replace(str.find("&nbsp;"), 6, " ");
    }


}

void TagRemover::print(ostream &out)
{
    out << str << endl;
}