#include "memorynewsgroupprovider.h"
#include "newsgroup.h"
#include "article.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;

void print_articles(vector<Article>& alist, string ind) {
	for (auto a : alist) {
		cout << ind << "id: " << a.get_id() << endl;
		cout << ind << "author: " << a.get_author() << endl;
		cout << ind << "title: " << a.get_title() << endl;
		cout << ind << "text: " << a.get_text() << endl << endl;	
	}
}

void print_newsgroup(Newsgroup& ng) {
	cout << "Newsgroup id: " << ng.get_id() << endl;
	cout << "Newsgroup name: " << ng.get_name() << endl;

	vector<Article> alist = ng.get_articles();
	if (alist.size() == 0)
		cout << "  Group is empty..." << endl;
	else
		print_articles(alist, "  ");
} 

void print_mngp(MemoryNewsgroupProvider& mngp) {
	vector<Newsgroup> list = mngp.list_news_groups();

    for (Newsgroup g : list) {
    	print_newsgroup(g);
    }	
}

int main() {
    MemoryNewsgroupProvider mngp;

    mngp.create_newsgroup("grupp1");
    mngp.create_newsgroup("grupp2");

    auto list = mngp.list_news_groups();

    for (auto g : list) {
    	cout << g.get_id() << endl;
    }

    mngp.create_article(1, "article1", "Brange", "Det var en gång...");
	mngp.create_article(1, "article2", "Anton", "Det var en annan gång...");
	mngp.create_article(1, "article3", "Linus", "Det var en tredje gång...");
	mngp.create_article(1, "article4", "Johan", "Håll käften.");

	print_mngp(mngp);

	cout << "Remove article1 in group1" << endl << endl;
	mngp.remove_article(1,1);

	print_mngp(mngp);

	mngp.remove_newsgroup(2);

	print_mngp(mngp);
}
