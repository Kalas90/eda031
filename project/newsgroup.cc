#include "newsgroup.h"
#include "article.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>

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

Article Newsgroup::get_article(unsigned int id) const {
    auto it = find_if(articles.begin(), articles.end(), [&id](const Article& a){return a.get_id() == id;});
    if (it != articles.end()) {
        return *it;
    } else {
        throw invalid_argument("Article ID does not exist!");
    }
}

vector<Article> Newsgroup::get_articles() const {
    return articles;
}

void Newsgroup::create_article(string author, string title, string text) {
    articles.push_back(Article(author, title, text));
}

void Newsgroup::delete_article(unsigned int id) {
    auto it = find_if(articles.begin(), articles.end(), [&id](const Article& a){return a.get_id() == id;});
    if (it != articles.end()) {
        articles.erase(it);
    } else {
        throw invalid_argument("Article ID does not exist!");
    }
}
