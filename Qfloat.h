#pragma once
#include <iostream>
#include <string>
#include "QInt.h"

using namespace std;

class QFloat
{
private:
	unsigned int data[4];

	bool getBit(int index) const; // 127 <-- 0
	void setBit(int index, bool value);	// Thay doi mot bit
	bool isZero() const; // Co phai la so 0
	int getExponent(); // Lay so mu
	void setExponent(int value); // Gan so mu
	QInt getValue(); // Lay phan gia tri
	void setValue(QInt value); // Gan phan gia tri
	static string stringx5(string s); // chuyen doi phan thap phan .5 .25 .125
	int compareStrToInt(string s1, string s2); // so sanh 2 so int o dang string
	bool isZeroString(string); // kiem tra xem chuoi do co phai chuoi toan so 0 khong?
	static string addString(string s1, string s2); // de tru hai chuoi cho nhau
	static string subString(string s1, string s2); // de cong hai chuoi voi nhau
	void ScanFrontPoint(string &, string &); // doc va chuyen ve nhi phan phan so nguyen
	string ScanAfterPoint(string);// doc va chuyen ve nhi phan phan thap phan
	string bias15bitConverter(int); // chuyen so int sang bias 15bits
	string PrintQFloatHelper(string &, bool); // bieu dien so phan nguyen nen boi cua 2

public:
	QFloat();  // Khoi tao QFloat
	QFloat operator=(const QFloat &); // Tao toan tu gan cho class QFloat
	void ScanQFloat(fstream&); // doc QFloat co the doc he 2 hoac he 10
	void PrintQFloat(fstream&); // de in ra cac so QFloat

	bool* DecToBin(); // chuyen do 4 so int tu he 10 sang he 2
	static QFloat BinToDec(bool*); // chuyen do bin sang 4 so int de luu duoi may tinh

	// Operator
	QFloat operator+(const QFloat &p); // Tra ve *this + p
	QFloat operator-(const QFloat &p); // Tra ve *this - p
	QFloat operator*(const QFloat &p); // Tra ve *this * p
	QFloat operator/(const QFloat &p); // Tra ve *this / p
};