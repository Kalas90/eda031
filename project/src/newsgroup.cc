#include "newsgroup.h"
#include "newsgroupprovider.h"
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

Newsgroup::Newsgroup(string name, unsigned id) : id(id), name(name) {
    count++;
}

unsigned int Newsgroup::get_id() const { return id; }

string Newsgroup::get_name() const {return name; }

Article Newsgroup::get_article(unsigned int id) const {
    auto it = find_if(articles.begin(), articles.end(), [id](const Article& a){return a.get_id() == id;});
    if (it != articles.end()) {
        return *it;
    } else {
        throw MissingArticleException(); //invalid_argument("Article ID does not exist!");
    }
}

vector<Article> Newsgroup::get_articles() const {
    return articles;
}

bool Newsgroup::create_article(string author, string title, string text, int id) {
    articles.push_back(Article(author, title, text, id));
    return true;
}

bool Newsgroup::delete_article(unsigned int id) {
    auto it = find_if(articles.begin(), articles.end(), [&id](const Article& a){return a.get_id() == id;});
    if (it != articles.end()) {
        articles.erase(it);
        return true;
    } else {
        throw MissingArticleException();
    }
}
