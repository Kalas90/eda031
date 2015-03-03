#include "newsgroup.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

unsigned int Newsgroup::count;


Newsgroup::Newsgroup(string name) : name(name) {
    id = ++count;
}

unsigned int Newsgroup::get_id() const { return id; }


void Newsgroup::list_articles(ostream& out) const {
    out << "Id| Author | Title" << endl;
    for (auto a : articles) {
        out << a.get_id() << " | " << a.get_author() << " | " << a.get_title() << endl;
    }
}

void Newsgroup::create_article(string author, string title, string text) {
    articles.push_back(Article(author, title, text));
}

void Newsgroup::delete_article(unsigned int id) {
    auto it = find_if(articles.begin(), articles.end(), [&id](const Article& a){return a.get_id() == id;});
    if (it != articles.end()) {
        articles.erase(it);
    }
}
