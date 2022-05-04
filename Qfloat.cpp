#include "Qfloat.h"

// Khoi tao QFloat
QFloat::QFloat()
{
	data[0] = data[1] = data[2] = data[3] = 0;
	// khoi tao voi gia tri bang 0
}
bool QFloat::getBit(int index) const { // get bit value at index
	return data[3 - index / 32] >> (index % 32) & 1;
}
void QFloat::setBit(int index, bool value) {
	bool getter = getBit(index);
	if (value) { // Turn on (-->1)
		if (getter) return;  // Da turn on
		else data[3 - index / 32] += pow(2, index % 32);
	}
	else { // Turn off (-->0)
		if (!getter) return; // Da turn off
		else data[3 - index / 32] -= pow(2, index % 32);
	}
}

// Tao toan tu gan cho class QFloat
QFloat QFloat::operator=(const QFloat &a) {
	for (int i = 0; i < 4; i++)
		this->data[i] = a.data[i];
	// vi luu duoi dang 4 so int nen ta so copy lan luot vao this
	return *this;
}

// chuyen doi phan thap phan .5 .25 .125
string QFloat::stringx5(string s)
{
	string res = "";
	int r = 0, t = 0;
	for (int i = s.length() - 1; i >= 0; i--)
	{
		int temp = (s[i] - '0') * 5 + r;
		r = temp / 10;
		t = temp % 10;
		res += t + '0';
	}
	res += r + '0';
	if (res.length() == s.length())
		res += '0';
	reverse(res.begin(), res.end());
	return res;
}

// de tru hai chuoi cho nhau
string QFloat::subString(string s1, string s2)
{
	string res = "";
	int len = s1.length() > s2.length() ? s1.length() : s2.length();
	if (s1.length() > s2.length())
		for (int i = 0; i < len; i++)
			s2 += '0';
	else if (s1.length() < s2.length())
		for (int i = 0; i < len; i++)
			s1 += '0';
	int r = 0;
	for (int i = len - 1; i >= 0; i--)
	{
		if (s1[i] >= s2[i])
		{
			res += s1[i] - s2[i] - r + '0';
			r = 0;
		}
		else
		{
			res += s1[i] - s2[i] + 10 + '0' - r;
			r = 1;
		}
	}
	reverse(res.begin(), res.end());
	return res;
}

// de cong hai chuoi voi nhau
string QFloat::addString(string s1, string s2)
{
	string res = "";
	int len = s1.length() > s2.length() ? s1.length() : s2.length();
	if (s1.length() > s2.length())
		for (int i = 0; i < len; i++)
			s2 += '0';
	else if (s1.length() < s2.length())
		for (int i = 0; i < len; i++)
			s1 += '0';
	int r = 0;
	for (int i = len - 1; i >= 0; i--)
	{
		res += (s1[i] + s2[i] + r - 2 * '0') % 10 + '0';
		r = (s1[i] + s2[i] + r - 2 * '0') / 10;
	}
	reverse(res.begin(), res.end());
	return res;
}

// doc va chuyen ve nhi phan phan so nguyen
void QFloat::ScanFrontPoint(string &number, string &result)
{
	while (number[0] == '0')
	{
		number.erase(number.begin());
	}

	if (number == "")
	{
		reverse(result.begin(), result.end());
		return;
	}

	int r = number[0] - '0';
	number[0] = (r / 2 + '0');
	for (int i = 1; i < number.length(); i++)
	{
		r = r % 2;
		r = r * 10 + number[i] - '0';
		number[i] = (r / 2 + '0');
	}
	r = r % 2;
	result.push_back(r + 48);
	ScanFrontPoint(number, result);
}

// kiem tra xem chuoi do co phai chuoi toan so 0 khong?
bool QFloat::isZeroString(string s)
{
	for (int i = 0; i < s.length(); i++)
		if (s[i] != '0')
			return false;
	return true;
}

// doc va chuyen ve nhi phan phan thap phan
string QFloat::ScanAfterPoint(string s1)
{
	string res = "";
	string t = "5";
	int count = 0, maxcount = 113;

	while (!isZeroString(s1))
	{
		if (compareStrToInt(s1, t) == -1)
			res += '0';
		else
		{
			res += '1';
			s1 = subString(s1, t);
			break;
		}
		t = stringx5(t);
	}
	t = stringx5(t);

	while (!isZeroString(s1) && count < maxcount)
	{
		if (compareStrToInt(s1, t) == -1)
			res += '0';
		else
		{
			res += '1';
			s1 = subString(s1, t);
		}
		t = stringx5(t);
		count++;
	}

	return res;
}

