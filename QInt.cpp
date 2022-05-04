#include "QInt.h"

QInt::QInt()
{
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
}
QInt::QInt(int a) {
	data[0] = a;
	data[1] = data[2] = data[3] = 0;
}
// Support
int QInt::powerOf2(int pow) { // return 2 ^ row
	return (int)pow(2, pow);
}
bool QInt::getBit(int index) const { // get bit value at index
	return data[3 - index / 32] >> (index % 32) & 1;
}
void QInt::setBit(int index, bool value) { // set bit at index to value
	bool getter = getBit(index);
	if (value) { // Turn on (-->1)
		if (getter) return;  // Da turn on
		else data[3 - index / 32] += powerOf2(index % 32);
	}
	else { // Turn off (-->0)
		if (!getter) return; // Da turn off
		else data[3 - index / 32] -= powerOf2(index % 32);
	}
}
void moveLeft(bool *A, bool *Q, bool &Q1) { // use in booth algorithm
	Q1 = Q[127];
	for (int i = 127; i >= 1; i--)
		Q[i] = Q[i - 1];
	Q[0] = A[127];
	for (int i = 127; i >= 1; i--)
		A[i] = A[i - 1];
}
void formBool(bool *&p, int n) { // n => 128 (101 => 00000......101)
	bool* q = new bool[n];
	for (int i = 0; i < n; i++)
		q[i] = p[i];
	delete[] p;
	p = new bool[128];
	for (int i = 0; i < 128 - n; i++)
		p[i] = 0;
	for (int i = 128 - n; i < 128; i++)
		p[i] = q[i - 128 + n];
}
bool* StrToBool(string str) {
	bool *rs = new bool[str.length()];
	for (int i = 0; i < str.length(); i++)
		rs[i] = str[i] == '0' ? 0 : 1;
	formBool(rs, str.length());
	return rs;
}
string BoolToStr(bool* p) {
	string rs;
	int runner = 0;
	while (!p[runner])
		runner++;
	while (runner <= 127)
		rs.push_back(p[runner++] + '0');
	return rs;
}

void QInt::ScanQIntHelper(string &number, string &result)
{
	while (number[0] == '0')
	{
		number.erase(number.begin());
	}

	if (number == "")
	{
		int len = result.length();
		for (int i = 0; i < 127 - len; i++)
			result += '0';

		reverse(result.begin(), result.end());
		if (result.length() > 127)
		{
			result = "N/A";
			cout << endl << "ERROR" << endl;
		}
		return;
	}

	string temp = "";
	int r = number[0] - '0';
	temp.push_back(r / 2 + '0');
	for (int i = 1; i < number.length(); i++)
	{
		r = r % 2;
		r = r * 10 + number[i] - '0';
		temp.push_back(r / 2 + '0');
	}
	r = r % 2;
	result.push_back(r + 48);
	ScanQIntHelper(temp, result);
}
void QInt::ScanQInt(fstream& fin)
{
	data[0] = data[1] = data[2] = data[3] = 0;
	string number;
	string result = "";
	bool sign = 0;

	fin >> number;
	if (number[0] == '-')
	{
		sign = 1;
		number.erase(number.begin());
	}

	ScanQIntHelper(number, result);

	//Convert to two's complement
	if (sign == 1)
	{
		if (result[result.length() - 1] == '1')
		{
			for (int i = result.length() - 2; i >= 0; i--)
				result[i] = '1' - result[i] + '0';
		}
		else
		{
			bool rem = 1;
			for (int i = result.length() - 2; i >= 0; i--)
			{
				result[i] = '1' - result[i] + '0' + rem;
				if (result[i] == '2')
				{
					result[i] = '0';
					rem = 1;
				}
				else
					rem = 0;
			}
		}

	}

	if (result == "N/A")
		return;


	int count = 3, ind = 0;
	for (int i = result.length() - 1; i >= 0; i--)
	{
		data[count] += (unsigned int)(result[i] - '0') * powerOf2(ind);
		ind++;
		if (ind == 32)
		{
			ind = 0;
			count--;
		}
	}
	data[0] += sign * powerOf2(31);
}
string QInt::PrintQIntHelper(string &s, bool c)
{
	int r = c;
	string temp = "";
	for (int i = s.length() - 1; i >= 0; i--)
	{
		int dig = (s[i] - '0') * 2 + r;
		temp.push_back(dig % 10 + '0');
		r = dig / 10;
	}
	if (r == 1)
		temp.push_back('1');
	reverse(temp.begin(), temp.end());
	return temp;
}
void QInt::PrintQInt(fstream& fout)
{
	bool* bin = this->DecToBin();
	bool sign = 0;

	if (bin[0] == 1)
	{
		sign = 1;
		if (bin[127] == 1)
		{
			for (int i = 126; i >= 0; i--)
				bin[i] = 1 - bin[i];
		}
		else
		{
			bool rem = 1;
			for (int i = 126; i >= 0; i--)
			{
				int bit = 1 - bin[i] + rem;
				if (bit == 2)
				{
					bin[i] = 0;
					rem = 1;
				}
				else
				{
					bin[i] = bit;
					rem = 0;
				}
			}
		}

	}


	string s = "";
	s += '0';
	int pos = 0;
	for (int i = 1; i < 128; i++)
		if (bin[i] == 1)
		{
			pos = i;
			break;
		}
	for (int i = pos; i < 128; i++)
	{
		s = PrintQIntHelper(s, bin[i]);
	}
	if (sign == 1)
		fout << "-";
	fout << s;
}
void QInt::PrintQInt4(fstream& fout)
{
	for (int i = 0; i < 4; i++)
		fout << data[i] << " ";
}

