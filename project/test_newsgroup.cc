#include "newsgroup.h"
#include "article.h"
#include "executionresult.h"

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

int main() {
	Newsgroup n1("spegatspexet.kavalkad");

	n1.create_article("Pillesnopp","Anton","Vi satt ett gäng på ...");
	n1.create_article("Champagne","Bjurnbjurn","Jag blir snygg av champagne ...");

  auto success = n1.get_article(1);
	auto missing_article = n1.get_article(3);

  cout << "Expect SUCCESS: ";
  if success.first == ExRes::SUC ?
    cout << "SUCCESS" << endl : cout << "FAIL" << endl;
  cout << "Expect MISSING_ARTICLE: ";
	if missing_article.first == ExRes::MISSING_ARTICLE ?
    cout << "MISSING_ARTICLE" << endl : cout << "FAIL" << endl;

}
