#include <iostream>
#include <iomanip> // for setw and setfill
#include "date.h"
#include <stdexcept>

using namespace std;

/*
 * Prints the date d in the format yyyy-mm-dd. You shall replace this
 * function with an overloaded operator<<, and add an overloaded operator>>.
 *
 */
void print(const Date& d) {
	cout << d;
}

int main() {
	// Check input and output of dates. Uncomment the following when you
	// have added operator>> and operator<<.

/*
	bool cont = true;
	while (cont) {
		cout << "Type a date: ";
		Date aDate;
		cin >> aDate;
		if (cin.eof()) {
			cont = false;
		} else if (!cin.good()) {
			cout << "Wrong input format" << endl;
			// restore stream state and ignore the rest of the line
			cin.clear();
			cin.ignore(10000, '\n');
		}
		else {
			cout << "Output: " << aDate << endl;
		}
	}

	return 0;
	*/


	// Check 'next' by creating an object describing today's date, then
	// printing dates more than a month ahead
	cout << "--- Today and more than a month ahead:" << endl;
	Date d1;
	print(d1);
	cout << endl;
	for (int i = 1; i <= 35 ; ++i) {
		d1.next();
		print(d1);
		cout << endl;
	}

	// Check so 'next' functions correctly from one year to the next
	cout << "--- New Year's Eve and the next day:" << endl;
	Date d2(2013, 12, 31);
	print(d2);
	cout << endl;
	d2.next();
	print(d2);

	cout << endl;

	cout << endl << endl << "TEST TOSTRING" << endl << endl;

	cout << toString<double>(1.123) << endl << toString<Date>(d2) << endl;


	cout << endl << endl << "TEST TYPECAST" << endl << endl;

	try {
		int i = toT<int>("123");
		cout << i << endl;

		double d = toT<double>("12.34");
		cout << d << endl;

		Date date = toT<Date>("sdfasdfasdfs");
		cout << date << endl;

	} catch (invalid_argument& e) {
		cout << "Error: " << e.what() << endl;
	}


}
