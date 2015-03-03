#include "memorynewsgroupprovider.h"
#include "newsgroup.h"
#include "article.h"

#include <string>
#include <vector>

using mngp = MemoryNewsGroupProvider

virtual Article mngp::article(int newsgroup_id, int article_id) const override {
    return Article(0, "", "", "");
}

virtual std::vector<NewsGroup> mngp::list_news_groups() const override {
    return std::vector<NewsGroup>;
}

virtual std::vector<Article> mngp::list_articles(int newsgroup_id) const override {
    return std::vector<Article>
}

virtual bool mngp::remove_article(int newsgroup_id, int article_id) override{
    return false;
}

virtual bool mngp::remove_newsgroup(int newsgroup_id) override {
    return false;
}

virtual bool mngp::create_newsgroup(std::String name) override {
    return false;
}

virtual bool mngp::create_article(int newsgroup_id,
            std::String title, std::String author, std::String text) override {
    return false;
}

