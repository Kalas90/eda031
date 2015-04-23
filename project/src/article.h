#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

class Article {
public:
    Article();
    Article(std::string, std::string, std::string, int article_id = -1);

    std::string get_author() const;
    std::string get_title() const;
    std::string get_text() const;
    unsigned int get_id() const;

private:
    static unsigned int count;
    std::string author;
    std::string title;
    std::string text;
    unsigned int id;
};


#endif
