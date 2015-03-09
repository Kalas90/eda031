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

    {
        cout << endl << "-- select * from newsgroups" << endl;

        vector<Newsgroup> newsgroups;

        TRY_PRINT_ERRORS(
            newsgroups = db.list_news_groups();
        );

        for (Newsgroup ng : newsgroups)
            cout << ng.get_name() << endl;
    }

    {
        cout << endl << "-- select article names for first newsgroup" << endl;

        vector<Article> article_names;

        TRY_PRINT_ERRORS(
            article_names = db.list_articles(1);
        );

        for (Article a : article_names)
            cout << a.get_title() << endl;
    }

    {
        cout << endl << "-- select single article" << endl;

        Article article("","","");

        TRY_PRINT_ERRORS(
            article = db.article(1, 1);
        );

        cout << article.get_author() << endl;
    }

    {
        cout << endl << "-- delete that article" << endl;

        Article article;

        TRY_PRINT_ERRORS(
            db.remove_article(1, 1);
        );

        cout << "-- try to select deleted article" << endl;

        TRY_PRINT_ERRORS(
            article = db.article(1, 1);
        );

        // We need a better way to say "no such article".
        
        vector<Article> articles;

        TRY_PRINT_ERRORS(
            articles = db.list_articles(1);
        );

        cout << "there are now " + to_string(articles.size()) << 
            " articles left in the newsgroup" << endl;
    }

    {
        cout << endl << "-- delete from newsgroups with owned articles" << endl;

        vector<Newsgroup> newsgroups;

        TRY_PRINT_ERRORS(
            db.remove_newsgroup(2);
        );

        cout << "-- select remaining" << endl;

        TRY_PRINT_ERRORS(
            newsgroups = db.list_news_groups();
        );

        for (Newsgroup ng : newsgroups)
            cout << ng.get_name() << endl;
    }

    {
        cout << endl << "-- create newsgroup" << endl;

        TRY_PRINT_ERRORS(
            db.create_newsgroup(string("alt.binaries"));
        );

        cout << "-- select newsgroups after insert" << endl;

        vector<Newsgroup> newsgroups;

        TRY_PRINT_ERRORS(
            newsgroups = db.list_news_groups();
        );

        for (Newsgroup ng : newsgroups)
            cout << ng.get_name() << endl;
    }

    {
        cout << endl << "-- create article" << endl;

        TRY_PRINT_ERRORS(
            db.create_article(
                3,
                string("my little article"),
                string("jforberg@yeah.com"),
                string("hey guys"));
        );

        cout << "-- select articles in newsgroup after insert" << endl;

        vector<Article> articles;

        TRY_PRINT_ERRORS(
            articles = db.list_articles(3);
        );

        for (auto a : articles)
            cout << a.get_title() << endl;
    }

    cout << endl << "-- all done gg" << endl;

    return 0;
}
