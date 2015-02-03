#include <iostream>

int main () {
    std::cout << "Enter two numbers: " << std::endl;
    int v0 = 0, v1 = 0;
    std::cin >> v0;
    std::cin >> v1;
    std::cout << "The product of ";
    std::cout << v0;
    std::cout <<" and ";
    std::cout << v1;
    std::cout << " is ";
    std::cout << v0 * v1;
    std::cout << std::endl;
}