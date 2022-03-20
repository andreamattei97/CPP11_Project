// original author: Bernt A Oedegaard
// modified by: Andrea Mattei
#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <string>

using namespace std;

class Date
{
protected:
	int year_;
	int month_;
	int day_;
public:
	Date();
	Date(const int d,const int m,const int y);

	static Date Today();

	bool valid() const;
	string toString() const;
	
	int day() const;
	int month() const;
	int year() const;

	void set_day(const int day);
	void set_month(const int month);
	void set_year(const int year);

};

bool operator == (const Date&, const Date&);
bool operator != (const Date&, const Date&);
bool operator < (const Date&, const Date&);
bool operator > (const Date&, const Date&);
bool operator <= (const Date&, const Date&);
bool operator >= (const Date&, const Date&);

ostream& operator << (ostream& os, const Date& d);

#endif
