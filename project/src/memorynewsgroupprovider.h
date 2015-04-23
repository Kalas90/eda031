#ifndef MEMORYNEWSGROUPPROVIDER_H
#define MEMORYNEWSGROUPPROVIDER_H

#include "newsgroupprovider.h"
#include "newsgroup.h"
#include "article.h"

class MemoryNewsgroupProvider : public NewsgroupProvider {
public:
    virtual Article article(unsigned int newsgroup_id, unsigned int article_id) const override;
    virtual std::vector<Newsgroup> list_news_groups() const override;
    virtual std::vector<Article> list_articles(unsigned int newsgroup_id) override;
    virtual bool remove_article(unsigned int newsgroup_id, unsigned int article_id) override;
    virtual bool remove_newsgroup(unsigned int newsgroup_id) override;
    virtual bool create_newsgroup(std::string name) override;
    virtual bool create_article(unsigned int newsgroup_id,
            std::string title, std::string author, std::string text, int article_id = -1) override;
private:
    std::vector<Newsgroup> news;
    Newsgroup& newsgroup(unsigned int newsgroup_id);
};

#endif
