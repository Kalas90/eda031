#include <iostream>
#include <vector>
#include <string>



using namespace std;

int main () {
    vector<int> vec;
    int i;

    while (cin >> i)
        vec.push_back(i);

    for (decltype(vec.size()) i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";
    }

    cout << endl;

    for (decltype(vec.size()) i = 0; i < vec.size() - 1; i++) {
        cout << vec[i] + vec[i + 1] << " ";
    }

    cout << endl;

    for (decltype(vec.size()) i = 0; i < vec.size() - 1; i++) {
        cout << vec[i] + vec[vec.size() - 1 - i] << " ";
    }

    cout << endl;

    return 0;

}