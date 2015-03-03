#include <iostream>

using namespace std;

string sieve(int);

int main() {
    string small = sieve(200);

    cout << small << endl;

    string big = sieve(1000000);

    cout << "Biggest: " << big.find_last_of('P') << endl;

}

string sieve(int m) {
    string str(m, 'P');
    str[0] = 'C';
    str[1] = 'C';

    int k;
    for (int i = 2; i < m; i++) {
        if (str[i] == 'P') {
            k = i+i;
            while (k < m) {
                str[k] = 'C';
                k += i;
            }
        }
    }
    return str;
}