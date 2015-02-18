#ifndef TAG_H
#define TAG_H

#include <string>
#include <regex>
#include <iostream>


class TagRemover {
public:
    TagRemover(std::istream& in);

    void print(std::ostream& out);


private:
    std::string str;

};

#endif