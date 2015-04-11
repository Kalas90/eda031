#include "database.h"

using namespace std;

static int read_newsgroup_list(vector<Newsgroup>*, int, char**, char**);
static int read_article_list(vector<Article>*, int, char**, char**);
static int read_article(Article*, int, char**, char**);
static int read_unsigned_maybe(unsigned*, int, char**, char**);

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
    if (out_article.get_title() == string())
        throw MissingArticleException(
                string("No article with newsgroup_id = ") + to_string(newsgroup_id) +
                string(", article_id = ") + to_string(article_id));
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
    const string count_sql = 
        string("select 1 from newsgroups where id = ") + id_str;
    const string select_sql =
        string("select id, title, author, content from articles where newsgroup_id = ") 
        + id_str;
    unsigned exists = 0;

    perform_query(count_sql, read_unsigned_maybe, &exists);
    if (!exists)
        throw MissingNewsgroupException(
                string("No newsgroup with id = ") + id_str);

    vector <Article> article_list;
    perform_query(select_sql, read_article_list, &article_list);

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
    const string select_last_id = 
        string("select max(id) from newsgroups");
    unsigned id = 0;

    perform_query(select_last_id, read_unsigned_maybe, &id);

    id++;

    const string id_str = to_string(id);
    const string insert_newsgroup =
        string("insert into newsgroups (id, name) values ('")
        + id_str + string("', '") + name + string("')"); 

    try {
        perform_query(insert_newsgroup);
    } catch (DatabaseError &e) {
        if (string(e.what()).find("UNIQUE") != string::npos)
            throw DuplicateNewsgroupException(
                    string("Duplicate name for newsgroup '") +
                    name + string("'"));
        else
            throw;
    }
    return true;
}

bool NewsgroupDatabase::create_article(unsigned newsgroup_id, string title, 
                                       string author, string text)
{
    const string select_last_id = 
        string("select max(id) from articles");
    unsigned article_id = 0;

    perform_query(select_last_id, read_unsigned_maybe, &article_id);

    article_id++;

    const string article_id_str = to_string(article_id);
    const string newsgroup_id_str = to_string(newsgroup_id);
    const string insert_article =
        string("insert into articles (id, newsgroup_id, title, author, content) values ('")
        + article_id_str + string("', '") 
        + newsgroup_id_str + string("', '") 
        + title + string("', '") 
        + author + string("', '") 
        + text + string("')");

    perform_query(insert_article);
    return true;
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

static int read_unsigned_maybe(unsigned *uns_ptr, int argc, char **argv, char **col_name)
{
    // select [some integer] limit 1
    (void) argc; (void) col_name;
    if (!argv[0])
        return 0;
    *uns_ptr = stoi(string(argv[0]));
    return 0;
}

