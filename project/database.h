#ifndef DATABASE_H 
#define DATABASE_H

#include <stdexcept>
#include <vector>

#include "sqlite3.h"
#include "newsgroupprovider.h"
#include "newsgroup.h"
#include "article.h"

class DatabaseError : public std::runtime_error {
public:
    DatabaseError(std::string message) : std::runtime_error(message) {}
};

class NewsgroupDatabase : public NewsgroupProvider {
private:
    sqlite3 *db;

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
    void begin_transaction();
    void commit_transaction();
    void rollback_transaction();
    // By reading past this line, you confirm that you are 18+ and not 
    // easily offended by ugly code.

    template<typename t> unsigned perform_query(const std::string &query,
            int (*func)(t*, int, char**, char**), t *data) const 
    {
        char *error_msg = nullptr;
        int (*unsafe_func)(void*, int, char**, char**) = 
                    reinterpret_cast<int (*)(void*, int, char**, char**)>(func);
        void *unsafe_data = static_cast<void*>(data);

        int rc = sqlite3_exec(db, query.c_str(), unsafe_func, unsafe_data, &error_msg);

        if (rc != SQLITE_OK) {
            std::string error_str(error_msg);
            sqlite3_free(error_msg);
            throw DatabaseError(
                    std::string("query fails with excuse: ") + error_str);
        } else {
            return sqlite3_changes(db);
        }
    }

    unsigned perform_query(const std::string &query) const
    {
        return perform_query<void*>(query, nullptr, nullptr);
    }
};

#endif // defined DATABASE_H 
