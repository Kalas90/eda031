#include <ctime>  // time and localtime
#include "date.h"

#include <iomanip> // for setw and setfill
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

using namespace std;

int Date::daysPerMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

Date::Date() {
	time_t timer = time(0); // time in seconds since 1970-01-01
	tm* locTime = localtime(&timer); // broken-down time
	year = 1900 + locTime->tm_year;
	month = 1 + locTime->tm_mon;
	day = locTime->tm_mday;
}

Date::Date(int y, int m, int d): year(y), month(m), day(d) {}

int Date::getYear() const {
	return year;
}

int Date::getMonth() const {
	return month;
}

int Date::getDay() const {
	return day;
}

void Date::next() {
    ++day;
    if (day > daysPerMonth[month-1]) {
        day = 1;
        ++month;
        if (month > 12){
            month = 1;
            ++year;
        }
    }

}

ostream& operator <<(ostream& stream, const Date &d) {
    stream << setw(4) << setfill('0') << d.getYear() << '-'
        << setw(2) << setfill('0') << d.getMonth() << '-'
        << setw(2) << setfill('0') << d.getDay();
    return stream;
}

istream& operator >>(istream& stream, Date &d) {
    string str;
    stream >> str;

    stringstream ss(str);

    int year, month, day;
    char delim1, delim2;

    ss >> year >> delim1 >> month >> delim2 >> day;
    if (delim1 == '-' && delim2 == '-' && month >= 1 && month <= 12 && day == Date::daysPerMonth[month]) {
        d.year = year;
        d.month = month;
        d.day = day;
    } else {
        stream.setstate(ios_base::failbit);
    }

    return stream;
}

template <class t> string toString(t val) {
    stringstream ss;
    ss << val;
    string s;

    if (ss >> s) {
        return s;
    } else {
        throw invalid_argument("Noo");
    }
}

template string toString<double>(double);
template string toString<Date>(Date);

// ohter

template <class t> t toT(string what) {
    stringstream ss;
    ss << what;

    t thet;
    if (ss >> thet)
        return thet;
    else
        throw invalid_argument("Nooo");
}

template double toT<double>(string);
template int toT<int>(string);
template Date toT<Date>(string);
