#include <iostream>

int main () {
    int limit = 0, sum = 0, value = 0;
    std::cout << "Number of integers in sum: ";
    std::cin >> limit;

    while (std::cin >> value && (--limit != 0))
        sum += value;

    std::cout << "The sum is: " << sum + value << std::endl;
}