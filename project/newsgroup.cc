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

bool Newsgroup::delete_article(unsigned int id) {
    auto it = find_if(articles.begin(), articles.end(), [&id](const Article& a){return a.get_id() == id;});
    if (it != articles.end()) {
        articles.erase(it);
        return true;
    } else {
        throw invalid_argument("Article ID does not exist!");
        return false;
    }
}
