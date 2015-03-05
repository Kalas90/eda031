#ifndef DATABASE_H 
#define DATABASE_H

#include <stdexcept>
#include <vector>

#include <sqlite3.h>

#include "newsgroupprovider.h"
#include "newsgroup.h"
#include "article.h"

class NewsgroupDatabase : public NewsgroupProvider {
public:
    NewsgroupDatabase(std::string filename);
    ~NewsgroupDatabase();

    virtual Article article(unsigned int newgroup_id, unsigned int article_id) const override;
    virtual std::vector<Newsgroup> list_news_groups() const override;
    virtual std::vector<Article> list_articles(unsigned int newsgroup_id) override;
    virtual bool remove_article(unsigned int newsgroup_id, unsigned int article_id) override;
    virtual bool remove_newsgroup(unsigned int newsgroup_id) override;
    virtual bool create_newsgroup(std::string name) override;
    virtual bool create_article(unsigned int newsgroup_id,
            std::string title, std::string author, std::string text) override;
protected:
    sqlite3 *db;
};

class DatabaseError : public std::runtime_error {
public:
    DatabaseError(std::string message) : std::runtime_error(message) {}
};

#endif // defined DATABASE_H 
