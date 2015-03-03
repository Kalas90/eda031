#include <iostream>
#include <cstdlib>

#include "database.h"

// deal with it
#define TRY_PRINT_ERRORS(stmts) try { \
        stmts; \
    } catch (DatabaseError e) { \
        cerr << e.what() << endl; \
        exit(1); \
    } \

using namespace std;

int main()
{
    cout << "-- begin glorious test program" << endl;

    cout << "-- seeding database" << endl;
    system("./mktestdb.sh");

    NewsgroupDatabase db("db.sqlite3");
    vector<string> newsgroups;

    cout << "-- select * from newsgroups" << endl;

    TRY_PRINT_ERRORS(
        newsgroups = db.list_newsgroups();
    );

    for (string ng : newsgroups)
        cout << ng << endl;

    cout << endl;
    cout << "-- delete from newsgroups with owned articles" << endl;

    TRY_PRINT_ERRORS(
       db.remove_newsgroup(1);
    )
    cout << "-- select remaining" << endl;
    TRY_PRINT_ERRORS(
        newsgroups = db.list_newsgroups();
    );
    for (string ng : newsgroups)
        cout << ng << endl;

    cout << "-- all done gg" << endl;

    return 0;
}
