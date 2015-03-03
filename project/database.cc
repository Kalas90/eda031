#include <iostream>
#include <cstdlib>

#include "database.h"

using namespace std;

static int read_newsgroups(void *vector_ptr, int argc, char **argv, char **col_name);

int main()
{
    NewsgroupDatabase db("db.sqlite3");
    vector<string> newsgroups;
    try {
        newsgroups = db.list_newsgroups();
    } catch (DatabaseError e) {
        cerr << e.what() << endl;
        exit(1);
    }

    for (string ng : newsgroups)
        cout << ng << endl;

    return 0;
}

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
    error_msg = nullptr;

    if (rc != SQLITE_OK) {
        string error_str(error_msg);
        sqlite3_free(error_msg);
        throw DatabaseError(
                string("query fails with excuse: ") + error_str);
    } else
        return newsgroups;
}

static int read_newsgroups(void *vector_ptr, int argc, char **argv, char **col_name)
{
    vector<string> *output_vector = static_cast<vector<string>*>(vector_ptr);

    for (int i = 0; i < argc; i++)
        output_vector->push_back(argv[i]);
    return 0;
}
