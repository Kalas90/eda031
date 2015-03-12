#include "newsgroup.h"
#include "missingarticleexception.h"
#include "article.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <utility>

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

std::Pair<ExRes, Article> Newsgroup::get_article(unsigned int id) const {
    auto it = find_if(articles.begin(), articles.end(), [&id](const Article& a){return a.get_id() == id;});
    if (it != articles.end()) {
        return std::make_pair(ExRes::SUCCESS, *it);
    } else {
        return std::make_pair(ExRes::MISSING_ARTICLE, *it);
    }
}

std::vector<Article> Newsgroup::get_articles() const {
    return articles;
}

ExRes Newsgroup::create_article(string author, string title, string text) {
    auto it = find_if(articles.begin(), articles.end(),
            [title](Article& a){ return a.get_title() == title; } );
    if (it == articles.end()) {
        articles.push_back(Article(author, title, text));
        return ExRes::SUCCESS;
    } else {
        return ExRes::ARTICLE_ALREADY_EXIST;
    }
}

ExRes Newsgroup::delete_article(unsigned int id) {
    auto it = find_if(articles.begin(), articles.end(), [&id](const Article& a){return a.get_id() == id;});
    if (it != articles.end()) {
        articles.erase(it);
        return ExRes::SUCCESS;
    } else {
        return ExRes::MISSING_ARTICLE;
    }
}
