#include <string>
#include "article.h"

using namespace std;

unsigned int Article::count;

Article::Article() {}

Article::Article(string author, string title, string text) : author(author), title(title), text(text) {
    id = ++count;
}

string Article::get_author() const { return author; }

string Article::get_title() const { return title; }

string Article::get_text() const { return text; }

unsigned int Article::get_id() const { return id; }
