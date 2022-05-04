#pragma once
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class QInt {
private:
	unsigned int data[4];

	int powerOf2(int pow); // 2 ^ pow
	void ScanQIntHelper(string &, string &); // support scan methods
	string PrintQIntHelper(string &, bool c); // support print methods
public:
	QInt();
	QInt(int);
	bool getBit(int index) const; // 127 <-- 0
	void setBit(int index, bool value); // set value for a bit
	// Print and scan
	void ScanQInt(fstream&); // scan (decimal) by fstream
	void PrintQInt4(fstream&); // print data[0], data[1], data[2], data[3]
	void PrintQInt(fstream&); // print (decimal)
	// Convert
	static QInt BinToDec(bool *); // convert binary (bool array) to decimal (QInt)
	static string BinToHex(bool *); // convert binary to hex (string)
	static bool* HexToBin(string); // convert hex tto binary
	static QInt HexToDec(string); // convert hex to decimal
	string DecToHex() const; // convert decima to hex
	bool* DecToBin() const; // convert decimal to binary
	// Operator
	QInt operator+(const QInt &p); // return *this + p
	QInt operator-(const QInt &p); // return *this - p
	QInt operator*(const QInt &p); // return *this * p
	QInt operator/(const QInt &p); // return *this / p
	QInt operator= (const QInt &a); // *this <-- a
	QInt operator& (const QInt &a); // return *this AND a
	QInt operator|(const QInt &a); // return *this OR a
	QInt operator~(); // return NOT *this
	QInt operator^(const QInt &a); // return *this XOR a
	QInt operator>>(const int &a); // bit right shift
	QInt operator<<(const int &a); // bit left shift
	QInt rol(const int &a); // rotate left
	QInt ror(const int &a); // rotate right
	// Compare
	bool operator>(const QInt &a); // Is *this > a?
	bool operator<(const QInt &a); // Is *this < a?
	bool operator==(const QInt &a); // Is *this = a?
	bool operator>=(const QInt &a); // Is *this > or = a?
	bool operator<=(const QInt &a); // Is *this < or = a?
};
// support
bool* StrToBool(string); // convert string to bool* with 128 elements
string BoolToStr(bool*); // convert bool* with 128 elements to string