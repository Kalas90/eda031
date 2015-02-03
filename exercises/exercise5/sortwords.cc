#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	vector<string> v = {"Mary", "had", "a", "little", "lamb", "and", "some",
		"olives", "on", "the", "side"};
	
	// print the words: Mary had a little lamb and some olives on the side
	//cout << v << endl;
	
	// sort in alphabetical order: Mary a and had lamb little olives on side some the

	//cout << v << endl;
	
	// in reverse order: the some side on olives little lamb had and a Mary

	//cout << v << endl;
	
	// by ascending length: a on the and had Mary lamb side some olives little

	//cout << v << endl;
	
	// sort in alphabetical order again, print three-letter words: and had the

	//cout << v << endl;
	
	// remove words with <= three letters: Mary lamb little olives side some

	//cout << v << endl;
}