bool* QInt::DecToBin() const
{
	bool *bin = new bool[128];
	int ind = 127;
	for (int i = 3; i >= 0; i--)
	{
		unsigned int number = data[i];
		for (int j = 0; j < 32; j++)
		{
			bin[ind] = number % 2;
			number /= 2;
			ind--;
		}
	}
	return bin;
}
QInt QInt::BinToDec(bool *bin)
{
	QInt res;
	int i = 0;
	while (bin[i] == 0)
		i++;

	int ind = i / 32, count = 32 * (ind + 1) - i - 1;

	while (ind <= 3)
	{
		unsigned int temp = 0;
		while (count >= 0)
		{
			temp += pow(2, count)*bin[i++];
			count--;
		}
		res.data[ind] = temp;
		count = 31;
		ind++;
	}
	return res;
}
string QInt::BinToHex(bool *bin)
{
	string res = "";
	int i = 0;
	while (bin[i] == 0)
		i++;

	int ind = (127 - i + 1) / 4, count = (127 - i + 1) % 4 - 1;
	while (ind >= 0)
	{
		int temp = 0;
		while (count >= 0)
		{
			temp += pow(2, count)*bin[i++];
			count--;
		}
		switch (temp)
		{
		case 10: res += 'A'; break;
		case 11: res += 'B'; break;
		case 12: res += 'C'; break;
		case 13: res += 'D'; break;
		case 14: res += 'E'; break;
		case 15: res += 'F'; break;
		default:
			res += temp + '0';
			break;
		}
		count = 3;
		ind--;
	}
	while (res[0] == '0')
		res.erase(res.begin());
	return res;
}
string QInt::DecToHex() const 
{
	bool *bin = this->DecToBin();
	return QInt::BinToHex(bin);
}
bool* QInt::HexToBin(string hexStr) {
	string bits;
	for (int i = 0; i < hexStr.length(); i++) {
		switch (hexStr[i])
		{
		case '0': bits += "0000"; break;
		case '1': bits += "0001"; break;
		case '2': bits += "0010"; break;
		case '3': bits += "0011"; break;
		case '4': bits += "0100"; break;
		case '5': bits += "0101"; break;
		case '6': bits += "0110"; break;
		case '7': bits += "0111"; break;
		case '8': bits += "1000"; break;
		case '9': bits += "1001"; break;
		case 'A': bits += "1010"; break;
		case 'B': bits += "1011"; break;
		case 'C': bits += "1100"; break;
		case 'D': bits += "1101"; break;
		case 'E': bits += "1110"; break;
		case 'F': bits += "1111"; break;
		}
	}
	bool *a = new bool[bits.length()];
	for (int i = 0; i < bits.length(); i++)
		a[i] = bits[i] == '0' ? 0 : 1;
	formBool(a, bits.length());
	return a;
}
QInt QInt::HexToDec(string hexStr) {
	bool* temp = QInt::HexToBin(hexStr);
	QInt res = QInt::BinToDec(temp);
	return res;
}

