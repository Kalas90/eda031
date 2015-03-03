#include "article.h"
#include "newsgroup.h"

#include <vector>
#include <string>

// Interface class, pure virtual
class NewGroupProvider {
public:
    virtual Article article(int newsgroup_id, int article_id) const = 0;
    virtual std::vector<NewsGroup> list_news_groups() const = 0;
    virtual std::vector<Article> list_articles(int newsgroup_id) const = 0;
    virtual bool remove_article(int newsgroup_id, int article_id) = 0;
    virtual bool remove_newsgroup(int newsgroup_id) = 0;
    virtual bool create_newsgroup(std::String name) = 0;
    virtual bool create_article(int newsgroup_id,
            std::String title, std::String author, std::String text) = 0;
};