// so sanh 2 so int o dang string
int QFloat::compareStrToInt(string s1, string s2)
{
	int len = s1.length() > s2.length() ? s1.length() : s2.length();
	if (s1.length() > s2.length())
		for (int i = 0; i < len; i++)
			s2 += '0';
	else if (s1.length() < s2.length())
		for (int i = 0; i < len; i++)
			s1 += '0';
	for (int i = 0; i < len; i++)
	{
		if (s1[i] > s2[i])
			return 1;
		else if (s1[i] < s2[i])
			return -1;
	}
	return 0;
}
// chuyen so int sang bias 15bits
string QFloat::bias15bitConverter(int number)
{
	string res = "";
	if (number > 0)
	{
		number = number - 1;
		res += '1';
	}
	else
	{
		number = 16383 + number;
		res += '0';
	}
	string temp = "";
	string numberS = to_string(number);
	ScanFrontPoint(numberS, temp);
	for (int i = 0; i < 14 - temp.length(); i++)
		res += '0';
	res += temp;
	return res;
}

// doc QFloat co the doc he 2 hoac he 10
void QFloat::ScanQFloat(fstream& fin)
{
	data[0] = data[1] = data[2] = data[3] = 0;
	string temp;
	string res = "";
	fin >> temp;

	if (temp[0] == '-')
	{
		res += '1';
		temp.erase(temp.begin());
	}
	else
		res += '0';
	size_t pos = temp.find('.');
	string frontPoint, afterPoint;


	if (pos == -1)
	{
		frontPoint = temp;
		afterPoint = "0";
	}
	else
	{
		frontPoint = temp.substr(0, pos);
		afterPoint = temp.substr(pos + 1);
	}

	if (isZeroString(frontPoint) && isZeroString(afterPoint))
	{
		for (int i = 0; i < 4; i++)
			data[i] = 0;
		return;
	}

	string bitFrontPoint = "";
	ScanFrontPoint(frontPoint, bitFrontPoint);

	string bitAfterPoint = ScanAfterPoint(afterPoint);

	int exponent = 0;

	if (bitFrontPoint.length() > 0)
	{
		exponent = bitFrontPoint.length() - 1;
		bitFrontPoint.erase(bitFrontPoint.begin());
	}
	else
	{
		for (int i = 0; i < bitAfterPoint.length(); i++)
		{
			if (bitAfterPoint[i] == '1')
			{
				exponent = -i - 1;
				break;
			}
		}

		bitAfterPoint = bitAfterPoint.substr(-exponent);
	}

	string bitExponent;
	if (exponent < -16382)
		bitExponent = "000000000000000";
	else
		bitExponent = bias15bitConverter(exponent);

	res += bitExponent + bitFrontPoint + bitAfterPoint;
	while (res.length() < 128)
		res += '0';

	int count = 3, ind = 0;
	for (int i = 127; i >= 0; i--)
	{
		data[count] += (unsigned int)(res[i] - '0') * pow(2, ind);
		ind++;
		if (ind == 32)
		{
			ind = 0;
			count--;
		}
	}
}


// chuyen do 4 so int tu he 10 sang he 2
bool* QFloat::DecToBin()
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

// chuyen do bin sang 4 so int de luu duoi may tinh
QFloat QFloat::BinToDec(bool *bit){
	QFloat res;
	int i = 0;
	while (bit[i] == 0)
		i++;

	int ind = i / 32, count = 32 * (ind + 1) - i - 1;

	while (ind <= 3)
	{
		unsigned int temp = 0;
		while (count >= 0)
		{
			temp += pow(2, count) * bit[i++];
			count--;
		}
		res.data[ind] = temp;
		count = 31;
		ind++;
	}
	return res;
}

// bieu dien so phan nguyen nen boi cua 2
string QFloat::PrintQFloatHelper(string &s, bool c)
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

