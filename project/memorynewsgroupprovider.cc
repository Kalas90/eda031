#include "memorynewsgroupprovider.h"
#include "newsgroup.h"
#include "article.h"

#include <string>
#include <vector>
#include <algorithm>

using mngp = MemoryNewsgroupProvider;

Newsgroup& mngp::newsgroup(unsigned int newsgroup_id) {
    auto it = std::find_if(news.begin(), news.end(),
            [newsgroup_id](Newsgroup g) {return g.get_id() == newsgroup_id;}
            );
    if (it == news.end())
        throw std::bad_function_call();
    else
        return *it;
}

Article mngp::article(unsigned int newsgroup_id, unsigned int article_id) const {
    auto it = std::find_if(news.begin(), news.end(),
            [newsgroup_id](Newsgroup g) {return g.get_id() == newsgroup_id;}
            );

    if (it == news.end())
        return Article("", "", "");
    
    return Article("","","");
    //return (*it).get_article(article_id); // Not yet implemented
}

std::vector<Newsgroup> mngp::list_news_groups() const {
    return std::vector<Newsgroup>();
}

std::vector<Article> mngp::list_articles(unsigned int newsgroup_id) const {
    return std::vector<Article>();
}

bool mngp::remove_article(unsigned int newsgroup_id, unsigned int article_id) {
    return false;
}

bool mngp::remove_newsgroup(unsigned int newsgroup_id) {
    return false;
}

bool mngp::create_newsgroup(std::string name) {
    news.push_back(Newsgroup(name));
    return false;
}

bool mngp::create_article(unsigned int newsgroup_id,
            std::string title, std::string author, std::string text) {
    newsgroup(newsgroup_id).create_article(author, title, text);
    return true;
}

