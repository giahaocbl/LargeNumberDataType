#include "QInt.h"
#include "File.h"


int main(int argc, char** argv)
{
	string in = argv[1];
	string out = argv[2];
	int type = stoi(argv[3]);

	fstream fin, fout;
	fin.open(in, ios::in);
	fout.open(out, ios::out);

	if (type == 1) execInt(fin, fout);
	else if (type == 2) execFloat(fin, fout);
	else cout << "TYPE ERROR." << endl;

	fin.close(); fout.close();
	return 0;
}
