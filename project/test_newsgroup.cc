#include "newsgroup.h"

using namespace std;

int main() {
  Newsgroup g ("Grupp1");
  cout << g.get_id() << endl;
  cout << g.get_name() << endl;

  g.create_article("anton", "en saga", "en massa text ... ");
  Article a = g.get_article(1);
  cout << a.get_id() << ", " << a.get_title() << ", " << a.get_author() << ", " << a.get_text() << endl;
}