QInt QInt::operator+(const QInt &p) {
	QInt result;
	int remember = 0, temp = 0;
	for (int i = 0; i < 128; i++) {
		temp += getBit(i) + p.getBit(i) + remember;
		result.setBit(i, temp % 2);
		remember = temp > 1;
		temp = 0;
	}
	return result;
}
QInt QInt::operator-(const QInt &p) {
	QInt result;
	int remember = 0, temp = 0;
	for (int i = 0; i < 128; i++) {
		temp += getBit(i);
		temp -= p.getBit(i) + remember;
		remember = 0; // het nho
		if (temp < 0) { // can muon
			temp += 2;
			remember = 1;
		}
		result.setBit(i, temp);
		temp = 0;
	}
	return result;
}
QInt QInt::operator*(const QInt &p) {
	QInt zero;
	zero = zero - p;
	bool *M = p.DecToBin();
	bool *negaM = zero.DecToBin(); // negaM = -M
	bool *A = new bool[128];
	for (int i = 0; i < 128; i++)
		A[i] = 0;
	bool *Q = DecToBin();
	bool Q1 = 0;
	// Booth
	for (int count = 1; count <= 128; count++) {
		if ((Q[127] && Q1) || (!Q[127] && !Q1)) // 11 or 00
			moveLeft(A, Q, Q1);
		else if (Q[127] && !Q1) { // 10
			QInt *tempNegaM = new QInt(QInt::BinToDec(negaM));
			QInt *tempA = new QInt(QInt::BinToDec(A) + *tempNegaM); // A + (-M)
			A = tempA->DecToBin();
			moveLeft(A, Q, Q1);
			delete tempA, tempNegaM;
		}
		else { // 01
			QInt *tempM = new QInt(QInt::BinToDec(M));
			QInt *tempA = new QInt(QInt::BinToDec(A) + *tempM); // A + M
			A = tempA->DecToBin();
			moveLeft(A, Q, Q1);
			delete tempA, tempM;
		}
	}
	
	QInt rs(QInt::BinToDec(Q));

	return rs;
}
QInt QInt::operator/(const QInt &p) {
	string result;
	QInt zero, one; one.data[3] = 1;
	QInt Q(*this), P(p); // Q / P
	bool sign = 0;
	if (P.getBit(127)) { // negative
		sign = !sign;
		P = zero - P; // result = 0
	}
	if (Q.getBit(127)) {
		sign = !sign;
		Q = zero - Q;
	}

	int lengP = 128;
	for (; lengP > 0; lengP--)
		if (p.getBit(lengP - 1))
			break;
	int QSweeper = 127;
	bool* Qarray = new bool[lengP + 1];
	Qarray[0] = 0;
	while (!Q.getBit(QSweeper))
		QSweeper--;

	for (int count = 1; count <= lengP; count++)
		Qarray[count] = Q.getBit(QSweeper--);
	formBool(Qarray, lengP + 1);
	QInt* Qtemp = new QInt(QInt::BinToDec(Qarray));
	QInt adder;
	while (QSweeper >= -1)
	{
		adder = getBit(QSweeper) ? one : zero;

		if (*Qtemp >= P) {
			result.push_back('1');
			*Qtemp = *Qtemp - P;
			*Qtemp = (*Qtemp << 1) + adder;
		}
		else {
			result.push_back('0');
			*Qtemp = (*Qtemp << 1) + adder;
		}
		QSweeper--;
	}
	bool *a = new bool[result.length()];
	for (int i = 0; i < result.length(); i++)
		a[i] = result[i] == '0' ? 0 : 1;
	formBool(a, result.length());
	QInt rs; rs = QInt::QInt::BinToDec(a);	
	delete[] Qarray, a; delete Qtemp;
	if (sign) return zero - rs;
	else return rs;
}

