#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>
#include <vector>
#include <unordered_set>

#include "word.h"

class Dictionary {
public:
	Dictionary();
	bool contains(const std::string& word) const;
	std::vector<std::string> get_suggestions(const std::string& word) const;
private:
    static const unsigned int max_size = 25;
    static const std::string::size_type number_of_suggestions = 5;
    std::unordered_set<std::string> dict;
    std::vector<Word> words[max_size];
    std::vector<std::string> get_trigrams(const std::string& word) const;
    void add_trigram_suggestions(std::vector<Word>& suggestions, const std::string& word) const;
    void rank_suggestions(std::vector<Word>& suggestions, const std::string& word) const;
    void trim_suggestions(std::vector<Word>& suggestions) const;
};

#endif
