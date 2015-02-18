#ifndef DATE_H
#define DATE_H

#include <iostream>

using namespace std;

class Date {
public:
	Date();                    // today's date
	Date(int y, int m, int d); // yyyy-mm-dd
	int getYear() const;       // get the year
	int getMonth() const;      // get the month
	int getDay() const;        // get the day
	void next();               // advance to next day

	friend ostream& operator<<(ostream& stream, const Date &d);
	friend istream& operator>>(istream& stream, Date &d);

private:
	int year;  // the year (four digits)
	int month; // the month (1-12)
	int day;   // the day (1-..)
	static int daysPerMonth[12]; // number of days in each month
};

template <class t> string toString(t val);
template <class t> t toT(string);


#endif
