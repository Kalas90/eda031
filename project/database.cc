#include "database.h"

using namespace std;

static int read_newsgroups(void *vector_ptr, int argc, char **argv, char **col_name);

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

vector<string> NewsgroupDatabase::list_newsgroups()
{
    const char *sql = "select name from newsgroups";
    vector<string> newsgroups;
    char *error_msg = nullptr;

    int rc = sqlite3_exec(db, sql, read_newsgroups, &newsgroups, &error_msg);

    if (rc != SQLITE_OK) {
        string error_str(error_msg);
        sqlite3_free(error_msg);
        throw DatabaseError(
                string("query fails with excuse: ") + error_str);
    } else
        return newsgroups;
}

bool NewsgroupDatabase::remove_newsgroup(int id)
{
    const string id_str = to_string(id);
    char *error_msg = nullptr;
    bool status = true;
    const string delete_articles =
        string("delete from articles where newsgroup_id = ") + id_str;
    const string delete_newsgroups = 
        string("delete from newsgroups where id = ") + id_str;
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

static int read_newsgroups(void *vector_ptr, int argc, char **argv, char **col_name)
{
    vector<string> *output_vector = static_cast<vector<string>*>(vector_ptr);

    for (int i = 0; i < argc; i++)
        output_vector->push_back(argv[i]);
    return 0;
}
