#include <iostream>
#include "src/Sales_item.h"

int main () {
    Sales_item item1, item2;
    std::cin >> item1 >> item2;

    std::cout << item1.isbn() << item2.isbn() << std::endl;

    if (item1.isbn() == item2.isbn()) {
        std::cout << item1 + item2 << std::endl;
        return 0;
    } else {
        std::cerr << "Input books must have same isbn" << std::endl;
        return -1;
    }
}