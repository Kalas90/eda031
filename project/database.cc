#include "database.h"

using namespace std;

static int read_newsgroup_list(vector<Newsgroup>*vector_ptr, int argc, char **argv, 
        char **col_name);
static int read_article_list(vector<Article> *vector_ptr, int argc, char **argv, 
        char **col_name);
static int read_article(Article *article_ptr, int argc, char **argv, char **col_name);

// Instantiate templates that we need.

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
    Article out_article;
    perform_query(sql, read_article, &out_article);
    return out_article;
}

vector<Newsgroup> NewsgroupDatabase::list_news_groups() const
{
    const char *sql = "select id, name from newsgroups";
    vector<Newsgroup> newsgroup_list;
    perform_query(sql, read_newsgroup_list, &newsgroup_list);
    return newsgroup_list;
}

vector<Article> NewsgroupDatabase::list_articles(unsigned newsgroup_id)
{
    const string id_str = to_string(newsgroup_id);
    const string sql =
        string("select id, title, author, content from articles where newsgroup_id = ") 
        + id_str;
    vector <Article> article_list;
    perform_query(sql, read_article_list, &article_list);
    return article_list;
}

bool NewsgroupDatabase::remove_newsgroup(unsigned id)
{
    const string id_str = to_string(id);
    const string delete_newsgroups = 
        string("delete from newsgroups where id = ") + id_str;
    const string delete_articles =
        string("delete from articles where newsgroup_id = ") + id_str;

    try {
        int affected;
        begin_transaction();
        affected = perform_query(delete_newsgroups);
        if (0 != affected) 
            perform_query(delete_articles);
        commit_transaction();
        return 0 != affected;
    } catch (DatabaseError ex) {
        rollback_transaction();
        throw;
    }
}

bool NewsgroupDatabase::remove_article(unsigned newsgroup_id, unsigned article_id)
{
    const string article_id_str = to_string(article_id);
    const string newsgroup_id_str = to_string(newsgroup_id);
    const string delete_article =
        string("delete from articles where id = ") + article_id_str + 
        string(" and newsgroup_id = ") + newsgroup_id_str;

    return 0 != perform_query(delete_article);
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

void NewsgroupDatabase::begin_transaction()
{
    sqlite3_exec(db, "begin", nullptr, nullptr, nullptr);
}

void NewsgroupDatabase::commit_transaction()
{
    sqlite3_exec(db, "commit", nullptr, nullptr, nullptr);
}

void NewsgroupDatabase::rollback_transaction()
{
    sqlite3_exec(db, "rollback", nullptr, nullptr, nullptr);
}

static int read_newsgroup_list(vector<Newsgroup> *vector_ptr, int argc, char **argv, 
        char **col_name)
{
    // select id, name from newsgroups
    (void) argc; (void) col_name;
    unsigned id = stoi(string(argv[0]));
    string name(argv[1]);
    Newsgroup ng(name, id);
    vector_ptr->push_back(ng);
    return 0;
}

static int read_article_list(vector<Article> *vector_ptr, int argc, char **argv, 
        char **col_name)
{
    // select id, title, author, content from articles
    (void) argc; (void) col_name;
    string title(argv[1]);
    string author(argv[2]);
    string content(argv[3]);
    Article article(author, title, content);
    vector_ptr->push_back(article);
    return 0;
}

static int read_article(Article *article_ptr, int argc, char **argv, char **col_name)
{
    // select id, title, content, author from articles limit 1
    (void) argc; (void) col_name;
    string title(argv[1]);
    string author(argv[2]);
    string content(argv[3]);
    *article_ptr = Article(author, title, content);
    return 0;
}

