#include "Date.hpp"

Date::Date() {
	m_year = 0;
	m_month = 0;
	m_day = 0;
	m_hour = 0;
	m_minute = 0;
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
	m_year = t_year;
	m_month = t_month;
	m_day = t_day;
	m_hour = t_hour;
	m_minute = t_minute;
}

Date::Date(std::string dateString) {
	Date t_date = stringToDate(dateString);
	m_year = t_date.m_year;
	m_month = t_date.m_month;
	m_day = t_date.m_day;
	m_hour = t_date.m_hour;
	m_minute = t_date.m_minute;
}

int Date::getYear(void) const {
	return m_year;
}

void Date::setYear(const int t_year) {
	m_year = t_year;
}

int Date::getMonth(void) const {
	return m_month;
}

void Date::setMonth(const int t_month) {
	m_month = t_month;
}

int Date::getDay(void) const {
	return m_day;
}

void Date::setDay(const int t_day) {
	m_day = t_day;
}

int Date::getHour(void) const {
	return m_hour;
}

void Date::setHour(const int t_hour) {
	m_hour = t_hour;
}

int Date::getMinute(void) const {
	return m_minute;
}

void Date::setMinute(const int t_minute) {
	m_minute = t_minute;
}

bool Date::isValid(const Date t_date) {
	if (t_date.m_year < 10000 && t_date.m_year >= 1000 && t_date.m_month > 0 && t_date.m_month <= 12) {
	} else {
		return false;
	}
	if (t_date.m_year % 4 != 0 || (t_date.m_year % 100 == 0 && t_date.m_year % 400 != 0)) {
		int day_num1[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
		if (t_date.m_day <= day_num1[t_date.m_month - 1] && t_date.m_day > 0) {
		} else 
			return false;
	} else {
		int day_num2[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
		if (t_date.m_day <= day_num2[t_date.m_month - 1] && t_date.m_day > 0) {
		} else 
			return false;
	}
	if (t_date.m_hour >= 0 && t_date.m_hour <= 23 && t_date.m_minute >= 0 && t_date.m_minute <= 59) {
		return true;
	} else 
		return false;
}

Date Date::stringToDate(const std::string t_dateString) {
	Date t_date;
	Date wrong_date(0, 0, 0, 0, 0);
	if (t_dateString[0] < '0' || t_dateString[0] > '9') {
		return wrong_date;
	}
	if (t_dateString[1] < '0' || t_dateString[1] > '9') {
		return wrong_date;
	}
	if (t_dateString[2] < '0' || t_dateString[2] > '9') {
		return wrong_date;
	}
	if (t_dateString[3] < '0' || t_dateString[3] > '9') {
		return wrong_date;
	}
	if (t_dateString[4] != '-') {
		return wrong_date;
	}
	if (t_dateString[5] < '0' || t_dateString[5] > '9') {
		return wrong_date;
	}
	if (t_dateString[6] < '0' || t_dateString[6] > '9') {
		return wrong_date;
	}
	if (t_dateString[7] != '-') {
		return wrong_date;
	}
	if (t_dateString[8] < '0' || t_dateString[8] > '9') {
		return wrong_date;
	}
	if (t_dateString[9] < '0' || t_dateString[9] > '9') {
		return wrong_date;
	}
	if (t_dateString[10] != '/') {
		return wrong_date;
	}
	if (t_dateString[11] < '0' || t_dateString[11] > '9') {
		return wrong_date;
	}
	if (t_dateString[12] < '0' || t_dateString[12] > '9') {
		return wrong_date;
	}
	if (t_dateString[13] != ':') {
		return wrong_date;
	}
	if (t_dateString[14] < '0' || t_dateString[14] > '9') {
		return wrong_date;
	}
	if (t_dateString[15] < '0' || t_dateString[15] > '9') {
		return wrong_date;
	}
	t_date.m_year = (t_dateString[0]-'0')*1000+(t_dateString[1]-'0')*100+(t_dateString[2]-'0')*10+t_dateString[3]-'0';
	t_date.m_month = (t_dateString[5]-'0')*10+t_dateString[6]-'0';
	t_date.m_day = (t_dateString[8]-'0')*10+t_dateString[9]-'0';
	t_date.m_hour = (t_dateString[11]-'0')*10+t_dateString[12]-'0';
	t_date.m_minute = (t_dateString[14]-'0')*10+t_dateString[15]-'0';
	return t_date;
}

std::string Date::dateToString(Date t_date) {
	if (!isValid(t_date)) {
		return "0000-00-00/00:00";
	}
	std::string t_dateString = "0000-00-00/00:00";
	t_dateString[0] = t_date.m_year / 1000 + '0';
	t_date.m_year = t_date.m_year % 1000;
	t_dateString[1] = t_date.m_year / 100 + '0';
	t_date.m_year = t_date.m_year % 100;
	t_dateString[2] = t_date.m_year / 10 + '0';
	t_date.m_year = t_date.m_year % 10;
	t_dateString[3] = t_date.m_year + '0';
	t_dateString[4] = '-';
	t_dateString[5] = t_date.m_month / 10 + '0';
	t_date.m_month = t_date.m_month % 10;
	t_dateString[6] = t_date.m_month + '0';
	t_dateString[7] = '-';
	t_dateString[8] = t_date.m_day / 10 + '0';
	t_date.m_day = t_date.m_day % 10;
	t_dateString[9] = t_date.m_day + '0';
	t_dateString[10] = '/';
	t_dateString[11] = t_date.m_hour / 10 + '0';
	t_date.m_hour = t_date.m_hour % 10;
	t_dateString[12] = t_date.m_hour + '0';
	t_dateString[13] = ':';
	t_dateString[14] = t_date.m_minute / 10 + '0';
	t_date.m_minute = t_date.m_minute % 10;
	t_dateString[15] = t_date.m_minute + '0';
	return t_dateString;
}

Date& Date::operator=(const Date &t_date) {
	(*this).m_year = t_date.m_year;
	(*this).m_month = t_date.m_month;
	(*this).m_day = t_date.m_day;
	(*this).m_hour = t_date.m_hour;
	(*this).m_minute = t_date.m_minute;
	return *this;
}

bool Date::operator==(const Date &t_date) const {
	if ((*this).m_year == t_date.m_year && (*this).m_month == t_date.m_month 
		&& (*this).m_day == t_date.m_day && (*this).m_hour == t_date.m_hour
		&& (*this).m_minute == t_date.m_minute) {
		return true;
	} else {
		return false;
	}
}

bool Date::operator>(const Date &t_date) const {
	if (this->m_year > t_date.m_year) {
		return true;
	} else if ((*this).m_year == t_date.m_year) {
		if ((*this).m_month > t_date.m_month) {
			return true;
		} else if ((*this).m_month == t_date.m_month) {
			if ((*this).m_day > t_date.m_day) {
				return true;
			} else if ((*this).m_day == t_date.m_day) {
				if ((*this).m_hour > t_date.m_hour) {
					return true;
				} else if ((*this).m_hour == t_date.m_hour) {
					if ((*this).m_minute > t_date.m_minute) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool Date::operator<(const Date &t_date) const {
	return !((*this) > t_date || (*this) == t_date);
}

bool Date::operator>=(const Date &t_date) const {
	return !((*this) < t_date);
}

bool Date::operator<=(const Date &t_date) const {
	return !((*this) > t_date);
}
