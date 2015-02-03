#include <iostream>

int main () {
    std::cout << "Enter two integers:" << std::endl;
    int n1, n2;
    std::cin >> n1 >> n2;
    if (n1 > n2) {
        while (n1 >= n2) {
            std::cout << n1 << std::endl;
            n1--;
        }
    } else {
        while (n1 <= n2) {
            std::cout << n1 << std::endl;
            n1++;
        }
    }
}