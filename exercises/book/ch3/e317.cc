#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main () {
    vector<string> vec;
    string s;

    while (cin >> s)
        vec.push_back(s);

    for (auto& w : vec)
        for (auto& ch : w)
            ch = toupper(ch);

    for (vector<string>::size_type i = 0; i < vec.size(); i++) {
        if (i%8 == 0)
            cout << endl;
        cout << vec[i] << " ";
    }

    cout << endl;

    return 0;

}