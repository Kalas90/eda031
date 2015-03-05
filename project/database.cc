#include "database.h"

using namespace std;

static int read_newsgroups(void *vector_ptr, int argc, char **argv, char **col_name);
static int read_article_list(void *vector_ptr, int argc, char **argv, char **col_name);
static int read_article(void *article_ptr, int argc, char **argv, char **col_name);

NewsgroupDatabase::NewsgroupDatabase(string filename)
{
    if (sqlite3_open(filename.c_str(), &db))
        throw DatabaseError(
                string("failed to open database ") + filename);
}

NewsgroupDatabase::~NewsgroupDatabase()
{
    sqlite3_close(db);
}

Article NewsgroupDatabase::article(unsigned newsgroup_id, unsigned article_id) const
{
    const string newsgroup_id_str = to_string(newsgroup_id);
    const string article_id_str = to_string(article_id);
    const string sql = 
        "select id, title, author, content from articles where newsgroup_id = " +
        newsgroup_id_str + " and id = " + article_id_str + " limit 1";
    Article out_article("", "", "");
    char *error_msg = nullptr;

    int rc = sqlite3_exec(db, sql.c_str(), read_article, &out_article, &error_msg);

    if (rc != SQLITE_OK) {
        string  error_str(error_msg);
        sqlite3_free(error_msg);
        throw DatabaseError(
                string("query fails with excuse: ") + error_str);
    } else
        return out_article;
}

vector<Newsgroup> NewsgroupDatabase::list_news_groups() const
{
    const char *sql = "select id, name from newsgroups";
    vector<Newsgroup> newsgroup_list;
    char *error_msg = nullptr;

    int rc = sqlite3_exec(db, sql, read_newsgroups, &newsgroup_list, &error_msg);

    if (rc != SQLITE_OK) {
        string error_str(error_msg);
        sqlite3_free(error_msg);
        throw DatabaseError(
                string("query fails with excuse: ") + error_str);
    } else
        return newsgroup_list;
}

vector<Article> NewsgroupDatabase::list_articles(unsigned newsgroup_id)
{
    const string id_str = to_string(newsgroup_id);
    const string sql =
        string("select id, title, author, content from articles where newsgroup_id = ") 
        + id_str;
    vector <Article> article_list;
    char *error_msg = nullptr;
    int rc;

    rc = sqlite3_exec(db, sql.c_str(), read_article_list, &article_list, &error_msg);
    if (rc != SQLITE_OK) {
        string error_str(error_msg);
        sqlite3_free(error_msg);
        throw DatabaseError(
                string("query fails with excuse: ") + error_str);
    } else 
        return article_list;
}

bool NewsgroupDatabase::remove_newsgroup(unsigned id)
{
    const string id_str = to_string(id);
    const string delete_articles =
        string("delete from articles where newsgroup_id = ") + id_str;
    const string delete_newsgroups = 
        string("delete from newsgroups where id = ") + id_str;
    bool status = true;
    char *error_msg = nullptr;
    int rc;

    // begin transaction
    sqlite3_exec(db, "begin", nullptr, nullptr, nullptr);

    // delete the newsgroups
    rc = sqlite3_exec(db, delete_newsgroups.c_str(), nullptr, nullptr, &error_msg);
    if (rc != SQLITE_OK)
        goto fail;

    // did we actually delete anything? if not, give up
    if (0 == sqlite3_changes(db)) {
        status = false;
        goto exit;
    }

    // delete owned articles
    rc = sqlite3_exec(db, delete_articles.c_str(), nullptr, nullptr, &error_msg);
    if (rc != SQLITE_OK)
        goto fail;
exit:
    // we done. commit, deallocate and exit
    sqlite3_exec(db, "commit", nullptr, nullptr, nullptr);
    sqlite3_free(error_msg);
    return status;

fail:
    // we messed up. rollback, deallocate and throw 
    sqlite3_exec(db, "rollback", nullptr, nullptr, nullptr);
    string error_str(error_msg);
    sqlite3_free(error_msg);
    throw DatabaseError(
            string("query fails with excuse: ") + error_str);
}

bool NewsgroupDatabase::remove_article(unsigned newsgroup_id, unsigned article_id)
{
    (void) newsgroup_id; (void) article_id;
    throw DatabaseError(string("remove_article: Not implemented."));
}

bool NewsgroupDatabase::create_newsgroup(string name)
{
    (void) name;
    throw DatabaseError(string("create_newsgroup: Not implemented."));
}

bool NewsgroupDatabase::create_article(unsigned newsgroup_id, string title, 
                                       string author, string text)
{
    (void) newsgroup_id; (void) title; (void) author; (void) text;
    throw DatabaseError(string("create_article: Not implemented."));
}

static int read_newsgroups(void *vector_ptr, int argc, char **argv, char **col_name)
{
    (void) argc; (void) col_name;
    vector<Newsgroup> *output_vector = static_cast<vector<Newsgroup>*>(vector_ptr);
    unsigned id = stoi(string(argv[0]));
    string name(argv[1]);
    Newsgroup ng(name, id);
    output_vector->push_back(ng);
    return 0;
}

static int read_article_list(void *vector_ptr, int argc, char **argv, char **col_name)
{
    (void) argc; (void) col_name;
    vector<Article> *output_vector = static_cast<vector<Article>*>(vector_ptr);
    // id is 0
    string title(argv[1]);
    string author(argv[2]);
    string content(argv[3]);
    // author, title, content
    Article article(author, title, content);
    output_vector->push_back(article);
    return 0;
}

static int read_article(void *article_ptr, int argc, char **argv, char **col_name)
{
    (void) argc; (void) col_name;
    Article *article = static_cast<Article*>(article_ptr);
    // id is 0
    string title(argv[1]);
    string author(argv[2]);
    string content(argv[3]);
    // author, title, content
    *article = Article(author, title, content);
    return 0;
}

