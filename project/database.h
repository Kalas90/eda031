#ifndef DATABASE_H 
#define DATABASE_H

#include <stdexcept>
#include <vector>

#include <sqlite3.h>

class NewsgroupDatabase {
    public:
    NewsgroupDatabase(std::string filename);
    ~NewsgroupDatabase();
    std::vector<std::string> list_newsgroups();

    protected:
    sqlite3 *db;
};

class DatabaseError : public std::runtime_error {
    public:
    DatabaseError(std::string message) : std::runtime_error(message) {}
};

#endif // defined DATABASE_H 