QInt QInt::operator=(const QInt & a) {
	for (int i = 0; i < 4; i++)
		data[i] = a.data[i];
	return *this;
}
QInt QInt::operator&(const QInt & a) {
	for (int i = 0; i < 4; i++)
		data[i] = data[i] & a.data[i];
	return *this;
}
QInt QInt::operator|(const QInt & a) {
	for (int i = 0; i < 4; i++)
		data[i] = data[i] | a.data[i];
	return *this;
}
QInt QInt::operator~() {
	for (int i = 0; i < 4; i++)
		data[i] = ~data[i];
	return *this;
}
QInt QInt::operator^(const QInt & a) {
	for (int i = 0; i < 4; i++)
		data[i] = data[i] ^ a.data[i];
	return *this;
}
QInt QInt::operator>>(const int & a) {
	bool *x = this->DecToBin();
	bool sign = x[0];
	int i = 127;
	while (i - a > 0) {
		x[i] = x[i - a];
		i--;
	}
	for (i = 0; i <= a; i++)
		x[i] = sign;
	QInt res = this->BinToDec(x);
	delete x;
	return res;
}
QInt QInt::operator<<(const int & a) {
	bool *x = this->DecToBin();
	int i = 0;
	while (i + a < 128) {
		x[i] = x[i + a];
		i++;
	}
	for (i = 128 - a; i < 128; i++)
		x[i] = 0;
	QInt res = this->BinToDec(x);
	delete x;
	return res;
}
QInt QInt::rol(const int & a) {
	bool *x = this->DecToBin();
	bool *temp = new bool[a];
	int j = 0;
	for (int i = a - 1; i >= 0; i--)
		temp[j++] = x[i];
	int i = 0;
	while (i + a < 128) {
		x[i] = x[i + a];
		i++;
	}
	j = 0;
	for (i = 128 - a; i < 128; i++)
		x[i] = temp[j++];
	QInt res = this->BinToDec(x);
	delete x;
	delete temp;
	return res;
}
QInt QInt::ror(const int & a) {
	bool *x = this->DecToBin();
	bool *temp = new bool[a];
	int j = 0;
	for (int i = 127; i > 127 - a; i--)
		temp[j++] = x[i];

	int i = 127;
	while (i - a > 0) {
		x[i] = x[i - a];
		i--;
	}
	for (i = 0; i < a; i++)
		x[i] = temp[i];
	QInt res = this->BinToDec(x);
	delete x;
	delete temp;
	return res;
}
bool QInt::operator>(const QInt & a) {
	int sign1 = this->data[0] >> 31;
	int sign2 = a.data[0] >> 31;
	if (sign1 == sign2) {
		for (int i = 0; i < 4; i++) {
			if (data[i] > a.data[i]) return (sign1 == 0);
			if (data[i] < a.data[i]) return (sign1 == 1);
		}
		return 0;
	}
	return (sign1 < sign2);
}
bool QInt::operator<(const QInt & a) {
	int sign1 = this->data[0] >> 31;
	int sign2 = a.data[0] >> 31;
	if (sign1 == sign2) {
		for (int i = 0; i < 4; i++) {
			if (data[i] < a.data[i]) return (sign1 == 0);
			if (data[i] > a.data[i]) return (sign1 == 1);
		}
		return 0;
	}
	return (sign1 > sign2);
}
bool QInt::operator==(const QInt & a) {
	if (!(*this > a) && !(*this < a)) return true;
	return false;
}
bool QInt::operator>=(const QInt & a) {
	if (*this > a || *this == a) return true;
	return false;
}
bool QInt::operator<=(const QInt & a) {
	if (*this < a || *this == a) return true;
	return false;
}	