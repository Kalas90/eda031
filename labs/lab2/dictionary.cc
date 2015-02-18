#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "word.h"
#include "dictionary.h"

using namespace std;

#define IN string("words.txt")


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
        if (word.size() > 1) {
            const vector<Word>& smaller_size_words = words[word.size()-1];
            for (Word smaller_size_word : smaller_size_words) {
                suggestions.push_back(smaller_size_word);
            }
        }

        // add words with size + 1 to suggestions.
        if (word.size() < max_size - 1) {
            const vector<Word>& smaller_size_words = words[word.size() + 1];
            for (Word smaller_size_word : smaller_size_words) {
                suggestions.push_back(smaller_size_word);
            }
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
    // initialize distance matrix
    int d [max_size + 1][max_size + 1];
    for (unsigned int i = 0; i < max_size + 1; i++) {
        d[i][0] = i;
        d[0][i] = i;
    }

    // calculate Levenshtein distance for all suggestions

    vector<pair<int, Word>> ranked_suggestions;
    for (Word suggestion : suggestions) {
        for (unsigned int j = 1; j < suggestion.get_word().size() + 1; j++) {
            for (unsigned int i = 1; i < word.size() + 1; i++) {
                int distance = min(d[i-1][j] + 1, d[i][j-1] + 1);
                if (word[i-1] == suggestion.get_word()[j-1]) {
                    distance = min(distance, d[i-1][j-1]);
                } else {
                    distance = min(distance, d[i-1][j-1] + 1);
                }
                d[i][j] = distance;
            }
        }

        unsigned int cost = d[word.size()][suggestion.get_word().size()];
        ranked_suggestions.push_back(make_pair(cost, suggestion));
    }

    sort(ranked_suggestions.begin(), ranked_suggestions.end());

    vector<Word> sorted_ranked_suggestions;
    for (pair<int, Word> pair : ranked_suggestions) {
        sorted_ranked_suggestions.push_back(pair.second);
    }

    suggestions.swap(sorted_ranked_suggestions);

}


void Dictionary::trim_suggestions(vector<Word>& suggestions) const {
     if (suggestions.size() > 5) {
        suggestions.resize(5);
    }
}

vector<string> Dictionary::get_suggestions(const string& word) const {
	vector<Word> suggestions;
    add_trigram_suggestions(suggestions, word);
    rank_suggestions(suggestions, word);
    trim_suggestions(suggestions);

    vector<string> suggestion_strings;
    for (Word suggestion : suggestions) {
        suggestion_strings.push_back(suggestion.get_word());
    }

	return suggestion_strings;
}
