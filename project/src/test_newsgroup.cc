#include "newsgroup.h"
#include "newsgroupprovider.h"
#include <cassert>

using namespace std;

int main() {
  // Create group and article
  Newsgroup g ("Grupp1");
    
  assert(g.get_name() == string("Grupp1"));

  g.create_article("anton", "en saga", "en massa text ... ");
  Article a = g.get_article(1);
  assert(a.get_title() == string("en saga"));
  assert(a.get_author() == string("anton"));
  assert(a.get_text() == string("en massa text ... "));

  // Delete non-existing article
  try {
      g.delete_article(2);
      assert(false);
  } catch (MissingArticleException&) {
      assert(true);
  } catch (...) {
      assert(false);
  }

  // Delete existing article
  try {
      g.delete_article(1);
      assert(true);
  } catch (...) {
      assert(false);
  }
}
