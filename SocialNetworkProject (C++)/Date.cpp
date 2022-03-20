// original author: Bernt A Oedegaard
// modified by: Andrea Mattei
#include "Date.hpp"

#include <ctime>

Date::Date()
{
	day_ = 0;
	month_ = 0;
	year_ = 0;
}

Date::Date(const int d, const int m, const int y) 
{
	day_ = d;
	month_ = m;
	year_ = y;
}

Date Date::Today()
{
	Date date;
	time_t currentTime = time(0);
	tm currentTimeStruct;
	localtime_s(&currentTimeStruct, &currentTime);

	date = Date(currentTimeStruct.tm_mday, 1 + currentTimeStruct.tm_mon, 1900 + currentTimeStruct.tm_year);
	return date;
}

int Date::day() const { return day_; }
int Date::month() const { return month_; }
int Date::year() const { return year_; }

void Date::set_day(const int day) { day_ = day; }
void Date::set_month(const int month) { month_ = month; }
void Date::set_year(const int year) { year_ = year; }

bool Date::valid() const 
{
	if (year_ < 0) return false;
	if (month_ > 12 || month_ < 1) return false;
	if (day_ > 31 || day_ < 1) return false;
	if ((day_ == 31 && (month_ == 2 || month_ == 4 || month_ == 6 || month_ == 9 || month_ == 11))) return false;
	if (day_ == 30 && month_ == 2) return false;
	if (year_ < 2000) 
	{
		if ((day_ == 29 && month_ == 2) && !((year_ - 1900) % 4 == 0))return false;
	}
	if (year_ > 2000) 
	{
		if ((day_ == 29 && month_ == 2) && !((year_ - 2000) % 4 == 0)) return false;
	}
	return true;
}

string Date::toString() const
{
	return to_string(day_) +"/" + to_string(month_) + "/" + to_string(year_);
}

bool operator==(const Date& d1, const Date& d2) 
{
	if (!d1.valid()) { return false; }
	if (!d2.valid()) { return false; }
	if ((d1.day() == d2.day()) && (d1.month() == d2.month()) && (d1.year() == d2.year())) return true;
	return false;
}

bool operator!=(const Date& d1, const Date& d2) 
{
	return !(d1 == d2);
}

bool operator <(const Date& d1, const Date& d2) 
{
	if (!d1.valid()) { return false; }
	if (!d2.valid()) { return false; }
	if (d1.year() < d2.year()) { return true; }
	else if (d1.year() > d2.year()) { return false; }
	else
	{
		if (d1.month() < d2.month()) { return true; }
		else if (d1.month() > d2.month()) { return false; }
		else 
		{
			if (d1.day() < d2.day()) { return true; }
			else { return false; }
		}
	}
	return false;
}

bool operator > (const Date& d1, const Date& d2) 
{
	if (d1 == d2) { return false; }
	if (d1 < d2) { return false; }
	return true;
}

bool operator <= (const Date& d1, const Date& d2) 
{
	if (d1 == d2) { return true; }
	return (d1 < d2);
}

bool operator >=(const Date& d1, const Date& d2) 
{
	if (d1 == d2) { return true; }
	return (d1 > d2);
}

ostream& operator << (ostream& os, const Date& d) 
{
	if (d.valid()) 
	{
		os << d.day() << "/" << d.month()<< "/" << d.year();
	}
	else 
	{
		os << "Data non valida";
	}
	return os;
}