#include "File.h"

bool isConvert(string request) { // is a convert request. Ex: decimal to binary
	if ((int)request.find('+') + 1 || (int)request.find(" - ") + 1 || (int)request.find('*') + 1 || (int)request.find('/') + 1
		|| (int)request.find('>') + 1 || (int)request.find('<') + 1 || (int)request.find('=') + 1
		|| (int)request.find('&') + 1 || (int)request.find('|') + 1 || (int)request.find('^') + 1 || (int)request.find('~') + 1
		|| (int)request.find('r') + 1)
		return false;
	else return true;
}
void outputQInt(fstream& fout, QInt res, int base) {
	if (base == 2)
		fout << BoolToStr(res.DecToBin()) << endl;
	else if (base == 10) {
		res.PrintQInt(fout);
		fout << endl;
	}
	else
		fout << res.DecToHex() << endl;
}
void outputQFloat(fstream& fout, QFloat res, int base) {
	if (base == 2) {
		bool* bins = res.DecToBin();
		for (int i = 0; i < 128; i++)
			fout << bins[i];
		fout << endl;
		delete[] bins;
	}
	else {
		res.PrintQFloat(fout);
	}
}
void execInt(fstream& fin, fstream& fout) {
	int base1, base2; string c;
	string tempIn; streampos saver;
	while (!fin.eof()) {
		saver = fin.tellg();
		getline(fin, tempIn); // scan to check isConvert
		fin.seekg(saver, ios::beg);
		if (tempIn.length() < 2) break; // for safe

		// #################### 1
		if (isConvert(tempIn)) { // Convert

			fin >> base1 >> base2;

			if (base1 == 10) {
				QInt in; in.ScanQInt(fin);
				outputQInt(fout, in, base2);
			}
			else if (base1 == 2) {
				string in; fin >> in;
				QInt out = QInt::BinToDec(StrToBool(in));
				outputQInt(fout, out, base2);
			}
			else { // base1 == 16
				string in; fin >> in;
				QInt out = QInt::HexToDec(in);
				outputQInt(fout, out, base2);
			}
		}
		// #################### 2
		else { // calculation
			fin >> base1;
			if ((int)tempIn.find('~') + 1) { // 1 parameter
				fin >> c;
				if (base1 == 2) {
					string in; fin >> in;
					outputQInt(fout, ~QInt::BinToDec(StrToBool(in)), 2);
				}
				else if (base1 == 10) {
					QInt in; in.ScanQInt(fin);
					outputQInt(fout, ~in, 10);
				}
				else {
					string in; fin >> in;
					outputQInt(fout, ~QInt::HexToDec(in), 16);
				}
			}
			// #################### 3
			else if ((int)tempIn.find("ror") + 1 || (int)tempIn.find("rol") + 1
				|| (int)tempIn.find("<<") + 1 || (int)tempIn.find(">>") + 1) { // 2 parameters: QInt & int
				QInt a; int b;
				// input
				if (base1 == 2) {
					string in1;
					fin >> in1 >> c >> b;
					a = QInt::BinToDec(StrToBool(in1));
				}
				else if (base1 == 10) {
					a.ScanQInt(fin);
					fin >> c >> b;
				}
				else {
					string in1;
					fin >> in1 >> c >> b;
					a = QInt::HexToDec(in1);
				}
				// result
				if (c == "ror") outputQInt(fout, a.ror(b), base1);
				else if (c == "rol") outputQInt(fout, a.rol(b), base1);
				else if (c == ">>") outputQInt(fout, a >> b, base1);
				else if (c == "<<") outputQInt(fout, a << b, base1);
			}
			// #################### 4
			else { // 2 parameters: QInt & QInt
				QInt a, b, res;
				// input
				if (base1 == 2) {
					string in1, in2;
					fin >> in1 >> c >> in2;
					a = QInt::BinToDec(StrToBool(in1));
					b = QInt::BinToDec(StrToBool(in2));
				}
				else if (base1 == 10) {
					a.ScanQInt(fin);
					fin >> c;
					b.ScanQInt(fin);
				}
				else {
					string in1, in2;
					fin >> in1 >> c >> in2;
					a = QInt::HexToDec(in1);
					b = QInt::HexToDec(in2);
				}
				// Result
				if (c == "+") outputQInt(fout, a + b, base1);
				else if (c == "-") outputQInt(fout, a - b, base1);
				else if (c == "*") outputQInt(fout, a * b, base1);
				else if (c == "/") outputQInt(fout, a / b, base1);
				else if (c == "&") outputQInt(fout, a & b, base1);
				else if (c == "|") outputQInt(fout, a | b, base1);
				else if (c == "^") outputQInt(fout, a ^ b, base1);
				else if (c == ">") fout << (a > b) << endl;
				else if (c == "<") fout << (a < b) << endl;
				else if (c == ">=") fout << (a >= b) << endl;
				else if (c == "<=") fout << (a <= b) << endl;
				else if (c == "==") fout << (a == b) << endl;
			}
		}
		getline(fin, tempIn); // scan buffer
	}
}
void execFloat(fstream& fin, fstream& fout) {
	int base; string c;
	string tempIn; streampos saver;

	while (!fin.eof()) {
		saver = fin.tellg();
		std::getline(fin, tempIn); // scan to check isConvert
		fin.seekg(saver, ios::beg);
		if (tempIn.length() < 2) break; // for safe

		fin >> base;
		// #################### 1
		if (isConvert(tempIn)) { // Convert
			QFloat in;
			if (base == 2) {
				string temp;
				fin >> base >> temp;
				in = QFloat::BinToDec(StrToBool(temp));
			}
			else { // base == 10
				fin >> base;
				in.ScanQFloat(fin);
			}
			outputQFloat(fout, in, base);
		}
		// #################### 2
		else { // calculation
			string c;
			QFloat a, b, res;
			// input
			if (base == 2) {
				string in1, in2;
				fin >> in1 >> c >> in2;
				a = QFloat::BinToDec(StrToBool(in1));
				b = QFloat::BinToDec(StrToBool(in2));
			}
			else {// base == 10
				a.ScanQFloat(fin);
				fin >> c;
				b.ScanQFloat(fin);
			}
			// Result
			if (c == "+") res = a + b;
			if (c == "-") res = a - b;
			if (c == "*") res = a * b;
			if (c == "/") res = a / b;
			outputQFloat(fout, res, base);
		}

		std::getline(fin, tempIn); // scan buffer
	}
}