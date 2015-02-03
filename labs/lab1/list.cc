#include <iostream>
#include "list.h"

using namespace std;

/**
 * Constructor creating an empty list.
 */

List::List() {
    first = nullptr;
}

/**
 * Deconstructing the list by iterating through the nodes.
 */

List::~List() {
    Node* n = first;
    while (n != nullptr) {
        Node* tmp = n;
        n = n->next;
        delete tmp;
    }
}

/**
 * Returns true if the list contains d.
 *
 * @param  d The integer to search for.
 * @return   True if integer d is part of the list.
 */
bool List::exists(int d) const {
    Node* n = first;
    bool result = false;
    while (n != nullptr) {
        if (n->value == d) {
            result = true;
            break;
        }
        n = n->next;
    }
	return result;
}


/**
 * Returns the number of nodes in the list.
 * @return number of list nodes.
 */
int List::size() const {
	int size = 0;
    Node* n = first;
    while (n != nullptr) {
        size++;
        n = n->next;
    }
    return size;
}

/**
 * Returns true if the list has no nodes.
 * @return True if the list is empty.
 */
bool List::empty() const {
	return first == nullptr;
}

/**
 * Inserts a new node first in the list.
 * @param d The integer to add to the list.
 */
void List::insertFirst(int d) {
    Node* n = new Node(d, first);
    first = n;
}

void List::remove(int d, DeleteFlag df) {
    Node* n = first;
    Node* prev = nullptr;

    if (df == DeleteFlag::LESS) {
        while (n != nullptr) {
            if (n->value < d) {
                break;
            }
            prev = n;
            n = n->next;
        }
    } else if (df == DeleteFlag::EQUAL) {
        while (n != nullptr) {
            if (n->value == d) {
                break;
            }
            prev = n;
            n = n->next;
        }
    } else if (df == DeleteFlag::GREATER) {
        while (n != nullptr) {
            if (n->value > d) {
                break;
            }
            prev = n;
            n = n->next;
        }
    } else {
        cout << "Unrecognized DeleteFlag in List::remove" << endl;
        exit(1);
    }

    // If there is no node to delete n will have value nullptr.
    if (n != nullptr) {
        if (prev != nullptr) { // if the node found is not the first element
            prev->next = n->next;
        } else { // if the node found is the first element.
            first = n->next;
        }
        delete n;
    }

}

/**
 * Prints the list
 */
void List::print() const {
    Node* n = first;
    cout << "[";
    while (n != nullptr) {
        cout << n->value;
        if (n->next != nullptr) {
            cout << ", ";
        }
        n = n->next;
    }
    cout << "]" << endl;
}

