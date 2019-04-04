#ifndef BigInt_H
#define BigInt_H

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include "bigint.h"


ostream& operator<<(ostream& os, const BigInt& num)
{
	if (num.sign) 
	{
		os << "-";
	}
	os << num.number.c_str();
	return os;
}

BigInt::BigInt()
{
	number = "0";
	sign = false;
}

BigInt::BigInt(const string& s)
{
	if (isdigit(s[0]))
	{
		setNumber(s);
		sign = false;
	}
	else
	{
		setNumber(s.substr(1));
		sign = (s[0] == '-');
	}
}

BigInt BigInt::absolute() const
{
	return getNumber();
}

BigInt::BigInt(const string& s, const bool sin)
{
	setNumber(s);
	setSign(sin);
}

BigInt::BigInt (const int64_t n)
{
	stringstream ss;
	string s;
	ss << n;
	ss >> s;


	if (isdigit(s[0]))
	{
		setNumber (s);
		setSign (false);
	}
	else
	{
		setNumber (s.substr(1));
		setSign (s[0] == '-');
	}
}

void BigInt::setNumber(const string& s)
{
	number = s;
}

const string& BigInt::getNumber()const
{
	return number;
}

void BigInt::setSign(bool s)
{
	sign = s;
}

const bool& BigInt::getSign() const
{
	return sign;
}

BigInt& BigInt::operator = (const BigInt& b)
{
	setNumber(b.getNumber());
	setSign(b.getSign());
	return *this;
}

BigInt& BigInt::operator = (const int64_t b)
{
	std::ostringstream o;
	o << b;
	setNumber(o.str());
	setSign(b < 0);
	return *this;
}

BigInt& BigInt::operator = (const int b)
{
	setNumber(to_string(b));
	setSign(b < 0);
	return *this;
}

bool BigInt::operator == (const BigInt& b) const
{
	return equals((*this), b);
}

bool BigInt::operator != (const BigInt& b) const
{
	return !equals((*this), b);
}

bool BigInt::operator > (const BigInt& b) const 
{
	return greater((*this), b);
}

bool BigInt::operator < (const BigInt& b) const 
{
	return less((*this), b);
}

bool BigInt::operator >= (const BigInt& b) const 
{
	return equals((*this), b)
		|| greater((*this), b);
}

bool BigInt::operator <= (const BigInt& b) const 
{
	return equals((*this), b)
		|| less((*this), b);
}

BigInt BigInt::operator + (const BigInt& b) const
{
	BigInt addition;
	if (getSign() == b.getSign())
	{
		addition.setNumber (add(getNumber(), b.getNumber()));
		addition.setSign (getSign());
	}
	else
	{
		if (absolute() > b.absolute())
		{
			addition.setNumber (subtract(getNumber(), b.getNumber()));
			addition.setSign(getSign());
		}
		else
		{
			addition.setNumber (subtract(b.getNumber(), getNumber()));
			addition.setSign (b.getSign());
		}
	}
	if (addition.getNumber() == "0")
		addition.setSign(false);

	return addition;
}

BigInt BigInt::operator - (const BigInt& b) const
{
	return (*this) + (b*-1);
}

BigInt& BigInt::operator += (const BigInt& b)
{
	(*this) = (*this) + b;
	return (*this);
}

BigInt& BigInt::operator -= (const BigInt& b)
{
	(*this) = (*this) - b;
	return (*this);
}
BigInt BigInt::operator -()
{
	return (*this) * -1;
}

BigInt BigInt::operator * (const BigInt& b) const 
{
	BigInt mul;

	mul.setNumber (multiply(getNumber(), b.getNumber()));
	mul.setSign(getSign() != b.getSign());

	if (mul.getNumber() == "0")
		mul.setSign(false);

	return mul;
}

BigInt::operator string()
{
	string signedString = (getSign()) ? "-" : "";
	signedString += number;
	return signedString;
}

bool BigInt::equals (const BigInt& n1, const BigInt& n2) const 
{
	return n1.getNumber() == n2.getNumber()
		&& n1.getSign() == n2.getSign();
}

bool BigInt::less (const BigInt& n1, const BigInt& n2) const 
{
	bool sign1 = n1.getSign();
	bool sign2 = n2.getSign();

	if (sign1 && !sign2)
		return true;

	else if (!sign1 && sign2)
		return false;

	else if (!sign1)
	{
		if (n1.getNumber().length() < n2.getNumber().length())
			return true;
		if (n1.getNumber().length() > n2.getNumber().length())
			return false;
		return n1.getNumber() < n2.getNumber();
	}
	else
	{
		if (n1.getNumber().length() > n2.getNumber().length())
			return true;
		if (n1.getNumber().length() < n2.getNumber().length())
			return false;
		return n1.getNumber().compare(n2.getNumber()) > 0;
	}
}

bool BigInt::greater (const BigInt& n1, const BigInt& n2) const 
{
	return !equals (n1, n2) && !less (n1, n2);
}

string BigInt::add (string number1, string number2) const 
{
	string add = (number1.length() > number2.length()) ? number1 : number2;
	char carry = '0';
	int differenceInLength = abs((int)(number1.size() - number2.size()));

	if (number1.size() > number2.size())
		number2.insert(0, differenceInLength, '0');

	else
		number1.insert(0, differenceInLength, '0');

	for (int i = number1.size() - 1; i >= 0; --i)
	{
		add[i] = ((carry - '0') + (number1[i] - '0') + (number2[i] - '0')) + '0';

		if (i != 0)
		{
			if (add[i] > '9')
			{
				add[i] -= 10;
				carry = '1';
			}
			else
				carry = '0';
		}
	}
	if (add[0] > '9')
	{
		add[0] -= 10;
		add.insert(0, 1, '1');
	}
	return add;
}

string BigInt::subtract (string number1, string number2) const 
{
	string sub = (number1.length()>number2.length()) ? number1 : number2;
	int differenceInLength = abs ((int)(number1.size() - number2.size()));

	if (number1.size() > number2.size())
		number2.insert(0, differenceInLength, '0');

	else
		number1.insert(0, differenceInLength, '0');

	for (int i = number1.length() - 1; i >= 0; --i)
	{
		if (number1[i] < number2[i])
		{
			number1[i] += 10;
			number1[i - 1]--;
		}
		sub[i] = ((number1[i] - '0') - (number2[i] - '0')) + '0';
	}

	while (sub[0] == '0' && sub.length() != 1)
		sub.erase(0, 1);

	return sub;
}

string BigInt::multiply (string n1, string n2) const 
{
	if (n1.length() > n2.length())
		n1.swap(n2);

	string res = "0";
	for (int i = n1.length() - 1; i >= 0; --i)
	{
		string temp = n2;
		int currentDigit = n1[i] - '0';
		int carry = 0;

		for (int j = temp.length() - 1; j >= 0; --j)
		{
			temp[j] = ((temp[j] - '0') * currentDigit) + carry;

			if (temp[j] > 9)
			{
				carry = (temp[j] / 10);
				temp[j] -= (carry * 10);
			}
			else
				carry = 0;

			temp[j] += '0';
		}

		if (carry > 0)
			temp.insert(0, 1, (carry + '0'));

		temp.append((n1.length() - i - 1), '0');

		res = add (res, temp);
	}

	while (res[0] == '0' && res.length() != 1)
		res.erase(0, 1);

	return res;
}

#endif
