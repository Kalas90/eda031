#include "newsgroupprovider.h"

class MemoryNewsGroupProvider : public NewsGroupProvider {
public:
    virtual Article article(int newsgroup_id, int article_id) const override;
    virtual std::vector<NewsGroup> list_news_groups() const override;
    virtual std::vector<Article> list_articles(int newsgroup_id) const override;
    virtual bool remove_article(int newsgroup_id, int article_id) override;
    virtual bool remove_newsgroup(int newsgroup_id) override;
    virtual bool create_newsgroup(std::String name) override;
    virtual bool create_article(int newsgroup_id,
            std::String title, std::String author, std::String text) override;
private:
    std::vector<NewsGroup> news;
};
