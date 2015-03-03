#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>
#include <vector>
#include <iostream>
#include "article.h"

class Newsgroup {
public:

    Newsgroup(std::string);

    std::string get_name() const;
    unsigned int get_id() const;

    //Article get_article(unsigned int) const;
    void list_articles(std::ostream&) const;
    void create_article(std::string, std::string, std::string);
    void delete_article(unsigned int);


private:
    static unsigned int count;
    unsigned int id;
    std::string name;
    std::vector<Article> articles;
};


#endif