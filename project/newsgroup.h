#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include "article.h"
#include "executionresult.h"

class Newsgroup {
public:

    Newsgroup(std::string);
    Newsgroup(std::string, unsigned);

    std::string get_name() const;
    unsigned int get_id() const;

    std::Pair<ExRes, Article> get_article(unsigned int) const;
    std::vector<Article> get_articles() const;
    ExRes create_article(std::string, std::string, std::string);
    ExRes delete_article(unsigned int);


private:
    static unsigned int count;
    unsigned int id;
    std::string name;
    std::vector<Article> articles;
};


#endif
