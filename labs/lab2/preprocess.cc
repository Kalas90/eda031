#include <string>
#include <fstream>
#include <iostream>


#define IN string("words")
#define OUT string("words.txt")

using namespace std;

int main() {

    ifstream in(IN);
    ofstream out(OUT);

    if (!in)
        cerr << "File " << IN << " not found!" << endl;

    if (!out)
        cerr << "File " << OUT << " not found!" << endl;

    string word;
    string letters;

    while(in >> word) {
        letters = "";
        for (unsigned int i = 0; i < word.length(); i++)
            if ((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z'))
                letters.push_back(word[i]);

        string::size_type s = letters.size();
        s = s < 2 ? 0 : s - 2;

        out << word << ' ' << s << ' ';

        for (size_t i = 0; i < s; i++)
            out << letters.substr(i, 3) << ' ';

        out << endl;
    }
}