// de in ra cac so QFloat
void QFloat::PrintQFloat(fstream& fout)
{
	bool *bin = this->DecToBin();
	int check = 0;
	for (int i = 1; i < 16; i++)
	{
		if (bin[i] == 1)
		{
			check = 1;
			break;
		}
	}
	if (check == 0)
	{
		fout << "0.0" << endl;
		return;
	}

	int exponent = 0;
	int ind = 0;
	if (bin[1] == 1)
	{
		for (int i = 15; i >= 2; i--)
		{
			exponent += bin[i] * pow(2, ind);
			ind++;
		}
		exponent++;
	}
	else
	{
		for (int i = 15; i >= 2; i--)
		{
			exponent += bin[i] * pow(2, ind);
			ind++;
		}
		exponent = exponent - 16383;
	}

	string mant = "1";
	string frontPoint = "", afterPoint = "";
	for (int i = 16; i < 128; i++)
		mant += bin[i] + '0';

	if (exponent >= 0)
	{
		if (exponent < mant.length())
		{
			afterPoint = mant.substr(exponent + 1);
			frontPoint = mant.substr(0, exponent + 1);
		}
		else
		{
			afterPoint = "0";
			frontPoint = mant;
			for (int i = 0; i < exponent - 112; i++)
				frontPoint += '0';
		}
	}
	else
	{
		frontPoint = "0";
		for (int i = 0; i < -exponent - 1; i++)
			afterPoint += '0';
		afterPoint += mant;
	}

	string frontPointDec = "0";
	string afterPointDec = "";

	for (int i = 0; i < frontPoint.length(); i++)
	{
		frontPointDec = PrintQFloatHelper(frontPointDec, frontPoint[i] - '0');
	}

	string multiString = "5";
	for (int i = 0; i < afterPoint.length(); i++)
	{
		if (afterPoint[i] == '1')
			afterPointDec = addString(afterPointDec, multiString);
		multiString = stringx5(multiString);
	}

	if (afterPointDec.length() == 0)
		afterPointDec += '0';


	if (bin[0] == 1)
		fout << "-";
	fout << frontPointDec << "." << afterPointDec << endl;
}


