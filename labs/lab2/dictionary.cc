#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "word.h"
#include "dictionary.h"

using namespace std;

#define IN string("words.txt")

using namespace std;

Dictionary::Dictionary() {
    ifstream in(IN);

    if (!in)
        cerr << "File " << IN << " not found!" << endl;

    string word;

    while (in >> word) {
        dict.insert(word);
    }
}

bool Dictionary::contains(const string& word) const {
	return dict.find(word) != dict.end();;
}

vector<string> Dictionary::get_suggestions(const string& word) const {
	vector<string> suggestions;
	return suggestions;
}
