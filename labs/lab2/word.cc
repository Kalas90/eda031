#include <string>
#include <vector>
#include "word.h"

using namespace std;

Word::Word(const string& w, const vector<string>& t): word(w), trigrams(t) {}

string Word::get_word() const {
	return word;
}

unsigned int Word::get_matches(const vector<string>& t) const {
	unsigned int nMatches = 0;
    unsigned int t_counter = 0;
    unsigned int tri_counter = 0;

    while (trigrams.size() > tri_counter && t.size() > t_counter) {
        if (trigrams[tri_counter] == t[t_counter]) {
            nMatches++;
            t_counter++;
            tri_counter++;
        }  else if (trigrams[tri_counter] > t[t_counter]){
            t_counter++;
        } else {
            tri_counter++;
        }
    }

    return nMatches;
}