// tinh toan
bool QFloat::isZero() const {
	for (int i = 0; i < 127; i++)
		if (getBit(i))
			return false;
	return true;
}
int QFloat::getExponent() {
	if (isZero())
		return 0;
	bool *bit;
	bit = this->DecToBin();
	int res = 0;
	for (int i = 15, j = 0; i > 0; i--, j++) {
		res += bit[i] * pow(2, j);
	}
	res -= 16383;
	delete[] bit;
	return res;
}
void QFloat::setExponent(int value) {
	value += 16383;
	bool* temp = new bool[15];
	for (int i = 0; i < 15; i++)
		temp[i] = (value >> (14 - i)) & 1;
	for (int i = 1; i < 16; i++)
		setBit(127 - i, temp[i - 1]);
	delete[] temp;
}
QInt QFloat::getValue() { // Lay 112 bit chuyen vao QInt de muon ham trong QInt tinh toan
	string temp;
	for (int i = 0; i < 112; i++) {
		if (getBit(i) == 1)
			temp.insert(temp.begin(), '1');
		else
			temp.insert(temp.begin(), '0');
	}
	return QInt::BinToDec(StrToBool(temp));
}
void QFloat::setValue(QInt value) {
	for (int i = 0; i < 112; i++)
		setBit(i, value.getBit(i));
}
QFloat QFloat::operator+(const QFloat &p) { // Tra ve *this + p
	if (p.isZero()) return *this;
	if ((*this).isZero()) return p;

	QFloat res, pCopy(p);
	QInt Q = (*this).getValue();  QInt P = pCopy.getValue();
	if (((*this).getExponent() != -16383)) Q.setBit(112, 1); // So khong chuan
	else Q.setBit(112, 0);
	if ((pCopy.getExponent() != -16383)) P.setBit(112, 1); // So khong chuan
	else P.setBit(112, 0);

	// Xac dinh mu lon
	int max = (*this).getExponent() > pCopy.getExponent() ? (*this).getExponent() : pCopy.getExponent();
	if ((*this).getExponent()  == max)
		P = P >> (*this).getExponent() - pCopy.getExponent();
	else
		Q = Q >> pCopy.getExponent() - (*this).getExponent();
	
	int sign = (*this).getBit(127) - p.getBit(127); // xet dau
	QInt result;
	if (sign == 0) { // cung dau thi cong
		result = Q + P;
		res.setBit(127, p.getBit(127));
	}
	else if (sign == -1) // trai dau thi tru
		result = Q - P;
	else 
		result = P - Q;

	QInt zero;
	if (result.getBit(127)) { // result < 0
		result = zero - result;
		res.setBit(127, 1);
	}

	int find = 127;
	while (result.getBit(find) != 1 && (find >= 0))
		find--;

	if (find == -1) { // Ket qua ra 0
		max = -16383;
	}
	else {
		if (find - 112 >= 0) {
			result = result >> find - 112;
			max += find - 112;
		}
		else {
			result = result << 112 - find;
			max += find - 112;
		}
	}	

	res.setValue(result);
	res.setExponent(max);

	return res;
}
QFloat QFloat::operator-(const QFloat &p) { // Tra ve *this - p
	QFloat pCopy(p);
	pCopy.setBit(127, !pCopy.getBit(127));
	QFloat res = (*this) + pCopy;
	return res;
}
QFloat QFloat::operator*(const QFloat &p) { // Tra ve *this * p
	if (p.isZero()) return p;
	if ((*this).isZero()) return (*this);

	QFloat pCopy(p);
	int sign = (*this).getBit(127) ^ p.getBit(127); // Tinh dau
	int ex = (*this).getExponent() + pCopy.getExponent();

	QInt Q = (*this).getValue();  QInt P = pCopy.getValue();
	if (((*this).getExponent() != -16383)) Q.setBit(112, 1); // So khong chuan
	else Q.setBit(112, 0);
	if ((pCopy.getExponent() != -16383)) P.setBit(112, 1); // So khong chuan
	else P.setBit(112, 0);
	
	// Giai doan nay de dich qua phai va bit dau LUON la 0 (de tinh toan)
	Q = Q >> 1;
	P = P >> 1;
	ex += 2;
	Q.setBit(127, 0); P.setBit(127, 0);
	// Dich binh thuong
	Q = Q >> 50;
	P = P >> 50;
	ex += 100;
	while (Q.getBit(0) == 0) {
		Q = Q >> 1;
		ex++;
	}
	while (P.getBit(0) == 0) {
		P = P >> 1;
		ex++;
	}

	QInt result = Q * P;
	
	int find = 127;
	while (result.getBit(find) != 1 && (find >= 0))
		find--;


	if (find - 112 >= 0) {
		result = result >> find - 112;
		ex += find - 112;
	}
	else {
		result = result << 112 - find;
		ex += find - 112;
	}

	ex -= 112;
	QFloat res;
	res.setExponent(ex);
	res.setValue(result);
	res.setBit(127, sign);
	return res;
}
QFloat QFloat::operator/(const QFloat &p) { // Tra ve *this / p
	if ((*this).isZero()) return (*this);

	QFloat pCopy(p);
	int sign = (*this).getBit(127) ^ p.getBit(127); // Tinh dau
	int ex = (*this).getExponent() - pCopy.getExponent();

	QInt Q = (*this).getValue();  QInt P = pCopy.getValue();
	if (((*this).getExponent() != -16383)) Q.setBit(112, 1); // So khong chuan
	else Q.setBit(112, 0);
	if ((pCopy.getExponent() != -16383)) P.setBit(112, 1); // So khong chuan
	else P.setBit(112, 0);

	// Giai doan nay de dich qua phai va bit dau LUON la 0 (de tinh toan)
	P = P >> 1;
	ex -= 1;
	P.setBit(127, 0);
	// Dich binh thuong
	P = P >> 50;
	ex -= 50;
	while (P.getBit(0) == 0) {
		P = P >> 1;
		ex--;
	}

	QInt result = Q / P;

	int find = 127;
	while (result.getBit(find) != 1 && (find >= 0))
		find--;
	
	if (find - 112 >= 0) {
			result = result >> find - 112;
			ex += find - 112;
	}
	else {
			result = result << 112 - find;
			ex += find - 112;
	}
	ex += 112;
	QFloat res;
	res.setValue(result);
	res.setExponent(ex);
	res.setBit(127, sign);
	return res;
}