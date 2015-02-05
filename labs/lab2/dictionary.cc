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
    string word;

    if (in) {
        while (in >> word) {
            int nTrigrams = 0;
            in >> nTrigrams;
            vector<string> trigrams;

            for (int i = 0; i < nTrigrams; i++) {
                string trigram;
                in >> trigram;
                trigrams.push_back(trigram);
            }

            sort(trigrams.begin(), trigrams.end());

            if (word.size() < max_size) {
                dict.insert(word);
                words[word.size()].push_back(Word(word, trigrams));
            }

        }

    } else {
        cerr << "File " << IN << " not found!" << endl;
    }

}

bool Dictionary::contains(const string& word) const {
	return dict.find(word) != dict.end();
}

void Dictionary::add_trigram_suggestions(vector<Word>& suggestions, const string& word) const {
    if (word.size() < max_size) {
        // add words with same size to suggestions.
        const vector<Word>& same_size_words = words[word.size()];
        for (Word same_size_word : same_size_words) {
            suggestions.push_back(same_size_word);
        }

        // add words with size - 1 to suggestions.
        const vector<Word>& smaller_size_words = words[word.size()-1];
        for (Word smaller_size_word : smaller_size_words) {
            suggestions.push_back(smaller_size_word);
        }

        // remove words with bad trigram matches.
        vector<string> trigrams = get_trigrams(word);
        vector<Word> filtered_suggestions;

        for (Word suggestion : suggestions) {
            if (suggestion.get_matches(trigrams)*2 >= trigrams.size()) {

                filtered_suggestions.push_back(suggestion);
            }
        }

        suggestions.swap(filtered_suggestions);

    }
}

vector<string> Dictionary::get_trigrams(const string& word) const {
    vector<string> trigrams;
    for (string::size_type i = 0; i < word.size() - 2; i++) {
        string trigram = word.substr(i, 3);
        trigrams.push_back(trigram);
    }

    sort(trigrams.begin(), trigrams.end());
    return trigrams;

}

void Dictionary::rank_suggestions(vector<Word>& suggestions, const string& word) const {

}

vector<string> Dictionary::get_suggestions(const string& word) const {
	vector<Word> suggestions;
    add_trigram_suggestions(suggestions, word);


    vector<string> suggestion_strings;
    for (Word suggestion : suggestions) {
        suggestion_strings.push_back(suggestion.get_word());
    }

	return suggestion_strings;
}
