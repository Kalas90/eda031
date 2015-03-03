#include "article.h"
#include "newsgroup.h"
#include <iostream>

using namespace std;

int main() {

    Newsgroup ng1 = Newsgroup("namn");
    ng1.create_article("author1", "title1", "text1");
    ng1.create_article("author2", "title2", "text2");
    ng1.create_article("author3", "title3", "text3");
    ng1.create_article("author4", "title4", "text4");
    ng1.list_articles(cout);

    ng1.delete_article(1);
    ng1.delete_article(3);
    ng1.list_articles(cout);

}