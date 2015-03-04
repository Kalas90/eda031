#ifndef NEWSGROUPPROVIDER_H
#define NEWSGROUPPROVIDER_H

#include "article.h"
#include "newsgroup.h"

#include <vector>
#include <string>

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

#endif