#include "memorynewsgroupprovider.h"
#include "newsgroup.h"
#include "article.h"

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

using mngp = MemoryNewsgroupProvider;

Newsgroup& mngp::newsgroup(unsigned int newsgroup_id) {
    auto it = std::find_if(news.begin(), news.end(),
            [newsgroup_id](Newsgroup& g) {return g.get_id() == newsgroup_id;}
            );
    if (it == news.end())
        throw MissingNewsgroupException(
                std::string("No newsgroup with id = ") +
                std::to_string(newsgroup_id));
    else
        return *it;
}

Article mngp::article(unsigned int newsgroup_id, unsigned int article_id) const {
    auto it = std::find_if(news.begin(), news.end(),
            [newsgroup_id](const Newsgroup& g) {return g.get_id() == newsgroup_id;}
            );
    if (it == news.end())
        throw MissingNewsgroupException(
                std::string("No newsgroup with id = ") +
                std::to_string(newsgroup_id));
    else
        return (*it).get_article(article_id);
}

std::vector<Newsgroup> mngp::list_news_groups() const {
    return news;
}

std::vector<Article> mngp::list_articles(unsigned int newsgroup_id) {
    return newsgroup(newsgroup_id).get_articles();
}

bool mngp::remove_article(unsigned int newsgroup_id, unsigned int article_id) {
    return newsgroup(newsgroup_id).delete_article(article_id);
}

bool mngp::remove_newsgroup(unsigned int newsgroup_id) {
    auto it = std::remove_if(news.begin(), news.end(),
        [newsgroup_id](Newsgroup g) {return g.get_id() == newsgroup_id;}
        );
    if (it != news.end()) {
        news.erase(it);
        return true;
    } else {
        throw MissingNewsgroupException();
    }
}

bool mngp::create_newsgroup(std::string name) {
    auto it = std::find_if(news.begin(), news.end(),
        [name](Newsgroup& g) { return g.get_name() == name; });
    if (it == news.end()) {
        news.push_back(Newsgroup(name));
        return true;
    } else
        throw DuplicateNewsgroupException();
}

bool mngp::create_article(unsigned int newsgroup_id,
            std::string title, std::string author, std::string text, int id) {
    return newsgroup(newsgroup_id).create_article(author, title, text, id);
}

