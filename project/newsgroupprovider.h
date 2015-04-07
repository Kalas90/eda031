#ifndef NEWSGROUPPROVIDER_H
#define NEWSGROUPPROVIDER_H

#include <vector>
#include <string>
#include <stdexcept>

#include "article.h"
#include "newsgroup.h"

// Interface class, pure virtual
class NewsgroupProvider {
public:
    virtual Article article(unsigned int newsgroup_id, unsigned int article_id) const = 0;
    virtual std::vector<Newsgroup> list_news_groups() const = 0;
    virtual std::vector<Article> list_articles(unsigned int newsgroup_id) = 0;
    virtual bool remove_article(unsigned int newsgroup_id, unsigned int article_id) = 0;
    virtual bool remove_newsgroup(unsigned int newsgroup_id) = 0;
    virtual bool create_newsgroup(std::string name) = 0;
    virtual bool create_article(unsigned int newsgroup_id,
            std::string title, std::string author, std::string text) = 0;
};

class MissingArticleException : public std::runtime_error {
public:
    MissingArticleException(std::string message) : std::runtime_error(message) {}
};

class MissingNewsgroupException : public std::runtime_error {
public:
    MissingNewsgroupException(std::string message) : std::runtime_error(message) {}
};

class DuplicateNewsgroupException : public std::runtime_error {
public:
    DuplicateNewsgroupException(std::string message) : std::runtime_error(message) {}
};

class ImplementationException : public std::runtime_error {
public:
    ImplementationException(std::string message) : std::runtime_error(message) {}
};

#endif
