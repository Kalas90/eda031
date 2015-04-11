#include "memorynewsgroupprovider.h"
#include "newsgroupprovider.h"
#include "newsgroup.h"
#include "article.h"

#include <string>
#include <vector>
#include <iostream>
#include <cassert>

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

    // list_news_groups
	print_mngp(mngp);

    // remove_article non-existing article
    try {
        mngp.remove_article(1,100);
        assert(false);
    } catch (MissingArticleException&) {
        assert(true);
    } catch (...) {
        assert(false);
    }

    // remove_article non-existing newsgroup
    try {
        mngp.remove_article(100,1);
        assert(false);
    } catch (MissingNewsgroupException&) {
        assert(true);
    } catch (...) {
        assert(false);
    }

    // remove_article
    try {
        mngp.remove_article(1,1);
        assert(mngp.list_articles(1).size() == 3);
    } catch (MissingNewsgroupException&) {
        cout << "Missing newsgroup" << endl;
        assert(false);
    } catch (MissingArticleException&) {
        cout << "Missing article" << endl;
        assert(false);
    }
    
    // remove_newsgroup non-existing newsgroup
    try {
        mngp.remove_newsgroup(100);
        assert(false);
    } catch (MissingNewsgroupException&) {
        assert(true);
    } catch (...) {
        assert(false);
    }

    // remove_newsgroup
    try {
        mngp.remove_newsgroup(2);
        assert(mngp.list_news_groups().size() == 1);
    } catch (MissingNewsgroupException&) {
        cout << "Missing newsgroup" << endl;
        assert(false);
    }

    // create_article in non-existing newsgroup
    try {
        mngp.create_article(100, "Title", "Author", "Content text...");
        assert(false);
    } catch (MissingNewsgroupException&) {
        assert(true);
    } catch (...) {
        assert(false);
    }
}
