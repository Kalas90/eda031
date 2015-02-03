#include <iostream>

using namespace std;

bool is_palindrome (const string& s) {
    string::size_type first = 0;
    string::size_type last = s.size() - 1;
    while (first < last && s[first] == s[last]) {
        first++;
        last--;
    }
    return last <= first;
}

int main () {
    cout << "Type a string: ";
    string str;
    while (cin >> str) {
        cout << "The string \"" << str << "\" is ";
        if (!is_palindrome(str)) {
            cout << "not ";
        }
        cout << "a palindrome!" << endl;
        cout << "Type a new string: ";
    }
}