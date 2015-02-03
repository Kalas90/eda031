#include <iostream>

int main () {
    std::cout << "Enter two numbers: " << std::endl;
    int v0 = 0, v1 = 0;
    std::cin >> v0 >> v1;
    std::cout << "The product of " << v0 << " and " << v1 << " is " << v0 * v1
        << std::endl;

}